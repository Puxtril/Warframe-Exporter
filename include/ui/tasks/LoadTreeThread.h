#pragma once

#include <set>
#include <vector>
#include <string>

#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtCore/QThread>
#include <QtWidgets/QApplication>

#include "LotusLib/TOCTree.h"
#include "EnumMapExtractor.h"

#include "ui/TreeItemSubtypes.h"

class LoadTreeThread : public QThread
{
    Q_OBJECT

    QBrush m_dirBrush;

    LotusLib::PackageCollection* m_pkgs;
    std::vector<std::string> m_exportPkgNames;
    WarframeExporter::ExtractorType m_extractTypes;
    QTreeWidget* m_treeWidget;
    bool m_shouldFilterFiles;

    size_t m_processedFileCount;

public:
    LoadTreeThread();

    void setData(
        WarframeExporter::ExtractorType extractTypes,
        LotusLib::PackageCollection& pkgs,
        QTreeWidget* parentWidget,
        bool shouldFilterFiles
    );

    void run() override;

private:
    // curEntries are a list of matching directories currently being processed.
    // Since we are merging m_viewPkgNames.size packages, we need to merge matching directory names.
    // Some directories may be null. That's fine. But we need to ensure curEntries[i] 
    //   lines up with pkgNames[i], even if curEntries[i] is currently null
    void setupTree();
    void setupTreeRecursive(std::vector<const LotusLib::DirNode*> curEntries, QTreeWidgetItem* parentWidget);

    // Increments the processed file count
    // Determines when to emit the signal `treeItemLoaded`
    void incrementFileCounter();

    // Derrive package names from extractor types
    std::vector<std::string> findPkgNames(LotusLib::PackageCollection& pkgsReader, WarframeExporter::ExtractorType extractTypes);

signals:
    void treeItemsLoaded(size_t newSize);
    void treeLoadingFinished();
};