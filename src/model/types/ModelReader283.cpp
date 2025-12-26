#include "model/types/ModelReader283.h"

using namespace WarframeExporter::Model;

void
ModelReader283::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    headerReader->seek(0x30, std::ios_base::cur);

    skipPhysicsStruct2(headerReader);

    headerReader->seek(0x2C, std::ios_base::cur);
    headerReader->readSingleArray(&outHeader.ensmallening1[0], 4);
    headerReader->readSingleArray(&outHeader.ensmallening2[0], 4);

    readWeightedBones(headerReader, outHeader.weightedBoneNames);

    // Main vertex counts
    outHeader.faceCount = headerReader->readUInt32();
    outHeader.boneCount = headerReader->readUInt32();
    outHeader.vertexCount = headerReader->readUInt32();
    outHeader.morphCount = headerReader->readUInt32();

    headerReader->seek(0x6, std::ios_base::cur);
    skipUnk64Array(headerReader);
    headerReader->seek(0xC, std::ios_base::cur);

    outHeader.vertexCountB = headerReader->readUInt32(0, outHeader.vertexCount, "B Cache Vertex count");
    outHeader.faceCountB = headerReader->readUInt32(0, outHeader.faceCount, "B Cache Face count");
    headerReader->seek(0x25, std::ios_base::cur);

    readBoneTree(headerReader, outHeader.boneTree);

    uint32_t unkStructCount = skipUnknownStructs(headerReader);

    headerReader->seek(0x1A, std::ios_base::cur);
    outHeader.bodySkipLen1 = headerReader->readUInt32(0, 5000, "BodySkipLen1");
    headerReader->seek(0x10 * unkStructCount, std::ios_base::cur);
    headerReader->seek(0x8, std::ios_base::cur);

    readMeshInfos(headerReader, outHeader.meshInfos);

    skipUnk16Array(headerReader);

    skipMorphs(headerReader);

    readBoneMaps(headerReader, outHeader.boneMaps);

    outHeader.bodySkipLen2 = skipMorphStructsAndFindSkip(headerReader, outHeader.meshInfos);

    headerReader->seek(0x2F, std::ios_base::cur);

    skipPhysicsStruct2(headerReader);

    readPhysxMeshes(headerReader, outHeader.physXMeshes);

    readErrors(headerReader, outHeader.errorMsgs);

    if (headerReader->tell() != headerReader->getLength())
        throw unknown_format_error("Did not reach end of H cache");
}

void
ModelReader283::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReaderB, BinaryReader::BinaryReaderBuffered* bodyReaderF, ModelBodyExternal& outBody)
{
    bodyReaderB->seek(0, std::ios_base::beg);

    for (const auto& x : extHeader.physXMeshes)
        bodyReaderB->seek(x.dataLength, std::ios_base::cur);

    bodyReaderB->seek(0x2, std::ios_base::cur);

    std::vector<glm::mat4x4>& reverseBinds = outBody.reverseBinds;
    reverseBinds.resize(extHeader.weightedBoneNames.size());
    for (size_t x = 0; x < extHeader.weightedBoneNames.size(); x++)
    {
        bodyReaderB->readSingleArray(glm::value_ptr(reverseBinds[x]), 16);
    }

    bodyReaderB->seek(extHeader.bodySkipLen1, std::ios_base::cur);

    std::vector<glm::quat>& outRotations = outBody.boneRotations;
    std::vector<glm::vec3>& outPositions = outBody.bonePositions;
    outRotations.resize(extHeader.boneTree.size());
    outPositions.resize(extHeader.boneTree.size());
    for (uint32_t x = 0; x < extHeader.boneTree.size(); x++)
    {
        bodyReaderB->readSingleArray(&outRotations[x].w, 4);
        bodyReaderB->readSingleArray(&outPositions[x].x, 3);
        bodyReaderB->seek(12, std::ios_base::cur);
    }

    bodyReaderB->seek(2 * extHeader.boneTree.size(), std::ios_base::cur);
    bodyReaderB->seek(2 * extHeader.boneTree.size(), std::ios_base::cur);

    if (extHeader.boneTree.size() > 0)
        bodyReaderB->seek(0x8, std::ios_base::cur);

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
    outBody.boneIndices.resize(extHeader.vertexCount);
    outBody.boneWeights.resize(extHeader.vertexCount);
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

        outBody.boneIndices[x][0] = bodyReaderB->readUInt8();
        outBody.boneIndices[x][1] = bodyReaderB->readUInt8();
        outBody.boneIndices[x][2] = bodyReaderB->readUInt8();
        outBody.boneIndices[x][3] = bodyReaderB->readUInt8();

        uint32_t weightUInt = bodyReaderB->readUInt32();
        float weightLast = 1.0F;
        for (int y = 0; y < 3; y++)
        {
            float curWeight = (float)(weightUInt & 0x3FF) / (float)0x3FF;
            outBody.boneWeights[x][y] = curWeight;
            weightUInt >>= 10;
            weightLast -= curWeight;
        }
        // Migitate floating point errors
        // This determines if the vertex is weighted,
        //  so it's important this gets set to exactly 0
        if (weightLast > 0.0001F)
            outBody.boneWeights[x][3] = std::max(weightLast, 0.0F);
    }

    bodyReaderB->seek(extHeader.bodySkipLen2 * 8U, std::ios_base::cur);

    outBody.indices.resize(extHeader.faceCount);
    bodyReaderB->readUInt16Array(outBody.indices.data(), faceBCache);

    if (bodyReaderB->tell() != bodyReaderB->getLength())
        throw unknown_format_error("Did not reach end of B cache");

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

        outBody.boneIndices[x][0] = bodyReaderF->readUInt8();
        outBody.boneIndices[x][1] = bodyReaderF->readUInt8();
        outBody.boneIndices[x][2] = bodyReaderF->readUInt8();
        outBody.boneIndices[x][3] = bodyReaderF->readUInt8();

        uint32_t weightUInt = bodyReaderF->readUInt32();
        float weightLast = 1.0F;
        for (int y = 0; y < 3; y++)
        {
            float curWeight = (float)(weightUInt & 0x3FF) / (float)0x3FF;
            outBody.boneWeights[x][y] = curWeight;
            weightUInt >>= 10;
            weightLast -= curWeight;
        }
        // Migitate floating point errors
        // This determines if the vertex is weighted,
        //  so it's important this gets set to exactly 0
        if (weightLast > 0.0001F)
            outBody.boneWeights[x][3] = std::max(weightLast, 0.0F);
    }

    size_t indexFfcount = extHeader.faceCount - faceBCache;
    bodyReaderF->readUInt16Array(outBody.indices.data() + faceBCache, indexFfcount);
}
