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
    setupTree(extractorTypes);
}

void
UiExporter::setupTree(WarframeExporter::ExtractorType extractorTypes)
{
    LotusLib::PackageReader pkg = m_packages.getPackage("Misc");
    LotusLib::DirMeta rootNode = pkg.getDirMeta("/");

    for (int i = 0; i < rootNode.getDirCount(); i++)
    {
        LotusLib::DirMeta curNode = rootNode.getChildDir(i);

        TreeItemDirectory* dirWidget = new TreeItemDirectory(curNode);
        dirWidget->setData(0, 0, curNode.getName().c_str());
        dirWidget->setForeground(0, m_dirBrush);
        treeWidget->addTopLevelItem(dirWidget);
        setupTreeRecursive(dirWidget, curNode);
    }

    treeWidget->sortItems(0, Qt::SortOrder::AscendingOrder);
}

void
UiExporter::setupTreeRecursive(QTreeWidgetItem* parentWidget, LotusLib::DirMeta& parentEntry)
{
    for (int i = 0; i < parentEntry.getFileCount(); i++)
    {
        LotusLib::FileMeta curNode = parentEntry.getChildFile(i);

        TreeItemFile* dirWidget = new TreeItemFile(parentWidget, curNode);
        dirWidget->setData(0, 0, curNode.getName().c_str());
    }
    for (int i = 0; i < parentEntry.getDirCount(); i++)
    {
        LotusLib::DirMeta curNode = parentEntry.getChildDir(i);

        TreeItemDirectory* dirWidget = new TreeItemDirectory(parentWidget, curNode);
        dirWidget->setData(0, 0, curNode.getName().c_str());
        dirWidget->setForeground(0, m_dirBrush);
        setupTreeRecursive(dirWidget, curNode);
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
}

void
UiExporter::setMetadata(TreeItemFile* file)
{
    this->NameData->setText(file->getName());
    this->FullPathData->setText(file->getFullpath());
    this->SizeData->setText(file->getSize());
    this->CompressedLengthData->setText(file->getCompressedSize());
    this->TimestampData->setText(file->getTimestamp());
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