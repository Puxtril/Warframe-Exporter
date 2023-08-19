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