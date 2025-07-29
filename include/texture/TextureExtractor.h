#pragma once

#include "Extractor.h"
#include "texture/TextureStructs.hpp"
#include "BinaryReaderBuffered.h"
#include "texture/TextureReader.h"
#include "texture/TextureConverter.h"
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
			TextureHeaderExternal extHeader = TextureReader::readHeader(hReader, commonHeader);
			hReader->seek(curOffset, std::ios::beg);

			static const std::string hdr = "hdr";
			if (extHeader.format == (uint8_t)TextureCompression::BC6 && m_exportType != TextureExportType::TEXTURE_EXPORT_DDS)
				return hdr;

			switch (m_exportType)
			{
			case TextureExportType::TEXTURE_EXPORT_DDS:
			{
				static const std::string dds = "dds";
				return dds;
			}
			case TextureExportType::TEXTURE_EXPORT_PNG:
			{
				static const std::string png = "png";
				return png;
			}
			case TextureExportType::TEXTURE_EXPORT_TGA:
			{
				static const std::string tga = "tga";
				return tga;
			}
			default:
				throw std::runtime_error("Unexpected export format");
			}
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

		inline std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> getEnumMapKeys() const override
		{
			static std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> extTypes = {
				{  LotusLib::Game::WARFRAME,  LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_BILLBOARD_SPRITEMAP_DIFFUSE },
				{  LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_BILLBOARD_SPRITEMAP_DIFFUSE },
				{  LotusLib::Game::WARFRAME,  LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_DIFFUSE_EMISSION_TINT },
				{  LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_DIFFUSE_EMISSION_TINT },
				{  LotusLib::Game::WARFRAME,  LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_BILLBOARD_SPRITEMAP_NORMAL },
				{  LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_BILLBOARD_SPRITEMAP_NORMAL },
				{  LotusLib::Game::WARFRAME,  LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_COMPOSITE },
				{  LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_COMPOSITE },
				{  LotusLib::Game::WARFRAME,  LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_ROUGHNESS },
				{  LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_ROUGHNESS },
				{ LotusLib::Game::WARFRAME,  LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_SKYBOX },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_SKYBOX },
				{ LotusLib::Game::WARFRAME,  LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_174 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_174 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_RAW_175 },
				{ LotusLib::Game::WARFRAME,  LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_176 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_176 },
				{ LotusLib::Game::WARFRAME,  LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_CUBEMAP },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_CUBEMAP },
				{ LotusLib::Game::WARFRAME,  LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_HDR },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_HDR },
				{ LotusLib::Game::WARFRAME,  LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_NORMAL_MAP },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_NORMAL_MAP },
				{ LotusLib::Game::WARFRAME,  LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_PACKMAP },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_PACKMAP },
				{ LotusLib::Game::WARFRAME,  LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_194 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_194 },
				{ LotusLib::Game::WARFRAME,  LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_DETAILSPACK },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::TEXTURE, (int)TextureType::TEXTURE_DETAILSPACK }
			};
			return extTypes;
		};

		static TextureExtractor* getInstance();

		TextureInternal getTexture(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs);
		static void writeData (TextureInternal& texture, const LotusLib::CommonHeader& commonHeader, const std::filesystem::path& outputFile);

		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, ExtractOptions options) override;

	private:
		static void writeArray(TextureInternal& texture, const LotusLib::CommonHeader& commonHeader, const char* data, size_t dataLen, const std::filesystem::path& outputFile);
		static void writeTextureToFile(TextureInternal& texture, const LotusLib::CommonHeader& commonHeader, const char* data, size_t dataLen, const std::filesystem::path& outputFile);
	};
}
