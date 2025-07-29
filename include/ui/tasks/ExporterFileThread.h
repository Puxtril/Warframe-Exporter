#pragma once

#include "LotusLib.h"
#include "LotusPath.h"
#include "ExportSingleFile.h"

#include <QtCore/QThread>
#include <string>
#include <memory>

class ExporterFileThread : public QThread
{
    Q_OBJECT

    LotusLib::PackagesReader* m_pkgsReader;
    std::filesystem::path m_exportPath;
    std::string m_pkgName;
    LotusLib::LotusPath m_internalFilePath;
    WarframeExporter::ExtractOptions m_options;

public:
    ExporterFileThread();

    void setData(LotusLib::PackagesReader* pkgsReader, std::filesystem::path exportPath, WarframeExporter::ExtractOptions options);
    void setFileData(LotusLib::LotusPath internalFilePath, std::string pkgName);
    void extractCancelled();
    void run();
    
signals:
    void extractStart(int totalItems);
    void extractError(std::string msg);
    void extractItemComplete(int curItemCount);
    void extractComplete();
};
