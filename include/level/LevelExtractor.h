#pragma once

#include "Extractor.h"
#include "LevelEnumMap.h"
#include "BinaryReaderBuffered.h"
#include "ExporterExceptions.h"

#include <iostream>

namespace WarframeExporter::Level
{
	class LevelExtractor : public Extractor
	{
		LevelExtractor() : Extractor() {}
		
	public:
		LevelExtractor(const LevelExtractor&) = delete;
		LevelExtractor operator=(const LevelExtractor&) = delete;

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReaderBuffered* hReader) const override
		{
			static std::string outFileExt = "glb";
			return outFileExt;
		}

		inline const std::string& getFriendlyName() const override
		{
			static std::string friendlyName = "Level";
			return friendlyName;
		}

		inline ExtractorType getExtractorType() const override
		{
			static ExtractorType type = ExtractorType::Level;
			return type;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = {
				(int)LevelType::LEVEL_196,
			};
			return extTypes;
		}

		static LevelExtractor* getInstance();

		void extract(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalPath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath) override;
		void extractDebug(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalPath, const Ensmallening& ensmalleningData) override;
	};
}
