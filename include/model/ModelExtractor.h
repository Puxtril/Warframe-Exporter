#pragma once

#include "../Extractor.h"
#include "ModelConverter.h"
#include "ModelExporterGltf.h"
#include "BinaryReaderBuffered.h"
#include "ModelStructs.hpp"
#include "model/ModelEnumMap.h"
#include "ExporterExceptions.h"
#include "model/ModelTypes.h"
#include "model/vertexcolor/VertexColorIndexer.h"

#include <iostream>

namespace WarframeExporter::Model
{
	class ModelExtractor : public Extractor
	{
		ModelExtractor() : Extractor() {}

		VertexColor::VertexColorIndexer m_vertexColorIndexer;
		
	public:
		ModelExtractor(const ModelExtractor&) = delete;
		ModelExtractor operator=(const ModelExtractor&) = delete;

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::BinaryReaderBuffered* hReader, WarframeExporter::ExtractOptions options) const override
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
			static std::string friendlyName = "3D Model";
			return friendlyName;
		}

		inline ExtractorType getExtractorType() const override
		{
			static ExtractorType type = ExtractorType::Model;
			return type;
		}

		inline std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> getEnumMapKeys() const override
		{
			std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> extTypes = {
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_STATIC_86 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_STATIC_96 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_LEVEL_99 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_LEVEL_99 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)ModelDCMType::MODEL_DCM_101 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, 102 }, // Shared by DCM/HLOD/Model
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_LEVEL_103 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_LEVEL_106 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::MISC, 105 }, // Shared by DCM/HLOD
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_TERRAIN_159 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_TERRAIN_160 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_TERRAIN_163 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_RIGGED_269 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_RIGGED_272 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_RIGGED_275 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_RIGGED_277 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_RIGGED_278 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_RIGGED_281 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_PACKED_289 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_PACKED_290 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_PACKED_299 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_PACKED_300 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::MISC, (int)ModelType::MODEL_PACKED_301 },
			};
			return extTypes;
		}

		static ModelExtractor* getInstance();

		void indexVertexColors(LotusLib::PackageReader& pkgs);
		void cancelVertexColorIndexing();

		ModelReader* getModelReader(LotusLib::FileEntry& fileEntry, LotusLib::Game game);
		void extractExternal(ModelReader* modelReader, LotusLib::FileEntry& fileEntry, LotusLib::Game game, ModelHeaderExternal& outHeaderExt, ModelBodyExternal& outBodyExt);
		std::vector<std::vector<glm::u8vec4>> getVertexColors(const LotusLib::LotusPath& modelPath, LotusLib::PackageReader& pkg, bool indexVertexColors);

		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, ExtractOptions options) override;
	};
}
