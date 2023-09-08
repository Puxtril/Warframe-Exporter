#pragma once

#include <wf_utils.hexpat>
#include <wf_common-header.hexpat>

/****************************************************************/
// Base Structs

struct boneTreeNode{
    str32 name;
    u16 parentID;
    u16 ID [[hidden]]; // Not used
} [[single_color]];

struct unkStruct{
    padding[0x10];
    str32 name;
    padding[0x4C];
};

struct meshInfo{
    vec4Float vec1;
    vec4Float vec2;
    str32 meshName;
    u32 faceLODOffsets[5];
    u32 faceLODLengths[5];
    padding[0x34];
};

struct morphNameEntry{
    str32 name;
    u32 unkNum;
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
        padding[8];;
    }
    s32 numCheck;
    u32 meshLen;
    u32 unkLen;
    u32 unkNum;
};

struct morphSubArr {
    u32 subArrCount [[hidden]];
    padding[subArrCount * 4];
};

/****************************************************************/
// 2nd tier structs

struct morphStruct {
    morphSubArr subArrs[4];
    str32Arr32 morphNames;
    u32 unkArrCount;
    padding[unkArrCount * 4];
};

struct boneMap{
    u32 boneMapBoneCount [[hidden]];
    u32 boneMapNums[boneMapBoneCount];
};

/****************************************************************/
// Main struct

struct Model86 {
    padding[0x26];
    
    u32 unkInt1;
    u32 unkint2;
    u32 physicsPath1Len;

    if (physicsPath1Len != 0) {
        u16 strCheck;
        if (strCheck != 0) {
            char physPath1[physicsPath1Len-2];
        }
    }
    
    padding[0x2E];

    vec4Float vec1;
    vec4Float vec2;

    u32 vertexCount;
    u32 faceCount;
    u32 morphCount;
    u32 boneCount;
    
    padding[0x18];
    
    u32 unkArrLen;
    be u64 unkArrNums[unkArrLen];

    padding[0xC];
    
    str32 somePath;
    
    padding[0x46];

    u32 meshInfoCount;
    meshInfo meshInfos[meshInfoCount];

    u32 unkWordCount;
    padding[unkWordCount * 2];

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
Model86 model @ $;