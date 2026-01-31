#pragma once

#include "ExporterLogger.h"
#include "ExporterExceptions.h"
#include "FileProperties.h"
#include "LotusLib/PackageCollection.h"
#include "EnumMapExtractor.h"

namespace WarframeExporter
{
    int getExtractableCount(
        const LotusLib::PackageCollection& pkgsDir,
        const std::string& basePath,
        ExtractorType types,
        LotusLib::Game game
    );

    void extractAllFiles(
        const LotusLib::PackageCollection& pkgsDir,
        const LotusLib::PackagesBin& pkgsBin,
        const std::filesystem::path& outputPath,
        const std::string& basePath,
        ExtractorType types,
        ExtractOptions options
    );

    bool tryExtractFile(
        const LotusLib::PackageCollection& pkgs,
        const LotusLib::PackagesBin& pkgsBin,
        const std::string& intPath,
        const std::filesystem::path outPath,
        WarframeExporter::ExtractorType types,
        WarframeExporter::ExtractOptions options
    );

    void extractFile(
        const LotusLib::PackageCollection& pkgs,
        const LotusLib::PackagesBin& pkgsBin,
        LotusLib::FileEntry& fileEntry,
        Extractor* extractor,
        const std::filesystem::path& outputPath,
        const ExtractOptions options
    );

    bool existingFileIdentical(int64_t timestamp, const std::filesystem::path& filePath);
    void writeFileProperties(int64_t timestamp, const std::filesystem::path filePath);
}