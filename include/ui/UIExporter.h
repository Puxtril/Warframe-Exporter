#pragma once

#include <filesystem>
#include <set>

#include <QTreeWidget>
#include <QBrush>
#include <QMessageBox>

#include "LotusLib.h"
#include "Extractor.h"
#include "LotusPath.h"
#include "ui/ui_Exporter.h"
#include "ui/TreeItemSubtypes.h"
#include "ui/ExporterThread.h"

class UiExporter : private Ui_MainWindow, public QObject
{
    LotusLib::PackagesReader m_packages;
    std::filesystem::path m_cacheDirPath;
    std::filesystem::path m_exportPath;
    WarframeExporter::ExtractorType m_viewTypes;
    WarframeExporter::ExtractorType m_extractTypes;
    std::vector<std::string> m_viewPkgNames;
    std::vector<std::string> m_exportPkgNames;
    QBrush m_dirBrush;
    ExporterThread m_exporterThread;

public:
    UiExporter();

    void setup(QMainWindow *MainWindow);

private:
    // curEntries are a list of matching directories currently being processed.
    // Since we are merging m_viewPkgNames.size packages, we need to merge matching directory names.
    // Some directories may be null. That's fine. But we need to ensure curEntries[i] 
    //   lines up with pkgNames[i], even if curEntries[i] is currently null
    void setupTree();
    void setupTreeRecursive(std::vector<LotusLib::DirMeta> curEntries, QTreeWidgetItem* parentWidget);

    void clearMetaData();
    void setMetadata(TreeItemFile* file);

    void extractDirectory(LotusLib::LotusPath internalPath);
    void extractFile(LotusLib::LotusPath internalPath);

    static std::vector<std::string> getPackageNames(WarframeExporter::ExtractorType extractTypes);

    void swapToExtractButton();
    void swapToCancelButton();

public slots:
    void itemClicked(QTreeWidgetItem *item, int column);
    void setData(std::filesystem::path cachePath, std::filesystem::path exportPath, WarframeExporter::ExtractorType viewTypes, WarframeExporter::ExtractorType extractTypes);
    void extractButtonClicked();
    void extractCancelButtonClicked();
    void extractIndexingStarted();
    void extractStart(int totalItems);
    void extractItemComplete(int curItemCount);
    void extractError(std::string msg);
    void extractComplete();
};