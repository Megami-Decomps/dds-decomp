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
    /* The selection helper indexes a 3-by-3 target-mask table with this cursor. */
    u16 cursorRow;          /* 0x18 */
    u16 cursorColumn;       /* 0x1A */
    s8 animationPhase;       /* 0x1C */
    u8 pad1D[3];
    s32 panelFade[4];        /* 0x20: signed panel fades. */
    u8 pad30[0x10];
    s32 animationCounter;   /* 0x40 */
    u8 pad44[0xC];
    f32 panelScale[4];      /* 0x50: sprite size percentages, not XY positions. */
    s8 rowPhase[3];          /* 0x60: three independent row-animation states */
    u8 pad63[5];
    f32 rowScale[3];        /* 0x68: row size in percent, initialized to 80 */
    s32 rowPosition[3][2];   /* 0x74: XY outputs from the direction-step table */
    s32 rowStep[3];          /* 0x8C: bounded to 0..5 during row animation */
    s32 rowFade[3];          /* 0x98: initialized to 128 for each row */
} SceneAiWork;

typedef struct BtlUnit BtlUnit;

/* Battle task link to a unit and task chain (0x170), shared by DDS1/2. */
typedef struct BtlTask {
    s32 state;              /* +0x00 */
    u8 pad04[4];
    u32 flags;               /* +0x08 */
    u8 unk_0C[0xC];
    BtlUnit *unit;           /* +0x18 */
    u8 unk_1C[4];
    s32 result;              /* +0x20 */
    s32 arg;                 /* +0x24 */
#ifdef VERSION_DDS1
    u8 unk_28[0x10];
    s32 value38; /* 0x38: count retained by the HARI2 command (0xD5). */
    u8 pad3C[0x14];
    u16 actionStage; /* 0x50: 4 while the gun-change command still owes its effect */
    u8 pad52[2];
    s32 effect; /* 0x54 */
    u8 pad58[8];
#else
    u8 unk_28[0x38];
#endif
    BtlIndexList *targetList; /* Selected unit/ID index list consumed by battle commands. */
    u8 unk_64[0xE4];
    u32 actions[8];         /* +0x148: opaque queued action slots */
    u8 pad168[4];
    struct BtlTask *next;   /* +0x16C */
} BtlTask;


#endif /* BTL_TASK_H */
