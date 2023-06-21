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
AudioOpusExtractor::extract(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	AudioReader* reader = g_enumMapAudioOpusReader[header.type];
	
	AudioHeader audioHeader;
	reader->readHeader(hReader, ensmalleningData, header, audioHeader);

	BinaryReaderBuffered* fReader = nullptr;
	try
	{
		const LotusLib::FileEntries::FileNode* entry = pkgDir[package][LotusLib::PackageTrioType::F]->getFileEntry(internalpath);
		std::unique_ptr<char[]> rawData = pkgDir[package][LotusLib::PackageTrioType::F]->getDataAndDecompress(entry);
		fReader = new BinaryReaderBuffered((uint8_t*)rawData.release(), entry->getLen());
	} catch (std::exception&) { }
	
	BinaryReaderBuffered* bReader = nullptr;
	try
	{
		const LotusLib::FileEntries::FileNode* entry = pkgDir[package][LotusLib::PackageTrioType::B]->getFileEntry(internalpath);
		std::unique_ptr<char[]> rawData = pkgDir[package][LotusLib::PackageTrioType::B]->getDataAndDecompress(entry);
		bReader = new BinaryReaderBuffered((uint8_t*)rawData.release(), entry->getLen());
	} catch (std::exception&) { }

	AudioBody audioBody;
	reader->readBody(audioHeader, fReader, bReader, audioBody);

	AudioOpusExporterOGG::writeData(audioHeader, audioBody, outputPath);
}

void
AudioOpusExtractor::extractDebug(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData)
{
	AudioReader* reader = g_enumMapAudioOpusReader[header.type];
	reader->readHeaderDebug(hReader, ensmalleningData, header);
}
