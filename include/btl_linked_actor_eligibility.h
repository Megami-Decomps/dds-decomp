#ifndef BTL_LINKED_ACTOR_ELIGIBILITY_H
#define BTL_LINKED_ACTOR_ELIGIBILITY_H

#include "common.h"

struct BtlLinkedCommand;

typedef s32 BtlLinkedActorEligibilityResult;
#define BTL_LINKED_ACTOR_INELIGIBLE ((BtlLinkedActorEligibilityResult)0)
#define BTL_LINKED_ACTOR_ELIGIBLE ((BtlLinkedActorEligibilityResult)1)

/* Returns whether status, linked-group marks and owner state permit linked use. */
BtlLinkedActorEligibilityResult btlCanUseLinkedActor(struct BtlLinkedCommand *actor);

#endif /* BTL_LINKED_ACTOR_ELIGIBILITY_H */
