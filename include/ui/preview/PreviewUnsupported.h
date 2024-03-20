#pragma once

#include "ui/preview/Preview.h"

#include <QtWidgets/QLabel>

class PreviewUnsupported : public Preview
{
    PreviewUnsupported() = default;
    PreviewUnsupported(const PreviewUnsupported&) = delete;
    const PreviewUnsupported& operator=(const PreviewUnsupported&) = delete;

    QLabel* m_widgetText;

public:
    static PreviewUnsupported* getInstance(); 

    void setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout) override;
    void unloadData() override;
    void hide() override;
    void show() override;
    void setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const WarframeExporter::Ensmallening& ensmalleningData) override;
};