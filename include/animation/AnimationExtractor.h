#pragma once

#include "Extractor.h"
#include "AnimationStructs.hpp"
#include "AnimationReader.h"
#include "AnimationConverter.h"
#include "AnimationExporterGltf.h"
#include "AnimationEnumMap.h"

namespace WarframeExporter::Animation
{
	

	class AnimationExtractor : public Extractor
	{
		AnimationExtractor() : Extractor() {}
			
	public:
		AnimationExtractor(const AnimationExtractor&) = delete;
		AnimationExtractor operator=(const AnimationExtractor&) = delete;

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::BinaryReaderBuffered* hReader, WarframeExporter::ExtractOptions options) const override
		{
			static std::string outFileExt = "glb";
			return outFileExt;
		}

		inline const std::string& getFriendlyName() const override
		{
			static std::string friendlyName = "Animation";
			return friendlyName;
		}

		inline bool isMultiExport() const override
		{
			return false;
		}

		inline std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> getEnumMapKeys() const override
		{
			std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> extTypes = {
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)AnimationType::ANIMATION_132 },
			};
			return extTypes;
		}

		inline ExtractorType getExtractorType() const override
		{
			static ExtractorType type = ExtractorType::Animation;
			return type;
		}

		static AnimationExtractor* getInstance();

		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, ExtractOptions options) override;
	};
}