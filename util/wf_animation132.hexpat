#include <wf_common-header.hexpat>
#include <wf_utils.hexpat>

struct boneTreeNode {
    str32 name;
    u16 parentID;
    u16 id;
};

struct action {
    padding[0x10];
    str32 name;
    padding[0x44];
    u32 strideLen;
    u32 offset;
};

struct action2 {
    padding[12];
    u32 frameCount;
};

struct AnimationSkeleton {
    str32 skeletonName;
    
    u32 unk [[hidden]];
    if (unk > 0) {
        padding[1];
    }
    
    vec4Float vec1;
    vec4Float vec2;
    
    padding[0x4];
    
    u32 boneTreeCount;
    boneTreeNode boneTree[boneTreeCount];
    
    u32 actionCount;
    action actions[actionCount];
    
    padding[0x1A];
    u32 frameDataLen;
    
    action2 actionsMisc[actionCount];
    
    padding[0x8];
    
};

struct Animation132 {
    str32 somePath1;
    str32 somePath2;
    
    padding[0x2];
    
    vec3Float importScaleMaybe;
    
    padding[0x4C];
    
    u32 skeletonCount;
    AnimationSkeleton skeletons[skeletonCount];
    
    str32Arr32 errors;
};

CommonHeader ch @ $;
Animation132 animation @ $;