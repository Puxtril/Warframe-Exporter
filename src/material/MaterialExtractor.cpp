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
	MaterialConverter::replaceCurlyBracketsWithSquare(internal);
	return internal;
}

void
MaterialExtractor::addPackgesBinAttributes(LotusLib::PackagesReader& pkgs, LotusLib::LotusPath internalPath, MaterialExternal& materialExternal)
{
	LotusLib::PackageReader pkg = pkgs.getPackage("Misc").value();
	LotusLib::LotusPath currentPath = internalPath;

	pkgs.initilizePackagesBin();
	while (currentPath != "")
	{
		MaterialAttributes curAttributes;

		LotusLib::FileEntry entry = pkg.getFile(currentPath, LotusLib::READ_EXTRA_ATTRIBUTES);
		curAttributes.attributes = std::move(entry.extra.attributes);
		curAttributes.filePath = currentPath;

		materialExternal.attributeChain.push_back(std::move(curAttributes));
		currentPath = entry.extra.parent;
	}
}

void
MaterialExtractor::writeOut(const MaterialInternal& materialInternal, const std::filesystem::path& outputPath)
{
	std::string output = MaterialConverter::combineMaterial(materialInternal);

	std::ofstream out;
	out.open(outputPath, std::ios::out | std::ofstream::trunc);
	out.write(output.c_str(), output.size());
	out.close();
}

void
MaterialExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath, bool dryRun)
{
	MaterialExternal external = getExternalMaterial(&fileEntry.headerData, fileEntry.commonHeader);
	addPackgesBinAttributes(pkgs, fileEntry.internalPath, external);
	MaterialInternal internal = formatMaterial(external);
	if (!dryRun)
		writeOut(internal, outputPath);
}
