#pragma once

#include <wf_utils.hexpat>
#include <wf_common-header.hexpat>
#include <wf_models.hexpat>

struct Model283 {
    padding[0x30];
    PhysicsPath2 physicsPath1;
    padding[0x4C];
    WeightedBones weightedBones;
    u32 faceCount;
    u32 boneCount;
    u32 vertexCount;
    u32 morphCount;
    padding[0x6];
    Unk64s unk64s;
    padding[0xC];
    u32 vertexCountBCache;
    u32 indexCountBCache;
    padding[0x25];
    BoneTree boneTree;
    UnkStructs unkStructs;
    padding[0x1A];
    u32 bodySkipLen1;
    padding[unkStructs.count * 0x10];
    padding[0x8];
    MeshInfos meshInfos;
    Unk16s unk16;
    Morphs morphs;
    BoneMapCollection boneMaps;
    Morphs2 morphs2;
    padding[0x2F];
    PhysicsPath2 physicsPath2;
    PhysXMeshes physXMeshes;
    Errors errors;
};

CommonHeader Header @ $;
Model283 model @ $;
