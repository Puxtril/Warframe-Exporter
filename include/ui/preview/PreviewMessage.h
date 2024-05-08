#pragma once

#include "ui/preview/Preview.h"

#include <QtWidgets/QLabel>

class PreviewMessage : public Preview
{
    PreviewMessage() = default;
    PreviewMessage(const PreviewMessage&) = delete;
    const PreviewMessage& operator=(const PreviewMessage&) = delete;

    QLabel* m_widgetText;

public:
    static PreviewMessage* getInstance(); 

    void setMessage(const std::string& msg);
    void setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout) override;
    void unloadData() override;
    void hide() override;
    void show() override;
    void setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const WarframeExporter::Ensmallening& ensmalleningData) override;
};