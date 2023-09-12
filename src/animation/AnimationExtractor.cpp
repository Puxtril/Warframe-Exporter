#include "animation/AnimationExtractor.h"

using namespace WarframeExporter::Animation;

AnimationExtractor*
AnimationExtractor::getInstance()
{
	static AnimationExtractor* instance = new AnimationExtractor();
	return instance;
}

void
AnimationExtractor::extract(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalPath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	const LotusLib::FileEntries::FileNode* bEntry = pkgDir[package][LotusLib::PackageTrioType::B]->getFileEntry(internalPath);
	std::unique_ptr<char[]> bRawData = pkgDir[package][LotusLib::PackageTrioType::B]->getDataAndDecompress(bEntry);
	BinaryReaderBuffered bReader = BinaryReaderBuffered((uint8_t*)bRawData.release(), bEntry->getLen());

	AnimationHeaderExternal extHeader;
	AnimationBodyExternal extBody;

	AnimationReader* reader = g_enumMapAnimation[(int)header.type];
	reader->readHeader(hReader, ensmalleningData, header, extHeader);
	reader->readBody(&bReader, extHeader, ensmalleningData, header, extBody);

	AnimationData combined;
	AnimationConverter::convertAnimation(extHeader, extBody, combined);

	gltfAnimation outAnimation;
	outAnimation.addAnimationData(combined);
	outAnimation.save(outputPath);
}

void
AnimationExtractor::extractDebug(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalPath, const Ensmallening& ensmalleningData)
{
	const LotusLib::FileEntries::FileNode* bEntry = pkgDir[package][LotusLib::PackageTrioType::B]->getFileEntry(internalPath);
	std::unique_ptr<char[]> bRawData = pkgDir[package][LotusLib::PackageTrioType::B]->getDataAndDecompress(bEntry);
	BinaryReaderBuffered bReader = BinaryReaderBuffered((uint8_t*)bRawData.release(), bEntry->getLen());

	AnimationReader* reader = g_enumMapAnimation[(int)header.type];

	size_t startPos = hReader->tell();
	reader->readHeaderDebug(hReader, ensmalleningData, header);

	hReader->seek(startPos, std::ios_base::beg);
	AnimationHeaderExternal extHeader;
	reader->readHeader(hReader, ensmalleningData, header, extHeader);
	
	reader->readBodyDebug(&bReader, extHeader, ensmalleningData, header);
}