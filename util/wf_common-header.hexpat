#pragma once

#include <wf_utils.hexpat>

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