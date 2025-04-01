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

		inline std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> getEnumMapKeys() const override
		{
			std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> extTypes = {
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::ANIM_RETARGET, (int)LevelType::LEVEL_201 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::ANIM_RETARGET, (int)LevelType::LEVEL_202 }
			};
			return extTypes;
		}

		static LevelExtractor* getInstance();

		LevelExternal getLevelExternal(LotusLib::FileEntry& fileEntry);
		LevelInternal convertToInternal(LotusLib::FileEntry& fileEntry, LevelExternal& levelExternal);
		Document createGltfCombined(LotusLib::PackagesReader& pkgs, LevelInternal& bodyInt);

		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, bool dryRun = false) override;
	
	private:
		void writeAndAdvanceBuffer(Document& gltfDoc, const std::filesystem::path& outputPath);
		void findLandscape(LevelExternal& levelExternal);
		void addLandscapeToGltf(Document& gltfDoc, const LevelInternal& bodyInt, LotusLib::PackagesReader& pkgs);
	};
}
