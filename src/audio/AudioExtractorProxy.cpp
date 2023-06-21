#include "audio/AudioExtractorProxy.h"

using namespace WarframeExporter::Audio;

const std::string&
AudioExtractorProxy::getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReaderBuffered* hReader) const
{
	AudioCompression compressionEnum = peekCompressionFormat(hReader);
	Extractor* extractor = g_enumMapAudioExtractor[(int)compressionEnum];
	return extractor->getOutputExtension(commonHeader, hReader);
}

AudioExtractorProxy*
AudioExtractorProxy::getInstance()
{
	static AudioExtractorProxy* instance = new AudioExtractorProxy();
	return instance;
}

void
AudioExtractorProxy::extract(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	AudioCompression compressionEnum = peekCompressionFormat(hReader);
	Extractor* extractor = g_enumMapAudioExtractor[(int)compressionEnum];
	extractor->extract(header, hReader, pkgDir, package, internalpath, ensmalleningData, outputPath);
}

void
AudioExtractorProxy::extractDebug(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData)
{
	AudioCompression compressionEnum = peekCompressionFormat(hReader);
	Extractor* extractor = g_enumMapAudioExtractor[(int)compressionEnum];
	extractor->extractDebug(header, hReader, pkgDir, package, internalpath, ensmalleningData);
}

AudioCompression
AudioExtractorProxy::peekCompressionFormat(BinaryReaderBuffered* headerReader) const
{
	int compressionFormat = headerReader->readUInt32();
	headerReader->seek(-4, std::ios::cur);
	return (AudioCompression)compressionFormat;
}
