#pragma once

#include "Extractor.h"
#include "Enums.h"
#include "ExtractFile.h"

#include <QtCore/QThread>
#include <string>
#include <memory>

class ExtractionCancelled {};

class ExporterDirectoryThread : public QThread
{
    Q_OBJECT

    LotusLib::PackageCollection* m_pkgsReader;
    LotusLib::PackagesBin* m_pkgsBin;
    std::filesystem::path m_exportPath;
    WarframeExporter::ExtractorType m_extractTypes;
    std::string m_internalPath;
    WarframeExporter::ExtractOptions m_options;

    size_t m_fileCountExtracted;
    bool m_extractCancelled;

public:
    ExporterDirectoryThread();

    void setData(LotusLib::PackageCollection* pkgsReader, LotusLib::PackagesBin* pkgsBin, std::filesystem::path exportPath, WarframeExporter::ExtractorType extractTypes, WarframeExporter::ExtractOptions options);
    void setInternalPath(std::string internalPath);
    void extractCancelled();
    void run();

private:
    void extractAllFiles(
        LotusLib::PackageCollection& pkgsDir,
        const std::filesystem::path& outputPath,
        const std::string& basePath,
        WarframeExporter::ExtractorType types,
        WarframeExporter::ExtractOptions options
    );
    
signals:
    void extractIndexingStarted();
    void extractStart(int totalItems);
    void extractItemComplete(int curItemCount);
    void extractError(std::string msg);
    void extractComplete(int totalItems);
};
