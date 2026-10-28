#ifndef BTL_ENEMY_SELECTION_CONTEXT_H
#define BTL_ENEMY_SELECTION_CONTEXT_H

#include "common.h"

/* A record byte is interpreted either as an action override or as an item. */
typedef s32 BtlEnemySelectionContext;

#define BTL_ENEMY_SELECTION_CONTEXT_ITEM_DROP 0
#define BTL_ENEMY_SELECTION_CONTEXT_ACTOR_ACTION 1

#endif /* BTL_ENEMY_SELECTION_CONTEXT_H */
