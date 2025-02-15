#include "audio/AudioPCM/AudioPCMExtractor.h"

using namespace WarframeExporter::Audio;

AudioPCMExtractor*
AudioPCMExtractor::getInstance()
{
	static AudioPCMExtractor* instance = new AudioPCMExtractor();
	return instance;
}

void
AudioPCMExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, bool dryRun)
{
	AudioReader* reader = g_enumMapAudioPCMReader[fileEntry.commonHeader.type];
	
	AudioHeader audioHeader;
	reader->readHeader(&fileEntry.headerData, fileEntry.commonHeader, audioHeader);

	AudioBody audioBody;
	reader->readBody(audioHeader, &fileEntry.fData, &fileEntry.bData, audioBody);

	if (!dryRun)
		AudioPCMExporterWAV::writeData(audioHeader, audioBody, outputPath);
}
