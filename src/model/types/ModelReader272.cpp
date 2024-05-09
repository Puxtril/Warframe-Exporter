#include "model/types/ModelReader272.h"

using namespace WarframeExporter::Model;

void
ModelReader272::readHeaderDebug(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header)
{
    headerReader->seek(0x1C, std::ios_base::cur);
    headerReader->readUInt32Array(5); // Possible LOD Data
    headerReader->readUInt32(); // Not an array
    headerReader->readUInt32(); // Not an array

    // This can be 255 and then not have any content
    static const std::string physicsPath1 = "First physics path";
    uint32_t physicsPathLen = headerReader->readUInt32(0, (int)UINT8_MAX + 1, physicsPath1);
    if (physicsPathLen != 0)
    {
        uint16_t physPathCheck = headerReader->readUInt16();
        if (physPathCheck != 0)
            headerReader->seek(physicsPathLen - 2, std::ios_base::cur);
    }

    headerReader->readUInt32(); // Not an array
    headerReader->readUInt32(); // Probably flags

    headerReader->seek(0x10, std::ios_base::cur);

    static const std::string randomFloat = "Random float test";
    headerReader->readFloat(1.0F, 10.0F, CONV_ZERO | FAIL_SUBNORM, randomFloat);

    headerReader->seek(0x10, std::ios_base::cur);

    // Vector4
    static const std::string firstVec4Msg = "First Vector4 pair";
    headerReader->seek(0xC, std::ios_base::cur);
    headerReader->readFloat(0.0F, 0.00001F, CONV_ZERO | FAIL_SUBNORM, firstVec4Msg);
    // Vector4
    headerReader->seek(0xC, std::ios_base::cur);
    headerReader->readFloat(0.0F, 0.00001F, CONV_ZERO | FAIL_SUBNORM, firstVec4Msg);

    static const std::string weightedBoneNameCountMsg = "Weighted bone array count";
    uint32_t weightedBoneCount = headerReader->readUInt32(0, UINT8_MAX, weightedBoneNameCountMsg);

    static const std::string boneNameCount = "Weighted Bone name length";
    for (uint32_t x = 0; x < weightedBoneCount; x++)
    {
        uint32_t curBoneNameLen = headerReader->readUInt32(0, 100, boneNameCount);
        headerReader->seek(curBoneNameLen, std::ios_base::cur);
    }

    headerReader->readUInt32(); // face count
    headerReader->readUInt32(); // bone count
    headerReader->readUInt32(); // vertex count
    // Can be massively high (and incorrect). Best to compare zero or non-zero
    headerReader->readUInt32(); // morph count

    headerReader->seek(0x8, std::ios_base::cur);
    uint32_t unkArrLen = headerReader->readUInt32();
    headerReader->seek(unkArrLen * 8, std::ios_base::cur);
    headerReader->seek(0x31, std::ios_base::cur);

    static const std::string boneTreeLenMsg = "Bone Data Array Count";
    uint32_t boneTreeLen = headerReader->readUInt32(0, 1000, boneTreeLenMsg); // sometimes can be 0 if weighted is 1

    static const std::string boneTreeNameLenMsg = "Bone Data name length";
    static const std::string boneTreeIndexMsg = "Bone Data index";
    for (uint32_t x = 0; x < boneTreeLen; x++)
    {
        uint32_t boneTreeNameLen = headerReader->readUInt32(0, 100, boneTreeNameLenMsg);
        headerReader->seek(boneTreeNameLen, std::ios_base::cur);
        headerReader->readUInt16(0, boneTreeLen + 1, boneTreeIndexMsg); //parent index
        headerReader->readUInt16();
    }

    //  0: /Lotus/Characters/Tenno/Accessory/Armour/PrimeOctaviaArmour/PrimeOctaviaArmourAL_skel.fbx
    //  1: Everything else
    //  2: /Lotus/Weapons/Thanotech/ThanoRifle/ThanotechRifle_skel.fbx
    uint32_t unkStructCount = headerReader->readUInt32();
    for (uint32_t x = 0; x < unkStructCount; x++)
    {
        headerReader->seek(0x10, std::ios_base::cur);
        uint32_t nameLen = headerReader->readUInt32();
        headerReader->seek(nameLen, std::ios_base::cur);
        headerReader->seek(0x4C, std::ios_base::cur);
    }

    const static std::string unkMsg = "Some UInt that should stay 1?";
    headerReader->readUInt32(1, 2, unkMsg);

    headerReader->seek(0x16, std::ios_base::cur);
    headerReader->readUInt32();
    headerReader->seek(0x10 * unkStructCount, std::ios_base::cur);
    headerReader->seek(0x8, std::ios_base::cur);

    const static std::string meshInfoMsg = "Mesh Info Count";
    uint32_t meshInfoCount = headerReader->readUInt32(1, 30, meshInfoMsg);
    for (uint32_t x = 0; x < meshInfoCount; x++)
    {
        static const std::string meshInfoVec4Msg = "MeshInfo Vector4 pair";
        headerReader->seek(0xC, std::ios_base::cur);
        headerReader->readFloat(0.0F, 0.00001F, CONV_ZERO | FAIL_SUBNORM, meshInfoVec4Msg);
        headerReader->seek(0xC, std::ios_base::cur);
        headerReader->readFloat(0.0F, 0.00001F, CONV_ZERO | FAIL_SUBNORM, meshInfoVec4Msg);

        uint32_t meshInfoNameLen = headerReader->readUInt32();
        headerReader->seek(meshInfoNameLen, std::ios_base::cur);

        auto meshInfoOffset = headerReader->tell();
        try {
            static const std::string LODOffsetMsg = "MeshInfo LOD Offset";
            uint32_t curFaceLODOffset = headerReader->readUInt32();
            curFaceLODOffset = headerReader->readUInt32(curFaceLODOffset, UINT32_MAX, LODOffsetMsg);
            curFaceLODOffset = headerReader->readUInt32(curFaceLODOffset, UINT32_MAX, LODOffsetMsg);
            curFaceLODOffset = headerReader->readUInt32(curFaceLODOffset, UINT32_MAX, LODOffsetMsg);
            curFaceLODOffset = headerReader->readUInt32(curFaceLODOffset, UINT32_MAX, LODOffsetMsg);

            static const std::string LODFaceCountMsg = "MeshInfo LOD Face";
            uint32_t curFaceLODCount = headerReader->readUInt32();
            curFaceLODCount = headerReader->readUInt32(0, curFaceLODCount + 1, LODFaceCountMsg);
            curFaceLODCount = headerReader->readUInt32(0, curFaceLODCount + 1, LODFaceCountMsg);
            curFaceLODCount = headerReader->readUInt32(0, curFaceLODCount + 1, LODFaceCountMsg);
            curFaceLODCount = headerReader->readUInt32(0, curFaceLODCount + 1, LODFaceCountMsg);
        }
        // Edge case where only 1 LOD exists
        catch (LimitException&)
        {
            headerReader->seek(meshInfoOffset, std::ios_base::beg);

            static const std::string LODOffsetMsg = "MeshInfo LOD Offset edge case";
            headerReader->readUInt32(0, UINT32_MAX, LODOffsetMsg);
            headerReader->readUInt32Array(4, 0, 1, LODOffsetMsg);

            static const std::string LODFaceCountMsg = "MeshInfo LOD Face count edge case";
            headerReader->readUInt32(0, UINT32_MAX, LODFaceCountMsg);
            headerReader->readUInt32Array(4, 0, 1, LODFaceCountMsg);
        }

        headerReader->seek(0x34, std::ios_base::cur);
    }

    uint32_t unkWordCount = headerReader->readUInt32();
    headerReader->seek(2U * unkWordCount, std::ios_base::cur);

    // Array of morph names
    const static std::string morphArrayMsg = "First morph array count";
    const static std::string morphUnkIntMsg = "Morph Array post-name unknown int";
    uint32_t morphArrCount = headerReader->readUInt32(0, 100, morphArrayMsg);
    for (uint32_t x = 0; x < morphArrCount; x++)
    {
        uint32_t morphNameLen = headerReader->readUInt32();
        headerReader->seek(morphNameLen, std::ios_base::cur);
        headerReader->readUInt32(0, 1, morphUnkIntMsg);
    }

    // Bone map
    static const std::string boneMapCountMsg = "Bone Map count, equals meshinfo count";
    uint32_t boneMapCount = headerReader->readUInt32(meshInfoCount, meshInfoCount + 1, boneMapCountMsg);
    for (uint32_t x = 0; x < boneMapCount; x++)
    {
        uint32_t curBoneMapBoneCount = headerReader->readUInt32();
        for (uint32_t y = 0; y < curBoneMapBoneCount; y++)
        {
            headerReader->readUInt32();
        }
    }

    static const std::string morphStructArrayCountMsg = "Morph struct array count";
    uint32_t morphStructArrayCount = headerReader->readUInt32(0, 10, morphStructArrayCountMsg);
    for (uint32_t x = 0; x < morphStructArrayCount; x++)
    {
        static const std::string unkMorphStructArrMsg = "Unknown array inside morph struct";
        for (int x = 0; x < 4; x++)
        {
            uint32_t recurringUnkSubArr = headerReader->readUInt32(5, 6, unkMorphStructArrMsg);
            headerReader->seek(4U * recurringUnkSubArr, std::ios_base::cur);
        }
    
        uint32_t morphNameArrCount = headerReader->readUInt32();
        for (uint32_t x = 0; x < morphNameArrCount; x++)
        {
            uint32_t morphNameLen = headerReader->readUInt32();
            headerReader->seek(morphNameLen, std::ios_base::cur);
        }

        uint32_t unkArrCount = headerReader->readUInt32();
        headerReader->seek(unkArrCount * 4U, std::ios_base::cur);
    }

    headerReader->seek(0x37, std::ios_base::cur);

    static const std::string physicsPath2 = "Second physics path";
    uint32_t physicsPath2Len = headerReader->readUInt32(0, (int)UINT8_MAX + 1, physicsPath2);
    if (physicsPath2Len != 0)
    {
        uint16_t physPathCheck = headerReader->readUInt16();
        if (physPathCheck != 0)
            headerReader->seek(physicsPath2Len - 2, std::ios_base::cur);
    }

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
    
        static const std::string physXNumCheckMsg = "PhysX Num check";
        headerReader->readInt32(-1, 0, physXNumCheckMsg);

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
ModelReader272::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    headerReader->seek(0x38, std::ios_base::cur);

    skipPhysicsPath2(headerReader);

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

    headerReader->seek(0x37, std::ios_base::cur);

    skipPhysicsPath2(headerReader);

    readPhysxMeshes(headerReader, outHeader.physXMeshes);

    readErrors(headerReader, outHeader.errorMsgs);

    if (headerReader->tell() != headerReader->getLength())
        throw unknown_format_error("Did not reach end of file");
}

void
ModelReader272::readBodyDebug(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader)
{
    bodyReader->seek(0, std::ios_base::beg);

    for (const auto& x : extHeader.physXMeshes)
        bodyReader->seek(x.dataLength, std::ios_base::cur);

    bodyReader->seek(0x2, std::ios_base::cur);

    bodyReader->readSingleArray(16 * (extHeader.weightedBoneNames.size()));

    bodyReader->seek(extHeader.bodySkipLen1, std::ios_base::cur);

    bodyReader->readSingleArray(10 * extHeader.boneTree.size());
    bodyReader->readUInt16Array(extHeader.boneTree.size());
    bodyReader->readUInt16Array(extHeader.boneTree.size());

    if (extHeader.boneTree.size() > 0)
    {
        bodyReader->seek(0x4, std::ios_base::cur);
        static const std::string preVertFloatMsg = "Pre Vertex Always 1 float";
        bodyReader->readFloat(1.0F, 1.01F, CONV_ZERO | FAIL_SUBNORM, preVertFloatMsg);
    }

    for (uint32_t x = 0; x < extHeader.vertexCount; x++)
    {
        bodyReader->seek(6, std::ios_base::cur); // Verts
        bodyReader->seek(6, std::ios_base::cur); // Normals
        bodyReader->seek(4, std::ios_base::cur); // Vertex Colors
        bodyReader->seek(8, std::ios_base::cur); // UV Maps
        bodyReader->seek(4, std::ios_base::cur); // Bone Indices
        bodyReader->seek(4, std::ios_base::cur); // Bone Weights
    }

    bodyReader->seek(extHeader.bodySkipLen2 * 8U, std::ios_base::cur);

    static const std::string facesIndiciesMsg = "Face Indices";
    bodyReader->readUInt16Array(extHeader.faceCount, 0, extHeader.vertexCount + 1, facesIndiciesMsg);

    if (bodyReader->tell() != bodyReader->getLength())
        throw unknown_format_error("Did not reach end of file");
}

void
ModelReader272::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody)
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

        bodyReader->seek(6, std::ios_base::cur); // Normals?

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