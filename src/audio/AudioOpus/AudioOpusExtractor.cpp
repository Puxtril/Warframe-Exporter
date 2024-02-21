#include "audio/AudioOpus/AudioOpusExtractor.h"
#include "FileNode.h"
#include "Package.h"
#include "audio/AudioOpus/AudioOpusExporterOGG.h"
#include "audio/AudioOpus/EnumMapAudioOpusReader.h"

using namespace WarframeExporter::Audio;

AudioOpusExtractor*
AudioOpusExtractor::getInstance()
{
	static AudioOpusExtractor* instance = new AudioOpusExtractor();
	return instance;
}

void
AudioOpusExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	AudioReader* reader = g_enumMapAudioOpusReader[fileEntry.commonHeader.type];
	
	AudioHeader audioHeader;
	reader->readHeader(&fileEntry.headerData, ensmalleningData, fileEntry.commonHeader, audioHeader);

	AudioBody audioBody;
	reader->readBody(audioHeader, &fileEntry.fData, &fileEntry.bData, audioBody);

	AudioOpusExporterOGG::writeData(audioHeader, audioBody, outputPath);
}

void
AudioOpusExtractor::extractDebug(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData)
{
	AudioReader* reader = g_enumMapAudioOpusReader[fileEntry.commonHeader.type];
	reader->readHeaderDebug(&fileEntry.headerData, ensmalleningData, fileEntry.commonHeader);
}
