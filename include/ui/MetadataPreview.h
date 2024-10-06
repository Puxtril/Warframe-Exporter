#pragma once

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QLabel>

#include "LotusLib.h"
#include "EnumMapExtractor.h"

class MetdataPreview
{
    QPlainTextEdit* m_textWidget;
    QLabel* m_labelCompressed;
    QLabel* m_labelDecompressed;
    QLabel* m_labelModified;

public:
    MetdataPreview();

    void setupUis(QPlainTextEdit* textWidget, QLabel* compressed, QLabel* decompressed, QLabel* modified);
    void setData(LotusLib::PackagesReader* pkgs, const std::string& pkgName, const LotusLib::LotusPath& internalPath);
    void clearPreview();

private:
    void setupCommonHeader(LotusLib::FileEntry& fileEntry);
    void setFiledata(LotusLib::PackageReader& pkgs, LotusLib::FileEntry& fileEntry);
    static QString timestampToQString(int64_t input);
    static QString filesizeToQString(int input);
};