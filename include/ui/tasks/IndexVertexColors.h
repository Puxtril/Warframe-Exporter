#pragma once

#include "LotusLib.h"
#include "model/ModelExtractor.h"

#include <QtCore/QThread>
#include <string>

class IndexVertexColorsThread : public QThread
{
    LotusLib::PackagesReader* m_pkgsReader;

public:
    IndexVertexColorsThread();

    void setData(LotusLib::PackagesReader* pkgsReader);
    void run() override;
    void stop();
};