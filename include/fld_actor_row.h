#ifndef FLD_ACTOR_ROW_H
#define FLD_ACTOR_ROW_H

#include "common.h"

/* Per-actor motion task state; retail advances the actor table at 0x5C strides. */
typedef struct FldActorRow {
    s32 kind;
    u32 actorId;
    u32 unk08;
    u32 firstKey;
    u32 secondaryMotion;
    u32 secondKey;
    u32 transitionKey;
    s32 firstFrame;
    s32 secondFrame;
    s32 transitionFrame;
    s32 frameCount;
    f32 firstValues[3];
    f32 firstStep[3];
    f32 secondValues[3];
    f32 secondStep[3];
} FldActorRow;

#endif
