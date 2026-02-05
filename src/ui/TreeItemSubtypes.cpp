#include "ui/TreeItemSubtypes.h"

#define _CRT_SECURE_NO_WARNINGS

TreeItemDirectory::TreeItemDirectory(const LotusLib::DirNode* node)
    : QTreeWidgetItem(TreeItemDirectory::QTreeWidgetItemType), m_node(node)
{}

TreeItemDirectory::TreeItemDirectory(QTreeWidgetItem* parentWidget, const LotusLib::DirNode* node)
    : QTreeWidgetItem(parentWidget, TreeItemDirectory::QTreeWidgetItemType), m_node(node)
{}

const LotusLib::DirNode*
TreeItemDirectory::getNode() const
{
    return m_node;
}

TreeItemFile::TreeItemFile(const LotusLib::FileNode& fileNode, const std::string& pkgName)
    : QTreeWidgetItem(TreeItemFile::QTreeWidgetItemType), m_file(fileNode), m_pkg(pkgName)
{}

TreeItemFile::TreeItemFile(QTreeWidgetItem* parentWidget, const LotusLib::FileNode& fileNode, const std::string& pkgName)
    : QTreeWidgetItem(parentWidget, TreeItemFile::QTreeWidgetItemType), m_file(fileNode), m_pkg(pkgName)
{}

QString
TreeItemFile::getQName() const
{
    std::string fullPath = m_file.name;
    QString fullPathQ(fullPath.c_str());
    return fullPathQ;
}

QString
TreeItemFile::getQFullpath() const
{
    std::string fullPath = LotusLib::getFullPath(m_file);
    QString fullPathQ(fullPath.c_str());
    return fullPathQ;
}

QString
TreeItemFile::getQSize() const
{
    std::string compressedLen = bytesToHumanReadable(m_file.compLen);
    QString compressedLenQ(compressedLen.c_str());
    return compressedLenQ;
}

QString
TreeItemFile::getQCompressedSize() const
{
    std::string compressedLen = bytesToHumanReadable(m_file.compLen);
    QString compressedLenQ(compressedLen.c_str());
    return compressedLenQ;
}

QString
TreeItemFile::getQTimestamp() const
{
    char buffer[80];

    time_t epochTime = m_file.timeStamp / 10000000UL - 11644473600UL;
    const tm* timeInfo = gmtime(&std::max((time_t)0, epochTime));
    strftime(buffer, 80, "%Y/%m/%d %H:%M:%S", timeInfo);

    QString timeQ(buffer);
    return timeQ;
}

const std::string&
TreeItemFile::getPkg() const
{
    return m_pkg;
}

QString
TreeItemFile::getQPkg() const
{
    return QString(m_pkg.data());
}

std::string
TreeItemFile::bytesToHumanReadable(int32_t size) const
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