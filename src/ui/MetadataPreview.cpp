#include "ui/MetadataPreview.h"

MetdataPreview::MetdataPreview()
    : m_textWidget(nullptr)
{ }

void
MetdataPreview::setupUis(QPlainTextEdit* textWidget, QLabel* compressed, QLabel* decompressed, QLabel* modified)
{
    m_textWidget = textWidget;
    m_labelCompressed = compressed;
    m_labelDecompressed = decompressed;
    m_labelModified = modified;
}

void
MetdataPreview::setData(LotusLib::PackagesReader* pkgs, const std::string& pkgName, const LotusLib::LotusPath& internalPath)
{
    pkgs->initilizePackagesBin();
    LotusLib::FileEntry fileEntry = pkgs->getPackage(pkgName).value().getFile(internalPath, LotusLib::READ_COMMON_HEADER);

    std::stringstream outStr;
    setupCommonHeader(outStr, pkgs->getGame(), fileEntry);
    addPackagesBinHeirarchy(outStr, pkgs->getPackage(pkgName).value(), internalPath);
    std::string outStrTmp = outStr.str();
    m_textWidget->setPlainText(QString(outStrTmp.c_str()));

    auto pkg = pkgs->getPackage(pkgName).value();
    setFiledata(pkg, fileEntry);
}

void
MetdataPreview::clearPreview()
{
    m_textWidget->clear();
    m_labelModified->clear();
    m_labelCompressed->clear();
    m_labelDecompressed->clear();
}

void
MetdataPreview::setupCommonHeader(std::stringstream& outStr, LotusLib::Game game, LotusLib::FileEntry& fileEntry)
{
    outStr << "Type Enum: ";
    outStr << fileEntry.commonHeader.type << " ";
    auto extractor = WarframeExporter::g_enumMapExtractor.at(game, LotusLib::findPackageCategory(fileEntry.srcPkgName), fileEntry.commonHeader.type);
    if (extractor != nullptr)
        outStr << "(" << extractor->getFriendlyName() << ")";
    else
        outStr << "(Unknown)";
    outStr << std::endl << std::endl;

    outStr << "Attributes" << std::endl;
    outStr << "----------" << std::endl;
    outStr << fileEntry.commonHeader.attributes;
    outStr << std::endl;

    outStr << "File Paths" << std::endl;
    outStr << "----------" << std::endl;
    for (const std::string& curPath : fileEntry.commonHeader.paths)
        outStr << " " << curPath << std::endl;
}

void
MetdataPreview::setFiledata(LotusLib::PackageReader& pkgs, LotusLib::FileEntry& fileEntry)
{
    m_labelModified->setText(timestampToQString(fileEntry.metadata->getTimeStamp()));
    int totalCompressed = fileEntry.metadata->getCompLen();
    int totalDecompressed = fileEntry.metadata->getLen();

    try
    {
        const LotusLib::FileEntries::FileNode* bNode = pkgs.getFileNode(fileEntry.internalPath, LotusLib::PackageTrioType::B);
        if (bNode != nullptr)
        {
            totalCompressed += bNode->getCompLen();
            totalDecompressed += bNode->getLen();
        }
    }
    catch (LotusLib::InternalEntryNotFound& ex) {}

    try
    {
        const LotusLib::FileEntries::FileNode* fNode = pkgs.getFileNode(fileEntry.internalPath, LotusLib::PackageTrioType::F);
        if (fNode != nullptr)
        {
            totalCompressed += fNode->getCompLen();
            totalDecompressed += fNode->getLen();
        }
    }
    catch (LotusLib::InternalEntryNotFound& ex) {}

    m_labelCompressed->setText(filesizeToQString(totalCompressed));
    m_labelDecompressed->setText(filesizeToQString(totalDecompressed));
}

QString
MetdataPreview::timestampToQString(int64_t input)
{
    time_t epochTime = input / 10000000UL - 11644473600UL;
	const tm* timeInfo = gmtime(&std::max((time_t)0, epochTime));

    char s[16];
    strftime(s, 16, "%b %d, %Y", timeInfo);
    return QString(s);
}

QString
MetdataPreview::filesizeToQString(int input)
{
    static std::array<std::string, 5> prefixes {"B", "KB", "MB", "GB", "TB"};

    int prefixIndex = 0;
    float sizedDown = input;

    while (sizedDown > 1024 && prefixIndex < 4)
    {
        sizedDown /= 1024;
        prefixIndex++;
    }

    std::string sizeStr = std::to_string((int)sizedDown) + " " + prefixes[prefixIndex];
    return QString(sizeStr.c_str());
}

void
MetdataPreview::addPackagesBinHeirarchy(std::stringstream& outStr, LotusLib::PackageReader pkg, const std::string& filePath)
{
    std::stack<std::string> heirarchy;

    std::string curFile = filePath;
    while (curFile != "")
    {
        heirarchy.push(curFile);
        LotusLib::FileEntry nextEntry = pkg.getFile(curFile, LotusLib::READ_EXTRA_ATTRIBUTES);
        curFile = nextEntry.extra.parent;
    }

    if (heirarchy.size() == 1)
        return;

    outStr << std::endl << std::endl;

    outStr << "Hierarchy" << std::endl;
    outStr << "----------" << std::endl;

    int curLevel = 1;
    while (!heirarchy.empty())
    {
        outStr << "[" << curLevel++ << "]  " << heirarchy.top() << std::endl;
        heirarchy.pop();
    }
}