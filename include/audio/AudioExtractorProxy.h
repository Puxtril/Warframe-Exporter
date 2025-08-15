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

		inline bool isMultiExport() const override
		{
			return false;
		}

		inline std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> getEnumMapKeys() const override
		{
			const static std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> extTypes = {
				//(int)AudioType::AUDIO_6,
				//(int)AudioType::AUDIO_23,
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)AudioType::AUDIO_139 },
				{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::MISC, (int)AudioType::AUDIO_139 }
			};
			return extTypes;
		}
		
		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::BinaryReaderBuffered* hReader, WarframeExporter::ExtractOptions options) const override;

		static AudioExtractorProxy* getInstance();

		AudioCompression peekCompressionFormat(BinaryReader::BinaryReaderBuffered* headerReader) const;

		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, ExtractOptions options) override;
	};
}
