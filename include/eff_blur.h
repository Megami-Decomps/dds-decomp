#ifndef EFF_BLUR_H
#define EFF_BLUR_H

#include "common.h"

/* Shared rectangle and sampling payload; color is packed or read by channel. */
typedef struct EffBlurQuad {
    u32 color; /* Byte consumers read the packed word through u8 object access. */
    s32 blendControl;
    f32 angle;
    f32 displacement;
    s32 x;
    s32 y;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} EffBlurQuad;

typedef char EffBlurQuadSizeCheck[sizeof(EffBlurQuad) == 0x28 ? 1 : -1];
typedef char EffBlurQuadCenterOffsetCheck[((u32)&((EffBlurQuad *)0)->x == 0x10) ? 1 : -1];
typedef char EffBlurQuadEdgesOffsetCheck[((u32)&((EffBlurQuad *)0)->left == 0x18) ? 1 : -1];

void effAppendBlurRectanglePackets(void *list, EffBlurQuad *source, u8 fixedPointCoordinates);
void effDrawBlurSource(EffBlurQuad *source, s32 resource, u8 fixedPointCoordinates);

#endif
