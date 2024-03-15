#pragma once

#include "ui/preview/Preview.h"

#include <QLabel>

class PreviewLoading : public Preview
{
    PreviewLoading() = default;
    PreviewLoading(const PreviewLoading&) = delete;
    const PreviewLoading& operator=(const PreviewLoading&) = delete;

    QLabel* m_widgetText;

public:
    static PreviewLoading* getInstance(); 

    void setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout) override;
    void unloadData() override;
    void hide() override;
    void show() override;
    void setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const WarframeExporter::Ensmallening& ensmalleningData) override;
};