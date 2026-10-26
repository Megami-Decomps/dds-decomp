#ifndef BTL_ENEMY_SPAWN_CAPACITY_API_H
#define BTL_ENEMY_SPAWN_CAPACITY_API_H

#include "common.h"

struct BtlUnit;

/* Whether the battle's active and cumulative spawn limits permit another enemy. */
typedef s32 BtlEnemySpawnCapacityResult;

#define BTL_ENEMY_SPAWN_CAPACITY_EXHAUSTED 0
#define BTL_ENEMY_SPAWN_CAPACITY_AVAILABLE 1

/* The requester may be null when querying capacity without a particular unit. */
BtlEnemySpawnCapacityResult btlCanSpawnAnotherEnemy(struct BtlUnit *requester);
BtlEnemySpawnCapacityResult btlCheckEnemySpawnCapacity(void);

#endif /* BTL_ENEMY_SPAWN_CAPACITY_API_H */
