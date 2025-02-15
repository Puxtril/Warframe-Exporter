#include "model/types/ModelReader99.h"

using namespace WarframeExporter::Model;

void
ModelReader99::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    headerReader->seek(0x30, std::ios_base::cur);

    skipPhysicsStruct(headerReader);

    headerReader->seek(0x4E, std::ios_base::cur);

    outHeader.vertexCount = headerReader->readUInt32();
    outHeader.faceCount = headerReader->readUInt32();
    outHeader.morphCount = headerReader->readUInt32();
    outHeader.boneCount = headerReader->readUInt32(0, 0, "Bones on static mesh");

    headerReader->seek(0x18, std::ios_base::cur);

    skipUnk64Array(headerReader);

    headerReader->seek(0x51, std::ios_base::cur);

    readMeshInfos(headerReader, outHeader.meshInfos);

    skipUnk16Array(headerReader);

    readMaterialPaths(headerReader, outHeader.materialPaths);

    skipPhysicsStruct(headerReader);

    readPhysxMeshes(headerReader, outHeader.physXMeshes);

    readErrors(headerReader, outHeader.errorMsgs);

    if (headerReader->tell() != headerReader->getLength())
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
        outBody.positions[x][3] = bodyReader->readInt16() / 32767.0F;

        bodyReader->seek(8, std::ios_base::cur); // No idea

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