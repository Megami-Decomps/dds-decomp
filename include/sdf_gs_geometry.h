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

void sdfBuildFillPacket101(SdfGsTwoVertexPayload *packet, s32 color, s32 primitive,
    s32 left, s32 top, s32 right, s32 bottom, s32 depth);
void sdfBuildFillPacket106(SdfGsTwoVertexPayload *packet, s32 color, s32 primitive,
    s32 left, s32 top, s32 right, s32 bottom, s32 depth);

#endif
