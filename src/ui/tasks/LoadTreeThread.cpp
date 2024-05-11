#include "ui/tasks/LoadTreeThread.h"

LoadTreeThread::LoadTreeThread()
    :   m_processedFileCount(0)
{
    QColor dirColor(QApplication::palette().text().color());
    dirColor.setAlpha(150);
    m_dirBrush = QBrush(dirColor);
}

void
LoadTreeThread::setData(
    std::vector<std::string> exportPkgNames,
    WarframeExporter::ExtractorType extractTypes,
    LotusLib::PackagesReader& pkgs,
    QTreeWidget* parentWidget,
    bool shouldFilterFiles
)
{
    m_pkgs = &pkgs;
    m_exportPkgNames = exportPkgNames;
    m_extractTypes = extractTypes;
    m_treeWidget = parentWidget;
    m_shouldFilterFiles = shouldFilterFiles;
}

void
LoadTreeThread::run()
{
    setupTree();
}

void
LoadTreeThread::setupTree()
{
    std::vector<QTreeWidgetItem*> topLevelItems;

    // Initial pass to get all directory names
    std::set<std::string> dirNamesInAllPkgs;
    for (size_t iPkg = 0; iPkg < m_exportPkgNames.size(); iPkg++)
    {
        std::string& curPkgName = m_exportPkgNames[iPkg];
        const LotusLib::FileEntries::DirNode* curEntry = m_pkgs->getPackage(curPkgName).getDirNode("/");

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

        for (int i = 0; i < static_cast<int>(curEntry->getDirCount()); i++)
        {
            const LotusLib::FileEntries::DirNode* curNode = curEntry->getChildDir(i);
            dirNamesInAllPkgs.insert(curNode->getName());
        }
    }

    // Build parameters for recursive call
    for (const std::string& curDirName : dirNamesInAllPkgs)
    {
        TreeItemDirectory* newDirWidget = nullptr;
        std::vector<const LotusLib::FileEntries::DirNode*> dirEntries;

        for (size_t iPkg = 0; iPkg < m_exportPkgNames.size(); iPkg++)
        {
            const std::string& curPkgName = m_exportPkgNames[iPkg];
            const LotusLib::FileEntries::DirNode* curEntry = m_pkgs->getPackage(curPkgName).getDirNode("/")->getChildDir(curDirName);

            // Ensure relation in pkgNames and curEntries
            if (curEntry == nullptr)
            {
                dirEntries.push_back(nullptr);
                continue;
            }

            if (newDirWidget == nullptr)
            {
                newDirWidget = new TreeItemDirectory(nullptr, curEntry->getFullPath());
                newDirWidget->setData(0, 0, curEntry->getName().c_str());
                newDirWidget->setForeground(0, m_dirBrush);
                topLevelItems.push_back(newDirWidget);
            }

            dirEntries.push_back(curEntry);
        }
        
        setupTreeRecursive(dirEntries, newDirWidget);
    }

    // Add top-level items from dummyRootWidget
    for (size_t i = 0; i < topLevelItems.size(); i++)
    {
        QTreeWidgetItem* curItem = topLevelItems[i];
        
        if (curItem->childCount() != 0)
            m_treeWidget->addTopLevelItem(curItem);
        else
            delete curItem;
    }

    m_treeWidget->sortItems(0, Qt::SortOrder::AscendingOrder);

    emit treeLoadingFinished();
}

void
LoadTreeThread::setupTreeRecursive(std::vector<const LotusLib::FileEntries::DirNode*> curEntries, QTreeWidgetItem* parentWidget)
{
    // 1. Collect all file names and put into parentWidget
    // 2. Initial pass to collect all unique directory names
    // Nasty but slightly more optimized this way
    std::set<std::string_view> dirNamesInAllPkgs;
    for (size_t iPkg = 0; iPkg < m_exportPkgNames.size(); iPkg++)
    {
        const LotusLib::FileEntries::DirNode* curEntry = curEntries[iPkg];

        if (curEntry == nullptr || (curEntry->getDirCount() == 0 && curEntry->getFileCount() == 0))
            continue;

        // Append file entries
        for (int i = 0; i < static_cast<int>(curEntry->getFileCount()); i++)
        {
            const LotusLib::FileEntries::FileNode* curNode = curEntry->getChildFile(i);
            incrementFileCounter();

            // Adds lots of extra processing time
            if (m_shouldFilterFiles)
            {   
                try
                {
                    int format = m_pkgs->getPackage(m_exportPkgNames[iPkg]).getFileFormat(curNode);
                    WarframeExporter::Extractor* extractor = WarframeExporter::g_enumMapExtractor[format];
                    if (extractor == nullptr)
                        continue;
                    WarframeExporter::ExtractorType curEntryType = extractor->getExtractorType();
                    if (((int)curEntryType & (int)m_extractTypes) == 0)
                    {
                        continue;
                    }
                }
                catch (std::exception&)
                {
                    continue;
                }
            }

            TreeItemFile* fileWidget = new TreeItemFile(parentWidget, curNode, m_exportPkgNames[iPkg]);
            fileWidget->setData(0, 0, curNode->getName().c_str());
        }

        // Collect directory names
        for (int i = 0; i < static_cast<int>(curEntry->getDirCount()); i++)
        {
            const LotusLib::FileEntries::DirNode* curNode = curEntry->getChildDir(i);
            dirNamesInAllPkgs.insert(curNode->getName());
        }
    }

    // Build parameters for recursive call
    for (const std::string_view& curDirName : dirNamesInAllPkgs)
    {
        TreeItemDirectory* newDirWidget = nullptr;
        std::vector<const LotusLib::FileEntries::DirNode*> dirEntries;
        size_t validDirCount = 0;

        for (size_t iPkg = 0; iPkg < m_exportPkgNames.size(); iPkg++)
        {
            // Ensure relation in m_viewPkgNames and curEntries
            if (curEntries[iPkg] == nullptr || (curEntries[iPkg]->getDirCount() == 0 && curEntries[iPkg]->getFileCount() == 0))
            {
                dirEntries.push_back(nullptr);
                continue;
            }

            const LotusLib::FileEntries::DirNode* curEntry = curEntries[iPkg]->getChildDir(std::string(curDirName));

            if (curEntry == nullptr || (curEntry->getDirCount() == 0 && curEntry->getFileCount() == 0))
            {
                dirEntries.push_back(nullptr);
                continue;
            }

            if (newDirWidget == nullptr)
            {
                newDirWidget = new TreeItemDirectory(parentWidget, curEntry->getFullPath());
                newDirWidget->setData(0, 0, curEntry->getName().c_str());
                newDirWidget->setForeground(0, m_dirBrush);
                validDirCount++;
            }

            dirEntries.push_back(curEntry);
        }

        // Base case!
        if (validDirCount == 0)
            continue;
        
        setupTreeRecursive(dirEntries, newDirWidget);

        if (newDirWidget->childCount() == 0)
            parentWidget->removeChild(newDirWidget);
    }
}

void
LoadTreeThread::incrementFileCounter()
{
    m_processedFileCount++;
    if (m_processedFileCount % 1000 == 0)
        emit treeItemsLoaded(m_processedFileCount);
}
