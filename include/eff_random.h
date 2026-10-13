#ifndef EFF_RANDOM_H
#define EFF_RANDOM_H

#include "common.h"

/* Four ordered 32-bit words in the shared effect random-number state. */
typedef struct EffRandState {
    u32 words[4];
} EffRandState;

typedef char EffRandState_size_must_be_0x10[
    (sizeof(EffRandState) == 0x10) ? 1 : -1];
typedef char EffRandState_last_word_offset_must_be_0x0C[
    ((u32)&((EffRandState *)0)->words[3] == 0x0C) ? 1 : -1];

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 effMiscRand(EffRandState *state);

/* Seed caller-owned state from the clock; NULL selects the shared state. */
void effMiscSeedRandomFromClock(EffRandState *state);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 effMiscRandUnitFloat(void *state);

#endif /* EFF_RANDOM_H */
