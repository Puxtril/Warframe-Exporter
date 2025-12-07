#include "model/types/ModelReader102SF.h"

using namespace WarframeExporter::Model;

void
ModelReader102SF::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    headerReader->seek(0x30, std::ios_base::cur);

    skipPhysicsStruct2(headerReader);

    headerReader->seek(0x2E, std::ios_base::cur);

    headerReader->readSingleArray(&outHeader.ensmallening1[0], 4);
    headerReader->readSingleArray(&outHeader.ensmallening2[0], 4);

    outHeader.vertexCount = headerReader->readUInt32(1, 100000, "Vertex count");
    outHeader.faceCount = headerReader->readUInt32();
    outHeader.morphCount = headerReader->readUInt32();
    // Sometimes not 0
    // Related to MeshInfo's Unknown name (skipped data)
    outHeader.boneCount = headerReader->readUInt32();

    headerReader->seek(24, std::ios_base::cur);

    skipUnk64Array(headerReader);

    headerReader->seek(12, std::ios_base::cur);

    outHeader.vertexCountB = headerReader->readUInt32(0, outHeader.vertexCount, "Vertex Count B Cache");
    outHeader.faceCountB = headerReader->readUInt32(0, outHeader.faceCount, "Face Count B Cache");
    
    uint32_t devPathLen = headerReader->readUInt32();
    headerReader->seek(devPathLen, std::ios::cur);
    
    headerReader->seek(0x41, std::ios_base::cur);

    readMeshInfos(headerReader, outHeader.meshInfos);

    skipUnk16Array(headerReader);

    skipPhysicsStruct2(headerReader);

    readPhysxMeshes(headerReader, outHeader.physXMeshes);

    readErrors(headerReader, outHeader.errorMsgs);

    if (headerReader->tell() != headerReader->getLength())
        throw unknown_format_error("Did not reach end of H file");
}

void
ModelReader102SF::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReaderB, BinaryReader::BinaryReaderBuffered* bodyReaderF, ModelBodyExternal& outBody)
{
    // Sometimes vertexCountB is 0 and F cache is null.
    // In that case, everything is in B.
    uint32_t vertexBCache = extHeader.vertexCountB;
    uint32_t faceBCache = extHeader.faceCountB;
    if (extHeader.vertexCountB == 0)
    {
        vertexBCache = extHeader.vertexCount;
        faceBCache = extHeader.faceCount;
    }

    outBody.positions.resize(extHeader.vertexCount);
    outBody.normals.resize(extHeader.vertexCount);
    outBody.tangents.resize(extHeader.vertexCount);
    outBody.UV1.resize(extHeader.vertexCount);
    outBody.UV2.resize(extHeader.vertexCount);
    outBody.AO.resize(extHeader.vertexCount);

    // B Cache
    for (const auto& x : extHeader.physXMeshes)
        bodyReaderB->seek(x.dataLength, std::ios_base::cur);

    bodyReaderB->seek(0x2, std::ios_base::cur);

    for (uint32_t x = 0; x < vertexBCache; x++)
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

    outBody.indices.resize(extHeader.faceCount);
    bodyReaderB->readUInt16Array(outBody.indices.data(), faceBCache);

    // Everything's in B
    if (bodyReaderF->getLength() == 0)
        return;

    // F Cache
    for (uint32_t x = vertexBCache; x < extHeader.vertexCount; x++)
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

    size_t indexFfcount = extHeader.faceCount - faceBCache;
    bodyReaderF->readUInt16Array(outBody.indices.data() + faceBCache, indexFfcount);
}