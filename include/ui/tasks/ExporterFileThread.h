#pragma once

#include "Extractor.h"
#include "LotusLib/PackageCollection.h"
#include "Enums.h"
#include "ExtractFile.h"

#include <QtCore/QThread>
#include <string>
#include <memory>

class ExporterFileThread : public QThread
{
    Q_OBJECT

    LotusLib::PackageCollection* m_pkgsReader;
    LotusLib::PackagesBin* m_pkgsBin;
    std::filesystem::path m_exportPath;
    WarframeExporter::ExtractorType m_extractTypes;
    std::string m_pkgName;
    std::string m_internalFilePath;
    WarframeExporter::ExtractOptions m_options;

public:
    ExporterFileThread();

    void setData(LotusLib::PackageCollection* pkgsReader, LotusLib::PackagesBin* pkgsBin, std::filesystem::path exportPath, WarframeExporter::ExtractorType extractTypes, WarframeExporter::ExtractOptions options);
    void setFileData(std::string internalFilePath, std::string pkgName);
    void extractCancelled();
    void run();
    
signals:
    void extractStart(int totalItems);
    void extractError(std::string msg);
    void extractItemComplete(int curItemCount);
    void extractComplete(int totalItems);
};
