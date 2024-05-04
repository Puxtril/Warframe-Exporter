#include "ExportSingleFile.h"

void
WarframeExporter::_FileExport::extractFile(LotusLib::PackagesReader& pkgs, const std::string& pkgName, LotusLib::FileEntry& fileEntry, Extractor* extractor, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
    processKnownFile(pkgs, pkgName, fileEntry, extractor, ensmalleningData, outputPath);
}

void
WarframeExporter::extractFile(LotusLib::PackagesReader& pkgs, const std::string& pkgName, const LotusLib::FileEntries::FileNode* fileNode, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
    static _FileExport fileExporter;

    LotusLib::FileEntry fileEntry = pkgs.getPackage(pkgName).getFile(fileNode, LotusLib::FileEntryReaderFlags::READ_COMMON_HEADER | LotusLib::FileEntryReaderFlags::READ_H_CACHE);

    Extractor* extractor = g_enumMapExtractor[fileEntry.commonHeader.type];

    if (extractor == nullptr)
    {
        throw unknown_format_error("File format not supported");
    }

    fileExporter.extractFile(pkgs, pkgName, fileEntry, extractor, ensmalleningData, outputPath);
}

void
WarframeExporter::extractFile(LotusLib::PackagesReader& pkgs, const std::string& pkgName, LotusLib::LotusPath& internalPath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
    const LotusLib::FileEntries::FileNode* fileNode = pkgs.getPackage(pkgName).getFileNode(internalPath);
    WarframeExporter::extractFile(pkgs, pkgName, fileNode, ensmalleningData, outputPath);
}
