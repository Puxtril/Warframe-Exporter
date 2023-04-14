#pragma once

#include "audio/types/AudioReader139.h"

namespace WarframeExporter::Audio
{
	class AudioOpusReader139 : public AudioReader139
	{
		AudioOpusReader139() = default;

	public:
		static AudioOpusReader139* getInstance();
		void readBody(const AudioHeader& extHeader, BinaryReaderBuffered* fReader, BinaryReaderBuffered* bReader, AudioBody& outBody) override;
	};
}
