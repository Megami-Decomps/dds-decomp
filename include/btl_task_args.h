#ifndef BTL_TASK_ARGS_H
#define BTL_TASK_ARGS_H

#include "common.h"

struct BtlUnit;

/* The linked-number task allocates the full 0x34-byte variant; the counter
 * task uses the common prefix and its 0x2C-byte counter variant. */
typedef struct BtlLinkedEffectArgs {
    f32 anchorOffset[4]; /* 0x00: initial label anchor minus referencePosition. */
    f32 referencePosition[4]; /* 0x10: saved reference for the update flags. */
    struct BtlUnit *unit; /* 0x20: owner used by both finish callbacks. */
    union {
        struct {
            s32 value; /* 0x24: signed number rendered by the update callback. */
            u32 elapsedTicks; /* 0x28: unsigned frame threshold. */
            u32 color; /* 0x2C */
            u8 kind; /* 0x30 */
            u8 offsetIndex; /* 0x31: indexes the twelve display offsets. */
        } linked;
        struct {
            u32 elapsedTicks; /* 0x24: counter timing and float conversion. */
            u8 kind; /* 0x28 */
            u8 offsetIndex; /* 0x29 */
        } counter;
    } payload;
} BtlLinkedEffectArgs;

typedef char BtlLinkedEffectArgsSizeCheck[
    (sizeof(BtlLinkedEffectArgs) == 0x34) ? 1 : -1];
typedef char BtlLinkedEffectArgsOwnerOffsetCheck[
    ((u32)&((BtlLinkedEffectArgs *)0)->unit == 0x20) ? 1 : -1];
typedef char BtlLinkedEffectArgsPayloadOffsetCheck[
    ((u32)&((BtlLinkedEffectArgs *)0)->payload == 0x24) ? 1 : -1];
typedef char BtlLinkedEffectArgsCounterExtentCheck[
    (sizeof(((BtlLinkedEffectArgs *)0)->payload.counter) == 8) ? 1 : -1];
typedef char BtlLinkedEffectArgsLinkedFieldsOffsetCheck[
    ((u32)&((BtlLinkedEffectArgs *)0)->payload.linked.color == 0x2C &&
     (u32)&((BtlLinkedEffectArgs *)0)->payload.linked.kind == 0x30 &&
     (u32)&((BtlLinkedEffectArgs *)0)->payload.linked.offsetIndex == 0x31) ? 1 : -1];

/* Complete eight-byte actor/amount block owned by the hunt-EP task. */
typedef struct BtlHuntExpArgs {
    struct BtlUnit *actor; /* 0x00 */
    u32 amount;            /* 0x04 */
} BtlHuntExpArgs;
typedef char BtlHuntExpArgsSizeCheck[sizeof(BtlHuntExpArgs) == 8 ? 1 : -1];
typedef char BtlHuntExpArgsAmountOffsetCheck[(u32)&((BtlHuntExpArgs *)0)->amount == 4 ? 1 : -1];

/* The battle reward callbacks receive this complete eight-byte task payload. */
typedef struct BtlRewardPacket {
    struct BtlUnit *actor; /* 0x00 */
    s32 amount;            /* 0x04 */
} BtlRewardPacket;
typedef char BtlRewardPacketSizeCheck[sizeof(BtlRewardPacket) == 8 ? 1 : -1];
typedef char BtlRewardPacketActorOffsetCheck[
    (u32)&((BtlRewardPacket *)0)->actor == 0 ? 1 : -1];
typedef char BtlRewardPacketAmountOffsetCheck[
    (u32)&((BtlRewardPacket *)0)->amount == 4 ? 1 : -1];

/* Return the runtime scheduler task's allocated argument block, or NULL. */
void *btlGetTaskArguments(void *task);

/* Category-stat damage task arguments, written by both title constructors. */
typedef struct BtlCategoryStatDamageArgs {
    struct BtlUnit *unit; /* 0x00 */
    s32 amount;           /* 0x04: HP or MP cost. */
    s32 category;         /* 0x08: indexes datCommandRecords. */
} BtlCategoryStatDamageArgs;
typedef char BtlCategoryStatDamageArgsSizeCheck[
    (sizeof(BtlCategoryStatDamageArgs) == 0x0C) ? 1 : -1];

/* The scene scheduler copies these request records into its argument block. */
typedef struct BtlSceneCounterArgs {
#ifdef VERSION_DDS1
    struct BtlTask *actor;
#else
    struct ActionStateLink *actor;
#endif
    u32 amount;
    u8 mode;
    u8 pad09[3];
} BtlSceneCounterArgs;

typedef struct BtlSceneInsertArgs {
#ifdef VERSION_DDS1
    struct BtlTask *actor;
#else
    struct ActionStateLink *actor;
#endif
    u32 count;
} BtlSceneInsertArgs;

/* The model-blend task allocates and initializes this complete 0x1C-byte packet. */
typedef struct BtlActorModelBlendArgs {
    struct BtlUnit *unit;
    struct BtlUnit *target;
    s32 index;
    s32 previousModelValue;
    s32 value;
    f32 scale;
    u32 stage;
} BtlActorModelBlendArgs;
typedef char BtlActorModelBlendArgsSizeCheck[sizeof(BtlActorModelBlendArgs) == 0x1C ? 1 : -1];

/* The model-change task allocates this 0x1C-byte record in both titles. */
typedef struct BtlModelChangeArgs {
    struct BtlUnit *unit; /* 0x00 */
    u32 resourceKind;     /* 0x04 */
    u32 resourceId;       /* 0x08 */
    s32 delay;            /* 0x0C */
    u32 duration;         /* 0x10 */
    s32 elapsed;          /* 0x14 */
    u8 phase;             /* 0x18 */
    u8 transitionMode;    /* 0x19 */
    u8 opaque_1A[2];      /* 0x1A: allocated but not interpreted by these readers. */
} BtlModelChangeArgs;
typedef char BtlModelChangeArgsSizeCheck[sizeof(BtlModelChangeArgs) == 0x1C ? 1 : -1];
typedef char BtlModelChangeArgsElapsedOffsetCheck[(u32)&((BtlModelChangeArgs *)0)->elapsed == 0x14 ? 1 : -1];
typedef char BtlModelChangeArgsPhaseOffsetCheck[(u32)&((BtlModelChangeArgs *)0)->phase == 0x18 ? 1 : -1];

/* Shared 0x28-byte record used by battle runtime callbacks and task setup. */
typedef struct BtlRuntimeTaskArgs {
    union {
        void *actor;
        s32 value;
    };
    union {
        s32 option;
        struct BtlUnit *unitA; /* Kind 0x2A: first linked camera unit. */
        u16 optionId;
        f32 scale;
    };
    union {
        u32 unk_08; /* Kind 9: indexed-unit-effect value; kind 0x2A: second linked camera unit. */
        struct BtlUnit *unitB;
    };
    union {
        u32 unk_0C;
        struct BtlUnit *unitC; /* Status callback receives a unit pointer in this slot. */
        f32 scale2;
        s8 mode;
    };
    u32 unk_10;
    union {
        u32 unk_14;
        f32 scale14;
    };
    union {
        u32 unk_18;
        struct {
            u8 flag18;
            u8 flag19;
        };
    };
    u32 unk_1C;
    u32 unk_20;
    u32 unk_24;
} BtlRuntimeTaskArgs;
typedef char BtlRuntimeTaskArgsSizeCheck[sizeof(BtlRuntimeTaskArgs) == 0x28 ? 1 : -1];
typedef char BtlRuntimeTaskArgsUnitAOffsetCheck[(u32)&((BtlRuntimeTaskArgs *)0)->unitA == 0x4 ? 1 : -1];
typedef char BtlRuntimeTaskArgsUnitBOffsetCheck[(u32)&((BtlRuntimeTaskArgs *)0)->unitB == 0x8 ? 1 : -1];
typedef char BtlRuntimeTaskArgsUnitCOffsetCheck[(u32)&((BtlRuntimeTaskArgs *)0)->unitC == 0xC ? 1 : -1];

#endif /* BTL_TASK_ARGS_H */
