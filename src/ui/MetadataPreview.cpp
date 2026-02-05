#include "ui/MetadataPreview.h"

#define _CRT_SECURE_NO_WARNINGS

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
MetdataPreview::setData(const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, const std::string& pkgName, LotusLib::FileEntry& fileEntry)
{
    std::stringstream outStr;
    setupCommonHeader(outStr, pkgs.getGame(), pkgName, fileEntry);
    addPackagesBinHeirarchy(outStr, pkgsBin, LotusLib::getFullPath(fileEntry.headerNode));
    std::string outStrTmp = outStr.str();
    m_textWidget->setPlainText(QString(outStrTmp.c_str()));

    setFiledata(pkgs.getPackage(pkgName), fileEntry.headerNode);
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
MetdataPreview::setupCommonHeader(std::stringstream& outStr, LotusLib::Game game, const std::string& pkgName, LotusLib::FileEntry& fileEntry)
{
    outStr << "Type Enum: ";
    outStr << fileEntry.commonHeader.type << " ";
    auto extractor = WarframeExporter::g_enumMapExtractor.at(game, LotusLib::findPackageCategory(pkgName), fileEntry.commonHeader.type);
    if (extractor != nullptr)
        outStr << "(" << extractor->getFriendlyName() << ")";
    else
        outStr << "(Unknown)";
    
    outStr << std::endl;
    outStr << "Package: " << pkgName << std::endl;
    
    outStr << std::endl;

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
MetdataPreview::setFiledata(const LotusLib::Package& pkgs, LotusLib::FileNode& fileNode)
{
    m_labelModified->setText(timestampToQString(fileNode.timeStamp));
    int totalCompressed = fileNode.compLen;
    int totalDecompressed = fileNode.len;

    try
    {
        const LotusLib::FileNode& bNode = pkgs.getFileNode(LotusLib::PkgSplitType::BODY, fileNode);
        totalCompressed += bNode.compLen;
        totalDecompressed += bNode.len;
    }
    catch (LotusLib::InternalEntryNotFound& ex) {}

    try
    {
        const LotusLib::FileNode& fNode = pkgs.getFileNode(LotusLib::PkgSplitType::FOOTER, fileNode);
        totalCompressed += fNode.compLen;
        totalDecompressed += fNode.len;
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
MetdataPreview::addPackagesBinHeirarchy(std::stringstream& outStr, const LotusLib::PackagesBin& pkgsBin, const std::string& filePath)
{
    std::stack<std::string> heirarchy;

    std::string curFile = filePath;
    while (curFile != "")
    {
        heirarchy.push(curFile);
        curFile = pkgsBin.getParent(curFile);
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