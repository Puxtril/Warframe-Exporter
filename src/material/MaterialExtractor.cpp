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
	std::stringstream ss;
	MaterialConverter::combineMaterial(ss, materialInternal);

	std::ofstream out;
	out.open(outputPath, std::ios::out | std::ofstream::trunc);
	std::string writeData = ss.str();
	out.write(writeData.c_str(), writeData.length());
	out.close();
}

void
MaterialExtractor::writeOut(const MaterialInternal& materialInternal, const std::filesystem::path& outputPath, LotusLib::PackagesReader& pkgsReader, const LotusLib::LotusPath& filePath)
{
	std::stringstream ss;
	MaterialConverter::combineMaterial(ss, materialInternal);
	MaterialConverter::addPackagesBinHeirarchy(ss, pkgsReader.getPackage("Misc").value(), filePath);

	std::ofstream out;
	out.open(outputPath, std::ios::out | std::ofstream::trunc);
	std::string writeData = ss.str();
	out.write(writeData.c_str(), writeData.length());
	out.close();
}

void
MaterialExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, ExtractOptions options)
{
	MaterialExternal external = getExternalMaterial(&fileEntry.headerData, fileEntry.commonHeader);
	addPackgesBinAttributes(pkgs, fileEntry.internalPath, external);
	MaterialInternal internal = formatMaterial(external);
	if (!options.dryRun)
		writeOut(internal, outputPath, pkgs, fileEntry.internalPath);
}
