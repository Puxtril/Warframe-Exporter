#include "model/types/ModelReader112.h"

using namespace WarframeExporter::Model;

void
ModelReader112::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    headerReader->seek(0x30, std::ios_base::cur);

    skipPhysicsStruct(headerReader);

    headerReader->seek(0x28, std::ios_base::cur);
    headerReader->readSingleArray(&outHeader.ensmallening1[0], 4);
    headerReader->readSingleArray(&outHeader.ensmallening2[0], 4);

    outHeader.vertexCount = headerReader->readUInt32();
    outHeader.faceCount = headerReader->readUInt32();
    outHeader.morphCount = headerReader->readUInt32(0, 0, "Non-zero Morphs");
    outHeader.boneCount = headerReader->readUInt32(0, 0, "Bones on static mesh");

    headerReader->seek(0x67, std::ios_base::cur);

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
ModelReader112::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReaderB, BinaryReader::BinaryReaderBuffered* bodyReaderF, ModelBodyExternal& outBody)
{
    for (const auto& x : extHeader.physXMeshes)
        bodyReaderB->seek(x.dataLength, std::ios_base::cur);

    bodyReaderB->seek(64U * extHeader.boneCount, std::ios_base::cur);

    while (isMorePhysX(bodyReaderB))
        bodyReaderB->seek(0x120, std::ios::cur);

    bodyReaderB->seek(0x2, std::ios_base::cur);

    outBody.positions.resize(extHeader.vertexCount);
    outBody.normals.resize(extHeader.vertexCount);
    outBody.tangents.resize(extHeader.vertexCount);
    outBody.UV1.resize(extHeader.vertexCount);
    outBody.UV2.resize(extHeader.vertexCount);
    outBody.AO.resize(extHeader.vertexCount);
    for (uint32_t x = 0; x < extHeader.vertexCount; x++)
    {
        outBody.positions[x][0] = bodyReaderB->readFloat();
        outBody.positions[x][1] = bodyReaderB->readFloat();
        outBody.positions[x][2] = bodyReaderB->readFloat();
        bodyReaderB->seek(4, std::ios_base::cur);

        outBody.normals[x][0] = bodyReaderB->readUInt8();
        outBody.normals[x][1] = bodyReaderB->readUInt8();
        outBody.normals[x][2] = bodyReaderB->readUInt8();
        outBody.normals[x][3] = bodyReaderB->readUInt8();
        
        outBody.tangents[x][0] = bodyReaderB->readUInt8();
        outBody.tangents[x][1] = bodyReaderB->readUInt8();
        outBody.tangents[x][2] = bodyReaderB->readUInt8();
        outBody.AO[x] = bodyReaderB->readUInt8();

        outBody.UV1[x][0] = bodyReaderB->readHalf();
        outBody.UV1[x][1] = bodyReaderB->readHalf();

        outBody.UV2[x][0] = bodyReaderB->readHalf();
        outBody.UV2[x][1] = bodyReaderB->readHalf();
    }

    if (!canContinueReading(bodyReaderB, extHeader.faceCount))
        throw unknown_format_error("Incorrect index count");

    outBody.indices.resize(extHeader.faceCount);
    bodyReaderB->readUInt16Array(outBody.indices.data(), extHeader.faceCount);

    if (extHeader.faceCount > 0 && outBody.indices[0] != 0)
        throw unknown_format_error("First index not 0, probably read model incorrectly");
}

bool
ModelReader112::isMorePhysX(BinaryReader::BinaryReaderBuffered* bodyReader)
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