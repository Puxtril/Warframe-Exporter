#include "CommonHeader.h"

using namespace WarframeExporter;

CommonFileHeader::CommonFileHeader(BinaryReaderBase& reader)
{
	reader.readUInt8Array(&hash[0], 16);

	static const std::string debugMsgSourcePath = "Common Header - Source Path Count";
	int sourcePathCount = reader.readUInt32(0, 1500, debugMsgSourcePath);

	static const std::string debugMsgSourcePathLen = "Common Header - Source Path Length";
	for (int x = 0; x < sourcePathCount; x++)
	{
		uint32_t curPathLen = reader.readUInt32(0, 200, debugMsgSourcePathLen);
		sourcePaths.push_back(reader.readAsciiString(curPathLen));
	}

	int attributeLen = reader.readUInt32();
	attributes = reader.readAsciiString(attributeLen);

	if (attributeLen > 0)
		reader.seek(1, std::ios_base::cur);

	typeEnum = reader.readUInt32();
}

const char*
CommonFileHeader::getHash() const
{
	return (char*)hash;
}

const std::vector<std::string>
CommonFileHeader::getPaths() const
{
	return sourcePaths;

}
const std::string&
CommonFileHeader::getAttributes() const
{
	return attributes;
}

uint32_t
CommonFileHeader::getEnum() const
{
	return typeEnum;
}