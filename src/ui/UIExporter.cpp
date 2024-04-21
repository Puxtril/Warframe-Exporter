#include "ui/UIExporter.h"

UiExporter::UiExporter()
{
    QColor dirColor(QApplication::palette().text().color());
    dirColor.setAlpha(150);
    m_dirBrush = QBrush(dirColor);
}

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
UiExporter::setupTree()
{
    std::vector<QTreeWidgetItem*> topLevelItems;

    // Initial pass to get all directory names
    std::set<std::string> dirNamesInAllPkgs;
    for (size_t iPkg = 0; iPkg < m_exportPkgNames.size(); iPkg++)
    {
        std::string& curPkgName = m_exportPkgNames[iPkg];
        LotusLib::DirMeta curEntry = m_packages.getPackage(curPkgName).getDirMeta("/");

        // Purposefully commented out because these clutter the root directory
        // Misc package has lots of root files that can't be extracted
        /*
        // Append file entries
        for (int i = 0; i < curEntry.getFileCount(); i++)
        {
            LotusLib::FileMeta curNode = curEntry.getChildFile(i);

            TreeItemFile* fileWidget = new TreeItemFile(curNode, m_pkgNames[iPkg]);
            fileWidget->setData(0, 0, curNode.getName().c_str());
            topLevelItems.push_back(fileWidget);
        }
        */

        for (int i = 0; i < static_cast<int>(curEntry.getDirCount()); i++)
        {
            LotusLib::DirMeta curNode = curEntry.getChildDir(i);
            dirNamesInAllPkgs.insert(curNode.getName());
        }
    }

    // Build parameters for recursive call
    for (const std::string& curDirName : dirNamesInAllPkgs)
    {
        TreeItemDirectory* newDirWidget = nullptr;
        std::vector<LotusLib::DirMeta> dirEntries;

        for (size_t iPkg = 0; iPkg < m_exportPkgNames.size(); iPkg++)
        {
            const std::string& curPkgName = m_exportPkgNames[iPkg];
            LotusLib::DirMeta curEntry = m_packages.getPackage(curPkgName).getDirMeta("/").getChildDir(curDirName);

            // Ensure relation in pkgNames and curEntries
            if (curEntry.isEmpty())
            {
                dirEntries.push_back(LotusLib::DirMeta());
                continue;
            }

            if (newDirWidget == nullptr)
            {
                newDirWidget = new TreeItemDirectory(nullptr, curEntry.getFullPath());
                newDirWidget->setData(0, 0, curEntry.getName().c_str());
                newDirWidget->setForeground(0, m_dirBrush);
                topLevelItems.push_back(newDirWidget);
            }

            dirEntries.push_back(curEntry);
        }
        
        setupTreeRecursive(dirEntries, newDirWidget);
    }

    // Add top-level items from dummyRootWidget
    for (QTreeWidgetItem* curItem : topLevelItems)
    {
        treeWidget->addTopLevelItem(curItem);
    }

    treeWidget->sortItems(0, Qt::SortOrder::AscendingOrder);
}

void
UiExporter::setupTreeRecursive(std::vector<LotusLib::DirMeta> curEntries, QTreeWidgetItem* parentWidget)
{
    // 1. Collect all file names and put into parentWidget
    // 2. Initial pass to collect all unique directory names
    // Nasty but slightly more optimized this way
    std::set<std::string_view> dirNamesInAllPkgs;
    for (size_t iPkg = 0; iPkg < m_exportPkgNames.size(); iPkg++)
    {
        LotusLib::DirMeta& curEntry = curEntries[iPkg];

        if (curEntry.isEmpty())
            continue;

        // Append file entries
        for (int i = 0; i < static_cast<int>(curEntry.getFileCount()); i++)
        {
            LotusLib::FileMeta curNode = curEntry.getChildFile(i);

            TreeItemFile* fileWidget = new TreeItemFile(parentWidget, curNode, m_exportPkgNames[iPkg]);
            fileWidget->setData(0, 0, curNode.getName().c_str());
        }

        // Collect directory names
        for (int i = 0; i < static_cast<int>(curEntry.getDirCount()); i++)
        {
            LotusLib::DirMeta curNode = curEntry.getChildDir(i);
            dirNamesInAllPkgs.insert(curNode.getName());
        }
    }

    // Build parameters for recursive call
    for (const std::string_view& curDirName : dirNamesInAllPkgs)
    {
        TreeItemDirectory* newDirWidget = nullptr;
        std::vector<LotusLib::DirMeta> dirEntries;
        size_t validDirCount = 0;

        for (size_t iPkg = 0; iPkg < m_exportPkgNames.size(); iPkg++)
        {
            // Ensure relation in m_viewPkgNames and curEntries
            if (curEntries[iPkg].isEmpty())
            {
                dirEntries.push_back(LotusLib::DirMeta());
                continue;
            }

            LotusLib::DirMeta curEntry = curEntries[iPkg].getChildDir(std::string(curDirName));

            if (curEntry.isEmpty())
            {
                dirEntries.push_back(LotusLib::DirMeta());
                continue;
            }

            if (newDirWidget == nullptr)
            {
                newDirWidget = new TreeItemDirectory(parentWidget, curEntry.getFullPath());
                newDirWidget->setData(0, 0, curEntry.getName().c_str());
                newDirWidget->setForeground(0, m_dirBrush);
                validDirCount++;
            }

            dirEntries.push_back(curEntry);
        }

        // Base case!
        if (validDirCount == 0)
            return;
        
        setupTreeRecursive(dirEntries, newDirWidget);
    }
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
UiExporter::getPackageNames(WarframeExporter::ExtractorType extractTypes)
{
    std::vector<std::string> pkgNames;
    if (((int)extractTypes & (int)WarframeExporter::ExtractorType::Model) > 0 ||
        ((int)extractTypes & (int)WarframeExporter::ExtractorType::Material) > 0 ||
        ((int)extractTypes & (int)WarframeExporter::ExtractorType::Audio) > 0)
    {
        pkgNames.push_back("Misc");
    }

    if (((int)extractTypes & (int)WarframeExporter::ExtractorType::Level) > 0)
    {
        pkgNames.push_back("AnimRetarget");
    }

    if (((int)extractTypes & (int)WarframeExporter::ExtractorType::Texture) > 0)
    {
        pkgNames.push_back("Texture");
    }

    if (((int)extractTypes & (int)WarframeExporter::ExtractorType::Shader) > 0)
    {
        pkgNames.push_back("ShaderDx11");
        pkgNames.push_back("ShaderPermutationDx11");
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
    m_exportPkgNames = getPackageNames(extractTypes);
    m_exporterDirectoryThread.setData(&m_packages, exportPath / "Extracted", extractTypes, m_exportPkgNames);
    m_exporterFileThread.setData(&m_packages, exportPath / "Extracted");
    m_previewManager.setData(&m_packages, {true, true, true});

    WarframeExporter::Model::ModelExtractor::getInstance()->m_indexVertexColors = false;
    WarframeExporter::Shader::ShaderExtractor::m_shaderExportType = shaderExportType;
    WarframeExporter::Texture::TextureExtractor::m_exportType = textureExportType;

    setupTree();
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