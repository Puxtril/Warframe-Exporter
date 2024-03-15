#pragma once

#include "LotusLib.h"
#include "Ensmallening.hpp"

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>

class Preview
{
public:
    virtual void setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout) = 0;
    virtual void unloadData() = 0;
    virtual void hide() = 0;
    virtual void show() = 0;
    virtual void setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const WarframeExporter::Ensmallening& ensmalleningData) = 0;
};