#include <wf_common-header.hexpat>

struct ShaderEntry {
    str32 shader;
    u8 n;
};

struct ShaderEntry2 {
    str32 shader;
    u32 index;
};

struct ShaderEntry3 {
    str32 shader;
};

struct Material208 {
    str32Arr32 attributes;
    ShaderEntry shaders[16];
    padding[3];
    ShaderEntry3 singleShader[29];
};

CommonHeader ch @ $;
Material208 material @ $;