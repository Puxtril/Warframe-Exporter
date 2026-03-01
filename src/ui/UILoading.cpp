#include "ui/UILoading.h"

void
UILoading::setupUi(QDialog* dialog)
{
    Ui_Loading::setupUi(dialog);
}

void
UILoading::initProgressBar(LotusLib::PackageCollection& pkgs, WarframeExporter::ExtractorType extractTypes)
{
    size_t totalFiles = getTotalFileCount(pkgs, extractTypes);
    this->progressBar->setRange(0, totalFiles);
    this->progressBar->setFormat("%p%");
}

size_t
UILoading::getTotalFileCount(LotusLib::PackageCollection& pkgs, WarframeExporter::ExtractorType extractTypes)
{
    size_t fileCount = 0;

    LotusLib::PackageCategory pkgCategories = WarframeExporter::g_enumMapExtractor.getPkgCategories(pkgs.getGame(), extractTypes);

    for (const LotusLib::Package& curPkg : pkgs)
    {
        if (((int)curPkg.getPkgCategory() & (int)pkgCategories) == 0)
            continue;
        for (auto& i : curPkg)
            fileCount++;
    }

    return fileCount;
}

void
UILoading::treeItemLoaded(size_t newSize)
{
    this->progressBar->setValue(newSize);
}