#pragma once

/*
    Why does this file have a "1" at the end?
    Well, removing that reveals `include` errors
    I don't know why
*/

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QLabel>

#include "EnumMapExtractor.h"
#include "Extractor.h"

class FormatPreview
{
    QWidget* m_parentWidget;
    QVBoxLayout* m_layout;

public:
    FormatPreview();

    void setupUis(QWidget* parentWidget, QVBoxLayout* parentLayout);
    void setData(LotusLib::PackagesReader* pkgs, const std::string& pkgName, const LotusLib::LotusPath& internalPath);
    void clearPreview();

private:
    void setupModel(std::stringstream& outStr, LotusLib::PackagesReader* pkgs, LotusLib::FileEntry& fileEntry);
    void setupTexture(std::stringstream& outStr, LotusLib::PackagesReader* pkgs, LotusLib::FileEntry& fileEntry);
    void setupLevel(std::stringstream& outStr, LotusLib::FileEntry& fileEntry);
    void setupAudio(std::stringstream& outStr, LotusLib::FileEntry& fileEntry);
    void setupShader(std::stringstream& outStr, LotusLib::FileEntry& fileEntry);
    void setupLandscape(std::stringstream& outStr, LotusLib::FileEntry& fileEntry);
};