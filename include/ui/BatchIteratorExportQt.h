#pragma once

#include <QProgressBar>

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
    BatchIteratorExportQt(const std::filesystem::path& pkgsDir, const WarframeExporter::Ensmallening& ensmalleningData, std::filesystem::path baseOutputPath);

    void processKnownFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, WarframeExporter::Extractor* extractor) override;
    void cancelExtraction();
    void resetFileCount();

signals:
    void extractedFileCount(int count);
};

class BatchIteratorCountQt : public WarframeExporter::BatchIterator
{
    int m_fileCount;

public:
    BatchIteratorCountQt(const std::filesystem::path& pkgsDir, const WarframeExporter::Ensmallening& ensmalleningData);

    int getFileCount() const;

    void processKnownFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, WarframeExporter::Extractor* extractor) override;
    void processUnknownFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry) override {}
    void processSkipFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, const WarframeExporter::Extractor* extractor) override {}
};