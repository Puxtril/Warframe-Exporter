#pragma once

#include "Extractor.h"
#include "audio/AudioStructs.h"
#include "audio/AudioPCM/EnumMapAudioPCMReader.h"

namespace WarframeExporter::Audio {
	class AudioPCMExtractor : public Extractor
	{
		AudioPCMExtractor() : Extractor() {}

	public:
		AudioPCMExtractor(const AudioPCMExtractor&) = delete;
		AudioPCMExtractor operator=(const AudioPCMExtractor&) = delete;
		
		inline const std::string& getFriendlyName() const override
		{
			const static std::string friendlyName = "Audio";
			return friendlyName;
		}
		
		inline ExtractorType getExtractorType() const override
		{
			static ExtractorType type = ExtractorType::Audio;
			return type;
		}

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReaderBuffered* hReader) const override
		{
			const static std::string outFileExt = "wav";
			return outFileExt;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			const static std::vector<int> extTypes = {
				(int)AudioCompression::PCM,
				(int)AudioCompression::ADPCM
			};
			return extTypes;
		}

		static AudioPCMExtractor* getInstance();

		void extract(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath) override;
		void extractDebug(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData) override;
	};
}
