#include "ui/BatchIteratorExportQt.h"

BatchIteratorExportQt::BatchIteratorExportQt()
    : m_fileCountExtracted(0), m_killExtraction(false), WarframeExporter::BatchIteratorExport()
{
}

void
BatchIteratorExportQt::processKnownFile(LotusLib::PackagesReader& pkgs, const std::string& pkgName, LotusLib::FileEntry& fileEntry, WarframeExporter::Extractor* extractor, const WarframeExporter::Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
    if (m_killExtraction)
    {
        m_killExtraction = false;
        resetFileCount();
        throw ExtractionCancelled();
    }
    BatchIteratorExport::processKnownFile(pkgs, pkgName, fileEntry, extractor, ensmalleningData, outputPath);
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
    : m_fileCount(0), WarframeExporter::BatchIterator()
{
}

int
BatchIteratorCountQt::getFileCount() const
{
    return m_fileCount;
}

void
BatchIteratorCountQt::processKnownFile(LotusLib::PackagesReader& pkgs, const std::string& pkgName, LotusLib::FileEntry& fileEntry, WarframeExporter::Extractor* extractor, const WarframeExporter::Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
    m_fileCount++;
}