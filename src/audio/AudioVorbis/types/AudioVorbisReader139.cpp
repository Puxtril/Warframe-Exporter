#include "audio/AudioVorbis/types/AudioVorbisReader139.h"

using namespace WarframeExporter::Audio;

namespace
{
	uint32_t
	readU32(const std::vector<uint8_t>& data, size_t offset)
	{
		if (offset + 4 > data.size())
			throw WarframeExporter::unknown_format_error("Vorbis packet table truncated");
		return data[offset] | (data[offset + 1] << 8) | (data[offset + 2] << 16) | ((uint32_t)data[offset + 3] << 24);
	}
}

AudioVorbisReader139*
AudioVorbisReader139::getInstance()
{
	static AudioVorbisReader139* instance = new AudioVorbisReader139();
	return instance;
}

void
AudioVorbisReader139::readBody(const AudioHeader& extHeader, BinaryReader::Buffered* fReader, BinaryReader::Buffered* bReader, AudioBody& outBody)
{
	if (bReader == nullptr)
		throw unknown_format_error("Vorbis body is missing");

	const std::vector<uint8_t>& body = bReader->getPtr();
	const bool hasFooter = fReader != nullptr && fReader->getLength() > 0;

	if (!hasFooter && body.size() < static_cast<size_t>(extHeader.size))
		throw unknown_format_error("Vorbis body smaller than header size");

	//packet table is in the body which means packet data either follows it in the body or lives in the footer
	//some files put an extra block (roughly 1 byte per 1/30th second) before the table so need to search for it
	size_t tableEnd = hasFooter ? body.size() : body.size() - extHeader.size;
	const uint8_t* rawData = hasFooter ? fReader->getPtr().data() : body.data() + tableEnd;
	size_t dataLen = hasFooter ? fReader->getLength() : static_cast<size_t>(extHeader.size);

	size_t packetCount = 0;
	size_t offsetsStart = 0;
	size_t granulesStart = 0;
	for (size_t tableStart = 0; tableStart + 8 <= tableEnd; tableStart++)
	{
		uint32_t tableInfo = readU32(body, tableStart);
		size_t count = tableInfo & 0xFFFF;
		size_t extraWords = tableInfo >> 16;
		if (count == 0 || extraWords > 64 || tableStart + 4 * (1 + extraWords) + 8 * count != tableEnd)
			continue;

		size_t offsets = tableStart + 4 * (1 + extraWords);
		size_t granules = offsets + 4 * count;
		if (readU32(body, offsets) != 0 || readU32(body, granules + 4 * (count - 1)) != extHeader.sampleCount)
			continue;

		packetCount = count;
		offsetsStart = offsets;
		granulesStart = granules;
		break;
	}
	if (packetCount == 0)
		throw unknown_format_error("Vorbis packet table not found");

	outBody.packets.resize(packetCount);
	for (size_t i = 0; i < packetCount; i++)
	{
		size_t start = readU32(body, offsetsStart + 4 * i);
		size_t end = i + 1 < packetCount ? readU32(body, offsetsStart + 4 * (i + 1)) : dataLen;
		if (start > end || end > dataLen)
			throw unknown_format_error("Vorbis packet offset out of range");
		if ((end - start) / 255 + 1 > 255)
			throw unknown_format_error("Vorbis packet too large");
		outBody.packets[i] = { start, end - start, readU32(body, granulesStart + 4 * i) };
	}

	outBody.data.resize(dataLen);
	if (dataLen > 0)
		memcpy(outBody.data.data(), rawData, dataLen);
}
