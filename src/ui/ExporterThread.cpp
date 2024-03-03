#include "ui/ExporterThread.h"

ExporterThread::ExporterThread()
    : QThread()
{
}

void
ExporterThread::setData(std::filesystem::path cacheDirPath, std::filesystem::path exportPath, WarframeExporter::ExtractorType extractTypes, std::vector<std::string> pkgNames)
{
    m_cacheDirPath = cacheDirPath;
    m_exportPath = exportPath;
    m_extractTypes = extractTypes;
    m_exportPkgNames = pkgNames;

    m_exporter = std::make_shared<BatchIteratorExportQt>(cacheDirPath, WarframeExporter::Ensmallening(true, true, true), exportPath);
}

void
ExporterThread::setInternalPath(LotusLib::LotusPath internalPath)
{
    m_internalPath = internalPath;
}

void
ExporterThread::run()
{
    emit ExporterThread::extractIndexingStarted();

    WarframeExporter::Ensmallening ensmalleningData(true, true, true);

    auto totalItemsIter = BatchIteratorCountQt(m_cacheDirPath, ensmalleningData);
    totalItemsIter.batchIterate(m_internalPath, m_exportPkgNames, m_extractTypes);
    size_t totalItems = totalItemsIter.getFileCount();

    emit ExporterThread::extractStart(totalItems);
    
    connect(m_exporter.get(), &BatchIteratorExportQt::extractedFileCount, this, &ExporterThread::_extractItemComplete);
    m_exporter->resetFileCount();

    try
    {
        m_exporter->batchIterate(m_internalPath, m_exportPkgNames, m_extractTypes);
    }
    catch (ExtractionCancelled& ex)
    {
        disconnect(m_exporter.get(), &BatchIteratorExportQt::extractedFileCount, this, &ExporterThread::_extractItemComplete);
        return;
    }
    catch (std::exception& ex)
    {
        emit ExporterThread::extractError(ex.what());
        disconnect(m_exporter.get(), &BatchIteratorExportQt::extractedFileCount, this, &ExporterThread::_extractItemComplete);
        return;
    }

    disconnect(m_exporter.get(), &BatchIteratorExportQt::extractedFileCount, this, &ExporterThread::_extractItemComplete);
    emit ExporterThread::extractComplete();
}

void
ExporterThread::_extractItemComplete(int count)
{
    emit extractItemComplete(count);
}

void
ExporterThread::extractCancelled()
{
    m_exporter->cancelExtraction();
}