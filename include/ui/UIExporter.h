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
    WarframeExporter::ExtractorType m_extractorTypes;
    std::vector<std::string> m_pkgNames;
    QBrush m_dirBrush;

public:
    UiExporter();

    void setup(QMainWindow *MainWindow);
    void setCacheAndLoad(const std::filesystem::path& cachePath, WarframeExporter::ExtractorType extractorTypes);

private:
    void setupTree();
    // pkgNames are WF package names selected by the user
    // curEntries are a list of matching directories currently being processed.
    //   Since we are merging pkgNames.size packages, we need to merge matching directory names.
    //   Some directories may be null. That's fine. But we need to ensure curEntries[i] 
    //     lines up with pkgNames[i], even if curEntries[i] is currently null
    void setupTreeRecursive(const std::vector<std::string>& pkgNames, std::vector<LotusLib::DirMeta> curEntries, QTreeWidgetItem* parentWidget);

    void clearMetaData();
    void setMetadata(TreeItemFile* file);

    std::vector<std::string> getPackageNames() const;

public slots:
    void itemClicked(QTreeWidgetItem *item, int column);
    void setData(std::filesystem::path cachePath, WarframeExporter::ExtractorType extractorTypes);
};