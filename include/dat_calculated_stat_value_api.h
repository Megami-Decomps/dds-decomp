#ifndef DAT_CALCULATED_STAT_VALUE_API_H
#define DAT_CALCULATED_STAT_VALUE_API_H

#include "common.h"
#include "dat_stat.h"

typedef s32 DatCalculatedStatValue;

struct datUnitWork_t;

/* Profile results are 1..99; DDS2's slot-adjusted result can also be zero. */
DatCalculatedStatValue datGetClampedProfileAdjustedStat(
    struct datUnitWork_t *unit, DatBaseStatIndex statIndex);
DatCalculatedStatValue datGetStatWithStatusOverride(
    struct datUnitWork_t *unit, DatBaseStatIndex statIndex);

#endif
