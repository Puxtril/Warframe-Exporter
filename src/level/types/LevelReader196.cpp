#include "level/types/LevelReader196.h"

using namespace WarframeExporter::Level;

void
LevelReader196::readHeaderDebug(BinaryReaderBase& reader, LevelHeaderExternal& outHeader) const
{
	const static std::string pathLenMsg = "Internal path length Level header";
	uint32_t pathCount = reader.readUInt32();
	while (pathCount--)
	{
		uint32_t pathLen = reader.readUInt32(0, 500, pathLenMsg);
		reader.seek(pathLen, std::ios_base::cur);
	}
	const static std::string actionPathLenMsg = "Internal path length Level header";
	pathCount = reader.readUInt32();
	while (pathCount--)
	{
		uint32_t pathLen = reader.readUInt32(0, 500, actionPathLenMsg);
		reader.seek(pathLen, std::ios_base::cur);
	}
	const static std::string assetPathLenMsg = "Internal path length Level header";
	pathCount = reader.readUInt32();
	while (pathCount--)
	{
		uint32_t pathLen = reader.readUInt32(0, 500, assetPathLenMsg);
		reader.seek(pathLen, std::ios_base::cur);
	}

	uint32_t unkNum = reader.readUInt32();
	uint32_t levelObjCount = reader.readUInt32();
	const static std::string levelObjPathLenMsg = "Level object path length";
	outHeader.levelObjs.resize(levelObjCount);
	for (uint32_t x = 0; x < levelObjCount; x++)
	{
		uint32_t pathLen = reader.readUInt32(0, 500, levelObjPathLenMsg);
		outHeader.levelObjs[x].objPath = reader.readAsciiString(pathLen);
		reader.seek(7 * 4, std::ios_base::cur);
		pathLen = reader.readUInt32();
		reader.seek(pathLen, std::ios_base::cur);
		outHeader.levelObjs[x].unk1 = reader.readUInt32();
		outHeader.levelObjs[x].unk2 = reader.readUInt32();
	}

	uint32_t unkIntCount = reader.readUInt32();
	reader.seek(unkIntCount * 4, std::ios_base::cur);

	const static std::string finalIntMsg = "Final Int 0";
	reader.readUInt32(0, 0, finalIntMsg);

	if (reader.tell() != reader.getLength())
		throw unknown_format_error("Did not reach end of file");
}

void
LevelReader196::readBodyDebug(BinaryReaderBase& reader, const LevelHeaderExternal& extHeader) const
{
	std::vector<uint32_t> indices(extHeader.levelObjs.size());
	for (int x = 0; x < extHeader.levelObjs.size(); x++)
		indices[x] = reader.readUInt32();
	std::vector<uint32_t> test;
}

void
LevelReader196::readHeader(BinaryReaderBase& reader, LevelHeaderExternal& outHeader) const
{
	
}

