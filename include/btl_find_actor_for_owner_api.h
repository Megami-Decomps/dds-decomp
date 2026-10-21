#ifndef BTL_FIND_ACTOR_FOR_OWNER_API_H
#define BTL_FIND_ACTOR_FOR_OWNER_API_H

#include "common.h"

struct BtlUnit;

struct BtlUnit *btlFindActorForOwner(u64 ownerId);

#endif
