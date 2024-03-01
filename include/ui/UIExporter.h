#pragma once

#include <filesystem>
#include <set>

#include <QTreeWidget>
#include <QBrush>

#include "LotusLib.h"
#include "ui/ui_Exporter.h"
#include "Extractor.h"
#include "TreeItemSubtypes.h"

class UiExporter : private Ui_MainWindow, public QObject
{
    LotusLib::PackagesReader m_packages;
    std::filesystem::path m_exportPath;
    WarframeExporter::ExtractorType m_viewTypes;
    WarframeExporter::ExtractorType m_extractTypes;
    std::vector<std::string> m_viewPkgNames;
    std::vector<std::string> m_exportPkgNames;
    QBrush m_dirBrush;

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

    static std::vector<std::string> getPackageNames(WarframeExporter::ExtractorType extractTypes);

public slots:
    void itemClicked(QTreeWidgetItem *item, int column);
    void setData(std::filesystem::path cachePath, std::filesystem::path exportPath, WarframeExporter::ExtractorType viewTypes, WarframeExporter::ExtractorType extractTypes);
};