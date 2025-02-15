#pragma once

#include "EnumMapExtractorValue.h"
#include "BinaryReaderBuffered.h"
#include "CommonHeader.h"
#include "PackageCollection.h"
#include "ExporterLogger.h"
#include "LotusLib.h"

#include <string>
#include <vector>

namespace WarframeExporter
{
	class Extractor : public EnumMapExtractorValue
	{
	protected:
		Logger& m_logger;

		Extractor() : m_logger(Logger::getInstance()) {}
	
	public:
		virtual const std::string& getFriendlyName() const = 0;
		virtual const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::BinaryReaderBuffered* hReader) const = 0;
		// If true, `outputPath` in `extract` will be a folder.
		// Otherwise, will be a file with the correct extension
		virtual bool isMultiExport() const = 0;
		virtual ExtractorType getExtractorType() const = 0;
		virtual void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, bool dryRun = false) = 0;
	};
}
