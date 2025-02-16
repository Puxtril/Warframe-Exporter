#pragma once

#include "EnumMapExtractor.h"
#include "ui/ui_Loading.h"

#include "LotusLib.h"

class UILoading : public QObject, private Ui_Loading
{
    Q_OBJECT

public:
    void setupUi(QDialog* dialog);
    void initProgressBar(LotusLib::PackagesReader& pkgs, WarframeExporter::ExtractorType extractTypes);

private:
    size_t getTotalFileCount(LotusLib::PackagesReader& pkgs, WarframeExporter::ExtractorType extractTypes);

public slots:
    void treeItemLoaded(size_t newSize);
};