#ifndef EFF_PCP_BURST_WORK_H
#define EFF_PCP_BURST_WORK_H

#include "eff_thunder_fragment.h"

/* Owner for a muzzle thunder ray and its fragment effect. */
typedef struct EffPCPBurstWork {
    f32 position[4];                         /* 0x00 */
    u32 tintColor;                            /* 0x10 */
    u8 opaque14[4];
    f32 startDistance;                       /* 0x18 */
    f32 length;                               /* 0x1C */
    EffThunderFragmentWork *fragmentWork;    /* 0x20 */
} EffPCPBurstWork;

typedef char EffPCPBurstWorkSizeCheck[
    (sizeof(EffPCPBurstWork) == 0x24) ? 1 : -1];
typedef char EffPCPBurstWorkStartDistanceOffsetCheck[
    ((u32)&((EffPCPBurstWork *)0)->startDistance == 0x18) ? 1 : -1];
typedef char EffPCPBurstWorkFragmentOffsetCheck[
    ((u32)&((EffPCPBurstWork *)0)->fragmentWork == 0x20) ? 1 : -1];

#endif
