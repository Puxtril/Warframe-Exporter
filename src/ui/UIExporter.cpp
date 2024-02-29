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
}

void
UiExporter::setCacheAndLoad(const std::filesystem::path& cachePath, WarframeExporter::ExtractorType extractorTypes)
{
    m_packages.setData(cachePath);
    m_extractorTypes = extractorTypes;
    m_pkgNames = getPackageNames();
    setupTree();
}

void
UiExporter::setupTree()
{
    std::vector<QTreeWidgetItem*> topLevelItems;

    // Initial pass to get all directory names
    std::set<std::string> dirNamesInAllPkgs;
    for (size_t iPkg = 0; iPkg < m_pkgNames.size(); iPkg++)
    {
        std::string& curPkgName = m_pkgNames[iPkg];
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

        for (size_t iPkg = 0; iPkg < m_pkgNames.size(); iPkg++)
        {
            const std::string& curPkgName = m_pkgNames[iPkg];
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
        
        setupTreeRecursive(m_pkgNames, dirEntries, newDirWidget);
    }

    // Add top-level items from dummyRootWidget
    for (QTreeWidgetItem* curItem : topLevelItems)
    {
        treeWidget->addTopLevelItem(curItem);
    }

    treeWidget->sortItems(0, Qt::SortOrder::AscendingOrder);
}

void
UiExporter::setupTreeRecursive(const std::vector<std::string>& pkgNames, std::vector<LotusLib::DirMeta> curEntries, QTreeWidgetItem* parentWidget)
{
    // 1. Collect all file names and put into parentWidget
    // 2. Initial pass to collect all unique directory names
    // Nasty but slightly more optimized this way
    std::set<std::string_view> dirNamesInAllPkgs;
    for (size_t iPkg = 0; iPkg < pkgNames.size(); iPkg++)
    {
        LotusLib::DirMeta& curEntry = curEntries[iPkg];

        if (curEntry.isEmpty())
            continue;

        // Append file entries
        for (int i = 0; i < curEntry.getFileCount(); i++)
        {
            LotusLib::FileMeta curNode = curEntry.getChildFile(i);

            TreeItemFile* fileWidget = new TreeItemFile(parentWidget, curNode, pkgNames[iPkg]);
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

        for (size_t iPkg = 0; iPkg < pkgNames.size(); iPkg++)
        {
            // Ensure relation in pkgNames and curEntries
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
        
        setupTreeRecursive(pkgNames, dirEntries, newDirWidget);
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

std::vector<std::string>
UiExporter::getPackageNames() const
{
    std::vector<std::string> pkgNames;
    if (((int)m_extractorTypes & (int)WarframeExporter::ExtractorType::Model) > 0 ||
        ((int)m_extractorTypes & (int)WarframeExporter::ExtractorType::Material) > 0 ||
        ((int)m_extractorTypes & (int)WarframeExporter::ExtractorType::Audio) > 0)
    {
        pkgNames.push_back("Misc");
    }

    if (((int)m_extractorTypes & (int)WarframeExporter::ExtractorType::Level) > 0)
    {
        pkgNames.push_back("AnimRetarget");
    }

    if (((int)m_extractorTypes & (int)WarframeExporter::ExtractorType::Texture) > 0)
    {
        pkgNames.push_back("Texture");
    }

    return pkgNames;
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
UiExporter::setData(std::filesystem::path cachePath, WarframeExporter::ExtractorType extractorTypes)
{
    setCacheAndLoad(cachePath, extractorTypes);
}