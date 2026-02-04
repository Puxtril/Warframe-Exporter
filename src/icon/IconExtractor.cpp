#include "icon/IconExtractor.h"

using namespace WarframeExporter::Icon;

IconExtractor*
IconExtractor::getInstance()
{
	static IconExtractor* instance = new IconExtractor();
	return instance;
}

std::vector<uint8_t>
IconExtractor::getExternalIcon(LotusLib::FileEntry& fileEntry)
{
	std::vector<uint8_t> data(fileEntry.body.getLength());
    fileEntry.body.readUInt8Array(data.data(), fileEntry.body.getLength());
	return data;
}

void
IconExtractor::extract(LotusLib::FileEntry& fileEntry, const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, const std::filesystem::path& outputPath, const ExtractOptions options)
{
	std::vector<uint8_t> data = getExternalIcon(fileEntry);

	if (!options.dryRun && !data.empty())
	{
        std::ofstream out;
        out.open(outputPath, std::ios::out | std::ofstream::trunc | std::ofstream::binary);
        out.write((char*)data.data(), data.size());
        out.close();
    }
}
