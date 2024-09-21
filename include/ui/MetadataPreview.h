#pragma once

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QLabel>

#include "LotusLib.h"
#include "EnumMapExtractor.h"

class MetdataPreview
{
    QWidget* m_parentWidget;
    QVBoxLayout* m_layout;

public:
    MetdataPreview();

    void setupUis(QWidget* parentWidget, QVBoxLayout* parentLayout);
    void setData(LotusLib::PackagesReader* pkgs, const std::string& pkgName, const LotusLib::LotusPath& internalPath);
    void clearPreview();

private:
    void setupCommonHeader(LotusLib::FileEntry& fileEntry);
};