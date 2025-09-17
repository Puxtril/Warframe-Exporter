#pragma once

#include "audio/types/AudioReader139.h"

namespace WarframeExporter::Audio
{
	class AudioPCMReader139 : public AudioReader139
	{
		AudioPCMReader139() = default;

	public:
		static AudioPCMReader139* getInstance();
		void readBody(const AudioHeader& extHeader, BinaryReader::BinaryReaderBuffered* fReader, BinaryReader::BinaryReaderBuffered* bReader, AudioBody& outBody) override;
	};
}
