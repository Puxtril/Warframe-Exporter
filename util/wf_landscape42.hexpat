#include <wf_common-header.hexpat>

struct LandscapeChunk {
    padding[0x3C];
    float scale[3];
    padding[0x44];
    u32 bodyLen;
    u32 unk4;
    u32 unk5;
};

struct Landscape42 {
    u32 pathArrCount;
    u32 unks[2];
    u32 rowCount;
    u32 columnCount;
    padding[2];

    u32 len1;
    u32 len2;
    u32 len3;

    padding[0xB];

    str32Arr32 pathArrs[9];

    u32 byteArrayCount;
    u8 bytes[byteArrayCount];

    padding[0x14];

    u32 chunkCount;
    LandscapeChunk chunks[chunkCount];
};

CommonHeader ch @ $;
Landscape42 landscape @ $;
