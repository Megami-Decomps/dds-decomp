#ifndef SDF_GS_GEOMETRY_H
#define SDF_GS_GEOMETRY_H

#include "common.h"

/* GIF REGLIST stream: PRIM, RGBAQ and two packed XYZ2 vertices. */
typedef struct SdfGsTwoVertexPayload {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[2];
} SdfGsTwoVertexPayload;

typedef char SdfGsTwoVertexPayload_size_must_be_0x30[
    (sizeof(SdfGsTwoVertexPayload) == 0x30) ? 1 : -1];
typedef char SdfGsTwoVertexPayload_xyz2_at_0x20[
    ((u32)&((SdfGsTwoVertexPayload *)0)->xyz2 == 0x20) ? 1 : -1];

/* DMA/VIF header followed by the complete two-vertex GIF payload. */
typedef struct SdfGsTwoVertexPacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsTwoVertexPayload drawing;
} SdfGsTwoVertexPacket;

typedef char SdfGsTwoVertexPacket_size_must_be_0x40[
    (sizeof(SdfGsTwoVertexPacket) == 0x40) ? 1 : -1];
typedef char SdfGsTwoVertexPacket_drawing_at_0x10[
    ((u32)&((SdfGsTwoVertexPacket *)0)->drawing == 0x10) ? 1 : -1];

void sdfBuildFillPacket101(SdfGsTwoVertexPayload *packet, s32 color, s32 primitive,
    s32 left, s32 top, s32 right, s32 bottom, s32 depth);
void sdfBuildFillPacket106(SdfGsTwoVertexPayload *packet, s32 color, s32 primitive,
    s32 left, s32 top, s32 right, s32 bottom, s32 depth);

/* GIF REGLIST stream: PRIM, RGBAQ, then UV/XYZ2 for each vertex. */
typedef struct SdfGsTexturedVertex {
    u64 uv;
    u64 xyz2;
} SdfGsTexturedVertex;

typedef struct SdfGsTexturedPairPayload {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq;
    SdfGsTexturedVertex vertices[2];
} SdfGsTexturedPairPayload;

typedef struct SdfGsTexturedPairPacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsTexturedPairPayload drawing;
} SdfGsTexturedPairPacket;

typedef char SdfGsTexturedVertex_size_must_be_0x10[
    (sizeof(SdfGsTexturedVertex) == 0x10) ? 1 : -1];
typedef char SdfGsTexturedPairPayload_size_must_be_0x40[
    (sizeof(SdfGsTexturedPairPayload) == 0x40) ? 1 : -1];
typedef char SdfGsTexturedPairPacket_size_must_be_0x50[
    (sizeof(SdfGsTexturedPairPacket) == 0x50) ? 1 : -1];
typedef char SdfGsTexturedPairPacket_drawing_at_0x10[
    ((u32)&((SdfGsTexturedPairPacket *)0)->drawing == 0x10) ? 1 : -1];

void sdfBuildPacket116(SdfGsTexturedPairPayload *packet, s32 color,
    s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
    s32 x1, s32 y1, s32 u1, s32 v1, s32 depth);

#endif
