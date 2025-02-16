#include "ui/UILoading.h"

void
UILoading::setupUi(QDialog* dialog)
{
    Ui_Loading::setupUi(dialog);
}

void
UILoading::initProgressBar(LotusLib::PackagesReader& pkgs, WarframeExporter::ExtractorType extractTypes)
{
    size_t totalFiles = getTotalFileCount(pkgs, extractTypes);
    this->progressBar->setRange(0, totalFiles);
    this->progressBar->setFormat("%p%");
}

size_t
UILoading::getTotalFileCount(LotusLib::PackagesReader& pkgs, WarframeExporter::ExtractorType extractTypes)
{
    size_t fileCount = 0;

    LotusLib::PackageCategory pkgCategories = WarframeExporter::g_enumMapExtractor.getPkgCategories(pkgs.getGame(), extractTypes);

    for (const std::string& curPkgName : pkgs)
    {
        std::optional<LotusLib::PackageReader> pkg = pkgs.getPackage(curPkgName);
        if (!pkg || ((int)pkg->getPkgCategory() & (int)pkgCategories) == 0)
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