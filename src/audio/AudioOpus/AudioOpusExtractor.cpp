#include "audio/AudioOpus/AudioOpusExtractor.h"

using namespace WarframeExporter::Audio;

AudioOpusExtractor*
AudioOpusExtractor::getInstance()
{
	static AudioOpusExtractor* instance = new AudioOpusExtractor();
	return instance;
}

void
AudioOpusExtractor::extract(LotusLib::FileEntry& fileEntry, const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, const std::filesystem::path& outputPath, const ExtractOptions options)
{
	AudioReader* reader = g_enumMapAudioOpusReader[fileEntry.commonHeader.type];
	
	AudioHeader audioHeader;
	reader->readHeader(&fileEntry.header, fileEntry.commonHeader, audioHeader);

	AudioBody audioBody;
	reader->readBody(audioHeader, &fileEntry.footer, &fileEntry.body, audioBody);

	if (!options.dryRun)
	{
		std::ofstream out;
		out.open(outputPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		AudioOpusExporterOGG::writeData(audioHeader, audioBody, out);
		out.close();
	}
}

void
AudioOpusExtractor::extract(LotusLib::FileEntry& fileEntry, const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, std::ostream& outStream, ExtractOptions options)
{
	AudioReader* reader = g_enumMapAudioOpusReader[fileEntry.commonHeader.type];
	
	AudioHeader audioHeader;
	reader->readHeader(&fileEntry.header, fileEntry.commonHeader, audioHeader);

	AudioBody audioBody;
	reader->readBody(audioHeader, &fileEntry.footer, &fileEntry.body, audioBody);

	if (!options.dryRun)
		AudioOpusExporterOGG::writeData(audioHeader, audioBody, outStream);
}
