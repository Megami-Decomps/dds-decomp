#ifndef PRF_FALLBACK_ITEM_REQUIREMENT_H
#define PRF_FALLBACK_ITEM_REQUIREMENT_H

#include "common.h"

typedef struct PrfItemRequirement {
    s32 id;
    s32 minimum;
} PrfItemRequirement;

/* A fallback entry has an active byte, two item requirements, then a result flag. */
typedef struct PrfFallbackItemRequirement {
    u8 active;
    u8 pad01[3];
    PrfItemRequirement requirement[2];
    u32 flagId;
} PrfFallbackItemRequirement;

#endif
