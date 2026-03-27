#pragma once

#include "Extractor.h"
#include "LevelEnumMap.h"
#include "BinaryReader/Buffered.h"
#include "ExporterExceptions.h"
#include "level/LevelConverter.h"
#include "level/LevelExporterGltf.h"
#include "model/ModelExtractor.h"
#include "landscape/LandscapeExtractor.h"
#include "LotusLib/CommonHeader.h"

namespace WarframeExporter::Level
{
	class LevelExtractor : public Extractor
	{
		LevelExtractor() : Extractor() {}
		
	public:
		LevelExtractor(const LevelExtractor&) = delete;
		LevelExtractor operator=(const LevelExtractor&) = delete;

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::Buffered* hReader, WarframeExporter::ExtractOptions options) const override
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
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::ANIM_RETARGET, (int)LevelType::LEVEL_202 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::ANIM_RETARGET, (int)LevelType::LEVEL_202 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::ANIM_RETARGET, (int)LevelType::LEVEL_207 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::ANIM_RETARGET, (int)LevelType::LEVEL_209 },
			};
			return extTypes;
		}

		static LevelExtractor* getInstance();

		LevelExternal getLevelExternal(uint32_t fileType, BinaryReader::Buffered& headerData, BinaryReader::Buffered& bodyData);
		LevelInternal convertToInternal(const std::string& internalPath, LevelExternal& levelExternal);
		Document createGltfCombined(const LotusLib::PackageCollection& pkgs, LevelInternal& bodyInt, ExtractOptions options);

		void extract(LotusLib::FileEntry& fileEntry, const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, const std::filesystem::path& outputPath, const ExtractOptions options) override;
		
		// More attribute may exist in Packages.bin
		void findExtraAttributes(const std::string& pkgsbinAttributes, LevelExternal& levelExternal);
	
	private:
		void writeAndAdvanceBuffer(Document& gltfDoc, const std::filesystem::path& outputPath);
		void findLandscape(LevelExternal& levelExternal);
		void addLandscapeToGltf(Document& gltfDoc, const LevelInternal& bodyInt, const LotusLib::PackageCollection& pkgs);
	};
}
