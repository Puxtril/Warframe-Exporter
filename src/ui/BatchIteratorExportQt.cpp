#include "ui/BatchIteratorExportQt.h"

BatchIteratorExportQt::BatchIteratorExportQt()
    : WarframeExporter::BatchIteratorExport(), m_fileCountExtracted(0), m_killExtraction(false)
{
}

void
BatchIteratorExportQt::processKnownFile(LotusLib::PackagesReader& pkgs, LotusLib::FileEntry& fileEntry, WarframeExporter::Extractor* extractor, const std::filesystem::path& outputPath, WarframeExporter::ExtractOptions options)
{
    if (m_killExtraction)
    {
        m_killExtraction = false;
        resetFileCount();
        throw ExtractionCancelled();
    }
    BatchIteratorExport::processKnownFile(pkgs, fileEntry, extractor, outputPath, options);
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

BatchIteratorCountQt::BatchIteratorCountQt()
    : WarframeExporter::BatchIterator(), m_fileCount(0)
{
}

int
BatchIteratorCountQt::getFileCount() const
{
    return m_fileCount;
}

void
BatchIteratorCountQt::processKnownFile(LotusLib::PackagesReader& pkgs, LotusLib::FileEntry& fileEntry, WarframeExporter::Extractor* extractor, const std::filesystem::path& outputPath, WarframeExporter::ExtractOptions options)
{
    m_fileCount++;
}