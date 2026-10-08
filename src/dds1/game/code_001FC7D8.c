#include "common.h"
#include "btl.h"
#include "btl_state.h"
#include "btl_task_args.h"
#include "pcp_vu0.h"
#include "dat_command.h"

struct SdfModel;

extern s32 btlGetSideIndexedActorStatusTable(s32, s32);
extern void effObjFetchInnerFirstVec(s32);
extern s32 sdfLoadMapRecordPositionVector(struct SdfModel *, s32);
extern void func_0011E280(s32, f32, f32, f32, f32);

extern s32 btlGetRuntime();
extern s8 btlHistoryCounter;
extern s32 btlActionScratchWork;
extern s32 btlIsLowHpActionReady(s32, s32);
extern s32 sdfAllocAndClearQuadwords(s32);
extern void sdfReleaseChipBlock(s32);

extern u32 btlRandomState;

extern void func_00202668(s32, s32);
extern void func_00202F90(s32, s32);
extern void btlSelectLowestHealthRateTarget(s32, s32);
extern void btlSelectLowestHealthElementBlockTarget();
extern void btlSelectLowestRankTarget(s32, s32);
extern void func_00203BA8(s32, s32);
extern void btlSelectTargetsPassingCheck();
extern void btlSelectTargetsByActionMask();
extern void btlSelectTargetsWithoutActionMask();
extern void btlSelectTargetsByMode();
extern void func_00204008();
extern void func_00204028();
extern void btlAppendSelfAfterTargetScan();
extern void btlSelectTargetsExcludingActorUnit();
extern void btlAppendEffectActorToCommandIndices(s32, s32);
extern void btlSelectTargetsBlockingElement();

/* Resource-name record consumed by the overwrite prompt, not an effect task.
 * The adjacent name-record allocator reserves 0x38 bytes in both games. */
typedef struct BtlResourceNameRecord {
    s32 x;
    s32 y;
    u32 unk8;
    s32 selection;
    s32 nameLength;
    u32 unk14;
    s32 promptState;
    char name[0x1C]; /* Suffix at 0x1C; editable name starts at 0x21. */
} BtlResourceNameRecord;

/* Linked-effect arguments share an owner and carry task-specific timing data.
 * The linked-number task allocates 0x34 bytes; the counter task uses 0x2C. */
/* The ASM transfers two qwords, but the argument type retains natural alignment:
 * the existing allocation sizes are 0x34 and 0x2C, not rounded-up vector structs. */
typedef struct BtlLinkedEffectArgs {
    f32 anchorOffset[4]; /* Initial label anchor minus referencePosition. */
    f32 referencePosition[4]; /* Saved reference; the update flags can select a live one. */
    BtlUnit *unit; /* 0x20: also used by both destruction callbacks */
    union {
        struct {
            s32 value; /* 0x24: signed number rendered by the update callback */
            s32 elapsedTicks; /* 0x28 */
            u32 color; /* 0x2C */
            u8 kind; /* 0x30 */
            u8 offsetIndex; /* 0x31: indexes the twelve display offsets */
        } linked;
        struct {
            s32 elapsedTicks; /* 0x24 */
            u8 kind; /* 0x28 */
            u8 offsetIndex; /* 0x29 */
        } counter;
    } payload;
} BtlLinkedEffectArgs;


extern s32 (*btlPackedEffectHandlers[])(s32, u32);
extern s32 btlWaitEffectTask();
extern s32 func_001FDA78();
extern s32 btlPollEffectWaitTask();
extern s32 btlHasRegisteredAphNamePanelTask();
extern void btlReplaceDialogTasksAndQueueMessage(s32, s32);
extern void itfMesSetTextSlotFromValue(s32, s32, s32, s32);
extern void func_003003F0(const char *fmt, ...);
extern s8 D_00324510[];
/* Name-record overwrite prompt: uses coordinates, selection, state and both text slices. */
INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FC7D8);

void func_001FC990(void) {
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FC998);


INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FCAC0);

extern u32 btlRollAiBucket(void);

/* Accepts a task's unit only when status bit 5 is set and task bit 6 is clear.
 * If bit 5 is the sole low status bit, the final check uses an external value. */
s32 btlAiCheckStatusRollEligibility(BtlTask *task) {
    s32 result = 0;
    BtlUnit *unit = task->unit;
    u16 flags;

    if (task->flags & 0x40) {
        return result;
    }
    flags = unit->partyRecord.status;
    if (!(flags & 0x20)) {
        return result;
    }
    if ((flags & 0x7FFF) != 0x20) {
        return 1;
    }
    return btlRollAiBucket() < 0x46;
}

extern s32 btlSetActorEffectParameter(u8 *object, s32 index);
extern void btlUnitGetMuzzlePosVU(BtlUnit *unit);
extern f32 btlUnitGetTopY(BtlUnit *unit);
extern s32 btlProjectForwardPositionToPackedScreen(s32 screen[4]);
extern void btlBossDebugPrintf(const char *format, ...);

/* vu0 routine: choose an on-screen HP/MP label anchor, leaving it in vf10. */
INCLUDE_RODATA(const s32, "game/code_001FC7D8", D_003A57E0);

INCLUDE_RODATA(const s32, "game/code_001FC7D8", D_003A57F0);

void func_001FCBA0(BtlUnit *unit) {
    s32 screen[4] __attribute__((aligned(16)));
    f32 baseline[4];
    f32 candidate[4];
    f32 distance;
    s8 visible;
    s32 x;
    s32 y;

    if (btlSetActorEffectParameter((u8 *)unit, 2) != 0) {
        if (unit->stateFlags & 0x80) {
            return;
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, baseline);
    } else {
        btlUnitGetMuzzlePosVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, baseline);
        baseline[1] = -btlUnitGetTopY(unit);
    }
    if (unit->stateFlags & 0x80) {
        VU0_LOAD_VF(vf10, baseline);
        return;
    }
    VU0_LOAD_VF(vf10, baseline);
    visible = btlProjectForwardPositionToPackedScreen(screen);
    x = screen[0] >> 4;
    y = screen[1] >> 3;
    if (!visible || x < 12 || x >= 501 || y < 48 || y >= 324) {
        if (btlSetActorEffectParameter((u8 *)unit, 1) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, candidate);
            visible = btlProjectForwardPositionToPackedScreen(screen);
            x = screen[0] >> 4;
            y = screen[1] >> 3;
            if (visible == 1 && x >= 12 && x < 501 && y >= 48 && y < 324) {
                btlBossDebugPrintf("btl:hpmp clip 1shot\n");
                VU0_LOAD_VF(vf10, candidate);
                return;
            }
        }
        btlUnitGetMuzzlePosVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, candidate);
        VU0_LOAD_VF(vf11, baseline);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(distance);
        if (distance < unit->reach * unit->scale * 4.0f) {
            candidate[0] = baseline[0];
            candidate[2] = baseline[2];
            candidate[1] -= unit->height * unit->scale * 0.25f;
            VU0_LOAD_VF(vf10, candidate);
            visible = btlProjectForwardPositionToPackedScreen(screen);
            x = screen[0] >> 4;
            y = screen[1] >> 3;
            if (visible == 1 && x >= 12 && x < 501 && y >= 48 && y < 324) {
                btlBossDebugPrintf("btl:hpmp clip height 3/4\n");
                VU0_LOAD_VF(vf10, candidate);
                return;
            }
        }
        btlUnitGetMuzzlePosVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, candidate);
        visible = btlProjectForwardPositionToPackedScreen(screen);
        x = screen[0] >> 4;
        y = screen[1] >> 3;
        if (visible == 1 && x >= 12 && x < 501 && y >= 48 && y < 324) {
            btlBossDebugPrintf("btl:hpmp clip cylinder center\n");
            VU0_LOAD_VF(vf10, candidate);
            return;
        }
        candidate[1] += unit->height * unit->scale * 0.5f;
        VU0_LOAD_VF(vf10, candidate);
        visible = btlProjectForwardPositionToPackedScreen(screen);
        x = screen[0] >> 4;
        y = screen[1] >> 3;
        if (visible == 1 && x >= 12 && x < 501 && y >= 48 && y < 324) {
            btlBossDebugPrintf("btl:hpmp clip cylinder bottom\n");
            VU0_LOAD_VF(vf10, candidate);
            return;
        }
        btlUnitGetMuzzlePosVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, candidate);
        candidate[1] -= unit->height * unit->scale * 0.5f;
        VU0_LOAD_VF(vf10, candidate);
        visible = btlProjectForwardPositionToPackedScreen(screen);
        x = screen[0] >> 4;
        y = screen[1] >> 3;
        if (visible == 1 && x >= 12 && x < 501 && y >= 48 && y < 324) {
            btlBossDebugPrintf("btl:hpmp clip cylinder top\n");
            VU0_LOAD_VF(vf10, candidate);
            return;
        }
    }
    VU0_LOAD_VF(vf10, baseline);
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FCFB8);

extern s32 effOffsetIfOwnerFlagClear(BtlUnit *, s32);
s32 func_001FD170(BtlTask *task) {
    BtlUnit *unit = task->unit;
    s32 status = unit->partyRecord.status & 0x7FFF;

    switch (status) {
    case 0x2000:
        switch (task->indexWork.phase) {
        case 1:
            return effOffsetIfOwnerFlagClear(unit, 0x78);
        case 2:
        case 3:
        case 7:
        case 8:
            if (task->indexWork.skillId == 0xE0) {
                effOffsetIfOwnerFlagClear(unit, 0x6E);
            }
            /* Retail discards the auxiliary ID before resolving the common ID. */
        default:
            return effOffsetIfOwnerFlagClear(unit, 2);
        }
    case 0x200:
        if (task->indexWork.phase != 1) {
            return effOffsetIfOwnerFlagClear(unit, 0x7A);
        }
        return effOffsetIfOwnerFlagClear(unit, 0x78);
    case 0x40:
        return effOffsetIfOwnerFlagClear(unit, 6);
    case 0x20:
        switch (task->indexWork.phase) {
        case 13:
            return effOffsetIfOwnerFlagClear(unit, 0x72);
        case 14:
            return effOffsetIfOwnerFlagClear(unit, 0x70);
        case 2:
        case 3:
        case 7:
        case 8:
            if (task->indexWork.skillId == 0xE0) {
                return effOffsetIfOwnerFlagClear(unit, 0x6E);
            }
            break;
        case 11:
            return effOffsetIfOwnerFlagClear(unit, 0x76);
        }
        return effOffsetIfOwnerFlagClear(unit, 0x6C);
    case 8:
        return effOffsetIfOwnerFlagClear(unit, 8);
    case 0x800:
        return effOffsetIfOwnerFlagClear(unit, 0x7C);
    case 1:
        return effOffsetIfOwnerFlagClear(unit, 0xC4);
    default:
        return -1;
    }
}

typedef struct BtlCommandEffect {
    s32 command;
    s16 effect;
    u8 adjustSide;
    u8 pad07;
} BtlCommandEffect;

extern BtlCommandEffect D_00360468[];
extern s32 btlCheckCommandRequiredEntryMatches(BtlIndexList *, s32);

/* Retained operand groups can suppress the command's effect. */
s16 btlGetCommandEffectId(BtlTask *task, s32 command) {
    BtlOperandGroup *result = task->indexWork.groups;
    u32 i;
    u32 count = btlGetIndexListCount(task->indexWork.indices);
    s8 rejected = 0;
    s16 effect;
    u8 adjustSide;

    for (i = 0; i < count; i++, result++) {
        if (result->inactive || result->kind == 2 || result->kind == 4) {
            rejected++;
        }
    }
    if (rejected == count) {
        return -1;
    }
    effect = -1;
    adjustSide = 0;
    for (i = 0; i < 59; i++) {
        if (D_00360468[i].command == command) {
            effect = D_00360468[i].effect;
            adjustSide = D_00360468[i].adjustSide;
            break;
        }
    }
    if (btlCheckCommandRequiredEntryMatches(task->indexWork.indices, command) != 0) {
        if (datCommandRecords[command].requirementBits == 0x800 ||
            datCommandRecords[command].requirementBits == 0x1000) {
            effect = 0x6A;
        } else {
            effect = 0x84;
        }
    }
    effect = effOffsetIfOwnerFlagClear(task->unit, effect);
    if (adjustSide) {
        s32 ownerSide = task->unit->flags & 0x600;
        s32 targetSides = 0;

        count = btlGetIndexListCount(task->indexWork.indices);
        for (i = 0; i < count; i++) {
            targetSides |= ((BtlUnit *)btlGetIndexListEntry(task->indexWork.indices, i))->flags & 0x600;
        }
        if (ownerSide != targetSides && targetSides != 0) {
            effect = (task->unit->flags & 0x200) ? effect + 1 : effect - 1;
        }
    }
    return effect;
}


/* Adds one to the base unless bit 9 of the owner's flags is set. */

s32 effOffsetIfOwnerFlagClear(BtlUnit *owner, s32 base) {
    return base + (((owner->flags >> 9) ^ 1U) & 1);
}

/* Number display: initializes both anchor vectors, then adds the offset before projection. */
INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FD5C8);

/* Decrement the linked-number task's unit counter without underflowing zero. */
void effDecrementFirstCountdown(BtlLinkedEffectArgs *args) {
    u8 remaining;
    BtlUnit *unit;

    unit = args->unit;
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

/* Scheduler predicate used for task entry and exit, not a battle actor record. */
typedef struct TaskCondition {
    u8 kind; /* 0 never, 1 always, 2 counter threshold, 3-10 task queries. */
    u8 pad01[7];
    union {
        s32 count;
        u64 handle;
        u64 owner;
        u16 taskKind;
    } value;
} TaskCondition;

/* Generic battle scheduler header; effect arguments begin at byte 0x70.
 * Creators choose start kind 1 (always) and end kind 0 (no exit predicate);
 * update reports completion instead. BtlTask in btl_task.h is a different object. */
typedef struct BtlEffectTask {
    TaskCondition startCondition;
    TaskCondition endCondition;
    u16 taskId;
    u16 state; /* 0 waiting, 1 start delay, 2 running, 3 end delay. */
    u16 flags;
    u8 unk26[2];
    s32 startDelay;
    s32 endDelay;
    u32 pollCount; /* Eligible scheduler polls, including waits and delays. */
    u32 runCount; /* Update calls that continued the running phase. */
    u64 handle; /* Installed by btlStartTask, independently of owner. */
    u64 owner; /* Actor identity, or the command's runtime-sequence token. */
    void (*onStart)(u32);
    s32 (*update)(); /* Called with args; nonzero completes the running phase. */
    void (*onFinish)(); /* Called with args before unlinking and release. */
    void *args; /* Allocator-recorded payload address, not an actor pointer. */
    struct BtlEffectTask *next;
    struct BtlEffectTask *prev;
    struct BtlEffectTask *deferNext;
    struct BtlEffectTask *deferPrev;
    u8 pad68[8];
} BtlEffectTask;

extern BtlEffectTask *btlAllocTask(s32);
extern s32 func_001FD5C8();

/* Create the selected numbered-display task with its frame count starting at zero. */
BtlEffectTask *btlCreateLinkedEffectTask(BtlUnit *owner, s32 value, u8 kind) {
    BtlEffectTask *task = btlAllocTask(0x34);
    BtlLinkedEffectArgs *args;

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    switch (kind) {
    case 0:
        task->taskId = 0x39;
        break;
    case 1:
        task->taskId = 0x3A;
        break;
    }
    task->flags |= 2;
    task->owner = owner->identity;
    task->update = func_001FD5C8;
    task->onFinish = effDecrementFirstCountdown;
    args = btlGetTaskArguments(task);
    args->payload.linked.kind = kind;
    args->unit = owner;
    args->payload.linked.value = value;
    args->payload.linked.elapsedTicks = 0;
    return task;
}

/* Counter display shares the same vector prefix, with the shorter counter payload. */
INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FDA78);

/* Decrement the counter-display task's unit counter without underflowing zero. */
void effDecrementSecondCountdown(BtlLinkedEffectArgs *args) {
    u8 remaining;
    BtlUnit *unit;

    unit = args->unit;
    remaining = unit->secondCountdown;
    if (remaining != 0) {
        unit->secondCountdown = remaining + 0xff;
    }
}

/* Create the counter-display task; its kind is stored as a byte. */
BtlEffectTask *btlCreateEffectCounterTask(BtlUnit *owner, s32 kind) {
    BtlEffectTask *task = btlAllocTask(0x2C);
    BtlLinkedEffectArgs *args;

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->flags |= 2;
    task->taskId = 0x3B;
    task->owner = owner->identity;
    task->update = func_001FDA78;
    task->onFinish = effDecrementSecondCountdown;
    args = (BtlLinkedEffectArgs *)btlGetTaskArguments(task);
    args->payload.counter.kind = kind;
    args->unit = owner;
    args->payload.counter.elapsedTicks = 0;
    return task;
}


extern void func_001AD970(s32);
extern s32 btlHasRegisteredSkillNamePanelTask(void);
extern void *btlGetIndexedUiResource();
extern s32 D_003BAA8C;
extern s32 D_00360458[];
extern s32 D_00360460[];

s32 btlPollActorOrEntryLabelTask(BtlObjLink *link) {
    BtlState *battleState = (BtlState *)btlGetRuntime();
    BtlUnit *owner = link->owner;

    if (link->elapsedTicks == 0) {
        s32 resourceIndex = link->arg;

        if (resourceIndex == 0) {
            if ((btlUnitStatusPair(owner) & 0x1400) != 0) {
                if (battleState->commandRestrictFlags & 0x400) {
                    if (owner->flags & 0x400) {
                        func_001AD970(D_00360460[0]);
                    } else {
                        func_001AD970(D_00360458[0]);
                    }
                } else {
                    func_001AD970(D_00360458[0]);
                }
            } else {
                func_001AD970((s32)btlGetIndexedUiResource(owner));
            }
        } else {
            func_001AD970(D_003BAA8C + resourceIndex * 17);
        }
    }
    if (btlHasRegisteredSkillNamePanelTask() == 0 || (u32)link->elapsedTicks >= 0x1E) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

extern s32 btlPollActorOrEntryLabelTask();

BtlEffectTask *btlCreateEffObjD(BtlUnit *owner, s32 arg) {
    BtlEffectTask *task = btlAllocTask(0xC);
    BtlObjLink *link;

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->flags |= 2;
    task->taskId = 0x40;
    task->owner = owner->identity;
    task->update = btlPollActorOrEntryLabelTask;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = arg;
    link->elapsedTicks = 0;
    return task;
}

extern s32 D_003BAA84;
extern void func_001AD970(s32);
extern s32 btlHasRegisteredSkillNamePanelTask(void);

s32 btlPollTimedTaskLink(BtlObjLink *link) {
    btlGetRuntime();
    if (link->elapsedTicks == 0) {
        func_001AD970(D_003BAA84 + link->arg * 0x19);
    }
    if (btlHasRegisteredSkillNamePanelTask() == 0 || (u32)link->elapsedTicks >= 0x1E) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

extern s32 btlPollTimedTaskLink();

BtlEffectTask *btlCreateOwnerLinkedTimedTask(BtlUnit *owner, s32 arg) {
    BtlEffectTask *task = btlAllocTask(0xC);
    BtlObjLink *link;

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->flags |= 2;
    task->taskId = 0x41;
    task->owner = owner->identity;
    task->update = btlPollTimedTaskLink;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = arg;
    link->elapsedTicks = 0;
    return task;
}

extern u8 D_00360428[];
extern u8 D_00360430[];
extern s32 D_00360438[];
extern s32 D_00360448[];
extern s32 D_0036044C[];
extern s32 D_00360450[];

s32 btlPollCategoryLabelTask(BtlObjLink *link) {
    BtlUnit *owner;

    btlGetRuntime();
    owner = link->owner;
    if (link->elapsedTicks == 0) {
        switch (link->arg) {
        case 10:
            func_001AD970((s32)D_00360428);
            break;
        case 5:
            if (owner->flags & 0x1000) {
                func_001AD970(D_00360448[0]);
            } else {
                func_001AD970(D_00360438[0]);
            }
            break;
        case 6:
            func_001AD970((s32)D_00360430);
            break;
        case 9:
            func_001AD970(D_0036044C[0]);
            break;
        case 11:
            func_001AD970(D_00360450[0]);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 7:
        case 8:
        case 12:
        case 13:
        case 14:
        case 15:
        default:
            return 1;
        }
    }
    if (btlHasRegisteredSkillNamePanelTask() == 0 || (u32)link->elapsedTicks >= 0x1E) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

extern s32 btlPollCategoryLabelTask();

/* Preserve the owner's effect data in the new task. */
BtlEffectTask *btlCreateEffObjA(BtlUnit *owner, s32 arg) {
    BtlEffectTask *task = btlAllocTask(0xC);
    BtlObjLink *link;

    task->startCondition.kind = 1;
    task->taskId = 0x42;
    task->flags |= 2;
    task->endCondition.kind = 0;
    if (owner != NULL) {
        task->owner = owner->identity;
    }
    task->update = btlPollCategoryLabelTask;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = arg;
    link->elapsedTicks = 0;
    return task;
}


typedef struct BtlJyokyoState {
    u8 pad00[0x498];
    s32 dialogId;          /* 0x498 */
    s32 alternateDialogId; /* 0x49C */
    s32 thirdDialogId;     /* 0x4A0 */
} BtlJyokyoState;

s32 btlJyokyoEffectUpdate(BtlObjLink *link) {
    BtlJyokyoState *state = (BtlJyokyoState *)btlGetRuntime();
    BtlUnit *owner = link->owner;

    if (link->elapsedTicks == 0) {
        if (owner != NULL) {
            itfMesSetTextSlotFromValue(state->dialogId, 0, owner->partyRecord.unitId, (owner->partyRecord.flags & 0x20) ? 0xE : 0xF);
        }
        func_003003F0("JYOKYO ID : %d\n", state->dialogId);
        btlReplaceDialogTasksAndQueueMessage(state->dialogId, link->arg);
    }
    if (btlHasRegisteredAphNamePanelTask() == 0 || (u32)link->elapsedTicks >= 0x2D) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

BtlEffectTask *btlCreateEffObjB(BtlUnit *owner, s32 messageId) {
    BtlEffectTask *task = btlAllocTask(0xC);
    BtlObjLink *link;

    task->startCondition.kind = 1;
    task->taskId = 0x3C;
    task->flags |= 2;
    task->endCondition.kind = 0;
    if (owner != NULL) {
        task->owner = owner->identity;
    }
    task->update = btlJyokyoEffectUpdate;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = messageId;
    link->elapsedTicks = 0;
    return task;
}

s32 btlUpdateLinkedDialogueEffect(BtlObjLink *link) {
    s32 battleState = btlGetRuntime();
    BtlUnit *owner = link->owner;

    if (owner != 0 && !(owner->partyRecord.status & 1)) {
        return 1;
    }
    if (link->elapsedTicks == 0) {
        if (owner != 0) {
            itfMesSetTextSlotFromValue(((BtlJyokyoState *)battleState)->alternateDialogId, 0, owner->partyRecord.unitId,
                          (owner->partyRecord.flags & 0x20) ? 0xE : 0xF);
        }
        btlReplaceDialogTasksAndQueueMessage(((BtlJyokyoState *)battleState)->alternateDialogId, link->arg);
    }
    if (btlHasRegisteredAphNamePanelTask() == 0 || (u32)link->elapsedTicks >= 0x2D) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

BtlEffectTask *btlCreateEffObjC(BtlUnit *owner, s32 messageId) {
    BtlEffectTask *task = btlAllocTask(0xC);
    BtlObjLink *link;

    task->startCondition.kind = 1;
    task->taskId = 0x3D;
    task->flags |= 2;
    task->endCondition.kind = 0;
    if (owner != NULL) {
        task->owner = owner->identity;
    }
    task->update = btlUpdateLinkedDialogueEffect;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = messageId;
    link->elapsedTicks = 0;
    return task;
}

u32 func_001FE658(void) {
    return 1;
}

BtlEffectTask *btlCreateEffectTask3E(BtlUnit *owner, u16 arg) {
    BtlEffectTask *task = btlAllocTask(0xC);
    BtlObjLink *link;

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->flags |= 2;
    task->taskId = 0x3E;
    task->owner = owner->identity;
    task->update = func_001FE658;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    *(u16 *)&link->arg = arg;
    link->elapsedTicks = 0;
    return task;
}

s32 btlPollTimedPresentationTask(BtlObjLink *link) {
    BtlJyokyoState *state = (BtlJyokyoState *)btlGetRuntime();
    BtlUnit *owner = link->owner;

    if (link->elapsedTicks == 0) {
        if (owner != NULL) {
            itfMesSetTextSlotFromValue(state->thirdDialogId, 0, owner->partyRecord.unitId,
                          (owner->partyRecord.flags & 0x20) ? 1 : 2);
        }
        btlReplaceDialogTasksAndQueueMessage(state->thirdDialogId, link->arg);
    }
    if (btlHasRegisteredAphNamePanelTask() == 0 || (u32)link->elapsedTicks >= 0x2D) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

extern s32 btlPollTimedPresentationTask();

BtlEffectTask *btlCreateOwnerLinkedTimedPresentation(BtlUnit *owner, s32 messageId) {
    BtlEffectTask *task = btlAllocTask(0xC);
    BtlObjLink *link;

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->flags |= 2;
    task->taskId = 0x3F;
    task->owner = owner->identity;
    task->update = btlPollTimedPresentationTask;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = messageId;
    link->elapsedTicks = 0;
    return task;
}

s32 btlPollEffectWaitTask(BtlObjLink *link) {
    s32 battleState = btlGetRuntime();

    if (link->elapsedTicks == 0) {
        if (link->owner != 0) {
            itfMesSetTextSlotFromValue(((BtlJyokyoState *)battleState)->dialogId, 0, *(u16 *)&link->arg, 0xD);
        }
        btlReplaceDialogTasksAndQueueMessage(((BtlJyokyoState *)battleState)->dialogId, 0x75);
    }
    if (btlHasRegisteredAphNamePanelTask() == 0 || (u32)link->elapsedTicks >= 0x2D) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

BtlEffectTask *btlCreateEffectWaitTask(BtlUnit *owner, u16 mode) {
    BtlEffectTask *task = btlAllocTask(0xC);
    BtlObjLink *link;

    task->startCondition.kind = 1;
    task->taskId = 0x43;
    task->flags |= 2;
    task->endCondition.kind = 0;
    if (owner != NULL) {
        task->owner = owner->identity;
    }
    task->update = btlPollEffectWaitTask;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    *(u16 *)&link->arg = mode;
    link->elapsedTicks = 0;
    return task;
}

typedef struct BtlWaitTask {
    s32 value;
    u32 ticks;
} BtlWaitTask;

/* Waits for the task startup delay, then finishes when its two actor slots are clear. */

s32 btlWaitEffectTask(BtlWaitTask *task) {
    if (task->ticks == 0) {
        btlCreateAnalysisPanelTask(task->value, 0);
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

BtlEffectTask *btlCreateEffectTask44(BtlUnit *owner) {
    BtlEffectTask *task = btlAllocTask(8);
    BtlObjLink *link;

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->flags |= 2;
    task->taskId = 0x44;
    task->owner = owner->identity;
    task->update = btlWaitEffectTask;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = 0;
    return task;
}

u32 btlNextScaledRandom(u32 limit) {
    btlRandomState = btlRandomState * 0x41c64e6d + 0x3039;
    return (btlRandomState >> 0x10) * (limit & 0xffff) >> 0x10;
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
    ((BtlTask *)task)->indexWork.phase = 0xb;
    ((BtlTask *)task)->indexWork.skillId = 0xc2;
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
    s32 node = *(s32 *)(btlGetRuntime() + 0x224);

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
    work[1] = owner->partyRecord.unitId;
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
        result = btlPackedEffectHandlers[type](context, packedAction) != 0;
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

extern AiSpecies *datEnemyAiRecords;
extern void btlSelectTierMatrixAiRoute(BtlUnit *unit, u16 species, s32 *row, s32 arg);
extern u32 btlPickWeightedAiSlot();
extern s32 btlRunAiAction();

/* Choose a row and a weighted slot of the unit's species AI table and run that action. */
s32 btlRunRandomWeightedAiTableAction(BtlTask *task) {
    s32 *work = (s32 *)sdfAllocAndClearQuadwords(0x10);
    BtlUnit *unit;
    u16 species;
    s32 row;
    s32 index;

    unit = task->unit;
    btlActionScratchWork = (s32)work;
    species = unit->partyRecord.unitId;
    work[0] = (s32)task;
    work[1] = species;
    btlSelectTierMatrixAiRoute(unit, species, &row, 0);
    index = btlPickWeightedAiSlot(unit, species, row);
    btlRunAiAction(task, datEnemyAiRecords[species].slot[row * 5 + index].actionId,
                   datEnemyAiRecords[species].slot[row * 5 + index].actionArg);
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
    btlSelectLowestHealthElementBlockTarget();
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
    btlSelectTargetsByMode();
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
    btlSelectTargetsExcludingActorUnit();
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

INCLUDE_SDATA(const s32, "game/code_001FC7D8", btlRandomState);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", btlPreviousAiCandidateBucket);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", btlActionScratchWork);

