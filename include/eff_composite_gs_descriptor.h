#ifndef EFF_COMPOSITE_GS_DESCRIPTOR_H
#define EFF_COMPOSITE_GS_DESCRIPTOR_H

#include "common.h"
#include "eff.h"
#include "sdf.h"

/* Caller-built composite draw description; 0x68 bytes. */
typedef struct EffCompositeGsDescriptor {
    f32 position[4];
    f32 scaleX;
    f32 scaleY;
    u8 unk18[8];
    f32 angle;
    u32 color;
    u32 blendMode;
    BillTextureQuad primaryUv;
    BillTextureQuad secondaryUv;
    SdfTex *primaryTexture;
    u64 primaryClamp;
    SdfTex *secondaryTexture;
    u8 unk5C[4];
    u64 secondaryClamp;
} EffCompositeGsDescriptor;

typedef char EffCompositeGsDescriptor_size_must_be_0x68[
    (sizeof(EffCompositeGsDescriptor) == 0x68) ? 1 : -1];

#endif
