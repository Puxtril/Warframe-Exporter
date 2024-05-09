#include "model/types/ModelReader99.h"

using namespace WarframeExporter::Model;

void
ModelReader99::readHeaderDebug(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header)
{
    headerReader->seek(0x1C, std::ios_base::cur);
    headerReader->readUInt32Array(5); // Possible LOD Data
    headerReader->readUInt32(); // Not an array
    headerReader->readUInt32(); // Not an array

    skipPhysicsPath1(headerReader);

    headerReader->seek(0x18, std::ios_base::cur);

    static const std::string randomFloat = "Random float test";
    headerReader->readFloat(1.0F, 101.0F, CONV_ZERO | FAIL_SUBNORM, randomFloat);

    headerReader->seek(0x12, std::ios_base::cur);

    // Vector4
    static const std::string vec4Msg = "Vector4 pair";
    headerReader->seek(0xC, std::ios_base::cur);
    headerReader->readFloat(0.0F, 0.00001F, CONV_ZERO | FAIL_SUBNORM, vec4Msg);
    // Vector4
    headerReader->seek(0xC, std::ios_base::cur);
    headerReader->readFloat(0.0F, 0.00001F, CONV_ZERO | FAIL_SUBNORM, vec4Msg);

    uint32_t vertexCount = headerReader->readUInt32();

    static const std::string faceCountMsg = "Face count";
    headerReader->readUInt32(vertexCount, UINT32_MAX, faceCountMsg);

    headerReader->readUInt32(); // morph count
    headerReader->readUInt32(); // bone count

    headerReader->seek(0x18, std::ios_base::cur);

    uint32_t skipLongCount = headerReader->readUInt32();
    headerReader->seek(8 * skipLongCount, std::ios::cur);

    headerReader->seek(0x51, std::ios_base::cur);

    const static std::string meshInfoMsg = "Mesh Info Count";
    uint32_t minMeshInfo = vertexCount > 0 ? 1 : 0;
    uint32_t meshInfoCount = headerReader->readUInt32(minMeshInfo, 30, meshInfoMsg);

    for (uint32_t x = 0; x < meshInfoCount; x++)
    {
        static const std::string meshInfoVec4Msg = "MeshInfo Vector4 pair";
        headerReader->seek(0xC, std::ios_base::cur);
        headerReader->readFloat(0.0F, 0.00001F, CONV_ZERO | FAIL_SUBNORM, meshInfoVec4Msg);
        headerReader->seek(0xC, std::ios_base::cur);
        headerReader->readFloat(0.0F, 0.00001F, CONV_ZERO | FAIL_SUBNORM, meshInfoVec4Msg);

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

        static const std::string LODFaceCountMsg = "MeshInfo LOD Face";
        uint32_t curFaceLODCount = headerReader->readUInt32();
        curFaceLODCount = headerReader->readUInt32(0, curFaceLODCount + 1, LODFaceCountMsg);
        curFaceLODCount = headerReader->readUInt32(0, curFaceLODCount + 1, LODFaceCountMsg);
        curFaceLODCount = headerReader->readUInt32(0, curFaceLODCount + 1, LODFaceCountMsg);
        curFaceLODCount = headerReader->readUInt32(0, curFaceLODCount + 1, LODFaceCountMsg);

        headerReader->seek(0x34, std::ios_base::cur);
    }

    uint32_t shortCount = headerReader->readUInt32();
    headerReader->seek(shortCount * 2U, std::ios_base::cur);

    uint32_t materialPathArrayLen = headerReader->readUInt32();
    for (uint32_t x = 0; x < materialPathArrayLen; x++)
    {
        uint32_t curMaterialPathLen = headerReader->readUInt32();
        headerReader->seek(curMaterialPathLen, std::ios::cur);
    }

    headerReader->seek(0x8, std::ios::cur);

    skipPhysicsPath1(headerReader);

    uint32_t physXMeshCount = headerReader->readUInt32();
    for (uint32_t x = 0; x < physXMeshCount; x++)
    {
        uint32_t meshTypeEnum = headerReader->readUInt32();
        if (meshTypeEnum == 1)
            headerReader->seek(0x4C, std::ios_base::cur);
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
ModelReader99::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    headerReader->seek(0x38, std::ios_base::cur);

    skipPhysicsPath1(headerReader);

    headerReader->seek(0x4E, std::ios_base::cur);

    outHeader.vertexCount = headerReader->readUInt32();
    outHeader.faceCount = headerReader->readUInt32();
    outHeader.morphCount = headerReader->readUInt32();
    outHeader.boneCount = headerReader->readUInt32();

    headerReader->seek(0x18, std::ios_base::cur);

    uint32_t skipLongCount = headerReader->readUInt32();
    headerReader->seek(8 * skipLongCount, std::ios::cur);

    headerReader->seek(0x51, std::ios_base::cur);

    readMeshInfos(headerReader, outHeader.meshInfos);

    uint32_t shortCount = headerReader->readUInt32();
    headerReader->seek(shortCount * 2U, std::ios_base::cur);

    readMaterialPaths(headerReader, outHeader.materialPaths);

    headerReader->seek(0x8, std::ios::cur);

    skipPhysicsPath1(headerReader);

    readPhysxMeshes(headerReader, outHeader.physXMeshes);

    readErrors(headerReader, outHeader.errorMsgs);

    if (headerReader->tell() != headerReader->getLength())
        throw unknown_format_error("Did not reach end of file");
}

void
ModelReader99::readBodyDebug(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader)
{
    for (const auto& x : extHeader.physXMeshes)
        bodyReader->seek(x.dataLength, std::ios_base::cur);

    bodyReader->seek(64 * extHeader.boneCount, std::ios_base::cur);

    // Such a hack, but it works very well (>99% success for models)
    // Most of the PhysX mesh data is empty space
    while (isMorePhysX(bodyReader))
        bodyReader->seek(0x120, std::ios::cur);

    bodyReader->seek(0x2, std::ios_base::cur);

    for (uint32_t x = 0; x < extHeader.vertexCount; x++)
    {
        bodyReader->seek(0x18, std::ios_base::cur);
    }

    static const std::string facesIndiciesMsg = "Face Indices";
    bodyReader->readUInt16Array(extHeader.faceCount, 0, extHeader.vertexCount + 1, facesIndiciesMsg);

    if (bodyReader->tell() != bodyReader->getLength())
        throw unknown_format_error("Did not reach end of file");
}

void
ModelReader99::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody)
{
    for (const auto& x : extHeader.physXMeshes)
        bodyReader->seek(x.dataLength, std::ios_base::cur);

    bodyReader->seek(64U * extHeader.boneCount, std::ios_base::cur);

    while (isMorePhysX(bodyReader))
        bodyReader->seek(0x120, std::ios::cur);

    bodyReader->seek(0x2, std::ios_base::cur);

    outBody.positions.resize(extHeader.vertexCount);
    outBody.UV1.resize(extHeader.vertexCount);
    outBody.UV2.resize(extHeader.vertexCount);
    outBody.boneIndices.resize(extHeader.vertexCount);
    outBody.boneWeights.resize(extHeader.vertexCount);
    for (uint32_t x = 0; x < extHeader.vertexCount; x++)
    {
        outBody.positions[x][0] = bodyReader->readInt16() / 32767.0F;
        outBody.positions[x][1] = bodyReader->readInt16() / 32767.0F;
        outBody.positions[x][2] = bodyReader->readInt16() / 32767.0F;

        bodyReader->seek(10, std::ios_base::cur); // No idea

        outBody.UV1[x][0] = bodyReader->readHalf();
        outBody.UV1[x][1] = bodyReader->readHalf();

        outBody.UV2[x][0] = bodyReader->readHalf();
        outBody.UV2[x][1] = bodyReader->readHalf();
    }

    outBody.indices.resize(extHeader.faceCount);
    bodyReader->readUInt16Array(outBody.indices.data(), extHeader.faceCount);
}

// This is 100% a workaround
bool
ModelReader99::isMorePhysX(BinaryReader::BinaryReaderBuffered* bodyReader)
{
    // Some models have no actual vertices, so this can hit EOF
    if (bodyReader->tell() + 0x120 > bodyReader->getLength())
        return false;

    // Some structs have data pretty far into the allocated 0x120 block size
    bodyReader->seek(0x100, std::ios::cur);
    uint64_t check1 = bodyReader->readUInt64();
    uint64_t check2 = bodyReader->readUInt64();
    bodyReader->seek(-0x110, std::ios::cur);

    return check1 == 0 && check2 == 0;
}