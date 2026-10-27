#pragma once

#include <vector>
#include <cstdint>

namespace WarframeExporter::Audio
{
	//returns nullptr when no setup header is known for this channel count or sample rate
	const std::vector<uint8_t>* findVorbisSetupHeader(int channels, int sampleRate);
}