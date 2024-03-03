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
	protected:
		std::filesystem::path m_outExportPath;

	public:
		BatchIteratorExport(const std::filesystem::path& pkgsDir, const Ensmallening& ensmalleningData, std::filesystem::path baseOutputPath);

	protected:
		void processKnownFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, Extractor* extractor) override;
		void processUnknownFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry) override;
		void processSkipFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, const Extractor* extractor) override;

		bool existingFileIdentical(int64_t timestamp, const std::filesystem::path& outputPath);
		void writeFileProperties(const std::filesystem::path filePath, LotusLib::FileEntry& fileEntry);
	};
}
