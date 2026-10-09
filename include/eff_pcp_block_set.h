#ifndef EFF_PCP_BLOCK_SET_H
#define EFF_PCP_BLOCK_SET_H

#include "eff_param.h"

struct SdfMemBlock;

/* The serialized parameter block copied into each block-set work record. */
typedef struct EffPCPBlockSetParams {
    f32 position[4];
    s32 fadeInFrames;
    s32 fadeOutFrames;
    f32 scale;
    s32 groupSize[3];
    f32 groupScale[3];
    f32 groupRandomScale[3];
    f32 groupScatter[3];
    s32 tailStartFrame;
} EffPCPBlockSetParams;

/* Shared retained work owner used by construction, cloning, drawing and release.
   A non-NULL source borrows its resources; NULL owns the listed handles. */
typedef struct EffPCPBlockSetWork {
    f32 matrix[16];
    f32 previousPosition[4];
    f32 currentPosition[4];
    EffPCPBlockSetParams params;
    u32 frame;
    u32 count;
    u32 color;
    u32 mode;
    EffParamWork *headHandle;
    EffParamWork *handleA[5];
    EffParamWork **list[3];
    EffParamWork *handleB[5];
    EffParamWork *tailHandle;
    struct SdfMemBlock *alloc[3];
    struct EffPCPBlockSetWork *source;
} EffPCPBlockSetWork;

typedef char EffPCPBlockSetParams_size_must_be_0x50[
    (sizeof(EffPCPBlockSetParams) == 0x50) ? 1 : -1];
typedef char EffPCPBlockSetParams_fadeInFrames_offset_must_be_0x10[
    ((u32)&((EffPCPBlockSetParams *)0)->fadeInFrames == 0x10) ? 1 : -1];
typedef char EffPCPBlockSetParams_fadeOutFrames_offset_must_be_0x14[
    ((u32)&((EffPCPBlockSetParams *)0)->fadeOutFrames == 0x14) ? 1 : -1];
typedef char EffPCPBlockSetParams_scale_offset_must_be_0x18[
    ((u32)&((EffPCPBlockSetParams *)0)->scale == 0x18) ? 1 : -1];
typedef char EffPCPBlockSetParams_groupSize_offset_must_be_0x1C[
    ((u32)&((EffPCPBlockSetParams *)0)->groupSize == 0x1C) ? 1 : -1];
typedef char EffPCPBlockSetParams_groupScale_offset_must_be_0x28[
    ((u32)&((EffPCPBlockSetParams *)0)->groupScale == 0x28) ? 1 : -1];
typedef char EffPCPBlockSetParams_groupRandomScale_offset_must_be_0x34[
    ((u32)&((EffPCPBlockSetParams *)0)->groupRandomScale == 0x34) ? 1 : -1];
typedef char EffPCPBlockSetParams_groupScatter_offset_must_be_0x40[
    ((u32)&((EffPCPBlockSetParams *)0)->groupScatter == 0x40) ? 1 : -1];
typedef char EffPCPBlockSetParams_tailStartFrame_offset_must_be_0x4C[
    ((u32)&((EffPCPBlockSetParams *)0)->tailStartFrame == 0x4C) ? 1 : -1];
typedef char EffPCPBlockSetWork_size_must_be_0x10C[
    (sizeof(EffPCPBlockSetWork) == 0x10C) ? 1 : -1];
typedef char EffPCPBlockSetWork_previousPosition_offset_must_be_0x40[
    ((u32)&((EffPCPBlockSetWork *)0)->previousPosition == 0x40) ? 1 : -1];
typedef char EffPCPBlockSetWork_currentPosition_offset_must_be_0x50[
    ((u32)&((EffPCPBlockSetWork *)0)->currentPosition == 0x50) ? 1 : -1];
typedef char EffPCPBlockSetWork_params_offset_must_be_0x60[
    ((u32)&((EffPCPBlockSetWork *)0)->params == 0x60) ? 1 : -1];
typedef char EffPCPBlockSetWork_frame_offset_must_be_0xB0[
    ((u32)&((EffPCPBlockSetWork *)0)->frame == 0xB0) ? 1 : -1];
typedef char EffPCPBlockSetWork_count_offset_must_be_0xB4[
    ((u32)&((EffPCPBlockSetWork *)0)->count == 0xB4) ? 1 : -1];
typedef char EffPCPBlockSetWork_color_offset_must_be_0xB8[
    ((u32)&((EffPCPBlockSetWork *)0)->color == 0xB8) ? 1 : -1];
typedef char EffPCPBlockSetWork_mode_offset_must_be_0xBC[
    ((u32)&((EffPCPBlockSetWork *)0)->mode == 0xBC) ? 1 : -1];
typedef char EffPCPBlockSetWork_headHandle_offset_must_be_0xC0[
    ((u32)&((EffPCPBlockSetWork *)0)->headHandle == 0xC0) ? 1 : -1];
typedef char EffPCPBlockSetWork_handleA_offset_must_be_0xC4[
    ((u32)&((EffPCPBlockSetWork *)0)->handleA == 0xC4) ? 1 : -1];
typedef char EffPCPBlockSetWork_list_offset_must_be_0xD8[
    ((u32)&((EffPCPBlockSetWork *)0)->list == 0xD8) ? 1 : -1];
typedef char EffPCPBlockSetWork_handleB_offset_must_be_0xE4[
    ((u32)&((EffPCPBlockSetWork *)0)->handleB == 0xE4) ? 1 : -1];
typedef char EffPCPBlockSetWork_tailHandle_offset_must_be_0xF8[
    ((u32)&((EffPCPBlockSetWork *)0)->tailHandle == 0xF8) ? 1 : -1];
typedef char EffPCPBlockSetWork_alloc_offset_must_be_0xFC[
    ((u32)&((EffPCPBlockSetWork *)0)->alloc == 0xFC) ? 1 : -1];
typedef char EffPCPBlockSetWork_source_offset_must_be_0x108[
    ((u32)&((EffPCPBlockSetWork *)0)->source == 0x108) ? 1 : -1];

/* The builder consumes a native parameter table and returns its retained work. */
EffPCPBlockSetWork *effPcpBuildBlockSet(void *parameterTable);
EffPCPBlockSetWork *effPcpCreateBlockSetWork(
    const EffPCPBlockSetParams *params, void **blocks);
EffPCPBlockSetWork *effPcpBlockSetCloneShared(EffPCPBlockSetWork *source);
EffPCPBlockSetWork *effPcpCloneBlockWithUnitMatrix(EffPCPBlockSetWork *source);
EffPCPBlockSetWork *effCloneBlockWorkFromSource(EffPCPBlockSetWork *source);
void effPcpBlockSetWorkRelease(EffPCPBlockSetWork *work);
void effPcpCopyVector60(EffPCPBlockSetWork *work, void *source);
void effPcpCopyBlockMatrix(EffPCPBlockSetWork *work, void *source);
void effPcpBlockSetSetColor(EffPCPBlockSetWork *work, u32 color);

#endif /* EFF_PCP_BLOCK_SET_H */
