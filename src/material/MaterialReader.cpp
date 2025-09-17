#include "material/MaterialReader.h"

using namespace WarframeExporter::Material;

std::vector<std::string>
MaterialReader::getExtraNames(BinaryReader::BinaryReaderBuffered* headerReader)
{
    std::vector<std::string> outPaths;

    uint32_t pathCount = headerReader->readUInt32();
	for (uint32_t x = 0; x < pathCount; x++)
	{
		uint32_t nameLen = headerReader->readUInt32();
		std::string name = headerReader->readAsciiString(nameLen);
		outPaths.push_back(name);
	}

    return outPaths;
}

std::vector<std::string>
MaterialReader::getHlm3Textures(BinaryReader::BinaryReaderBuffered* headerReader)
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

std::vector<std::string>
MaterialReader::getShaderSet1(BinaryReader::BinaryReaderBuffered* headerReader, int count)
{
    std::vector<std::string> shaders;
    for (int i = 0; i < count; i++)
    {
        uint32_t len = headerReader->readUInt32();
        std::string shader = headerReader->readAsciiString(len);
        shaders.push_back(shader);
        headerReader->seek(1, std::ios::cur);
    }
    return shaders;
}

std::vector<std::string>
MaterialReader::getShaderSet2(BinaryReader::BinaryReaderBuffered* headerReader, int count)
{
    std::vector<std::string> shaders;
    for (int i = 0; i < count; i++)
    {
        uint32_t len = headerReader->readUInt32();
        std::string shader = headerReader->readAsciiString(len);
        shaders.push_back(shader);
    }
    return shaders;
}
