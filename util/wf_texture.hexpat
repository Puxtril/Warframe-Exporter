#pragma once

#include <wf_utils.hexpat>
#include <wf_common-header.hexpat>

/****************************************************************/
// Main struct

struct Texture {
    u8 unks[3];
    u8 format;
    u32 unkArrCount;
    padding[unkArrCount * 4];
    u16 width;
    u16 height;
};

/****************************************************************/
// File definition

CommonHeader Header @ $;
Texture texture @ $;