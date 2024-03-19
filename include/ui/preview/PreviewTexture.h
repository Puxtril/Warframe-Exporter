#pragma once

#include "ui/preview/Preview.h"
#include "texture/TextureExtractor.h"
#include "ui/preview/TextureRenderWidget.h"

class PreviewTexture : public Preview
{
    PreviewTexture() = default;
    PreviewTexture(const PreviewTexture&) = delete;
    const PreviewTexture& operator=(const PreviewTexture&) = delete;

    TextureRenderWidget* m_textureWidget;

public:
    static PreviewTexture* getInstance(); 

    void setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout) override;
    void unloadData() override;
    void hide() override;
    void show() override;
    void setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const WarframeExporter::Ensmallening& ensmalleningData) override;
};