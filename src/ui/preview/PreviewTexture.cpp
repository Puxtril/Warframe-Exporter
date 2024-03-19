#include "ui/preview/PreviewTexture.h"

PreviewTexture*
PreviewTexture::getInstance()
{
    static PreviewTexture instance;
    return &instance;
}

void
PreviewTexture::setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout)
{
    m_textureWidget = new TextureRenderWidget(parentWidget);
    parentLayout->addWidget(m_textureWidget);

    m_textureWidget->hide();
}

void
PreviewTexture::unloadData()
{
}

void
PreviewTexture::hide()
{
    m_textureWidget->hide();
}

void
PreviewTexture::show()
{
    m_textureWidget->show();
}

void
PreviewTexture::setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const WarframeExporter::Ensmallening& ensmalleningData)
{
    auto textureExtractor = WarframeExporter::Texture::TextureExtractor::getInstance();

    WarframeExporter::Texture::TextureInternal textureData = textureExtractor->getTexture(fileEntry, pkgs, ensmalleningData);
    m_textureWidget->setTexture(textureData);
}
