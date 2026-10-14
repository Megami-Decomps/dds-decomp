#ifndef EFF_OWNER_RECORDS_H
#define EFF_OWNER_RECORDS_H

#include "common.h"

typedef struct EffectRecord {
    void *owner;
    s32 slot;
    struct EffectRecord *prev;
    struct EffectRecord *next;
} EffectRecord;

typedef struct EffectOwnerRecord {
    void *owner;
    EffectRecord *entries[16];
} EffectOwnerRecord;

/* Per-slot effect data; only its sixteen-bucket index is recovered. */
typedef struct EffectSlot {
    u8 opaque00[0x28];
    s32 bucketIndex; /* 0x28: indexes EffectOwnerRecord.entries. */
    u8 opaque2C[0x54];
} EffectSlot;

typedef struct EffectSlotOwner {
    u8 opaque00[0x10];
    EffectSlot *slots; /* 0x10 */
} EffectSlotOwner;

typedef char EffectRecordSizeCheck[sizeof(EffectRecord) == 0x10 ? 1 : -1];
typedef char EffectOwnerRecordSizeCheck[sizeof(EffectOwnerRecord) == 0x44 ? 1 : -1];
typedef char EffectSlotSizeCheck[sizeof(EffectSlot) == 0x80 ? 1 : -1];
typedef char EffectSlot_bucketIndex_offset_check[
    ((u32)&((EffectSlot *)0)->bucketIndex == 0x28) ? 1 : -1];
typedef char EffectSlotOwnerSizeCheck[sizeof(EffectSlotOwner) == 0x14 ? 1 : -1];
typedef char EffectSlotOwner_slots_offset_check[
    ((u32)&((EffectSlotOwner *)0)->slots == 0x10) ? 1 : -1];

#endif
