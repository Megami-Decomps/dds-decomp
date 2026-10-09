#ifndef SDF_GS_GOURAUD_SHAPES_H
#define SDF_GS_GOURAUD_SHAPES_H

#include "common.h"

/* GIF REGLIST vertices carry their own RGBAQ and a packed XYZ2 position. */
typedef struct SdfGsGouraudVertex {
    u64 rgbaq;
    u64 xyz2;
} SdfGsGouraudVertex;

typedef char SdfGsGouraudVertex_size_must_be_0x10[
    (sizeof(SdfGsGouraudVertex) == 0x10) ? 1 : -1];
typedef char SdfGsGouraudVertex_xyz2_at_0x08[
    ((u32)&((SdfGsGouraudVertex *)0)->xyz2 == 0x08) ? 1 : -1];

/* PRIM followed by three independently colored vertices. */
typedef struct SdfGsGouraudTrianglePayload {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    SdfGsGouraudVertex vertices[3];
} SdfGsGouraudTrianglePayload;

typedef struct SdfGsGouraudTrianglePacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsGouraudTrianglePayload drawing;
    u64 unwrittenTail; /* Allocation rounds the REGLIST payload up to a qword. */
} SdfGsGouraudTrianglePacket;

typedef char SdfGsGouraudTrianglePayload_size_must_be_0x48[
    (sizeof(SdfGsGouraudTrianglePayload) == 0x48) ? 1 : -1];
typedef char SdfGsGouraudTrianglePayload_primitive_at_0x10[
    ((u32)&((SdfGsGouraudTrianglePayload *)0)->primitive == 0x10) ? 1 : -1];
typedef char SdfGsGouraudTrianglePayload_vertices_at_0x18[
    ((u32)&((SdfGsGouraudTrianglePayload *)0)->vertices == 0x18) ? 1 : -1];
typedef char SdfGsGouraudTrianglePacket_size_must_be_0x60[
    (sizeof(SdfGsGouraudTrianglePacket) == 0x60) ? 1 : -1];
typedef char SdfGsGouraudTrianglePacket_drawing_at_0x10[
    ((u32)&((SdfGsGouraudTrianglePacket *)0)->drawing == 0x10) ? 1 : -1];
typedef char SdfGsGouraudTrianglePacket_unwrittenTail_at_0x58[
    ((u32)&((SdfGsGouraudTrianglePacket *)0)->unwrittenTail == 0x58) ? 1 : -1];

void sdfBuildPacket10C(SdfGsGouraudTrianglePayload *packet, s32 primitive,
    s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1,
    s32 x2, s32 y2, s32 color2, s32 depth);

#endif
