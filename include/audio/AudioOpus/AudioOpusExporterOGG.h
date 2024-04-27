#pragma once

#include "audio/AudioStructs.h"
#include "audio/AudioOpus/AudioOpusExporterOggChecksum.h"
#include "audio/AudioOpus/AudioOpusExporterOggStructs.h"

#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <cstring>

namespace WarframeExporter::Audio
{

	class AudioOpusExporterOGG
	{
	public:
		static void writeData(const AudioHeader& header, const AudioBody& body, const std::filesystem::path& outPath);

	private:	
		static void writeOpusPage(const AudioHeader& header, int pageIndex, std::ofstream& outFile);
		static void writeTagPage(const AudioHeader& header, int pageIndex, std::ofstream& outFile);
		
		static void writeBodyPages(const AudioHeader& header, int pageIndex, const char* data, size_t dataLen, std::ofstream& outFile);
		static void writeBodyPage(const AudioHeader& header, const char* data, size_t dataLen, uint64_t granPos, int pageIndex, bool isLast, std::ofstream& outFile);
	
		static void addChecksumAndWrite(OggHeader& oggHeader, const char* data, size_t dataLen, std::ofstream& outFile);

		static std::vector<uint8_t> createSegmentTable(int dataLen, int segmentSize);
	};
}
