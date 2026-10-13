#ifndef EFF_RANDOM_MOD_H
#define EFF_RANDOM_MOD_H

#include "eff_random.h"

/* Return the next caller-owned or shared state value reduced modulo `modulus`. */
u32 effMiscRandMod(EffRandState *state, u32 modulus);

#endif /* EFF_RANDOM_MOD_H */
