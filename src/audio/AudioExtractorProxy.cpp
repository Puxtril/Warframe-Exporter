#include "audio/AudioExtractorProxy.h"

using namespace WarframeExporter::Audio;

const std::string&
AudioExtractorProxy::getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::BinaryReaderBuffered* hReader) const
{
	AudioCompression compressionEnum = peekCompressionFormat(hReader);
	Extractor* extractor = g_enumMapAudioExtractor.at(LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)compressionEnum);
	return extractor->getOutputExtension(commonHeader, hReader);
}

AudioExtractorProxy*
AudioExtractorProxy::getInstance()
{
	static AudioExtractorProxy* instance = new AudioExtractorProxy();
	return instance;
}

void
AudioExtractorProxy::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, bool dryRun)
{
	AudioCompression compressionEnum = peekCompressionFormat(&fileEntry.headerData);
	Extractor* extractor = g_enumMapAudioExtractor.at(LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, (int)compressionEnum);
	extractor->extract(fileEntry, pkgs, outputPath, dryRun);
}

AudioCompression
AudioExtractorProxy::peekCompressionFormat(BinaryReader::BinaryReaderBuffered* headerReader) const
{
	int compressionFormat = headerReader->readUInt32();
	headerReader->seek(-4, std::ios::cur);
	return (AudioCompression)compressionFormat;
}
