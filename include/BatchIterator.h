#pragma once

#include "ExporterLogger.h"
#include "BinaryReaderBuffered.h"
#include "CommonHeader.h"
#include "FileNode.h"
#include "PackageCollection.h"
#include "ExporterExceptions.h"
#include "FileProperties.h"
#include "EnumMapExtractor.h"
#include "LotusUtils.h"

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

		// Derrives which packages to read based on `types`
		void batchIterate(LotusLib::PackagesReader& pkgsDir, const std::filesystem::path& outputPath, const LotusLib::LotusPath& basePath, ExtractorType types, LotusLib::Game game, ExtractOptions options);
		void batchIterate(LotusLib::PackagesReader& pkgsDir, const std::filesystem::path& outputPath, const LotusLib::LotusPath& basePath, LotusLib::PackageCategory pkgCategory, ExtractorType types, LotusLib::Game game, ExtractOptions options);

	protected:
		// fileEntry will only contain the Common Header and H package header
		virtual void processKnownFile(
			LotusLib::PackagesReader& pkg,
			LotusLib::FileEntry& fileEntry,
			Extractor* extractor,
			const std::filesystem::path& outputPath,
			ExtractOptions options
		) = 0;
		
		virtual void processUnknownFile(
			LotusLib::PackagesReader& pkg,
			LotusLib::FileEntry& fileEntry,
			const std::filesystem::path& outputPath
		) = 0;
		
		virtual void processSkipFile(
			LotusLib::PackagesReader& pkg,
			LotusLib::FileEntry& fileEntry,
			const Extractor* extractor
		) = 0;

		bool pkgHasDir(LotusLib::PackageReader& pkg, const LotusLib::LotusPath& path);
	};
}
