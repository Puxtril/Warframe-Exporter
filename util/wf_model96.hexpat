#pragma once

#include <wf_utils.hexpat>
#include <wf_common-header.hexpat>
#include <wf_models.hexpat>

struct Model96 {
    padding[0x30];
    PhysicsPath physicsPath1;
    padding[0x2C];
    // May need to comment this out
    u16 num;
    padding[0x20];
    u32 vertexCount;
    u32 faceCount;
    u32 morphCount;
    u32 boneCount;
    padding[0x18];
    Unk64s unk64;
    padding[0xC];
    str32 somePath;
    padding[0x41];
    MeshInfos meshInfos;
    Unk16s unk16;
    PhysicsPath physicsPath2;
    PhysXMeshes physxMeshes;
    Errors errors;
};

CommonHeader Header @ $;
Model96 model @ $;
