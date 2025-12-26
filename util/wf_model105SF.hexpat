#pragma once

#include <wf_utils.hexpat>
#include <wf_common-header.hexpat>
#include <wf_models.hexpat>

struct Model105SF {
    padding[0x30];
    PhysicsPath2 physicsPath1;
    padding[0x28];
    float ensmall1[4];
    float ensmall2[4];
    u32 vertexCount;
    u32 faceCount;
    u32 morphCount;
    u32 boneCount;
    padding[0x22];
    u32 vertexCountBCache;
    u32 indexCountBCache;
    padding[0x3D];
    MeshInfos meshInfos;
    Unk16s unk16;
    PhysicsPath2 physicsPath2;
    PhysXMeshes physxMeshes;
    Errors errors;
};

CommonHeader Header @ $;
Model105SF model @ $;
