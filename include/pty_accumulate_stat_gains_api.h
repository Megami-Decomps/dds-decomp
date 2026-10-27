#ifndef PTY_ACCUMULATE_STAT_GAINS_API_H
#define PTY_ACCUMULATE_STAT_GAINS_API_H

#include "common.h"
#include "dat_state.h"
#include "dat_stat.h"

/* Number of consecutive per-level growth rows to apply, starting at the
 * unit's current level. This accumulates base-stat gains without advancing
 * the unit's level. */
typedef s32 PtyStatGrowthRowCount;

void ptyAccumulateStatGains(
    s32 gains[DAT_BASE_STAT_COUNT], PtyStatGrowthRowCount growthRowCount,
    const datUnitWork_t *unit);

#endif /* PTY_ACCUMULATE_STAT_GAINS_API_H */
