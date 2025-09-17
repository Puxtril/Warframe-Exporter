#pragma once

#include <wf_utils.hexpat>
#include <wf_common-header.hexpat>
#include <wf_models.hexpat>

struct UnkTerrainStruct {
    u32 count;
    u32 int32s;
    u32 maybeMatrix[4];
};

struct Model159 {
    str32 str1;
    str32 str2;
    padding[0x20];
    u64 faceCount;
    MeshInfos meshInfos;
    u32 vertexCount;
    padding[0x10];
    u32 bodySkipLen1;
    padding[0x20];
    u32 unkTerrainStructCount;
    UnkTerrainStruct unkstruct[unkTerrainStructCount];
    padding[0xC];
    PhysXMeshes physxMeshes;
};

CommonHeader Header @ $;
Model159 model @ $;