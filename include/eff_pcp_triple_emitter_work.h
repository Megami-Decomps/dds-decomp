#ifndef EFF_PCP_TRIPLE_EMITTER_WORK_H
#define EFF_PCP_TRIPLE_EMITTER_WORK_H

#include "common.h"

struct EffNode;

/* Three view-relative offsets and stagger thresholds for seven handles per group. */
typedef struct {
    f32 origin[4];
    f32 offset[3][2];
    s32 groupStartFrame[3];
    s32 handleDelay[7];
} EffPCPTripleParams;

/* Three groups retain one source node and two clones per staggered slot. */
typedef struct EffPCPTripleWork {
    EffPCPTripleParams head;
    s32 frame;
    u32 color;
    struct EffNode *handles[21];
} EffPCPTripleWork;

typedef char EffPCPTripleParamsSizeCheck[
    (sizeof(EffPCPTripleParams) == 0x50) ? 1 : -1];
typedef char EffPCPTripleParams_groupStartFrame_at_0x28[
    ((u32)&((EffPCPTripleParams *)0)->groupStartFrame == 0x28) ? 1 : -1];
typedef char EffPCPTripleParams_handleDelay_at_0x34[
    ((u32)&((EffPCPTripleParams *)0)->handleDelay == 0x34) ? 1 : -1];
typedef char EffPCPTripleWorkSizeCheck[
    (sizeof(EffPCPTripleWork) == 0xAC) ? 1 : -1];
typedef char EffPCPTripleWork_frame_at_0x50[
    ((u32)&((EffPCPTripleWork *)0)->frame == 0x50) ? 1 : -1];
typedef char EffPCPTripleWork_handles_at_0x58[
    ((u32)&((EffPCPTripleWork *)0)->handles == 0x58) ? 1 : -1];

#endif
