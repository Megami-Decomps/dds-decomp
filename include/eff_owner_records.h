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

typedef char EffectRecordSizeCheck[sizeof(EffectRecord) == 0x10 ? 1 : -1];
typedef char EffectOwnerRecordSizeCheck[sizeof(EffectOwnerRecord) == 0x44 ? 1 : -1];

#endif
