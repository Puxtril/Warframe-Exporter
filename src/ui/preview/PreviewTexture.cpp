#include "ui/preview/PreviewTexture.h"

PreviewTexture*
PreviewTexture::getInstance()
{
    static PreviewTexture instance;
    return &instance;
}

void
PreviewTexture::setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout, QWidget* previewButtonsArea, QHBoxLayout* layout)
{
    m_textureWidget = new TextureRenderWidget(parentWidget);
    parentLayout->addWidget(m_textureWidget);
    m_textureWidget->hide();

    m_showAlphaCheckbox = new QCheckBox(previewButtonsArea);
    layout->addWidget(m_showAlphaCheckbox);
    m_showAlphaCheckbox->setText("Apply Alpha");
    m_showAlphaCheckbox->hide();

    QObject::connect(m_showAlphaCheckbox, &QCheckBox::stateChanged, m_textureWidget, &TextureRenderWidget::showAlpha);
}

void
PreviewTexture::unloadData()
{
}

void
PreviewTexture::hide()
{
    m_textureWidget->hide();
    m_showAlphaCheckbox->hide();
}

void
PreviewTexture::show()
{
    m_textureWidget->show();
    m_showAlphaCheckbox->show();
}

void
PreviewTexture::setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs)
{
    auto textureExtractor = WarframeExporter::Texture::TextureExtractor::getInstance();

    WarframeExporter::Texture::TextureInternal textureData = textureExtractor->getTexture(fileEntry, pkgs);
    m_textureWidget->setTexture(textureData);

    bool hasAlpha = textureData.header.ddsFormat == ddspp::DXGIFormat::BC1_UNORM ? false : ddspp::hasAlpha(textureData.header.ddsFormat);
    m_showAlphaCheckbox->setHidden(!hasAlpha);
}
