#pragma once

#include "Extractor.h"
#include "BinaryReaderBuffered.h"
#include "material/MaterialTypes.h"
#include "material/MaterialStructs.h"
#include "material/MaterialEnumMap.h"
#include "material/MaterialConverter.h"

namespace WarframeExporter::Material
{
	class MaterialExtractor : public Extractor
	{
		MaterialExtractor() : Extractor() {}
		
	public:
		MaterialExtractor(const MaterialExtractor&) = delete;
		MaterialExtractor operator=(const MaterialExtractor&) = delete;

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::BinaryReaderBuffered* hReader, WarframeExporter::ExtractOptions options) const override
		{
			const static std::string outFileExt = "txt";
			return outFileExt;
		}
		
		inline bool isMultiExport() const override
		{
			return false;
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

		inline std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> getEnumMapKeys() const override
		{
			const static std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> extTypes = {
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)MaterialType::MATERIAL_203 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)MaterialType::MATERIAL_204 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)MaterialType::MATERIAL_205 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)MaterialType::MATERIAL_206 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)MaterialType::MATERIAL_208 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::MISC, (int)MaterialType::MATERIAL_208 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::MISC, (int)MaterialType::MATERIAL_210 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)MaterialType::MATERIAL_214 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)MaterialType::MATERIAL_216 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::MISC, (int)MaterialType::MATERIAL_216 }
			};
			return extTypes;
		}

		static MaterialExtractor* getInstance();

		MaterialExternal getExternalMaterial(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader);
		void writeOut(const MaterialInternal& materialInternal, const std::filesystem::path& outputPath);
		void writeOut(const MaterialInternal& materialInternal, const std::filesystem::path& outputPath, LotusLib::PackagesReader& pkgsReader, const std::string& filePath);

		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, ExtractOptions options) override;
	};
}
