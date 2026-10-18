#ifndef EFF_BLOCK128_H
#define EFF_BLOCK128_H

#include "common.h"

/* A 128-byte block the effect code copies whole (the effect file queue's saved state). */
typedef struct EffectBlock128 {
    u32 word[32];
} EffectBlock128;

#endif
