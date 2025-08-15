#include "model/types/ModelReader290.h"

using namespace WarframeExporter::Model;

void
ModelReader290::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    headerReader->seek(0x30, std::ios_base::cur);

    skipPhysicsStruct(headerReader);

    headerReader->seek(0x4C, std::ios_base::cur);

    readWeightedBones(headerReader, outHeader.weightedBoneNames);

    // Main vertex counts
    outHeader.faceCount = headerReader->readUInt32();
    outHeader.boneCount = headerReader->readUInt32();
    outHeader.vertexCount = headerReader->readUInt32();
    outHeader.morphCount = headerReader->readUInt32(0, 0, "Non-zero Morphs");

    headerReader->seek(0x8, std::ios_base::cur);
    
    skipUnk64Array(headerReader);
    
    headerReader->seek(0x31, std::ios_base::cur);

    readBoneTree(headerReader, outHeader.boneTree);

    uint32_t unkStructCount = skipUnknownStructs(headerReader);

    headerReader->seek(0x1A, std::ios_base::cur);
    outHeader.bodySkipLen1 = headerReader->readUInt32(0, 1000, "BodySkipLen1");
    headerReader->seek(0x10 * unkStructCount, std::ios_base::cur);
    headerReader->seek(0x8, std::ios_base::cur);

    readMeshInfos(headerReader, outHeader.meshInfos);

    skipUnk16Array(headerReader);

    skipMorphs(headerReader);

    readBoneMaps(headerReader, outHeader.boneMaps);

    outHeader.bodySkipLen2 = skipMorphStructsAndFindSkip(headerReader);

    headerReader->seek(0x2F, std::ios_base::cur);

    // Sub models?
    uint32_t subModelCount = headerReader->readUInt32(0, 30, "Submodel count");
    for (uint32_t x = 0; x < subModelCount; x++)
    {
        headerReader->seek(0xC, std::ios::cur);
        uint32_t nameCount = headerReader->readUInt32(0, 100, "Sub-model name");
        headerReader->seek(nameCount, std::ios::cur);
        headerReader->seek(80, std::ios::cur);
    }

    headerReader->seek(0x1D, std::ios::cur);

    skipPhysicsStruct(headerReader);

    readPhysxMeshes(headerReader, outHeader.physXMeshes);

    readErrors(headerReader, outHeader.errorMsgs);

    if (headerReader->tell() != headerReader->getLength())
        throw unknown_format_error("Did not reach end of file");
}

void
ModelReader290::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody)
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
        bodyReader->seek(12, std::ios_base::cur);
    }

    bodyReader->seek(2 * extHeader.boneTree.size(), std::ios_base::cur);
    bodyReader->seek(2 * extHeader.boneTree.size(), std::ios_base::cur);

    if (extHeader.boneTree.size() > 0)
        bodyReader->seek(0x4, std::ios_base::cur);

    outBody.positions.resize(extHeader.vertexCount);
    outBody.normals.resize(extHeader.vertexCount);
    outBody.tangents.resize(extHeader.vertexCount);
    outBody.UV1.resize(extHeader.vertexCount);
    outBody.UV2.resize(extHeader.vertexCount);
    outBody.AO.resize(extHeader.vertexCount);
    outBody.boneIndices.resize(extHeader.vertexCount);
    outBody.boneWeights.resize(extHeader.vertexCount);
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

    if (!canContinueReading(bodyReader, extHeader.faceCount))
        throw unknown_format_error("Incorrect index count");

    outBody.indices.resize(extHeader.faceCount);
    bodyReader->readUInt16Array(outBody.indices.data(), extHeader.faceCount);

    if (bodyReader->tell() != bodyReader->getLength())
        throw unknown_format_error("Did not reach end of file");
}