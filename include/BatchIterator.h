#pragma once

#include "ExporterLogger.h"
#include "BinaryReaderBuffered.h"
#include "CommonHeader.h"
#include "Ensmallening.hpp"
#include "FileNode.h"
#include "PackageCollection.h"
#include "ExporterExceptions.h"
#include "FileProperties.h"
#include "EnumMapExtractor.h"

#include <filesystem>

namespace WarframeExporter
{
	using BinReader = BinaryReader::BinaryReaderBuffered;

	class BatchIterator
	{
	protected:
		LotusLib::PackagesReader m_package;
		const Ensmallening m_ensmalleningData;
		std::filesystem::path m_baseOutPath;
		Logger& m_logger;

	public:
		BatchIterator(const std::filesystem::path& pkgsDir, const Ensmallening& ensmalleningData, std::filesystem::path baseOutputPath);

		void batchIterate(const LotusLib::LotusPath& basePath, const std::vector<std::string>& packages, ExtractorType types);

	protected:
		// fileEntry will only contain the Common Header and H package header
		virtual void processKnownFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, Extractor* extractor) = 0;
		virtual void processUnknownFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry) = 0;
		virtual void processSkipFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, const Extractor* extractor) = 0;

		void validatePackages(const std::vector<std::string>& packages);
	};
}
