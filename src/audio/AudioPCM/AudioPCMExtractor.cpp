#include "audio/AudioPCM/AudioPCMExtractor.h"
#include "audio/AudioPCM/AudioPCMExporterWAV.h"

using namespace WarframeExporter::Audio;

AudioPCMExtractor*
AudioPCMExtractor::getInstance()
{
	static AudioPCMExtractor* instance = new AudioPCMExtractor();
	return instance;
}

void
AudioPCMExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	AudioReader* reader = g_enumMapAudioPCMReader[fileEntry.commonHeader.type];
	
	AudioHeader audioHeader;
	reader->readHeader(&fileEntry.headerData, ensmalleningData, fileEntry.commonHeader, audioHeader);

	AudioBody audioBody;
	reader->readBody(audioHeader, &fileEntry.fData, &fileEntry.bData, audioBody);

	AudioPCMExporterWAV::writeData(audioHeader, audioBody, outputPath);
}

void
AudioPCMExtractor::extractDebug(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData)
{
	AudioReader* reader = g_enumMapAudioPCMReader[fileEntry.commonHeader.type];
	reader->readHeaderDebug(&fileEntry.headerData, ensmalleningData, fileEntry.commonHeader);
}
