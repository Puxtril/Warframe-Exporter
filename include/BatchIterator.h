#pragma once

#include "Logger.h"
#include "BinaryReaderBase.h"
#include "CommonHeader.h"
#include "Ensmallening.hpp"
#include "ExportPathManager.h"
#include "PackageDir.h"
#include "ExporterExceptions.h"
#include "FileProperties.h"
#include "CacheReaderLimited.h"
#include "EnumMapExtractor.h"

#include <filesystem>

namespace WarframeExporter
{
	class BatchIterator
	{
	protected:
		PackageReader::PackageDir* m_package;
		const Ensmallening m_ensmalleningData;
		ExportPathManager m_pathManager;
		Logger& m_logger;

	public:
		BatchIterator(PackageReader::PackageDir* package, const Ensmallening& ensmalleningData, std::string baseOutputPath);

		void batchIterate(std::string basePath, std::vector<std::string> packages, ExtractorType types);

	protected:
		virtual void processKnownFile(PackageDirLimited& pkgParam, const std::string& packageName, const std::string& internalPath, BinaryReaderBuffered* hReader, const CommonFileHeader& header, Extractor* extractor) = 0;
		virtual void processUnknownFile(const std::string& internalPath, const CommonFileHeader& header, const Entries::FileNode* file) = 0;
		virtual void processSkipFile(const std::string& internalPath, const CommonFileHeader& header, const Entries::FileNode* file, const Extractor* extractor) = 0;

	private:
		void validatePackages(std::vector<std::string> packages);

		// Only needed because this class iterates over every single file
		// Some files do not have valid headers because they are encrypted or used a different layout
		// We only care about files that have a valid Common Header
		bool tryReadHeader(BinaryReaderBuffered& rawData, CommonFileHeader& outHeader);
	};
}
