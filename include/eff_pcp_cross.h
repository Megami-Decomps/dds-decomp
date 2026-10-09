#ifndef EFF_PCP_CROSS_H
#define EFF_PCP_CROSS_H

#include "common.h"
#include "eff_param.h"

typedef struct EffPCPCrossWork {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    u8 pad0C[4];
    u32 color;
    f32 scale;
    EffParamWork *base; /* Parameter work for the anchor model. */
    EffParamWork *handle[4][3];
    u8 pad4C[0x60];
    u32 remainingDelay[4][3];
} EffPCPCrossWork;

typedef char EffPCPCrossWork_size_must_be_0xDC[
    (sizeof(EffPCPCrossWork) == 0xDC) ? 1 : -1];
typedef char EffPCPCrossWork_handles_offset_must_be_0x1C[
    ((u32)&((EffPCPCrossWork *)0)->handle == 0x1C) ? 1 : -1];
typedef char EffPCPCrossWork_remainingDelay_offset_must_be_0xAC[
    ((u32)&((EffPCPCrossWork *)0)->remainingDelay == 0xAC) ? 1 : -1];

#endif
