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

/* Retained operand payloads are 0x28 bytes in DDS1 and 0x2C in DDS2. */
typedef struct BtlOperandEntry {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    u8 pad14[4];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
#ifdef VERSION_DDS1
    u8 pad24[2];
    u16 flags;
#else
    u8 pad24[4];
    u32 flags;
#endif
} BtlOperandEntry;

/* The command work owns thirteen groups. Their payload counts and strides
 * differ: 64 operands/0xA1C bytes in DDS1, 32 operands/0x59C in DDS2. */
typedef struct BtlOperandGroup {
    u8 count;
#ifdef VERSION_DDS1
    u8 pad01[7];
    s32 kind;
#else
    u8 pad01[3];
    s32 interval;
    u32 unk08;
#endif
    s32 unk0C;
#ifdef VERSION_DDS1
    u8 skipped;
    u8 pad11[3];
    u8 unk14;
    u8 pad15[7];
    BtlOperandEntry entries[64];
#else
    u8 unk10; /* Reset bytewise; some predicates read the +0x10/+0x11 pair. */
    u8 unk11;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 pad16[6];
    BtlOperandEntry entries[32];
#endif
} BtlOperandGroup;

/* Embedded command work at actor +0x20. DDS1 btlInitBattleIndexWork owns
 * thirteen retained groups at +0x60/+0x64 (0x68 bytes); DDS2 uses
 * +0x68/+0x6C (0x70 bytes). Neither record is the whole owning actor. */
typedef struct BattleIndexWork {
    s32 phase;
    s32 skillId;
    s32 reference;
    s32 unk0C;
    s32 unk10;
    BtlUnit *linkedUnit;
    s32 unk18;
    s32 stageValue;
    s32 unk20;
#ifdef VERSION_DDS1
    u8 unk24[9];
#else
    s32 slot;
    s32 adjustedValue;
    s8 resultKind;
#endif
    u8 unk2D;
    u8 unk2E;
    u8 pad2F;
    u16 stage;
    u8 pad32[2];
    s32 parameter;
    s32 wait;
    s32 unk3C;
    BtlIndexList *indices;
    u8 pad44[4];
    u64 unk48;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    u16 flags;
#ifdef VERSION_DDS1
    u8 unk5E;
    u8 pad5F;
#else
    u8 pad5E[2];
    s32 unk60;
    u8 unk64;
    u8 pad65[3];
#endif
    BtlOperandGroup *groups;
    u32 allocationHandle;
} BattleIndexWork;

void btlInitBattleIndexWork(BattleIndexWork *work);
void btlResetIndexWork(BattleIndexWork *work);
void btlReleaseObjectBuffers(BattleIndexWork *work);

/* Battle task link to a unit and task chain (0x170), shared by DDS1/2. */
typedef struct BtlTask {
    s32 state;              /* +0x00 */
    u16 actionNumber;       /* +0x04: "btl:actnum 0" guard and scene-slot repetitions. */
    u8 pad06[2];
    u32 flags;               /* +0x08 */
    u32 options;             /* +0x0C: command selection options. */
#ifdef VERSION_DDS2
    u8 unk_10[8];
#else
    s32 stateTime; /* +0x10: reset by dispatch, advanced by the actor update loop */
    s32 unk14;
#endif
    BtlUnit *unit;           /* +0x18 */
    u8 unk_1C[4];
#ifdef VERSION_DDS1
    BattleIndexWork indexWork; /* +0x20..+0x87 */
    u8 pad88[0xC0];
#else
    s32 result;              /* +0x20 */
    s32 arg;                 /* +0x24 */
    s32 commandReference;    /* +0x28: command 4 resolves this item reference. */
    u8 pad2C[0x24];
    u16 actionStage; /* 0x50: 4 while the gun-change command still owes its effect */
    u8 pad52[2];
    s32 effect; /* 0x54 */
    u8 pad58[8];
    BtlIndexList *targetList; /* Selected unit/ID index list consumed by battle commands. */
    u8 unk_64[0xE4];
#endif
    u32 actions[8];         /* +0x148: opaque queued action slots */
#ifdef VERSION_DDS1
    struct BtlTask *prev;   /* +0x168: set by the 0x170-byte actor constructor */
#else
    u8 pad168[4];
#endif
    struct BtlTask *next;   /* +0x16C */
} BtlTask;


#endif /* BTL_TASK_H */
