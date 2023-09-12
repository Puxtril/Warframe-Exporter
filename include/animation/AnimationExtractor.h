#pragma once

#include "Extractor.h"
#include "EnumMapValue.h"
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

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReaderBuffered* hReader) const override
		{
			static std::string outFileExt = "glb";
			return outFileExt;
		}

		inline const std::string& getFriendlyName() const override
		{
			static std::string friendlyName = "Animation";
			return friendlyName;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = {
				(int)AnimationType::ANIMATION_132
			};
			return extTypes;
		}

		inline ExtractorType getExtractorType() const override
		{
			static ExtractorType type = ExtractorType::Animation;
			return type;
		}

		static AnimationExtractor* getInstance();

		void extract(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalPath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath) override;
		void extractDebug(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalPath, const Ensmallening& ensmalleningData) override;
	};
}