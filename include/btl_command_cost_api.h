#ifndef BTL_COMMAND_COST_API_H
#define BTL_COMMAND_COST_API_H

#include "btl_command_id.h"

struct BtlUnit;
struct datUnitWork_t;

/* These APIs take the command-table key, not the computed cost. */
s32 btlApplyCommandAbilityMultiplier(
    struct datUnitWork_t *battler, BtlCommandId commandId);
s32 btlGetCombinedPartyCommandPower(
    struct datUnitWork_t *base, struct BtlUnit *first,
    struct BtlUnit *second, struct BtlUnit *third, BtlCommandId commandId);

/* Both slot-cost helpers convert command IDs to affinity-table rows. */
s8 btlGetLinkageSlotCost(BtlCommandId commandId);
s32 btlGetSlotValueAdjustedForSpecialAbility(
    struct BtlUnit *battler, BtlCommandId commandId);

#endif /* BTL_COMMAND_COST_API_H */
