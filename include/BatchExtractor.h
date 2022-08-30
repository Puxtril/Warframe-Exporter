#pragma once

#include "spdlog/spdlog.h"
#include "BaseExtractor.h"
#include "BinaryReaderBase.h"
#include "CommonHeader.h"
#include "Ensmallening.hpp"
#include "ExportPathManager.h"
#include "PackageDir.h"
#include "ExporterExceptions.h"
#include "FileTypeMap.hpp"
#include "FileProperties.h"
#include "CacheReaderLimited.h"

#include <unordered_map>
#include <chrono>
#include <filesystem>

namespace WarframeExporter
{
	class BatchExtractor
	{
	private:
		PackageReader::PackageDir* m_package;
		const Ensmallening m_ensmalleningData;
		ExportPathManager m_pathManager;
		FileTypeMap m_enumMap;
		std::shared_ptr<spdlog::logger> m_logger;

	public:
		BatchExtractor(PackageReader::PackageDir* package, const Ensmallening& ensmalleningData, std::string baseOutputPath);

		void batchExtract(std::string internalBasePath, std::vector<std::string> packages, FileTypeInternal types);

	private:
		void extractOrLog(PackageDirLimited& pkgParam, const std::string& packageName, const std::string internalPath, BinaryReaderBuffered* hReader, const CommonFileHeader& header, const std::string& outputPath, Extractor& extractor);

		bool existingFileIdentical(const std::string& internalPath, const std::string& outputPath, const PackageReader::CachePair* curPair, const std::string& packageName);
		void validatePackages(std::vector<std::string> packages);
		//BinaryReaderBuffered* getBodyReader(const std::string& internalPath, const std::string& packageName, PackageReader::PackageTrioType type);

		// Only needed because this class iterates over every single file
		// Some files do not have valid headers because they are encrypted or used a different layout
		// We only care about files that have a valid Common Header
		bool tryReadHeader(BinaryReaderBuffered& rawData, CommonFileHeader& outHeader);

		void writeFileProperties(const std::string filePath, const std::string internalPath, const std::string& packageName);
	};
}
