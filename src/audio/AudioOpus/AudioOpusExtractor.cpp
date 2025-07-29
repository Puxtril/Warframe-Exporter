#include "audio/AudioOpus/AudioOpusExtractor.h"

using namespace WarframeExporter::Audio;

AudioOpusExtractor*
AudioOpusExtractor::getInstance()
{
	static AudioOpusExtractor* instance = new AudioOpusExtractor();
	return instance;
}

void
AudioOpusExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, ExtractOptions options)
{
	AudioReader* reader = g_enumMapAudioOpusReader[fileEntry.commonHeader.type];
	
	AudioHeader audioHeader;
	reader->readHeader(&fileEntry.headerData, fileEntry.commonHeader, audioHeader);

	AudioBody audioBody;
	reader->readBody(audioHeader, &fileEntry.fData, &fileEntry.bData, audioBody);

	if (!options.dryRun)
		AudioOpusExporterOGG::writeData(audioHeader, audioBody, outputPath);
}
