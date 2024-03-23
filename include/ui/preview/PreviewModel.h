#pragma once

#include "ui/preview/Preview.h"
#include "model/ModelExtractor.h"
#include "ui/preview/ModelRenderWidget.h"

class PreviewModel : public Preview
{
    PreviewModel() = default;
    PreviewModel(const PreviewModel&) = delete;
    const PreviewModel& operator=(const PreviewModel&) = delete;

    ModelRenderWidget* m_modelWidget;

public:
    static PreviewModel* getInstance(); 

    void setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout) override;
    void unloadData() override;
    void hide() override;
    void show() override;
    void setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const WarframeExporter::Ensmallening& ensmalleningData) override;
};