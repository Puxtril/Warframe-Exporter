#pragma once

#include "Extractor.h"
#include "../Ensmallening.hpp"
#include "texture/TextureStructs.hpp"
#include "BinaryReaderBuffered.h"
#include "texture/TextureReader.h"
#include "texture/TextureConverter.h"
#include "texture/TextureEnumMap.h"
#include "texture/TextureExporterDDS.h"
#include "texture/TextureExportTypes.h"
#include "texture/TextureTypes.h"
#include "texture/TextureExporterConvert.h"


namespace WarframeExporter::Texture
{	
	class TextureExtractor : public Extractor
	{
		TextureExtractor() : Extractor() {}
		
	public:
		TextureExtractor(const TextureExtractor&) = delete;
		TextureExtractor operator=(const TextureExtractor&) = delete;

		static inline TextureExportType m_exportType = TextureExportType::TEXTURE_EXPORT_DDS;

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::BinaryReaderBuffered* hReader) const override
		{
			size_t curOffset = hReader->tell();
			TextureHeaderExternal extHeader = TextureReader::readHeader(hReader, {true, true, true});
			hReader->seek(curOffset, std::ios::beg);
			
			static const std::string hdr = "hdr";
			if (extHeader.format == (uint8_t)TextureCompression::BC6)
				return hdr;

			static const std::string dds = "dds";
			if (m_exportType == TextureExportType::TEXTURE_EXPORT_DDS)
				return dds;

			static const std::string png = "png";
			if (m_exportType == TextureExportType::TEXTURE_EXPORT_PNG)
				return png;

			static const std::string tga = "tga";
			if (m_exportType == TextureExportType::TEXTURE_EXPORT_TGA)
				return tga;
		}	
		
		inline bool isMultiExport() const override
		{
			return false;
		}

		inline const std::string& getFriendlyName() const override
		{
			static std::string friendlyName = "Texture";
			return friendlyName;
		}

		inline ExtractorType getExtractorType() const override
		{
			static ExtractorType type = ExtractorType::Texture;
			return type;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			static std::vector<int> extTypes = {
				(int)TextureType::TEXTURE_DIFFUSE_EMISSION_TINT,
				(int)TextureType::TEXTURE_BILLBOARD_SPRITEMAP_DIFFUSE,
				(int)TextureType::TEXTURE_BILLBOARD_SPRITEMAP_NORMAL,
				(int)TextureType::TEXTURE_COMPOSITE,
				(int)TextureType::TEXTURE_ROUGHNESS,
				(int)TextureType::TEXTURE_SKYBOX,
				(int)TextureType::TEXTURE_174,
				(int)TextureType::TEXTURE_176,
				(int)TextureType::TEXTURE_CUBEMAP,
				(int)TextureType::TEXTURE_NORMAL_MAP,
				(int)TextureType::TEXTURE_PACKMAP,
				(int)TextureType::TEXTURE_194,
				(int)TextureType::TEXTURE_DETAILSPACK
			};
			return extTypes;
		};

		static TextureExtractor* getInstance();

		TextureInternal getTexture(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData);
		static void writeData (TextureInternal& texture, const LotusLib::CommonHeader& commonHeader, const std::filesystem::path& outputFile);

		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath) override;
		void extractDebug(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData) override;
	};
}
