#ifndef EFF_PCP_DELAYED_PAIRS_H
#define EFF_PCP_DELAYED_PAIRS_H

#include "eff_param.h"

/* Six paired parameter handles share this 0x60-byte allocation. The first
 * three words are initialized by creation and cloning; their meanings remain
 * unknown. The value at +0x14 is initialized to 1 and written by the scale
 * setter. */
typedef struct EffPCPDelayedPairs {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    u8 unk0C[4];
    u32 color;
    f32 unk14;
    EffParamWork *handle[12];
    u32 remainingDelay[6];
} EffPCPDelayedPairs;

typedef char EffPCPDelayedPairs_size_must_be_0x60[
    (sizeof(EffPCPDelayedPairs) == 0x60) ? 1 : -1];
typedef char EffPCPDelayedPairs_color_offset_must_be_0x10[
    ((u32)&((EffPCPDelayedPairs *)0)->color == 0x10) ? 1 : -1];
typedef char EffPCPDelayedPairs_unk14_offset_must_be_0x14[
    ((u32)&((EffPCPDelayedPairs *)0)->unk14 == 0x14) ? 1 : -1];
typedef char EffPCPDelayedPairs_handle_offset_must_be_0x18[
    ((u32)&((EffPCPDelayedPairs *)0)->handle == 0x18) ? 1 : -1];
typedef char EffPCPDelayedPairs_remainingDelay_offset_must_be_0x48[
    ((u32)&((EffPCPDelayedPairs *)0)->remainingDelay == 0x48) ? 1 : -1];

#endif /* EFF_PCP_DELAYED_PAIRS_H */
