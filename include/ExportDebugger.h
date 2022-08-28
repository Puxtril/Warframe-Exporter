#pragma once

#include "spdlog/spdlog.h"
#include "BaseExtractor.h"
#include "BinaryReaderExceptions.h"
#include "BinaryReaderBuffered.h"
#include "ExportPathManager.h"
#include "PackageDir.h"
#include "ExporterExceptions.h"
#include "FileTypeMap.hpp"

#include <vector>
#include <map>
#include <iostream>
#include <filesystem>

namespace WarframeExporter
{
	class ExportDebugger
	{
		PackageReader::PackageDir* m_package;
		ExportPathManager m_pathManager;
		const Ensmallening m_ensmalleningData;
		FileTypeMap m_enumMap;
		std::shared_ptr<spdlog::logger> m_logger;

	public:
		ExportDebugger(PackageReader::PackageDir* package, const Ensmallening& ensmallData, std::string baseOutputPath);

		void debugBatchExtract(std::string internalBasePath, std::vector<std::string> packages, FileTypeInternal types);

		void printEnumCounts(const std::string& package);
		void printEnumExamples(const std::string& package, uint32_t enumNum, int printCount);

	private:
		void validatePackages(std::string internalBasePath, std::vector<std::string> packages);
		BinaryReaderBuffered* getBodyReader(const std::string& internalPath, const std::string& packageName, PackageReader::PackageTrioType type);
		void debugExtract(const std::string internalPath, const std::string& pkgName, BinaryReaderBuffered* hReader, Extractor& extractor, const CommonFileHeader& header);
		void writeAllDebugs(const std::string& internalPath, BinaryReaderBuffered* HReader, BinaryReaderBuffered* BReader, BinaryReaderBuffered* FReader);
	};
}
