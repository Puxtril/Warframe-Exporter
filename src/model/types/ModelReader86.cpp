#include "model/types/ModelReader86.h"

using namespace WarframeExporter::Model;

void
ModelReader86::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    headerReader->seek(0x2E, std::ios_base::cur);

    skipPhysicsPath1(headerReader);

    headerReader->seek(0x2C, std::ios_base::cur);

    uint16_t vecCheck = headerReader->readUInt16();
    if (vecCheck != 256 && vecCheck != 257 && vecCheck != 258 && vecCheck != 0)
        headerReader->seek(-2, std::ios_base::cur);

    headerReader->seek(0x20, std::ios_base::cur);

    // Main model data
    outHeader.vertexCount = headerReader->readUInt32();
    outHeader.faceCount = headerReader->readUInt32();
    outHeader.morphCount = headerReader->readUInt32();
    outHeader.boneCount = headerReader->readUInt32();

    headerReader->seek(0x18, std::ios_base::cur);

    uint32_t UInt64LODUnkLen = headerReader->readUInt32();
    headerReader->seek(UInt64LODUnkLen * 8U, std::ios_base::cur);

    headerReader->seek(0xC, std::ios_base::cur);
    uint32_t somePathLen = headerReader->readUInt32();
    headerReader->seek(somePathLen, std::ios_base::cur);
    
    headerReader->seek(0x46, std::ios_base::cur);

    readMeshInfos(headerReader, outHeader.meshInfos);

    uint32_t shortCount = headerReader->readUInt32();
    headerReader->seek(shortCount * 2U, std::ios_base::cur);

    headerReader->seek(0x8, std::ios_base::cur);

    skipPhysicsPath1(headerReader);

    readPhysxMeshes(headerReader, outHeader.physXMeshes);

    readErrors(headerReader, outHeader.errorMsgs);

    if (headerReader->tell() != headerReader->getLength())
        throw unknown_format_error("Did not reach end of file");
}

void
ModelReader86::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody)
{
    for (const auto& x : extHeader.physXMeshes)
        bodyReader->seek(x.dataLength, std::ios_base::cur);

    bodyReader->seek(64U * extHeader.boneCount, std::ios_base::cur);

    bodyReader->seek(0x2, std::ios_base::cur);

    outBody.positions.resize(extHeader.vertexCount);
    outBody.UV1.resize(extHeader.vertexCount);
    outBody.UV2.resize(extHeader.vertexCount);
    outBody.colors.resize(1);
    outBody.colors[0].resize(extHeader.vertexCount);
    for (uint32_t x = 0; x < extHeader.vertexCount; x++)
    {
        outBody.positions[x][0] = bodyReader->readHalf();
        outBody.positions[x][1] = bodyReader->readHalf();
        outBody.positions[x][2] = bodyReader->readHalf();

        bodyReader->seek(6, std::ios_base::cur); // Normals?
        
        bodyReader->seek(3, std::ios::cur); // Tangent data?

        // Ambient Occlusion
        uint8_t col = bodyReader->readUInt8();
        outBody.colors[0][x][0] = col;
        outBody.colors[0][x][1] = col;
        outBody.colors[0][x][2] = col;

        outBody.UV1[x][0] = bodyReader->readHalf();
        outBody.UV1[x][1] = bodyReader->readHalf();

        outBody.UV2[x][0] = bodyReader->readHalf();
        outBody.UV2[x][1] = bodyReader->readHalf();
    }

    outBody.indices.resize(extHeader.faceCount);
    bodyReader->readUInt16Array(outBody.indices.data(), extHeader.faceCount);
}