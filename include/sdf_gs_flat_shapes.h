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
typedef char SdfGsFlatTrianglePacket_size_must_be_0x50[
    (sizeof(SdfGsFlatTrianglePacket) == 0x50) ? 1 : -1];
typedef char SdfGsFlatTrianglePacket_drawing_at_0x10[
    ((u32)&((SdfGsFlatTrianglePacket *)0)->drawing == 0x10) ? 1 : -1];
typedef char SdfGsFlatTrianglePacket_unwritten_word_at_0x48[
    ((u32)&((SdfGsFlatTrianglePacket *)0)->unwrittenAllocationWord == 0x48) ? 1 : -1];

void sdfBuildTriPacket104(SdfGsFlatTrianglePayload *dst, s32 color,
    s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2,
    s32 depth);

#endif /* SDF_GS_FLAT_SHAPES_H */
