#include "model/types/ModelReader103.h"

using namespace WarframeExporter::Model;

void
ModelReader103::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    headerReader->seek(0x38, std::ios_base::cur);

    skipPhysicsPath1(headerReader);

    headerReader->seek(0x4E, std::ios_base::cur);

    outHeader.vertexCount = headerReader->readUInt32();
    outHeader.faceCount = headerReader->readUInt32();
    outHeader.morphCount = headerReader->readUInt32();
    outHeader.boneCount = headerReader->readUInt32();

    headerReader->seek(0x75, std::ios_base::cur);

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
ModelReader103::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody)
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
        outBody.positions[x][0] = bodyReader->readFloat();
        outBody.positions[x][1] = bodyReader->readFloat();
        outBody.positions[x][2] = bodyReader->readFloat();

        // /shrug
        bodyReader->seek(12, std::ios_base::cur);

        outBody.UV1[x][0] = bodyReader->readHalf();
        outBody.UV1[x][1] = bodyReader->readHalf();

        outBody.UV2[x][0] = bodyReader->readHalf();
        outBody.UV2[x][1] = bodyReader->readHalf();
    }

    outBody.indices.resize(extHeader.faceCount);
    bodyReader->readUInt16Array(outBody.indices.data(), extHeader.faceCount);

    if (extHeader.faceCount > 0 && outBody.indices[0] != 0)
        throw unknown_format_error("First index not 0, probably read model incorrectly");
}

bool
ModelReader103::isMorePhysX(BinaryReader::BinaryReaderBuffered* bodyReader)
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