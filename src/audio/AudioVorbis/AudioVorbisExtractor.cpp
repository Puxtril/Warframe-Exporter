#include "audio/AudioVorbis/AudioVorbisExtractor.h"

using namespace WarframeExporter::Audio;

AudioVorbisExtractor*
AudioVorbisExtractor::getInstance()
{
	static AudioVorbisExtractor* instance = new AudioVorbisExtractor();
	return instance;
}

void
AudioVorbisExtractor::extract(LotusLib::FileEntry& fileEntry, const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, const std::filesystem::path& outputPath, const ExtractOptions options)
{
	AudioReader* reader = g_enumMapAudioVorbisReader[fileEntry.commonHeader.type];
	if (reader == nullptr)
		throw unknown_format_error("No Vorbis audio reader for type " + std::to_string(fileEntry.commonHeader.type));

	AudioHeader audioHeader;
	reader->readHeader(&fileEntry.header, fileEntry.commonHeader, audioHeader);

	if (findVorbisSetupHeader(audioHeader.channelCount, audioHeader.samplesPerSec) == nullptr)
		throw unknown_format_error("No Vorbis setup header for " + std::to_string(audioHeader.channelCount) + " channels at " + std::to_string(audioHeader.samplesPerSec) + " Hz");

	AudioBody audioBody;
	reader->readBody(audioHeader, &fileEntry.footer, &fileEntry.body, audioBody);

	if (!options.dryRun)
	{
		std::ofstream out;
		out.open(outputPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		AudioVorbisExporterOGG::writeData(audioHeader, audioBody, out);
		out.close();
	}
}

void
AudioVorbisExtractor::extract(LotusLib::FileEntry& fileEntry, const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, std::ostream& outStream, ExtractOptions options)
{
	AudioReader* reader = g_enumMapAudioVorbisReader[fileEntry.commonHeader.type];
	if (reader == nullptr)
		throw unknown_format_error("No Vorbis audio reader for type " + std::to_string(fileEntry.commonHeader.type));

	AudioHeader audioHeader;
	reader->readHeader(&fileEntry.header, fileEntry.commonHeader, audioHeader);

	if (findVorbisSetupHeader(audioHeader.channelCount, audioHeader.samplesPerSec) == nullptr)
		throw unknown_format_error("No Vorbis setup header for " + std::to_string(audioHeader.channelCount) + " channels at " + std::to_string(audioHeader.samplesPerSec) + " Hz");

	AudioBody audioBody;
	reader->readBody(audioHeader, &fileEntry.footer, &fileEntry.body, audioBody);

	if (!options.dryRun)
		AudioVorbisExporterOGG::writeData(audioHeader, audioBody, outStream);
}
