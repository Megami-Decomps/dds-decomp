#ifndef BTL_ACTOR_ENTRY_CODE_API_H
#define BTL_ACTOR_ENTRY_CODE_API_H

#include "common.h"

struct BtlUnit;

/* Signed index into BtlUnit.entrySlots; valid values are 0 through 6. */
typedef s32 BtlActorEntrySlotIndex;

enum {
    BTL_ACTOR_ENTRY_SLOT_PHYSICAL_CHARGE = 5,
    BTL_ACTOR_ENTRY_SLOT_MAGIC_CHARGE = 6
};

/* This is a three-state classification of the stored code, not a predicate. */
typedef s32 BtlActorEntryCodeMatch;
enum {
    BTL_ACTOR_ENTRY_CODE_MATCH_NONE = 0,
    BTL_ACTOR_ENTRY_CODE_MATCH_MAX_CODE = 1,
    BTL_ACTOR_ENTRY_CODE_MATCH_MIN_CODE = 2
};

BtlActorEntryCodeMatch btlMatchActorEntryCode(
    const struct BtlUnit *unit, BtlActorEntrySlotIndex slotIndex);
/* True only when the slot has a code and its countdown has elapsed. */
u8 btlActorEntryIsExpired(const struct BtlUnit *unit, BtlActorEntrySlotIndex slotIndex);
s16 btlGetActorEntryCode(const struct BtlUnit *unit, BtlActorEntrySlotIndex slotIndex);
void btlAdjustActorEntryCode(struct BtlUnit *unit, BtlActorEntrySlotIndex slotIndex,
                             s16 delta);
void btlSetActorEntryCode(struct BtlUnit *unit, BtlActorEntrySlotIndex slotIndex,
                          s16 code);
void btlClearActorEntrySlot(struct BtlUnit *unit, BtlActorEntrySlotIndex slotIndex);

#endif
