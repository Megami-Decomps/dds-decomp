#ifndef DAT_AFFINITY_VALUE_API_H
#define DAT_AFFINITY_VALUE_API_H

#include "common.h"
#include "dat_affinity_selector.h"

/* Low 16-bit value channel of the packed effective-affinity result. */
typedef u16 DatAffinityValue;

struct datUnitWork_t;

DatAffinityValue datGetAisyoValue(
    const struct datUnitWork_t *unit, DatAffinitySelector selector);

#endif /* DAT_AFFINITY_VALUE_API_H */
