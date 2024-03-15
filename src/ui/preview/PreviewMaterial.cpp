#include "ui/preview/PreviewMaterial.h"

PreviewMaterial*
PreviewMaterial::getInstance()
{
    static PreviewMaterial instance;
    return &instance;
}

void
PreviewMaterial::setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout)
{
    m_widgetText = new QPlainTextEdit(parentWidget);
    parentLayout->addWidget(m_widgetText);

    m_widgetText->viewport()->setProperty("cursor", QVariant(QCursor(Qt::IBeamCursor)));
    m_widgetText->setAcceptDrops(false);
    m_widgetText->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_widgetText->setReadOnly(true);
    m_widgetText->hide();
}

void
PreviewMaterial::unloadData()
{
    m_widgetText->clear();
}

void
PreviewMaterial::hide()
{
    m_widgetText->hide();
}

void
PreviewMaterial::show()
{
    m_widgetText->show();
}

void
PreviewMaterial::setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const WarframeExporter::Ensmallening& ensmalleningData)
{
    auto materialExtractor = WarframeExporter::Material::MaterialExtractor::getInstance();

    WarframeExporter::Material::MaterialExternal matExternal = materialExtractor->getExternalMaterial(&fileEntry.headerData, fileEntry.commonHeader);
    WarframeExporter::Material::MaterialInternal matInternal = materialExtractor->formatMaterial(matExternal);
    
    QString materialData(matInternal.formatted.c_str());
    m_widgetText->setPlainText(materialData);
}
