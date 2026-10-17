#ifndef MNU_EFFECT_PAIR_H
#define MNU_EFFECT_PAIR_H

#include "common.h"

/* These two signed words are written by terminal effect setup and reused as
 * drawing coordinates by menu helpers. Their role varies by caller. */
typedef struct EffectPair {
    s32 firstValue;
    s32 secondValue;
} EffectPair;

typedef char EffectPair_size_must_be_0x8[
    (sizeof(EffectPair) == 0x8) ? 1 : -1];
typedef char EffectPair_firstValue_offset_must_be_0x0[
    ((u32)&((EffectPair *)0)->firstValue == 0x0) ? 1 : -1];
typedef char EffectPair_secondValue_offset_must_be_0x4[
    ((u32)&((EffectPair *)0)->secondValue == 0x4) ? 1 : -1];

#endif /* MNU_EFFECT_PAIR_H */
