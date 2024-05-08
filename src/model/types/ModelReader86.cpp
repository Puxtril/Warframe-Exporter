#include "model/types/ModelReader86.h"

using namespace WarframeExporter::Model;

void
ModelReader86::readHeaderDebug(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header)
{
    headerReader->seek(0x26, std::ios_base::cur);

    headerReader->readUInt32(); // Not an array
    headerReader->readUInt32(); // Not an array

    // Missing pre-check, len is non-zero, path doesn't exist: /Lotus/Characters/Orokin/OrokinStatue/OrokinStatueDestroyedHead.fbx
    // Missing pre-check, len is non-zero, path exists:        /Lotus/Characters/Grineer/GrineerTwinQueens/GrineerQueenSceptreBrokenVesselB.fbx
    // Has pre-check,     len is zero,     path doesn't exist: /Lotus/Characters/NewWar/Tefillah/TefillahDamaged.fbx
    // Has pre-check,     len is non-zero, path exists:        /Lotus/Objects/SolarisUnited/ExteriorKit/SUExtPipe90Corner.fbx
    static const std::string physicsPath1 = "First physics path Check";
    uint32_t physicsPathCheck = headerReader->readUInt16(0, (int)UINT8_MAX + 1, physicsPath1);
    if (physicsPathCheck > 0)
    {
        uint32_t physPathLen;
        if (physicsPathCheck == 255)
        {
            physPathLen = headerReader->readUInt32();
        }
        else
        {
            physPathLen = physicsPathCheck;
            headerReader->seek(0x2, std::ios_base::cur);
        }
    
        // Because the path may not actually exist...
        if (physPathLen > 0)
        {
            uint16_t nullCheck = headerReader->readUInt16();
            if (nullCheck != 0)
                headerReader->seek(physPathLen - 2, std::ios_base::cur);
        }
    }
    else
        headerReader->seek(0x2, std::ios_base::cur);

    headerReader->readUInt32(); // Not an array
    headerReader->readUInt32(); // Probably flags

    headerReader->seek(0x10, std::ios_base::cur);

    // 99% this is 1
    // 100: /Lotus/Characters/Tenno/Operator/Hair/HairD/HairD_sort.fbx
    // 50:  /Lotus/Characters/Tenno/Operator/Hair/HairO/HairO_sort.fbx
    static const std::string randomFloat = "Random float test";
    headerReader->readFloat(1.0F, 101.0F, CONV_ZERO | FAIL_SUBNORM, randomFloat);

    // Can't find the control variable for this
    // I hate lookaheads but /shrug
    // 0x10: 
    // 0x12: /Lotus/Objects/Sentient/Devourer/Meshes/Custom/SDxJunctionFloorTrimB.fbx
    headerReader->seek(0x10, std::ios_base::cur);
    uint16_t vecCheck = headerReader->readUInt16();
    if (vecCheck != 256 && vecCheck != 257 && vecCheck != 0)
        headerReader->seek(-2, std::ios_base::cur);

    // Vector4
    static const std::string firstVec4Msg = "First Vector4 pair";
    headerReader->seek(0xC, std::ios_base::cur);
    //headerReader->readFloat(0.0F, 0.00001F, firstVec4Msg);
    headerReader->readFloat();
    // Vector4
    headerReader->seek(0xC, std::ios_base::cur);
    headerReader->readFloat(0.0F, 0.00001F, CONV_ZERO | FAIL_SUBNORM, firstVec4Msg);

    uint32_t vertexCount = headerReader->readUInt32();
    static const std::string faceCountMsg = "Face count";
    headerReader->readUInt32(vertexCount, UINT32_MAX, faceCountMsg); // face count
    // Can be massively high (and incorrect)
    // Literally have no idea how to read these
    headerReader->readUInt32(); // morph count
    // Because Fuck you
    // These only have 1 bone
    // /Lotus/Characters/Tenno/Operator/Clothes/DrifterGrineer/GrineerDrifterOperatorChestWeapon.fbx
    // /Lotus/Weapons/Grineer/Melee/GrineerCombatKnife/GrineerCombatKnife.fbx
    headerReader->readUInt32(); // bone count

    headerReader->seek(0x18, std::ios_base::cur);

    static const std::string UInt64LODUnkMsg = "UInt64 Array length";
    uint32_t UInt64LODUnkLen = headerReader->readUInt32(0, 10, UInt64LODUnkMsg);
    headerReader->seek(UInt64LODUnkLen * 8, std::ios_base::cur);

    headerReader->seek(0xC, std::ios_base::cur);
    // Only one with a path: /Lotus/Weapons/Grineer/Shield/RiotShield.x
    uint32_t somePathLen = headerReader->readUInt32();
    headerReader->seek(somePathLen, std::ios_base::cur);

    headerReader->seek(0x46, std::ios_base::cur);

    // 0  Meshinfos: /Lotus/Objects/Zariman/Door/ZARxBlastDoorACollisionLeft.fbx
    // 12 Meshinfos: /Lotus/Animations/LevelEvents/CrpVesoSpawn/FallingDebris/CrpVesoSpawnLevelDebrisEndPosition.fbx
    const static std::string meshInfoMsg = "Mesh Info Count";
    uint32_t minMeshInfo = vertexCount > 0 ? 1 : 0;
    uint32_t meshInfoCount = headerReader->readUInt32(minMeshInfo, 30, meshInfoMsg);
    for (uint32_t x = 0; x < meshInfoCount; x++)
    {
        static const std::string meshInfoVec4Msg = "MeshInfo Vector4 pair";
        headerReader->seek(0xC, std::ios_base::cur);
        headerReader->readFloat(0.0F, 0.00001F, CONV_ZERO | FAIL_SUBNORM, meshInfoVec4Msg);
        headerReader->seek(0xC, std::ios_base::cur);
        headerReader->readFloat(0.0F, 0.00001F, CONV_ZERO | FAIL_SUBNORM, meshInfoVec4Msg);

        uint32_t meshInfoNameLen = headerReader->readUInt32();
        headerReader->seek(meshInfoNameLen, std::ios_base::cur);
    
        uint32_t validLODCount = UInt64LODUnkLen;
        uint32_t emptyLODCount = 5 - UInt64LODUnkLen;
    
        static const std::string LODOffsetValidMsg = "MeshInfo LOD Offset Valid";
        uint32_t curFaceLODOffset = 0;
        for (uint32_t x = 0; x < validLODCount; x++)
            curFaceLODOffset = headerReader->readUInt32(curFaceLODOffset, UINT32_MAX, LODOffsetValidMsg);

        static const std::string LODOffsetEmptyMsg = "MeshInfo LOD Offset Empty";
        for (uint32_t x = 0; x < emptyLODCount; x++)
            headerReader->readUInt32(0, 1, LODOffsetEmptyMsg);

        static const std::string LODFaceCountValidMsg = "MeshInfo LOD Face Count Valid";
        uint32_t curFaceLODCount = UINT32_MAX - 1;
        for (uint32_t x = 0; x < validLODCount; x++)
            curFaceLODCount = headerReader->readUInt32(0, curFaceLODCount + 1, LODFaceCountValidMsg);

        static const std::string LODFaceCountEmptyMsg = "MeshInfo LOD Face Count Empty";
        for (uint32_t x = 0; x < emptyLODCount; x++)
            headerReader->readUInt32(0, 1, LODFaceCountEmptyMsg);

        headerReader->seek(0x20, std::ios_base::cur);
        // First model to have something here: /Lotus/Characters/Tenno/Operator/Clothes/DrifterGrineer/GrineerDrifterOperatorChestWeapon.fbx
        uint32_t unkNameLen = headerReader->readUInt32();
        headerReader->seek(unkNameLen, std::ios_base::cur);
        headerReader->seek(0x10, std::ios_base::cur);
    }

    uint32_t shortCount = headerReader->readUInt32();
    headerReader->seek(shortCount * 2U, std::ios_base::cur);

    headerReader->seek(0x8, std::ios_base::cur);

    static const std::string physicsPath2 = "Second physics path";
    int32_t physicsPath2Check = headerReader->readUInt16(0, (int)UINT8_MAX + 1, physicsPath2);
    if (physicsPath2Check > 0)
    {
        uint32_t physPath2Len;
        if (physicsPath2Check == 255)
        {
            physPath2Len = headerReader->readUInt32();
        }
        else
        {
            physPath2Len = physicsPath2Check;
            headerReader->seek(0x2, std::ios_base::cur);
        }

        // Because the path may not actually exist...
        if (physPath2Len > 0)
        {
            uint16_t nullCheck = headerReader->readUInt16();
            if (nullCheck != 0)
                headerReader->seek(physPath2Len - 2, std::ios_base::cur);
        }
    }
    else
        headerReader->seek(0x2, std::ios_base::cur);

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
    
        // Removed number check
        headerReader->readInt32();

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
ModelReader86::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    headerReader->seek(0x2E, std::ios_base::cur);

    uint32_t physicsPathCheck = headerReader->readUInt16();
    if (physicsPathCheck > 0)
    {
        uint32_t physPathLen;
        if (physicsPathCheck == 255)
        {
            physPathLen = headerReader->readUInt32();
        }
        else
        {
            physPathLen = physicsPathCheck;
            headerReader->seek(0x2, std::ios_base::cur);
        }

        // Because the path may not actually exist...
        if (physPathLen > 0)
        {
            uint16_t nullCheck = headerReader->readUInt16();
            if (nullCheck != 0)
                headerReader->seek(physPathLen - 2, std::ios_base::cur);
        }
    }
    else
        headerReader->seek(0x2, std::ios_base::cur);

    headerReader->seek(0x2C, std::ios_base::cur);

    uint16_t vecCheck = headerReader->readUInt16();
    if (vecCheck != 256 && vecCheck != 257 && vecCheck != 258 && vecCheck != 0)
        headerReader->seek(-2, std::ios_base::cur);

    headerReader->seek(0x20, std::ios_base::cur);

    // Main model data
    outHeader.vertexCount = headerReader->readUInt32();
    outHeader.faceCount = headerReader->readUInt32();
    outHeader.morphCount = headerReader->readUInt32();
    outHeader.boneCount = headerReader->readUInt32();

    headerReader->seek(0x18, std::ios_base::cur);

    uint32_t UInt64LODUnkLen = headerReader->readUInt32();
    headerReader->seek(UInt64LODUnkLen * 8U, std::ios_base::cur);

    headerReader->seek(0xC, std::ios_base::cur);
    uint32_t somePathLen = headerReader->readUInt32();
    headerReader->seek(somePathLen, std::ios_base::cur);
    
    headerReader->seek(0x46, std::ios_base::cur);

    // Mesh Infos
    uint32_t meshInfoCount = headerReader->readUInt32();
    outHeader.meshInfos.resize(meshInfoCount);
    for (uint32_t x = 0; x < meshInfoCount; x++)
    {
        MeshInfoExternal& curMeshInfo = outHeader.meshInfos[x];

        headerReader->readSingleArray(&curMeshInfo.vector1.x, 4);
        headerReader->readSingleArray(&curMeshInfo.vector2.x, 4);

        uint32_t meshInfoNameLen = headerReader->readUInt32();
        curMeshInfo.name = headerReader->readAsciiString(meshInfoNameLen);

        headerReader->readUInt32Array(curMeshInfo.faceLODOffsets.data(), 5);
        headerReader->readUInt32Array(curMeshInfo.faceLODCounts.data(), 5);

        headerReader->seek(0x20, std::ios_base::cur);
        uint32_t unkNameLen = headerReader->readUInt32();
        headerReader->seek(unkNameLen, std::ios_base::cur);
        headerReader->seek(0x10, std::ios_base::cur);
    }

    uint32_t shortCount = headerReader->readUInt32();
    headerReader->seek(shortCount * 2U, std::ios_base::cur);

    headerReader->seek(0x8, std::ios_base::cur);

    int32_t physicsPath2Check = headerReader->readUInt16();
    if (physicsPath2Check > 0)
    {
        uint32_t physPath2Len;
        if (physicsPath2Check == 255)
        {
            physPath2Len = headerReader->readUInt32();
        }
        else
        {
            physPath2Len = physicsPath2Check;
            headerReader->seek(0x2, std::ios_base::cur);
        }

        // Because the path may not actually exist...
        if (physPath2Len > 0)
        {
            uint16_t nullCheck = headerReader->readUInt16();
            if (nullCheck != 0)
                headerReader->seek(physPath2Len - 2, std::ios_base::cur);
        }
    }
    else
        headerReader->seek(0x2, std::ios_base::cur);

    // PhysX Meshes
    uint32_t physXMeshCount = headerReader->readUInt32();
    outHeader.physXMeshes.resize(physXMeshCount);
    for (uint32_t x = 0; x < physXMeshCount; x++)
    {
        PhysXMesh& curPhys = outHeader.physXMeshes[x];

        curPhys.typeEnum = headerReader->readUInt32();
        if (curPhys.typeEnum == 1)
            headerReader->seek(0x4C, std::ios_base::cur);
        else
            headerReader->seek(0x50, std::ios_base::cur);

        headerReader->seek(0x20, std::ios_base::cur);

        if (curPhys.typeEnum != 0 && curPhys.typeEnum != 2 && curPhys.typeEnum != 3)
            headerReader->seek(0x4, std::ios_base::cur);

        headerReader->seek(0x4, std::ios_base::cur);

        curPhys.dataLength = headerReader->readUInt32();
        headerReader->seek(0x8, std::ios_base::cur);
    }

    uint32_t errorCount = headerReader->readUInt32();
    outHeader.errorMsgs.resize(errorCount);
    for (uint32_t x = 0; x < errorCount; x++)
    {
        uint32_t errorCountStrLen = headerReader->readUInt32();
        outHeader.errorMsgs[x] = headerReader->readAsciiString(errorCountStrLen);
    }

    if (headerReader->tell() != headerReader->getLength())
        throw unknown_format_error("Did not reach end of file");
}

void
ModelReader86::readBodyDebug(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader)
{
    for (const auto& x : extHeader.physXMeshes)
        bodyReader->seek(x.dataLength, std::ios_base::cur);

    bodyReader->seek(64 * extHeader.boneCount, std::ios_base::cur);

    bodyReader->seek(0x2, std::ios_base::cur);

    for (uint32_t x = 0; x < extHeader.vertexCount; x++)
    {
        bodyReader->seek(6, std::ios_base::cur); // Verts
        bodyReader->seek(6, std::ios_base::cur); // Normals
        bodyReader->seek(4, std::ios_base::cur); // Vertex Colors
        bodyReader->seek(8, std::ios_base::cur); // UV Maps
    }

    //bodyReader->seek(extHeader.getBodySkipLen2() * 8U, std::ios_base::cur);

    static const std::string facesIndiciesMsg = "Face Indices";
    bodyReader->readUInt16Array(extHeader.faceCount, 0, extHeader.vertexCount + 1, facesIndiciesMsg);

    // Extra shorts, no idea where the count is
    // /Lotus/Characters/Tenno/Garuda/GarudaDeluxe/GarudaDeluxeArmClawRParticleEmit.fbx
    // /Lotus/Fx/Weapons/Tenno/Miter/TnoMiterSparkEmitter.fbx
//		if (bodyReader->tell() != bodyReader->getLength())
//			throw unknown_format_error("Did not reach end of file");
}

void
ModelReader86::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody)
{
    for (const auto& x : extHeader.physXMeshes)
        bodyReader->seek(x.dataLength, std::ios_base::cur);

    bodyReader->seek(64U * extHeader.boneCount, std::ios_base::cur);

    bodyReader->seek(0x2, std::ios_base::cur);

    outBody.positions.resize(extHeader.vertexCount);
    outBody.UV1.resize(extHeader.vertexCount);
    outBody.UV2.resize(extHeader.vertexCount);
    outBody.colors.resize(1);
    outBody.colors[0].resize(extHeader.vertexCount);
    for (uint32_t x = 0; x < extHeader.vertexCount; x++)
    {
        outBody.positions[x][0] = bodyReader->readHalf();
        outBody.positions[x][1] = bodyReader->readHalf();
        outBody.positions[x][2] = bodyReader->readHalf();

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
    }

    outBody.indices.resize(extHeader.faceCount);
    bodyReader->readUInt16Array(outBody.indices.data(), extHeader.faceCount);
}