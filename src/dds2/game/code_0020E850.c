#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
#include "btl_indexed_ui_resource_api.h"
#include "btl_set_actor_effect_parameter_api.h"
#include "btl_actor_status_table_api.h"
#include "btl_effect_position.h"
#include "eff_offset_if_owner_flag_clear.h"
#include "btl_command_effect.h"
#include "btl_ai_slot.h"
#include "dds3obj.h"
#include "btl_resource_name.h"
#include "sdf_chip.h"
#include "btl_task_state.h"
#include "btl_task_condition.h"
#include "btl.h"
#include "btl_analysis_panel_wait_args.h"
#include "btl_command.h"
#include "btl_action.h"
#include "btl_ai_action_api.h"
#include "btl_ai_table.h"
#include "btl_ai_scratch.h"
#include "btl_state.h"
#include "btl_task_args.h"
#include "evt_unit.h"
#include "mdl.h"
#include "eff_transform.h"
#include "btl_model_record.h"
#include "pcp_vu0.h"
#include "dat_command.h"
#include "btl_boss_debug_api.h"
#include "eff.h"
/* Declared before first use (were implicit int). */
extern s32 btlCreateAnalysisPanelTask(s32 entry, s32 duration);
extern s32 btlGetRegisteredTaskValueOrDefault(void);
extern void btlSetCommandPanelFadeMode(s8 mode, s32 duration);
extern s32 btlIsLowHpActionReady(BtlUnit *unit, s32 input);

extern u32 btlRandomState;

extern void btlPickCommandElementCheckedTarget(ActionStateLink *, s32);

extern void btlPickCommandLowestHpBlockTarget(ActionStateLink *, s32);

extern void btlPickCommandTargetWithoutStatusMask(ActionStateLink *, s32);

extern void btlFilterCommandTargetsByUnitId(ActionStateLink *, s32);

extern void btlPickCommandElementBlockTarget(ActionStateLink *, s32);

extern void btlPickCommandTargetWithStatusMask(ActionStateLink *, s32);



extern s32 btlDispatchPackedActionWithScratch(s32 context, BtlUnit *owner, s32 mask);


extern BtlAiScratchWork *btlActionScratchWork;

extern s32 btlDispatchPackedEffectAction(s32 context, u32 packedAction);



extern s32 func_002152D8(ActionStateLink *, s32);

extern s32 btlSelectLowestNonzeroHpTarget(ActionStateLink *, s32);

extern void btlSelectLowestHealthRateTarget(ActionStateLink *, s32);

extern s32 btlSelectLowestRankTarget(ActionStateLink *, s32);

extern s32 btlSelectHighestCurrentMpTarget(ActionStateLink *, s32);

extern void btlPickLinkedPartyTarget(ActionStateLink *, u32);

extern void btlPickUnlinkedPartyTarget(ActionStateLink *, u32);

extern void btlAppendSelfAfterTargetScan(ActionStateLink *);

extern s32 btlSelectTargetsExcludingActorUnit(ActionStateLink *);

extern void btlAppendEffectActorToCommandIndices(ActionStateLink *, s32);

extern void btlAppendSelectedOrCurrentActorTarget(ActionStateLink *, s32);

extern void btlSelectTargetsByMode(ActionStateLink *, s32);


extern s8 dds3PadButtonState[];

extern u32 btlRequestAnalysisPanelClose(void);

extern s32 (*btlPackedEffectHandlers[])(s32, u32);

typedef struct BtlEffLink {
    BtlUnit *owner;  /* 0x00: effect owner */
    union {
        s32 arg;
        u16 shortArg; /* Message/wait variants write only the low halfword. */
    };             /* 0x04 */
    u32 elapsedTicks;  /* 0x08: elapsed callback updates */
} BtlEffLink;




extern BtlRuntimeTask *btlAllocTask(s32 size);


extern void btlShiftActorStateHistory(ActionStateLink *obj, s8 flag);
extern s8 btlHistoryCounter;

typedef struct BtlPackedCtx {
    s32 context;
    s32 unk4;
} BtlPackedCtx;
extern u8 *D_00435E5C;
extern void itfMesSetTextSlotFromValue(s32, s32, s32, s32);
extern s32 btlCreateSkillNamePanel(const u8 *text);
extern void btlReplaceDialogTasksAndQueueMessage(s32, s32);
extern u32 btlHasRegisteredSkillNamePanelTask(void);
extern u32 btlHasRegisteredAphNamePanelTask(void);
extern char D_00436CA8[];
extern void itfMesMngSetVarImmediate(s32, u32, const char *);
extern u8 *datSkillNames;
extern u8 btlActionPassLabel[];
extern u8 btlActionRetreatLabel[];
extern const u8 *D_003BEB38[];
extern const u8 *D_003BEB48[];
extern const u8 *D_003BEB4C[];
extern const u8 *D_003BEB50[];
extern const u8 *D_003BEB58[];
extern const u8 *D_003BEB60[];
extern s32 btlRollAiBucket(void);
extern s32 btlPollCategoryLabelTask(BtlEffLink *args);
extern s32 btlPollActorOrEntryLabelTask(BtlEffLink *args);
extern s32 func_0020FA98(BtlLinkedEffectArgs *args);
extern s32 btlPollTimedTaskLink(BtlEffLink *args);
extern s32 btlPollActorDialogTask(BtlEffLink *args);
extern s32 btlUpdateLinkedDialogueEffect(BtlEffLink *args);
extern s32 btlPollTimedPresentationTask(BtlEffLink *args);
extern s32 btlPollEffectWaitTask(BtlEffLink *args);
extern s32 btlAdvanceActorEffectLabelTask(BtlEffLink *args);

extern s32 btlUpdateLinkedNumberDisplay(BtlLinkedEffectArgs *args);


/* Set the native editor's name-length limit. */
void btlSetResourceNameLengthLimit(struct BtlResourceNameRecord *record, u32 maximumNameLength) {
    record->maximumNameLength = maximumNameLength;
}

/* Name-record overwrite prompt: uses coordinates, selection, state and both text slices. */
INCLUDE_ASM(const s32, "game/code_0020E850", btlPollResourceNameOverwrite);


void func_0020EA10(void) {
}



extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *model, s32 id);
extern void fldDrawShadowDisc(f32 x, f32 y, f32 z, f32 radius, s32 fade);

void btlDrawActorGroundDisc(BtlUnit *unit) {
    f32 position[4] __attribute__((aligned(16)));
    f32 actorPosition[4] __attribute__((aligned(16)));
    f32 radius;
    s32 alpha;
    BtlActorStatusRecord *status;
    dds3ModelHandle_t *model;

    if (!(unit->status.flags & 8)) {
        return;
    }
    if (!(unit->status.flags & 2)) {
        return;
    }
    if (unit->modelBorrowed != 0) {
        return;
    }

    model = unit->ext->owner;
    alpha = ((u8 *)&unit->overlayColor)[3];
    if (alpha == 0) {
        return;
    }

    status = (BtlActorStatusRecord *)btlGetSideIndexedActorStatusTable(
        unit->resourceKind, unit->resourceIndex);
    switch (status->shadowKind) {
    case 0:
        return;
    case 1:
        radius = 50.0f;
        break;
    case 2:
        radius = 100.0f;
        break;
    case 3:
        radius = 200.0f;
        break;
    case 4:
        radius = 300.0f;
        break;
    default:
        radius = 0.0f;
        break;
    }

    dds3GetPosPoseSet((dds3Basic_t *)unit->effectObject);
    SDF_VU0_STOREVECTOR(actorPosition);
    if (sdfModelGetMarkPos(model->inner, 0) == 0) {
        SDF_VU0_LOADVECTOR(actorPosition);
    }
    SDF_VU0_STOREVECTOR(position);
    fldDrawShadowDisc(position[0], actorPosition[1], position[2], radius, alpha);
}

/* Requires task bit 6 clear and a matching 0x2A49 status bit.
 * The masked-status switch assigns 25-bucket tests to cases 0x40/0x400. */
s32 btlCheckStatusActionEligibility(ActionStateLink *task) {
    BtlUnit *unit = task->unit;
    u16 flags;

    if (task->pendingFlags & 0x40) {
        return 0;
    }
    flags = unit->partyRecord.status;
    if (!(flags & 0x2A49)) {
        return 0;
    }
    switch (flags & 0x7FFF) {
    /* Separate arms keep two failure predecessors through 15.ce, so the
     * zero return is not hoisted over either roll test. After allocation,
     * 27.jump2 merges their identical call tails (qualified cc1probe). */
    case 0x40:
        if ((u32)btlRollAiBucket() >= 25) {
            break;
        }
        return 1;
    case 0x400:
        if ((u32)btlRollAiBucket() >= 25) {
            break;
        }
        return 1;
    case 0x800:
        return 1;
    default:
        return 1;
    }
    return 0;
}


/* Accepts only when status bit 5 is set and task bit 6 is clear.
 * For a lone bit-5 status, compare the unsigned AI roll bucket to 70. */
s32 btlAiCheckStatusRollEligibility(ActionStateLink *task) {
    s32 result = 0;
    BtlUnit *unit = task->unit;
    u16 flags;

    if (task->pendingFlags & 0x40) {
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

extern f32 btlUnitGetTopY(BtlUnit *unit);
extern s32 nbClipEffectNum(s32 screen[4]);
/* vu0 routine: choose an on-screen HP/MP label anchor, leaving it in vf10. */
INCLUDE_RODATA(const s32, "game/code_0020E850", D_00419910);

INCLUDE_RODATA(const s32, "game/code_0020E850", D_00419920);

void btlSelectHpMpDisplayPositionVU(BtlUnit *unit) {
    s32 screen[4] __attribute__((aligned(16)));
    f32 baseline[4];
    f32 candidate[4];
    f32 distance;
    s8 visible;
    s32 x;
    s32 y;

    if (btlSetActorEffectParameter(unit, 2) != 0) {
        if (unit->status.stateFlags & 0x80) {
            return;
        }
        SDF_VU0_STOREVECTOR_NOMEM(baseline);
    } else {
        btlUnitGetMuzzlePosVU(unit);
        SDF_VU0_STOREVECTOR_NOMEM(baseline);
        baseline[1] = -btlUnitGetTopY(unit);
    }
    if (unit->status.stateFlags & 0x80) {
        SDF_VU0_LOADVECTOR(baseline);
        return;
    }
    SDF_VU0_LOADVECTOR(baseline);
    visible = nbClipEffectNum(screen);
    x = screen[0] >> 4;
    y = screen[1] >> 3;
    if (!visible || x < 12 || x >= 501 || y < 48 || y >= 324) {
        if (btlSetActorEffectParameter(unit, 1) != 0) {
            SDF_VU0_STOREVECTOR_NOMEM(candidate);
            visible = nbClipEffectNum(screen);
            x = screen[0] >> 4;
            y = screen[1] >> 3;
            if (visible == 1 && !(unit->status.stateFlags & 0x400000) &&
                x >= 12 && x < 501 && y >= 48 && y < 324) {
                btlBossDebugPrintf("btl:hpmp clip 1shot\n");
                SDF_VU0_LOADVECTOR(candidate);
                return;
            }
        }
        btlUnitGetMuzzlePosVU(unit);
        SDF_VU0_STOREVECTOR_NOMEM(candidate);
        SDF_VU0_LOADVECTOR_B(baseline);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_GETLENGTH(distance);
        if (distance < unit->reach * unit->scale * 4.0f) {
            candidate[0] = baseline[0];
            candidate[2] = baseline[2];
            candidate[1] -= unit->height * unit->scale * 0.25f;
            SDF_VU0_LOADVECTOR(candidate);
            visible = nbClipEffectNum(screen);
            x = screen[0] >> 4;
            y = screen[1] >> 3;
            if (visible == 1 && x >= 12 && x < 501 && y >= 48 && y < 324) {
                btlBossDebugPrintf("btl:hpmp clip height 3/4\n");
                SDF_VU0_LOADVECTOR(candidate);
                return;
            }
        }
        btlUnitGetMuzzlePosVU(unit);
        SDF_VU0_STOREVECTOR_NOMEM(candidate);
        visible = nbClipEffectNum(screen);
        x = screen[0] >> 4;
        y = screen[1] >> 3;
        if (visible == 1 && x >= 12 && x < 501 && y >= 48 && y < 324) {
            btlBossDebugPrintf("btl:hpmp clip cylinder center\n");
            SDF_VU0_LOADVECTOR(candidate);
            return;
        }
        candidate[1] += unit->height * unit->scale * 0.5f;
        SDF_VU0_LOADVECTOR(candidate);
        visible = nbClipEffectNum(screen);
        x = screen[0] >> 4;
        y = screen[1] >> 3;
        if (visible == 1 && x >= 12 && x < 501 && y >= 48 && y < 324) {
            btlBossDebugPrintf("btl:hpmp clip cylinder bottom\n");
            SDF_VU0_LOADVECTOR(candidate);
            return;
        }
        btlUnitGetMuzzlePosVU(unit);
        SDF_VU0_STOREVECTOR_NOMEM(candidate);
        candidate[1] -= unit->height * unit->scale * 0.5f;
        SDF_VU0_LOADVECTOR(candidate);
        visible = nbClipEffectNum(screen);
        x = screen[0] >> 4;
        y = screen[1] >> 3;
        if (visible == 1 && x >= 12 && x < 501 && y >= 48 && y < 324) {
            btlBossDebugPrintf("btl:hpmp clip cylinder top\n");
            SDF_VU0_LOADVECTOR(candidate);
            return;
        }
    }
    SDF_VU0_LOADVECTOR(baseline);
}
extern void effDrawSpriteWithCornerColors(s32, s32, s32, u32 *, s32, EffectSlotSet *, s32, s32);

/* Draw a centered decimal value, using optional per-digit offsets. */
void btlDrawCenteredUnsignedNumber(s32 x, s32 y, u32 number, u32 color, s32 *offsets) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    EffectSlotSet *set;
    BdWork *work;
    u32 digits[16];
    u32 colors[4];
    s32 digitCount = 0;
    s32 width;
    s32 height;
    s32 drawX;
    s32 baseY;
    u32 drawColor;

    while (number != 0) {
        digits[digitCount++] = number % 10;
        number /= 10;
    }
    if (digitCount == 0) {
        digits[0] = 0;
        digitCount = 1;
    }

    set = battle->resB;
    work = set->workEntries;
    width = work[4].geometry.bounds[2];
    height = work[4].geometry.bounds[3];

    drawColor = (color >> 24) | (color << 24);
    drawColor |= ((color << 8) & 0x00FF0000) |
                 ((color >> 8) & 0x0000FF00);
    colors[0] = drawColor;
    colors[1] = drawColor;
    colors[2] = drawColor;
    colors[3] = drawColor;
    drawX = x - (s32)((u32)(width * digitCount) >> 1);

    if (digitCount != 0) {
        s32 offsetIndex = -1;

        baseY = y - (height >> 1);
        do {
            offsetIndex++;
            digitCount--;
            if (offsets != NULL) {
                s32 yOffset = offsets[offsetIndex];
                if (yOffset >= 0) {
                    effDrawSpriteWithCornerColors(drawX, baseY + yOffset, -0x10, colors, 0,
                                  battle->resB, digits[digitCount] + 4, 0x5E);
                }
            } else {
                effDrawSpriteWithCornerColors(drawX, y - (height >> 1), -0x10, colors, 0,
                              battle->resB, digits[digitCount] + 4, 0x5E);
            }
            drawX += width;
        } while (digitCount != 0);
    }
}

/* Select the owner's effect variant for its current condition and task action. */
s32 btlGetStatusActionEffectId(ActionStateLink *task) {
    BtlUnit *unit = task->unit;
    switch (unit->partyRecord.status & 0x7FFF) {
    case 0x2000:
        return effOffsetIfOwnerFlagClear(unit, 0xD2);
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



extern BtlCommandEffect btlCommandEffectMappings[];
extern s32 btlCheckCommandRequiredEntryMatches(BtlIndexList *, s32);

s16 btlGetCommandEffectId(ActionStateLink *link, s32 command) {
    BtlOperandGroup *result = link->indexWork.groups;
    u32 i;
    u32 count = btlGetIndexListCount(link->indexWork.indices);
    s32 rejected = 0;
    s16 effect;
    u8 adjustSide;

    for (i = 0; i < count; i++, result++) {
        if (result->inactive) {
            rejected++;
        } else {
            switch (result->kind) {
            case 2:
            case 4:
            case 0x10000:
            case 0x40000:
                rejected++;
                break;
            default:
                if (result->reflected) {
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
        if (btlCommandEffectMappings[i].command == command) {
            effect = btlCommandEffectMappings[i].effect;
            adjustSide = btlCommandEffectMappings[i].adjustSide;
            break;
        }
    }
    if (btlCheckCommandRequiredEntryMatches(link->indexWork.indices, command) != 0) {
        if (datCommandRecords[command].requirementBits == 0x800 ||
            datCommandRecords[command].requirementBits == 0x1000) {
            effect = 0x6A;
        } else {
            effect = 0x84;
        }
    }
    effect = effOffsetIfOwnerFlagClear(link->unit, effect);
    if (adjustSide) {
        s32 ownerSide = link->unit->status.flags & 0x600;
        s32 targetSides = 0;

        count = btlGetIndexListCount(link->indexWork.indices);
        for (i = 0; i < count; i++) {
            targetSides |= ((BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, i))->status.flags & 0x600;
        }
        if (ownerSide != targetSides && targetSides != 0) {
            effect = (link->unit->status.flags & 0x200) ? effect + 1 : effect - 1;
        }
    }
    return effect;
}

/* Adds one to the base unless bit 9 of the owner's flags is set. */
s32 effOffsetIfOwnerFlagClear(BtlUnit *owner, s32 base) {
    return base + (((owner->status.flags >> 9) ^ 1U) & 1);
}

/* Number display: initializes both anchor vectors, then adds the offset before projection. */
extern f32 btlLinkedNumberScreenOffsets[12][2];
extern s32 btlWouldUiValueFallBelowQuarter(BtlUnit *, s32);

s32 btlUpdateLinkedNumberDisplay(BtlLinkedEffectArgs *args) {
    s32 screen[4] __attribute__((aligned(16)));
    s32 bounce[16];
    s32 value = args->payload.linked.value;
    u32 color;
    s32 rise;
    s32 *offsets;
    BtlUnit *unit = args->unit;
    u32 ticks;
    u64 status;
    f32 phase;
    f32 displacement;

    if (args->payload.linked.elapsedTicks == 0) {
        args->payload.linked.offsetIndex = unit->linkedNumberCount % 12;
        unit->linkedNumberCount++;
        btlSelectHpMpDisplayPositionVU(args->unit);
        SDF_VU0_STOREVECTOR_NOMEM(args->anchorOffset);
        btlUnitGetMuzzlePosVU(args->unit);
        SDF_VU0_STOREVECTOR_NOMEM(args->referencePosition);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(args->anchorOffset);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(args->anchorOffset);
        switch (args->payload.linked.kind) {
        case 0:
            if (value <= 0) {
                if ((args->unit->status.flags & 0x400) &&
                    btlWouldUiValueFallBelowQuarter(args->unit, 0)) {
                    args->payload.linked.color = 0x805050B0;
                } else {
                    args->payload.linked.color = 0x80808080;
                }
            } else {
                args->payload.linked.color = 0x8050A050;
            }
            break;
        case 1:
            args->payload.linked.color = value <= 0 ? 0x80B030B0 : 0x80A08020;
            break;
        default:
            args->payload.linked.color = 0;
            break;
        }
    }
    if (value < 0) {
        value = -value;
    }
    color = args->payload.linked.color;
    rise = 0;
    offsets = NULL;
    ticks = args->payload.linked.elapsedTicks;
    if (ticks < 5) {
        s32 remaining = ticks;
        u32 i = 0;
        if (remaining >= 0) {
            do {
                f32 fraction = (f32)remaining / 5.0f;
                phase = 1.0f - fraction;
                phase *= phase;
                displacement = phase * 34.0f;
                bounce[i] = (s32)(displacement * 8.0f);
                remaining--;
                i++;
                if (i >= 16) {
                    break;
                }
            } while (remaining >= 0);
        }
        for (; i < 16; i++) {
            bounce[i] = -1;
        }
        offsets = bounce;
    } else if (ticks >= 24) {
        phase = (f32)(s32)(ticks - 23) / 12.0f;
        phase *= phase;
        displacement = phase * 30.0f;
        rise = (s32)(displacement * 8.0f);
        color = (color & 0xFFFFFF) | ((u32)((1.0f - phase) * 128.0f) << 24);
    }
    status = btlUnitStatusPair(args->unit);
    rise += 128;
    if (status & 0x08000004) {
        if ((status & 0x21) == 1) {
            btlUnitGetMuzzlePosVU(args->unit);
        } else {
            SDF_VU0_LOADVECTOR(args->referencePosition);
        }
        SDF_VU0_LOADVECTOR_B(args->anchorOffset);
        SDF_VU0_ADDVECTOR();
        if (nbClipEffectNum(screen) != 0) {
            screen[0] += (s32)(btlLinkedNumberScreenOffsets[args->payload.linked.offsetIndex][0] * 40.0f) << 4;
            screen[1] += ((s32)(btlLinkedNumberScreenOffsets[args->payload.linked.offsetIndex][1] * 40.0f) << 3) + 320;
            btlDrawCenteredUnsignedNumber(screen[0], screen[1] - rise, value, color, offsets);
        }
    }
    args->payload.linked.elapsedTicks++;
    return args->payload.linked.elapsedTicks >= 36;
}


/* Decrement the linked-number task's unit counter without underflowing zero. */
void effDecrementFirstCountdown(BtlLinkedEffectArgs *args) {
    BtlUnit *unit = args->unit;
    u8 count = unit->linkedNumberCount;
    if (count != 0) {
        unit->linkedNumberCount = count - 1;
    }
}

/* Create the selected numbered-display task with its frame count starting at zero. */
BtlRuntimeTask *btlCreateLinkedEffectTask(BtlUnit *owner, s32 value, u8 kind) {
    BtlRuntimeTask *task = btlAllocTask(0x34);
    BtlLinkedEffectArgs *args;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    switch (kind) {
    case 0:
        task->taskId = 0x3C;
        break;
    case 1:
        task->taskId = 0x3D;
        break;
    }
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->ownerId = owner->owner;
    task->callback = btlUpdateLinkedNumberDisplay;
    task->onFinish = effDecrementFirstCountdown;
    args = btlGetTaskArguments(task);
    args->payload.linked.kind = kind;
    args->unit = owner;
    args->payload.linked.value = value;
    args->payload.linked.elapsedTicks = 0;
    return task;
}

/* Counter display shares the same vector prefix, with the shorter counter payload. */
s32 func_0020FA98(BtlLinkedEffectArgs *args) {
    u64 status;
    BtlState *battle;
    EffectSlotSet *set;
    s32 kind;
    s32 slot;
    s32 mode;
    s32 width;
    s32 height;
    u32 ticks;
    s32 animationOffset;
    f32 phase;
    f32 displacement;
    u32 packedColor;
    s32 screen[4] __attribute__((aligned(16)));
    u32 colors[4];

    if (!(args->unit->status.flags & 2)) {
        return 1;
    }

    slot = 3;
    battle = (BtlState *)btlGetRuntime();
    kind = args->payload.counter.kind;
    mode = 0;
    if (kind != 2) {
        if (kind < 3) {
            slot = 2;
            if (kind != 1) {
                slot = 0;
            }
        } else {
            slot = 0;
            mode = 1;
            if (kind != 3) {
                if (kind == 4) {
                    slot = 1;
                } else {
                    mode = 0;
                }
            }
        }
    }

    set = battle->resB;
    width = set->workEntries[slot].geometry.bounds[2];
    height = set->workEntries[slot].geometry.bounds[3];

    if (args->payload.counter.elapsedTicks == 0) {
        args->payload.counter.offsetIndex = args->unit->counterDisplayCount % 12;
        args->unit->counterDisplayCount++;

        btlSelectHpMpDisplayPositionVU(args->unit);
        SDF_VU0_STOREVECTOR_NOMEM(args->anchorOffset);
        btlUnitGetMuzzlePosVU(args->unit);
        SDF_VU0_STOREVECTOR_NOMEM(args->referencePosition);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(args->anchorOffset);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(args->anchorOffset);
    }

    ticks = (u32)args->payload.counter.elapsedTicks;
    animationOffset = 0;
    packedColor = 0x80808080;
    if (mode == 0) {
        if (ticks < 7) {
            phase = 1.0f - (f32)ticks / 7.0f;
            phase *= phase;
            displacement = phase * 40.0f;
            animationOffset = -(s32)(displacement * 8.0f);
        } else if (ticks >= 22) {
            phase = (f32)(u32)(ticks - 21);
            phase *= 0.125f;
            phase *= phase;
            displacement = phase * 36.0f;
            animationOffset = (s32)(displacement * 8.0f);
            packedColor = (packedColor & 0xFFFFFF00) | ((u32)((1.0f - phase) * 128.0f) & 0xFF);
        }
    } else {
        phase = 0.0f;
        if (ticks >= 22) {
            phase = (f32)(u32)(ticks - 21);
            phase *= 0.125f;
        }
        packedColor = (packedColor & 0xFFFFFF00) | ((u32)((1.0f - phase) * 128.0f) & 0xFF);
        animationOffset = (s32)(ticks << 3);
        animationOffset -= 0x120;
    }
    animationOffset += 0x120;
    status = btlUnitStatusPair(args->unit);
    if (status & 0x08000004) {
        if ((status & 0x21) == 1) {
            btlUnitGetMuzzlePosVU(args->unit);
        } else {
            SDF_VU0_LOADVECTOR(args->referencePosition);
        }
        SDF_VU0_LOADVECTOR_B(args->anchorOffset);
        SDF_VU0_ADDVECTOR();

        if (nbClipEffectNum(screen)) {
            screen[0] += (s32)(btlLinkedNumberScreenOffsets[args->payload.counter.offsetIndex][0] * 40.0f) << 4;
            screen[1] += ((s32)(btlLinkedNumberScreenOffsets[args->payload.counter.offsetIndex][1] * 40.0f) << 3) + 320;

            colors[0] = packedColor;
            colors[1] = packedColor;
            colors[2] = packedColor;
            colors[3] = packedColor;
            set = battle->resB;
            effDrawSpriteWithCornerColors(screen[0] - (width >> 1),
                          screen[1] - (height >> 1) - animationOffset,
                          -0x10, colors, 0, set, slot, 0x5E);
        }
    }

    args->payload.counter.elapsedTicks++;
    return args->payload.counter.elapsedTicks >= 0x1E;
}

/* Decrement the counter-display task's unit counter without underflowing zero. */
void effDecrementSecondCountdown(BtlLinkedEffectArgs *args) {
    BtlUnit *unit = args->unit;
    u8 count = unit->counterDisplayCount;
    if (count != 0) {
        unit->counterDisplayCount = count - 1;
    }
}

/* Create the counter-display task; its kind is stored as a byte. */
BtlRuntimeTask *btlCreateEffectCounterTask(BtlUnit *owner, s32 kind) {
    BtlRuntimeTask *task = btlAllocTask(0x2C);
    BtlLinkedEffectArgs *args;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
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

s32 btlPollActorOrEntryLabelTask(BtlEffLink *link) {
    BtlState *battleState = (BtlState *)btlGetRuntime();
    BtlUnit *owner = link->owner;

    if (link->elapsedTicks == 0) {
        s32 resourceIndex = link->arg;

        if (resourceIndex == 0) {
            if ((btlUnitStatusPair(owner) & 0x1400) == 0 && (owner->partyRecord.flags & 0x10) == 0) {
                btlCreateSkillNamePanel(btlGetIndexedUiResource(owner));
            } else if (battleState->commandRestrictFlags & 0x400) {
                if (owner->status.flags & 0x400) {
                    btlCreateSkillNamePanel(D_003BEB60[0]);
                } else {
                    btlCreateSkillNamePanel(D_003BEB58[0]);
                }
            } else {
                btlCreateSkillNamePanel(D_003BEB58[0]);
            }
        } else {
            btlCreateSkillNamePanel(datSkillNames + resourceIndex * 17);
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

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->taskId = 0x43;
    task->ownerId = owner->owner;
    task->callback = btlPollActorOrEntryLabelTask;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = resourceIndex;
    link->elapsedTicks = 0;
    return task;
}

s32 btlPollTimedTaskLink(BtlEffLink *link) {
    btlGetRuntime();
    if (link->elapsedTicks == 0) {
        btlCreateSkillNamePanel(D_00435E5C + link->arg * 25);
    }
    if (btlHasRegisteredSkillNamePanelTask() == 0 || link->elapsedTicks >= 0x1E) {
        return 1;
    }
    link->elapsedTicks++;
    return 0;
}

BtlRuntimeTask *btlCreateOwnerLinkedTimedTask(BtlUnit *owner, s32 skillNameIndex) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    BtlEffLink *link;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->taskId = 0x44;
    task->ownerId = owner->owner;
    task->callback = btlPollTimedTaskLink;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = skillNameIndex;
    link->elapsedTicks = 0;
    return task;
}

s32 btlPollCategoryLabelTask(BtlEffLink *link) {
    BtlUnit *owner;

    btlGetRuntime();
    owner = link->owner;
    if (link->elapsedTicks == 0) {
        switch (link->arg) {
        case 10:
            btlCreateSkillNamePanel(btlActionPassLabel);
            break;
        case 5: {
            u32 flags = owner->status.flags;
            if (flags & 0x200) {
                if (!(flags & 0x1000)) {
                    btlCreateSkillNamePanel(D_003BEB38[0]);
                    break;
                }
            }
            btlCreateSkillNamePanel(D_003BEB48[0]);
            break;
        }
        case 6:
            btlCreateSkillNamePanel(btlActionRetreatLabel);
            break;
        case 9:
            btlCreateSkillNamePanel(D_003BEB4C[0]);
            break;
        case 11:
            btlCreateSkillNamePanel(D_003BEB50[0]);
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

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x45;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
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

s32 btlPollActorDialogTask(BtlEffLink *link) {
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

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x3F;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
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

s32 btlUpdateLinkedDialogueEffect(BtlEffLink *link) {
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

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x40;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
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

/* Return completion on the first poll; this effect task has no per-frame work. */
s32 btlCompleteEffectTaskImmediately(void *unused) {
    return 1;
}

BtlRuntimeTask *btlCreateEffectTask3E(BtlUnit *owner, u16 messageId) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    BtlEffLink *link;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->taskId = 0x41;
    task->ownerId = owner->owner;
    task->callback = btlCompleteEffectTaskImmediately;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->shortArg = messageId;
    link->elapsedTicks = 0;
    return task;
}

s32 btlPollTimedPresentationTask(BtlEffLink *link) {
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

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->taskId = 0x42;
    task->ownerId = owner->owner;
    task->callback = btlPollTimedPresentationTask;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = messageId;
    link->elapsedTicks = 0;
    return task;
}

s32 btlPollEffectWaitTask(BtlEffLink *link) {
    BtlState *battleState = (BtlState *)btlGetRuntime();

    if (link->elapsedTicks == 0) {
        if (link->owner != 0) {
            itfMesSetTextSlotFromValue(battleState->messageWindows[0], 0, link->shortArg, 0xD);
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

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x46;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    if (owner != NULL) {
        task->ownerId = owner->owner;
    }
    task->callback = btlPollEffectWaitTask;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->shortArg = mode;
    link->elapsedTicks = 0;
    return task;
}

/* Waits for the task startup delay, then finishes when its two actor slots are clear. */
s32 btlWaitEffectTask(BtlAnalysisPanelWaitArgs *task) {
    if (task->elapsedTicks == 0) {
        btlCreateAnalysisPanelTask(task->panelEntry, 0);
        btlSetCommandPanelFadeMode(0, 8);
    }
    if (task->elapsedTicks >= 0x11) {
        if (btlGetRegisteredTaskValueOrDefault() == 1) {
            if (dds3PadButtonState[0x21] < 0 || dds3PadButtonState[0x23] < 0) {
                btlSetCommandPanelFadeMode(1, 8);
                btlRequestAnalysisPanelClose();
                return 1;
            }
        }
    } else {
        task->elapsedTicks += 1;
    }
    return 0;
}

BtlRuntimeTask *btlCreateEffectTask44(BtlUnit *owner) {
    BtlRuntimeTask *task = btlAllocTask(8);
    BtlEffLink *link;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->taskId = 0x47;
    task->ownerId = owner->owner;
    task->callback = btlWaitEffectTask;
    link = btlGetTaskArguments(task);
    link->owner = owner;
    link->arg = 0;
    return task;
}

s32 btlAdvanceActorEffectLabelTask(BtlEffLink *link) {
    BtlState *battleState = (BtlState *)btlGetRuntime();
    BtlUnit *owner = link->owner;
    char text[0x100];

    if (link->elapsedTicks == 0) {
        if (owner != 0) {
            itfMesSetTextSlotFromValue(battleState->messageWindows[0], 0, owner->partyRecord.unitId, (owner->partyRecord.flags & 0x20) ? 0xE : 0xF);
            sprintf(text, D_00436CA8, link->arg < 0 ? -link->arg : link->arg);
            itfMesMngSetVarImmediate(battleState->messageWindows[0], 1, text);
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

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x48;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
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

extern s32 scrGetIntPara(s32 index);
extern BtlUnit *btlFindUnitByModeClear(s32 id);
extern BtlUnit *btlFindUnitByModeFlagged(s32 id);
extern BtlRuntimeTask *btlCreateEffObjB(BtlUnit *actor, s32 arg);
extern u64 btlStartTask(void *);
extern u64 btlAdvanceRuntimeSequenceCounter(void);

s32 btlCmdSpawnEffectTaskForSelectedUnit(void) {
    s32 mode;
    s32 unitId;
    s32 arg;
    BtlUnit *unit;
    BtlRuntimeTask *task;

    mode = scrGetIntPara(0);
    unitId = scrGetIntPara(1);
    arg = scrGetIntPara(2);
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

s32 btlAllocAndCheck(ActionStateLink *task) {
    BtlAiScratchWork *work = sdfAllocAndClearQuadwords(sizeof(BtlAiScratchWork));
    BtlUnit *actor = task->unit;

    btlActionScratchWork = work;
    work->actor = task;
    if (btlIsLowHpActionReady(actor, 0) != 0) {
        sdfChipFreeDirect((void *)btlActionScratchWork);
        return 1;
    }
    sdfChipFreeDirect((void *)btlActionScratchWork);
    return 0;
}

u32 btlAssignTaskResultAndArgument(ActionStateLink *task) {
    task->indexWork.phase = 0xb;
    task->indexWork.skillId = 0xc2;
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
    BtlAiScratchWork *work = sdfAllocAndClearQuadwords(sizeof(BtlAiScratchWork));
    s32 result;

    btlActionScratchWork = work;
    work->speciesId = owner->partyRecord.unitId;
    work->actor = (ActionStateLink *)context;
    result = btlDispatchPackedEffectAction((s32)owner, mask);
    sdfChipFreeDirect((void *)btlActionScratchWork);
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

extern AiSpecies *datEnemyAiRecords;
extern void btlSelectTierMatrixAiRoute(BtlUnit *unit, s32 species, s32 *row, s32 requestedRow);

/* Choose a row and a weighted slot of the unit's species AI table and run that action. */
s32 btlRunRandomWeightedAiTableAction(ActionStateLink *task) {
    BtlAiScratchWork *work = sdfAllocAndClearQuadwords(sizeof(BtlAiScratchWork));
    BtlUnit *unit;
    u16 species;
    s32 row;
    s32 index;

    unit = task->unit;
    btlActionScratchWork = work;
    species = unit->partyRecord.unitId;
    work->actor = task;
    work->speciesId = species;
    btlSelectTierMatrixAiRoute(unit, species, &row, 0);
    index = btlPickWeightedAiSlot(unit, species, row);
    btlExecuteAiTableAction(task, datEnemyAiRecords[species].slot[row * 5 + index].actionId,
                  datEnemyAiRecords[species].slot[row * 5 + index].actionArg);
    sdfChipFreeDirect((void *)btlActionScratchWork);
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

/* Choose command-record-driven targets; retain the native zero mode argument. */
void btlChooseCommandActionTargets(ActionStateLink *context) {
    func_002152D8(context, 0);
}

/* Delegate minimum nonzero-current-HP selection for this command, with mode zero. */
void btlPickCommandLowestHpTarget(ActionStateLink *context) {
    btlSelectLowestNonzeroHpTarget(context, 0);
}

/* Delegate minimum HP-percentage selection without changing native fallback modes. */
void btlPickCommandLowestHpRatioTarget(ActionStateLink *context) {
    btlSelectLowestHealthRateTarget(context, 0);
}

/* Forward mode selection. */
void btlSelectCommandTargetsByMode(ActionStateLink *task, s32 mode) {
    btlSelectTargetsByMode(task, mode);
}

/* Append the selected/current actor pointer with the native zero mode argument. */
void btlCmdAppendCurrentUnitIdToCommandIndices(ActionStateLink *context) {
    btlAppendSelectedOrCurrentActorTarget(context, 0);
}

extern void btlSelectLowestHealthElementBlockTarget(ActionStateLink *, s32);

/* Select a lowest-HP element-block candidate using the supplied query input. */
void btlPickCommandLowestHpBlockTarget(ActionStateLink *context, s32 value) {
    btlSelectLowestHealthElementBlockTarget(context, value);
}

/* Forward command selection with its native zero second argument. */
void btlPickCommandLowestLevelTarget(ActionStateLink *context) {
    btlSelectLowestRankTarget(context, 0);
}

/* Select the highest-current-MP candidate through the native mode-zero entry. */
void btlPickCommandHighestMpTarget(ActionStateLink *context) {
    btlSelectHighestCurrentMpTarget(context, 0);
}

extern s32 btlSelectTargetsPassingCheck(ActionStateLink *, s32);

/* Select by group element checks; action is the query input, not a status mask. */
void btlPickCommandElementCheckedTarget(ActionStateLink *commandContext, s32 action) {
    btlSelectTargetsPassingCheck(commandContext, action);
}

extern void btlSelectTargetsByActionMask(ActionStateLink *, s32);

/* Select by status-mask presence, preserving native fallback and list-copy modes. */
void btlPickCommandTargetWithStatusMask(ActionStateLink *context, s32 value) {
    btlSelectTargetsByActionMask(context, value);
}

extern void btlSelectTargetsWithoutActionMask(ActionStateLink *, s32);

/* Select by status-mask absence, preserving native fallback and list-copy modes. */
void btlPickCommandTargetWithoutStatusMask(ActionStateLink *context, s32 value) {
    btlSelectTargetsWithoutActionMask(context, value);
}

extern s32 btlFilterTargetListByUnitId(ActionStateLink *, s32);

/* Filter the command's target list by the requested unit ID. */
void btlFilterCommandTargetsByUnitId(ActionStateLink *context, s32 value) {
    btlFilterTargetListByUnitId(context, value);
}

/* Current-command adapter for the unlinked-party selector. */
void btlPickUnlinkedCommandTarget(ActionStateLink *task, u32 input) {
    btlPickUnlinkedPartyTarget(task, input);
}

/* Current-command adapter for the linked-party selector. */
void btlPickLinkedCommandTarget(ActionStateLink *task, u32 input) {
    btlPickLinkedPartyTarget(task, input);
}

/* Append the command's own actor after the native scan. */
void btlAppendActorToCommandTargets(ActionStateLink *battle, s32 unused) {
    btlAppendSelfAfterTargetScan(battle);
}

/* Default selection excludes the actor, with native self fallback; unused stays unused. */
void btlPickCommandTargetExcludingActor(ActionStateLink *context, s32 unused) {
    btlSelectTargetsExcludingActorUnit(context);
}

/* Append the global effect actor, distinct from this command's own actor. */
void btlAddEffectActorTargetToCommand(ActionStateLink *context) {
    btlAppendEffectActorToCommandIndices(context, 0);
}

extern void btlSelectTargetsBlockingElement(ActionStateLink *, s32);

/* Select element-block candidates using the supplied query input. */
void btlPickCommandElementBlockTarget(ActionStateLink *context, s32 value) {
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
