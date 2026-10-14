#ifndef KWLN_DRAW_PARAMS_H
#define KWLN_DRAW_PARAMS_H

#include "common.h"

/* Pending/saved renderer parameter bodies shared by both title implementations.
 * These exclude each effect owner's resource handle and allocation fields. */
typedef union KwlnColor {
    u32 rgba;
    u8 channels[4];
} KwlnColor;

typedef struct KwlnRectBounds {
    u32 left;
    u32 top;
    u32 right;
    u32 bottom;
} KwlnRectBounds;

typedef struct KwlnSolidRectParams {
    KwlnColor color;
    u32 blendControl;
    KwlnRectBounds bounds;
} KwlnSolidRectParams;

typedef struct KwlnResourceRectParams {
    u32 extent;
    u32 centerX;
    u32 centerY;
    KwlnColor color;
    u32 blendControl;
    KwlnRectBounds bounds;
} KwlnResourceRectParams;

typedef struct KwlnBlurRectParams {
    KwlnColor color;
    u32 blendControl;
    f32 rotation;
    f32 scale;
    u32 centerX;
    u32 centerY;
    KwlnRectBounds bounds;
} KwlnBlurRectParams;

typedef struct KwlnPixelBlurParams {
    u32 extent;
    KwlnBlurRectParams source;
} KwlnPixelBlurParams;

/* Staggered (ripple) blur parameter body copied at eight-byte alignment. */
typedef struct KwlnScaleBlurParams {
    u32 count;
    f32 phaseStep;
    f32 spacing;
    KwlnColor color;
    u32 unk10;
    f32 unk14;
    f32 unk18;
    f32 angleStep;
    u32 x;
    u32 y;
    u32 size;
} KwlnScaleBlurParams __attribute__((aligned(8)));

typedef char KwlnColor_size_must_be_4[sizeof(KwlnColor) == 0x04 ? 1 : -1];
typedef char KwlnRectBounds_size_must_be_0x10[sizeof(KwlnRectBounds) == 0x10 ? 1 : -1];
typedef char KwlnSolidRectParams_size_must_be_0x18[sizeof(KwlnSolidRectParams) == 0x18 ? 1 : -1];
typedef char KwlnResourceRectParams_size_must_be_0x24[sizeof(KwlnResourceRectParams) == 0x24 ? 1 : -1];
typedef char KwlnBlurRectParams_size_must_be_0x28[sizeof(KwlnBlurRectParams) == 0x28 ? 1 : -1];
typedef char KwlnPixelBlurParams_size_must_be_0x2C[sizeof(KwlnPixelBlurParams) == 0x2C ? 1 : -1];
typedef char KwlnScaleBlurParams_size_must_be_0x2C[sizeof(KwlnScaleBlurParams) == 0x2C ? 1 : -1];

#endif
