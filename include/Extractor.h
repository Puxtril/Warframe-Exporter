#pragma once

#include "EnumMapValue.h"
#include "BinaryReaderBuffered.h"
#include "CommonHeader.h"
#include "Ensmallening.hpp"
#include "PackageCollection.h"
#include "ExporterLogger.h"
#include "LotusLib.h"

#include <string>
#include <vector>

namespace WarframeExporter
{
	enum class ExtractorType
	{
		Model = 1,
		Texture = 2,
		Material = 4,
		VERTEX_COLOR = 8,
		Level = 32,
		Audio = 64,
		Shader = 128
	};
	
	class Extractor : public EnumMapValue
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
		virtual void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath) = 0;
	};
}
