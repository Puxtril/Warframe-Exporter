#pragma once

#include "audio/AudioStructs.h"

#include <fstream>
#include <istream>
#include <filesystem>

namespace WarframeExporter::Audio
{
	class AudioPCMExporterWAV
	{
	public:
		static void writeData(const AudioHeader& header, const AudioBody& body, const std::filesystem::path& outPath);

	private:
		static void writePCMHeader(const AudioHeader& header, std::ofstream& outFile);
		static void writeADPCMHeader(const AudioHeader& header, std::ofstream& outFile);
		static void writeBody(const AudioBody& body, std::ofstream& outFile);
	};
}
