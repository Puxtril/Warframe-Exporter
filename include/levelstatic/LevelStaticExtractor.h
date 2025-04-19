#pragma once

#include "fx/gltf.h"

#include "Extractor.h"
#include "BinaryReaderBuffered.h"
#include "levelstatic/LevelStaticTypes.h"
#include "levelstatic/LevelStaticStructs.h"
#include "levelstatic/LevelStaticEnumMap.h"
#include "levelstatic/LevelStaticExporterGltf.h"

#include "model/ModelExtractor.h"

namespace WarframeExporter::LevelStatic
{
	class LevelStaticExtractor : public Extractor
	{
		LevelStaticExtractor() : Extractor() {}
		
	public:
		LevelStaticExtractor(const LevelStaticExtractor&) = delete;
		LevelStaticExtractor operator=(const LevelStaticExtractor&) = delete;

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::BinaryReaderBuffered* hReader) const override
		{
			const static std::string outFileExt = "glb";
			return outFileExt;
		}
		
		inline bool isMultiExport() const override
		{
			return false;
		}

		inline const std::string& getFriendlyName() const override
		{
			const static std::string friendlyName = "LevelStatic";
			return friendlyName;
		}

		inline ExtractorType getExtractorType() const override
		{
			static ExtractorType type = ExtractorType::LevelStatic;
			return type;
		}

		inline std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> getEnumMapKeys() const override
		{
			const static std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> extTypes = {
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::ANIM_RETARGET, (int)LevelStaticType::LEVELSTATIC_28 },
			};
			return extTypes;
		}

		static LevelStaticExtractor* getInstance();

		LevelStaticExternal read(LotusLib::FileEntry& fileEntry);
		// TODO: Make separate function for conversion
		void addModelsToGltf(LevelStaticExternal& external, LotusLib::PackagesReader& pkgs, fx::gltf::Document& gltf);
		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, bool dryRun = false) override;
	};
}
