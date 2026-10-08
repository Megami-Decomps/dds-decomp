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

/* Retained command operands: 0x28 bytes in DDS1, 0x2C in DDS2.
 * The task constructors copy the complete operand after the actor pointer. */
typedef struct BtlOperandEntry {
    s32 hpDelta;
    s32 mpDelta;
    u32 addedStatus;
    u32 removedStatus;
    u32 entryChangeMask;
    s16 entryChange;
    u16 entryChangeFlags;
    s32 entrySelection;
    s32 hpRecovery;
    s32 mpRecovery;
    u16 deferredStatus;
#ifdef VERSION_DDS1
    u16 flags;
#else
    u8 pad26[2];
    u32 flags;
#endif
} BtlOperandEntry;

/* btlAllocTask reserves 0x2C/0x30 argument bytes for these command tasks. */
typedef struct BtlOperandTaskArgs {
    BtlUnit *unit;
    BtlOperandEntry operand;
} BtlOperandTaskArgs;

/* The command work owns thirteen groups. Their payload counts and strides
 * differ: 64 operands/0xA1C bytes in DDS1, 32 operands/0x59C in DDS2. */
typedef struct BtlOperandGroup {
    u8 count;
    u8 pad01[3];
    s32 interval;
#ifdef VERSION_DDS1
    s32 kind;
#else
    u32 kind;
#endif
    s32 parameter;
    /* Dispatch tests both bytes with lhu; reset and population use sb. */
    union {
        u16 inactiveOrStatusChanged;
        struct {
            u8 inactive;
            u8 statusChanged;
        };
    };
    u8 targetSpecialHit;
    u8 sourceSpecialHit;
    u8 reflected; /* Population prints "btl:HANSYA" when this is set. */
    u8 reflectionKind;
    u8 pad16[2];
    s32 reactionCode;
#ifdef VERSION_DDS1
    BtlOperandEntry entries[64];
#else
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
    BtlUnit *companionA;
    BtlUnit *companionB;
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
    u64 ownerId;
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

#ifdef VERSION_DDS1
/* DDS1 btlCreateActionSeq owns one 0x170-byte command actor. */
typedef struct BtlTask {
    s32 state;
    u16 actionNumber;
    u8 pad06[2];
    u32 flags;
    u32 options;
    s32 stateTime;
    s32 unk14;
    BtlUnit *unit;
    u8 unk_1C[4];
    BattleIndexWork indexWork; /* +0x20..+0x87 */
    u8 pad88[0xC0];
    u32 actions[8]; /* +0x148 */
    struct BtlTask *prev; /* +0x168 */
    struct BtlTask *next; /* +0x16C */
} BtlTask;
#endif

#ifdef VERSION_DDS2
/* Queued action slots are queried as complete words and low halfword IDs. */
typedef union BattleActionSlot {
    s32 word;
    s16 actionId;
} BattleActionSlot;

/* DDS2 0x1DCF58 allocates this 0x180-byte command actor. It is not the
 * 0x70-byte scheduler task or the 0x368-byte world unit. */
typedef struct ActionStateLink {
    u32 state;
    u16 actionNumber;
    u8 pad06[2];
    /* fldUpdateSceneGroupTask reads both flag words with ld at 0x1D3C68. */
    union {
        u64 combinedFlags;
        struct {
            u32 pendingFlags;
            u32 flags;
        };
    };
    s32 stateTime;
    s32 completedTurns;
    BtlUnit *unit; /* +0x18 */
    u8 pad1C[4];
    BattleIndexWork indexWork; /* +0x20..+0x8F */
    u16 aiCounter;
    u8 pad92[0xBC];
    s8 lowHpActionHold;
    u8 pad14F;
    BattleActionSlot actions[8]; /* +0x150 */
    s32 lastMode;
    struct ActionStateLink *prev; /* +0x174 */
    struct ActionStateLink *next; /* +0x178 */
    u8 pad17C[4];
} ActionStateLink;
#endif


#endif /* BTL_TASK_H */
