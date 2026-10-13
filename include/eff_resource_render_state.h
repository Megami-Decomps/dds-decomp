#ifndef EFF_RESOURCE_RENDER_STATE_H
#define EFF_RESOURCE_RENDER_STATE_H

#include "common.h"

typedef struct EffResourceRenderState {
    u16 parameterCount;
    u16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 color;
    void *parameters;
    f32 (*positions)[4];
    f32 (*normals)[4];
    u8 opaque18[8];
    u32 *colors;
    u8 opaque24[8];
} EffResourceRenderState;

typedef char EffResourceRenderStateSizeAssert[
    (sizeof(EffResourceRenderState) == 0x2C) ? 1 : -1];
typedef char EffResourceRenderStateLayoutAssert[
    ((u32)&((EffResourceRenderState *)0)->color == 8 &&
     (u32)&((EffResourceRenderState *)0)->parameters == 0xC &&
     (u32)&((EffResourceRenderState *)0)->positions == 0x10 &&
     (u32)&((EffResourceRenderState *)0)->normals == 0x14 &&
     (u32)&((EffResourceRenderState *)0)->colors == 0x20) ? 1 : -1];

#endif
