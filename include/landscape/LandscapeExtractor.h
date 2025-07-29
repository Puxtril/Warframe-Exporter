#pragma once

#include "Extractor.h"
#include "BinaryReaderBuffered.h"
#include "landscape/LandscapeTypes.h"
#include "landscape/LandscapeStructs.h"
#include "landscape/LandscapeEnumMap.h"
#include "landscape/LandscapeConverter.h"
#include "landscape/LandscapeExporterGltf.h"

namespace WarframeExporter::Landscape
{
	class LandscapeExtractor : public Extractor
	{
		LandscapeExtractor() : Extractor() {}
		
	public:
		LandscapeExtractor(const LandscapeExtractor&) = delete;
		LandscapeExtractor operator=(const LandscapeExtractor&) = delete;

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::BinaryReaderBuffered* hReader) const override
		{
			const static std::string outFileExt = "glb";
			return outFileExt;
		}
		
		inline bool isMultiExport() const override
		{
			return false;
		}

		inline const std::string& getFriendlyName() const override
		{
			const static std::string friendlyName = "Landscape";
			return friendlyName;
		}

		inline ExtractorType getExtractorType() const override
		{
			static ExtractorType type = ExtractorType::Landscape;
			return type;
		}

		inline std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> getEnumMapKeys() const override
		{
			const static std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> extTypes = {
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)LandscapeType::LANDSCAPE_42 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::MISC, (int)LandscapeType::LANDSCAPE_42 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::MISC, (int)LandscapeType::LANDSCAPE_43 },
			};
			return extTypes;
		}

		static LandscapeExtractor* getInstance();

		LandscapeHeaderExternal readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader);
		std::vector<LandscapeBodyChunkExternal> readLandscapeChunks(BinaryReader::BinaryReaderBuffered* bodyReader, const LandscapeHeaderExternal extHeader, const LotusLib::CommonHeader& commonHeader);
		LandscapeInternal formatLandscape(const LandscapeHeaderExternal& landscapeHeader, const std::vector<LandscapeBodyChunkExternal>& landscapeBody);
		Document convertToGltf(const LandscapeInternal& internal);

		void write(Document& gltf, const std::filesystem::path& outputPath);

		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, ExtractOptions options) override;
	};
}
