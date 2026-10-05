#ifndef BTL_TASK_H
#define BTL_TASK_H

#include "common.h"

typedef struct BtlIndexList {
    s32 capacity;       /* 0x00: allocated entry count */
    s32 count;          /* 0x04: live entry count */
    void **entries;     /* 0x08: points just past this header */
} BtlIndexList;

BtlIndexList *btlAllocateIndexList(s32 capacity);
void btlFreeIndexList(BtlIndexList *list);
void btlAppendIndexListEntry(BtlIndexList *list, void *entry);
void btlClearIndexList(BtlIndexList *list);
u32 btlGetIndexListCount(BtlIndexList *list);
void *btlGetIndexListEntry(BtlIndexList *list, s32 index);
void btlCopyIndexList(BtlIndexList *destination, BtlIndexList *source);
void btlSwapIndexListEntries(BtlIndexList *list, s32 first, s32 second);
u32 btlFindListIndex(BtlIndexList *list, void *entry);

/* The battle AI task's 0xA4-byte user-data allocation in both games. */
typedef struct SceneAiWork {
    s32 state;              /* 0x00 */
    u32 result;             /* 0x04 */
    s32 entry;              /* 0x08 */
    BtlIndexList *listA;     /* 0x0C */
    BtlIndexList *listB;     /* 0x10 */
    s32 source;             /* 0x14 */
    u8 pad18[0x8C];
} SceneAiWork;

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
    BtlIndexList *targetList; /* Selected unit/ID index list consumed by battle commands. */
    u8 unk_64[0x108];
    struct BtlTask *next;   /* +0x16C */
} BtlTask;

#endif /* BTL_TASK_H */
