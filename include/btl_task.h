#ifndef BTL_TASK_H
#define BTL_TASK_H

#include "common.h"

typedef struct BtlUnit BtlUnit;

/* Battle unit model flags pointer at +0x8C (0x90); DDS1/2 battle units. */
typedef struct BtlUnitModel {
    u8 unk_00[0x8C];
    u32 *flags; /* 0x8C */
} BtlUnitModel;

/* Battle task link to a unit and task chain (0x170); four identical DDS1/2 views.
 * Declared before the DDS2 BtlUnit block so a unit that keeps its own
 * struct BtlUnit can still include this header for BtlTask. */
typedef struct BtlTask {
    u8 unk_00[8];
    u32 flags;               /* +0x08 */
    u8 unk_0C[0xC];
    BtlUnit *unit;           /* +0x18 */
    u8 unk_1C[4];
    s32 result;              /* +0x20 */
    s32 arg;                 /* +0x24 */
    u8 unk_28[0x38];
    s32 targetList; /* Selected unit/ID index list consumed by battle commands. */
    u8 unk_64[0x108];
    struct BtlTask *next;   /* +0x16C */
} BtlTask;

#endif /* BTL_TASK_H */
