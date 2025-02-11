#include "ExportSingleFile.h"

void
WarframeExporter::_FileExport::extractFile(LotusLib::PackagesReader& pkgs, const std::string& pkgName, LotusLib::FileEntry& fileEntry, Extractor* extractor, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
    processKnownFile(pkgs, pkgName, fileEntry, extractor, ensmalleningData, outputPath);
}

void
WarframeExporter::extractFile(LotusLib::PackagesReader& pkgs, const std::string& pkgName, const LotusLib::FileEntries::FileNode* fileNode, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath, LotusLib::Game game)
{
    static _FileExport fileExporter;

    std::optional<LotusLib::PackageReader> pkgReader = pkgs.getPackage(pkgName);
    if (!pkgReader)
        throw std::runtime_error("Package does not exist");
    
    LotusLib::FileEntry fileEntry = pkgReader->getFile(fileNode, LotusLib::FileEntryReaderFlags::READ_COMMON_HEADER | LotusLib::FileEntryReaderFlags::READ_H_CACHE);

    Extractor* extractor = g_enumMapExtractor.at(game, pkgReader->getPkgCategory(), fileEntry.commonHeader.type);

    if (extractor == nullptr)
    {
        throw unknown_format_error("File format not supported");
    }

    fileExporter.extractFile(pkgs, pkgName, fileEntry, extractor, ensmalleningData, outputPath);
}

void
WarframeExporter::extractFile(LotusLib::PackagesReader& pkgs, const std::string& pkgName, LotusLib::LotusPath& internalPath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath, LotusLib::Game game)
{
    std::optional<LotusLib::PackageReader> pkgReader = pkgs.getPackage(pkgName);
    if (!pkgReader)
        throw std::runtime_error("Package does not exist");

    const LotusLib::FileEntries::FileNode* fileNode = pkgReader->getFileNode(internalPath);
    WarframeExporter::extractFile(pkgs, pkgName, fileNode, ensmalleningData, outputPath, game);
}
