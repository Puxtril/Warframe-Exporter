#pragma once

#include "Logger.h"
#include "BinaryReaderBase.h"
#include "ExporterExceptions.h"
#include "FileProperties.h"
#include "CacheReaderLimited.h"
#include "EnumMapExtractor.h"
#include "BatchIterator.h"

#include <filesystem>

namespace WarframeExporter
{
	class BatchIteratorExport : public BatchIterator
	{
	public:
		BatchIteratorExport(PackageReader::PackageDir* package, const Ensmallening& ensmalleningData, std::string baseOutputPath);

	protected:
		void processKnownFile(PackageDirLimited& pkgParam, const std::string& packageName, const std::string& internalPath, BinaryReaderBuffered* hReader, const CommonFileHeader& header, Extractor* extractor) override;
		void processUnknownFile(const std::string& internalPath, const CommonFileHeader& header, const Entries::FileNode* file) override;
		void processSkipFile(const std::string& internalPath, const CommonFileHeader& header, const Entries::FileNode* file, const Extractor* extractor) override;

		bool existingFileIdentical(const std::string& internalPath, const std::string& outputPath, const PackageReader::CachePair* curPair, const std::string& packageName);
		void writeFileProperties(const std::string filePath, const std::string internalPath, const std::string& packageName);
	};
}
