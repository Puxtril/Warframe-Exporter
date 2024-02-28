#include "ui/TreeItemSubtypes.h"

TreeItemDirectory::TreeItemDirectory(LotusLib::DirMeta dirMeta)
    : m_dir(dirMeta), QTreeWidgetItem(TreeItemDirectory::QTreeWidgetItemType)
{}

TreeItemDirectory::TreeItemDirectory(QTreeWidgetItem* parentWidget, LotusLib::DirMeta dirMeta)
    : m_dir(dirMeta), QTreeWidgetItem(parentWidget, TreeItemDirectory::QTreeWidgetItemType)
{}

const LotusLib::DirMeta&
TreeItemDirectory::getDir()
{
    return m_dir;
}

TreeItemFile::TreeItemFile(LotusLib::FileMeta fileMeta)
    : m_file(fileMeta), QTreeWidgetItem(TreeItemFile::QTreeWidgetItemType)
{}

TreeItemFile::TreeItemFile(QTreeWidgetItem* parentWidget, LotusLib::FileMeta fileMeta)
    : m_file(fileMeta), QTreeWidgetItem(parentWidget, TreeItemFile::QTreeWidgetItemType)
{}

QString
TreeItemFile::getName()
{
    std::string fullPath = m_file.getName();
    QString fullPathQ(fullPath.c_str());
    return std::move(fullPathQ);
}

QString
TreeItemFile::getFullpath()
{
    std::string fullPath = m_file.getFullPath();
    QString fullPathQ(fullPath.c_str());
    return std::move(fullPathQ);
}

QString
TreeItemFile::getSize()
{
    std::string compressedLen = bytesToHumanReadable(m_file.getLen());
    QString compressedLenQ(compressedLen.c_str());
    return std::move(compressedLenQ);
}

QString
TreeItemFile::getCompressedSize()
{
    std::string compressedLen = bytesToHumanReadable(m_file.getCompLen());
    QString compressedLenQ(compressedLen.c_str());
    return std::move(compressedLenQ);
}

QString
TreeItemFile::getTimestamp()
{
    char buffer[80];

    time_t epochTime = m_file.getTimeStamp() / 10000000UL - 11644473600UL;
    const tm* timeInfo = gmtime(&std::max((time_t)0, epochTime));
    strftime(buffer, 80, "%Y/%m/%d %H:%M:%S", timeInfo);

    QString timeQ(buffer);
    return std::move(timeQ);
}

std::string
TreeItemFile::bytesToHumanReadable(int32_t size)
{
    static const std::string prefixes[4] = {"B", "KB", "MB", "GB"};

    int prefixLevel = 0;
    float sizeCut = size;

    while (sizeCut > 1024.0F && prefixLevel < 3)
    {
        sizeCut /= 1024.0F;
        prefixLevel++;
    }

    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << sizeCut << prefixes[prefixLevel];
    
    return stream.str();
}