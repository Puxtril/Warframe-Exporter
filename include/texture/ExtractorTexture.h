#pragma once

#include "Extractor.h"
#include "../Ensmallening.hpp"
#include "TextureStructs.hpp"
#include "BinaryReaderBase.h"
#include "BinaryReaderBuffered.h"
#include "TextureHeader.h"
#include "TextureBody.h"
#include "CacheReaderLimited.h"

#include <iostream>

namespace WarframeExporter
{
	namespace Texture
	{
		enum class TextureType {
			TEXTURE_DIFFUSE_EMISSION_TINT = 163,
			TEXTURE_BILLBOARD_SPRITEMAP_DIFFUSE = 164,
			TEXTURE_BILLBOARD_SPRITEMAP_NORMAL = 165,
			TEXTURE_ROUGHNESS = 167,
			TEXTURE_SKYBOX = 171,
			TEXTURE_174 = 174,
			TEXTURE_176 = 176,
			TEXTURE_CUBEMAP = 177,
			TEXTURE_NORMAL_MAP = 184,
			TEXTURE_PACKMAP = 188,
			TEXTURE_194 = 194,
			TEXTURE_DETAILSPACK = 195
		};
	
		class ExtractorTexture : public Extractor
		{
		public:
			ExtractorTexture() : Extractor() {}

			ExtractorTexture(const ExtractorTexture&) = delete;
			ExtractorTexture operator=(const ExtractorTexture&) = delete;

			inline const std::string& getOutputExtension() const override
			{
				static std::string outFileExt = "dds";
				return outFileExt;
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

			static ExtractorTexture* getInstance();
			static TextureHeaderInternal readHeader	(BinaryReaderBuffered* HfileReader, BinaryReaderBuffered* FfileReader, BinaryReaderBuffered* BfileReader, const Ensmallening& ensmalleningData, const CommonFileHeader& header, std::shared_ptr<spdlog::logger>& logger);
			static TextureBodyInternal readBody (BinaryReaderBuffered* FfileReader, BinaryReaderBuffered* BfileReader, const Ensmallening& ensmalleningData, const CommonFileHeader& header, const TextureHeaderInternal& headerInternal, std::shared_ptr<spdlog::logger>& logger);
			static void writeData (const std::string& outputFile, const TextureHeaderInternal& header, const TextureBodyInternal& body, const CommonFileHeader& comHeader);

			void extract(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& pkgDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData, const std::string& outputPath) override;
			void extractDebug(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& pkgDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData) override;
		};
	}
}
