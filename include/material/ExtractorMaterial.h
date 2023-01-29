#pragma once

#include "Extractor.h"
#include "BinaryReaderBuffered.h"
#include "../Ensmallening.hpp"

namespace WarframeExporter
{
	namespace Material
	{
		enum class MaterialType
		{
			MATERIAL_203 = 203,
			MATERIAL_204 = 204,
			MATERIAL_205 = 205,
		};

		class ExtractorMaterial : public Extractor
		{
		public:
			ExtractorMaterial() : Extractor() {}

			ExtractorMaterial(const ExtractorMaterial&) = delete;
			ExtractorMaterial operator=(const ExtractorMaterial&) = delete;

			inline const std::string& getOutputExtension() const override
			{
				const static std::string outFileExt = "txt";
				return outFileExt;
			}

			inline const std::string& getFriendlyName() const override
			{
				const static std::string friendlyName = "Material";
				return friendlyName;
			}

			inline ExtractorType getExtractorType() const override
			{
				static ExtractorType type = ExtractorType::Material;
				return type;
			}

			inline std::vector<int> getEnumMapKeys() const override
			{
				const static std::vector<int> extTypes = {
					(int)MaterialType::MATERIAL_203,
					(int)MaterialType::MATERIAL_204,
					(int)MaterialType::MATERIAL_205
				};
				return extTypes;
			}

			static ExtractorMaterial* getInstance();

			void getExtraNames(BinaryReaderBuffered* headerReader, std::vector<std::string>& outPaths);
			void extract(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& cacheDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData, const std::string& outputPath) override;
			void extractDebug(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& cacheDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData) override;
		};
	}
}
