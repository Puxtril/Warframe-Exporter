#pragma once

#include "Extractor.h"
#include "audio/AudioReader.h"
#include "audio/EnumMapAudioExtractor.h"

namespace WarframeExporter::Audio
{

	class AudioExtractorProxy : public Extractor
	{
		AudioExtractorProxy() : Extractor() {}
		
	public:
		AudioExtractorProxy(const AudioExtractorProxy&) = delete;
		AudioExtractorProxy operator=(const AudioExtractorProxy&) = delete;

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

		inline std::vector<int> getEnumMapKeys() const override
		{
			const static std::vector<int> extTypes = {
				//(int)AudioType::AUDIO_6,
				//(int)AudioType::AUDIO_23,
				(int)AudioType::AUDIO_139	
			};
			return extTypes;
		}
		
		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::BinaryReaderBuffered* hReader) const override;

		static AudioExtractorProxy* getInstance();

		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath) override;
		void extractDebug(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData) override;

	private:
		AudioCompression peekCompressionFormat(BinaryReader::BinaryReaderBuffered* headerReader) const;
	};
}
