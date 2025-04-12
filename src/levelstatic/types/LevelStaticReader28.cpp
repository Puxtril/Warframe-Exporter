#include "levelstatic/types/LevelStaticReader28.h"

using namespace WarframeExporter::LevelStatic;

void
LevelStaticReader28::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, LevelStaticHeaderExternal& outHeader)
{
    uint32_t unkPaths = headerReader->readUInt32();
    while (unkPaths--)
    {
        uint32_t curPathLen = headerReader->readUInt32();
        headerReader->seek(curPathLen, std::ios::cur);
    }

    headerReader->readUInt32(0, 0, "Always 0");
    outHeader.vertexCount = headerReader->readUInt32();

    uint32_t indexGroupCount = headerReader->readUInt32();
    outHeader.indices1Count = 0;
    while (indexGroupCount--)
    {
        outHeader.indices1Count += headerReader->readUInt32();
        outHeader.indices1Count += headerReader->readUInt32();
    }

    uint32_t modelPathLen = headerReader->readUInt32();
    outHeader.modelPaths.resize(modelPathLen);
    for (uint i = 0; i < modelPathLen; i++)
    {
        uint32_t curPathLen = headerReader->readUInt32();
        outHeader.modelPaths[i] = headerReader->readAsciiString(curPathLen);
    }

    uint32_t unkPaths2 = headerReader->readUInt32(0, 1000, "Unknown paths");
    while (unkPaths2--)
    {
        uint32_t curPathLen = headerReader->readUInt32();
        headerReader->seek(curPathLen, std::ios::cur);
    }

    uint32_t textureNames1 = headerReader->readUInt32();
    while (textureNames1--)
    {
        uint32_t curPathLen = headerReader->readUInt32();
        headerReader->seek(curPathLen, std::ios::cur);
    }

    uint32_t textureNames2 = headerReader->readUInt32();
    while (textureNames2--)
    {
        uint32_t curPathLen = headerReader->readUInt32();
        headerReader->seek(curPathLen, std::ios::cur);
    }

    uint32_t naturalLen = headerReader->readUInt32(0, 1000, "Natural Paths");
    outHeader.naturalPaths.resize(naturalLen);
    for (uint i = 0; i < naturalLen; i++)
    {
        uint32_t curPathLen = headerReader->readUInt32();
        outHeader.naturalPaths[i] = headerReader->readAsciiString(curPathLen);
    }

    uint32_t attributeLen = headerReader->readUInt32(0, 1000, "Attribute Paths");
    outHeader.attributes.resize(attributeLen);
    for (uint i = 0; i < attributeLen; i++)
    {
        uint32_t curPathLen = headerReader->readUInt32();
        outHeader.attributes[i] = headerReader->readAsciiString(curPathLen);
    }

    uint32_t materialLen = headerReader->readUInt32(0, 1000, "Material Paths");
    outHeader.materialPaths.resize(materialLen);
    for (uint i = 0; i < materialLen; i++)
    {
        uint32_t curPathLen = headerReader->readUInt32();
        outHeader.materialPaths[i] = headerReader->readAsciiString(curPathLen);
    }

    outHeader.indices2Count = headerReader->readUInt32();
    outHeader.unk1Count = headerReader->readUInt32();
    outHeader.unk2Count = headerReader->readUInt32();
    headerReader->readUInt32(0, 0, "Unk Empty Space");
    outHeader.matrixCount = headerReader->readUInt32();
    headerReader->readUInt32(outHeader.vertexCount, outHeader.vertexCount, "Vertex 2 count");
    headerReader->readUInt32(0, 0, "Unk Final Empty Space");

    if (headerReader->tell() != headerReader->getLength())
        throw unknown_format_error("Did not reach end of header");
}

void
LevelStaticReader28::readBody(BinaryReader::BinaryReaderBuffered* bodyReader, const LevelStaticHeaderExternal& extHeader, LevelStaticBodyExternal& outBody)
{
    outBody.objects.resize(extHeader.vertexCount);

    for (uint i = 0; i < extHeader.vertexCount; i++)
    {
        bodyReader->readSingleArray(&outBody.objects[i].pos[0], 3);
        bodyReader->seek(4, std::ios::cur);
        
        //bodyReader->readSingleArray(&outBody.objects[i].rot[0], 4);
        outBody.objects[i].rot.x = bodyReader->readFloat();
        outBody.objects[i].rot.y = bodyReader->readFloat();
        outBody.objects[i].rot.z = bodyReader->readFloat();
        outBody.objects[i].rot.w = bodyReader->readFloat();
        
        outBody.objects[i].scale = bodyReader->readFloat();
        bodyReader->seek(12, std::ios::cur);
    }

    bodyReader->seek(extHeader.indices1Count * 2, std::ios::cur);
    bodyReader->seek(extHeader.indices2Count * 2, std::ios::cur);
    bodyReader->seek(extHeader.unk1Count * 20, std::ios::cur);
    bodyReader->seek(extHeader.unk2Count * 48, std::ios::cur);
    bodyReader->seek(extHeader.matrixCount * 36, std::ios::cur);

    for (uint i = 0; i < extHeader.vertexCount; i++)
    {
        bodyReader->seek(120, std::ios::cur);
        outBody.objects[i].modelIndex = bodyReader->readUInt16(0, extHeader.modelPaths.size(), "Model index");
        bodyReader->seek(6, std::ios::cur);
        outBody.objects[i].naturalIndex = bodyReader->readUInt16();
        outBody.objects[i].materialIndex = bodyReader->readUInt16();
        outBody.objects[i].matrixIndex = bodyReader->readUInt16();
        bodyReader->seek(42, std::ios::cur);
    }

    if (bodyReader->tell() != bodyReader->getLength())
        throw unknown_format_error("Did not reach end of body");
}