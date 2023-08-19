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
    padding[0x20];
    str32 unkName;
    float vec3[4];
};

struct unkStruct {
   u32 arrCount [[hidden]];
   u32 varArr[arrCount] [[color("eb4034")]];
   u32 sequenceIndices[4] [[color("3f98c4")]];
};

struct meshPhysX {
    u32 meshType;
    if (meshType == 1) {
        padding[0x4C];
    } else if (meshType == 7) {
        padding[0x60];
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

struct Model159 {
    str32 path;
    str32 path2;
    
    padding[0x8];
    
    float unkFloats[6];
    
    u64 faceCount;

    u32 meshInfoCount;
    meshInfo meshInfos[meshInfoCount];
    
    u32 vertexCount;
    padding[16];
    
    u32 bodyUnkStructCount;
    vec4Float vec1;
    vec4Float vec2;
    
    u32 unkCount;
    unkStruct unks[unkCount];
    
    padding[12];
    
    u32 physXCount;
    meshPhysX physXMeshes[physXCount];
};

/****************************************************************/
// File definition

CommonHeader Header @ $;
Model159 model @ $;