#include "common.h"
#include "btl.h"


extern s32 btlGetSideIndexedActorStatusTable(s32, s32);
extern void effObjFetchInnerFirstVec(s32);
extern s32 sdfLoadMapRecordPositionVector(s32, s32);
extern void func_0011E280(s32, f32, f32, f32, f32);

extern s32 func_001A17F0();
extern s8 btlHistoryCounter;
extern s32 btlActionScratchWork;
extern s32 btlIsLowHpActionReady(s32, s32);
extern s32 sdfAllocAndClearQuadwords(s32);
extern void sdfReleaseChipBlock(s32);

extern u32 D_003BB874;

extern void func_00202668(s32, s32);
extern void func_00202F90(s32, s32);
extern void btlSelectLowestHealthRateTarget(s32, s32);
extern void func_00203098();
extern void btlSelectLowestRankTarget(s32, s32);
extern void func_00203BA8(s32, s32);
extern void btlSelectTargetsPassingCheck();
extern void btlSelectTargetsByActionMask();
extern void btlSelectTargetsWithoutActionMask();
extern void func_002035E0();
extern void func_00204008();
extern void func_00204028();
extern void btlAppendSelfAfterTargetScan();
extern void func_00203CA8();
extern void btlAppendEffectActorToCommandIndices(s32, s32);
extern void btlSelectTargetsBlockingElement();

typedef struct EffCounterOwner {
    u8 pad00[0x20];
    BtlUnit *unit; /* 0x20: battle unit retained by the effect argument block */
} EffCounterOwner;


extern s32 (*D_00360D10[])(s32, u32);
extern s32 btlWaitEffectTask();
extern void func_001FDA78();
extern s32 btlPollEffectWaitTask();
extern s32 func_001ADB30();
extern void btlReplaceDialogTasksAndQueueMessage(s32, s32);
extern void func_0019C590(s32, s32, s32, s32);
extern void func_003003F0(const char *fmt, ...);
extern s8 D_00324510[];
INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FC7D8);

void func_001FC990(void) {
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FC998);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FCAC0);

extern u32 btlRollAiBucket(void);

/* Accepts a task's unit only when status bit 5 is set and task bit 6 is clear.
 * If bit 5 is the sole low status bit, the final check uses an external value. */
s32 func_001FCB50(BtlTask *task) {
    s32 result = 0;
    BtlUnit *unit = task->unit;
    u16 flags;

    if (task->flags & 0x40) {
        return result;
    }
    flags = unit->conditionFlags;
    if (!(flags & 0x20)) {
        return result;
    }
    if ((flags & 0x7FFF) != 0x20) {
        return 1;
    }
    return btlRollAiBucket() < 0x46;
}

INCLUDE_RODATA(const s32, "game/code_001FC7D8", D_003A57E0);

INCLUDE_RODATA(const s32, "game/code_001FC7D8", D_003A57F0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FCBA0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FCFB8);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FD170);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FD3C0);


/* Adds one to the base unless bit 9 of the owner's flags is set. */

s32 effOffsetIfOwnerFlagClear(BtlUnit *owner, s32 base) {
    return base + ((((s32)owner->flags >> 9) ^ 1U) & 1);
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FD5C8);

void effDecrementFirstCountdown(EffCounterOwner *owner) {
    u8 remaining;
    BtlUnit *unit;

    unit = owner->unit;
    remaining = unit->firstCountdown;
    if (remaining != 0) {
        unit->firstCountdown = remaining + 0xff;
    }
}

typedef struct BtlObjLink {
    void *owner; /* 0x00: effect owner */
    s32 arg; /* Some effect variants write/read only the low halfword. */
    s32 elapsedTicks; /* 0x08: elapsed callback updates */
} BtlObjLink;

typedef struct BtlEffObj {
    u8 kind;          /* 0x00 */
    u8 pad01[0xF];
    u8 unk10;         /* 0x10 */
    u8 pad11[0xF];
    s16 id;           /* 0x20 */
    u8 pad22[2];
    u16 flags;        /* 0x24 */
    u8 pad26[0x1A];
    u64 ownerData;    /* 0x40 */
    u8 pad48[4];
    void (*update)(); /* 0x4C */
    void (*destroy)(); /* 0x50 */
} BtlEffObj;

extern BtlEffObj *btlAllocTask(s32);
extern BtlObjLink *btlGetTaskArguments(BtlEffObj *);
extern void func_001FD5C8();

/* Link block hung off a freshly created effect object. */
typedef struct BtlEffLinkEx {
    u8 pad00[0x20];
    BtlUnit *owner; /* 0x20 */
    s32 arg;            /* 0x24 */
    s32 unk28;          /* 0x28 */
    u8 pad2C[4];
    u8 kind;            /* 0x30 */
} BtlEffLinkEx;

BtlEffObj *btlCreateLinkedEffectTask(BtlUnit *owner, s32 arg, u8 kind) {
    BtlEffObj *obj = btlAllocTask(0x34);
    BtlEffLinkEx *link;

    obj->kind = 1;
    obj->unk10 = 0;
    switch (kind) {
    case 0:
        obj->id = 0x39;
        break;
    case 1:
        obj->id = 0x3A;
        break;
    }
    obj->flags |= 2;
    obj->ownerData = owner->identity;
    obj->update = func_001FD5C8;
    obj->destroy = effDecrementFirstCountdown;
    link = (BtlEffLinkEx *)btlGetTaskArguments(obj);
    link->kind = kind;
    link->owner = owner;
    link->arg = arg;
    link->unk28 = 0;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FDA78);

void effDecrementSecondCountdown(EffCounterOwner *owner) {
    u8 remaining;
    BtlUnit *unit;

    unit = owner->unit;
    remaining = unit->secondCountdown;
    if (remaining != 0) {
        unit->secondCountdown = remaining + 0xff;
    }
}


typedef struct BtlExtendedLink {
    u8 pad00[0x20];
    BtlUnit *owner; /* 0x20 */
    s32 state;          /* 0x24 */
    u8 parameter;       /* 0x28 */
} BtlExtendedLink;

BtlEffObj *btlCreateEffectCounterTask(BtlUnit *owner, s32 arg) {
    BtlEffObj *obj = btlAllocTask(0x2C);
    BtlExtendedLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x3B;
    obj->ownerData = owner->identity;
    obj->update = func_001FDA78;
    obj->destroy = effDecrementSecondCountdown;
    link = (BtlExtendedLink *)btlGetTaskArguments(obj);
    link->parameter = arg;
    link->owner = owner;
    link->state = 0;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FDF98);

extern void func_001FDF98();

BtlEffObj *btlCreateEffObjD(BtlUnit *owner, s32 arg) {
    BtlEffObj *obj = btlAllocTask(0xC);
    BtlObjLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x40;
    obj->ownerData = owner->identity;
    obj->update = func_001FDF98;
    link = btlGetTaskArguments(obj);
    link->owner = owner;
    link->arg = arg;
    link->elapsedTicks = 0;
    return obj;
}

extern s32 D_003BAA84;
extern void func_001AD970(s32);
extern s32 func_001AD928(void);

s32 btlPollTimedTaskLink(BtlObjLink *link) {
    func_001A17F0();
    if (link->elapsedTicks == 0) {
        func_001AD970(D_003BAA84 + link->arg * 0x19);
    }
    if (func_001AD928() == 0 || (u32)link->elapsedTicks >= 0x1E) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

extern s32 btlPollTimedTaskLink();

BtlEffObj *btlCreateOwnerLinkedTimedTask(BtlUnit *owner, s32 arg) {
    BtlEffObj *obj = btlAllocTask(0xC);
    BtlObjLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x41;
    obj->ownerData = owner->identity;
    obj->update = btlPollTimedTaskLink;
    link = btlGetTaskArguments(obj);
    link->owner = owner;
    link->arg = arg;
    link->elapsedTicks = 0;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE228);

extern void func_001FE228();

/* Preserve the owner's effect data in the new task. */
BtlEffObj *btlCreateEffObjA(BtlUnit *owner, s32 arg) {
    BtlEffObj *obj = btlAllocTask(0xC);
    BtlObjLink *link;

    obj->kind = 1;
    obj->id = 0x42;
    obj->flags |= 2;
    obj->unk10 = 0;
    if (owner != NULL) {
        obj->ownerData = owner->identity;
    }
    obj->update = func_001FE228;
    link = btlGetTaskArguments(obj);
    link->owner = owner;
    link->arg = arg;
    link->elapsedTicks = 0;
    return obj;
}


typedef struct BtlJyokyoState {
    u8 pad00[0x498];
    s32 dialogId;          /* 0x498 */
    s32 alternateDialogId; /* 0x49C */
    s32 thirdDialogId;     /* 0x4A0 */
} BtlJyokyoState;

s32 btlJyokyoEffectUpdate(BtlObjLink *link) {
    BtlJyokyoState *state = (BtlJyokyoState *)func_001A17F0();
    BtlUnit *owner = link->owner;

    if (link->elapsedTicks == 0) {
        if (owner != NULL) {
            func_0019C590(state->dialogId, 0, owner->mode, (owner->statBits & 0x20) ? 0xE : 0xF);
        }
        func_003003F0("JYOKYO ID : %d\n", state->dialogId);
        btlReplaceDialogTasksAndQueueMessage(state->dialogId, link->arg);
    }
    if (func_001ADB30() == 0 || (u32)link->elapsedTicks >= 0x2D) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

BtlEffObj *btlCreateEffObjB(BtlUnit *owner, s32 messageId) {
    BtlEffObj *obj = btlAllocTask(0xC);
    BtlObjLink *link;

    obj->kind = 1;
    obj->id = 0x3C;
    obj->flags |= 2;
    obj->unk10 = 0;
    if (owner != NULL) {
        obj->ownerData = owner->identity;
    }
    obj->update = btlJyokyoEffectUpdate;
    link = btlGetTaskArguments(obj);
    link->owner = owner;
    link->arg = messageId;
    link->elapsedTicks = 0;
    return obj;
}

s32 func_001FE500(BtlObjLink *link) {
    s32 battleState = func_001A17F0();
    BtlUnit *owner = link->owner;

    if (owner != 0 && !(owner->conditionFlags & 1)) {
        return 1;
    }
    if (link->elapsedTicks == 0) {
        if (owner != 0) {
            func_0019C590(((BtlJyokyoState *)battleState)->alternateDialogId, 0, owner->mode,
                          (owner->statBits & 0x20) ? 0xE : 0xF);
        }
        btlReplaceDialogTasksAndQueueMessage(((BtlJyokyoState *)battleState)->alternateDialogId, link->arg);
    }
    if (func_001ADB30() == 0 || (u32)link->elapsedTicks >= 0x2D) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

BtlEffObj *btlCreateEffObjC(BtlUnit *owner, s32 messageId) {
    BtlEffObj *obj = btlAllocTask(0xC);
    BtlObjLink *link;

    obj->kind = 1;
    obj->id = 0x3D;
    obj->flags |= 2;
    obj->unk10 = 0;
    if (owner != NULL) {
        obj->ownerData = owner->identity;
    }
    obj->update = func_001FE500;
    link = btlGetTaskArguments(obj);
    link->owner = owner;
    link->arg = messageId;
    link->elapsedTicks = 0;
    return obj;
}

u32 func_001FE658(void) {
    return 1;
}

BtlEffObj *btlCreateEffectTask3E(BtlUnit *owner, u16 arg) {
    BtlEffObj *obj = btlAllocTask(0xC);
    BtlObjLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x3E;
    obj->ownerData = owner->identity;
    obj->update = func_001FE658;
    link = btlGetTaskArguments(obj);
    link->owner = owner;
    *(u16 *)&link->arg = arg;
    link->elapsedTicks = 0;
    return obj;
}

s32 btlPollTimedPresentationTask(BtlObjLink *link) {
    BtlJyokyoState *state = (BtlJyokyoState *)func_001A17F0();
    BtlUnit *owner = link->owner;

    if (link->elapsedTicks == 0) {
        if (owner != NULL) {
            func_0019C590(state->thirdDialogId, 0, owner->mode,
                          (owner->statBits & 0x20) ? 1 : 2);
        }
        btlReplaceDialogTasksAndQueueMessage(state->thirdDialogId, link->arg);
    }
    if (func_001ADB30() == 0 || (u32)link->elapsedTicks >= 0x2D) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

extern s32 btlPollTimedPresentationTask();

BtlEffObj *btlCreateOwnerLinkedTimedPresentation(BtlUnit *owner, s32 messageId) {
    BtlEffObj *obj = btlAllocTask(0xC);
    BtlObjLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x3F;
    obj->ownerData = owner->identity;
    obj->update = btlPollTimedPresentationTask;
    link = btlGetTaskArguments(obj);
    link->owner = owner;
    link->arg = messageId;
    link->elapsedTicks = 0;
    return obj;
}

s32 btlPollEffectWaitTask(BtlObjLink *link) {
    s32 battleState = func_001A17F0();

    if (link->elapsedTicks == 0) {
        if (link->owner != 0) {
            func_0019C590(((BtlJyokyoState *)battleState)->dialogId, 0, *(u16 *)&link->arg, 0xD);
        }
        btlReplaceDialogTasksAndQueueMessage(((BtlJyokyoState *)battleState)->dialogId, 0x75);
    }
    if (func_001ADB30() == 0 || (u32)link->elapsedTicks >= 0x2D) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

BtlEffObj *btlCreateEffectWaitTask(BtlUnit *owner, u16 mode) {
    BtlEffObj *obj = btlAllocTask(0xC);
    BtlObjLink *link;

    obj->kind = 1;
    obj->id = 0x43;
    obj->flags |= 2;
    obj->unk10 = 0;
    if (owner != NULL) {
        obj->ownerData = owner->identity;
    }
    obj->update = btlPollEffectWaitTask;
    link = btlGetTaskArguments(obj);
    link->owner = owner;
    *(u16 *)&link->arg = mode;
    link->elapsedTicks = 0;
    return obj;
}

typedef struct BtlWaitTask {
    s32 value;
    u32 ticks;
} BtlWaitTask;

/* Waits for the task startup delay, then finishes when its two actor slots are clear. */

s32 btlWaitEffectTask(BtlWaitTask *task) {
    if (task->ticks == 0) {
        func_001AD230(task->value, 0);
        func_001BCB88(0, 8);
    }
    if (task->ticks >= 0x11) {
        if (btlGetRegisteredTaskValueOrDefault() == 1) {
            if (D_00324510[0x21] < 0 || D_00324510[0x23] < 0) {
                func_001BCB88(1, 8);
                func_001AD428();
                return 1;
            }
        }
    } else {
        task->ticks += 1;
    }
    return 0;
}

BtlEffObj *btlCreateEffectTask44(BtlUnit *owner) {
    BtlEffObj *obj = btlAllocTask(8);
    BtlObjLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x44;
    obj->ownerData = owner->identity;
    obj->update = btlWaitEffectTask;
    link = btlGetTaskArguments(obj);
    link->owner = owner;
    link->arg = 0;
    return obj;
}

u32 btlNextScaledRandom(u32 limit) {
    D_003BB874 = D_003BB874 * 0x41c64e6d + 0x3039;
    return (D_003BB874 >> 0x10) * (limit & 0xffff) >> 0x10;
}

extern u32 btlNextScaledRandom(u32 limit);

s32 btlRandomInclusiveRange(s32 a0, s32 a1) {
    s32 lo = a1;
    s32 hi = a0;

    if (lo < hi) {
        s32 t = lo;
        lo = hi;
        hi = t;
    }
    lo = lo - hi + 1;
    return (s32)btlNextScaledRandom(0xFFFF) % lo + hi;
}

s32 btlAllocAndCheck(s32 object) {
    s32 allocation = sdfAllocAndClearQuadwords(0x10);
    s32 actor = (s32)((BtlTask *)object)->unit;

    btlActionScratchWork = allocation;
    *(s32 *)allocation = object;
    if (btlIsLowHpActionReady(actor, 0) != 0) {
        sdfReleaseChipBlock(btlActionScratchWork);
        return 1;
    }
    sdfReleaseChipBlock(btlActionScratchWork);
    return 0;
}

u32 btlAssignTaskResultAndArgument(s32 task) {
    ((BtlTask *)task)->result = 0xb;
    ((BtlTask *)task)->arg = 0xc2;
    return 1;
}

extern void btlShiftActorStateHistory(u8 *work, s8 flag);

typedef struct BtlHistObj {
    u8 pad00[0x146];
    s8 counter; /* 0x146 */
} BtlHistObj;

void btlAdvanceHistoryCounter(BtlHistObj *obj) {
    obj->counter++;
    obj->counter = obj->counter <= 0 ? 0 : obj->counter >= 0x21 ? 0x20 : obj->counter;
    btlShiftActorStateHistory((u8 *)obj, 0);
    btlHistoryCounter++;
    btlHistoryCounter = btlHistoryCounter <= 0 ? 0 : btlHistoryCounter >= 0x21 ? 0x20 : btlHistoryCounter;
}

void btlClearNodeFlags(void) {
    s32 node = *(s32 *)(func_001A17F0() + 0x224);

    if (node != NULL) {
        do {
            if (((BtlTask *)node)->unit != NULL) {
                ((BtlHistObj *)node)->counter = 0;
            }
            node = (s32)((BtlTask *)node)->next;
        } while (node != NULL);
    }
    btlHistoryCounter = 0;
}

extern s32 btlDispatchPackedEffectAction(s32 context, u32 packedAction);

s32 btlDispatchPackedActionWithScratch(s32 context, BtlUnit *owner, s32 mask) {
    s32 *work = (s32 *)sdfAllocAndClearQuadwords(0x10);
    s32 result;

    btlActionScratchWork = (s32)work;
    work[1] = owner->mode;
    work[0] = context;
    result = btlDispatchPackedEffectAction((s32)owner, mask);
    sdfReleaseChipBlock(btlActionScratchWork);
    return result;
}

/* Top ten bits select the callback; the lower 22 bits are its argument. */
s32 btlDispatchPackedEffectAction(s32 context, u32 packedAction) {
    u32 type = packedAction >> 22;
    s32 result = 0;

    packedAction &= 0x3FFFFF;
    if (type != 0) {
        result = D_00360D10[type](context, packedAction) != 0;
    }
    return result;
}

typedef struct AiSlot {
    u8 weight;
    u8 pad1;
    u16 actionId;
    u32 actionArg;
} AiSlot;

/* Per-species AI table (0x15C bytes each): five rows of five weighted slots. */
typedef struct AiSpecies {
    u8 pad00[0x40];
    AiSlot slot[25];
    u8 pad108[0x54];
} AiSpecies;

extern AiSpecies *D_003BAA24;
extern void func_001FF560(BtlUnit *unit, u16 species, s32 *row, s32 arg);
extern u32 btlPickWeightedAiSlot();
extern s32 btlRunAiAction();

/* Choose a row and a weighted slot of the unit's species AI table and run that action. */
s32 func_001FED20(BtlTask *task) {
    s32 *work = (s32 *)sdfAllocAndClearQuadwords(0x10);
    BtlUnit *unit;
    u16 species;
    s32 row;
    s32 index;

    unit = task->unit;
    btlActionScratchWork = (s32)work;
    species = unit->mode;
    work[0] = (s32)task;
    work[1] = species;
    func_001FF560(unit, species, &row, 0);
    index = btlPickWeightedAiSlot(unit, species, row);
    btlRunAiAction(task, D_003BAA24[species].slot[row * 5 + index].actionId,
                   D_003BAA24[species].slot[row * 5 + index].actionArg);
    sdfReleaseChipBlock(btlActionScratchWork);
    return 1;
}

void btlShiftActorStateHistory(u8 *work, s8 flag) {
    s32 *slot;
    s32 i;

    for (i = 6, slot = (s32 *)(work + 0x164); i >= 0; i--, slot--) {
        *slot = slot[-1];
    }
    if (flag == 0) {
        *(s32 *)(work + 0x148) = *(s32 *)(work + 0x24);
    } else {
        *(s32 *)(work + 0x148) = *(s16 *)(*(u8 **)(work + 0x18) + 0x2B0);
    }
}

void btlCmdWithArgA(s32 context) {
    func_00202668(context, 0);
}

void btlCmdWithArgB(s32 context) {
    func_00202F90(context, 0);
}

void btlCmdWithArgC(s32 context) {
    btlSelectLowestHealthRateTarget(context, 0);
}

void btlCmdSimpleA(void) {
    func_00203098();
}

void btlCmdWithArgD(s32 context) {
    btlSelectLowestRankTarget(context, 0);
}

void btlCmdWithArgE(s32 context) {
    func_00203BA8(context, 0);
}

void btlCmdSimpleB(void) {
    btlSelectTargetsPassingCheck();
}

void btlCmdSimpleC(void) {
    btlSelectTargetsByActionMask();
}

void btlCmdSimpleD(void) {
    btlSelectTargetsWithoutActionMask();
}

void btlCmdSimpleE(void) {
    func_002035E0();
}

void btlCmdSimpleF(void) {
    func_00204008();
}

void btlCmdSimpleG(void) {
    func_00204028();
}

void btlCmdSimpleH(void) {
    btlAppendSelfAfterTargetScan();
}

void btlCmdSimpleI(void) {
    func_00203CA8();
}

void btlCmdWithArgF(s32 context) {
    btlAppendEffectActorToCommandIndices(context, 0);
}

void btlCmdSimpleJ(void) {
    btlSelectTargetsBlockingElement();
}

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB840);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB848);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB850);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB858);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB860);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB868);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", btlHistoryCounter);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB874);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB878);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", btlActionScratchWork);

