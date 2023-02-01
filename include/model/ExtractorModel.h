#pragma once

#include "../Extractor.h"
#include "../Ensmallening.hpp"
#include "ModelConverter.h"
#include "GltfExporter.h"
#include "BinaryReaderBuffered.h"
#include "ModelStructs.hpp"
#include "types/ModelReader269.hpp"
#include "types/ModelReader272.hpp"
#include "types/ModelReader96.hpp"

#include <iostream>

namespace WarframeExporter
{
	namespace Model
	{
		enum class ModelType
		{
			MODEL_STATIC_96 = 96,
			MODEL_RIGGED_269 = 269,
			MODEL_RIGGED_272 = 272
		};

		class ExtractorModel : public Extractor
		{
			ExtractorModel() : Extractor() {}
		
		public:
			ExtractorModel(const ExtractorModel&) = delete;
			ExtractorModel operator=(const ExtractorModel&) = delete;

			inline const std::string& getOutputExtension() const override
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
					(int)ModelType::MODEL_RIGGED_269,
					(int)ModelType::MODEL_RIGGED_272
				};
				return extTypes;
			}

			static ExtractorModel* getInstance();

			void extract(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& cacheDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData, const std::string& outputPath) override;
			void extractDebug(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& cacheDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData) override;
		};
	}
}
