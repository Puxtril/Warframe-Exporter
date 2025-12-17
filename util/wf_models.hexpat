#pragma once

#include <wf_utils.hexpat>

/****************************************************************/
// Sub-Structs (Don't use these directly)

struct BoneTreeNode {
    str32 name;
    u16 parentIndex;
    padding[0x2];
};

struct BoneMap {
    u32 boneCount;
    u32 targetIndex[boneCount];
};

struct MeshInfo {
    vec4Float floats1;
    vec4Float floats2;
    str32 name;
    u32 faceLODOffsets[5];
    u32 faceLODCounts[5];
    padding[0x20];
    str32 unkName;
    padding[0x10];
};

struct PhysXMesh {
    u32 type;
    match (type) {
        (1): padding[0x4C];
        (7): padding[0x60];
        (_): padding[0x50];
    }
    vec4Float floats1;
    vec4Float floats2;
    if (type != 0 && type != 2 && type != 3) {
        padding[0x4];
    }
    padding[0x4];
    u32 bodyLen;
    padding[0x8];
};

struct UnkStruct {
    padding[0x10];
    str32 name;
    padding[0x4C];
};

struct Morph {
    str32 name;
    padding[0x4];
};

struct Morph2Sub {
    u32 subArrCount;
    padding[subArrCount * 4];
};

struct Morph2 {
    Morph2 subArrs[4];
    str32Arr32 morphNames;
    u32 unkArrCount;
    padding[unkArrCount * 4];
};

/****************************************************************/
// Structs used directly by formats

struct WeightedBones {
    str32Arr32 bones;
};

struct BoneTree {
    u32 count;
    BoneTreeNode bones[count];
};

struct BoneMapCollection {
    u32 count;
    BoneMap maps[count];
};

struct MeshInfos {
    u32 count;
    MeshInfo meshInfos[count];
};

struct MaterialPaths {
    str32Arr32 materialPaths;
};

struct PhysXMeshes {
    u32 count;
    PhysXMesh physxMeshes[count];
};

struct Errors {
    str32Arr32 errors;
};

struct Unk16s {
    u32 count;
    u16 unks[count];
};

struct Unk64s {
    u32 count;
    u64 unks[count];
};

struct UnkStructs {
    u32 count;
    UnkStruct structs[count];
};

struct Morphs {
    u32 count;
    Morph morphs[count];
};

struct Morphs2 {
    u32 count;
    Morph2 morphs[count];
};

struct PhysicsPath {
    u32 type;
    u32 subtype;
    if (type & 8 || subtype & 8) {
        padding[0x2];
    }
    str32 path;
};

struct PhysicsPath2 {
    u32 type;
    u32 subtype;
    if (type == 8 || subtype == 8) {
        padding[0x2];
    }
    str32 path;
};
