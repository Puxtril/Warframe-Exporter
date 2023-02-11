#pragma once

#include "Extractor.h"
#include "../Ensmallening.hpp"
#include "TextureStructs.hpp"
#include "BinaryReaderBase.h"
#include "BinaryReaderBuffered.h"
#include "TextureReader.h"
#include "TextureConverter.h"
#include "TextureEnumMap.h"
#include "TextureExporterDDS.h"

#include <iostream>

namespace WarframeExporter::Texture
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
	
	class TextureExtractor : public Extractor
	{
		TextureExtractor() : Extractor() {}
		
	public:
		TextureExtractor(const TextureExtractor&) = delete;
		TextureExtractor operator=(const TextureExtractor&) = delete;

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

		static TextureExtractor* getInstance();
		static void writeData (const std::string& outputFile, const TextureHeaderInternal& header, const TextureBodyInternal& body, const LotusLib::CommonHeader& comHeader);

		void extract(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const std::string& internalPath, const Ensmallening& ensmalleningData, const std::string& outputPath) override;
		void extractDebug(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const std::string& internalPath, const Ensmallening& ensmalleningData) override;
	};
}
