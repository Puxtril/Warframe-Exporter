#include "ui/UILoading.h"

void
UILoading::setupUi(QDialog* dialog)
{
    Ui_Loading::setupUi(dialog);
}

void
UILoading::initProgressBar(std::vector<std::string> exportPkgNames, LotusLib::PackagesReader& pkgs)
{
    size_t totalFiles = getTotalFileCount(pkgs, exportPkgNames);
    this->progressBar->setRange(0, totalFiles);
    this->progressBar->setFormat("%p%");
}

size_t
UILoading::getTotalFileCount(LotusLib::PackagesReader& pkgs, std::vector<std::string> exportPkgNames)
{
    size_t fileCount = 0;

    for (const std::string& curPkg : exportPkgNames)
    {
        std::optional<LotusLib::PackageReader> pkg = pkgs.getPackage(curPkg);
        if (!pkg)
            continue;
        auto iter = pkg.value().begin();
        while (iter++ != pkg.value().end())
            fileCount++;
    }

    return fileCount;
}

void
UILoading::treeItemLoaded(size_t newSize)
{
    this->progressBar->setValue(newSize);
}