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

#include "ui/tasks/ExporterDirectoryThread.h"
#include "ui/tasks/ExporterFileThread.h"

class UiExporter : private Ui_MainWindow, public QObject
{
    LotusLib::PackagesReader m_packages;
    std::filesystem::path m_cacheDirPath;
    std::filesystem::path m_exportPath;
    WarframeExporter::ExtractorType m_extractTypes;
    std::vector<std::string> m_exportPkgNames;
    QBrush m_dirBrush;
    ExporterDirectoryThread m_exporterDirectoryThread;
    ExporterFileThread m_exporterFileThread;
    PreviewManager m_previewManager;

public:
    UiExporter();
    virtual ~UiExporter();

    void setup(UiMainWindow* MainWindow);

private:
    void saveGeometry();
    void loadGeometry();

    // curEntries are a list of matching directories currently being processed.
    // Since we are merging m_viewPkgNames.size packages, we need to merge matching directory names.
    // Some directories may be null. That's fine. But we need to ensure curEntries[i] 
    //   lines up with pkgNames[i], even if curEntries[i] is currently null
    void setupTree();
    void setupTreeRecursive(std::vector<const LotusLib::FileEntries::DirNode*> curEntries, QTreeWidgetItem* parentWidget);

    void clearMetaData();
    void clearPreview();
    void setMetadata(TreeItemFile* file);
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
    void extractButtonClicked();
    void extractCancelButtonClicked();
    void extractIndexingStarted();
    void extractStart(int totalItems);
    void extractItemComplete(int curItemCount);
    void extractError(std::string msg);
    void extractComplete();
};