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
    connect(this->tabWidget, &QTabWidget::currentChanged, this, &UiExporter::itemChanged);
    connect(this->searchLineEdit, &QLineEdit::textChanged, this, &UiExporter::onSearchTextChanged);

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

    connect(&m_searchTimer, &QTimer::timeout, this, &UiExporter::filterTree);

    m_searchTimer.setSingleShot(true);

    m_previewManager.setupUis(this->Preview, this->verticalLayout_3, this->PreviewButtonsArea, this->horizontalLayout_5);
    m_metadataPreview.setupUis(this->MetadataCommonHeader, this->MetadataCompressedValue, this->MetadataDecompressedValue, this->MetadataModifiedValue);
    m_formatPreview.setupUis(this->Format, this->verticalLayout_6);
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
UiExporter::clearPreview()
{
    m_previewManager.clearPreview();
    m_metadataPreview.clearPreview();
    m_formatPreview.clearPreview();
}

void
UiExporter::setPreview(TreeItemFile* file)
{
    if (this->tabWidget->currentWidget() == this->Preview)
    {
        LotusLib::FileEntry fileEntry = m_packages.getPackage(file->getPkg()).value().getFile(file->getQFullpath().toStdString());
        m_previewManager.swapToFilePreview(fileEntry);
    }
    else if (this->tabWidget->currentWidget() == this->Metadata)
    {
        m_metadataPreview.clearPreview();
        m_metadataPreview.setData(&m_packages, file->getPkg(), file->getQFullpath().toStdString());
    }
    else if (this->tabWidget->currentWidget() == this->Format)
    {
        m_formatPreview.clearPreview();
        m_formatPreview.setData(&m_packages, file->getPkg(), file->getQFullpath().toStdString());
    }
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
        this->clearPreview();
    }

    else if (itemType == TreeItemFile::QTreeWidgetItemType)
    {
        TreeItemFile* itemFile = static_cast<TreeItemFile*>(item);
        setPreview(itemFile);
    }
}

void
UiExporter::setData(
        std::filesystem::path cachePath,
        std::filesystem::path exportPath,
        WarframeExporter::ExtractorType extractTypes,
        WarframeExporter::Shader::ShaderExportType shaderExportType,
        WarframeExporter::Texture::TextureExportType textureExportType,
        bool indexVertexColors,
        LotusLib::Game game
)
{
    m_packages.setData(cachePath, game);
    m_cacheDirPath = cachePath;
    m_exportPath = exportPath / "Extracted";
    m_extractTypes = extractTypes;
    m_exporterDirectoryThread.setData(&m_packages, exportPath, extractTypes);
    m_exporterFileThread.setData(&m_packages, exportPath);
    m_previewManager.setData(&m_packages);

    WarframeExporter::Model::ModelExtractor::getInstance()->m_indexVertexColors = indexVertexColors;
    WarframeExporter::Shader::ShaderExtractor::m_shaderExportType = shaderExportType;
    WarframeExporter::Texture::TextureExtractor::m_exportType = textureExportType;

    m_loading.initProgressBar(m_packages, extractTypes);
    m_loadingDialog.show();

    m_loadTreeThread.setData(m_extractTypes, m_packages, this->treeWidget, UiSettings::getInstance().getFilterFiles());
    m_loadTreeThread.start();
}

void
UiExporter::loadTreeFinished()
{
    buildSearchIndex(treeWidget->invisibleRootItem());
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

void UiExporter::buildSearchIndex(QTreeWidgetItem* parent)
{
    for (int i = 0; i < parent->childCount(); ++i) {
        QTreeWidgetItem* item = parent->child(i);
        QString text;
        if (item->type() == TreeItemFile::QTreeWidgetItemType) {
            TreeItemFile* fileItem = static_cast<TreeItemFile*>(item);
            text = fileItem->getQFullpath().toLower();
        } else if (item->type() == TreeItemDirectory::QTreeWidgetItemType) {
            TreeItemDirectory* dirItem = static_cast<TreeItemDirectory*>(item);
            text = QString::fromStdString(dirItem->getFullInternalPath()).toLower();
        }
        m_searchIndex.append(qMakePair(text, item));
        buildSearchIndex(item);
    }
}

void 
UiExporter::onSearchTextChanged(const QString& text) 
{
    m_searchTimer.start(300);
}

void 
UiExporter::filterTree() 
{
    const QString searchText = searchLineEdit->text().trimmed().toLower();
    const bool isSearching = !searchText.isEmpty();

    if (!isSearching) {
        for (const auto& pair : m_searchIndex) {
            pair.second->setHidden(false);
        }
        return;
    }

    for (const auto& pair : m_searchIndex) {
        const QString& itemText = pair.first;
        QTreeWidgetItem* item = pair.second;
        item->setHidden(true);
        item->setExpanded(false);
        if (itemText.contains(searchText)) {
            item->setHidden(false);
            QTreeWidgetItem* parent = item->parent();
            while (parent) {
                parent->setHidden(false);
                parent->setExpanded(true);
                parent = parent->parent();
            }
        }
    }
}