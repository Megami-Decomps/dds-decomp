#ifndef BTL_ENEMY_SELECTION_RESULT_H
#define BTL_ENEMY_SELECTION_RESULT_H

#include "common.h"

/* The enemy record stores this selection in a byte. Zero means no selection;
 * the nonzero value is interpreted as an action or item by its caller. */
typedef u8 BtlEnemySelectionResult;

#define BTL_ENEMY_SELECTION_NONE ((BtlEnemySelectionResult)0)

#endif /* BTL_ENEMY_SELECTION_RESULT_H */
