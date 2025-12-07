#pragma once

#include <wf_utils.hexpat>
#include <wf_common-header.hexpat>
#include <wf_models.hexpat>

struct Model102SF {
    padding[0x30];
    PhysicsPath2 physicsPath1;
    padding[0x2E];
    float ensmall1[4];
    float ensmall2[4];
    u32 vertexCount;
    u32 faceCount;
    u32 morphCount;
    u32 boneCount;
    padding[0x50];
    u32 vertexCountBCache;
    u32 indexCountBCache;
    str32 devPath;
    padding[0x41];
    MeshInfos meshInfos;
    Unk16s unk16;
    PhysicsPath2 physicsPath2;
    PhysXMeshes physxMeshes;
    Errors errors;
};

CommonHeader Header @ $;
Model102SF model @ $;
