#pragma once

#include <wf_utils.hexpat>
#include <wf_common-header.hexpat>
#include <wf_models.hexpat>

struct Model102 {
    padding[0x30];
    PhysicsPath physicsPath1;
    padding[0x4E];
    u32 vertexCount;
    u32 faceCount;
    u32 morphCount;
    u32 boneCount;
    padding[0x18];
    Unk64s unk64s;
    padding[0x51];
    MeshInfos meshInfos;
    Unk16s unk16s;
    PhysicsPath physicsPath2;
    PhysXMeshes physx;
};

CommonHeader Header @ $;
Model102 model @ $;