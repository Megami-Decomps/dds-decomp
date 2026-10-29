#ifndef FLD_SPARK_OBJECT_ENTRY_H
#define FLD_SPARK_OBJECT_ENTRY_H

#include "common.h"

struct dds3Basic_t;

/* Index into the field-spark slot table. */
typedef s32 FldSparkSlotIndex;
#define FLD_SPARK_SLOT_INDEX_UNBOUND (-1)

/* Object-slot row shared by the field spark controller in both titles. */
typedef struct FldSparkObjectEntry {
    struct dds3Basic_t *objectHandle; /* 0x00 */
    FldSparkSlotIndex sparkSlotIndex; /* 0x04: unbound when the object slot is free. */
    u8 pad8[0x0C]; /* 0x08: only the first word is cleared here. */
} FldSparkObjectEntry;

typedef char FldSparkObjectEntrySizeCheck[
    sizeof(FldSparkObjectEntry) == 0x14 ? 1 : -1];
typedef char FldSparkObjectEntryIndexOffsetCheck[
    (u32)&((FldSparkObjectEntry *)0)->sparkSlotIndex == 0x04 ? 1 : -1];

#endif /* FLD_SPARK_OBJECT_ENTRY_H */
