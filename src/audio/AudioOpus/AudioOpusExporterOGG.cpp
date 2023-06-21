#include "audio/AudioOpus/AudioOpusExporterOGG.h"
		
using namespace WarframeExporter::Audio;

void
AudioOpusExporterOGG::writeData(const AudioHeader& header, const AudioBody& body, const std::filesystem::path& outPath)
{
	std::ofstream out;
	out.open(outPath, std::ios::binary | std::ios::out | std::ofstream::trunc);

	writeOpusPage(header, 0, out);
	writeTagPage(header, 1, out);
	writeBodyPages(header, 2, body.data.data(), body.data.size(), out);

	out.close();
}
		
void
AudioOpusExporterOGG::writeOpusPage(const AudioHeader& header, int pageIndex, std::ofstream& outFile)
{
	OpusHeader opusHeader;
	opusHeader.version = 1;
	opusHeader.channels = header.channelCount;
	opusHeader.preSkip = 312;
	opusHeader.inputSampleRate = header.samplesPerSec;
	opusHeader.outputGain = 0;
	opusHeader.channelMappingFamily = 0;

	OggHeader opusOggHeader;
	opusOggHeader.version = 0;
	opusOggHeader.headerType = (int)OggHeaderTypeFlags::FIRST;
	opusOggHeader.granulePosition = 0;
	opusOggHeader.streamSerialNumber = 531113123;
	opusOggHeader.pageSequenceNumber = pageIndex;
	opusOggHeader.segmentTable = createSegmentTable(opusHeader.getByteSize(), 255);
	opusOggHeader.pageSegments = opusOggHeader.segmentTable.size();

	char* data = new char[opusHeader.getByteSize()];
	opusHeader.serialize(data);
	addChecksumAndWrite(opusOggHeader, data, opusHeader.getByteSize(), outFile);
	delete[] data;
}

void
AudioOpusExporterOGG::writeTagPage(const AudioHeader& header, int pageIndex, std::ofstream& outFile)
{
	OpusTags infoTag;
	infoTag.vendor = "Warframe";
	infoTag.comments = {"ARTIST=Warframe"};

	OggHeader tagsOggHeader;
	tagsOggHeader.version = 0;
	tagsOggHeader.headerType = 0;
	tagsOggHeader.granulePosition = 0;
	tagsOggHeader.streamSerialNumber = 531113123;
	tagsOggHeader.pageSequenceNumber = pageIndex;
	tagsOggHeader.segmentTable = createSegmentTable(infoTag.getByteSize(), 255);
	tagsOggHeader.pageSegments = tagsOggHeader.segmentTable.size();
	
	char* data = new char[infoTag.getByteSize()];
	infoTag.serialize(data);
	addChecksumAndWrite(tagsOggHeader, data, infoTag.getByteSize(), outFile);
	delete[] data;
}

void
AudioOpusExporterOGG::writeBodyPages(const AudioHeader& header, int pageIndex, const char* data, size_t dataLen, std::ofstream& outFile)
{
	size_t chunkSize = header.blockAllign * 50;
	uint64_t granulePosition = header.samplesPerSec;
	for (size_t cursor = 0; cursor < dataLen; cursor += chunkSize)
	{
		size_t upperCursor = std::min(cursor + chunkSize, dataLen);
		bool isLast = upperCursor < dataLen ? true : false;
		writeBodyPage(header, data + cursor, upperCursor - cursor, granulePosition, pageIndex++, isLast, outFile);
		granulePosition += header.samplesPerSec;
	}
}

void
AudioOpusExporterOGG::writeBodyPage(const AudioHeader& header, const char* data, size_t dataLen, uint64_t granPos, int pageIndex, bool isLast, std::ofstream& outFile)
{
	OggHeader bodyHeader;
	bodyHeader.version = 0;
	bodyHeader.headerType = 0;
	bodyHeader.granulePosition = granPos;
	bodyHeader.streamSerialNumber = 531113123;
	bodyHeader.pageSequenceNumber = pageIndex;
	bodyHeader.segmentTable = createSegmentTable(dataLen, header.blockAllign);
	bodyHeader.pageSegments = bodyHeader.segmentTable.size();

	addChecksumAndWrite(bodyHeader, data, dataLen, outFile);
}

void
AudioOpusExporterOGG::addChecksumAndWrite(OggHeader& oggHeader, const char* data, size_t dataLen, std::ofstream& outFile)
{
	oggHeader.checksum = 0;

	size_t pageSize = oggHeader.getByteSize() + dataLen;
	std::vector<char> pageBytes(pageSize);
	size_t headerDataOffset = oggHeader.serialize(pageBytes.data());
	memcpy(pageBytes.data() + headerDataOffset, data, dataLen);

	// Find checksum, then re-write
	// Not the most optimized, but future-proof
	oggHeader.checksum  = AudioOpusExporterOggChecksum::calculateHash((unsigned char*)pageBytes.data(), pageSize);
	oggHeader.serialize(pageBytes.data());

	outFile.write((char*)pageBytes.data(), pageBytes.size());
}

std::vector<char>
AudioOpusExporterOGG::createSegmentTable(int dataLen, int segmentSize)
{
	std::vector<char> segmentTable;

	for (int x = 0; x < dataLen; x += segmentSize)
	{
		int upperLimit = std::min(x + segmentSize, dataLen);
		int curSegmentSize = upperLimit - x;
		
		std::div_t div = std::div(curSegmentSize, 255);
		for (int y = 0; y < div.quot; y++)
			segmentTable.push_back(255);
		segmentTable.push_back(div.rem);
	}

	return segmentTable;
}
