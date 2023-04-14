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
AudioPCMExtractor::extract(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	AudioReader* reader = g_enumMapAudioPCMReader[header.type];
	
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

	AudioPCMExporterWAV::writeData(audioHeader, audioBody, outputPath);
}

void
AudioPCMExtractor::extractDebug(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData)
{
	AudioReader* reader = g_enumMapAudioPCMReader[header.type];
	reader->readHeaderDebug(hReader, ensmalleningData, header);
}
