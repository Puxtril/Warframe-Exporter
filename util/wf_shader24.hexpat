#include <wf_common-header.hexpat>

fn fmt_name(auto var) {
	return var.name;
};

struct AttributePackaged {
    str32 name;
    str32Arr32 attributes;
    u8 skip;
} [[format("fmt_name")]];

struct AttributeIdk {
    padding[6 * 4];
};

struct AttributePadded {
    str32 name;
    u32 idkLen;
    AttributeIdk idks[idkLen];
} [[format("fmt_name")]];

struct Attribute2 {
    str32 name;
    padding[24];
} [[format("fmt_name")]];

struct ShaderAttribute {
    padding[32];
    u16 seemsImportant1;
    padding[7];
    u16 seemsImportant2;
    padding[5];
    str32 help;
    padding[12];
    str32 untintable;
    padding[34];
    str32 name;
};

struct LightmapEntry {
    padding[33];
    str32 material1;
    str32 material2;
    padding[2];
    str32 name;
} [[format("fmt_name")]];

struct ShaderMaster {
    u32 shaderAttributeLen;
    ShaderAttribute shaderAttributes[shaderAttributeLen];
    u32 LightmapEntryLen;
    LightmapEntry lightmapEntries[LightmapEntryLen];
};

// Very similar to Shader21
struct ShaderBytecodes {
    padding[59];
    u32 shaderCodeTotalSize;
    padding[12];
    u32 shaderCount;
    u32 shaderBytecodeLengths[shaderCount];
};

struct Shader24 {
    padding[22];

    str32Arr32 attArrs[3];

    u32 attributePacakgedCount;
    AttributePackaged attributePacked[attributePacakgedCount];

    u32 attributePacakgedCount1;
    AttributePackaged attributePacked1[attributePacakgedCount1];

    padding[8];

    u32 skip24Len;
    padding[24 * skip24Len];

    u32 attPadLen;
    AttributePadded att6[attPadLen];

    u32 arrcount [[hidden]];
    str32Arr32 Att9999[arrcount * 2];

    u32 Att2Len [[hidden]];
    Attribute2 att7[Att2Len];

    ShaderMaster masters[4];

    s32 shaderBytecodeCheck;
    if (shaderBytecodeCheck != -1) {
        ShaderBytecodes bytecodes;
    }
};

CommonHeader ch @ $;
Shader24 shader @ $;
