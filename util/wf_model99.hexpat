#pragma once

#include <wf_utils.hexpat>
#include <wf_common-header.hexpat>

/****************************************************************/
// Base Structs

struct meshInfo{
    vec4Float vec1;
    vec4Float vec2;
    str32 meshName;
    u32 faceLODOffsets[5];
    u32 faceLODLengths[5];
    padding[0x34];
};

struct meshPhysX {
    u32 meshType;
    if (meshType == 1) {
        padding[0x4C];
    } else {
        padding[0x50];
    }
    
    vec4Float vec1;
    vec4Float vec2;
    if (meshType != 3 && meshType != 2 && meshType != 0) {
        padding[4];
    }
    s32 numCheck;
    u32 meshLen;
    u32 unkLen;
    u32 unkNum;
};

/****************************************************************/
// Main struct

struct Model99 {
    padding[0x1C];
    u32 LODs[5];
    u32 unkScalar1;
    u32 unkScalar2;

    u32 physicsPath1Len;
    if (physicsPath1Len != 0) {
        u16 strCheck;
        if (strCheck != 0) {
            char physPath1[physicsPath1Len-2];
        }
    }
    
    padding[0x18];
    float randomFloatTest;
    padding[0x12];
    vec4Float vec1;
    vec4Float vec2;

    u32 vertexCount;
    u32 faceCount;
    u32 morphCount;
    u32 boneCount;

    padding[0x14];
    u32 randomAlways2Int;
    
    u32 skipIntCount;
    u64 floats[skipIntCount];
    
    padding[0x51];

    u32 meshInfoCount;
    meshInfo meshInfos[meshInfoCount];

    u32 unkWordCount;
    padding[unkWordCount * 2];

    u32 materialPathArrLen;
    str32 materials[materialPathArrLen];

    padding[0x8];

    u32 physicsPath2Len;
    if (physicsPath2Len != 0) {
        u16 strCheck;
        if (strCheck != 0) {
            char physPath2[physicsPath2Len-2];
        }
    }
    
    u32 physXArrLen;
    meshPhysX physXArray[physXArrLen];
    
    str32Arr32 errors;
};

/****************************************************************/
// File definition

CommonHeader Header @ $;
Model99 model @ $;