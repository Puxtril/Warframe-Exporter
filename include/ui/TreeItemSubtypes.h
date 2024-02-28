#pragma once

#include "LotusLib.h"

#include <QTreeWidget>

class TreeItemDirectory : public QTreeWidgetItem
{
    LotusLib::DirMeta m_dir;

public:
    static constexpr int QTreeWidgetItemType = 1005;

    TreeItemDirectory(LotusLib::DirMeta dirMeta);
    TreeItemDirectory(QTreeWidgetItem* parentWidget, LotusLib::DirMeta dirMeta);

    const LotusLib::DirMeta& getDir();
};

class TreeItemFile : public QTreeWidgetItem
{
    LotusLib::FileMeta m_file;

public:
    static constexpr int QTreeWidgetItemType = 1006;

    TreeItemFile(LotusLib::FileMeta fileMeta);
    TreeItemFile(QTreeWidgetItem* parentWidget, LotusLib::FileMeta fileMeta);

    QString getName();
    QString getFullpath();
    QString getSize();
    QString getCompressedSize();
    QString getTimestamp();

private:
    std::string bytesToHumanReadable(int32_t size);
};