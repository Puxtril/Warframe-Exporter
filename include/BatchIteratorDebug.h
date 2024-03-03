#pragma once

#include "CachePairReader.h"
#include "ExporterLogger.h"
#include "BinaryReaderExceptions.h"
#include "BinaryReaderBuffered.h"
#include "ExporterExceptions.h"
#include "EnumMapExtractor.h"
#include "BatchIterator.h"
#include "LotusPath.h"
#include "PackageCollection.h"

#include <vector>
#include <map>
#include <iostream>
#include <filesystem>

namespace WarframeExporter
{
	class BatchIteratorDebug : public BatchIterator
	{
	public:
		BatchIteratorDebug();

		void printEnumCounts(LotusLib::PackageReader& pkg, const LotusLib::LotusPath& internalPath);
		void writeAllDebugs(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, const std::filesystem::path& baseOutputPath);

	protected:
		void processKnownFile(
			LotusLib::PackagesReader& pkgs,
			const std::string& pkgName,
			LotusLib::FileEntry& fileEntry,
			Extractor* extractor,
			const Ensmallening& ensmalleningData,
			const std::filesystem::path& outputPath
		) override;
		
		void processUnknownFile(
			LotusLib::PackagesReader& pkgs,
			const std::string& pkgName,
			LotusLib::FileEntry& fileEntry,
			const Ensmallening& ensmalleningData,
			const std::filesystem::path& outputPath
		) override;
		
		void processSkipFile(
			LotusLib::PackagesReader& pkgs,
			const std::string& pkgName,
			LotusLib::FileEntry& fileEntry,
			const Extractor* extractor
		) override;

	};
}
