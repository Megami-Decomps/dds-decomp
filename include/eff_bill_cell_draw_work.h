#ifndef EFF_BILL_CELL_DRAW_WORK_H
#define EFF_BILL_CELL_DRAW_WORK_H

#include "common.h"

typedef struct BillCellDrawWork {
    u8 opaque00[0x10];
    u8 transform[0x10];
    f32 scale;
    s32 baseColor;
    u32 frameLimit;
    u8 opaque2C[4];
    u32 *instances;
    u8 *config;
} BillCellDrawWork;

typedef char BillCellDrawWorkSizeCheck[(sizeof(BillCellDrawWork) == 0x38) ? 1 : -1];
typedef char BillCellDrawWorkTransformOffsetCheck[
    ((u32)&((BillCellDrawWork *)0)->transform == 0x10) ? 1 : -1];
typedef char BillCellDrawWorkScaleOffsetCheck[
    ((u32)&((BillCellDrawWork *)0)->scale == 0x20) ? 1 : -1];
typedef char BillCellDrawWorkBaseColorOffsetCheck[
    ((u32)&((BillCellDrawWork *)0)->baseColor == 0x24) ? 1 : -1];
typedef char BillCellDrawWorkFrameLimitOffsetCheck[
    ((u32)&((BillCellDrawWork *)0)->frameLimit == 0x28) ? 1 : -1];
typedef char BillCellDrawWorkInstancesOffsetCheck[
    ((u32)&((BillCellDrawWork *)0)->instances == 0x30) ? 1 : -1];
typedef char BillCellDrawWorkConfigOffsetCheck[
    ((u32)&((BillCellDrawWork *)0)->config == 0x34) ? 1 : -1];

#endif
