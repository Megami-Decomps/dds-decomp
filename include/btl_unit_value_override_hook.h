#ifndef BTL_UNIT_VALUE_OVERRIDE_HOOK_H
#define BTL_UNIT_VALUE_OVERRIDE_HOOK_H

#include "dat_affinity_selector_type.h"

struct datUnitWork_t;

/* Return -1 to use the normal affinity-table resolver. */
typedef s32 (*BtlUnitValueOverrideHook)(
    struct datUnitWork_t *record, DatAffinitySelector selector);

#endif /* BTL_UNIT_VALUE_OVERRIDE_HOOK_H */
