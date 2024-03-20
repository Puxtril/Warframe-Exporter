#pragma once

#include "ui/preview/Preview.h"
#include "material/MaterialExtractor.h"

#include <QtWidgets/QPlainTextEdit>

class PreviewMaterial : public Preview
{
    PreviewMaterial() = default;
    PreviewMaterial(const PreviewMaterial&) = delete;
    const PreviewMaterial& operator=(const PreviewMaterial&) = delete;

    QPlainTextEdit* m_widgetText;

public:
    static PreviewMaterial* getInstance(); 

    void setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout) override;
    void unloadData() override;
    void hide() override;
    void show() override;
    void setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const WarframeExporter::Ensmallening& ensmalleningData) override;
};