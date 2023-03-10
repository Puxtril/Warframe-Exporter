#pragma once

#include "EnumMapValue.h"
#include "BinaryReaderBuffered.h"
#include "CommonHeader.h"
#include "Ensmallening.hpp"
#include "PackageCollection.h"
#include "ExporterLogger.h"

#include <string>
#include <vector>

namespace WarframeExporter
{
	enum class ExtractorType
	{
		Model = 1,
		Texture = 2,
		Material = 4,
		VERTEX_COLOR = 8
	};
	
	class Extractor : public EnumMapValue
	{
	protected:
		Logger& m_logger;

		Extractor() : m_logger(Logger::getInstance()) {}
	
	public:
		virtual const std::string& getFriendlyName() const = 0;
		virtual const std::string& getOutputExtension() const = 0;
		virtual ExtractorType getExtractorType() const = 0;
		virtual void extract(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath) = 0;
		virtual void extractDebug(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData) = 0;
	};
}
