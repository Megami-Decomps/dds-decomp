#ifndef DAT_EFFECTIVE_AFFINITY_API_H
#define DAT_EFFECTIVE_AFFINITY_API_H

#include "common.h"
#include "dat_affinity_selector.h"

struct datUnitWork_t;

/* Signed packed result: the low half is the affinity value and high bits carry flags. */
typedef s32 DatEffectiveAffinityResult;

DatEffectiveAffinityResult datGetEffectiveAffinity(
    const struct datUnitWork_t *unit, DatAffinitySelector selector);

#endif /* DAT_EFFECTIVE_AFFINITY_API_H */
