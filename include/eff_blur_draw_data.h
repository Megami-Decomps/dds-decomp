#ifndef EFF_BLUR_DRAW_DATA_H
#define EFF_BLUR_DRAW_DATA_H

#include "common.h"

typedef struct EffBlurDrawVertex {
    f32 u;
    f32 v;
    u8 opaque08[8];
    s32 x;
    s32 y;
    u32 opaque18;
    u16 opaque1C;
    u16 opaque1E;
} EffBlurDrawVertex;

typedef struct EffBlurDrawData {
    u32 color[4];
    EffBlurDrawVertex vertices[4];
} EffBlurDrawData;

typedef char EffBlurDrawVertexSizeAssert[
    (sizeof(EffBlurDrawVertex) == 0x20) ? 1 : -1];
typedef char EffBlurDrawVertexLayoutAssert[
    ((u32)&((EffBlurDrawVertex *)0)->x == 0x10 &&
     (u32)&((EffBlurDrawVertex *)0)->y == 0x14) ? 1 : -1];
typedef char EffBlurDrawDataSizeAssert[
    (sizeof(EffBlurDrawData) == 0x90) ? 1 : -1];
typedef char EffBlurDrawDataLayoutAssert[
    ((u32)&((EffBlurDrawData *)0)->vertices == 0x10) ? 1 : -1];

#endif
