#pragma once

#include "Extractor.h"
#include "LevelEnumMap.h"
#include "BinaryReaderBuffered.h"
#include "ExporterExceptions.h"
#include "level/LevelConverter.h"
#include "level/LevelExporterGltf.h"
#include "model/ModelExtractor.h"
#include "landscape/LandscapeExtractor.h"
#include "CommonHeader.h"

namespace WarframeExporter::Level
{
	class LevelExtractor : public Extractor
	{
		LevelExtractor() : Extractor() {}
		
	public:
		LevelExtractor(const LevelExtractor&) = delete;
		LevelExtractor operator=(const LevelExtractor&) = delete;

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::BinaryReaderBuffered* hReader) const override
		{
			static std::string outFileExt = "glb";
			return outFileExt;
		}
		
		inline bool isMultiExport() const override
		{
			return false;
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
				(int)LevelType::LEVEL_201,
			};
			return extTypes;
		}

		static LevelExtractor* getInstance();

		LevelExternal getLevelExternal(LotusLib::FileEntry& fileEntry);
		LevelInternal convertToInternal(LotusLib::FileEntry& fileEntry, LevelExternal& levelExternal);
		Document createGltfCombined(LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, LevelInternal& bodyInt);

		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath) override;
	
	private:
		void findLandscape(LevelExternal& levelExternal);
		void addLandscapeToGltf(Document& gltfDoc, const LevelInternal& bodyInt, LotusLib::PackagesReader& pkgs);
	};
}
