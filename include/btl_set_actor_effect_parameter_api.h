#ifndef BTL_SET_ACTOR_EFFECT_PARAMETER_API_H
#define BTL_SET_ACTOR_EFFECT_PARAMETER_API_H

#include "common.h"

struct BtlUnit;

typedef s8 BtlActorEffectPositionResult;
/* Reports whether the requested model marker position was applied. */
enum {
    BTL_ACTOR_EFFECT_POSITION_UNAVAILABLE = 0,
    BTL_ACTOR_EFFECT_POSITION_APPLIED = 1
};

#ifdef VERSION_DDS1
BtlActorEffectPositionResult btlSetActorEffectParameter(struct BtlUnit *unit, s32 mode);
#else
BtlActorEffectPositionResult btlSetActorEffectParameter(struct BtlUnit *unit, s32 mode);
#endif

#endif
