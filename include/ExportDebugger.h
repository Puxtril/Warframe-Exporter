#pragma once

#include "Logger.h"
#include "BinaryReaderExceptions.h"
#include "BinaryReaderBuffered.h"
#include "ExportPathManager.h"
#include "PackageDir.h"
#include "ExporterExceptions.h"
#include "Ensmallening.hpp"
#include "EnumMap.h"

#include "model/ExtractorModel.h"
#include "texture/ExtractorTexture.h"
#include "material/ExtractorMaterial.h"

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
		EnumMap<Extractor> m_enumMapExtractor;
		Logger& m_logger;

	public:
		ExportDebugger(PackageReader::PackageDir* package, const Ensmallening& ensmallData, std::string baseOutputPath);

		void debugBatchExtract(std::string internalBasePath, std::vector<std::string> packages, ExtractorType types);

		void printEnumCounts(const std::string& package);
		void printEnumExamples(const std::string& package, uint32_t enumNum, int printCount);

	private:
		void validatePackages(std::string internalBasePath, std::vector<std::string> packages);
		//BinaryReaderBuffered* getBodyReader(const std::string& internalPath, const std::string& packageName, PackageReader::PackageTrioType type);
		void debugExtract(PackageDirLimited& pkgParam, const std::string& packageName, const std::string internalPath, BinaryReaderBuffered* hReader, const CommonFileHeader& header, Extractor* extractor);
		void writeAllDebugs(PackageDirLimited& pkgParam, const std::string& packageName, const std::string internalPath);

		// Only needed because this class iterates over every single file
		// Some files do not have valid headers because they are encrypted or used a different layout
		// We only care about files that have a valid Common Header
		bool tryReadHeader(BinaryReaderBuffered& rawData, CommonFileHeader& outHeader);
	};
}
