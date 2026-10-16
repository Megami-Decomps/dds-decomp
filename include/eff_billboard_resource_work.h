#ifndef EFF_BILLBOARD_RESOURCE_WORK_H
#define EFF_BILLBOARD_RESOURCE_WORK_H

#include "common.h"

/* The first sixteen bytes of this allocated billboard set remain opaque. */
typedef struct EffBillboardWork {
    u8 opaque00[0x10];
    u32 mode;
    u32 color;
    f32 scale;
    struct BillObj *handle;
} EffBillboardWork;

/* Creation reads the requested mode from this parameter record. */
typedef struct EffBillboardParams {
    s32 mode;
} EffBillboardParams;

typedef char EffBillboardWorkSizeCheck[(sizeof(EffBillboardWork) == 0x20) ? 1 : -1];
typedef char EffBillboardWorkModeOffsetCheck[
    ((u32)&((EffBillboardWork *)0)->mode == 0x10) ? 1 : -1];
typedef char EffBillboardWorkColorOffsetCheck[
    ((u32)&((EffBillboardWork *)0)->color == 0x14) ? 1 : -1];
typedef char EffBillboardWorkScaleOffsetCheck[
    ((u32)&((EffBillboardWork *)0)->scale == 0x18) ? 1 : -1];
typedef char EffBillboardWorkHandleOffsetCheck[
    ((u32)&((EffBillboardWork *)0)->handle == 0x1C) ? 1 : -1];
typedef char EffBillboardParamsModeOffsetCheck[
    ((u32)&((EffBillboardParams *)0)->mode == 0x00) ? 1 : -1];

#endif
