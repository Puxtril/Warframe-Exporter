#include "animation/AnimationExtractor.h"

using namespace WarframeExporter::Animation;

AnimationExtractor*
AnimationExtractor::getInstance()
{
	static AnimationExtractor* instance = new AnimationExtractor();
	return instance;
}

void
AnimationExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, ExtractOptions options)
{
	AnimationHeaderExternal extHeader;
	AnimationBodyExternal extBody;

	AnimationReader* reader = g_enumMapAnimation.at(pkgs.getGame(), (int)fileEntry.commonHeader.type);
	reader->readHeader(&fileEntry.headerData, fileEntry.commonHeader, extHeader);
	reader->readBody(&fileEntry.bData, extHeader, fileEntry.commonHeader, extBody);

	AnimationData combined;
	AnimationConverter::convertAnimation(extHeader, extBody, combined);

	gltfAnimation outAnimation;
	outAnimation.addAnimationData(combined);
	outAnimation.save(outputPath);
}
