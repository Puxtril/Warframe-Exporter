#include "level/types/LevelReader20x.h"

using namespace WarframeExporter::Level;

void
LevelReader20x::readHeader(BinaryReader::BinaryReaderBuffered& reader, LevelHeaderExternal& outHeader) const
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

	reader.readUInt32();
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
LevelReader20x::readBody(BinaryReader::BinaryReaderBuffered& reader, const LevelHeaderExternal& extHeader, LevelBodyExternal& outBody) const
{
	std::vector<uint32_t> attributeLens(extHeader.levelObjs.size());
	reader.readUInt32Array(attributeLens.data(), extHeader.levelObjs.size());
	outBody.attributes.resize(extHeader.levelObjs.size());

	for (size_t x = 0; x < extHeader.levelObjs.size(); x++)
	{
		uint32_t curAttrLen = attributeLens[x];
		outBody.attributes[x].resize((curAttrLen + 1));
		reader.readUInt8Array((uint8_t*)outBody.attributes[x].data(), curAttrLen + 1);
	}
}
