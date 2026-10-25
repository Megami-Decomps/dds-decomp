#ifndef BTL_ACTOR_ENTRY_CODE_API_H
#define BTL_ACTOR_ENTRY_CODE_API_H

#include "common.h"

struct BtlUnit;

/* This is a three-state classification of the stored code, not a predicate. */
typedef s32 BtlActorEntryCodeMatch;
enum {
    BTL_ACTOR_ENTRY_CODE_MATCH_NONE = 0,
    BTL_ACTOR_ENTRY_CODE_MATCH_MAX_CODE = 1,
    BTL_ACTOR_ENTRY_CODE_MATCH_MIN_CODE = 2
};

BtlActorEntryCodeMatch btlMatchActorEntryCode(struct BtlUnit *unit, s32 index);
s16 btlGetActorEntryCode(const struct BtlUnit *unit, s32 index);

#endif
