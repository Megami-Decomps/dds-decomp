#ifndef BTL_ACTOR_RESOURCE_CLEANUP_API_H
#define BTL_ACTOR_RESOURCE_CLEANUP_API_H

#include "common.h"

struct BtlUnit;

typedef s32 BtlBattleResourceCleanupResult;

#define BTL_BATTLE_RESOURCE_CLEANUP_PENDING 0
#define BTL_BATTLE_RESOURCE_CLEANUP_COMPLETE 1

typedef s32 BtlActorResourceCleanupResult;

#define BTL_ACTOR_RESOURCE_CLEANUP_PENDING 0
#define BTL_ACTOR_RESOURCE_CLEANUP_COMPLETE 1

/* COMPLETE means actor resources and battle tasks no longer block scene progress. */
BtlBattleResourceCleanupResult btlPollBattleResourceCleanup(struct BtlUnit *unit);

/* Frees inactive resource nodes when possible. PENDING means cleanup blockers
 * remain; COMPLETE means the actor's resources and tasks can be torn down. */
BtlActorResourceCleanupResult btlPollActorResourceCleanup(struct BtlUnit *actor);

#endif /* BTL_ACTOR_RESOURCE_CLEANUP_API_H */
