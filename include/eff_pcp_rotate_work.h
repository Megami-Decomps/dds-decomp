#ifndef EFF_PCP_ROTATE_WORK_H
#define EFF_PCP_ROTATE_WORK_H

#include "eff_pcp_block_set.h"

typedef struct EffPCPRotateParams {
    s32 startFrame[3];
    EffPCPBlockSetParams group[3];
} EffPCPRotateParams;

typedef struct EffPCPRotateWork {
    EffPCPRotateParams params;
    EffPCPBlockSetWork *blockSets[3];
    s32 frame;
} EffPCPRotateWork;

typedef char EffPCPRotateParams_size_must_be_0xFC[
    (sizeof(EffPCPRotateParams) == 0xFC) ? 1 : -1];
typedef char EffPCPRotateParams_group_offset_must_be_0x0C[
    ((u32)&((EffPCPRotateParams *)0)->group == 0x0C) ? 1 : -1];
typedef char EffPCPRotateWork_size_must_be_0x10C[
    (sizeof(EffPCPRotateWork) == 0x10C) ? 1 : -1];
typedef char EffPCPRotateWork_blockSets_offset_must_be_0xFC[
    ((u32)&((EffPCPRotateWork *)0)->blockSets == 0xFC) ? 1 : -1];
typedef char EffPCPRotateWork_frame_offset_must_be_0x108[
    ((u32)&((EffPCPRotateWork *)0)->frame == 0x108) ? 1 : -1];

#endif /* EFF_PCP_ROTATE_WORK_H */
