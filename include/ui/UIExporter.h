#pragma once

#include <filesystem>

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
    QBrush m_dirBrush;

public:
    UiExporter();

    void setup(QMainWindow *MainWindow);
    void setCacheAndLoad(const std::filesystem::path& cachePath, WarframeExporter::ExtractorType extractorTypes);

private:
    void setupTree(WarframeExporter::ExtractorType extractorTypes);
    void setupTreeRecursive(QTreeWidgetItem* parentWidget, LotusLib::DirMeta& parentEntry);

    void clearMetaData();
    void setMetadata(TreeItemFile* file);

public slots:
    void itemClicked(QTreeWidgetItem *item, int column);
    void setData(std::filesystem::path cachePath, WarframeExporter::ExtractorType extractorTypes);
};