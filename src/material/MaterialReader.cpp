#include "material/MaterialReader.h"

using namespace WarframeExporter::Material;

// Taken from
// https://www.martinbroadhurst.com/how-to-split-a-string-in-c
std::vector<std::string>
MaterialReader::splitAttributes(const std::string& attributes)
{
    std::size_t current, previous = 0;
    char delim = '\n';
    std::vector<std::string> attributesSplit;

    current = attributes.find(delim);
    while (current != std::string::npos)
    {
        attributesSplit.push_back(attributes.substr(previous, current - previous));
        previous = current + 1;
        current = attributes.find(delim, previous);
    }
    attributesSplit.push_back(attributes.substr(previous, current - previous));

    return attributesSplit;
}

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
