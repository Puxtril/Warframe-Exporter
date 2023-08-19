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
        FSkip(4);
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

struct Model272 {
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
    
    u32 unkScalar3;
    u32 unkFlags1;

    padding[0x10];
    float floatTest;

    padding[0x10];
    vec4Float vec1;
    vec4Float vec2;

    str32Arr32 bones;

    u32 faceCount;
    u32 boneCount;
    u32 vertexCount;
    u32 morphCount;

    padding[0x8];
    u32 unkArrLen;
    be u64 unkArrNums[unkArrLen];
    padding[0x31];
    
    u32 boneTreeLen;
    boneTreeNode boneTree[boneTreeLen];

    u32 unkStructArrLen;
    unkStruct unknownStruct[unkStructArrLen];

    u32 randomAlways1Int;
    padding[0x1A];
    padding[unkStructArrLen * 0x10];
    padding[0x8];

    u32 meshInfoCount;
    meshInfo meshInfos[meshInfoCount];

    u32 unkWordCount;
    padding[unkWordCount * 2];

    u32 morphArrayLen;
    morphNameEntry morphNameArr[morphArrayLen];

    u32 boneMapCount;
    boneMap boneMaps[boneMapCount];

    u32 morphStructArrCount;
    morphStruct morphStructArray[morphStructArrCount];

    padding[0x37];

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
Model269 model @ $;