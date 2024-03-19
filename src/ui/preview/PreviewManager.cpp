#include "ui/preview/PreviewManager.h"

PreviewManager::PreviewManager()
    : m_ensmallening(true, true, true)
{
    m_previewWidgets = {
        {WarframeExporter::ExtractorType::Material, PreviewMaterial::getInstance()},
        {WarframeExporter::ExtractorType::Texture, PreviewTexture::getInstance()}
    };
}

void
PreviewManager::setupUis(QWidget* parentWidget, QVBoxLayout* parentLayout)
{
    for (auto& child : m_previewWidgets)
    {
        std::get<1>(child)->setupUi(parentWidget, parentLayout);
    }
    PreviewUnsupported::getInstance()->setupUi(parentWidget, parentLayout);
}

void
PreviewManager::setData(LotusLib::PackagesReader* pkgs, const WarframeExporter::Ensmallening& ensmalleningData)
{
    m_pkgs = pkgs;
    m_ensmallening = ensmalleningData;
}

void
PreviewManager::swapToFilePreview(LotusLib::FileEntry& fileEntry)
{
    Preview* preview = getPreview(fileEntry);
    
    clearPreview();

    // OpenGL widgets do initlization before the first draw call.
    // So lets initilize OpenGL (by rendering once) before setting textures/models.
    preview->show();
    preview->setupWidget(fileEntry, *m_pkgs, m_ensmallening);
}

void
PreviewManager::clearPreview()
{
    for (auto& child : m_previewWidgets)
    {
        std::get<1>(child)->hide();
        std::get<1>(child)->unloadData();
    }
    PreviewUnsupported::getInstance()->hide();
}

Preview*
PreviewManager::getPreview(LotusLib::FileEntry& fileEntry)
{
    WarframeExporter::Extractor* extractor = WarframeExporter::g_enumMapExtractor[fileEntry.commonHeader.type];

    if (extractor == nullptr)
    {
        return PreviewUnsupported::getInstance();
    }

    if (m_previewWidgets.count(extractor->getExtractorType()))
        return m_previewWidgets[extractor->getExtractorType()];

    return PreviewUnsupported::getInstance();
}