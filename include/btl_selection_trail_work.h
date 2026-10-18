#ifndef BTL_SELECTION_TRAIL_WORK_H
#define BTL_SELECTION_TRAIL_WORK_H

#include "common.h"

/* The battle selection trails' work: four fading markers sliding to the target row. */
typedef struct BattleSelectionTrailWork {
    s32 direction;
    s32 positions[4];
    s32 fadeCountdowns[4];
    s32 targetRow;
} BattleSelectionTrailWork;

#endif
