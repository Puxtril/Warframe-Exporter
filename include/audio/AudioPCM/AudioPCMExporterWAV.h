#pragma once

#include "audio/AudioStructs.h"
#include "ExporterExceptions.h"

#include <fstream>
#include <filesystem>

namespace WarframeExporter::Audio
{
	class AudioPCMExporterWAV
	{
	public:
		static void writeData(const AudioHeader& header, const AudioBody& body, std::ostream& outFile);

	private:
		static void writePCMHeader(const AudioHeader& header, std::ostream& outFile);
		static void writeADPCMHeader(const AudioHeader& header, std::ostream& outFile);
		static void writeBody(const AudioBody& body, const AudioHeader& header, std::ostream& outFile);
	};
}
