#pragma once

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QLabel>
#include <stack>

#include "EnumMapExtractor.h"

// Windows...
#undef max

class MetdataPreview
{
    QPlainTextEdit* m_textWidget;
    QLabel* m_labelCompressed;
    QLabel* m_labelDecompressed;
    QLabel* m_labelModified;

public:
    MetdataPreview();

    void setupUis(QPlainTextEdit* textWidget, QLabel* compressed, QLabel* decompressed, QLabel* modified);
    void setData(const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, const std::string& pkgName, LotusLib::FileEntry& fileEntry);
    void clearPreview();

private:
    void setupCommonHeader(std::stringstream& outStr, LotusLib::Game game, const std::string& pkgName, LotusLib::FileEntry& fileEntry);
    void setFiledata(const LotusLib::Package& pkgs, LotusLib::FileNode& fileNode);
    static QString timestampToQString(int64_t input);
    static QString filesizeToQString(int input);
    static void addPackagesBinHeirarchy(std::stringstream& outStr, const LotusLib::PackagesBin& pkgsBin, const std::string& filePath);
};