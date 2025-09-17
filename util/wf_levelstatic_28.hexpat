#pragma once

#include <wf_utils.hexpat>
#include <wf_common-header.hexpat>
#include <wf_models.hexpat>

struct IndexGroup {
    u32 indexcount1;
    u32 indexcount2;
};

struct LevelStatic28 {
    str32Arr32 unkpaths;
    u32 always0;
    u32 vertexCount;
    
    u32 indexGroupCount;
    IndexGroup indexGroups[indexGroupCount];

    str32Arr32 modelPaths;
    str32Arr32 unkPaths2;
    str32Arr32 textureNames1;
    str32Arr32 textureNames2;
    str32Arr32 naturalPaths;
    str32Arr32 attributes;
    str32Arr32 materials;
    
    u32 indices2Count;
    u32 unk1Count;
    u32 unk2Count;
    u32 emptySpace;
    u32 matrixCount;
    u32 vertexCount2;
    u32 emptySpace2;
};

CommonHeader Header @ $;
LevelStatic28 levelstatic @ $;
