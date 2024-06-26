#pragma once

#include "ui/preview/Preview.h"
#include "texture/TextureExtractor.h"
#include "ui/preview/TextureRenderWidget.h"

#include <QtWidgets/QCheckBox>
#include <QtCore/QObject>

class PreviewTexture : public Preview
{
    PreviewTexture() = default;
    PreviewTexture(const PreviewTexture&) = delete;
    const PreviewTexture& operator=(const PreviewTexture&) = delete;

    TextureRenderWidget* m_textureWidget;
    QCheckBox* m_showAlphaCheckbox;

public:
    static PreviewTexture* getInstance(); 

    void setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout, QWidget* previewButtonsArea, QHBoxLayout* layout) override;
    void unloadData() override;
    void hide() override;
    void show() override;
    void setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const WarframeExporter::Ensmallening& ensmalleningData) override;
};