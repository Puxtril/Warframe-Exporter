#pragma once

#include "EnumMapExtractor.h"
#include "ui/ui_Loading.h"

#include "LotusLib/PackageCollection.h"

class UILoading : public QObject, private Ui_Loading
{
    Q_OBJECT

public:
    void setupUi(QDialog* dialog);
    void initProgressBar(LotusLib::PackageCollection& pkgs, WarframeExporter::ExtractorType extractTypes);

private:
    size_t getTotalFileCount(LotusLib::PackageCollection& pkgs, WarframeExporter::ExtractorType extractTypes);

public slots:
    void treeItemLoaded(size_t newSize);
};