#include "audio/AudioVorbis/AudioVorbisExporterOGG.h"

using namespace WarframeExporter::Audio;

namespace
{
	const uint32_t g_vorbisSerialNumber = 531113124;
	const size_t g_maxPageBytes = 8192;

	void
	appendU32(std::vector<uint8_t>& out, uint32_t value)
	{
		for (int i = 0; i < 4; i++)
			out.push_back((value >> (i * 8)) & 0xFF);
	}
}

void
AudioVorbisExporterOGG::writeData(const AudioHeader& header, const AudioBody& body, std::ostream& outFile)
{
	const std::vector<uint8_t>* setupHeader = findVorbisSetupHeader(header.channelCount, header.samplesPerSec);
	if (setupHeader == nullptr)
		throw unknown_format_error("No Vorbis setup header for " + std::to_string(header.channelCount) + " channels at " + std::to_string(header.samplesPerSec) + " Hz");

	uint32_t sequence = 0;

	std::vector<uint8_t> idHeader = createIdentificationHeader(header);
	std::vector<uint8_t> commentHeader = createCommentHeader();
	writePage({ { idHeader.data(), idHeader.size(), 0 } }, 0, sequence++, (uint8_t)OggHeaderTypeFlags::FIRST, outFile);
	writePage({ { commentHeader.data(), commentHeader.size(), 0 }, { setupHeader->data(), setupHeader->size(), 0 } }, 0, sequence++, 0, outFile);

	const uint8_t* rawData = reinterpret_cast<const uint8_t*>(body.data.data());
	std::vector<PacketSlice> page;
	size_t pageBytes = 0;
	size_t pageSegments = 0;

	for (const AudioPacket& pkt : body.packets)
	{
		size_t segments = pkt.len / 255 + 1;

		if (!page.empty() && (pageSegments + segments > 255 || pageBytes + pkt.len > g_maxPageBytes))
		{
			writePage(page, page.back().granulePosition, sequence++, 0, outFile);
			page.clear();
			pageBytes = 0;
			pageSegments = 0;
		}

		page.push_back({ rawData + pkt.offset, pkt.len, pkt.granulePosition });
		pageBytes += pkt.len;
		pageSegments += segments;
	}

	writePage(page, page.back().granulePosition, sequence++, (uint8_t)OggHeaderTypeFlags::LAST, outFile);
}

std::vector<uint8_t>
AudioVorbisExporterOGG::createIdentificationHeader(const AudioHeader& header)
{
	std::vector<uint8_t> out = { 0x01, 'v', 'o', 'r', 'b', 'i', 's' };
	appendU32(out, 0); //the vorbis version
	out.push_back((uint8_t)header.channelCount);
	appendU32(out, header.samplesPerSec);
	appendU32(out, 0); //bitrate maximum
	appendU32(out, 0); //bitrate nominal
	appendU32(out, 0); //bitrate minimum
	out.push_back((11 << 4) | 8); //block sizes of 2048 and 256
	out.push_back(1); //framing
	return out;
}

std::vector<uint8_t>
AudioVorbisExporterOGG::createCommentHeader()
{
	const std::string vendor = "Warframe";
	std::vector<uint8_t> out = { 0x03, 'v', 'o', 'r', 'b', 'i', 's' };
	appendU32(out, static_cast<uint32_t>(vendor.size()));
	out.insert(out.end(), vendor.begin(), vendor.end());
	appendU32(out, 0); //comment count
	out.push_back(1); //framing
	return out;
}

void
AudioVorbisExporterOGG::writePage(const std::vector<PacketSlice>& packets, uint64_t granulePosition, uint32_t sequence, uint8_t flags, std::ostream& outFile)
{
	OggHeader oggHeader;
	oggHeader.version = 0;
	oggHeader.headerType = flags;
	oggHeader.granulePosition = granulePosition;
	oggHeader.streamSerialNumber = g_vorbisSerialNumber;
	oggHeader.pageSequenceNumber = sequence;
	oggHeader.checksum = 0;

	size_t dataLen = 0;
	for (const PacketSlice& packet : packets)
	{
		for (size_t i = 0; i < packet.len / 255; i++)
			oggHeader.segmentTable.push_back(255);
		oggHeader.segmentTable.push_back(packet.len % 255);
		dataLen += packet.len;
	}
	if (oggHeader.segmentTable.size() > 255)
		throw unknown_format_error("Too many Ogg segments in page");
	oggHeader.pageSegments = static_cast<uint8_t>(oggHeader.segmentTable.size());

	std::vector<uint8_t> pageBytes(oggHeader.getByteSize() + dataLen);
	size_t cursor = oggHeader.serialize((char*)pageBytes.data());
	for (const PacketSlice& packet : packets)
	{
		memcpy(pageBytes.data() + cursor, packet.data, packet.len);
		cursor += packet.len;
	}

	oggHeader.checksum = AudioOpusExporterOggChecksum::calculateHash(pageBytes.data(), static_cast<uint32_t>(pageBytes.size()));
	oggHeader.serialize((char*)pageBytes.data());

	outFile.write((const char*)pageBytes.data(), pageBytes.size());
}
