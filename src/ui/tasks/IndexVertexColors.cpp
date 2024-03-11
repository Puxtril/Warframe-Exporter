#include "ui/tasks/IndexVertexColors.h"

IndexVertexColorsThread::IndexVertexColorsThread()
    : QThread()
{
}

void
IndexVertexColorsThread::setData(LotusLib::PackagesReader* pkgsReader)
{
    m_pkgsReader = pkgsReader;
}

void
IndexVertexColorsThread::run()
{
    LotusLib::PackageReader pkg = m_pkgsReader->getPackage("Misc");
    WarframeExporter::Model::ModelExtractor::getInstance()->indexVertexColors(pkg);
}

void
IndexVertexColorsThread::stop()
{
    WarframeExporter::Model::ModelExtractor::getInstance()->cancelVertexColorIndexing();
}