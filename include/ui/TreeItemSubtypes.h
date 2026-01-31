#pragma once

#include "LotusLib/TOCTree.h"
#include "LotusLib/Utils.h"

#include <QtWidgets/QTreeWidget>

class TreeItemDirectory : public QTreeWidgetItem
{
    const LotusLib::DirNode* m_node;

public:
    static constexpr int QTreeWidgetItemType = 1005;

    TreeItemDirectory(const LotusLib::DirNode* node);
    TreeItemDirectory(QTreeWidgetItem* parentWidget, const LotusLib::DirNode* node);

    const LotusLib::DirNode* getNode() const;
};

class TreeItemFile : public QTreeWidgetItem
{
    const LotusLib::FileNode& m_file;
    const std::string& m_pkg;

public:
    static constexpr int QTreeWidgetItemType = 1006;

    TreeItemFile(const LotusLib::FileNode& fileNode, const std::string& pkgName);
    TreeItemFile(QTreeWidgetItem* parentWidget, const LotusLib::FileNode& fileNode, const std::string& pkgName);

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