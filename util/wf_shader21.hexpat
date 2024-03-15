#include <wf_common-header.hexpat>

struct Shader21 {
    padding[8];
    str32 shaderPath;
    u32 unk1;
    padding[14];
    u32 shaderCount;
    u16 skips[8];
    padding[6];
    u32 shaderCodeTotalSize;
    u32 unks[7];
    u32 shaderLengthCount;
    u32 shaderLength[shaderLengthCount];
};

CommonHeader ch @ $;
Shader21 shader @ $;
