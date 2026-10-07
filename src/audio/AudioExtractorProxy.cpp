#include "audio/AudioExtractorProxy.h"

using namespace WarframeExporter::Audio;

const std::string&
AudioExtractorProxy::getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::Buffered* hReader, WarframeExporter::ExtractOptions options) const
{
	AudioCompression compressionEnum = peekCompressionFormat(hReader);
	Extractor* extractor = g_enumMapAudioExtractor.at(LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)compressionEnum);
	return extractor->getOutputExtension(commonHeader, hReader, options);
}

AudioExtractorProxy*
AudioExtractorProxy::getInstance()
{
	static AudioExtractorProxy* instance = new AudioExtractorProxy();
	return instance;
}

void
AudioExtractorProxy::extract(LotusLib::FileEntry& fileEntry, const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, const std::filesystem::path& outputPath, const ExtractOptions options)
{
	AudioCompression compressionEnum = peekCompressionFormat(&fileEntry.header);
	Extractor* extractor = g_enumMapAudioExtractor.at(LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)compressionEnum);
	extractor->extract(fileEntry, pkgs, pkgsBin, outputPath, options);
}

AudioCompression
AudioExtractorProxy::peekCompressionFormat(BinaryReader::Buffered* headerReader) const
{
	int compressionFormat = headerReader->readUInt32();
	headerReader->seek(-4, std::ios::cur);
	AudioCompression format = (AudioCompression)compressionFormat;
	switch(format)
	{
		case AudioCompression::PCM:
		case AudioCompression::ADPCM:
		case AudioCompression::OPUS:
			return format;
		default:
			throw unknown_format_error("Unknown audio compression: " + std::to_string(compressionFormat));
	}
}
