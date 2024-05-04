#pragma once

#include "Extractor.h"
#include "EnumMapExtractor.h"
#include "Ensmallening.hpp"
#include "FileNode.h"
#include "LotusLib.h"
#include "ExporterExceptions.h"
#include "BatchIteratorExport.h"
#include "LotusPath.h"

namespace WarframeExporter
{
    class _FileExport : private BatchIteratorExport
    {
    public:
        void extractFile(LotusLib::PackagesReader& pkgs,
			const std::string& pkgName,
			LotusLib::FileEntry& fileEntry,
			Extractor* extractor,
			const Ensmallening& ensmalleningData,
			const std::filesystem::path& outputPath
		);
    };

	void
    extractFile(LotusLib::PackagesReader& pkgs,
			const std::string& pkgName,
			const LotusLib::FileEntries::FileNode* fileNode,
			const Ensmallening& ensmalleningData,
			const std::filesystem::path& outputPath
    );

    void
    extractFile(LotusLib::PackagesReader& pkgs,
			const std::string& pkgName,
			LotusLib::LotusPath& internalPath,
			const Ensmallening& ensmalleningData,
			const std::filesystem::path& outputPath
    );
}