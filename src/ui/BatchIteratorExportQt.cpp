#include "ui/BatchIteratorExportQt.h"

BatchIteratorExportQt::BatchIteratorExportQt(const std::filesystem::path& pkgsDir, const WarframeExporter::Ensmallening& ensmalleningData, std::filesystem::path baseOutputPath)
    : m_fileCountExtracted(0), m_killExtraction(false), WarframeExporter::BatchIteratorExport(pkgsDir, ensmalleningData, baseOutputPath)
{
}

void
BatchIteratorExportQt::processKnownFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, WarframeExporter::Extractor* extractor)
{
    if (m_killExtraction)
    {
        m_killExtraction = false;
        resetFileCount();
        throw ExtractionCancelled();
    }
    BatchIteratorExport::processKnownFile(pkg, fileEntry, extractor);
    emit extractedFileCount(++m_fileCountExtracted);
}

void
BatchIteratorExportQt::cancelExtraction()
{
    m_killExtraction = true;
}

void
BatchIteratorExportQt::resetFileCount()
{
    m_fileCountExtracted = 0;
}

BatchIteratorCountQt::BatchIteratorCountQt(const std::filesystem::path& pkgsDir, const WarframeExporter::Ensmallening& ensmalleningData)
    : m_fileCount(0), WarframeExporter::BatchIterator(pkgsDir, ensmalleningData, "")
{
}

int
BatchIteratorCountQt::getFileCount() const
{
    return m_fileCount;
}

void
BatchIteratorCountQt::processKnownFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, WarframeExporter::Extractor* extractor)
{
    m_fileCount++;
}