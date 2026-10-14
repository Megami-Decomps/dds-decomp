#ifndef FLD_SPARK_OBJECT_ENTRY_H
#define FLD_SPARK_OBJECT_ENTRY_H

#include "common.h"

struct EffWorldNode;

/* Object-slot row shared by the field spark controller in both titles. */
typedef struct FldSparkObjectEntry {
    struct EffWorldNode *objectHandle; /* 0x00 */
    s32 sparkSlotIndex; /* 0x04: -1 when the object slot is free. */
    u8 pad8[0x0C]; /* 0x08: only the first word is cleared here. */
} FldSparkObjectEntry;

typedef char FldSparkObjectEntrySizeCheck[
    sizeof(FldSparkObjectEntry) == 0x14 ? 1 : -1];
typedef char FldSparkObjectEntryIndexOffsetCheck[
    (u32)&((FldSparkObjectEntry *)0)->sparkSlotIndex == 0x04 ? 1 : -1];

#endif /* FLD_SPARK_OBJECT_ENTRY_H */
