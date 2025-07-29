#pragma once

#include "CachePair.h"
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
			LotusLib::FileEntry& fileEntry,
			Extractor* extractor,
			const std::filesystem::path& outputPath,
			ExtractOptions options
		) override;
		
		void processUnknownFile(
			LotusLib::PackagesReader& pkgs,
			LotusLib::FileEntry& fileEntry,
			const std::filesystem::path& outputPath
		) override;
		
		void processSkipFile(
			LotusLib::PackagesReader& pkgs,
			LotusLib::FileEntry& fileEntry,
			const Extractor* extractor
		) override;

		bool existingFileIdentical(int64_t timestamp, const std::filesystem::path& outputPath);
		void writeFileProperties(const std::filesystem::path filePath, LotusLib::FileEntry& fileEntry);
	};
}
