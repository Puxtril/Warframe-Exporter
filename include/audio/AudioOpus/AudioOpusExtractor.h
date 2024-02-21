#pragma once

#include "Extractor.h"
#include "audio/AudioOpus/AudioOpusExporterOGG.h"
#include "audio/AudioStructs.h"
#include "audio/AudioOpus/EnumMapAudioOpusReader.h"

namespace WarframeExporter::Audio {
	class AudioOpusExtractor : public Extractor
	{
		AudioOpusExtractor() : Extractor() {}

	public:
		AudioOpusExtractor(const AudioOpusExtractor&) = delete;
		AudioOpusExtractor operator=(const AudioOpusExtractor&) = delete;
		
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

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::BinaryReaderBuffered* hReader) const override
		{
			const static std::string outFileExt = "ogg";
			return outFileExt;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			const static std::vector<int> extTypes = {
				(int)AudioCompression::OPUS
			};
			return extTypes;
		}

		static AudioOpusExtractor* getInstance();

		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath) override;
		void extractDebug(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData) override;
	};
}
