#include "audio/AudioPCM/AudioPCMExporterWAV.h"

using namespace WarframeExporter::Audio;

void
AudioPCMExporterWAV::writeData(const AudioHeader& header, const AudioBody& body, std::ostream& outFile)
{
	if (header.compression == AudioCompression::PCM)
		writePCMHeader(header, outFile);
	else if (header.compression == AudioCompression::ADPCM)
		writeADPCMHeader(header, outFile);
	
	writeBody(body, header, outFile);
}

void
AudioPCMExporterWAV::writePCMHeader(const AudioHeader& header, std::ostream& outFile)
{
	int blockAllign = ((uint16_t)(header.channelCount * header.bitsPerSample)) >> 3;
	int bytesPerSec = header.samplesPerSec * blockAllign;

	const static uint32_t RIFF = 0x46464952;
	const static uint32_t WAVE = 0x45564157;
	const static uint32_t fmt  = 0x20746D66;
	const static uint32_t fmtChunkSize = 16;
	const static uint16_t fmtTag = 1;
	const static uint32_t data = 0x61746164;

	outFile.write((char*)&RIFF, 4);
	uint32_t writeSize = header.size + 32;
	outFile.write((char*)&writeSize, 4);
	outFile.write((char*)&WAVE, 4);
	outFile.write((char*)&fmt, 4);
	outFile.write((char*)&fmtChunkSize, 4);
	outFile.write((char*)&fmtTag, 2);
	outFile.write((char*)&header.channelCount, 2);
	outFile.write((char*)&header.samplesPerSec, 4);
	outFile.write((char*)&bytesPerSec, 4);
	outFile.write((char*)&blockAllign, 2);
	outFile.write((char*)&header.bitsPerSample, 2);
	outFile.write((char*)&data, 4);
	outFile.write((char*)&header.size, 4);
}

void
AudioPCMExporterWAV::writeADPCMHeader(const AudioHeader& header, std::ostream& outFile)
{
	const static uint32_t RIFF = 0x46464952;
	const static uint32_t WAVE = 0x45564157;
	const static uint32_t fmt  = 0x20746D66;
	const static uint32_t fmtChunkSize = 50;
	const static uint16_t fmtTag = 2;
	const static uint32_t data = 0x61746164;
	const static uint16_t extSize = 32;
	const static uint16_t coeffCount = 7;
	const static int16_t coefficients[7][2] = {
		{256, 0},
		{512, -256},
		{0, 0},
		{192, 64},
		{240, 0},
		{460, -208},
		{392, -232}
	};

	outFile.write((char*)&RIFF, 4);
	uint32_t writeSize = header.size + 66;
	outFile.write((char*)&writeSize, 4);
	outFile.write((char*)&WAVE, 4);
	outFile.write((char*)&fmt, 4);
	outFile.write((char*)&fmtChunkSize, 4);
	outFile.write((char*)&fmtTag, 2);
	outFile.write((char*)&header.channelCount, 2);
	outFile.write((char*)&header.samplesPerSec, 4);
	outFile.write((char*)&header.averageBytesPerSecond, 4);
	outFile.write((char*)&header.blockAllign, 2);
	outFile.write((char*)&header.bitsPerSample, 2);
	outFile.write((char*)&extSize, 2);
	outFile.write((char*)&header.samplesPerBlock, 2);
	outFile.write((char*)&coeffCount, 2);
	for (int x = 0; x < 7; x++)
	{		
		outFile.write((char*)&coefficients[x][0], 2);
		outFile.write((char*)&coefficients[x][1], 2);
	}
	outFile.write((char*)&data, 4);
	outFile.write((char*)&header.size, 4);
}

void
AudioPCMExporterWAV::writeBody(const AudioBody& body, const AudioHeader& header, std::ostream& outFile)
{
	if (header.size > body.data.size())
		throw unknown_format_error("Header size > body: " + std::to_string(header.size) + " > " + std::to_string(body.data.size()));
	size_t start = body.data.size() - header.size;
	outFile.write(body.data.data() + start, body.data.size());
}
