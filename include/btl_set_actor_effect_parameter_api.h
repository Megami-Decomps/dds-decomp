#ifndef BTL_SET_ACTOR_EFFECT_PARAMETER_API_H
#define BTL_SET_ACTOR_EFFECT_PARAMETER_API_H

#include "common.h"

struct BtlUnit;

#ifdef VERSION_DDS1
s32 btlSetActorEffectParameter(struct BtlUnit *unit, s32 mode);
#else
s8 btlSetActorEffectParameter(struct BtlUnit *unit, s32 mode);
#endif

#endif
