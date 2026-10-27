#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

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
		OPUS = 7,
		VORBIS = 8
	};

	struct AudioHeader
	{
		AudioCompression compression;
		unsigned int streamSerialNumber;
		int samplesPerSec;
		int bitsPerSample;
		int channelCount;
		unsigned int sampleCount;
		int averageBytesPerSecond;
		int blockAllign;
		int samplesPerBlock;
		int size;
	};

	struct AudioPacket
	{
		size_t offset;
		size_t len;
		uint64_t granulePosition;
	};

	struct AudioBody
	{
		std::vector<char> data;
		std::vector<AudioPacket> packets;
	};
}
