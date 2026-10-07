#include "common.h"
#include "btl.h"
#include "btl_command.h"
#include "btl_action.h"
#include "btl_state.h"
#include "btl_task_args.h"
#include "pcp_vu0.h"

extern u32 btlRandomState;

extern void btlCmdSimpleB(s32, s32);

extern void btlCmdSimpleA(s32, s32);

extern void btlCmdSimpleD(s32, s32);

extern void btlCmdSimpleE(s32, s32);

extern void btlCmdSimpleJ(s32, s32);

extern void btlCmdSimpleC(s32, s32);



extern s32 btlDispatchPackedActionWithScratch(s32 context, BtlUnit *owner, s32 mask);


extern s32 btlActionScratchWork;

extern s32 btlDispatchPackedEffectAction(s32 context, u32 packedAction);

extern s32 sdfAllocAndClearQuadwords(s32);

extern void sdfReleaseChipBlock(s32);

extern void func_002152D8(s32, s32);

extern void func_00215C70(s32, s32);

extern void btlSelectLowestHealthRateTarget(s32, s32);

extern void btlSelectLowestRankTarget(s32, s32);

extern void func_00216888(s32, s32);

extern void func_00216D30();

extern void func_00216D10();

extern void btlAppendSelfAfterTargetScan();

extern void btlSelectTargetsExcludingActorUnit();

extern void btlAppendEffectActorToCommandIndices(s32, s32);

extern void btlAppendCurrentUnitIdToCommandIndices(s32, s32);

extern void btlSelectTargetsByMode();

typedef struct EffChildCounters {
    u8 pad00[0x338];
    u8 firstCountdown;
    u8 secondCountdown;
} EffChildCounters;

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

extern s8 D_0037F510[];

typedef struct BtlWaitTask {
    s32 value;
    u32 ticks;
} BtlWaitTask;

extern s32 (*btlPackedEffectHandlers[])(s32, u32);

typedef struct BtlEffLink {
    BtlUnit *owner;  /* 0x00: effect owner */
    s32 arg;    /* 0x04 */
    u32 elapsedTicks;  /* 0x08: elapsed callback updates */
} BtlEffLink;




extern BtlRuntimeTask *btlAllocTask(s32 size);


extern void btlShiftActorStateHistory(ActionStateLink *obj, s8 flag);
extern s8 btlHistoryCounter;

typedef struct BtlPackedCtx {
    s32 context;
    s32 unk4;
} BtlPackedCtx;
extern s32 D_00435E5C;
extern void itfMesSetTextSlotFromValue(s32, s32, s32, s32);
extern void func_001B8580(s32);
extern void btlReplaceDialogTasksAndQueueMessage(s32, s32);
extern u32 btlHasRegisteredSkillNamePanelTask(void);
extern u32 btlHasRegisteredAphNamePanelTask(void);
extern char D_00436CA8[];
extern s32 func_0035C860(char *, const char *, ...);
extern void itfMesCopyStringToWindowTableSlot(s32, s32, char *);
extern s32 D_00435E64;
extern u8 D_003BEB28[];
extern u8 D_003BEB30[];
extern s32 D_003BEB38[];
extern s32 D_003BEB48[];
extern s32 D_003BEB4C[];
extern s32 D_003BEB50[];
extern s32 D_003BEB58[];
extern s32 D_003BEB60[];
extern void *btlGetIndexedUiResource();
extern s32 btlRollAiBucket(void);
extern s32 btlPollCategoryLabelTask(void *args);
extern s32 btlPollActorOrEntryLabelTask(void *args);
extern s32 func_0020FA98(BtlLinkedEffectArgs *args);
extern s32 btlPollTimedTaskLink(void *args);
extern s32 btlPollActorDialogTask(void *args);
extern s32 btlUpdateLinkedDialogueEffect(void *args);
extern s32 btlPollTimedPresentationTask(void *args);
extern s32 btlPollEffectWaitTask(void *args);
extern s32 btlAdvanceActorEffectLabelTask(void *args);

extern s32 func_0020F5E0(BtlLinkedEffectArgs *args);


/* Store the supplied name-record word without interpreting its bits. */
void func_0020E850(BtlResourceNameRecord *record, u32 value) {
    record->unk14 = value;
}

/* Name-record overwrite prompt: uses coordinates, selection, state and both text slices. */
INCLUDE_ASM(const s32, "game/code_0020E850", func_0020E858);


void func_0020EA10(void) {
}


INCLUDE_ASM(const s32, "game/code_0020E850", func_0020EA18);

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020EB40);

/* Accepts only when status bit 5 is set and task bit 6 is clear.
 * For a lone bit-5 status, compare the unsigned AI roll bucket to 70. */
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
    return (u32)btlRollAiBucket() < 0x46;
}

extern s32 btlSetActorEffectParameter(u8 *object, s32 index);
extern void btlUnitGetMuzzlePosVU(BtlUnit *unit);
extern f32 btlUnitGetTopY(BtlUnit *unit);
extern s32 btlProjectForwardPositionToPackedScreen(s32 screen[4]);
extern void btlBossDebugPrintf(const char *format, ...);

/* vu0 routine: choose an on-screen HP/MP label anchor, leaving it in vf10. */
INCLUDE_RODATA(const s32, "game/code_0020E850", D_00419910);

INCLUDE_RODATA(const s32, "game/code_0020E850", D_00419920);

void func_0020EC20(BtlUnit *unit) {
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
            if (visible == 1 && !(unit->stateFlags & 0x400000) &&
                x >= 12 && x < 501 && y >= 48 && y < 324) {
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

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020F048);

extern s32 effOffsetIfOwnerFlagClear(BtlUnit *, s32);

/* Select the owner's effect variant for its current condition and task action. */
s32 func_0020F200(BtlTask *task) {
    BtlUnit *unit = task->unit;
    switch (unit->partyRecord.status & 0x7FFF) {
    case 0x2000:
        return effOffsetIfOwnerFlagClear(unit, 0xD2);
    case 0x200:
        if (task->result != 1) {
            return effOffsetIfOwnerFlagClear(unit, 0x7A);
        }
        return effOffsetIfOwnerFlagClear(unit, 0x78);
    case 0x40:
        return effOffsetIfOwnerFlagClear(unit, 6);
    case 0x20:
        switch (task->result) {
        case 13:
            return effOffsetIfOwnerFlagClear(unit, 0x72);
        case 14:
            return effOffsetIfOwnerFlagClear(unit, 0x70);
        case 2:
        case 3:
        case 7:
        case 8:
            if (task->arg == 0xE0) {
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

typedef struct BtlEffectCommandRecord {
    u8 pad00[0x28];
    s32 requiredFlags;
    u8 pad2C[12];
} BtlEffectCommandRecord;

extern BtlCommandEffect D_003BEB68[];
extern BtlEffectCommandRecord *datCommandRecords;
extern s32 btlCheckCommandRequiredEntryMatches(BtlIndexList *, s32);

s16 btlGetCommandEffectId(ActionStateLink *link, s32 command) {
    BtlOperandGroup *result = link->indexWork.groups;
    u32 i;
    u32 count = btlGetIndexListCount(link->indexWork.indices);
    s32 rejected = 0;
    s16 effect;
    u8 adjustSide;

    for (i = 0; i < count; i++, result++) {
        if (result->unk10) {
            rejected++;
        } else {
            switch (result->unk08) {
            case 2:
            case 4:
            case 0x10000:
            case 0x40000:
                rejected++;
                break;
            default:
                if (result->unk14) {
                    rejected++;
                }
                break;
            }
        }
    }
    if (rejected == count) {
        return -1;
    }
    effect = -1;
    adjustSide = 0;
    for (i = 0; i < 77; i++) {
        if (D_003BEB68[i].command == command) {
            effect = D_003BEB68[i].effect;
            adjustSide = D_003BEB68[i].adjustSide;
            break;
        }
    }
    if (btlCheckCommandRequiredEntryMatches(link->indexWork.indices, command) != 0) {
        if (datCommandRecords[command].requiredFlags == 0x800 ||
            datCommandRecords[command].requiredFlags == 0x1000) {
            effect = 0x6A;
        } else {
            effect = 0x84;
        }
    }
    effect = effOffsetIfOwnerFlagClear(link->unit, effect);
    if (adjustSide) {
        s32 ownerSide = link->unit->flags & 0x600;
        s32 targetSides = 0;

        count = btlGetIndexListCount(link->indexWork.indices);
        for (i = 0; i < count; i++) {
            targetSides |= ((BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, i))->flags & 0x600;
        }
        if (ownerSide != targetSides && targetSides != 0) {
            effect = (link->unit->flags & 0x200) ? effect + 1 : effect - 1;
        }
    }
    return effect;
}

/* Adds one to the base unless bit 9 of the owner's flags is set. */
s32 effOffsetIfOwnerFlagClear(BtlUnit *owner, s32 base) {
    return base + (((owner->flags >> 9) ^ 1U) & 1);
}

/* Number display: initializes both anchor vectors, then adds the offset before projection. */
INCLUDE_ASM(const s32, "game/code_0020E850", func_0020F5E0);

/* Decrement the linked-number task's unit counter without underflowing zero. */
void effDecrementFirstCountdown(u32 *arguments) {
    BtlLinkedEffectArgs *args = (BtlLinkedEffectArgs *)arguments;
    EffChildCounters *unit = (EffChildCounters *)args->unit;
    u8 count = unit->firstCountdown;
    if (count != 0) {
        unit->firstCountdown = count - 1;
    }
}

/* Create the selected numbered-display task with its frame count starting at zero. */
BtlRuntimeTask *btlCreateLinkedEffectTask(BtlUnit *owner, s32 value, u8 kind) {
    BtlRuntimeTask *task = btlAllocTask(0x34);
    BtlLinkedEffectArgs *args;

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    switch (kind) {
    case 0:
        task->taskId = 0x3C;
        break;
    case 1:
        task->taskId = 0x3D;
        break;
    }
    task->flags |= 2;
    task->ownerId = owner->owner;
    task->callback = func_0020F5E0;
    task->onFinish = effDecrementFirstCountdown;
    args = btlGetTaskArguments(task);
    args->payload.linked.kind = kind;
    args->unit = owner;
    args->payload.linked.value = value;
    args->payload.linked.elapsedTicks = 0;
    return task;
}

/* Counter display shares the same vector prefix, with the shorter counter payload. */
INCLUDE_ASM(const s32, "game/code_0020E850", func_0020FA98);

/* Decrement the counter-display task's unit counter without underflowing zero. */
void effDecrementSecondCountdown(u32 *arguments) {
    BtlLinkedEffectArgs *args = (BtlLinkedEffectArgs *)arguments;
    EffChildCounters *unit = (EffChildCounters *)args->unit;
    u8 count = unit->secondCountdown;
    if (count != 0) {
        unit->secondCountdown = count - 1;
    }
}

/* Create the counter-display task; its kind is stored as a byte. */
BtlRuntimeTask *btlCreateEffectCounterTask(BtlUnit *owner, s32 kind) {
    BtlRuntimeTask *task = btlAllocTask(0x2C);
    BtlLinkedEffectArgs *args;

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->flags |= 2;
    task->taskId = 0x3E;
    task->ownerId = owner->owner;
    task->callback = func_0020FA98;
    task->onFinish = effDecrementSecondCountdown;
    args = (BtlLinkedEffectArgs *)btlGetTaskArguments(task);
    args->payload.counter.kind = kind;
    args->unit = owner;
    args->payload.counter.elapsedTicks = 0;
    return task;
}

s32 btlPollActorOrEntryLabelTask(void *arguments) {
    BtlEffLink *link = arguments;
    BtlState *battleState = (BtlState *)btlGetRuntime();
    BtlUnit *owner = link->owner;

    if (link->elapsedTicks == 0) {
        s32 resourceIndex = link->arg;

        if (resourceIndex == 0) {
            if ((btlUnitStatusPair(owner) & 0x1400) == 0 && (owner->partyRecord.flags & 0x10) == 0) {
                func_001B8580((s32)btlGetIndexedUiResource(owner));
            } else if (battleState->commandRestrictFlags & 0x400) {
                if (owner->flags & 0x400) {
                    func_001B8580(D_003BEB60[0]);
                } else {
                    func_001B8580(D_003BEB58[0]);
                }
            } else {
                func_001B8580(D_003BEB58[0]);
            }
        } else {
            func_001B8580(D_00435E64 + resourceIndex * 17);
        }
    }
    if (btlHasRegisteredSkillNamePanelTask() == 0 || link->elapsedTicks >= 0x1E) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

BtlRuntimeTask *btlCreateEffObjD(BtlUnit *owner, s32 resourceIndex) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    BtlEffLink *link;

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->flags |= 2;
    task->taskId = 0x43;
    task->ownerId = owner->owner;
    task->callback = btlPollActorOrEntryLabelTask;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = resourceIndex;
    link->elapsedTicks = 0;
    return task;
}

s32 btlPollTimedTaskLink(void *arguments) {
    BtlEffLink *link = arguments;
    btlGetRuntime();
    if (link->elapsedTicks == 0) {
        func_001B8580(D_00435E5C + link->arg * 25);
    }
    if (btlHasRegisteredSkillNamePanelTask() == 0 || link->elapsedTicks >= 0x1E) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

BtlRuntimeTask *btlCreateOwnerLinkedTimedTask(BtlUnit *owner, s32 arg) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    BtlEffLink *link;

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->flags |= 2;
    task->taskId = 0x44;
    task->ownerId = owner->owner;
    task->callback = btlPollTimedTaskLink;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = arg;
    link->elapsedTicks = 0;
    return task;
}

s32 btlPollCategoryLabelTask(void *arguments) {
    BtlEffLink *link = arguments;
    BtlUnit *owner;

    btlGetRuntime();
    owner = link->owner;
    if (link->elapsedTicks == 0) {
        switch (link->arg) {
        case 10:
            func_001B8580((s32)D_003BEB28);
            break;
        case 5: {
            u32 flags = owner->flags;
            if (flags & 0x200) {
                if (!(flags & 0x1000)) {
                    func_001B8580(D_003BEB38[0]);
                    break;
                }
            }
            func_001B8580(D_003BEB48[0]);
            break;
        }
        case 6:
            func_001B8580((s32)D_003BEB30);
            break;
        case 9:
            func_001B8580(D_003BEB4C[0]);
            break;
        case 11:
            func_001B8580(D_003BEB50[0]);
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
    if (btlHasRegisteredSkillNamePanelTask() == 0 || link->elapsedTicks >= 0x1E) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

/* Preserve the owner's effect data in the new task. */
BtlRuntimeTask *btlCreateEffObjA(BtlUnit *owner, s32 category) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    BtlEffLink *link;

    task->startCondition.kind = 1;
    task->taskId = 0x45;
    task->flags |= 2;
    task->endCondition.kind = 0;
    if (owner != NULL) {
        task->ownerId = owner->owner;
    }
    task->callback = btlPollCategoryLabelTask;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = category;
    link->elapsedTicks = 0;
    return task;
}

s32 btlPollActorDialogTask(void *arguments) {
    BtlEffLink *link = arguments;
    BtlState *battleState = (BtlState *)btlGetRuntime();
    BtlUnit *owner = link->owner;

    if (link->elapsedTicks == 0) {
        if (owner != 0) {
            itfMesSetTextSlotFromValue(battleState->messageWindows[0], 0, owner->partyRecord.unitId, (owner->partyRecord.flags & 0x20) ? 0xE : 0xF);
        }
        btlReplaceDialogTasksAndQueueMessage(battleState->messageWindows[0], link->arg);
    }
    if (btlHasRegisteredAphNamePanelTask() == 0 || link->elapsedTicks >= 0x2D) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

BtlRuntimeTask *btlCreateEffObjB(BtlUnit *owner, s32 messageId) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    BtlEffLink *link;

    task->startCondition.kind = 1;
    task->taskId = 0x3F;
    task->flags |= 2;
    task->endCondition.kind = 0;
    if (owner != NULL) {
        task->ownerId = owner->owner;
    }
    task->callback = btlPollActorDialogTask;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = messageId;
    link->elapsedTicks = 0;
    return task;
}

s32 btlUpdateLinkedDialogueEffect(void *arguments) {
    BtlEffLink *link = arguments;
    BtlState *battleState = (BtlState *)btlGetRuntime();
    BtlUnit *owner = link->owner;

    if (owner != 0 && !(owner->partyRecord.status & 1)) {
        return 1;
    }
    if (link->elapsedTicks == 0) {
        if (owner != 0) {
            itfMesSetTextSlotFromValue(battleState->messageWindows[1], 0, owner->partyRecord.unitId, (owner->partyRecord.flags & 0x20) ? 0xE : 0xF);
        }
        btlReplaceDialogTasksAndQueueMessage(battleState->messageWindows[1], link->arg);
    }
    if (btlHasRegisteredAphNamePanelTask() == 0 || link->elapsedTicks >= 0x2D) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

BtlRuntimeTask *btlCreateEffObjC(BtlUnit *owner, s32 messageId) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    BtlEffLink *link;

    task->startCondition.kind = 1;
    task->taskId = 0x40;
    task->flags |= 2;
    task->endCondition.kind = 0;
    if (owner != NULL) {
        task->ownerId = owner->owner;
    }
    task->callback = btlUpdateLinkedDialogueEffect;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = messageId;
    link->elapsedTicks = 0;
    return task;
}

s32 func_00210688(void *unused) {
    return 1;
}

BtlRuntimeTask *btlCreateEffectTask3E(BtlUnit *owner, u16 arg) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    BtlEffLink *link;

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->flags |= 2;
    task->taskId = 0x41;
    task->ownerId = owner->owner;
    task->callback = func_00210688;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    *(u16 *)&link->arg = arg;
    link->elapsedTicks = 0;
    return task;
}

s32 btlPollTimedPresentationTask(void *arguments) {
    BtlEffLink *link = arguments;
    BtlState *battleState = (BtlState *)btlGetRuntime();
    BtlUnit *owner = link->owner;

    if (link->elapsedTicks == 0) {
        if (owner != 0) {
            itfMesSetTextSlotFromValue(battleState->scriptTarget, 0, owner->partyRecord.unitId, (owner->partyRecord.flags & 0x20) ? 1 : 2);
        }
        btlReplaceDialogTasksAndQueueMessage(battleState->scriptTarget, link->arg);
    }
    if (btlHasRegisteredAphNamePanelTask() == 0 || link->elapsedTicks >= 0x2D) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

BtlRuntimeTask *btlCreateOwnerLinkedTimedPresentation(BtlUnit *owner, s32 messageId) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    BtlEffLink *link;

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->flags |= 2;
    task->taskId = 0x42;
    task->ownerId = owner->owner;
    task->callback = btlPollTimedPresentationTask;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = messageId;
    link->elapsedTicks = 0;
    return task;
}

s32 btlPollEffectWaitTask(void *arguments) {
    BtlEffLink *link = arguments;
    BtlState *battleState = (BtlState *)btlGetRuntime();

    if (link->elapsedTicks == 0) {
        if (link->owner != 0) {
            itfMesSetTextSlotFromValue(battleState->messageWindows[0], 0, *(u16 *)&link->arg, 0xD);
        }
        btlReplaceDialogTasksAndQueueMessage(battleState->messageWindows[0], 0x75);
    }
    if (btlHasRegisteredAphNamePanelTask() == 0 || link->elapsedTicks >= 0x2D) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

BtlRuntimeTask *btlCreateEffectWaitTask(BtlUnit *owner, u16 mode) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    BtlEffLink *link;

    task->startCondition.kind = 1;
    task->taskId = 0x46;
    task->flags |= 2;
    task->endCondition.kind = 0;
    if (owner != NULL) {
        task->ownerId = owner->owner;
    }
    task->callback = btlPollEffectWaitTask;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    *(u16 *)&link->arg = mode;
    link->elapsedTicks = 0;
    return task;
}

/* Waits for the task startup delay, then finishes when its two actor slots are clear. */
s32 btlWaitEffectTask(void *arguments) {
    BtlWaitTask *task = arguments;
    if (task->ticks == 0) {
        btlCreateAnalysisPanelTask(task->value, 0);
        func_001C7DB8(0, 8);
    }
    if (task->ticks >= 0x11) {
        if (btlGetRegisteredTaskValueOrDefault() == 1) {
            if (D_0037F510[0x21] < 0 || D_0037F510[0x23] < 0) {
                func_001C7DB8(1, 8);
                func_001B8038();
                return 1;
            }
        }
    } else {
        task->ticks += 1;
    }
    return 0;
}

BtlRuntimeTask *btlCreateEffectTask44(BtlUnit *owner) {
    BtlRuntimeTask *task = btlAllocTask(8);
    BtlEffLink *link;

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->flags |= 2;
    task->taskId = 0x47;
    task->ownerId = owner->owner;
    task->callback = btlWaitEffectTask;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = 0;
    return task;
}

s32 btlAdvanceActorEffectLabelTask(void *arguments) {
    BtlEffLink *link = arguments;
    BtlState *battleState = (BtlState *)btlGetRuntime();
    BtlUnit *owner = link->owner;
    char text[0x100];

    if (link->elapsedTicks == 0) {
        if (owner != 0) {
            itfMesSetTextSlotFromValue(battleState->messageWindows[0], 0, owner->partyRecord.unitId, (owner->partyRecord.flags & 0x20) ? 0xE : 0xF);
            func_0035C860(text, D_00436CA8, link->arg < 0 ? -link->arg : link->arg);
            itfMesCopyStringToWindowTableSlot(battleState->messageWindows[0], 1, text);
        }
        btlReplaceDialogTasksAndQueueMessage(battleState->messageWindows[0], 0xD5);
    }
    if (btlHasRegisteredAphNamePanelTask() == 0 || link->elapsedTicks >= 0x2D) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

BtlRuntimeTask *btlCreateTimedActorEffectLinkTask(BtlUnit *owner, s32 arg) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    BtlEffLink *link;

    task->startCondition.kind = 1;
    task->taskId = 0x48;
    task->flags |= 2;
    task->endCondition.kind = 0;
    if (owner != NULL) {
        task->ownerId = owner->owner;
    }
    task->callback = btlAdvanceActorEffectLabelTask;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = arg;
    link->elapsedTicks = 0;
    return task;
}

extern s32 scrReadIntParameter(s32 index);
extern BtlUnit *btlFindUnitByModeClear(s32 id);
extern BtlUnit *btlFindUnitByModeFlagged(s32 id);
extern BtlRuntimeTask *btlCreateEffObjB(BtlUnit *actor, s32 arg);
extern u64 btlStartTask(void *);
extern s64 btlAdvanceRuntimeSequenceCounter();

s32 btlCmdSpawnEffectTaskForSelectedUnit(void) {
    s32 mode;
    s32 unitId;
    s32 arg;
    BtlUnit *unit;
    BtlRuntimeTask *task;

    mode = scrReadIntParameter(0);
    unitId = scrReadIntParameter(1);
    arg = scrReadIntParameter(2);
    if (mode == 0) {
        unit = btlFindUnitByModeClear(unitId);
    } else {
        unit = btlFindUnitByModeFlagged(unitId);
    }
    if (unit == NULL) {
        return 1;
    }
    task = btlCreateEffObjB(unit, arg);
    task->ownerId = btlAdvanceRuntimeSequenceCounter();
    btlStartTask(task);
    return 1;
}

u32 btlNextScaledRandom(u32 limit) {
    btlRandomState = btlRandomState * 0x41c64e6d + 0x3039;
    return (btlRandomState >> 0x10) * (limit & 0xffff) >> 0x10;
}

/* Inclusive random selection between either ordering of the endpoints. */
s32 btlRandomInclusiveRange(s32 lower, s32 upper) {
    s32 swap;

    if (upper < lower) {
        swap = upper;
        upper = lower;
        lower = swap;
    }
    upper = upper - lower + 1;
    return (s32)btlNextScaledRandom(0xFFFF) % upper + lower;
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

void btlAdvanceHistoryCounter(ActionStateLink *obj) {
    obj->lowHpActionHold++;
    obj->lowHpActionHold = obj->lowHpActionHold <= 0 ? 0 : obj->lowHpActionHold >= 0x21 ? 0x20 : obj->lowHpActionHold;
    btlShiftActorStateHistory(obj, 0);
    btlHistoryCounter++;
    btlHistoryCounter = btlHistoryCounter <= 0 ? 0 : btlHistoryCounter >= 0x21 ? 0x20 : btlHistoryCounter;
}


void btlResetBattleHistoryCounters(void) {
    ActionStateLink *node = ((BtlState *)btlGetRuntime())->tasks;
    if (node != 0) {
        do {
            if (node->unit != NULL) {
                node->lowHpActionHold = 0;
                node->lastMode = 0;
            }
            node = node->next;
        } while (node != 0);
    }
    btlHistoryCounter = 0;
}

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
extern void func_00211658(BtlUnit *unit, u16 species, s32 *row, s32 arg);
extern u32 btlPickWeightedAiSlot();
extern s32 func_00211EA8();

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
    func_00211658(unit, species, &row, 0);
    index = btlPickWeightedAiSlot(unit, species, row);
    func_00211EA8(task, datEnemyAiRecords[species].slot[row * 5 + index].actionId,
                  datEnemyAiRecords[species].slot[row * 5 + index].actionArg);
    sdfReleaseChipBlock(btlActionScratchWork);
    return 1;
}

void btlShiftActorStateHistory(ActionStateLink *obj, s8 flag) {
    s32 i;

    for (i = 6; i >= 0; i--) {
        obj->actions[i + 1].word = obj->actions[i].word;
    }
    if ((u32)(obj->indexWork.phase - 1) < 4) {
        if (flag == 0) {
            obj->actions[0].word = obj->indexWork.skillId;
        } else {
            obj->actions[0].word = obj->unit->partyRecord.actionSlot;
        }
    } else {
        obj->actions[0].word = 0;
    }
}

void btlCmdWithArgA(s32 context) {
    func_002152D8(context, 0);
}

void btlCmdWithArgB(s32 context) {
    func_00215C70(context, 0);
}

void btlCmdWithArgC(s32 context) {
    btlSelectLowestHealthRateTarget(context, 0);
}

void func_002110E8(void) {
    btlSelectTargetsByMode();
}

void func_00211108(s32 context) {
    btlAppendCurrentUnitIdToCommandIndices(context, 0);
}

extern void btlSelectLowestHealthElementBlockTarget(s32, s32);

void btlCmdSimpleA(s32 context, s32 value) {
    btlSelectLowestHealthElementBlockTarget(context, value);
}

void btlCmdWithArgD(s32 context) {
    btlSelectLowestRankTarget(context, 0);
}

void btlCmdWithArgE(s32 context) {
    func_00216888(context, 0);
}

extern void btlSelectTargetsPassingCheck(s32, s32);

void btlCmdSimpleB(s32 context, s32 value) {
    btlSelectTargetsPassingCheck(context, value);
}

extern void btlSelectTargetsByActionMask(s32, s32);

void btlCmdSimpleC(s32 context, s32 value) {
    btlSelectTargetsByActionMask(context, value);
}

extern void btlSelectTargetsWithoutActionMask(s32, s32);

void btlCmdSimpleD(s32 context, s32 value) {
    btlSelectTargetsWithoutActionMask(context, value);
}

extern void func_002162C0(s32, s32);

void btlCmdSimpleE(s32 context, s32 value) {
    func_002162C0(context, value);
}

void btlCmdSimpleG(void) {
    func_00216D10();
}

void func_00211228(void) {
    func_00216D30();
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

extern void btlSelectTargetsBlockingElement(s32, s32);

void btlCmdSimpleJ(s32 context, s32 value) {
    btlSelectTargetsBlockingElement(context, value);
}

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436C78);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436C80);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436C88);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436C90);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436C98);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436CA0);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436CA8);

INCLUDE_SDATA(const s32, "game/code_0020E850", btlHistoryCounter);

INCLUDE_SDATA(const s32, "game/code_0020E850", btlRandomState);

INCLUDE_SDATA(const s32, "game/code_0020E850", btlPreviousAiCandidateBucket);

INCLUDE_SDATA(const s32, "game/code_0020E850", btlActionScratchWork);

