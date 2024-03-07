#pragma once

#include <QtWidgets/QProgressBar>

#include "BatchIteratorExport.h"
#include "Ensmallening.hpp"

// Exception that doesn't inherit from std::exception
// This shouldn't be caught by LotusLib
class ExtractionCancelled {};

class BatchIteratorExportQt : public QObject, public WarframeExporter::BatchIteratorExport
{
    Q_OBJECT

    int m_fileCountExtracted;
    bool m_killExtraction;

public:
    BatchIteratorExportQt();

    void processKnownFile(
        LotusLib::PackagesReader& pkgs,
        const std::string& pkgName,
        LotusLib::FileEntry& fileEntry,
        WarframeExporter::Extractor* extractor,
        const WarframeExporter::Ensmallening& ensmalleningData,
        const std::filesystem::path& outputPath
    ) override;

    void cancelExtraction();
    void resetFileCount();

signals:
    void extractedFileCount(int count);
};

class BatchIteratorCountQt : public WarframeExporter::BatchIterator
{
    int m_fileCount;

public:
    BatchIteratorCountQt();

    int getFileCount() const;

    void processKnownFile(
        LotusLib::PackagesReader& pkgs,
        const std::string& pkgName,
        LotusLib::FileEntry& fileEntry,
        WarframeExporter::Extractor* extractor,
        const WarframeExporter::Ensmallening& ensmalleningData,
        const std::filesystem::path& outputPath
    ) override;
		
    void processUnknownFile(
        LotusLib::PackagesReader& pkgs,
        const std::string& pkgName,
        LotusLib::FileEntry& fileEntry,
        const WarframeExporter::Ensmallening& ensmalleningData,
        const std::filesystem::path& outputPath
    ) override
    {}
		
    void processSkipFile(
        LotusLib::PackagesReader& pkgs,
        const std::string& pkgName,
        LotusLib::FileEntry& fileEntry,
        const WarframeExporter::Extractor* extractor
    ) override
    {}
};