#pragma once

#include "ExporterLogger.h"
#include "BinaryReaderBase.h"
#include "CommonHeader.h"
#include "Ensmallening.hpp"
#include "ExportPathManager.h"
#include "PackageCollection.h"
#include "ExporterExceptions.h"
#include "FileProperties.h"
#include "EnumMapExtractor.h"

#include <filesystem>

namespace WarframeExporter
{
	class BatchIterator
	{
	protected:
		LotusLib::PackageCollection<LotusLib::CachePairReader>* m_package;
		const Ensmallening m_ensmalleningData;
		ExportPathManager m_pathManager;
		Logger& m_logger;

	public:
		BatchIterator(LotusLib::PackageCollection<LotusLib::CachePairReader>* package, const Ensmallening& ensmalleningData, std::string baseOutputPath);

		void batchIterate(const LotusLib::LotusPath& basePath, const std::vector<std::string>& packages, ExtractorType types);

	protected:
		virtual void processKnownFile(const std::string& packageName, const LotusLib::LotusPath& internalPath, BinaryReaderBuffered* hReader, const LotusLib::CommonHeader& header, Extractor* extractor) = 0;
		virtual void processUnknownFile(const LotusLib::LotusPath& internalPath, const LotusLib::CommonHeader& header, const LotusLib::FileEntries::FileNode* file) = 0;
		virtual void processSkipFile(const LotusLib::LotusPath& internalPath, const LotusLib::CommonHeader& header, const LotusLib::FileEntries::FileNode* file, const Extractor* extractor) = 0;

		void validatePackages(const std::vector<std::string>& packages) const;

		// Only needed because this class iterates over every single file
		// Some files do not have valid headers because they are encrypted or used a different layout
		// We only care about files that have a valid Common Header
		bool tryReadHeader(BinaryReaderBuffered& rawData, LotusLib::CommonHeader& outHeader) const;
	};
}
