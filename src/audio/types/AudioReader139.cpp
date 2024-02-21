#include "audio/types/AudioReader139.h"
#include "ExporterExceptions.h"
#include "audio/AudioStructs.h"

using namespace WarframeExporter::Audio;

void
AudioReader139::readHeaderDebug(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header)
{
	uint32_t compressionFormat = headerReader->readUInt32();
	if (compressionFormat != (int)AudioCompression::ADPCM &&
		compressionFormat != (int)AudioCompression::PCM &&
		compressionFormat != (int)AudioCompression::OPUS)
	{
		throw unknown_format_error("Unknown compression format: " + std::to_string(compressionFormat));
	}

	headerReader->seek(4, std::ios::cur);
	headerReader->seek(24, std::ios::cur);

	headerReader->readUInt32();

	headerReader->readUInt8();
	headerReader->readUInt8();
	
	headerReader->readUInt32();
	headerReader->readUInt32();

	const static std::string blockAllignMsg = "Block Allign";
	headerReader->readUInt16(0, 1000, blockAllignMsg);

	// Samples per block
	const static std::string samplesPerBlockMsg = "Samples per Block";
	headerReader->readUInt16(32, 5000, samplesPerBlockMsg);
	
	headerReader->seek(12, std::ios::cur);
	
	// Size
	headerReader->readUInt32();
}

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
