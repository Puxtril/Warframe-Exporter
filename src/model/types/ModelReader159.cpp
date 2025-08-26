#include "model/types/ModelReader159.h"

using namespace WarframeExporter::Model;

void
ModelReader159::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    uint32_t strLen = headerReader->readUInt32(0, 200, "First strLen");
    headerReader->seek(strLen, std::ios::cur);
    
    uint32_t strLen2 = headerReader->readUInt32(0, 200, "Second strLen");
    headerReader->seek(strLen2, std::ios::cur);

    headerReader->readSingleArray(&outHeader.ensmallening1[0], 4);
    headerReader->readSingleArray(&outHeader.ensmallening2[0], 4);

    outHeader.faceCount = (uint32_t)headerReader->readUInt64();

    readMeshInfos(headerReader, outHeader.meshInfos);
    
    // Terrains should only have 1 meshinfo
    // It causes issues when there are more than 1
    // Surely this won't bite me on the ass one day
    outHeader.meshInfos.resize(1);

    outHeader.vertexCount = headerReader->readUInt32();

    headerReader->seek(0x10, std::ios::cur);

    outHeader.bodySkipLen1 = headerReader->readUInt32(0, 1000, "BodySkipLen1");

    headerReader->seek(0x20, std::ios::cur);

    uint32_t unkStructCount = headerReader->readUInt32(0, 100, "UnkStructCount");
    for (uint32_t x = 0; x < unkStructCount; x++)
    {
        uint32_t arrCount = headerReader->readUInt32();
        headerReader->seek(4 * arrCount, std::ios::cur);
        headerReader->seek(4 * 4, std::ios::cur);
    }

    headerReader->seek(12, std::ios::cur);

    readPhysxMeshes(headerReader, outHeader.physXMeshes);
}

void
ModelReader159::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody)
{
    for (const auto& x : extHeader.physXMeshes)
        bodyReader->seek(x.dataLength, std::ios_base::cur);

    bodyReader->seek(0x40 * extHeader.bodySkipLen1, std::ios::cur);

    outBody.positions.resize(extHeader.vertexCount);
    outBody.UV1.resize(extHeader.vertexCount);
    outBody.UV2.resize(extHeader.vertexCount);
    outBody.colors.resize(1);
    outBody.colors[0].resize(extHeader.vertexCount);
    for (uint32_t x = 0; x < extHeader.vertexCount; x++)
    {
        outBody.positions[x][0] = bodyReader->readHalf() - 0.5F;
        outBody.positions[x][1] = bodyReader->readHalf();
        outBody.positions[x][2] = bodyReader->readHalf() - 0.5F;
        
        bodyReader->seek(6, std::ios::cur);
        outBody.colors[0][x][0] = bodyReader->readUInt8();
        outBody.colors[0][x][1] = bodyReader->readUInt8();
        outBody.colors[0][x][2] = bodyReader->readUInt8();
        outBody.colors[0][x][3] = bodyReader->readUInt8();
    }

    if (!canContinueReading(bodyReader, extHeader.faceCount))
        throw unknown_format_error("Incorrect index count");

    outBody.indices.resize(extHeader.faceCount);
    bodyReader->readUInt16Array(outBody.indices.data(), extHeader.faceCount);
}
