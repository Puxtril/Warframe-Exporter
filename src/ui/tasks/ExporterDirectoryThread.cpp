#include "ui/tasks/ExporterDirectoryThread.h"

ExporterDirectoryThread::ExporterDirectoryThread()
    : QThread()
{
}

void
ExporterDirectoryThread::setData(LotusLib::PackagesReader* pkgsReader, std::filesystem::path exportPath, WarframeExporter::ExtractorType extractTypes, WarframeExporter::ExtractOptions options)
{
    m_pkgsReader = pkgsReader;
    m_exportPath = exportPath;
    m_extractTypes = extractTypes;
    m_options = options;

    m_exporter = std::make_shared<BatchIteratorExportQt>();
}

void
ExporterDirectoryThread::setInternalPath(LotusLib::LotusPath internalPath)
{
    m_internalPath = internalPath;
}

void
ExporterDirectoryThread::run()
{
    emit ExporterDirectoryThread::extractIndexingStarted();

    BatchIteratorCountQt totalItemsIter;
    totalItemsIter.batchIterate(*m_pkgsReader, m_exportPath, m_internalPath, m_extractTypes, m_pkgsReader->getGame(), m_options);
    int totalItems = static_cast<int>(totalItemsIter.getFileCount());

    emit ExporterDirectoryThread::extractStart(totalItems);
    
    connect(m_exporter.get(), &BatchIteratorExportQt::extractedFileCount, this, &ExporterDirectoryThread::_extractItemComplete);
    m_exporter->resetFileCount();

    try
    {
        m_exporter->batchIterate(*m_pkgsReader, m_exportPath, m_internalPath, m_extractTypes, m_pkgsReader->getGame(), m_options);
    }
    catch (ExtractionCancelled&)
    {
        disconnect(m_exporter.get(), &BatchIteratorExportQt::extractedFileCount, this, &ExporterDirectoryThread::_extractItemComplete);
        return;
    }
    catch (std::exception& ex)
    {
        emit ExporterDirectoryThread::extractError(ex.what());
        disconnect(m_exporter.get(), &BatchIteratorExportQt::extractedFileCount, this, &ExporterDirectoryThread::_extractItemComplete);
        return;
    }

    disconnect(m_exporter.get(), &BatchIteratorExportQt::extractedFileCount, this, &ExporterDirectoryThread::_extractItemComplete);
    emit ExporterDirectoryThread::extractComplete();
}

void
ExporterDirectoryThread::_extractItemComplete(int count)
{
    emit extractItemComplete(count);
}

void
ExporterDirectoryThread::extractCancelled()
{
    m_exporter->cancelExtraction();
}