#pragma once

#include "../Extractor.h"
#include "ModelConverter.h"
#include "ModelExporterGltf.h"
#include "BinaryReaderBuffered.h"
#include "ModelStructs.hpp"
#include "model/ModelEnumMap.h"
#include "ExporterExceptions.h"

#include <iostream>

namespace WarframeExporter::Model
{
	class ModelExtractor : public Extractor
	{
		ModelExtractor() : Extractor() {}
		
	public:
		ModelExtractor(const ModelExtractor&) = delete;
		ModelExtractor operator=(const ModelExtractor&) = delete;

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReaderBuffered* hReader) const override
		{
			static std::string outFileExt = "glb";
			return outFileExt;
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
				(int)ModelType::MODEL_STATIC_96,
				(int)ModelType::MODEL_LEVEL_99,
				(int)ModelType::MODEL_LEVEL_103,
				(int)ModelType::MODEL_TERRAIN_159,
				(int)ModelType::MODEL_RIGGED_269,
				(int)ModelType::MODEL_RIGGED_272
			};
			return extTypes;
		}

		static ModelExtractor* getInstance();

		void extractExternal(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalPath, const Ensmallening& ensmalleningData, ModelHeaderExternal& outHeaderExt, ModelBodyExternal& outBodyExt);

		void extract(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalPath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath) override;
		void extractDebug(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalPath, const Ensmallening& ensmalleningData) override;
	};
}
