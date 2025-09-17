#include "audio/AudioPCM/AudioPCMExtractor.h"

using namespace WarframeExporter::Audio;

AudioPCMExtractor*
AudioPCMExtractor::getInstance()
{
	static AudioPCMExtractor* instance = new AudioPCMExtractor();
	return instance;
}

void
AudioPCMExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, ExtractOptions options)
{
	AudioReader* reader = g_enumMapAudioPCMReader[fileEntry.commonHeader.type];
	
	AudioHeader audioHeader;
	reader->readHeader(&fileEntry.headerData, fileEntry.commonHeader, audioHeader);

	AudioBody audioBody;
	reader->readBody(audioHeader, &fileEntry.fData, &fileEntry.bData, audioBody);

	if (!options.dryRun)
	{
		std::ofstream out;
		out.open(outputPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		AudioPCMExporterWAV::writeData(audioHeader, audioBody, out);
		out.close();
	}
}

void
AudioPCMExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, std::ostream& outStream, ExtractOptions options)
{
	AudioReader* reader = g_enumMapAudioPCMReader[fileEntry.commonHeader.type];
	
	AudioHeader audioHeader;
	reader->readHeader(&fileEntry.headerData, fileEntry.commonHeader, audioHeader);

	AudioBody audioBody;
	reader->readBody(audioHeader, &fileEntry.fData, &fileEntry.bData, audioBody);

	if (!options.dryRun)
		AudioPCMExporterWAV::writeData(audioHeader, audioBody, outStream);
}
