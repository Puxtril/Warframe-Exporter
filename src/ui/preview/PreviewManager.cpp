#include "ui/preview/PreviewManager.h"

PreviewManager::PreviewManager()
{
    m_previewWidgets = {
        {WarframeExporter::ExtractorType::Material, PreviewMaterial::getInstance()},
        {WarframeExporter::ExtractorType::Texture, PreviewTexture::getInstance()},
        {WarframeExporter::ExtractorType::Model, PreviewModel::getInstance()},
        {WarframeExporter::ExtractorType::Audio, PreviewAudio::getInstance()}
    };
}

void
PreviewManager::setupUis(QWidget* parentWidget, QVBoxLayout* parentLayout, QWidget* previewButtonArea, QHBoxLayout* previewButtonLayout)
{
    for (auto& child : m_previewWidgets)
    {
        std::get<1>(child)->setupUi(parentWidget, parentLayout, previewButtonArea, previewButtonLayout);
    }
    PreviewMessage::getInstance()->setupUi(parentWidget, parentLayout, previewButtonArea, previewButtonLayout);
}

void
PreviewManager::setData(LotusLib::PackagesReader* pkgs)
{
    m_pkgs = pkgs;
}

void
PreviewManager::swapToFilePreview(LotusLib::FileEntry& fileEntry)
{
    Preview* preview = getPreview(fileEntry);
    
    clearPreview();

    try
    {
        // OpenGL widgets do initlization before the first draw call.
        // So lets initilize OpenGL (by rendering once) before setting textures/models.
        preview->show();
        preview->setupWidget(fileEntry, *m_pkgs);
    }
    catch (std::exception&)
    {
        preview->hide();
        preview->unloadData();
        PreviewMessage::getInstance()->setMessage("Error loading preview");
        PreviewMessage::getInstance()->show();
    }
}

void
PreviewManager::clearPreview()
{
    for (auto& child : m_previewWidgets)
    {
        std::get<1>(child)->hide();
        std::get<1>(child)->unloadData();
    }
    PreviewMessage::getInstance()->hide();
}

Preview*
PreviewManager::getPreview(LotusLib::FileEntry& fileEntry)
{
    WarframeExporter::Extractor* extractor = WarframeExporter::g_enumMapExtractor.at(m_pkgs->getGame(), LotusLib::findPackageCategory(fileEntry.srcPkgName), fileEntry.commonHeader.type);

    if (extractor == nullptr)
    {
        PreviewMessage::getInstance()->setMessage("Preview Not Supported");
        return PreviewMessage::getInstance();
    }

    if (m_previewWidgets.count(extractor->getExtractorType()))
    {
        return m_previewWidgets[extractor->getExtractorType()];
    }

    PreviewMessage::getInstance()->setMessage("Preview Not Supported");
    return PreviewMessage::getInstance();
}