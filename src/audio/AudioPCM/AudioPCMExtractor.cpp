#include "audio/AudioPCM/AudioPCMExtractor.h"

using namespace WarframeExporter::Audio;

AudioPCMExtractor*
AudioPCMExtractor::getInstance()
{
	static AudioPCMExtractor* instance = new AudioPCMExtractor();
	return instance;
}

void
AudioPCMExtractor::extract(LotusLib::FileEntry& fileEntry, const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, const std::filesystem::path& outputPath, const ExtractOptions options)
{
	AudioReader* reader = g_enumMapAudioPCMReader[fileEntry.commonHeader.type];
	
	AudioHeader audioHeader;
	reader->readHeader(&fileEntry.header, fileEntry.commonHeader, audioHeader);

	AudioBody audioBody;
	reader->readBody(audioHeader, &fileEntry.footer, &fileEntry.body, audioBody);

	if (!options.dryRun)
	{
		std::ofstream out;
		out.open(outputPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		AudioPCMExporterWAV::writeData(audioHeader, audioBody, out);
		out.close();
	}
}

void
AudioPCMExtractor::extract(LotusLib::FileEntry& fileEntry, const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, std::ostream& outStream, ExtractOptions options)
{
	AudioReader* reader = g_enumMapAudioPCMReader[fileEntry.commonHeader.type];
	
	AudioHeader audioHeader;
	reader->readHeader(&fileEntry.header, fileEntry.commonHeader, audioHeader);

	AudioBody audioBody;
	reader->readBody(audioHeader, &fileEntry.footer, &fileEntry.body, audioBody);

	if (!options.dryRun)
		AudioPCMExporterWAV::writeData(audioHeader, audioBody, outStream);
}
