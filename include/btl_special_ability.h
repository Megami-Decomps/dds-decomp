#ifndef BTL_SPECIAL_ABILITY_H
#define BTL_SPECIAL_ABILITY_H

#include "common.h"

struct datUnitWork_t;

/* Tests whether a party record has the requested special ability. */
s32 btlCheckSpecialAbility(struct datUnitWork_t *record, s32 ability);

#endif /* BTL_SPECIAL_ABILITY_H */
