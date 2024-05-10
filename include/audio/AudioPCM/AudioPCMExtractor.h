#pragma once

#include "Extractor.h"
#include "audio/AudioStructs.h"
#include "audio/AudioPCM/EnumMapAudioPCMReader.h"
#include "audio/AudioPCM/AudioPCMExporterWAV.h"

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
		
		inline bool isMultiExport() const override
		{
			return false;
		}

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::BinaryReaderBuffered* hReader) const override
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

		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath) override;
	};
}
