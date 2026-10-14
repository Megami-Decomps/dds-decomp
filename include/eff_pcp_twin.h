#ifndef EFF_PCP_TWIN_H
#define EFF_PCP_TWIN_H

#include "eff_param.h"

/* The twin effect owns eight pairs of parameter works, one shared work per
 * slot, and per-slot state/countdowns. Its constructors allocate 0xBC bytes. */
typedef struct EffPCPTwinWork {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    u8 pad0C[4];
    u32 color;
    u32 frame;
    f32 scale;
    EffParamWork *pair[8][2];
    EffParamWork *shared[8];
    u32 state[8];
    u32 counter[8];
} EffPCPTwinWork;

typedef char EffPCPTwinWork_size_must_be_0xBC[
    (sizeof(EffPCPTwinWork) == 0xBC) ? 1 : -1];
typedef char EffPCPTwinWork_pairs_offset_must_be_0x1C[
    ((u32)&((EffPCPTwinWork *)0)->pair == 0x1C) ? 1 : -1];
typedef char EffPCPTwinWork_shared_offset_must_be_0x5C[
    ((u32)&((EffPCPTwinWork *)0)->shared == 0x5C) ? 1 : -1];
typedef char EffPCPTwinWork_state_offset_must_be_0x7C[
    ((u32)&((EffPCPTwinWork *)0)->state == 0x7C) ? 1 : -1];
typedef char EffPCPTwinWork_counter_offset_must_be_0x9C[
    ((u32)&((EffPCPTwinWork *)0)->counter == 0x9C) ? 1 : -1];

#endif /* EFF_PCP_TWIN_H */
