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

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::BinaryReaderBuffered* hReader) const override
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

		MaterialExternal getExternalMaterial(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader);
		MaterialInternal formatMaterial(const MaterialExternal& materialExternal);
		void addPackgesBinAttributes(LotusLib::PackagesReader& pkgs, LotusLib::LotusPath internalPath, MaterialExternal& materialExternal);
		void writeOut(const MaterialInternal& materialInternal, const std::filesystem::path& outputPath);

		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath) override;
		void extractDebug(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData) override;
	};
}
