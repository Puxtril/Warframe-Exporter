#pragma once

#include "CachePairReader.h"
#include "ExporterLogger.h"
#include "BinaryReaderBuffered.h"
#include "ExporterExceptions.h"
#include "FileProperties.h"
#include "PackageCollection.h"
#include "EnumMapExtractor.h"
#include "BatchIterator.h"

#include <filesystem>

namespace WarframeExporter
{
	class BatchIteratorExport : public BatchIterator
	{
	public:
		BatchIteratorExport();

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

		bool existingFileIdentical(int64_t timestamp, const std::filesystem::path& outputPath);
		void writeFileProperties(const std::filesystem::path filePath, LotusLib::FileEntry& fileEntry);
	};
}
