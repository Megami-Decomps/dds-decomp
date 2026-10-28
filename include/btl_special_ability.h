#ifndef BTL_SPECIAL_ABILITY_H
#define BTL_SPECIAL_ABILITY_H

#include "common.h"

struct datUnitWork_t;

typedef s32 BtlSpecialAbilityAvailability;
enum {
    BTL_SPECIAL_ABILITY_UNAVAILABLE = 0,
    BTL_SPECIAL_ABILITY_AVAILABLE = 1,
};

/* Returns availability after ownership and any ability-specific phase checks. */
BtlSpecialAbilityAvailability btlCheckSpecialAbility(
    const struct datUnitWork_t *record, s32 ability);

#endif /* BTL_SPECIAL_ABILITY_H */
