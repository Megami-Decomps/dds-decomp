#ifndef FLD_SPARK_SLOT_H
#define FLD_SPARK_SLOT_H

#include "common.h"

/* One field-spark record. The paired field systems index 64 records at a
 * 0x30-byte stride. */
typedef struct FldSparkSlot {
    f32 position[4];         /* 0x00 */
    f32 rotation[4];         /* 0x10 */
    s32 hasVectors;          /* 0x20 */
    s32 active;              /* 0x24 */
    s32 reservedObjectSlot;  /* 0x28 */
    s16 objectSlot;           /* 0x2C */
    s16 tickCounter;          /* 0x2E */
} FldSparkSlot;

typedef char FldSparkSlot_size_check[
    (sizeof(FldSparkSlot) == 0x30) ? 1 : -1];
typedef char FldSparkSlot_position_offset_check[
    ((u32)&((FldSparkSlot *)0)->position == 0x00) ? 1 : -1];
typedef char FldSparkSlot_rotation_offset_check[
    ((u32)&((FldSparkSlot *)0)->rotation == 0x10) ? 1 : -1];
typedef char FldSparkSlot_hasVectors_offset_check[
    ((u32)&((FldSparkSlot *)0)->hasVectors == 0x20) ? 1 : -1];
typedef char FldSparkSlot_active_offset_check[
    ((u32)&((FldSparkSlot *)0)->active == 0x24) ? 1 : -1];
typedef char FldSparkSlot_reservedObjectSlot_offset_check[
    ((u32)&((FldSparkSlot *)0)->reservedObjectSlot == 0x28) ? 1 : -1];
typedef char FldSparkSlot_objectSlot_offset_check[
    ((u32)&((FldSparkSlot *)0)->objectSlot == 0x2C) ? 1 : -1];
typedef char FldSparkSlot_tickCounter_offset_check[
    ((u32)&((FldSparkSlot *)0)->tickCounter == 0x2E) ? 1 : -1];

#endif /* FLD_SPARK_SLOT_H */
