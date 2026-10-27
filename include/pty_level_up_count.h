#ifndef PTY_LEVEL_UP_COUNT_H
#define PTY_LEVEL_UP_COUNT_H

#include "common.h"
#include "dat_state.h"

/* Count of level thresholds reached by stored EXP, without changing the unit. */
typedef s32 PtyLevelUpCount;

/* Return the currently available level-up count; returns zero at level 99. */
PtyLevelUpCount ptyCalcLevelUps(datUnitWork_t *unit);

#endif /* PTY_LEVEL_UP_COUNT_H */
