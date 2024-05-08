#pragma once

#include "ui/ui_Loading.h"

#include "LotusLib.h"

class UILoading : public QObject, private Ui_Loading
{
    Q_OBJECT

public:
    void setupUi(QDialog* dialog);
    void initProgressBar(std::vector<std::string> exportPkgNames, LotusLib::PackagesReader& pkgs);

private:
    size_t getTotalFileCount(LotusLib::PackagesReader& pkgs, std::vector<std::string> exportPkgNames);

public slots:
    void treeItemLoaded(size_t newSize);
};