#include "material/ExtractorMaterial.h"

using namespace WarframeExporter::Material;

ExtractorMaterial&
ExtractorMaterial::getInstance()
{
	static ExtractorMaterial instance;
	return instance;
}

void
ExtractorMaterial::getExtraNames(BinaryReaderBuffered* headerReader, std::vector<std::string>& outPaths)
{
	uint32_t pathCount = headerReader->readUInt32();
	for (uint32_t x = 0; x < pathCount; x++)
	{
		uint32_t nameLen = headerReader->readUInt32();
		std::string name = headerReader->readAsciiString(nameLen);
		outPaths.push_back(name);
	}
}

void
ExtractorMaterial::extract(const CommonFileHeader& header, BinaryReaderBuffered* hReader, BinaryReaderBuffered* bReader, BinaryReaderBuffered* fReader, const Ensmallening& ensmalleningData, const std::string& outputPath)
{
	std::ofstream out;
	out.open(outputPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
	
	out.write(header.getAttributes().c_str(), header.getAttributes().length());
	
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
ExtractorMaterial::extractDebug(const CommonFileHeader& header, BinaryReaderBuffered* hReader, BinaryReaderBuffered* bReader, BinaryReaderBuffered* fReader, const Ensmallening& ensmalleningData)
{

}