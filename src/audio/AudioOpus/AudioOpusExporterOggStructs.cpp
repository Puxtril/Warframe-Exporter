#include "audio/AudioOpus/AudioOpusExporterOggStructs.h"

using namespace WarframeExporter::Audio;

size_t
OggHeader::serialize(char* dest) const
{
	size_t offset = 0;

	memcpy(dest + offset, &magic, 4);
	offset += 4;	
	memcpy(dest + offset, &version, 1);
	offset += 1;	
	memcpy(dest + offset, &headerType, 1);
	offset += 1;	
	memcpy(dest + offset, &granulePosition, 8);
	offset += 8;	
	memcpy(dest + offset, &streamSerialNumber, 4);
	offset += 4;	
	memcpy(dest + offset, &pageSequenceNumber, 4);
	offset += 4;
	memcpy(dest + offset, &checksum, 4);
	offset += 4;	
	memcpy(dest + offset, &pageSegments, 1);
	offset += 1;	
	memcpy(dest + offset, segmentTable.data(), pageSegments);
	offset += pageSegments;

	return offset;
}

size_t
OggHeader::getByteSize() const
{
	return 27 + segmentTable.size();
}

size_t
OpusHeader::serialize(char* dest) const
{
	size_t offset = 0;

	memcpy(dest + offset, &magic, 8);
	offset += 8;
	memcpy(dest + offset, &version, 1);
	offset += 1;
	memcpy(dest + offset, &channels, 1);
	offset += 1;
	memcpy(dest + offset, &preSkip, 2);
	offset += 2;
	memcpy(dest + offset, &inputSampleRate, 4);
	offset += 4;
	memcpy(dest + offset, &outputGain, 2);
	offset += 2;
	memcpy(dest + offset, &channelMappingFamily, 1);
	offset += 1;

	return offset;
}

size_t
OpusHeader::getByteSize() const
{
	return 19;
}

size_t
OpusTags::serialize(char* dest) const
{
	size_t offset = 0;

	memcpy(dest + offset, &magic, 8);
	offset += 8;
	uint32_t vendorStrLen = static_cast<uint32_t>(vendor.length());
	memcpy(dest + offset, &vendorStrLen, 4);
	offset += 4;
	memcpy(dest + offset, vendor.c_str(), vendorStrLen);
	offset += vendorStrLen;
	uint32_t strCount = static_cast<uint32_t>(comments.size());
	memcpy(dest + offset, &strCount, 4);
	offset += 4;
	for (size_t x = 0; x < comments.size(); x++)
	{
		uint32_t curComStrLen = static_cast<uint32_t>(comments[x].length());
		memcpy(dest + offset, &curComStrLen, 4);
		offset += 4;
		memcpy(dest + offset, comments[x].c_str(), curComStrLen);
		offset += curComStrLen;
	}
	
	return offset;
}

size_t
OpusTags::getByteSize() const
{
	size_t size = 8;
	size += vendor.length() + 4;
	size += 4; // length of comments array
	for (const auto& x : comments)
		size += x.length() + 4;
	return size;
}
