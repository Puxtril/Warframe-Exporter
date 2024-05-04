#pragma once

#include "LotusLib.h"

#include <QtWidgets/QTreeWidget>

class TreeItemDirectory : public QTreeWidgetItem
{
    std::string m_fullInternalPath;

public:
    static constexpr int QTreeWidgetItemType = 1005;

    TreeItemDirectory(std::string fullInternalPath);
    TreeItemDirectory(QTreeWidgetItem* parentWidget, std::string fullInternalPath);

    const std::string& getFullInternalPath() const;
};

class TreeItemFile : public QTreeWidgetItem
{
    const LotusLib::FileEntries::FileNode* m_file;
    const std::string& m_pkg;

public:
    static constexpr int QTreeWidgetItemType = 1006;

    TreeItemFile(const LotusLib::FileEntries::FileNode* fileNode, const std::string& pkgName);
    TreeItemFile(QTreeWidgetItem* parentWidget, const LotusLib::FileEntries::FileNode* fileNode, const std::string& pkgName);

    QString getQName() const;
    QString getQFullpath() const;
    QString getQSize() const;
    QString getQCompressedSize() const;
    QString getQTimestamp() const;

    const std::string& getPkg() const;
    QString getQPkg() const;

private:
    std::string bytesToHumanReadable(int32_t size) const;
};