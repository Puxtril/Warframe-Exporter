#pragma once

#include <std/string.pat>
#include <std/io.pat>

using str32;
fn fmt_str32(str32 var) {
    return var.s;
};
struct str32 {
    u32 strLen [[hidden]];
    char s[strLen];
} [[format("fmt_str32")]];

struct str32Arr32 {
    u32 strCount;
    str32 strs[strCount];
};

using vec2Float;
fn fmt_vec2Float(vec2Float var) {
    return std::format("({:.3f}, {:.3f})", var.x, var.y);
};
struct vec2Float {
    float x;
    float y;
};

using vec3Float;
fn fmt_vec3Float(vec3Float var) {
    return std::format("({:.3f}, {:.3f}, {:.3f})", var.x, var.y, var.z);
};
struct vec3Float {
    float x;
    float y;
    float z;
} [[format("fmt_vec3Float")]];

using vec4Float;
fn fmt_vec4Float(vec4Float var) {
    return std::format("({:.3f}, {:.3f}, {:.3f}, {:.3f})", var.x, var.y, var.z, var.w);
};
struct vec4Float {
    float x;
    float y;
    float z;
    float w;
} [[format("fmt_vec4Float")]];

struct CommonHeader {
    char hash[16];
    str32Arr32 entries;
    u32 propsLen;
    char propsStr[propsLen];
    if (propsLen != 0) {
        char;
    }
    u32 fileTypeEnum;
};

fn fmt_name(auto var) {
    return var.name;
};

fn fmt_attr(auto var) {
    return var.name.s + "=" + std::string::to_string(var.seemsImportant2);
};

fn fmt_lightmap(auto var) {
    return var.name.s + "=" + std::string::to_string(var.shaderIndex);
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
} [[format("fmt_attr")]];

struct LightmapEntry {
    padding[28];
    u16 shaderIndex;
    padding[3];
    str32 material1;
    str32 material2;
    padding[2];
    str32 name;
} [[format("fmt_lightmap")]];

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

struct Shader32 {
    padding[30];

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
    
    // Seems like an array length....
    padding[4];

    ShaderMaster masters[4];

    s32 shaderBytecodeCheck;
    if (shaderBytecodeCheck != -1) {
        ShaderBytecodes bytecodes;
    }
};

CommonHeader ch @ $;
Shader32 shader @ $;
