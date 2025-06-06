#pragma once

#include "audio/AudioReader.h"

namespace WarframeExporter::Audio
{
	class AudioReader139 : public AudioReader
	{
	protected:
		AudioReader139() = default;

	public:
		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = { (int)AudioType::AUDIO_139 };
			return extTypes;
		}

		void readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, AudioHeader& outHeader) override;
	};
}
