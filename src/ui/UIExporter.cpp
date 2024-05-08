#include "ui/UIExporter.h"

UiExporter::~UiExporter()
{
    if (m_exporterDirectoryThread.isRunning())
    {
        m_exporterDirectoryThread.extractCancelled();
        m_exporterDirectoryThread.wait();
    }
}

void
UiExporter::setup(UiMainWindow *MainWindow)
{
    Ui_MainWindow::setupUi(MainWindow);
    connect(this->treeWidget, &QTreeWidget::itemSelectionChanged, this, &UiExporter::itemChanged);
    connect(this->ExtractButton, &QPushButton::clicked, this, &UiExporter::extractButtonClicked);

    loadGeometry();
    connect(MainWindow, &UiMainWindow::mainWindowClose, this, &UiExporter::aboutToClose);

    // Loading dialog
    m_loading.setupUi(&m_loadingDialog);
    m_loadingDialog.setModal(true);
    connect(&m_loadTreeThread, &LoadTreeThread::treeItemsLoaded, &m_loading, &UILoading::treeItemLoaded);
    connect(&m_loadTreeThread, &LoadTreeThread::treeLoadingFinished, this, &UiExporter::loadTreeFinished);

    // Directory exporter
    connect(&m_exporterDirectoryThread, &ExporterDirectoryThread::extractIndexingStarted, this, &UiExporter::extractIndexingStarted);
    connect(&m_exporterDirectoryThread, &ExporterDirectoryThread::extractStart, this, &UiExporter::extractStart);
    connect(&m_exporterDirectoryThread, &ExporterDirectoryThread::extractItemComplete, this, &UiExporter::extractItemComplete);
    connect(&m_exporterDirectoryThread, &ExporterDirectoryThread::extractError, this, &UiExporter::extractError);
    connect(&m_exporterDirectoryThread, &ExporterDirectoryThread::extractComplete, this, &UiExporter::extractComplete);
    // File exporter
    connect(&m_exporterFileThread, &ExporterFileThread::extractStart, this, &UiExporter::extractStart);
    connect(&m_exporterFileThread, &ExporterFileThread::extractError, this, &UiExporter::extractError);
    connect(&m_exporterFileThread, &ExporterFileThread::extractItemComplete, this, &UiExporter::extractItemComplete);
    connect(&m_exporterFileThread, &ExporterFileThread::extractComplete, this, &UiExporter::extractComplete);

    m_previewManager.setupUis(this->Preview, this->verticalLayout_3);
}

void
UiExporter::saveGeometry()
{
    UiSettings& settings = UiSettings::getInstance();
    
    settings.saveSplitterData(this->splitter->saveGeometry(), this->splitter->saveState());
}

void
UiExporter::loadGeometry()
{
    UiSettings& settings = UiSettings::getInstance();

    this->splitter->restoreState(settings.getSplitterState());
    this->splitter->restoreGeometry(settings.getSplitterGeometry());
}

void
UiExporter::clearMetaData()
{
    this->NameData->clear();
    this->FullPathData->clear();
    this->SizeData->clear();
    this->CompressedLengthData->clear();
    this->TimestampData->clear();
    this->PackageData->clear();
}

void
UiExporter::clearPreview()
{
    m_previewManager.clearPreview();
}

void
UiExporter::setMetadata(TreeItemFile* file)
{
    this->NameData->setText(file->getQName());
    this->FullPathData->setText(file->getQFullpath());
    this->SizeData->setText(file->getQSize());
    this->CompressedLengthData->setText(file->getQCompressedSize());
    this->TimestampData->setText(file->getQTimestamp());
    this->PackageData->setText(file->getQPkg());
}

void
UiExporter::setPreview(TreeItemFile* file)
{ 
    LotusLib::FileEntry fileEntry = m_packages.getPackage(file->getPkg()).getFile(file->getQFullpath().toStdString());
    m_previewManager.swapToFilePreview(fileEntry);
}

void
UiExporter::extractDirectory(LotusLib::LotusPath internalPath)
{
    m_exporterDirectoryThread.setInternalPath(internalPath);
    m_exporterDirectoryThread.start();
}

void
UiExporter::extractFile(LotusLib::LotusPath internalPath, const std::string& pkgName)
{
    m_exporterFileThread.setFileData(internalPath, pkgName);
    m_exporterFileThread.start();
}

std::vector<std::string>
UiExporter::getPackageNames(LotusLib::PackagesReader& packages, WarframeExporter::ExtractorType extractTypes)
{
    std::vector<std::string> pkgNames;
    if (((int)extractTypes & (int)WarframeExporter::ExtractorType::Model) > 0 ||
        ((int)extractTypes & (int)WarframeExporter::ExtractorType::Material) > 0 ||
        ((int)extractTypes & (int)WarframeExporter::ExtractorType::Audio) > 0)
    {
        pkgNames.push_back("Misc");
        pkgNames.push_back("Misc_xx");
    }

    if (((int)extractTypes & (int)WarframeExporter::ExtractorType::Level) > 0)
    {
        pkgNames.push_back("AnimRetarget");
    }

    if (((int)extractTypes & (int)WarframeExporter::ExtractorType::Texture) > 0)
    {
        pkgNames.push_back("Texture");
        pkgNames.push_back("LightMap");
    }

    if (((int)extractTypes & (int)WarframeExporter::ExtractorType::Shader) > 0)
    {
        pkgNames.push_back("ShaderDx11");
        pkgNames.push_back("ShaderPermutationDx11");
        
        try
		{
			packages.getPackage("ShaderDx12");
			pkgNames.push_back("ShaderDx12");
			pkgNames.push_back("ShaderPermutationDx12");
		}
		catch (LotusLib::LotusException&) { }
    }

    return pkgNames;
}

void
UiExporter::swapToExtractButton()
{
    disconnect(this->ExtractButton, &QPushButton::clicked, this, &UiExporter::extractCancelButtonClicked);
    this->ExtractButton->setText("Extract");
    connect(this->ExtractButton, &QPushButton::clicked, this, &UiExporter::extractButtonClicked);
    this->ExtractButton->setEnabled(true);
}

void
UiExporter::swapToCancelButton()
{
    disconnect(this->ExtractButton, &QPushButton::clicked, this, &UiExporter::extractButtonClicked);
    this->ExtractButton->setText("Cancel");
    connect(this->ExtractButton, &QPushButton::clicked, this, &UiExporter::extractCancelButtonClicked);
}

void
UiExporter::aboutToClose()
{
    saveGeometry();
}

void
UiExporter::itemChanged()
{
    QTreeWidgetItem* item = this->treeWidget->currentItem();

    int itemType = item->type();
    
    if (itemType == TreeItemDirectory::QTreeWidgetItemType)
    {
        this->clearMetaData();
        this->clearPreview();
    }

    else if (itemType == TreeItemFile::QTreeWidgetItemType)
    {
        TreeItemFile* itemFile = static_cast<TreeItemFile*>(item);
        setMetadata(itemFile);
        setPreview(itemFile);
    }
}

void
UiExporter::setData(
        std::filesystem::path cachePath,
        std::filesystem::path exportPath,
        WarframeExporter::ExtractorType extractTypes,
        WarframeExporter::Shader::ShaderExportType shaderExportType,
        WarframeExporter::Texture::TextureExportType textureExportType    
)
{
    m_packages.setData(cachePath);
    m_cacheDirPath = cachePath;
    m_exportPath = exportPath / "Extracted";
    m_extractTypes = extractTypes;
    m_exportPkgNames = getPackageNames(m_packages, extractTypes);
    m_exporterDirectoryThread.setData(&m_packages, exportPath / "Extracted", extractTypes, m_exportPkgNames);
    m_exporterFileThread.setData(&m_packages, exportPath / "Extracted");
    m_previewManager.setData(&m_packages, {true, true, true});

    WarframeExporter::Model::ModelExtractor::getInstance()->m_indexVertexColors = false;
    WarframeExporter::Shader::ShaderExtractor::m_shaderExportType = shaderExportType;
    WarframeExporter::Texture::TextureExtractor::m_exportType = textureExportType;

    m_loading.initProgressBar(m_exportPkgNames, m_packages);
    m_loadingDialog.show();

    m_loadTreeThread.setData(m_exportPkgNames, m_extractTypes, m_packages, this->treeWidget, UiSettings::getInstance().getFilterFiles());
    m_loadTreeThread.start();
}

void
UiExporter::loadTreeFinished()
{
    m_loadingDialog.hide();
}

void
UiExporter::extractButtonClicked()
{
    QList<QTreeWidgetItem*> selectedTreeItems = this->treeWidget->selectedItems();

    if (selectedTreeItems.size() == 0)
    {
        QMessageBox errBox;
        errBox.critical(nullptr, "Error", "No folder/file selected");
        errBox.setFixedSize(500, 200);
        errBox.show();
        return;
    }

    QTreeWidgetItem* selectedItem = selectedTreeItems[0];
    int itemType = selectedItem->type();

    this->ExtractProgressBar->setValue(0);
    this->ExtractProgressBar->setMaximum(0);
    
    if (itemType == TreeItemDirectory::QTreeWidgetItemType)
    {
        TreeItemDirectory* itemCasted = static_cast<TreeItemDirectory*>(selectedItem);
        LotusLib::LotusPath internalPath = itemCasted->getFullInternalPath();
        this->extractDirectory(internalPath);
        swapToCancelButton();
    }

    else if (itemType == TreeItemFile::QTreeWidgetItemType)
    {
        TreeItemFile* itemCasted = static_cast<TreeItemFile*>(selectedItem);
        LotusLib::LotusPath internalPath = itemCasted->getQFullpath().toStdString();
        this->extractFile(internalPath, itemCasted->getPkg());
        this->ExtractButton->setEnabled(false);
    }
}

void
UiExporter::extractCancelButtonClicked()
{
    m_exporterDirectoryThread.extractCancelled();
    m_exporterDirectoryThread.wait();
    this->ExtractProgressBar->setFormat("Cancelled %v/%m");
    swapToExtractButton();
}

void
UiExporter::extractIndexingStarted()
{
    this->ExtractProgressBar->setMinimum(0);
    this->ExtractProgressBar->setMaximum(1);
    this->ExtractProgressBar->setFormat("Indexing...");
}

void
UiExporter::extractStart(int totalItems)
{
    this->ExtractProgressBar->setMinimum(0);
    this->ExtractProgressBar->setMaximum(totalItems);
    this->ExtractProgressBar->setFormat("Extracting %v/%m");
}

void
UiExporter::extractItemComplete(int curItemCount)
{
    this->ExtractProgressBar->setValue(curItemCount);
}

void
UiExporter::extractError(std::string msg)
{
    this->ExtractProgressBar->setFormat("Error");

    QMessageBox errBox;
    errBox.critical(nullptr, "Error During Extraction", msg.c_str());
    errBox.setFixedSize(500, 200);
    errBox.show();

    swapToExtractButton();

    return;
}

void
UiExporter::extractComplete()
{
    this->ExtractProgressBar->setFormat("Done %v/%m");\
    swapToExtractButton();
}