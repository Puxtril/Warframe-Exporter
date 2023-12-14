#pragma once

#include "Extractor.h"
#include "BinaryReaderBuffered.h"

namespace WarframeExporter::Material
{
	enum class MaterialType
	{
		MATERIAL_203 = 203,
		MATERIAL_204 = 204,
		MATERIAL_205 = 205,
		MATERIAL_206 = 206,
		MATERIAL_208 = 208,
		MATERIAL_214 = 214,
		MATERIAL_216 = 216
	};

	class MaterialExtractor : public Extractor
	{
		MaterialExtractor() : Extractor() {}
		
	public:
		MaterialExtractor(const MaterialExtractor&) = delete;
		MaterialExtractor operator=(const MaterialExtractor&) = delete;

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReaderBuffered* hReader) const override
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
				(int)MaterialType::MATERIAL_205,
				(int)MaterialType::MATERIAL_206,
				(int)MaterialType::MATERIAL_208,
				(int)MaterialType::MATERIAL_214,
				(int)MaterialType::MATERIAL_216
			};
			return extTypes;
		}

		static MaterialExtractor* getInstance();

		std::vector<std::string> getHlm3Textures(BinaryReaderBuffered* headerReader);
		void getExtraNames(BinaryReaderBuffered* headerReader, std::vector<std::string>& outPaths);
		void extract(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath) override;
		void extractDebug(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData) override;
	};
}
