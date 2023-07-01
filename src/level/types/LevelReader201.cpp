#include "level/types/LevelReader201.h"

using namespace WarframeExporter::Level;

void
LevelReader201::readHeaderDebug(BinaryReaderBase& reader) const
{
	const static std::string pathLenMsg = "Internal path length Level header";
	uint32_t pathCount1 = reader.readUInt32();
	std::cout << pathCount1 << " ";
	while (pathCount1--)
	{
		uint32_t pathLen = reader.readUInt32(0, 500, pathLenMsg);
		reader.seek(pathLen, std::ios_base::cur);
	}
	const static std::string actionPathLenMsg = "Internal path length Level header";
	uint32_t pathCount2 = reader.readUInt32();
	std::cout << pathCount2 << " ";
	while (pathCount2--)
	{
		uint32_t pathLen = reader.readUInt32(0, 500, actionPathLenMsg);
		reader.seek(pathLen, std::ios_base::cur);
	}
	const static std::string assetPathLenMsg = "Internal path length Level header";
	uint32_t pathCount3 = reader.readUInt32();
	std::cout << pathCount3 << " ";
	while (pathCount3--)
	{
		uint32_t pathLen = reader.readUInt32(0, 500, assetPathLenMsg);
		reader.seek(pathLen, std::ios_base::cur);
	}

	uint32_t unkNum = reader.readUInt32();
	uint32_t levelObjCount = reader.readUInt32();
	const static std::string levelObjPathLenMsg = "Level object path length";
	for (uint32_t x = 0; x < levelObjCount; x++)
	{
		uint32_t pathLen = reader.readUInt32(0, 500, levelObjPathLenMsg);
		reader.readAsciiString(pathLen);
		reader.seek(7 * 4, std::ios_base::cur);
		pathLen = reader.readUInt32();
		reader.seek(pathLen, std::ios_base::cur);
		reader.readUInt32();
		reader.readUInt32();
	}

	std::cout << levelObjCount << std::endl;
}

void
LevelReader201::readBodyDebug(BinaryReaderBase& reader, const LevelHeaderExternal& extHeader) const
{
	std::vector<uint32_t> indices(extHeader.levelObjs.size());
	for (int x = 0; x < extHeader.levelObjs.size(); x++)
		indices[x] = reader.readUInt32();
}

void
LevelReader201::readHeader(BinaryReaderBase& reader, LevelHeaderExternal& outHeader) const
{	
	uint32_t pathCount1 = reader.readUInt32();
	outHeader.paths.resize(pathCount1);
	for(uint32_t x = 0; x < pathCount1; x++)
	{
		uint32_t pathLen = reader.readUInt32();
		outHeader.paths[x] = reader.readAsciiString(pathLen);
	}
	
	uint32_t actionPathCount = reader.readUInt32();
	outHeader.actionPaths.resize(actionPathCount);
	for (uint32_t x = 0; x < actionPathCount; x++)
	{
		uint32_t pathLen = reader.readUInt32();
		outHeader.actionPaths[x] = reader.readAsciiString(pathLen);
	}

	uint32_t assetPathCount = reader.readUInt32();
	outHeader.assetPaths.resize(assetPathCount);
	for (uint32_t x = 0; x < assetPathCount; x++)
	{
		uint32_t pathLen = reader.readUInt32();
		outHeader.assetPaths[x] = reader.readAsciiString(pathLen);
	}

	uint32_t unkNum = reader.readUInt32();
	uint32_t levelObjCount = reader.readUInt32();
	outHeader.levelObjs.resize(levelObjCount);
	for (uint32_t x = 0; x < levelObjCount; x++)
	{
		LevelObjectHeaderExternal& curLevel = outHeader.levelObjs[x];

		uint32_t pathLen = reader.readUInt32();
		curLevel.objTypePath = reader.readAsciiString(pathLen);
		
		curLevel.pos.x = reader.readFloat();
		curLevel.pos.y = reader.readFloat();
		curLevel.pos.z = reader.readFloat();
		
		curLevel.rot.x = reader.readFloat();
		curLevel.rot.y = reader.readFloat();
		curLevel.rot.z = reader.readFloat();
		curLevel.rot.w = reader.readFloat();

		pathLen = reader.readUInt32();
		curLevel.objName = reader.readAsciiString(pathLen);
		
		curLevel.meshInstanceIndex = reader.readUInt32();
		curLevel.unk2 = reader.readUInt32();
	}
}

void
LevelReader201::readBody(BinaryReaderBase& reader, const LevelHeaderExternal& extHeader, LevelBodyExternal& outBody) const
{
	outBody.unkIndices.resize(extHeader.levelObjs.size());
	for (int x = 0; x < extHeader.levelObjs.size(); x++)
		outBody.unkIndices[x] = reader.readUInt32();

	int remainingBytes = reader.getLength() - reader.tell();
	outBody.attributes = reader.readAsciiString(remainingBytes);
}
