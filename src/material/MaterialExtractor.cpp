#include "material/MaterialExtractor.h"

using namespace WarframeExporter::Material;

MaterialExtractor*
MaterialExtractor::getInstance()
{
	static MaterialExtractor* instance = new MaterialExtractor();
	return instance;
}

MaterialExternal
MaterialExtractor::getExternalMaterial(BinaryReader::Buffered* headerReader, const LotusLib::CommonHeader& commonHeader)
{
	MaterialReader* reader = g_enumMapMaterial[commonHeader.type];
	MaterialExternal external = reader->readData(headerReader, commonHeader);
	return external;
}

void
MaterialExtractor::writeOut(const MaterialInternal& materialInternal, const std::filesystem::path& outputPath, ExtractOptions options)
{
	std::string outTxt;
	if (options.materialExtractMode == MaterialExtractType::TXT)
		outTxt = MaterialExporter::combineMaterialToTxt(materialInternal);
	else if (options.materialExtractMode == MaterialExtractType::JSON)
		outTxt = MaterialExporter::combineMaterialToJson(materialInternal);

	std::ofstream out;
	out.open(outputPath, std::ios::out | std::ofstream::trunc);
	out.write(outTxt.c_str(), outTxt.length());
	out.close();
}

void
MaterialExtractor::extract(LotusLib::FileEntry& fileEntry, const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, const std::filesystem::path& outputPath, const ExtractOptions options)
{
	MaterialExternal external = getExternalMaterial(&fileEntry.header, fileEntry.commonHeader);
	MaterialInternal internal = MaterialConverter::convertMaterial(external, LotusLib::getFullPath(fileEntry.headerNode), pkgs, pkgsBin);

	if (!options.dryRun)
		writeOut(internal, outputPath, options);
}
