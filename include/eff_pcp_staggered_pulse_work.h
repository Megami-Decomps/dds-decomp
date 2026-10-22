#ifndef EFF_PCP_STAGGERED_PULSE_WORK_H
#define EFF_PCP_STAGGERED_PULSE_WORK_H

#include "common.h"
#include "eff_param.h"

/* Ten staggered model copies share this 0x98-byte lifecycle record. */
typedef struct EffPCPSprayWork {
    f32 pos[4];
    f32 scale;
    u32 color;
    u32 count;
    s32 frame;
    s32 startFrame[10];
    f32 rotationY[10];
    effPartHandle_t *handle[10];
} EffPCPSprayWork;

typedef char EffPCPSprayWork_size_must_be_0x98[
    (sizeof(EffPCPSprayWork) == 0x98) ? 1 : -1];
typedef char EffPCPSprayWork_startFrame_at_0x20[
    ((u32)&((EffPCPSprayWork *)0)->startFrame == 0x20) ? 1 : -1];
typedef char EffPCPSprayWork_rotationY_at_0x48[
    ((u32)&((EffPCPSprayWork *)0)->rotationY == 0x48) ? 1 : -1];
typedef char EffPCPSprayWork_handle_at_0x70[
    ((u32)&((EffPCPSprayWork *)0)->handle == 0x70) ? 1 : -1];

#endif
