#pragma once

#include <wf_utils.hexpat>
#include <wf_common-header.hexpat>
#include <wf_models.hexpat>

struct SubModel {
    padding[0xC];
    str32 name;
    padding[0x50];
};

struct SubModels {
    u32 count;
    SubModel models[count];
};

struct Model310 {
    padding[0x30];
    PhysicsPath physicsPath1;
    padding[0x4C];
    WeightedBones weightedBones;
    u32 faceCount;
    u32 boneCount;
    u32 vertexCount;
    u32 morphCount;
    padding[0x6];
    Unk64s unk64;
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
    SubModels subModels;
    padding[0x24];
    PhysicsPath physicsPath2;
    PhysXMeshes physxMeshes;
    Errors errors;
};

CommonHeader Header @ $;
Model310 model @ $;