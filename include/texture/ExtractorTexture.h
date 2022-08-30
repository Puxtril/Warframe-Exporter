#pragma once

#include "../BaseExtractor.h"
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
		class ExtractorTexture : public Extractor
		{
		public:
			inline std::string outFileExt()
			{
				static std::string outFileExt = "dds";
				return outFileExt;
			}

			inline std::string friendlyName()
			{
				static std::string friendlyName = "Texture";
				return friendlyName;
			}

			inline std::vector<FileTypeExternal> externalTypes()
			{
				static std::vector<FileTypeExternal> extTypes = {
					FileTypeExternal::TEXTURE_DIFFUSE_EMISSION_TINT,
					FileTypeExternal::TEXTURE_BILLBOARD_SPRITEMAP_DIFFUSE,
					FileTypeExternal::TEXTURE_BILLBOARD_SPRITEMAP_NORMAL,
					FileTypeExternal::TEXTURE_ROUGHNESS,
					FileTypeExternal::TEXTURE_SKYBOX,
					FileTypeExternal::TEXTURE_174,
					FileTypeExternal::TEXTURE_176,
					FileTypeExternal::TEXTURE_CUBEMAP,
					FileTypeExternal::TEXTURE_NORMAL_MAP,
					FileTypeExternal::TEXTURE_PACKMAP,
					FileTypeExternal::TEXTURE_194,
					FileTypeExternal::TEXTURE_DETAILSPACK
				};
				return extTypes;
			};

			inline FileTypeInternal internalType()
			{
				static FileTypeInternal type = FileTypeInternal::Texture;
				return type;
			}

			static ExtractorTexture& getInstance();
			static TextureHeaderInternal readHeader	(BinaryReaderBuffered* HfileReader, BinaryReaderBuffered* FfileReader, BinaryReaderBuffered* BfileReader, const Ensmallening& ensmalleningData, const CommonFileHeader& header, std::shared_ptr<spdlog::logger>& logger);
			static TextureBodyInternal readBody (BinaryReaderBuffered* FfileReader, BinaryReaderBuffered* BfileReader, const Ensmallening& ensmalleningData, const CommonFileHeader& header, const TextureHeaderInternal& headerInternal, std::shared_ptr<spdlog::logger>& logger);
			static void writeData (const std::string& outputFile, const TextureHeaderInternal& header, const TextureBodyInternal& body, const CommonFileHeader& comHeader);

			void extract(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& pkgDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData, const std::string& outputPath) override;
			void extractDebug(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& pkgDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData) override;
		};
	}
}