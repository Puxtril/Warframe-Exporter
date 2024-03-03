#include "ui/UIExporter.h"

UiExporter::UiExporter()
{
    QColor dirColor(QApplication::palette().text().color());
    dirColor.setAlpha(150);
    m_dirBrush = QBrush(dirColor);
}

void
UiExporter::setup(QMainWindow *MainWindow)
{
    Ui_MainWindow::setupUi(MainWindow);
    connect(this->treeWidget, &QTreeWidget::itemClicked, this, &UiExporter::itemClicked);
    connect(this->ExtractButton, &QPushButton::clicked, this, &UiExporter::extractButtonClicked);
    // Exporter
    connect(&m_exporterThread, &ExporterThread::extractIndexingStarted, this, &UiExporter::extractIndexingStarted);
    connect(&m_exporterThread, &ExporterThread::extractStart, this, &UiExporter::extractStart);
    connect(&m_exporterThread, &ExporterThread::extractItemComplete, this, &UiExporter::extractItemComplete);
    connect(&m_exporterThread, &ExporterThread::extractError, this, &UiExporter::extractError);
    connect(&m_exporterThread, &ExporterThread::extractComplete, this, &UiExporter::extractComplete);
}

void
UiExporter::setupTree()
{
    std::vector<QTreeWidgetItem*> topLevelItems;

    // Initial pass to get all directory names
    std::set<std::string> dirNamesInAllPkgs;
    for (size_t iPkg = 0; iPkg < m_viewPkgNames.size(); iPkg++)
    {
        std::string& curPkgName = m_viewPkgNames[iPkg];
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

        for (int i = 0; i < curEntry.getDirCount(); i++)
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

        for (size_t iPkg = 0; iPkg < m_viewPkgNames.size(); iPkg++)
        {
            const std::string& curPkgName = m_viewPkgNames[iPkg];
            LotusLib::DirMeta curEntry = m_packages.getPackage(curPkgName).getDirMeta("/").getChildDir(std::string(curDirName));

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
    for (size_t iPkg = 0; iPkg < m_viewPkgNames.size(); iPkg++)
    {
        LotusLib::DirMeta& curEntry = curEntries[iPkg];

        if (curEntry.isEmpty())
            continue;

        // Append file entries
        for (int i = 0; i < curEntry.getFileCount(); i++)
        {
            LotusLib::FileMeta curNode = curEntry.getChildFile(i);

            TreeItemFile* fileWidget = new TreeItemFile(parentWidget, curNode, m_viewPkgNames[iPkg]);
            fileWidget->setData(0, 0, curNode.getName().c_str());
        }

        // Collect directory names
        for (int i = 0; i < curEntry.getDirCount(); i++)
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

        for (size_t iPkg = 0; iPkg < m_viewPkgNames.size(); iPkg++)
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
UiExporter::extractDirectory(LotusLib::LotusPath internalPath)
{
    m_exporterThread.setInternalPath(internalPath);
    m_exporterThread.start();
}

void
UiExporter::extractFile(LotusLib::LotusPath internalPath)
{
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

    return pkgNames;
}

void
UiExporter::swapToExtractButton()
{
    disconnect(this->ExtractButton, &QPushButton::clicked, this, &UiExporter::extractCancelButtonClicked);
    this->ExtractButton->setText("Extract");
    connect(this->ExtractButton, &QPushButton::clicked, this, &UiExporter::extractButtonClicked);
}

void
UiExporter::swapToCancelButton()
{
    disconnect(this->ExtractButton, &QPushButton::clicked, this, &UiExporter::extractButtonClicked);
    this->ExtractButton->setText("Cancel");
    connect(this->ExtractButton, &QPushButton::clicked, this, &UiExporter::extractCancelButtonClicked);
}


void
UiExporter::itemClicked(QTreeWidgetItem *item, int column)
{
    int itemType = item->type();
    
    if (itemType == TreeItemDirectory::QTreeWidgetItemType)
    {
        this->clearMetaData();
    }

    else if (itemType == TreeItemFile::QTreeWidgetItemType)
    {
        TreeItemFile* itemFile = static_cast<TreeItemFile*>(item);
        setMetadata(itemFile);
    }
}

void
UiExporter::setData(std::filesystem::path cachePath, std::filesystem::path exportPath, WarframeExporter::ExtractorType viewTypes, WarframeExporter::ExtractorType extractTypes)
{
    m_packages.setData(cachePath);
    m_cacheDirPath = cachePath;
    m_exportPath = exportPath;
    m_viewTypes = viewTypes;
    m_extractTypes = extractTypes;
    m_viewPkgNames = getPackageNames(viewTypes);
    m_exportPkgNames = getPackageNames(extractTypes);
    m_exporterThread.setData(cachePath, exportPath, extractTypes, m_exportPkgNames);

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
    }

    else if (itemType == TreeItemFile::QTreeWidgetItemType)
    {
        QMessageBox errBox;
        errBox.critical(nullptr, "Error", "File extraction not currently supported");
        errBox.setFixedSize(500, 200);
        errBox.show();
        return;
    }

    swapToCancelButton();
}

void
UiExporter::extractCancelButtonClicked()
{
    //m_exporterThread.exit();
    m_exporterThread.extractCancelled();
    m_exporterThread.wait();
    this->ExtractProgressBar->setFormat("Cancelled %v/%m");
    swapToExtractButton();
}

void
UiExporter::extractIndexingStarted()
{
    this->ExtractProgressBar->setFormat("Indexing");
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