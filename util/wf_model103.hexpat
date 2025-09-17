#pragma once

#include <wf_utils.hexpat>
#include <wf_common-header.hexpat>
#include <wf_models.hexpat>

struct Model103 {
    padding[0x30];
    PhysicsPath physicsPath1;
    padding[0x4E];
    u32 vertexCount;
    u32 faceCount;
    u32 morphCount;
    u32 boneCount;
    padding[0x75];
    MeshInfos meshInfos;
    Unk16s unk16;
    MaterialPaths materialPaths;
    PhysicsPath physicsPath2;
    PhysXMeshes physxMeshes;
    Errors errors;
};

CommonHeader Header @ $;
Model103 model @ $;