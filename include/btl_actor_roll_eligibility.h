#ifndef BTL_ACTOR_ROLL_ELIGIBILITY_H
#define BTL_ACTOR_ROLL_ELIGIBILITY_H

#include "common.h"

struct BtlUnit;

/* Whether the actor passes the override, status and ability/chance checks. */
typedef s32 BtlActorRollEligibilityResult;

#define BTL_ACTOR_ROLL_ELIGIBILITY_REJECTED \
    ((BtlActorRollEligibilityResult)0)
#define BTL_ACTOR_ROLL_ELIGIBILITY_ACCEPTED \
    ((BtlActorRollEligibilityResult)1)

BtlActorRollEligibilityResult btlRollActorEligibilityWithAbilityOverride(
    struct BtlUnit *unit);

#endif /* BTL_ACTOR_ROLL_ELIGIBILITY_H */
