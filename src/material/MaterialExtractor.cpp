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
MaterialExtractor::writeOut(const MaterialInternal& materialInternal, const std::filesystem::path& outputPath, LotusLib::PackagesReader& pkgsReader, const std::string& filePath)
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
	MaterialInternal internal = MaterialConverter::convertMaterial(external, fileEntry.internalPath, pkgs);

	if (!options.dryRun)
		writeOut(internal, outputPath, pkgs, fileEntry.internalPath);
}
