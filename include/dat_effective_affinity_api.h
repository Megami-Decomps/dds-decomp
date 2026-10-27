#ifndef DAT_EFFECTIVE_AFFINITY_API_H
#define DAT_EFFECTIVE_AFFINITY_API_H

#include "dat_effective_affinity_result_type.h"
#include "dat_affinity_selector.h"

struct datUnitWork_t;

DatEffectiveAffinityResult datGetEffectiveAffinity(
    const struct datUnitWork_t *unit, DatAffinitySelector selector);

#endif /* DAT_EFFECTIVE_AFFINITY_API_H */
