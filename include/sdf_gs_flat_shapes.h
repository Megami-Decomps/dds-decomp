#ifndef SDF_GS_FLAT_SHAPES_H
#define SDF_GS_FLAT_SHAPES_H

#include "common.h"

/* GIF stream for one flat-color triangle: tag/register list, PRIM, RGBAQ,
 * and the three XYZ2 values. */
typedef struct SdfGsFlatTrianglePayload {
    u64 gifTag;
    u64 gifRegisterList;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[3];
} SdfGsFlatTrianglePayload;

/* The caller prepends DMA/VIF words to the triangle GIF stream. Allocation
 * reserves 0x50 bytes; the final qword is not written by these helpers. */
typedef struct SdfGsFlatTrianglePacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsFlatTrianglePayload drawing;
    u64 unwrittenAllocationWord;
} SdfGsFlatTrianglePacket;

typedef char SdfGsFlatTrianglePayload_size_must_be_0x38[
    (sizeof(SdfGsFlatTrianglePayload) == 0x38) ? 1 : -1];
typedef char SdfGsFlatTrianglePayload_primitive_at_0x10[
    ((u32)&((SdfGsFlatTrianglePayload *)0)->primitive == 0x10) ? 1 : -1];
typedef char SdfGsFlatTrianglePayload_rgbaq_at_0x18[
    ((u32)&((SdfGsFlatTrianglePayload *)0)->rgbaq == 0x18) ? 1 : -1];
typedef char SdfGsFlatTrianglePayload_xyz2_at_0x20[
    ((u32)&((SdfGsFlatTrianglePayload *)0)->xyz2 == 0x20) ? 1 : -1];
typedef char SdfGsFlatTrianglePacket_size_must_be_0x50[
    (sizeof(SdfGsFlatTrianglePacket) == 0x50) ? 1 : -1];
typedef char SdfGsFlatTrianglePacket_drawing_at_0x10[
    ((u32)&((SdfGsFlatTrianglePacket *)0)->drawing == 0x10) ? 1 : -1];
typedef char SdfGsFlatTrianglePacket_unwritten_word_at_0x48[
    ((u32)&((SdfGsFlatTrianglePacket *)0)->unwrittenAllocationWord == 0x48) ? 1 : -1];

void sdfBuildTriPacket104(SdfGsFlatTrianglePayload *dst, s32 color,
    s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2,
    s32 depth);

/* GIF stream for four flat-color vertices: PRIM, RGBAQ and four XYZ2 values. */
typedef struct SdfGsFlatQuadPayload {
    u64 gifTag;
    u64 gifRegisterList;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[4];
} SdfGsFlatQuadPayload;

typedef char SdfGsFlatQuadPayload_size_must_be_0x40[
    (sizeof(SdfGsFlatQuadPayload) == 0x40) ? 1 : -1];
typedef char SdfGsFlatQuadPayload_primitive_at_0x10[
    ((u32)&((SdfGsFlatQuadPayload *)0)->primitive == 0x10) ? 1 : -1];
typedef char SdfGsFlatQuadPayload_rgbaq_at_0x18[
    ((u32)&((SdfGsFlatQuadPayload *)0)->rgbaq == 0x18) ? 1 : -1];
typedef char SdfGsFlatQuadPayload_xyz2_at_0x20[
    ((u32)&((SdfGsFlatQuadPayload *)0)->xyz2 == 0x20) ? 1 : -1];

/* The allocation contains the DMA/VIF prefix and the entire quad GIF stream. */
typedef struct SdfGsFlatQuadPacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsFlatQuadPayload drawing;
} SdfGsFlatQuadPacket;

typedef char SdfGsFlatQuadPacket_size_must_be_0x50[
    (sizeof(SdfGsFlatQuadPacket) == 0x50) ? 1 : -1];
typedef char SdfGsFlatQuadPacket_drawing_at_0x10[
    ((u32)&((SdfGsFlatQuadPacket *)0)->drawing == 0x10) ? 1 : -1];

void sdfBuildPacket104x4(SdfGsFlatQuadPayload *packet, s32 color,
    s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2,
    s32 x3, s32 y3, s32 depth);

/* GIF stream for a closed rectangle: its first XYZ2 value is repeated last. */
typedef struct SdfGsClosedRectanglePayload {
    u64 gifTag;
    u64 gifRegisterList;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[5];
} SdfGsClosedRectanglePayload;

/* The caller prepends DMA/VIF words. The final allocated qword is not written. */
typedef struct SdfGsClosedRectanglePacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsClosedRectanglePayload drawing;
    u64 unwrittenAllocationWord;
} SdfGsClosedRectanglePacket;

typedef char SdfGsClosedRectanglePayload_size_must_be_0x48[
    (sizeof(SdfGsClosedRectanglePayload) == 0x48) ? 1 : -1];
typedef char SdfGsClosedRectanglePayload_primitive_at_0x10[
    ((u32)&((SdfGsClosedRectanglePayload *)0)->primitive == 0x10) ? 1 : -1];
typedef char SdfGsClosedRectanglePayload_rgbaq_at_0x18[
    ((u32)&((SdfGsClosedRectanglePayload *)0)->rgbaq == 0x18) ? 1 : -1];
typedef char SdfGsClosedRectanglePayload_xyz2_at_0x20[
    ((u32)&((SdfGsClosedRectanglePayload *)0)->xyz2 == 0x20) ? 1 : -1];
typedef char SdfGsClosedRectanglePacket_size_must_be_0x60[
    (sizeof(SdfGsClosedRectanglePacket) == 0x60) ? 1 : -1];
typedef char SdfGsClosedRectanglePacket_drawing_at_0x10[
    ((u32)&((SdfGsClosedRectanglePacket *)0)->drawing == 0x10) ? 1 : -1];
typedef char SdfGsClosedRectanglePacket_unwritten_word_at_0x58[
    ((u32)&((SdfGsClosedRectanglePacket *)0)->unwrittenAllocationWord == 0x58) ? 1 : -1];

void sdfBuildQuadPacket(SdfGsClosedRectanglePayload *dst, s32 color,
    s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth);

#endif /* SDF_GS_FLAT_SHAPES_H */
