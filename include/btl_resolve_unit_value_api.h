#ifndef BTL_RESOLVE_UNIT_VALUE_API_H
#define BTL_RESOLVE_UNIT_VALUE_API_H

#include "dat_effective_affinity_api.h"
#include "btl_unit_value_override_hook.h"

struct BtlUnit;
struct datUnitWork_t;

DatEffectiveAffinityResult btlGetUnitElementAffinity(
    struct BtlUnit *unit, DatAffinitySelector selector);
DatEffectiveAffinityResult btlResolveUnitValueWithOverride(
    struct datUnitWork_t *record, DatAffinitySelector selector);

#endif
