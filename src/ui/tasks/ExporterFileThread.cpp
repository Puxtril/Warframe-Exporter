#include "ui/tasks/ExporterFileThread.h"

ExporterFileThread::ExporterFileThread()
    : QThread()
{
}

void
ExporterFileThread::setData(LotusLib::PackageCollection* pkgsReader, LotusLib::PackagesBin* pkgsBin, std::filesystem::path exportPath, WarframeExporter::ExtractorType extractTypes, WarframeExporter::ExtractOptions options)
{
    m_pkgsReader = pkgsReader;
    m_pkgsBin = pkgsBin;
    m_exportPath = exportPath;
    m_extractTypes = extractTypes;
    m_options = options;
}

void
ExporterFileThread::setFileData(std::string internalFilePath, std::string pkgName)
{
    m_pkgName = pkgName;
    m_internalFilePath = internalFilePath;
}

void
ExporterFileThread::run()
{
    emit ExporterFileThread::extractStart(1);

    try
    {
        WarframeExporter::tryExtractFile(*m_pkgsReader, *m_pkgsBin, m_internalFilePath, m_exportPath, m_extractTypes, m_options);
    }
    catch (std::exception& ex)
    {
        emit ExporterFileThread::extractError(ex.what());
        return;
    }

    emit ExporterFileThread::extractItemComplete(1);
    emit ExporterFileThread::extractComplete(1);
}
