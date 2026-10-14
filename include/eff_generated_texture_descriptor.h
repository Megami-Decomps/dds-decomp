#ifndef EFF_GENERATED_TEXTURE_DESCRIPTOR_H
#define EFF_GENERATED_TEXTURE_DESCRIPTOR_H

#include "common.h"

struct SdfTex;

/* Producer-built input shared with the generated-texture packet providers.
 * Bytes 0x18..0x1F remain opaque; the provider does not read them. */
typedef struct EffGeneratedTextureDescriptor {
    f32 position[4];
    f32 scaleX;
    f32 scaleY;
    u8 opaque18[8];
    f32 angle;
    u32 color;
    struct SdfTex *texture;
} EffGeneratedTextureDescriptor;

typedef char EffGeneratedTextureDescriptorSizeAssert[
    (sizeof(EffGeneratedTextureDescriptor) == 0x2C) ? 1 : -1];
typedef char EffGeneratedTextureDescriptorLayoutAssert[
    ((u32)&((EffGeneratedTextureDescriptor *)0)->position == 0x00 &&
     (u32)&((EffGeneratedTextureDescriptor *)0)->scaleX == 0x10 &&
     (u32)&((EffGeneratedTextureDescriptor *)0)->scaleY == 0x14 &&
     (u32)&((EffGeneratedTextureDescriptor *)0)->opaque18 == 0x18 &&
     (u32)&((EffGeneratedTextureDescriptor *)0)->angle == 0x20 &&
     (u32)&((EffGeneratedTextureDescriptor *)0)->color == 0x24 &&
     (u32)&((EffGeneratedTextureDescriptor *)0)->texture == 0x28) ? 1 : -1];

#endif
