#ifndef EFF_BATTLE_TRANSITION_DURATION_H
#define EFF_BATTLE_TRANSITION_DURATION_H

#include "common.h"

/* The table selector is signed; each entry supplies an unsigned frame count. */
typedef s32 EffBattleTransitionDurationIndex;
typedef u32 EffBattleTransitionDuration;

extern EffBattleTransitionDuration effBattleTransitionDurations[];
EffBattleTransitionDuration effBattleMiscGetTableEntry(EffBattleTransitionDurationIndex durationIndex);

#endif
