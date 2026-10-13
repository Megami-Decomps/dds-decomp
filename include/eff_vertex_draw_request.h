#ifndef EFF_VERTEX_DRAW_REQUEST_H
#define EFF_VERTEX_DRAW_REQUEST_H

#include "common.h"

/* Shared particle and polygon-track view passed to effCreateVertexDrawPacket. */
typedef struct EffVertexDrawRequest {
    u16 width;
    u16 height;
    u16 flags;
    u8 pad06[2];
    u32 color;
    const u32 *indices;
    u128 *positions;
    u8 opaque14[0xC];
    u32 *colors;
    u8 opaque24[8];
} EffVertexDrawRequest;

typedef char EffVertexDrawRequestSizeAssert[
    (sizeof(EffVertexDrawRequest) == 0x2C) ? 1 : -1];
typedef char EffVertexDrawRequestOffsetsAssert[
    ((u32)&((EffVertexDrawRequest *)0)->width == 0 &&
     (u32)&((EffVertexDrawRequest *)0)->height == 2 &&
     (u32)&((EffVertexDrawRequest *)0)->flags == 4 &&
     (u32)&((EffVertexDrawRequest *)0)->color == 8 &&
     (u32)&((EffVertexDrawRequest *)0)->indices == 0xC &&
     (u32)&((EffVertexDrawRequest *)0)->positions == 0x10 &&
     (u32)&((EffVertexDrawRequest *)0)->colors == 0x20 &&
     (u32)&((EffVertexDrawRequest *)0)->opaque24 == 0x24) ? 1 : -1];

#endif
