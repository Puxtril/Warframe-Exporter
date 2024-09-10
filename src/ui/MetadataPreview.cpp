#include "ui/MetadataPreview.h"

MetdataPreview::MetdataPreview()
    : m_parentWidget(nullptr), m_layout(nullptr)
{ }

void
MetdataPreview::setupUis(QWidget* parentWidget, QVBoxLayout* parentLayout)
{
    m_parentWidget = parentWidget;
    m_layout = parentLayout;
}

void
MetdataPreview::setData(LotusLib::PackagesReader* pkgs, const std::string& pkgName, const LotusLib::LotusPath& internalPath)
{
    LotusLib::FileEntry fileEntry = pkgs->getPackage(pkgName).getFile(internalPath, LotusLib::READ_COMMON_HEADER);
    setupCommonHeader(fileEntry);
}

void
MetdataPreview::clearPreview()
{
    for (QWidget* x : m_parentWidget->findChildren<QWidget*>(Qt::FindDirectChildrenOnly))
        delete x;
}

void
MetdataPreview::setupCommonHeader(LotusLib::FileEntry& fileEntry)
{
    QLabel* label = new QLabel(m_parentWidget);
    m_layout->addWidget(label);

    label->setText("Common Header");

    QPlainTextEdit* textEdit = new QPlainTextEdit(m_parentWidget);
    m_layout->addWidget(textEdit);

    std::stringstream outStr;

    outStr << "Type Enum: ";
    outStr << fileEntry.commonHeader.type << " ";
    auto extractor = WarframeExporter::g_enumMapExtractor[fileEntry.commonHeader.type];
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


    std::string outStrTmp = outStr.str();
    textEdit->setPlainText(QString(outStrTmp.c_str()));
}