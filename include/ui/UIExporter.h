#pragma once

#include <filesystem>
#include <set>

#include <QtWidgets/QTreeWidget>
#include <QtGui/QBrush>
#include <QtWidgets/QMessageBox>

#include "LotusLib.h"
#include "Extractor.h"
#include "LotusPath.h"

#include "ui/UIMainWindow.h"
#include "ui/Settings.h"
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
    std::vector<std::string> m_exportPkgNames;
    ExporterDirectoryThread m_exporterDirectoryThread;
    ExporterFileThread m_exporterFileThread;

    PreviewManager m_previewManager;
    MetdataPreview m_metadataPreview;
    FormatPreview m_formatPreview;

    LoadTreeThread m_loadTreeThread;
    QDialog m_loadingDialog;
    UILoading m_loading;

public:
    virtual ~UiExporter();

    void setup(UiMainWindow* MainWindow);

private:
    void saveGeometry();
    void loadGeometry();

    void clearPreview();
    void setPreview(TreeItemFile* file);

    void extractDirectory(LotusLib::LotusPath internalPath);
    void extractFile(LotusLib::LotusPath internalPath, const std::string& pkgName);

    static std::vector<std::string> getPackageNames(LotusLib::PackagesReader& packges, WarframeExporter::ExtractorType extractTypes);

    void swapToExtractButton();
    void swapToCancelButton();

public slots:
    void aboutToClose();
    void itemChanged();
    void setData(
        std::filesystem::path cachePath,
        std::filesystem::path exportPath,
        WarframeExporter::ExtractorType extractTypes,
        WarframeExporter::Shader::ShaderExportType shaderExportType,
        WarframeExporter::Texture::TextureExportType textureExportType    
    );
    void loadTreeFinished();
    void extractButtonClicked();
    void extractCancelButtonClicked();
    void extractIndexingStarted();
    void extractStart(int totalItems);
    void extractItemComplete(int curItemCount);
    void extractError(std::string msg);
    void extractComplete();
};