#pragma once

#include "../Extractor.h"
#include "ModelConverter.h"
#include "ModelExporterGltf.h"
#include "BinaryReaderBuffered.h"
#include "ModelStructs.hpp"
#include "model/ModelEnumMap.h"
#include "ExporterExceptions.h"
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

		bool m_indexVertexColors = true;

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
			static std::string friendlyName = "3D Model";
			return friendlyName;
		}

		inline ExtractorType getExtractorType() const override
		{
			static ExtractorType type = ExtractorType::Model;
			return type;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = {
				(int)ModelType::MODEL_STATIC_86,
				(int)ModelType::MODEL_STATIC_96,
				(int)ModelType::MODEL_LEVEL_99,
				(int)ModelType::MODEL_DCM_101,
				(int)ModelType::MODEL_HLOD_OR_DCM_102,
				(int)ModelType::MODEL_LEVEL_103,
				(int)ModelType::MODEL_TERRAIN_159,
				(int)ModelType::MODEL_TERRAIN_160,
				(int)ModelType::MODEL_RIGGED_269,
				(int)ModelType::MODEL_RIGGED_272,
				(int)ModelType::MODEL_PACKED_289,
				(int)ModelType::MODEL_PACKED_290
			};
			return extTypes;
		}

		static ModelExtractor* getInstance();

		void indexVertexColors(LotusLib::PackageReader& pkgs);
		void cancelVertexColorIndexing();

		void extractExternal(LotusLib::FileEntry& fileEntry, const Ensmallening& ensmalleningData, ModelHeaderExternal& outHeaderExt, ModelBodyExternal& outBodyExt);
		std::vector<std::vector<glm::u8vec4>> getVertexColors(const LotusLib::LotusPath& modelPath, LotusLib::PackageReader& pkg);

		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath) override;
	};
}
