#ifndef EFF_CUBIC_BEZIER_ARRAY_H
#define EFF_CUBIC_BEZIER_ARRAY_H

#include "common.h"

struct EffCubicBezierSlot;

/* The 0xC owner follows an array of cubic slots in the same allocation. */
typedef struct EffCubicBezierArray {
    struct EffCubicBezierSlot *slots;
    u32 slotCount;
    struct sdfMemHandle_t *allocation;
} EffCubicBezierArray;

typedef char EffCubicBezierArraySizeAssert[(sizeof(EffCubicBezierArray) == 0xC) ? 1 : -1];
typedef char EffCubicBezierArraySlotsOffsetAssert[
    ((u32)&((EffCubicBezierArray *)0)->slots == 0x00) ? 1 : -1];
typedef char EffCubicBezierArrayCountOffsetAssert[
    ((u32)&((EffCubicBezierArray *)0)->slotCount == 0x04) ? 1 : -1];
typedef char EffCubicBezierArrayAllocationOffsetAssert[
    ((u32)&((EffCubicBezierArray *)0)->allocation == 0x08) ? 1 : -1];

#endif
