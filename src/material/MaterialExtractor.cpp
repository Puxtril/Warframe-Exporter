#include "material/MaterialExtractor.h"

using namespace WarframeExporter::Material;

MaterialExtractor*
MaterialExtractor::getInstance()
{
	static MaterialExtractor* instance = new MaterialExtractor();
	return instance;
}

MaterialExternal
MaterialExtractor::getExternalMaterial(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader)
{
	MaterialReader* reader = g_enumMapMaterial[commonHeader.type];
	MaterialExternal external = reader->readData(headerReader, commonHeader);
	return external;
}

MaterialInternal
MaterialExtractor::formatMaterial(const MaterialExternal& materialExternal)
{
	MaterialInternal internal = MaterialConverter::convertMaterial(materialExternal);
	return internal;
}

void
MaterialExtractor::writeOut(const MaterialInternal& materialInternal, const std::filesystem::path& outputPath)
{
	std::ofstream out;
	out.open(outputPath, std::ios::out | std::ofstream::trunc);
	out.write(materialInternal.formatted.c_str(), materialInternal.formatted.size());
	out.close();
}

void
MaterialExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	MaterialExternal external = getExternalMaterial(&fileEntry.headerData, fileEntry.commonHeader);
	MaterialInternal internal = formatMaterial(external);
	writeOut(internal, outputPath);
}

void
MaterialExtractor::extractDebug(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData)
{

}
