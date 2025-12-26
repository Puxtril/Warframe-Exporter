#include "model/ModelReader.h"

using namespace WarframeExporter::Model;

void
ModelReader::readWeightedBones(BinaryReader::BinaryReaderBuffered* reader, std::vector<std::string>& outWeightedBones)
{
    uint32_t weightedBoneCount = reader->readUInt32(0, 500, "Too many weighted bones");
    for (uint32_t x = 0; x < weightedBoneCount; x++)
    {
        uint32_t curBoneNameLen = reader->readUInt32();
        outWeightedBones.push_back(reader->readAsciiString(curBoneNameLen));
    }
}

void
ModelReader::readBoneTree(BinaryReader::BinaryReaderBuffered* reader, std::vector<BoneTreeNodeExternal>& outBoneTree)
{
    uint32_t boneTreeLen = reader->readUInt32(0, 500, "Too many items in Bone Tree");
    outBoneTree.resize(boneTreeLen);

    for (uint32_t x = 0; x < boneTreeLen; x++)
    {
        BoneTreeNodeExternal& boneNode = outBoneTree[x];

        uint32_t boneTreeNameLen = reader->readUInt32();
        boneNode.name = reader->readAsciiString(boneTreeNameLen);
        
        boneNode.parentIndex = reader->readUInt16();
        reader->seek(2, std::ios_base::cur);
    }
}

void
ModelReader::readBoneMaps(BinaryReader::BinaryReaderBuffered* reader, std::vector<std::vector<uint32_t>>& outBoneMaps)
{
    uint32_t boneMapCount = reader->readUInt32(0, 500, "Too many Bone Maps");
    outBoneMaps.resize(boneMapCount);

    for (uint32_t x = 0; x < boneMapCount; x++)
    {
        std::vector<uint32_t>& curBoneMap = outBoneMaps[x];

        uint32_t curBoneMapBoneCount = reader->readUInt32();
        curBoneMap.resize(curBoneMapBoneCount);

        for (uint32_t y = 0; y < curBoneMapBoneCount; y++)
            curBoneMap[y] = reader->readUInt32();
    }
}

void
ModelReader::readMeshInfos(BinaryReader::BinaryReaderBuffered* reader, std::vector<MeshInfoExternal>& outMeshInfos)
{
    uint32_t meshInfoCount = reader->readUInt32(0, 50, "Too many meshinfos");
    
    outMeshInfos.resize(meshInfoCount);
    for (uint32_t x = 0; x < meshInfoCount; x++)
    {
        MeshInfoExternal& meshInfo = outMeshInfos[x];

        reader->readSingleArray(&meshInfo.vector1.x, 4);
        reader->readSingleArray(&meshInfo.vector2.x, 4);

        uint32_t meshInfoNameLen = reader->readUInt32();
        meshInfo.name = reader->readAsciiString(meshInfoNameLen);

        reader->readUInt32Array(meshInfo.faceLODOffsets.data(), 5);
        reader->readUInt32Array(meshInfo.faceLODCounts.data(), 5);

        reader->seek(0x20, std::ios_base::cur);
        uint32_t unkNameLen = reader->readUInt32();
        reader->seek(unkNameLen, std::ios_base::cur);
        reader->seek(0x10, std::ios_base::cur);
    }
}

void
ModelReader::readMaterialPaths(BinaryReader::BinaryReaderBuffered* reader, std::vector<std::string>& outMaterialpaths)
{
    uint32_t materialPathArrayLen = reader->readUInt32(0, 100, "Too many materials");

    for (uint32_t x = 0; x < materialPathArrayLen; x++)
    {
        uint32_t curMaterialPathLen = reader->readUInt32();
        outMaterialpaths.push_back(reader->readAsciiString(curMaterialPathLen));
    }
}

void
ModelReader::readPhysxMeshes(BinaryReader::BinaryReaderBuffered* reader, std::vector<PhysXMesh>& outPhysxMeshes)
{
    uint32_t physXMeshCount = reader->readUInt32(0, 300, "Too many PhysX Meshes");
    outPhysxMeshes.resize(physXMeshCount);

    for (uint32_t x = 0; x < physXMeshCount; x++)
    {
        PhysXMesh& curPhysXMesh = outPhysxMeshes[x];

        curPhysXMesh.typeEnum = reader->readUInt32();
        if (curPhysXMesh.typeEnum == 1)
            reader->seek(0x4C, std::ios_base::cur);
        else if (curPhysXMesh.typeEnum == 7)
            reader->seek(0x60, std::ios_base::cur);
        else
            reader->seek(0x50, std::ios_base::cur);

        reader->readSingleArray(&curPhysXMesh.vector1.x, 4);
        reader->readSingleArray(&curPhysXMesh.vector2.x, 4);

        if (curPhysXMesh.typeEnum != 0 && curPhysXMesh.typeEnum != 2 && curPhysXMesh.typeEnum != 3)
            reader->seek(0x4, std::ios_base::cur);

        reader->seek(0x4, std::ios_base::cur);

        curPhysXMesh.dataLength = reader->readUInt32();
        reader->seek(0x8, std::ios_base::cur);
    }
}

void
ModelReader::readErrors(BinaryReader::BinaryReaderBuffered* reader, std::vector<std::string>& outErrors)
{
    uint32_t errorCount = reader->readUInt32();
    outErrors.resize(errorCount);

    for (uint32_t x = 0; x < errorCount; x++)
    {
        uint32_t errorCountStrLen = reader->readUInt32();
        outErrors[x] = reader->readAsciiString(errorCountStrLen);
    }
}

void
ModelReader::skipUnknownVector(BinaryReader::BinaryReaderBuffered* reader)
{
    // I really don't know
    uint16_t vecCheck = reader->readUInt16();
    if (vecCheck != 256 && vecCheck != 257 && vecCheck != 258 && vecCheck != 260 && vecCheck != 0)
        reader->seek(-2, std::ios_base::cur);
}

void
ModelReader::skipUnk16Array(BinaryReader::BinaryReaderBuffered* reader)
{
    uint32_t unkShortLen = reader->readUInt32(0, 30, "Too many UnkShort values");
    reader->seek(unkShortLen * 2, std::ios_base::cur);
}

void
ModelReader::skipUnk64Array(BinaryReader::BinaryReaderBuffered* reader)
{
    uint32_t uint64LODUnkLen = reader->readUInt32(0, 10, "Too many Unk64 values");
    reader->seek(uint64LODUnkLen * 8U, std::ios_base::cur);
}

uint32_t
ModelReader::skipUnknownStructs(BinaryReader::BinaryReaderBuffered* reader)
{
    uint32_t unkStructCount = reader->readUInt32();
    for (uint32_t x = 0; x < unkStructCount; x++)
    {
        reader->seek(0x10, std::ios_base::cur);
        uint32_t nameLen = reader->readUInt32();
        reader->seek(nameLen, std::ios_base::cur);
        reader->seek(0x4C, std::ios_base::cur);
    }

    return unkStructCount;
}

void
ModelReader::skipMorphs(BinaryReader::BinaryReaderBuffered* reader)
{
    uint32_t morphArrCount = reader->readUInt32();
    for (uint32_t x = 0; x < morphArrCount; x++)
    {
        uint32_t morphNameLen = reader->readUInt32();
        reader->seek(morphNameLen, std::ios_base::cur);
        reader->seek(4, std::ios_base::cur);
    }
}

uint32_t
ModelReader::skipMorphStructsAndFindSkip(BinaryReader::BinaryReaderBuffered* reader, std::vector<MeshInfoExternal>& outMeshInfos)
{
    uint32_t totalSkip = 0;

    uint32_t morphStructArrayCount = reader->readUInt32();
    if (morphStructArrayCount > 0 && morphStructArrayCount != outMeshInfos.size())
        throw unknown_format_error("Morph Struct array count != meshinfo count (" + std::to_string(morphStructArrayCount) + " != " + std::to_string(outMeshInfos.size()) + ")");

    for (uint32_t iMorphStruct = 0; iMorphStruct < morphStructArrayCount; iMorphStruct++)
    {
        MeshInfoExternal& curMeshInfo = outMeshInfos[iMorphStruct];

        // 4 arrays of 5 integers each
        // 1st: Length Multipliers
        // 2nd: Vertex Offsets
        // 3rd: Face offsets?
        // 4th: ???
        reader->readUInt32(5, 5, "MorphStruct 1st 5");
        uint32_t lengthMult = reader->readUInt32();
        reader->seek(16, std::ios::cur);

        reader->readUInt32(5, 5, "MorphStruct 2nd 5");
        reader->readUInt32Array(&curMeshInfo.faceLODVertexOffsets[0], 5);

        reader->readUInt32(5, 5, "MorphStruct 3rd 5");
        reader->seek(20, std::ios::cur);

        reader->readUInt32(5, 5, "MorphStruct 4th 5");
        uint32_t lengthBase = reader->readUInt32();
        reader->seek(16, std::ios::cur);

        totalSkip += lengthBase * std::max(1u, lengthMult);
        
        int morphNameArrayLen = reader->readUInt32(1, 30, "Morph name array len");
        for (uint32_t iMorphName = 0; iMorphName < morphNameArrayLen; iMorphName++)
        {
            int morphNameLen = reader->readUInt32(1, 80, "Morph name");
            reader->seek(morphNameLen, std::ios::cur);
        }
        
        int unkArrayCount = reader->readUInt32(0, 30, "Morph unk array count34");
        reader->seek(unkArrayCount * 4, std::ios::cur);
    }

    return totalSkip;
}

void
ModelReader::skipPhysicsStruct(BinaryReader::BinaryReaderBuffered* reader)
{
    uint32_t type = reader->readUInt32();
    uint32_t subType = reader->readUInt32();

    if (type & 8 || subType & 8)
        reader->seek(2, std::ios::cur);

    uint32_t pathLen = reader->readUInt32();
    reader->seek(pathLen, std::ios::cur);
}

void
ModelReader::skipPhysicsStruct2(BinaryReader::BinaryReaderBuffered* reader)
{
    uint32_t type = reader->readUInt32();
    uint32_t subType = reader->readUInt32();

    if (type == 8 || subType == 8)
        reader->seek(2, std::ios::cur);

    uint32_t pathLen = reader->readUInt32();
    reader->seek(pathLen, std::ios::cur);
}

bool
ModelReader::canContinueReading(BinaryReader::BinaryReaderBuffered* reader, int vertexIndexCount)
{
    int remainingBytes = reader->getLength() - reader->tell();
    if ((vertexIndexCount * 2) > remainingBytes)
        return false;
    return true;
}