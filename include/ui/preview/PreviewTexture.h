#pragma once

#include "ui/preview/Preview.h"
#include "texture/TextureExtractor.h"
#include "ui/preview/TextureRenderWidget.h"

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSlider>
#include <QtCore/QObject>

class PreviewTexture : public Preview, public QObject
{
    PreviewTexture() = default;
    PreviewTexture(const PreviewTexture&) = delete;
    const PreviewTexture& operator=(const PreviewTexture&) = delete;

    TextureRenderWidget* m_textureWidget;
    QCheckBox* m_showAlphaCheckbox;
    QSlider* m_exposureSlider;
    bool m_hasAlpha;

public:
    static PreviewTexture* getInstance(); 

    void setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout, QWidget* previewButtonsArea, QHBoxLayout* layout) override;
    void unloadData() override;
    void hide() override;
    void show() override;
    void setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs) override;    
};