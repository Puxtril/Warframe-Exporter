#include "model/types/ModelDCMReader108SF.h"

using namespace WarframeExporter::Model;

void
ModelDCMReader108SF::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    headerReader->seek(0x30, std::ios_base::cur);

    skipPhysicsStruct(headerReader);

    headerReader->seek(0x2E, std::ios_base::cur);

    headerReader->readSingleArray(&outHeader.ensmallening1[0], 4);
    headerReader->readSingleArray(&outHeader.ensmallening2[0], 4);

    outHeader.vertexCount = headerReader->readUInt32();
    outHeader.faceCount = headerReader->readUInt32();
    outHeader.morphCount = headerReader->readUInt32(0, 0, "Non-zero Morphs");
    outHeader.boneCount = headerReader->readUInt32(0, 0, "Bones on static mesh");

    headerReader->seek(0x18, std::ios_base::cur);

    skipUnk64Array(headerReader);

    headerReader->seek(0xC, std::ios_base::cur);

    outHeader.vertexCountB = headerReader->readUInt32(1, outHeader.vertexCount, "B Cache Vertex count");
    outHeader.faceCountB = headerReader->readUInt32(1, outHeader.faceCount, "B Cache Face count");
    headerReader->seek(0x45, std::ios_base::cur);

    readMeshInfos(headerReader, outHeader.meshInfos);

    skipUnk16Array(headerReader);

    headerReader->seek(0x1, std::ios::cur);
    int dcmSubEnum = headerReader->readUInt32();
    if (dcmSubEnum == 2)
        headerReader->seek(6, std::ios::cur);

    skipPhysicsStruct(headerReader);

    readPhysxMeshes(headerReader, outHeader.physXMeshes);

    readErrors(headerReader, outHeader.errorMsgs);

    if (headerReader->tell() != headerReader->getLength())
        throw unknown_format_error("Did not reach end of file");
}

void
ModelDCMReader108SF::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReaderB, BinaryReader::BinaryReaderBuffered* bodyReaderF, ModelBodyExternal& outBody)
{
    for (const auto& x : extHeader.physXMeshes)
        bodyReaderB->seek(x.dataLength, std::ios_base::cur);

    bodyReaderB->seek(64U * extHeader.boneCount, std::ios_base::cur);

    bodyReaderB->seek(0x2, std::ios_base::cur);

    outBody.positions.resize(extHeader.vertexCount);
    outBody.normals.resize(extHeader.vertexCount);
    outBody.tangents.resize(extHeader.vertexCount);
    outBody.UV1.resize(extHeader.vertexCount);
    outBody.UV2.resize(extHeader.vertexCount);
    outBody.AO.resize(extHeader.vertexCount);
    for (uint32_t x = 0; x < extHeader.vertexCountB; x++)
    {
        outBody.positions[x][0] = bodyReaderB->readInt16() / 32767.0F;
        outBody.positions[x][1] = bodyReaderB->readInt16() / 32767.0F;
        outBody.positions[x][2] = bodyReaderB->readInt16() / 32767.0F;
        outBody.positions[x][3] = bodyReaderB->readInt16() / 32767.0F;

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

    size_t remainingBytes = bodyReaderB->getLength() - bodyReaderB->tell();
    size_t indicesSize = static_cast<size_t>(extHeader.faceCountB) * 2;
    if (remainingBytes > indicesSize)
    {
        bodyReaderB->seek(extHeader.vertexCountB * 8, std::ios_base::cur);
    }

    outBody.indices.resize(extHeader.faceCount);
    bodyReaderB->readUInt16Array(outBody.indices.data(), extHeader.faceCountB);

    if (extHeader.faceCount > 0 && outBody.indices[0] != 0)
        throw unknown_format_error("First index not 0, probably read model incorrectly");

    for (uint32_t x = extHeader.vertexCountB; x < extHeader.vertexCount; x++)
    {
        outBody.positions[x][0] = bodyReaderF->readInt16() / 32767.0F;
        outBody.positions[x][1] = bodyReaderF->readInt16() / 32767.0F;
        outBody.positions[x][2] = bodyReaderF->readInt16() / 32767.0F;
        outBody.positions[x][3] = bodyReaderF->readInt16() / 32767.0F;

        outBody.normals[x][0] = bodyReaderF->readUInt8();
        outBody.normals[x][1] = bodyReaderF->readUInt8();
        outBody.normals[x][2] = bodyReaderF->readUInt8();
        outBody.normals[x][3] = bodyReaderF->readUInt8();
        
        outBody.tangents[x][0] = bodyReaderF->readUInt8();
        outBody.tangents[x][1] = bodyReaderF->readUInt8();
        outBody.tangents[x][2] = bodyReaderF->readUInt8();
        outBody.AO[x] = bodyReaderF->readUInt8();

        outBody.UV1[x][0] = bodyReaderF->readHalf();
        outBody.UV1[x][1] = bodyReaderF->readHalf();

        outBody.UV2[x][0] = bodyReaderF->readHalf();
        outBody.UV2[x][1] = bodyReaderF->readHalf();
    }

    size_t indexFfcount = extHeader.faceCount - extHeader.faceCountB;
    size_t vertexFcount = extHeader.vertexCount - extHeader.vertexCountB;

    remainingBytes = bodyReaderF->getLength() - bodyReaderF->tell();
    indicesSize = static_cast<size_t>(indexFfcount) * 2;
    if (remainingBytes > indicesSize)
    {
        bodyReaderF->seek(vertexFcount * 8, std::ios_base::cur);
    }

    bodyReaderF->readUInt16Array(outBody.indices.data() + extHeader.faceCountB, indexFfcount);
}
