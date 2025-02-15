#pragma once

#include "Extractor.h"
#include "LotusPath.h"
#include "ui/BatchIteratorExportQt.h"

#include <QtCore/QThread>
#include <string>
#include <memory>

class ExporterDirectoryThread : public QThread
{
    Q_OBJECT

    LotusLib::PackagesReader* m_pkgsReader;
    std::filesystem::path m_exportPath;
    WarframeExporter::ExtractorType m_extractTypes;
    std::vector<std::string> m_exportPkgNames;
    LotusLib::LotusPath m_internalPath;
    std::shared_ptr<BatchIteratorExportQt> m_exporter;

public:
    ExporterDirectoryThread();

    void setData(LotusLib::PackagesReader* pkgsReader, std::filesystem::path exportPath, WarframeExporter::ExtractorType extractTypes, std::vector<std::string> pkgNames);
    void setInternalPath(LotusLib::LotusPath internalPath);
    void extractCancelled();
    void run();

private slots:
    void _extractItemComplete(int count);
    
signals:
    void extractIndexingStarted();
    void extractStart(int totalItems);
    void extractItemComplete(int curItemCount);
    void extractError(std::string msg);
    void extractComplete();
};
