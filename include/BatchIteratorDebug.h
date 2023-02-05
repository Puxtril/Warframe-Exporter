#pragma once

#include "Logger.h"
#include "BinaryReaderExceptions.h"
#include "BinaryReaderBuffered.h"
#include "ExporterExceptions.h"
#include "EnumMapExtractor.h"
#include "BatchIterator.h"
#include "BatchIterator.h"

#include <vector>
#include <map>
#include <iostream>
#include <filesystem>

namespace WarframeExporter
{
	class BatchIteratorDebug : public BatchIterator
	{
	public:
		BatchIteratorDebug(PackageReader::PackageDir* package, const Ensmallening& ensmallData, std::string baseOutputPath);

		void printEnumCounts(const std::string& package);

	protected:
		void processKnownFile(PackageDirLimited& pkgParam, const std::string& packageName, const std::string& internalPath, BinaryReaderBuffered* hReader, const CommonFileHeader& header, Extractor* extractor) override;
		void processUnknownFile(const std::string& internalPath, const CommonFileHeader& header, const Entries::FileNode* file) override;
		void processSkipFile(const std::string& internalPath, const CommonFileHeader& header, const Entries::FileNode* file, const Extractor* extractor) override;

		void writeAllDebugs(PackageDirLimited& pkgParam, const std::string& packageName, const std::string internalPath);
	};
}
