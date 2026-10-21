#ifndef DDS_EFF_SCALED_CLASS_FRAME_ROWS_H
#define DDS_EFF_SCALED_CLASS_FRAME_ROWS_H

#include "common.h"

/* Shared frame and output views consumed by effUpdateScaledClassFrameRows. */
typedef struct EffClassFrameResource {
    u8 pad_00[8];
    u32 frameCount;
    u8 pad_0C[4];
    u8 *rows;
} EffClassFrameResource;

typedef struct EffClassRowOutput {
    u8 pad_00[0x1C];
    u8 *rows;
} EffClassRowOutput;

typedef char EffClassFrameResource_size_check[
    (sizeof(EffClassFrameResource) == 0x14) ? 1 : -1];
typedef char EffClassFrameResource_frameCount_offset_check[
    ((u32)&((EffClassFrameResource *)0)->frameCount == 0x08) ? 1 : -1];
typedef char EffClassFrameResource_rows_offset_check[
    ((u32)&((EffClassFrameResource *)0)->rows == 0x10) ? 1 : -1];
typedef char EffClassRowOutput_size_check[
    (sizeof(EffClassRowOutput) == 0x20) ? 1 : -1];
typedef char EffClassRowOutput_rows_offset_check[
    ((u32)&((EffClassRowOutput *)0)->rows == 0x1C) ? 1 : -1];

#endif
