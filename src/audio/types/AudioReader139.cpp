#include "audio/types/AudioReader139.h"

using namespace WarframeExporter::Audio;

void
AudioReader139::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, AudioHeader& outHeader)
{
	outHeader.compression = (AudioCompression)headerReader->readUInt32();
	headerReader->seek(28, std::ios::cur);
	outHeader.samplesPerSec = headerReader->readUInt32();
	outHeader.bitsPerSample = headerReader->readUInt8();
	outHeader.channelCount = headerReader->readUInt8();
	headerReader->seek(4, std::ios::cur);
	outHeader.averageBytesPerSecond = headerReader->readUInt32();
	outHeader.blockAllign = headerReader->readUInt16();
	outHeader.samplesPerBlock = headerReader->readUInt16();
	headerReader->seek(12, std::ios::cur);
	outHeader.size = headerReader->readUInt32();
}
