#ifndef BTL_SPECIAL_ABILITY_H
#define BTL_SPECIAL_ABILITY_H

#include "common.h"

struct DatPartyRecord;

/* Tests whether a party record has the requested special ability. */
s32 btlCheckSpecialAbility(struct DatPartyRecord *record, s32 ability);

#endif /* BTL_SPECIAL_ABILITY_H */
