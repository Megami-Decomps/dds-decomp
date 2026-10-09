#ifndef SDF_GS_TEXTURED_SHAPES_H
#define SDF_GS_TEXTURED_SHAPES_H

#include "sdf_gs_geometry.h"

/* GIF REGLIST stream: PRIM, one RGBAQ, then UV/XYZ2 for each vertex. */
typedef struct SdfGsTexturedTrianglePayload {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq;
    SdfGsTexturedVertex vertices[3];
} SdfGsTexturedTrianglePayload;

typedef struct SdfGsTexturedTrianglePacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsTexturedTrianglePayload drawing;
} SdfGsTexturedTrianglePacket;

typedef char SdfGsTexturedTrianglePayload_size_must_be_0x50[
    (sizeof(SdfGsTexturedTrianglePayload) == 0x50) ? 1 : -1];
typedef char SdfGsTexturedTrianglePayload_primitive_at_0x10[
    ((u32)&((SdfGsTexturedTrianglePayload *)0)->primitive == 0x10) ? 1 : -1];
typedef char SdfGsTexturedTrianglePayload_rgbaq_at_0x18[
    ((u32)&((SdfGsTexturedTrianglePayload *)0)->rgbaq == 0x18) ? 1 : -1];
typedef char SdfGsTexturedTrianglePayload_vertices_at_0x20[
    ((u32)&((SdfGsTexturedTrianglePayload *)0)->vertices == 0x20) ? 1 : -1];
typedef char SdfGsTexturedTrianglePacket_size_must_be_0x60[
    (sizeof(SdfGsTexturedTrianglePacket) == 0x60) ? 1 : -1];
typedef char SdfGsTexturedTrianglePacket_drawing_at_0x10[
    ((u32)&((SdfGsTexturedTrianglePacket *)0)->drawing == 0x10) ? 1 : -1];

void sdfBuildPacket114(SdfGsTexturedTrianglePayload *packet, s32 color, s32 primitive,
    s32 x0, s32 y0, s32 u0, s32 v0,
    s32 x1, s32 y1, s32 u1, s32 v1,
    s32 x2, s32 y2, s32 u2, s32 v2, s32 depth);

/* GIF REGLIST stream: PRIM, one RGBAQ, then UV/XYZ2 for each vertex. */
typedef struct SdfGsTexturedQuadPayload {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq;
    SdfGsTexturedVertex vertices[4];
} SdfGsTexturedQuadPayload;

typedef struct SdfGsTexturedQuadPacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsTexturedQuadPayload drawing;
} SdfGsTexturedQuadPacket;

typedef char SdfGsTexturedQuadPayload_size_must_be_0x60[
    (sizeof(SdfGsTexturedQuadPayload) == 0x60) ? 1 : -1];
typedef char SdfGsTexturedQuadPayload_primitive_at_0x10[
    ((u32)&((SdfGsTexturedQuadPayload *)0)->primitive == 0x10) ? 1 : -1];
typedef char SdfGsTexturedQuadPayload_rgbaq_at_0x18[
    ((u32)&((SdfGsTexturedQuadPayload *)0)->rgbaq == 0x18) ? 1 : -1];
typedef char SdfGsTexturedQuadPayload_vertices_at_0x20[
    ((u32)&((SdfGsTexturedQuadPayload *)0)->vertices == 0x20) ? 1 : -1];
typedef char SdfGsTexturedQuadPacket_size_must_be_0x70[
    (sizeof(SdfGsTexturedQuadPacket) == 0x70) ? 1 : -1];
typedef char SdfGsTexturedQuadPacket_drawing_at_0x10[
    ((u32)&((SdfGsTexturedQuadPacket *)0)->drawing == 0x10) ? 1 : -1];

void sdfWriteTexturedQuadPacket(SdfGsTexturedQuadPayload *packet, s32 color, s32 primitive,
    s32 x0, s32 y0, s32 u0, s32 v0,
    s32 x1, s32 y1, s32 u1, s32 v1,
    s32 x2, s32 y2, s32 u2, s32 v2,
    s32 x3, s32 y3, s32 u3, s32 v3, s32 depth);

#endif
