#include "level/LevelExtractor.h"

using namespace WarframeExporter::Level;

LevelExtractor*
LevelExtractor::getInstance()
{
	static LevelExtractor* instance = new LevelExtractor();
	return instance;
}

void
LevelExtractor::extract(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	
}

void
LevelExtractor::extractDebug(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData)
{

}
