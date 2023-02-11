#pragma once

#include "CachePairReader.h"
#include "ExporterLogger.h"
#include "BinaryReaderExceptions.h"
#include "BinaryReaderBuffered.h"
#include "ExporterExceptions.h"
#include "EnumMapExtractor.h"
#include "BatchIterator.h"
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
		BatchIteratorDebug(LotusLib::PackageCollection<LotusLib::CachePairReader>* package, const Ensmallening& ensmallData, std::string baseOutputPath);

		void printEnumCounts(const std::string& package);

	protected:
		void processKnownFile(const std::string& packageName, const std::string& internalPath, BinaryReaderBuffered* hReader, const LotusLib::CommonHeader& header, Extractor* extractor) override;
		void processUnknownFile(const std::string& internalPath, const LotusLib::CommonHeader& header, const LotusLib::FileEntries::FileNode* file) override;
		void processSkipFile(const std::string& internalPath, const LotusLib::CommonHeader& header, const LotusLib::FileEntries::FileNode* file, const Extractor* extractor) override;

		void writeAllDebugs(const std::string& packageName, const std::string& internalPath);
	};
}
