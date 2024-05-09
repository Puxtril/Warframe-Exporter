#include "model/types/ModelReader159.h"

using namespace WarframeExporter::Model;

void
ModelReader159::readHeaderDebug(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header)
{
    uint32_t strLen = headerReader->readUInt32();
    headerReader->seek(strLen, std::ios::cur);

    uint32_t strLen2 = headerReader->readUInt32();
    headerReader->seek(strLen2, std::ios::cur);

    headerReader->seek(8, std::ios::cur);

    // 6 floats
    headerReader->seek(4 * 6, std::ios::cur);

    static const std::string faceCountMaxMsg = "UInt64 face count";
    headerReader->readUInt64(0, UINT32_MAX, faceCountMaxMsg);

    const static std::string meshInfoMsg = "Mesh Info Count";
    uint32_t meshInfoCount = headerReader->readUInt32(1, 5, meshInfoMsg);

    for (uint32_t x = 0; x < meshInfoCount; x++)
    {
        headerReader->seek(0x20, std::ios_base::cur);

        uint32_t meshInfoNameLen = headerReader->readUInt32();
        headerReader->seek(meshInfoNameLen, std::ios_base::cur);

        // Apparently these can have less than 1 LOD
        // Took out some debug logic because these are all over the place
        static const std::string LODOffsetMsg = "MeshInfo LOD Offset";
        headerReader->readUInt32();
        headerReader->readUInt32(0, UINT32_MAX, LODOffsetMsg);
        headerReader->readUInt32(0, UINT32_MAX, LODOffsetMsg);
        headerReader->readUInt32(0, UINT32_MAX, LODOffsetMsg);
        headerReader->readUInt32(0, UINT32_MAX, LODOffsetMsg);

        // Current face LOD Count (5 UInt32)
        headerReader->seek(20, std::ios::cur);

        headerReader->seek(0x20, std::ios::cur);

        uint32_t strLen = headerReader->readUInt32();
        headerReader->seek(strLen, std::ios::cur);

        headerReader->seek(0x10, std::ios::cur);
    }
    
    headerReader->seek(0x38, std::ios::cur);

    uint32_t unkStructCount = headerReader->readUInt32();
    for (uint32_t i = 0; i < unkStructCount; i++)
    {
        uint32_t unkIntCount = headerReader->readUInt32();
        
        // Cannot make any assumptions about this data

        // Usually below UINT32_MAX (obviously, if >0)
        headerReader->seek(4 * unkIntCount, std::ios::cur);

        // If 0, these are usually all below UINT32_MAX
        // If >0, usually all UINT32_MAX
        headerReader->seek(16, std::ios::cur);
    }

    headerReader->seek(12, std::ios::cur);

    uint32_t physXMeshCount = headerReader->readUInt32();
    for (uint32_t x = 0; x < physXMeshCount; x++)
    {
        uint32_t meshTypeEnum = headerReader->readUInt32();
        if (meshTypeEnum == 1)
            headerReader->seek(0x4C, std::ios_base::cur);
        else if (meshTypeEnum == 7)
            headerReader->seek(0x60, std::ios_base::cur);
        else
            headerReader->seek(0x50, std::ios_base::cur);

        // Vector4
        headerReader->seek(0x10, std::ios_base::cur);
        // Vector4
        headerReader->seek(0x10, std::ios_base::cur);

        if (meshTypeEnum != 0 && meshTypeEnum != 2 && meshTypeEnum != 3)
            headerReader->seek(0x4, std::ios_base::cur);

        // Removed number check
        headerReader->readInt32();

        headerReader->readUInt32();
        headerReader->seek(0x8, std::ios_base::cur);
    }

    uint32_t errorCount = headerReader->readUInt32();
    for (uint32_t x = 0; x < errorCount; x++)
    {
        uint32_t errorCountStrLen = headerReader->readUInt32();
        headerReader->seek(errorCountStrLen, std::ios_base::cur);
    }

    if (headerReader->tell() != headerReader->getLength())
        throw unknown_format_error("Did not reach end of file");
}

void
ModelReader159::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    uint32_t strLen = headerReader->readUInt32();
    headerReader->seek(strLen, std::ios::cur);
    
    uint32_t strLen2 = headerReader->readUInt32();
    headerReader->seek(strLen2, std::ios::cur);

    headerReader->seek(0x20, std::ios::cur);

    outHeader.faceCount = (uint32_t)headerReader->readUInt64();

    readMeshInfos(headerReader, outHeader.meshInfos);
    
    // Terrains should only have 1 meshinfo
    // It causes issues when there are more than 1
    // Surely this won't bite me on the ass one day
    outHeader.meshInfos.resize(1);

    outHeader.vertexCount = headerReader->readUInt32();

    headerReader->seek(0x10, std::ios::cur);

    outHeader.bodySkipLen1 = headerReader->readUInt32();

    headerReader->seek(0x20, std::ios::cur);

    uint32_t unkStructCount = headerReader->readUInt32();
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

    outBody.indices.resize(extHeader.faceCount);
    bodyReader->readUInt16Array(outBody.indices.data(), extHeader.faceCount);
}

void
ModelReader159::readBodyDebug(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader)
{

}