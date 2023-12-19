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

struct PhysicsPath {
    u32 type;
    u32 subType;
    if (type == 0)
	{
		if (subType == 0 || subType == 4) {
		   u32 pathLen;
	       padding[pathLen];
		}
	}
	else if (type == 1)
	{
		if (subType == 0 || subType == 1)
		{
			u32 pathLen;
			padding[pathLen];
		}
		else if (subType == 8)
		{
			padding[2];
			u32 pathLen;
			padding[pathLen];
		}
	}
	else if (type == 2) {
	   if (subType == 5) {
	       u32 pathLen;
		   padding[pathLen];
	   }
	}
};

/****************************************************************/
// Main struct

struct Model102 {
    padding[0x1C];
    u32 LODs[5];

    PhysicsPath physPath1;

    padding[0x18];
    float randomFloatTest;

    padding[0x12];
    vec4Float vec1;
    vec4Float vec2;

    u32 vertexCount;
    u32 faceCount;
    u32 morphCount;
    u32 boneCount;

    padding[0x18];

    u32 skipArrayCount2;
    padding[8 * skipArrayCount2];

    padding[0x51];

    u32 meshInfoCount;
    meshInfo meshInfos[meshInfoCount];

    u32 idkSomeCheck;

    if (idkSomeCheck == 2) {
        padding[4];
    } else if (idkSomeCheck == 3) {
        padding[6];
    }

    // HLOD is 4, DCM is 5
    padding[0x4];

    PhysicsPath physPath2;

    u32 physXArrLen;
    meshPhysX physXArray[physXArrLen];
};

/****************************************************************/
// File definition

CommonHeader Header @ $;
Model102 model @ $;
