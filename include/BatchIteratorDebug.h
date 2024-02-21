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
	protected:
		std::filesystem::path m_outDebugPath;

	public:
		BatchIteratorDebug(const std::filesystem::path& pkgsDir, const Ensmallening& ensmallData, std::filesystem::path baseOutputPath);

		void printEnumCounts(const std::string& package, const LotusLib::LotusPath& internalPath);
		void writeAllDebugs(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry);

	protected:
		void processKnownFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, Extractor* extractor) override;
		void processUnknownFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry) override;
		void processSkipFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, const Extractor* extractor) override;

	};
}
