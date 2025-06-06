#include "model/types/ModelReader102.h"

using namespace WarframeExporter::Model;

void
ModelReader102::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    headerReader->seek(0x30, std::ios_base::cur);

    skipPhysicsStruct(headerReader);

    headerReader->seek(0x4E, std::ios_base::cur);

    outHeader.vertexCount = headerReader->readUInt32();
    outHeader.faceCount = headerReader->readUInt32();
    outHeader.morphCount = headerReader->readUInt32(0, 0, "Non-zero Morphs");
    outHeader.boneCount = headerReader->readUInt32(0, 0, "Bones on static mesh");

    headerReader->seek(0x18, std::ios_base::cur);

    skipUnk64Array(headerReader);

    headerReader->seek(0x51, std::ios_base::cur);

    readMeshInfos(headerReader, outHeader.meshInfos);

    skipUnk16Array(headerReader);

    if (this->isDCM(header))
    {
        headerReader->seek(0x1, std::ios::cur);

        int dcmSubEnum = headerReader->readUInt32();
        if (dcmSubEnum == 2)
            headerReader->seek(6, std::ios::cur);
    }
    else
    {
        headerReader->seek(4, std::ios::cur);
    }

    skipPhysicsStruct(headerReader);

    readPhysxMeshes(headerReader, outHeader.physXMeshes);
}

void
ModelReader102::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody)
{
    for (const auto& x : extHeader.physXMeshes)
        bodyReader->seek(x.dataLength, std::ios_base::cur);

    bodyReader->seek(64U * extHeader.boneCount, std::ios_base::cur);

    bodyReader->seek(0x2, std::ios_base::cur);

    outBody.positions.resize(extHeader.vertexCount);
    outBody.normals.resize(extHeader.vertexCount);
    outBody.tangents.resize(extHeader.vertexCount);
    outBody.UV1.resize(extHeader.vertexCount);
    outBody.UV2.resize(extHeader.vertexCount);
    outBody.AO.resize(extHeader.vertexCount);
    for (uint32_t x = 0; x < extHeader.vertexCount; x++)
    {
        outBody.positions[x][0] = bodyReader->readInt16() / 32767.0F;
        outBody.positions[x][1] = bodyReader->readInt16() / 32767.0F;
        outBody.positions[x][2] = bodyReader->readInt16() / 32767.0F;
        outBody.positions[x][3] = bodyReader->readInt16() / 32767.0F;

        outBody.normals[x][0] = bodyReader->readUInt8();
        outBody.normals[x][1] = bodyReader->readUInt8();
        outBody.normals[x][2] = bodyReader->readUInt8();
        outBody.normals[x][3] = bodyReader->readUInt8();
        
        outBody.tangents[x][0] = bodyReader->readUInt8();
        outBody.tangents[x][1] = bodyReader->readUInt8();
        outBody.tangents[x][2] = bodyReader->readUInt8();
        outBody.AO[x] = bodyReader->readUInt8();

        outBody.UV1[x][0] = bodyReader->readHalf();
        outBody.UV1[x][1] = bodyReader->readHalf();

        outBody.UV2[x][0] = bodyReader->readHalf();
        outBody.UV2[x][1] = bodyReader->readHalf();
    }

    size_t remainingBytes = bodyReader->getLength() - bodyReader->tell();
    size_t indicesSize = static_cast<size_t>(extHeader.faceCount) * 2;
    
    // DCM Model
    if (remainingBytes > indicesSize)
    {
        bodyReader->seek(extHeader.vertexCount * 8, std::ios_base::cur);
    }

    outBody.indices.resize(extHeader.faceCount);
    bodyReader->readUInt16Array(outBody.indices.data(), extHeader.faceCount);

    if (extHeader.faceCount > 0 && outBody.indices[0] != 0)
        throw unknown_format_error("First index not 0, probably read model incorrectly");
}

bool
ModelReader102::isDCM(const LotusLib::CommonHeader& header)
{
    if (header.paths.size() != 1)
        return false;

    const std::string& filePath = header.paths[0];

    constexpr std::string_view dcmExt1 = "_dcm.fbx";
    constexpr std::string_view dcmExt2 = "_dcm";

    std::string_view ext1 = std::string_view(filePath.data() + (filePath.length() - 8), 8);
    std::string_view ext2 = std::string_view(filePath.data() + (filePath.length() - 4), 4);

    if (dcmExt1 == ext1 || dcmExt2 == ext2)
        return true;
    return false;
}
