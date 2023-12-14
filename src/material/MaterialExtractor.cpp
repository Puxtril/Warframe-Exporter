#include "material/MaterialExtractor.h"

using namespace WarframeExporter::Material;

MaterialExtractor*
MaterialExtractor::getInstance()
{
	static MaterialExtractor* instance = new MaterialExtractor();
	return instance;
}

void
MaterialExtractor::getExtraNames(BinaryReaderBuffered* headerReader, std::vector<std::string>& outPaths)
{
	uint32_t pathCount = headerReader->readUInt32();
	for (uint32_t x = 0; x < pathCount; x++)
	{
		uint32_t nameLen = headerReader->readUInt32();
		std::string name = headerReader->readAsciiString(nameLen);
		outPaths.push_back(name);
	}
}

std::vector<std::string>
MaterialExtractor::getHlm3Textures(BinaryReaderBuffered* headerReader)
{
	std::vector<std::string> outPaths;
	for (int x = 0; x < 3; x++)
	{
		uint32_t nameLen = headerReader->readUInt32();
		std::string name = headerReader->readAsciiString(nameLen);
		outPaths.push_back(name);
	}
	return outPaths;
}

void
MaterialExtractor::extract(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	std::ofstream out;
	out.open(outputPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
	
	out.write(header.attributes.c_str(), header.attributes.length());

	if (header.type == (int)MaterialType::MATERIAL_214 ||
		header.type == (int)MaterialType::MATERIAL_216)
	{
		std::vector<std::string> threeTextures = getHlm3Textures(hReader);
		out.write("\nHLM Textures:\n", 15);
		for (const auto& name : threeTextures)
		{
			out.write(name.c_str(), name.length());
			out.write("\n", 1);
		}
	}
	
	std::vector<std::string> extraNames;
	getExtraNames(hReader, extraNames);

	out.write("\n", 1);
	for (const auto& name : extraNames)
	{
		out.write(name.c_str(), name.length());
		out.write("\n", 1);
	}

	out.close();
}

void
MaterialExtractor::extractDebug(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData)
{

}
