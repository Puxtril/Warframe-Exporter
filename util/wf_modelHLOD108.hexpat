#pragma once

#include <wf_utils.hexpat>
#include <wf_common-header.hexpat>
#include <wf_models.hexpat>

struct ModelHLOD108 {
    padding[0x30];
    PhysicsPath physicsPath1;
    padding[0x48];
    u32 vertexCount;
    u32 faceCount;
    u32 morphCount;
    u32 boneCount;
    padding[0x67];
    MeshInfos meshInfos;
    Unk16s unk16s;
    PhysicsPath physicsPath2;
    PhysXMeshes physx;
};

CommonHeader Header @ $;
ModelHLOD108 model @ $;