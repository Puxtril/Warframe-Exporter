#include "model/types/ModelReader269.h"

using namespace WarframeExporter::Model;

void
ModelReader269::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    headerReader->seek(0x30, std::ios_base::cur);

    skipPhysicsStruct(headerReader);

    headerReader->seek(0x4C, std::ios_base::cur);

    readWeightedBones(headerReader, outHeader.weightedBoneNames);

    // Main vertex counts
    outHeader.faceCount = headerReader->readUInt32();
    outHeader.boneCount = headerReader->readUInt32();
    outHeader.vertexCount = headerReader->readUInt32();
    outHeader.morphCount = headerReader->readUInt32();

    headerReader->seek(0x8, std::ios_base::cur);
    uint32_t unkArrLen = headerReader->readUInt32();
    headerReader->seek(unkArrLen * 8U, std::ios_base::cur);
    headerReader->seek(0x31, std::ios_base::cur);

    readBoneTree(headerReader, outHeader.boneTree);

    uint32_t unkStructCount = skipUnknownStructs(headerReader);

    headerReader->seek(0x1A, std::ios_base::cur);
    outHeader.bodySkipLen1 = headerReader->readUInt32();
    headerReader->seek(0x10 * unkStructCount, std::ios_base::cur);
    headerReader->seek(0x8, std::ios_base::cur);

    readMeshInfos(headerReader, outHeader.meshInfos);

    uint32_t unkWordCount = headerReader->readUInt32();
    headerReader->seek(2U * unkWordCount, std::ios_base::cur);

    skipMorphs(headerReader);

    readBoneMaps(headerReader, outHeader.boneMaps);

    outHeader.bodySkipLen2 = skipMorphStructsAndFindSkip(headerReader);

    headerReader->seek(0x2F, std::ios_base::cur);

    skipPhysicsStruct(headerReader);

    readPhysxMeshes(headerReader, outHeader.physXMeshes);

    readErrors(headerReader, outHeader.errorMsgs);

    if (headerReader->tell() != headerReader->getLength())
        throw unknown_format_error("Did not reach end of file");
}

void
ModelReader269::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody)
{
    bodyReader->seek(0, std::ios_base::beg);

    for (const auto& x : extHeader.physXMeshes)
        bodyReader->seek(x.dataLength, std::ios_base::cur);

    bodyReader->seek(0x2, std::ios_base::cur);

    std::vector<glm::mat4x4>& reverseBinds = outBody.reverseBinds;
    reverseBinds.resize(extHeader.weightedBoneNames.size());
    for (size_t x = 0; x < extHeader.weightedBoneNames.size(); x++)
    {
        bodyReader->readSingleArray(glm::value_ptr(reverseBinds[x]), 16);
    }

    bodyReader->seek(extHeader.bodySkipLen1, std::ios_base::cur);

    std::vector<glm::quat>& outRotations = outBody.boneRotations;
    std::vector<glm::vec3>& outPositions = outBody.bonePositions;
    outRotations.resize(extHeader.boneTree.size());
    outPositions.resize(extHeader.boneTree.size());
    for (uint32_t x = 0; x < extHeader.boneTree.size(); x++)
    {
        bodyReader->readSingleArray(&outRotations[x].w, 4);
        bodyReader->readSingleArray(&outPositions[x].x, 3);
        bodyReader->seek(36, std::ios_base::cur);
    }

    bodyReader->seek(2 * extHeader.boneTree.size(), std::ios_base::cur);
    bodyReader->seek(2 * extHeader.boneTree.size(), std::ios_base::cur);

    if (extHeader.boneTree.size() > 0)
        bodyReader->seek(0x8, std::ios_base::cur);

    outBody.positions.resize(extHeader.vertexCount);
    outBody.UV1.resize(extHeader.vertexCount);
    outBody.UV2.resize(extHeader.vertexCount);
    outBody.colors.resize(1);
    outBody.colors[0].resize(extHeader.vertexCount);
    outBody.boneIndices.resize(extHeader.vertexCount);
    outBody.boneWeights.resize(extHeader.vertexCount);
    for (uint32_t x = 0; x < extHeader.vertexCount; x++)
    {
        outBody.positions[x][0] = bodyReader->readInt16() / 32767.0F;
        outBody.positions[x][1] = bodyReader->readInt16() / 32767.0F;
        outBody.positions[x][2] = bodyReader->readInt16() / 32767.0F;
        outBody.positions[x][3] = bodyReader->readInt16() / 32767.0F;

        bodyReader->seek(4, std::ios_base::cur); // Normals?
        
        bodyReader->seek(3, std::ios::cur); // Tangent data?
        
        // Ambient Occlusion
        uint8_t col = bodyReader->readUInt8();
        outBody.colors[0][x][0] = col;
        outBody.colors[0][x][1] = col;
        outBody.colors[0][x][2] = col;

        outBody.UV1[x][0] = bodyReader->readHalf();
        outBody.UV1[x][1] = bodyReader->readHalf();

        outBody.UV2[x][0] = bodyReader->readHalf();
        outBody.UV2[x][1] = bodyReader->readHalf();

        outBody.boneIndices[x][0] = bodyReader->readUInt8();
        outBody.boneIndices[x][1] = bodyReader->readUInt8();
        outBody.boneIndices[x][2] = bodyReader->readUInt8();
        outBody.boneIndices[x][3] = bodyReader->readUInt8();

        uint32_t weightUInt = bodyReader->readUInt32();
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

    bodyReader->seek(extHeader.bodySkipLen2 * 8U, std::ios_base::cur);

    outBody.indices.resize(extHeader.faceCount);
    bodyReader->readUInt16Array(outBody.indices.data(), extHeader.faceCount);

    if (bodyReader->tell() != bodyReader->getLength())
        throw unknown_format_error("Did not reach end of file");
}