#ifndef EFF_BILLBOARD_RESOURCE_WORK_H
#define EFF_BILLBOARD_RESOURCE_WORK_H

#include "common.h"

/* The first sixteen bytes of this allocated billboard set remain opaque. */
typedef struct EffBillboardSetWork {
    u8 opaque00[0x10];
    u32 mode;
    u32 color;
    f32 scale;
    struct BillObj *handle;
} EffBillboardSetWork;

/* Creation reads the requested mode from this parameter record. */
typedef struct EffBillboardSetParams {
    s32 mode;
} EffBillboardSetParams;

typedef char EffBillboardSetWorkSizeCheck[(sizeof(EffBillboardSetWork) == 0x20) ? 1 : -1];
typedef char EffBillboardSetWorkModeOffsetCheck[
    ((u32)&((EffBillboardSetWork *)0)->mode == 0x10) ? 1 : -1];
typedef char EffBillboardSetWorkColorOffsetCheck[
    ((u32)&((EffBillboardSetWork *)0)->color == 0x14) ? 1 : -1];
typedef char EffBillboardSetWorkScaleOffsetCheck[
    ((u32)&((EffBillboardSetWork *)0)->scale == 0x18) ? 1 : -1];
typedef char EffBillboardSetWorkHandleOffsetCheck[
    ((u32)&((EffBillboardSetWork *)0)->handle == 0x1C) ? 1 : -1];
typedef char EffBillboardSetParamsModeOffsetCheck[
    ((u32)&((EffBillboardSetParams *)0)->mode == 0x00) ? 1 : -1];

#endif
