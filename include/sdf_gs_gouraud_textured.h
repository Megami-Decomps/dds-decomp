#ifndef SDF_GS_GOURAUD_TEXTURED_H
#define SDF_GS_GOURAUD_TEXTURED_H

#include "common.h"

/* Each GIF REGLIST vertex carries UV, its own RGBAQ, and packed XYZ2. */
typedef struct SdfGsGouraudTexturedVertex {
    u64 uv;
    u64 rgbaq;
    u64 xyz2;
} SdfGsGouraudTexturedVertex;

typedef char SdfGsGouraudTexturedVertex_size_must_be_0x18[
    (sizeof(SdfGsGouraudTexturedVertex) == 0x18) ? 1 : -1];
typedef char SdfGsGouraudTexturedVertex_rgbaq_at_0x08[
    ((u32)&((SdfGsGouraudTexturedVertex *)0)->rgbaq == 0x08) ? 1 : -1];
typedef char SdfGsGouraudTexturedVertex_xyz2_at_0x10[
    ((u32)&((SdfGsGouraudTexturedVertex *)0)->xyz2 == 0x10) ? 1 : -1];

/* PRIM followed by 3 individually textured and colored vertices. */
typedef struct SdfGsGouraudTexturedTrianglePayload {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    SdfGsGouraudTexturedVertex vertices[3];
} SdfGsGouraudTexturedTrianglePayload;

typedef struct SdfGsGouraudTexturedTrianglePacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsGouraudTexturedTrianglePayload drawing;
} SdfGsGouraudTexturedTrianglePacket;

typedef char SdfGsGouraudTexturedTrianglePayload_size_must_be_0x60[
    (sizeof(SdfGsGouraudTexturedTrianglePayload) == 0x60) ? 1 : -1];
typedef char SdfGsGouraudTexturedTrianglePayload_primitive_at_0x10[
    ((u32)&((SdfGsGouraudTexturedTrianglePayload *)0)->primitive == 0x10) ? 1 : -1];
typedef char SdfGsGouraudTexturedTrianglePayload_vertices_at_0x18[
    ((u32)&((SdfGsGouraudTexturedTrianglePayload *)0)->vertices == 0x18) ? 1 : -1];
typedef char SdfGsGouraudTexturedTrianglePacket_size_must_be_0x70[
    (sizeof(SdfGsGouraudTexturedTrianglePacket) == 0x70) ? 1 : -1];
typedef char SdfGsGouraudTexturedTrianglePacket_drawing_at_0x10[
    ((u32)&((SdfGsGouraudTexturedTrianglePacket *)0)->drawing == 0x10) ? 1 : -1];

void sdfWriteGouraudTexturedTrianglePacket(SdfGsGouraudTexturedTrianglePayload *packet, s32 primitive,
    s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
    s32 x1, s32 y1, s32 u1, s32 v1, s32 color1,
    s32 x2, s32 y2, s32 u2, s32 v2, s32 color2, s32 depth);

#endif
