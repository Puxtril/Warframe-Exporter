#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <cstring>

/* There are multiple opus headers that can be written to an Ogg file.
 * All require a prerequisite OggHeader to define the Opus header/data.
 * Inside the Oggheader is a checksum field, which is a checksum of the
 *   OggHeader AND whatever Opus data proceeds.
 * This means the OggHeader and Opus data must be written to memory
 *   before being written to a file.
*/
namespace WarframeExporter::Audio
{
	enum class OggHeaderTypeFlags
	{
		MIDDLE = 1,
		FIRST = 2,
		LAST = 4
	};

	struct OggHeader
	{
		uint32_t magic = 0x5367674F; //OggS
		uint8_t version;
		uint8_t headerType;
		uint64_t granulePosition;
		uint32_t streamSerialNumber;
		uint32_t pageSequenceNumber;
		uint32_t checksum;
		uint8_t pageSegments;
		std::vector<char> segmentTable;

		size_t serialize(char* dest) const;
		size_t getByteSize() const;
	};

	struct OpusHeader
	{
		uint64_t magic = 0x646165487375704F; //OpusHead
		uint8_t version;
		uint8_t channels;
		uint16_t preSkip;
		uint32_t inputSampleRate;
		uint16_t outputGain;
		uint8_t channelMappingFamily;

		size_t serialize(char* dest) const;
		size_t getByteSize() const;
	};

	struct OpusTags
	{
		uint64_t magic = 0x736761547375704F; //OpusTags
		std::string vendor;
		std::vector<std::string> comments;

		size_t serialize(char* dest) const;
		size_t getByteSize() const;
	};
}
