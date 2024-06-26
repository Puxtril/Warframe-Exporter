#include "ui/tasks/ExporterFileThread.h"

ExporterFileThread::ExporterFileThread()
    : QThread()
{
}

void
ExporterFileThread::setData(LotusLib::PackagesReader* pkgsReader, std::filesystem::path exportPath)
{
    m_pkgsReader = pkgsReader;
    m_exportPath = exportPath;
}

void
ExporterFileThread::setFileData(LotusLib::LotusPath internalFilePath, std::string pkgName)
{
    m_pkgName = pkgName;
    m_internalFilePath = internalFilePath;
}

void
ExporterFileThread::run()
{
    WarframeExporter::Ensmallening ensmalleningData(true, true, true);

    emit ExporterFileThread::extractStart(1);

    try
    {
        WarframeExporter::extractFile(*m_pkgsReader, m_pkgName, m_internalFilePath, ensmalleningData, m_exportPath);
    }
    catch (std::exception& ex)
    {
        emit ExporterFileThread::extractError(ex.what());
        return;
    }

    emit ExporterFileThread::extractItemComplete(1);
    emit ExporterFileThread::extractComplete();
}
