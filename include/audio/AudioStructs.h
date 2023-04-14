#pragma once

#include <vector>

namespace WarframeExporter::Audio {	
	enum class AudioType
	{
		AUDIO_6 = 6,
		AUDIO_23 = 23,
		AUDIO_139 = 139
	};
	
	enum class AudioCompression
	{
		PCM = 0,
		ADPCM = 5,
		OPUS = 7
	};

	struct AudioHeader
	{
		AudioCompression compression;
		unsigned int streamSerialNumber;
		int samplesPerSec;
		int bitsPerSample;
		int channelCount;
		int averageBytesPerSecond;
		int blockAllign;
		int samplesPerBlock;
		int size;
	};

	struct AudioBody
	{
		std::vector<char> data;
	};
}
