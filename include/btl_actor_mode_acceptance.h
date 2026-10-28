#ifndef BTL_ACTOR_MODE_ACCEPTANCE_H
#define BTL_ACTOR_MODE_ACCEPTANCE_H

#include "common.h"

struct BtlUnit;

/* Whether the active actor's current motion is accepted by battle policy. */
typedef s32 BtlActorModeAcceptanceResult;
enum {
    BTL_ACTOR_MODE_NOT_ACCEPTED = 0,
    BTL_ACTOR_MODE_ACCEPTED = 1
};

BtlActorModeAcceptanceResult btlIsActorModeAcceptedByBattleHook(struct BtlUnit *unit);

#endif /* BTL_ACTOR_MODE_ACCEPTANCE_H */
