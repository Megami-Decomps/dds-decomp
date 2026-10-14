#ifndef EFF_SCATTER_DRAW_REQUEST_H
#define EFF_SCATTER_DRAW_REQUEST_H

#include "common.h"

/* Scatter renderer's 0x2C request view consumed by effCreateVertexDrawPacket. */
typedef struct EffScatterDrawRequest {
    u16 primitiveCount;
    u16 vertexCount;
    u16 flags;
    u8 opaque06[2];
    u32 color;
    void *parameters;
    f32 *points;
    u8 opaque14[4];
    f32 *uv;
    u8 opaque1C[4];
    u32 *colors;
    u8 opaque24[8];
} EffScatterDrawRequest;

typedef char EffScatterDrawRequest_size_must_be_0x2C[
    sizeof(EffScatterDrawRequest) == 0x2C ? 1 : -1];
typedef char EffScatterDrawRequest_offsets_must_match[
    ((u32)&((EffScatterDrawRequest *)0)->parameters == 0x0C &&
     (u32)&((EffScatterDrawRequest *)0)->points == 0x10 &&
     (u32)&((EffScatterDrawRequest *)0)->uv == 0x18 &&
     (u32)&((EffScatterDrawRequest *)0)->colors == 0x20) ? 1 : -1];

#endif
