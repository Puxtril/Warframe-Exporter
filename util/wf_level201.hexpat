#include <wf_common-header.hexpat>

struct LevelObj {
    str32 path;
    vec3Float pos;
    vec4Float rot;
    str32 path2;
    u32 instanceID;
    u32 unk2;
};

struct Level201 {
    str32Arr32 paths;
    str32Arr32 actionPaths;
    str32Arr32 assetPaths;
    padding[4];
    u32 levelObjCount;
    LevelObj objs[levelObjCount];
    u32 unkIntCount;
    padding[4 * unkIntCount];
    u32 finalInt;
};

CommonHeader ch @ $;
Level201 level @ $;