#pragma once

#include <filesystem>

#include <QtWidgets/QTreeWidget>
#include <QtGui/QBrush>
#include <QtWidgets/QMessageBox>
#include <QLineEdit>
#include <QTimer>
#include <QHash>
#include <QSet>
#include <QRegularExpression>
#include <QShortcut>

#include "LotusLib.h"
#include "Extractor.h"
#include "LotusPath.h"
#include "ExtractOptions.h"

#include "ui/UIMainWindow.h"
#include "ui/ui_Exporter.h"
#include "ui/TreeItemSubtypes.h"
#include "ui/preview/PreviewManager.h"
#include "ui/MetadataPreview.h"
#include "ui/FormatPreview1.h"

#include "ui/UILoading.h"

#include "ui/tasks/ExporterDirectoryThread.h"
#include "ui/tasks/ExporterFileThread.h"
#include "ui/tasks/LoadTreeThread.h"

class UiExporter : public QObject, private Ui_MainWindow
{
    Q_OBJECT

    LotusLib::PackagesReader m_packages;
    std::filesystem::path m_cacheDirPath;
    std::filesystem::path m_exportPath;
    WarframeExporter::ExtractorType m_extractTypes;
    ExporterDirectoryThread m_exporterDirectoryThread;
    ExporterFileThread m_exporterFileThread;

    PreviewManager m_previewManager;
    MetdataPreview m_metadataPreview;
    FormatPreview m_formatPreview;

    LoadTreeThread m_loadTreeThread;
    QDialog m_loadingDialog;
    UILoading m_loading;    
    
    QTimer m_searchTimer;    
    QList<QPair<QString, QTreeWidgetItem*>> m_searchIndex;

public:
    virtual ~UiExporter();

    void setup(UiMainWindow* MainWindow);

private:
    void setupShortcuts(UiMainWindow* MainWindow);
    void saveGeometry();
    void loadGeometry();

    void clearPreview();
    void setPreview(TreeItemFile* file);

    void extractDirectory(LotusLib::LotusPath internalPath);
    void extractFile(LotusLib::LotusPath internalPath, const std::string& pkgName);

    void swapToExtractButton();
    void swapToCancelButton();

public slots:
    void aboutToClose();
    void itemChanged();
    void setData(
        std::filesystem::path cachePath,
        std::filesystem::path exportPath,
        WarframeExporter::ExtractorType extractTypes,
        LotusLib::Game game,
        WarframeExporter::ExtractOptions options
    );
    void loadTreeFinished();
    void extractButtonClicked();
    void extractCancelButtonClicked();
    void extractIndexingStarted();
    void extractStart(int totalItems);
    void extractItemComplete(int curItemCount);
    void extractError(std::string msg);
    void extractComplete();
    void onSearchTextChanged(const QString& text);
    void filterTree();
    void buildSearchIndex(QTreeWidgetItem* parent);
};