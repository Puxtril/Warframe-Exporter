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
		Logger& m_logger;

	public:
		BatchIterator();

		void batchIterate(LotusLib::PackagesReader& pkgsDir, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath, const LotusLib::LotusPath& basePath, const std::vector<std::string>& packages, ExtractorType types);
		static std::vector<std::string> getPackageNames(WarframeExporter::ExtractorType types, const std::filesystem::path& cacheDirPath);

	protected:
		// fileEntry will only contain the Common Header and H package header
		virtual void processKnownFile(
			LotusLib::PackagesReader& pkg,
			const std::string& pkgName,
			LotusLib::FileEntry& fileEntry,
			Extractor* extractor,
			const Ensmallening& ensmalleningData,
			const std::filesystem::path& outputPath
		) = 0;
		
		virtual void processUnknownFile(
			LotusLib::PackagesReader& pkg,
			const std::string& pkgName,
			LotusLib::FileEntry& fileEntry,
			const Ensmallening& ensmalleningData,
			const std::filesystem::path& outputPath
		) = 0;
		
		virtual void processSkipFile(
			LotusLib::PackagesReader& pkg,
			const std::string& pkgName,
			LotusLib::FileEntry& fileEntry,
			const Extractor* extractor
		) = 0;

		bool pkgHasDir(LotusLib::PackageReader& pkg, const LotusLib::LotusPath& path);
	};
}
