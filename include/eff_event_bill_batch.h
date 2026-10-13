#ifndef EFF_EVENT_BILL_BATCH_H
#define EFF_EVENT_BILL_BATCH_H

#include "common.h"

#define EFF_EVENT_BILL_BATCH_PARTICLE_COUNT 15

typedef struct EffEventBillBatch {
    u128 positions[EFF_EVENT_BILL_BATCH_PARTICLE_COUNT]; /* 0x000 */
    u32 colors[EFF_EVENT_BILL_BATCH_PARTICLE_COUNT];     /* 0x0F0 */
    s16 uv[EFF_EVENT_BILL_BATCH_PARTICLE_COUNT][8];      /* 0x12C */
    f32 corners[EFF_EVENT_BILL_BATCH_PARTICLE_COUNT][8]; /* 0x21C */
    s32 count;                                           /* 0x3FC */
} EffEventBillBatch;

typedef char EffEventBillBatch_colors_at_0F0[
    ((u32)&((EffEventBillBatch *)0)->colors == 0x0F0) ? 1 : -1];
typedef char EffEventBillBatch_uv_at_12C[
    ((u32)&((EffEventBillBatch *)0)->uv == 0x12C) ? 1 : -1];
typedef char EffEventBillBatch_corners_at_21C[
    ((u32)&((EffEventBillBatch *)0)->corners == 0x21C) ? 1 : -1];
typedef char EffEventBillBatch_count_at_3FC[
    ((u32)&((EffEventBillBatch *)0)->count == 0x3FC) ? 1 : -1];
typedef char EffEventBillBatch_size_must_be_0x400[
    (sizeof(EffEventBillBatch) == 0x400) ? 1 : -1];

#endif
