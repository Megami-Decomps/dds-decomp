#ifndef EFF_BILLBOARD_WORK_H
#define EFF_BILLBOARD_WORK_H

#include "common.h"

struct BillObj;

/* Shared DDS1/DDS2 animated billboard effect state. */
typedef struct EffBillboardWork {
    f32 position[4];
    f32 rotation[4];
    f32 scale;
    u32 color;
    s32 frame;
    f32 heightScale;
    f32 widthScale;
    u8 opaque34[0x30];
    struct BillObj *billboard;
} EffBillboardWork;

typedef char EffBillboardWork_size_must_be_0x68[
    (sizeof(EffBillboardWork) == 0x68) ? 1 : -1];
typedef char EffBillboardWork_rotation_offset_must_be_0x10[
    ((u32)&((EffBillboardWork *)0)->rotation == 0x10) ? 1 : -1];
typedef char EffBillboardWork_scale_offset_must_be_0x20[
    ((u32)&((EffBillboardWork *)0)->scale == 0x20) ? 1 : -1];
typedef char EffBillboardWork_frame_offset_must_be_0x28[
    ((u32)&((EffBillboardWork *)0)->frame == 0x28) ? 1 : -1];
typedef char EffBillboardWork_heightScale_offset_must_be_0x2C[
    ((u32)&((EffBillboardWork *)0)->heightScale == 0x2C) ? 1 : -1];
typedef char EffBillboardWork_billboard_offset_must_be_0x64[
    ((u32)&((EffBillboardWork *)0)->billboard == 0x64) ? 1 : -1];

#endif /* EFF_BILLBOARD_WORK_H */
