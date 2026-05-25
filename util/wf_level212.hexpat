#include <wf_common-header.hexpat>

struct LevelObj {
    str32 path;
    vec3Float pos;
    vec4Float rot;
    str32 path2;
    u32 instanceID;
    u32 unk2;
};

struct TLevelObj {
    str32 unkStr;
    str32 tlevelPath;
    padding[8];
};

struct Level212 {
    str32Arr32 paths;
    str32Arr32 actionPaths;
    str32Arr32 assetPaths;
    u32 tlevelCount;
    TLevelObj tlevels[tlevelCount];
    padding[4];
    u32 levelObjCount;
    LevelObj objs[levelObjCount];
    u32 unkIntCount;
    padding[4 * unkIntCount];
    u32 finalInt;
};

CommonHeader ch @ $;
Level212 level @ $;