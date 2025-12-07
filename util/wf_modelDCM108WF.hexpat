#pragma once

#include <wf_utils.hexpat>
#include <wf_common-header.hexpat>
#include <wf_models.hexpat>

struct ModelDCM108WF {
    padding[0x30];
    PhysicsPath physicsPath1;
    padding[0x48];
    u32 vertexCount;
    u32 faceCount;
    u32 morphCount;
    u32 boneCount;
    padding[0x22];
    u32 vertexCountBCache;
    u32 faceCountBCache;
    padding[0x3D];
    MeshInfos meshInfos;
    Unk16s unk16s;
    padding[0x1];
    u32 dcmEnum;
    if (dcmEnum == 2) {
        padding[6];
    }
    PhysicsPath physicsPath2;
    PhysXMeshes physx;
};

CommonHeader Header @ $;
ModelDCM108WF model @ $;