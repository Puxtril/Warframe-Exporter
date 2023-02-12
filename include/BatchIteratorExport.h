#pragma once

#include "CachePairReader.h"
#include "ExporterLogger.h"
#include "BinaryReaderBase.h"
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
		BatchIteratorExport(LotusLib::PackageCollection<LotusLib::CachePairReader>* package, const Ensmallening& ensmalleningData, std::string baseOutputPath);

	protected:
		void processKnownFile(const std::string& packageName, const LotusLib::LotusPath& internalPath, BinaryReaderBuffered* hReader, const LotusLib::CommonHeader& header, Extractor* extractor) override;
		void processUnknownFile(const LotusLib::LotusPath& internalPath, const LotusLib::CommonHeader& header, const LotusLib::FileEntries::FileNode* file) override;
		void processSkipFile(const LotusLib::LotusPath& internalPath, const LotusLib::CommonHeader& header, const LotusLib::FileEntries::FileNode* file, const Extractor* extractor) override;

		bool existingFileIdentical(const LotusLib::LotusPath& internalPath, const std::string& outputPath, const std::shared_ptr<LotusLib::CachePairReader> curPair, const std::string& packageName);
		void writeFileProperties(const std::string filePath, const LotusLib::LotusPath internalPath, const std::string& packageName);
	};
}
