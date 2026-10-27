#ifndef BTL_FIND_ACTOR_FOR_OWNER_API_H
#define BTL_FIND_ACTOR_FOR_OWNER_API_H

#include "common.h"

/* Runtime-sequence key stored in a battle unit's identity/owner field. */
typedef u64 BtlUnitIdentity;

struct BtlUnit;

/* Find the battle unit with this runtime identity; return NULL if absent. */
struct BtlUnit *btlFindActorForOwner(BtlUnitIdentity identity);

#endif
