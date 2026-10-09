#include "btl_motion_transform.h"
#include "common.h"
#include "btl_stage_task_cleanup.h"
#include "mdl_motion_api.h"
#include "sdf_motion.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "btl_effect_position.h"
#include "sdf_chip.h"
#include "snd_slot.h"
#include "btl_task_state.h"
#include "btl_task_condition.h"
#include "sdf_resource.h"
#include "sdf_model.h"
#include "btl.h"
#include "sdf_texture_offset_list.h"
#include "btl_model_record.h"
#include "btl_command.h"
#include "btl_state.h"
#include "btl_task_args.h"
#include "btl_sound.h"
#include "eff_field_color.h"
#include "file.h"
#include "eff_update_flags.h"
#include "sdf.h"
#include "btl_action.h"
#include "btl_unit_tasks.h"
#include "dds3obj.h"
#include "evt_unit.h"
#include "eff_transform.h"
#include "eff_object.h"
#include "mdl.h"
#include "mdl_asset_request.h"
#include "file_request_api.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "fpu.h"
#include "dat_state.h"
#include "dat_command.h"

/* SYSEFF metadata and runtime registrations share these indices. */
enum {
    BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT = 0x26
};


extern void btlFlagMatchingUnitsDefeatCandidate(s32);

extern void btlClearAllUnitDefeatCandidates(void);

extern void func_001F3C30(BtlLinkedCommand *action);

extern s32 btlIsUnitInActiveList(void *unit);

extern void btlBossDebugPrintfN(s32, s32, s32, const char *, ...);

extern f32 effMiscRandUnitFloat(void *state);

extern u8 effSharedRandomState[];

extern void func_001E38F0(BtlUnit *, MdlCtx *, SdfModel *, SdfPoolNode **, u32);

extern void dds3ClearObjectFlags(void *, u32);

extern struct SdfPoolNode *D_00380788[13][4];

extern SdfPoolNode *D_003B6BD0[];

extern void func_001ECBF8();

typedef struct BtlUnit BtlUnit;

/* Motion selection and approach tasks share these 0x14-byte resource nodes. */
typedef struct BtlEffectNode {
    s16 triggerKind;
    u8 pad02[2];
    s16 rateKind;
    u8 pad06[2];
    f32 scale;
    f32 reachOffset;
    u8 pad10[2];
    u16 frameCount;
} BtlEffectNode;

typedef struct BtlEffectResource {
    s128 vec0;
    f32 f10;
    f32 f14;
    f32 f18;
    f32 f1C;
    f32 f20;
    u8 pad24[6];
    u16 unk2A;
    BtlEffectNode nodes[1];
} BtlEffectResource;

/* Metadata records reached through the battle table pointers. */
typedef struct BtlActionTableEntry {
    u8 kind;               /* 0x00 */
    u8 pad01[2];
    u8 resourceType;       /* 0x03 */
    u8 pad04[4];
    f32 lightColorMode;
    f32 lightColor[3];
    s32 defaultValue;      /* 0x18 */
    u16 flags;             /* 0x1C */
    u8 pad1E[2];
} BtlActionTableEntry;

typedef struct BtlFxSrcA {
    f32 f0;
    f32 f4;
    f32 f8;
    f32 fC;
    f32 f10;
    f32 f14;
} BtlFxSrcA;

typedef struct FxTask {
    u8 pad0[0x10];
    s32 unk10;
    BtlUnit *unit;
} FxTask;

extern s128 D_003B6B80;

typedef struct BtlCameraTaskArgs {
    u32 kind;
    f32 component[8]; /* camera origin/direction inputs, offsets 0x04..0x20 */
} BtlCameraTaskArgs;

typedef struct BtlVectorTaskArgs {
    u8 pad00[0x20];
    f32 scale;
    u32 state24;
    u32 state28;
    union {
        u32 unit2C;
        s8 mode2C;
    };
    u32 unit30;
} BtlVectorTaskArgs;

typedef struct BtlCommandOption {
    u8 pad00[0xC];
    s32 kind;           /* 0x0C */
    u8 pad10[4];
    u8 inactive;        /* 0x14 */
} BtlCommandOption;

typedef struct BtlCommandArgument {
    s32 command;        /* 0x00 */
    s32 index;          /* 0x04 */
    u8 pad08[0x38];
    struct BtlIndexList *actorIndices;   /* 0x40 */
    u8 pad44[0x24];
    BtlCommandOption *option; /* 0x68 */
} BtlCommandArgument;

typedef struct BtlActiveSlot {
    u8 pad00[0x18];
    void *unit;         /* 0x18 */
} BtlActiveSlot;

extern u32 dds3AdvanceWorldCounter(void);

extern s8 btlSetActorEffectParameter(BtlUnit *, s32);

extern s32 mdlFlagTest(u32);

extern s32 btlGetRuntime(void);

extern void func_001AA850(void *, s32);


extern void mdlSetAmountOnAllContextResources(MdlCtx *, f32);

extern struct BtlRuntimeTask *btlDeferredTaskTail;

extern struct BtlRuntimeTask *btlDeferredTaskHead;

extern u32 kwlnDrawControlFlags;

extern void evtInitializeUnitColorTransition(EvtUnit *, s32, u32, u32);

extern s32 datActionAnimationRecords;

typedef struct ActiveSoundNode {
    u32 flags;
    u8 unk_04[8];
    struct ActiveSoundNode *previous;
    struct ActiveSoundNode *next;
} ActiveSoundNode;

extern s32 btlCountTasksForOwner(s64);

extern void btlRunTask(BtlRuntimeTask *);

extern void btlBossDebugPrintf(const char *format, ...);

extern f32 func_00208000(s32, f32 *, f32 *);

extern s32 func_001E3230(BtlUnit *, s32);

extern s32 func_0035C860(char *, const char *, ...);

extern u32 btlApplyDeferredUnitStatus(void *);

extern void btlApplyScaledUnitEffectParameter(BtlUnit *, s32, s32, f32);

typedef struct {
    union {
        void *actor;
        s32 value;
    };
    union {
        s32 option;
        u16 optionId;
        f32 scale;
    };
    u32 unk_08;
    union {
        u32 unk_0C;
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
} SoundTaskArgs;

extern BtlRuntimeTask *btlAllocTask(s32);

extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);

extern u8 D_0037F510[];

extern char D_004178A8[];

extern char D_004178B8[];

extern char D_00417AF0[];

extern char D_00417B10[];

extern void btlBindUnitModel(u8 *, u32, u32);

extern char D_00417B30[];

extern s32 btlCheckModelAssetByMode(u8 *, u32, u32);

extern void func_001E8258(s32, s32, s32, s32, s32);

extern void btlSetEffectCameraKeys(s32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

extern void effMiscQuaternionToMatrixVU(void);

extern void effMiscQuaternionNlerpVU(f32);

extern void btlClearRuntimeFlag2000(void);

extern u8 D_003E9130[];

extern void btlBuildApproachCamera(BtlLinkedCommand *, BtlCamState *);

extern void btlUpdateActionTargetCameraPose(BtlLinkedCommand *);

extern void func_001F3E48(BtlLinkedCommand *);

extern void btlAdvanceCursorForUnmarkedUnit(BtlLinkedCommand *, BtlCamState *);

extern f32 *D_0037F770[];

extern u8 kwlnDefaultColorVector[];

extern f32 *D_0037F770[];

extern s32 btlDoesEnabledStatusMatchCurrentId(DatPartyRecord *, u32);

extern void btlUnitGetMuzzlePosVU(BtlUnit *);

extern s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *);

extern u16 btlRefreshUnitMaximumHpAndClampCurrentHp(DatPartyRecord *);

extern u16 btlRefreshUnitMaximumMpAndClampCurrentMp(DatPartyRecord *);

extern void evtSetTransitionMotionScale(EvtUnit *, f32);

extern s32 btlIsCurrentValueBelowQuarterThreshold(BtlUnit *);

extern s32 btlTestActorStatusPredicate(BtlUnit *);

extern s32 btlIsUnitDefeatTriggeredByValueDelta(BtlUnit *, s32);

extern void btlApplyUnitModelScaledValue(u8 *);

extern s32 btlIsActorModeAcceptedByBattleHook(BtlUnit *);

extern s32 btlGetSideIndexedActorStatusTable(s32, s32);

extern void btlApplyUnitMotionSelection(BtlUnit *, u32, s32, f32);

extern s32 btlGetSlotRateKind(u8 *, s32);

extern BtlRuntimeTask *btlCreateStiffenDamageShakeTask(BtlUnit *, f32);

extern void evtPrepareUnitMotionState(EvtUnit *, s32, s32, s32, s32);

extern void evtStoreUnitMotionShortParameters(EvtUnit *, s32, s32);

extern void btlRefreshUnitMotionSelection(BtlUnit *);

extern void btlClearActorSelectedEntryIndex(BtlUnit *);

extern void btlClearAllActorEntrySlots(BtlUnit *);

extern void btlReleaseUnitResources(BtlUnit *);

extern void btlInitUnitFxDefaults(BtlUnit *);

extern s32 btlCheckSpecialAbility(DatPartyRecord *, s32);

extern void func_001AA868(void *, s32);

extern void func_001ADFE0(BtlUnit *, u32, s16);

extern s32 btlCountTasksByKind(u16 kind);

extern s32 btlHasSingleLinkedResource(BtlLinkedCommand *);

extern void btlResetCameraMotion(BtlLinkedCommand *);

extern s32 func_001FF5D8(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvanceCursorForUnmarkedUnit(BtlLinkedCommand *, BtlCamState *);

extern void btlDispatchStateHandler(void *obj, s32 kind);

extern s32 btlGetLoggedIndexedCommandItem(s32);

extern void scrSetGlobalBitFlag(u32);

/* A hook result of -1 leaves dispatch to the command-kind handler. */
void func_001DD390(u8 *command, u8 *argument) {
    s32 (*handler)(s32, s32) = ((BtlState *)btlGetRuntime())->commandHook;

    if (handler != 0) {
        s32 result = handler((s32)command, (s32)argument);
        if (result != -1) {
            btlDispatchStateHandler(command, result);
            return;
        }
    }
    switch (*(s32 *)argument) {
    case 1:
        btlDispatchStateHandler(command, 0xD);
        break;
    case 4:
        *(s32 *)(argument + 4) = btlGetLoggedIndexedCommandItem(*(s32 *)(argument + 8));
        /* fallthrough */
    case 2:
    case 3:
    case 7:
    case 8:
        if (datCommandSelectors[*(s32 *)(argument + 4)].kind != 1) {
            btlDispatchStateHandler(command, 0xE);
        } else {
            if ((u32)((ActionStateLink *)command)->unit->status.flags & 0x200) {
                scrSetGlobalBitFlag(*(u16 *)(argument + 4));
            }
            btlDispatchStateHandler(command, 0xF);
        }
        break;
    case 5: {
        u32 flags = (u32)((ActionStateLink *)command)->unit->status.flags;
        if (flags & 0x200) {
            if (flags & 0x1000) {
                btlDispatchStateHandler(command, 0x10);
            } else {
                btlDispatchStateHandler(command, 0x11);
            }
        } else {
            btlDispatchStateHandler(command, 0x13);
        }
        break;
    }
    case 10:
    case 13:
    case 14:
    case 18:
        btlDispatchStateHandler(command, 0x14);
        break;
    case 9:
        if ((u32)((ActionStateLink *)command)->indexWork.linkedUnit->status.flags & 1) {
            btlDispatchStateHandler(command, 0x17);
        } else {
            btlDispatchStateHandler(command, 0x16);
        }
        break;
    case 12:
        btlDispatchStateHandler(command, 0x16);
        break;
    case 6: {
        u32 flags = (u32)((ActionStateLink *)command)->unit->status.flags;
        if (flags & 0x200) {
            btlDispatchStateHandler(command, 0x18);
        } else if (flags & 0x400) {
            btlDispatchStateHandler(command, 0x15);
        }
        break;
    }
    case 11:
        btlDispatchStateHandler(command, 0x15);
        break;
    case 15:
        btlDispatchStateHandler(command, 0x19);
        break;
    case 16:
        btlDispatchStateHandler(command, 0x1A);
        break;
    case 17:
        btlDispatchStateHandler(command, 0x20);
        break;
    }
}

s32 btlIsSupportedCommandKind(s32 *state) {
    switch (*state) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
    case 9:
        return 1;
    default:
        return 0;
    }
}

s32 btlResolveActionOperand(BtlUnit *unit, s32 *argument) {
    s32 value;
    switch (argument[0]) {
    case 1:
        if ((btlUnitStatusPair(unit) & 0x1200) == 0x200 && (unit->partyRecord.flags & 0x10) == 0) {
            return btlGetActorBedAssetIdFromIndex(unit->partyRecord.menuValue);
        }
        if (argument[1] > 0) {
            return argument[1];
        }
        value = func_001B5688();
        if (value > 0) {
            return value;
        }
        return 0;
    case 4:
        return btlGetLoggedIndexedCommandItem(argument[2]);
    case 2:
    case 3:
    case 7:
    case 8:
        return argument[1];
    default:
        return -1;
    }
}

u32 btlClassifyActionOperand(BtlUnit *unit, u8 *argument) {
    switch (((BtlCommandArgument *)argument)->command) {
    case 1: {
        u32 count = btlGetIndexListCount(((BtlCommandArgument *)argument)->actorIndices);
        if ((unit->status.flags & 0x200) && ((unit->status.flags & 0x1000) || (unit->partyRecord.flags & 0x10)) &&
            (unit->partyRecord.status & 0x1000) == 0 && count == 1) {
            BtlCommandOption *option = ((BtlCommandArgument *)argument)->option;
            if (option->kind == 2 && option->inactive == 0) {
                return 0x17;
            }
        }
        return 3;
    }
    case 4:
        return (unit->status.flags & 0x200) ? 0xC : 4;
    case 2:
    case 3:
    case 7:
    case 8: {
        s32 index = ((BtlCommandArgument *)argument)->index;
        if (index == 0xD6 && (btlUnitStatusPair(unit) & 0x1200) == 0x200 && (unit->partyRecord.flags & 0x10) == 0) {
            return 0xC;
        }
        return ((BtlActionTableEntry *)datActionAnimationRecords)[index].kind;
    }
    default:
        return 0;
    }
}

s32 btlClassifyActionResult(BtlUnit *actor, u32 arg1, s32 arg2, u32 arg3, s32 arg4, u8 arg5, s32 commandIndex) {
    s32 resultCode;

    btlGetEntryFlagsUnlessDisabled(&actor->partyRecord);
    if (commandIndex >= 0) {
        switch (datCommandRecords[commandIndex].unk30) {
        case 1:
        case 2:
        case 9:
        case 10:
            return -1;
        case 22:
            if (actor->status.flags & 0x200) {
                return 0x12;
            }
            break;
        }
    }
    if ((datCommandRecords[commandIndex].attribute.bits & 0x400000FF) == 0x40000002) {
        return -1;
    }
    if (arg1 & 0x50004) {
        return -1;
    }
    if (arg3 & 0xE0001) {
        resultCode = -1;
    } else if ((actor->status.flags & 0x200) != 0 && arg2 == 2 && arg4 == 1 && arg5 == 0) {
        resultCode = 0x12;
    } else {
        resultCode = 1;
    }
    if (arg5 != 0 && (btlUnitStatusPair(actor) & 0x4000000200) == 0x200) {
        resultCode = 0xB;
    }
    if ((arg1 & 0x20001) == 0) {
        resultCode = -1;
    }
    return resultCode;
}

/* Test whether the operand is empty, subject to command-category and slot-kind exclusions. */
s32 btlActionEntryIsEmpty(s32 index, BtlOperandGroup *slot, BtlOperandEntry *entry) {
    s32 kind;

    if (index >= 0) {
        switch (index) {
        case 0x109:
        case 0x179:
        case 0x19C:
        case 0x1A5:
            return 0;
        }
        switch (datCommandRecords[index].unk30) {
        case 1:
        case 2:
        case 9:
        case 10:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
            return 0;
        }
    }
    if (slot != 0) {
        kind = slot->kind;
        if (kind == 2 || kind == 0x10000) {
            return 0;
        }
    }
    if ((entry->flags & 1) != 0) {
        return 0;
    }
    if ((entry->flags & 2) != 0) {
        return 0;
    }
    if (entry->hpDelta == 0) {
        if (entry->mpDelta == 0) {
            if (entry->addedStatus == 0) {
                if (entry->removedStatus == 0) {
                    if (entry->hpRecovery == 0) {
                        if (entry->mpRecovery == 0) {
                            if (entry->entryChangeMask == 0) {
                                if (entry->entrySelection == 0) {
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/* Return whether any operand in the live groups has flag bit zero set. */
s32 func_001DDAD0(s32 unused, BattleIndexWork *state) {
    u32 groupIndex;
    u32 entryIndex;
    u32 groupCount = btlGetIndexListCount(state->indices);
    BtlOperandGroup *group = state->groups;

    for (groupIndex = 0; groupIndex < groupCount; groupIndex++, group++) {
        u32 entryCount = group->count;

        for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
            if (group->entries[entryIndex].flags & 1) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001DDB60);

/* Reset work status and group headers, preserving the retained payload and allocation. */
void btlResetIndexWork(BattleIndexWork *work) {
    u32 i;
    work->phase = -1;
    work->skillId = -1;
    work->reference = -1;
    work->companionA = 0;
    work->companionB = 0;
    work->linkedUnit = 0;
    work->unk18 = -1;
    work->stageValue = 0;
    work->unk20 = 8;
    work->stage = 0;
    work->parameter = 0;
    work->wait = -1;
    work->unk3C = 0;
    work->unk2D = 0;
    work->unk2E = 0;
    work->unk50 = 0;
    work->unk54 = 0;
    work->unk58 = 0;
    work->flags = 0;
    work->unk64 = 0;
    work->unk60 = 0;
    for (i = 0; i < 13; i++) {
        work->groups[i].count = 0;
        work->groups[i].kind = 0;
        work->groups[i].reflected = 0;
        work->groups[i].inactive = 0;
    }
    btlClearIndexList(work->indices);
}

/* Allocate the index list and retained groups, then initialize their headers. */
void btlInitBattleIndexWork(BattleIndexWork *work) {
    struct SdfMemBlock *allocation;
    work->indices = btlAllocateIndexList(13);
    allocation = sdfAllocGeneralBlock(0x48EC);
    work->groups = (BtlOperandGroup *)sdfResourceRetainAddress(allocation);
    work->allocationHandle = allocation;
    work->ownerId = 0;
    btlResetIndexWork(work);
}

/* Release each owned buffer once. The cached group address is deliberately not cleared. */
void btlReleaseObjectBuffers(BattleIndexWork *object) {
    if (object->allocationHandle != 0) {
        sdfReleaseResourceAllocation(object->allocationHandle);
        object->allocationHandle = 0;
    }
    if (object->indices != 0) {
        btlFreeIndexList(object->indices);
        object->indices = 0;
    }
}

extern void btlAdjustUnitHp(DatPartyRecord *, s32);

extern void btlAdjustUnitMp(DatPartyRecord *, s32);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001DF860);

extern u32 func_001DF860(BtlOperandTaskArgs *);

BtlRuntimeTask *btlCreateActorParameterDeltaTask(BtlUnit *unit, BtlOperandEntry *block) {
    BtlRuntimeTask *task = btlAllocTask(0x30);
    BtlOperandTaskArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x49;
    task->ownerId = unit->owner;
    task->callback = func_001DF860;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    memcpy(&args->operand, block, sizeof(*block));
    return task;
}

u32 btlApplyDeferredActorStats(BtlOperandTaskArgs *arguments) {
    BtlState *context = (BtlState *)btlGetRuntime();
    BtlUnit *actor = arguments->unit;
    s32 primary;
    DatPartyRecord *resource;
    if ((context->battleFlags & 0x80) == 0) {
        return 1;
    }
    primary = arguments->operand.hpRecovery;
    if (primary == 0 && arguments->operand.mpRecovery == 0) {
        return 1;
    }
    if (actor->status.flags & 0x60) {
        return 1;
    }
    resource = &actor->partyRecord;
    btlAdjustUnitHp(resource, primary);
    btlAdjustUnitMp(resource, arguments->operand.mpRecovery);
    btlRefreshUnitMotionSelection(actor);
    btlIsUnitDefeatTriggeredByValueDelta(actor, 0);
    return 1;
}

BtlRuntimeTask *btlCreateDeferredActorStatsTask(BtlUnit *unit, BtlOperandEntry *block) {
    BtlRuntimeTask *task = btlAllocTask(0x30);
    BtlOperandTaskArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x4A;
    task->ownerId = unit->owner;
    task->callback = btlApplyDeferredActorStats;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    memcpy(&args->operand, block, sizeof(*block));
    return task;
}

u32 btlApplyDeferredUnitStatus(void *arg) {
    s32 *args = arg;
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit = (BtlUnit *)args[0];
    if (!(work->battleFlags & 0x80)) {
        return 1;
    }
    func_001AA850(&unit->partyRecord.flags, args[1]);
    btlRefreshUnitMotionSelection(unit);
    btlIsUnitDefeatTriggeredByValueDelta(unit, 0);
    return 1;
}

BtlRuntimeTask *btlCreateDeferredUnitStatusTask(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x4B;
    task->ownerId = actor->owner;
    task->callback = btlApplyDeferredUnitStatus;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = option;
    return task;
}

typedef struct BtlStatArgs {
    BtlUnit *unit;
    s32 amount;
    s32 category;
} BtlStatArgs;

s32 btlApplyCategoryStatDamage(BtlStatArgs *args) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit = args->unit;
    if (!(work->battleFlags & 0x80)) {
        return 1;
    }
    if (datCommandRecords[args->category].flags & 8) {
        btlAdjustUnitHp(&unit->partyRecord, -0x7FFF);
        func_001AA850(&unit->partyRecord.flags, 0x4000);
        unit->status.flags |= 0x20;
    }
    if (args->amount == 0) {
        return 1;
    }
    switch (datCommandRecords[args->category].costMode) {
    case DAT_COMMAND_COST_MODE_HP:
        btlAdjustUnitHp(&unit->partyRecord, -args->amount);
        return 1;
    case DAT_COMMAND_COST_MODE_MP:
        btlAdjustUnitMp(&unit->partyRecord, -args->amount);
        return 1;
    default:
        return 1;
    }
}

BtlRuntimeTask *btlCreateCategoryStatDamageTask(BtlUnit *unit, u32 target, u32 option) {
    BtlRuntimeTask *task = btlAllocTask(12);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlApplyCategoryStatDamage;
    task->taskId = 0x4C;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    args->unk_08 = target;
    args->option = option;
    return task;
}

s32 func_001DFFE0(BtlOperandTaskArgs *args) {
    func_001ADFE0(args->unit, args->operand.entryChangeMask, args->operand.entryChange);
    return 1;
}

BtlRuntimeTask *btlCreateMaskedActorEntryUpdateTask(BtlUnit *unit, BtlOperandEntry *block) {
    BtlRuntimeTask *task = btlAllocTask(0x30);
    BtlOperandTaskArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x4D;
    task->ownerId = unit->owner;
    task->callback = func_001DFFE0;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    memcpy(&args->operand, block, sizeof(*block));
    return task;
}

u32 btlApplyQueuedActorEntrySelection(BtlOperandTaskArgs *taskArgs) {
    if (0 < taskArgs->operand.entrySelection) {
        btlSetActorSelectedEntryIndex(taskArgs->unit, taskArgs->operand.entrySelection);
        btlRefreshUnitMotionSelection(taskArgs->unit);
    }
    return 1;
}

BtlRuntimeTask *btlCreateQueuedActorEntrySelectionTask(BtlUnit *unit, BtlOperandEntry *block) {
    BtlRuntimeTask *task = btlAllocTask(0x30);
    BtlOperandTaskArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x4E;
    task->ownerId = unit->owner;
    task->callback = btlApplyQueuedActorEntrySelection;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    memcpy(&args->operand, block, sizeof(*block));
    return task;
}

u32 btlClearQueuedActorEntrySelection(BtlUnit **taskArgs) {
    btlClearActorSelectedEntryIndex(*taskArgs);
    btlRefreshUnitMotionSelection(*taskArgs);
    return 1;
}

BtlRuntimeTask *func_001E0238(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    BtlUnit **args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlClearQueuedActorEntrySelection;
    task->taskId = 0x4F;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    *args = unit;
    return task;
}

/* Complete eight-byte argument allocation owned by the hunt-EP task. */
typedef struct BtlHuntExpArgs {
    BtlUnit *actor;
    u32 amount;
} BtlHuntExpArgs;

typedef char BtlHuntExpArgsSizeCheck[sizeof(BtlHuntExpArgs) == 8 ? 1 : -1];

extern DatPartyRecord *btlGetIndexedPartyEntryRecord(s32);

u32 func_001E02A8(s32 address) {
    BtlHuntExpArgs *args = (BtlHuntExpArgs *)address;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    BtlUnit *head;
    DatPartyRecord *record;
    u32 count;
    u32 index;
    u32 share;

    if (args->amount == 0) {
        return 1;
    }
    if (args->actor->status.flags & 0x400) {
        return 1;
    }
    if (btlCheckSpecialAbility(&args->actor->partyRecord, 0x26C)) {
        count = 0;
        record = btlGetIndexedPartyEntryRecord(args->actor->unk2E4);
        record->huntExp += args->amount;
        head = battle->units;
        for (unit = head; unit != NULL; unit = unit->nextActor) {
            u32 flags = unit->status.flags;
            if (flags & 0x200) {
                if (flags & 1) {
                    if (args->actor != unit && !(flags & 0x20) && !(unit->partyRecord.status & 0x40)) {
                        count++;
                    }
                }
            }
        }
        for (index = 0; index < 5; index++) {
            u16 flags = datGameState->party[index].flags;
            if (flags & 1) {
                if (!(flags & 2) && !(datGameState->party[index].status & 0x4040)) {
                    count++;
                }
            }
        }
        if (count == 0) {
            return 1;
        }
        share = (u32)((f32)args->amount / (f32)count);
        for (unit = head; unit != NULL; unit = unit->nextActor) {
            u32 flags = unit->status.flags;
            if (flags & 0x200) {
                if (flags & 1) {
                    if (args->actor != unit && !(flags & 0x20) && !(unit->partyRecord.status & 0x40)) {
                        record = btlGetIndexedPartyEntryRecord(unit->unk2E4);
                        record->huntExp += share;
                    }
                }
            }
        }
        for (index = 0; index < 5; index++) {
            u16 flags = datGameState->party[index].flags;
            if (flags & 1) {
                if (!(flags & 2) && !(datGameState->party[index].status & 0x4040)) {
                    datGameState->party[index].huntExp += share;
                }
            }
        }
        btlBossDebugPrintf("btl:AUTO ep=%d[%d],count=%d\n", share, args->amount, count);
    } else {
        record = btlGetIndexedPartyEntryRecord(args->actor->unk2E4);
        record->huntExp += args->amount;
        btlBossDebugPrintf("btl:hunt ep=%d[%p]\n", args->amount, record);
    }
    return 1;
}

extern u32 func_001E02A8(s32);

BtlRuntimeTask *btlCreateActorSoundOptionTask(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    BtlHuntExpArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x50;
    task->ownerId = actor->owner;
    task->callback = func_001E02A8;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->amount = (u32)option;
    return task;
}

typedef struct {
    u32 unk0;
    s32 soundIndex;
} BattleVoiceWork;

extern void ptyAdjustItemQuantity(s32, s32);

extern void btlSyncModelFlagFromEventThresholds(void);

s32 btlPlayPermittedBattleVoice(BattleVoiceWork *work) {
    s32 index = work->soundIndex;
    if (datItemSkillRecords[index].unk01 & 4) {
        ptyAdjustItemQuantity(index, -1);
        switch (work->soundIndex) {
        case 0x53:
        case 0x54:
            btlSyncModelFlagFromEventThresholds();
            break;
        }
    }
    return 1;
}

BtlRuntimeTask *btlCreatePermittedBattleVoiceTask(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x51;
    task->ownerId = actor->owner;
    task->callback = btlPlayPermittedBattleVoice;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = option;
    return task;
}

u32 btlPlayQueuedBattleVoice(s32 taskArgs) {
    ptyAdjustItemQuantity(*(u16 *)(taskArgs + 4), 1);
    return 1;
}

BtlRuntimeTask *btlCreateQueuedBattleVoiceTask(BtlUnit *actor, u16 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x52;
    task->ownerId = actor->owner;
    task->callback = btlPlayQueuedBattleVoice;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->optionId = option;
    return task;
}

/* Task payload shared by the experience and money reward callbacks. */
typedef struct BattleRewardPacket {
    BtlUnit *actor;
    s32 amount;
} BattleRewardPacket;

u32 btlAddEpFromPacket(s32 packetAddress) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BattleRewardPacket *packet = (BattleRewardPacket *)packetAddress;
    if (packet->amount == 0) {
        return 1;
    }
    if (packet->actor->status.flags & 0x400) {
        return 1;
    }
    work->epEarned += packet->amount;
    btlBossDebugPrintf("btl:epall=%d[%d](packet)\n", work->epEarned, packet->amount);
    return 1;
}

extern u32 btlAddEpFromPacket(s32);

BtlRuntimeTask *btlScheduleEpPacketTask(BtlUnit *actor, s32 amount) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x53;
    task->ownerId = actor->owner;
    task->callback = btlAddEpFromPacket;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = amount;
    return task;
}

u32 btlAddMoneyFromPacket(s32 packetAddress) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BattleRewardPacket *packet = (BattleRewardPacket *)packetAddress;
    if (packet->amount == 0) {
        return 1;
    }
    if (packet->actor->status.flags & 0x400) {
        return 1;
    }
    work->moneyEarned += packet->amount;
    btlBossDebugPrintf("btl:money=%d[%d](packet)\n", work->moneyEarned, packet->amount);
    return 1;
}

extern u32 btlAddMoneyFromPacket(s32);

BtlRuntimeTask *btlScheduleMoneyPacketTask(BtlUnit *actor, s32 amount) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x54;
    task->ownerId = actor->owner;
    task->callback = btlAddMoneyFromPacket;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = amount;
    return task;
}

extern DatEnemyRecord *datEnemyRecords;

u32 btlRefreshEligibleActors(void) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit = work->units;
    while (unit != 0) {
        u32 flags = unit->status.flags;
        if (flags & 0x400) {
            if (flags & 1) {
                if ((flags & 0xE0) == 0 && (u16)(unit->partyRecord.unitId - 1) < 0x17F) {
                    u32 entry = datEnemyRecords[unit->partyRecord.unitId].flags;
                    if ((entry & 0x40) == 0) {
                        if ((entry & 0x400) == 0) {
                            if ((unit->status.stateFlags & 8) == 0) {
                                u16 prior = unit->partyRecord.status;
                                func_001AA850(&unit->partyRecord.flags, 1);
                                btlRefreshUnitMotionSelection(unit);
                                if (unit->partyRecord.status == 1 && prior != unit->partyRecord.status) {
                                    unit->status.stateFlags |= 4;
                                    work->commandRestrictFlags |= 0x100;
                                }
                            }
                        }
                    }
                }
            }
        }
        unit = unit->nextActor;
    }
    return 1;
}

BtlRuntimeTask *btlCreateRefreshEligibleActorsTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlRefreshEligibleActors;
    task->taskId = 0x55;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

u32 btlApplyQueuedCurrencyReward(s32 taskArgs) {
    datAddCurrencyClamped(*(u32 *)(taskArgs + 4));
    return 1;
}

BtlRuntimeTask *btlCreateCurrencyRewardTask(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x56;
    task->ownerId = actor->owner;
    task->callback = btlApplyQueuedCurrencyReward;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = option;
    return task;
}

void btlUpdateAutoMusic(void) {
    u8 *context = (u8 *)btlGetRuntime();
    u32 flags = ((BtlState *)context)->battleFlags;
    if ((flags & 0x100000) == 0 || (flags & 0x6000000) == 0x6000000 ||
        (flags & 0x800) != 0) {
        return;
    }
    if (flags & 0x8000) {
        if ((s8)D_0037F510[0x22] < 0 || (s8)D_0037F510[0x23] < 0) {
            ((BtlState *)context)->battleFlags = flags & ~0x8000;
            sndSetSequenceVolumePan(6, 0x7F, 0x3F);
            btlSetTrackedTaskDisplayMode(0);
            btlBossDebugPrintf(D_004178A8);
        }
    } else if ((s8)D_0037F510[0x22] < 0) {
        ((BtlState *)context)->battleFlags = flags | 0x8000;
        sndSetSequenceVolumePan(5, 0x7F, 0x3F);
        btlSetTrackedTaskDisplayMode(1);
        btlBossDebugPrintf(D_004178B8);
    }
}

extern s32 fileTestSavedSlotFlags(u32);
extern s32 btlCheckSpecialAbility(DatPartyRecord *, s32);
extern s32 func_001ABB10(BtlUnit *, s32);
extern u32 func_001AC360(ActionStateLink *, BtlIndexList *, s32);
extern s32 btlGetCommandTargetEligibility(BtlIndexList *, s32);
extern BtlUnit *btlFindActorForOwner(u64);
extern BtlUnit *btlSelectUnitAtExtremeX(BtlUnit *, BtlIndexList *);

/* Retail 0x001E107C calls btlGetIndexListCount even though its result is discarded. */
void func_001E0CE0(ActionStateLink *task, BattleIndexWork *work) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit = task->unit;
    BtlIndexList *list;
    u32 kind;
    u32 count;
    u32 i;
    u32 sides;
    BtlUnit *target;
    s32 reason;

    if (fileTestSavedSlotFlags(2)) {
        if ((s16)unit->partyRecord.unk1AC == 10) {
            work->phase = (s16)unit->partyRecord.unk1AC;
            return;
        }
        if (((s16)unit->partyRecord.unk1AC == 2 ||
             (s16)unit->partyRecord.unk1AC == 7) &&
            ((btlUnitStatusPair(unit) & 0x1400) || (unit->partyRecord.flags & 0x10))) {
            if ((s16)unit->partyRecord.actionSlot == 0) {
                work->phase = 1;
            } else if ((unit->status.flags & 0x400) || (battle->battleFlags & 0x1000000) ||
                       btlCheckSpecialAbility(&unit->partyRecord, (s16)unit->partyRecord.actionSlot)) {
                work->phase = (s16)unit->partyRecord.unk1AC;
                work->skillId = (s16)unit->partyRecord.actionSlot;
            } else {
                work->skillId = 0;
                work->phase = 1;
            }
        } else {
            work->skillId = 0;
            work->phase = 1;
        }
        if (datCommandSelectors[work->skillId].kind == 1) {
            work->phase = 1;
            work->skillId = 0;
        }
        if (work->skillId < 0xAD) {
            if (work->skillId >= 0xAB) {
                work->phase = 1;
                work->skillId = 0;
            }
        }
        if ((u32)work->skillId >= 0x2A0) {
            work->skillId = 0;
            work->phase = 1;
        }
        if (func_001ABB10(unit, work->skillId)) {
            work->skillId = 0;
            work->phase = 1;
        }
        unit->partyRecord.unk1AC = work->phase;
        unit->partyRecord.actionSlot = work->skillId;
        list = btlAllocateIndexList(13);
        kind = func_001AC360(task, list, 0);
        reason = btlGetCommandTargetEligibility(list, work->skillId);
        switch (reason) {
        case 5:
        case 7:
        case 9:
            work->skillId = 0;
            work->phase = 1;
            unit->partyRecord.actionSlot = work->skillId;
            unit->partyRecord.unk1AC = work->phase;
            btlClearIndexList(list);
            kind = func_001AC360(task, list, 0);
            break;
        }
        count = btlGetIndexListCount(list);
        sides = 0;
        for (i = 0; i < count;) {
            target = btlGetIndexListEntry(list, i++);
            sides |= target->status.flags & 0x600;
        }
        btlClearIndexList(work->indices);
        switch (kind) {
        case 0:
            target = btlFindActorForOwner(work->ownerId);
            if (target == NULL || !(target->status.flags & sides) ||
                (btlUnitStatusPair(target) & 0xE1) != 1) {
                target = unit;
                if ((u8)(datCommandRecords[work->skillId].options & 1) == 0) {
                    target = btlSelectUnitAtExtremeX(task->unit, list);
                }
            }
            btlAppendIndexListEntry(work->indices, target);
            break;
        case 1:
        case 2:
            btlCopyIndexList(work->indices, list);
            break;
        }
        btlFreeIndexList(list);
        return;
    } else {
        work->skillId = 0;
        work->phase = 1;
        list = btlAllocateIndexList(13);
        kind = func_001AC360(task, list, 0);
        btlGetIndexListCount(list);
        btlClearIndexList(work->indices);
        switch (kind) {
        case 0:
            btlAppendIndexListEntry(work->indices, btlSelectUnitAtExtremeX(task->unit, list));
            break;
        case 1:
        case 2:
            btlCopyIndexList(work->indices, list);
            break;
        }
        btlFreeIndexList(list);
        return;
    }
}

/* The resolver supplies its result kind and press value to this no-op hook. */
void btlFindSoundTaskByWorkValue(u32 kind, s32 press) {
}

/* Return the oldest matching handle, or zero; unstarted tasks may have handle 0. */
BtlRuntimeTask *btlFindTaskByHandle(u64 value) {
    BtlRuntimeTask *task;
    for (task = ((BtlState *)btlGetRuntime())->taskHead; task != 0; task = task->next) {
        if (task->handle == value) {
            return task;
        }
    }
    return 0;
}

/* Return the oldest task with this owner, or zero. */
BtlRuntimeTask *btlFindTaskByOwner(u64 owner) {
    BtlRuntimeTask *task;
    for (task = ((BtlState *)btlGetRuntime())->taskHead; task != 0; task = task->next) {
        if (task->ownerId == owner) {
            return task;
        }
    }
    return 0;
}

/* Return the oldest registered task of this kind, or zero. */
BtlRuntimeTask *btlFindTaskByKind(u16 kind) {
    BtlRuntimeTask *task;
    for (task = ((BtlState *)btlGetRuntime())->taskHead; task != 0; task = task->next) {
        if (task->taskId == kind) {
            return task;
        }
    }
    return 0;
}

/* Count all registrations, including tasks awaiting startup or release. */
s32 btlCountRegisteredTasks(void) {
    s32 task;
    s32 count;

    task = btlGetRuntime();
    count = 0;
    for (task = (s32)((BtlState *)task)->taskHead; task != 0; task = (s32)((BtlRuntimeTask *)task)->next) {
        count = count + 1;
    }
    return count;
}

/* Count registrations with this full-width owner key. */
s32 btlCountTasksForOwner(s64 owner) {
    s32 count = 0;
    BtlRuntimeTask *task;
    for (task = ((BtlState *)btlGetRuntime())->taskHead; task != 0; task = task->next) {
        if (task->ownerId == owner) {
            count++;
        }
    }
    return count;
}

/* Count registrations of the requested task kind. */
s32 btlCountTasksByKind(u16 kind) {
    s32 count = 0;
    BtlRuntimeTask *task;
    for (task = ((BtlState *)btlGetRuntime())->taskHead; task != 0; task = task->next) {
        if (task->taskId == kind) {
            count++;
        }
    }
    return count;
}

/* Walk newest first and request release for tasks carrying allocation bit 1. */
void btlFlagTasksForUpdate(void) {
    BtlRuntimeTask *task;
    BtlRuntimeTask *next;
    for (task = ((BtlState *)btlGetRuntime())->taskTail; task != 0; task = next) {
        u16 flags = task->flags;
        next = task->prev;
        if (flags & 1) {
            task->flags = flags | 4;
        }
    }
}

extern BtlRuntimeTask *btlFindTaskByHandle(u64);

extern BtlRuntimeTask *btlFindTaskByOwner(u64);

extern BtlRuntimeTask *btlFindTaskByKind(u16);

/* Return whether the predicate is satisfied by value or registered tasks.
 * Kinds 5/8 accept running (phase 2) or absent, not an existing finishing task. */
INCLUDE_RODATA(const s32, "game/code_001DD390", D_004178A8);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004178B8);

s32 btlEvalTaskCondition(BtlTaskCondition *condition, s32 value) {
    s32 result = 0;
    BtlRuntimeTask *task;
    switch (condition->kind) {
    case BTL_TASK_CONDITION_NEVER:
        break;
    case BTL_TASK_CONDITION_ALWAYS:
        result = 1;
        break;
    case BTL_TASK_CONDITION_COUNT_REACHED:
        if (!(value < condition->value.count)) {
            result = 1;
        }
        break;
    case BTL_TASK_CONDITION_HANDLE_PRESENT:
        if (btlFindTaskByHandle(condition->value.handle) != 0) {
            result = 1;
        }
        break;
    case BTL_TASK_CONDITION_HANDLE_ABSENT:
        if (btlFindTaskByHandle(condition->value.handle) == 0) {
            result = 1;
        }
        break;
    case BTL_TASK_CONDITION_HANDLE_RUNNING_OR_ABSENT:
        task = btlFindTaskByHandle(condition->value.handle);
        if (task != 0) {
            if (task->state == BTL_TASK_PHASE_RUNNING) {
                result = 1;
            }
        } else {
            result = 1;
        }
        break;
    case BTL_TASK_CONDITION_OWNER_PRESENT:
        if (btlFindTaskByOwner(condition->value.owner) != 0) {
            result = 1;
        }
        break;
    case BTL_TASK_CONDITION_OWNER_ABSENT:
        if (btlFindTaskByOwner(condition->value.owner) == 0) {
            result = 1;
        }
        break;
    case BTL_TASK_CONDITION_OWNER_RUNNING_OR_ABSENT:
        task = btlFindTaskByOwner(condition->value.owner);
        if (task != 0) {
            if (task->state == BTL_TASK_PHASE_RUNNING) {
                result = 1;
            }
        } else {
            result = 1;
        }
        break;
    case BTL_TASK_CONDITION_KIND_PRESENT:
        if (btlFindTaskByKind(condition->value.taskKind) != 0) {
            result = 1;
        }
        break;
    case BTL_TASK_CONDITION_KIND_ABSENT:
        result = btlFindTaskByKind(condition->value.taskKind) == 0;
        break;
    }
    return result;
}

/* Append a cleared task; positive size exposes argument bytes after the header. */
BtlRuntimeTask *btlAllocTask(s32 size) {
    BtlRuntimeTask *task = sdfAllocAndClearQuadwords(size + 0x70);
    BtlState *work;
    if (size > 0) {
        task->args = task + 1;
    } else {
        task->args = 0;
    }
    work = (BtlState *)btlGetRuntime();
    task->next = 0;
    if (work->taskTail != 0) {
        work->taskTail->next = task;
        task->prev = work->taskTail;
    } else {
        work->taskHead = task;
        task->prev = 0;
    }
    work->taskTail = task;
    task->flags |= BTL_TASK_FLAG_REGISTERED;
    return task;
}

/* Return the argument address recorded by allocation (zero for no arguments). */
void *btlGetTaskArguments(void *task) {
    return ((BtlRuntimeTask *)task)->args;
}

/* Invoke the finish hook before unlinking, then release the task block. */
void btlFreeTask(BtlRuntimeTask *task) {
    BtlState *work;
    if (task->onFinish != 0) {
        task->onFinish((u32 *)task->args);
    }
    work = (BtlState *)btlGetRuntime();
    if (task->prev != 0) {
        task->prev->next = task->next;
    } else {
        work->taskHead = task->next;
    }
    if (task->next != 0) {
        task->next->prev = task->prev;
    } else {
        work->taskTail = task->prev;
    }
    sdfReleaseChipBlock(task);
}

/* Install a fresh handle/reset phase counters, invoke startup, then reread handle. */
u64 btlStartTask(taskObject)
    void *taskObject;
{
    BtlRuntimeTask *task = taskObject;
    task->handle = btlAdvanceRuntimeSequenceCounter();
    task->flags |= BTL_TASK_FLAG_STARTED;
    task->pollCount = 0;
    task->runCount = 0;
    task->state = BTL_TASK_PHASE_WAITING;
    task->deferNext = 0;
    task->deferPrev = 0;
    if (task->onStart != 0) {
        task->onStart((u32)task->args);
    }
    return task->handle;
}

void btlResetDeferredTaskQueue(void) {
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

/* Advance a started task through wait/delay/update/release.
 * Fallthrough is intentional: zero delays permit all phases in one poll. */
void btlRunTask(BtlRuntimeTask *task) {
    u32 counter;
    if (!(task->flags & BTL_TASK_FLAG_STARTED)) {
        return;
    }
    if (task->flags & BTL_TASK_FLAG_RELEASE_REQUESTED) {
        btlFreeTask(task);
        return;
    }
    counter = task->pollCount;
    task->pollCount = counter + 1;
    switch (task->state) {
    case BTL_TASK_PHASE_WAITING:
        if (btlEvalTaskCondition(&task->startCondition, counter) == 0) {
            break;
        }
        task->state = BTL_TASK_PHASE_START_DELAY;
    case BTL_TASK_PHASE_START_DELAY:
        if (task->startDelay <= 0) {
            task->state = BTL_TASK_PHASE_RUNNING;
        } else {
            task->startDelay = task->startDelay - 1;
            break;
        }
    case BTL_TASK_PHASE_RUNNING:
        if (btlEvalTaskCondition(&task->endCondition, task->runCount) != 0) {
            task->state = BTL_TASK_PHASE_END_DELAY;
        } else if (task->callback(task->args) != 0) {
            task->state = BTL_TASK_PHASE_END_DELAY;
        } else {
            task->runCount = task->runCount + 1;
            break;
        }
    case BTL_TASK_PHASE_END_DELAY:
        if (task->endDelay <= 0) {
            btlFreeTask(task);
        } else {
            task->endDelay = task->endDelay - 1;
        }
        break;
    }
}

/* Run ordinary registrations now; queue deferred registrations for the later pass. */
void btlSweepFinishedTasks(void) {
    BtlRuntimeTask *task;
    BtlRuntimeTask *next;
    for (task = ((BtlState *)btlGetRuntime())->taskHead; task != 0; task = next) {
        next = task->next;
        if (!(task->flags & BTL_TASK_FLAG_DEFERRED)) {
            btlRunTask(task);
        } else {
            task->deferNext = 0;
            if (btlDeferredTaskTail != 0) {
                btlDeferredTaskTail->deferNext = task;
                task->deferPrev = btlDeferredTaskTail;
            } else {
                btlDeferredTaskHead = task;
                task->deferPrev = 0;
            }
            btlDeferredTaskTail = task;
        }
    }
}

/* Run deferred tasks, saving the next link before callbacks may free the task. */
void btlClearDeferredTasks(void) {
    BtlRuntimeTask *node = btlDeferredTaskHead;
    while (node != 0) {
        BtlRuntimeTask *next = node->deferNext;
        btlRunTask(node);
        node = next;
    }
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

/* Release newest first; cache the previous registration before its block is freed. */
void btlClearTaskLists(void) {
    BtlRuntimeTask *task;
    BtlRuntimeTask *next;
    for (task = ((BtlState *)btlGetRuntime())->taskTail; task != 0; task = next) {
        next = task->prev;
        btlFreeTask(task);
    }
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

u32 func_001E1848(void) {
    return 1;
}

BtlRuntimeTask *btlCreateImmediateCompletionTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = func_001E1848;
    task->taskId = 0x6A;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

void btlDumpTaskQueue(void) {
    s32 context = btlGetRuntime();
    s32 node = (s32)((BtlState *)context)->taskHead;
    while (node != 0) {
        btlBossDebugPrintf("btl:packet[%d]\n", ((BtlRuntimeTask *)node)->taskId);
        node = (s32)((BtlRuntimeTask *)node)->next;
    }
    btlBossDebugPrintf("btl:packet head[%p]\n", *(void **)(context + 0x250));
    btlBossDebugPrintf("btl:packet tail[%p]\n", *(void **)(context + 0x254));
}

void btlInitUnitFxDefaults(BtlUnit *unit) {
    PCP_COPY_VECTOR(unit->bodyOffset, &D_003B6B80);
    unit->height = 220.0f;
    unit->reach = 80.0f;
    unit->unkC0 = 75.0f;
}

extern s128 D_003B6B90;

extern s128 D_003B6BA0;

void btlInitFxLights(BtlUnit *unit) {
    PCP_COPY_VECTOR(unit->position, &D_003B6B90);
    PCP_COPY_VECTOR(unit->rotation, &D_003B6BA0);
    unit->unk58 = 0.0f;
    unit->unk50 = 1.0f;
    unit->baseColor = 0x80808080;
    PCP_COPY_VECTOR(unit->currentPosition, &D_003B6B90);
    PCP_COPY_VECTOR(unit->orientation, &D_003B6BA0);
    unit->scale = 1.0f;
    unit->overlayColor = 0x80808080;
    unit->positionZOffset = 0.0f;
}

extern void *btlSelectSharedOrIndexedTransformParameters(s32, s32);

void btlInitializeEffectVectorsFromSourceRecords(BtlUnit *unit, s32 kind, s32 index) {
    BtlFxSrcA *alt = btlSelectSharedOrIndexedTransformParameters(kind, index);
    BtlEffectResource *base = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(kind, index);
    if (alt->fC == 0.0f) {
        PCP_COPY_VECTOR(unit->bodyOffset, base);
        unit->reach = base->f18;
        unit->height = base->f1C;
        unit->unkC0 = base->f20;
    } else {
        unit->bodyOffset[0] = alt->f0;
        unit->bodyOffset[1] = alt->f4;
        unit->bodyOffset[2] = alt->f8;
        unit->bodyOffset[3] = 0.0f;
        unit->reach = alt->f10;
        unit->height = alt->f14;
    }
    PCP_COPY_VECTOR(unit->muzzleOffset, base);
    unit->unkBC = base->f18;
    unit->unkB8 = base->f1C;
    unit->unkC0 = base->f20;
    unit->scale = base->f10;
    unit->unk50 = base->f10;
    unit->positionZOffset = base->f14;
    unit->unk58 = base->f14;
}

s32 btlHasMatchingModel(s32 effect, s32 model) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit = work->units;
    while (unit != NULL) {
        if ((unit->status.flags & 2) != 0 &&
            unit->ext != NULL &&
            unit->unk328 != 0 &&
            mdlGetContextResourceGroup(unit->ext->owner) == effect &&
            mdlGetContextResourceId(unit->ext->owner) == model) {
            return 1;
        }
        unit = unit->nextActor;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E1B80);

/* Native actor-model setup providers used by btlBindUnitModel. */
extern s32 mdlSpawnCameraSlotViewerObject(s32 kind, s32 id);

extern void *dds3GetWorldObject(void);

extern struct SoundSlotOwner *sndAcquireSlotOwner(s32 category, s32 id);

extern void btlResetUnitModelProgress(BtlUnit *unit);

extern void btlSetUnitPosition(BtlUnit *unit, f32 *position);

extern void btlSetUnitRotation(BtlUnit *unit, s128 *rotation);

extern void func_001E1B80(BtlUnit *unit, BtlUnit *reused);

void btlBindUnitModel(u8 *unitAddress, u32 kind, u32 index) {
    BtlUnit *unit = (BtlUnit *)unitAddress;
    BtlUnit *reused = NULL;
    BtlState *battle = (BtlState *)btlGetRuntime();
    MdlCtx *model;
    BtlActorStatusRecord *status;
    EffWorldNode *object;
    ObjectTransform *inner;
    EffectObjectData *objectData;
    s32 key;

    unit->resourceKind = kind;
    unit->resourceIndex = index;
    unit->unkCC = 0;
    btlInitializeEffectVectorsFromSourceRecords(unit, kind, index);
    if (battle->findModelActor != NULL) {
        reused = battle->findModelActor(kind, index);
        if (reused != NULL) {
            if (reused->resourceKind != kind || reused->resourceIndex != index) {
                key = mdlSpawnCameraSlotViewerObject(kind, index);
                object = dds3FindWorldObjectNodeByKey(
                    (EffWorldNode *)dds3GetWorldObject(), (u32)key, 5);
                dds3RemoveWorldObjectNode(object);
            }
            func_001E1B80(unit, reused);
        }
    }
    if (battle->beforeActorModelReady != NULL) {
        battle->beforeActorModelReady(unit);
    }
    if (reused == NULL) {
        key = mdlSpawnCameraSlotViewerObject(kind, index);
        unit->effectObject = dds3FindWorldObjectNodeByKey(
            (EffWorldNode *)dds3GetWorldObject(), (u32)key, 5);
        objectData = (EffectObjectData *)unit->effectObject->data;
        unit->ext = objectData->transitionWork;
        model = unit->ext->owner;
        unit->ext->flags |= 0x200000;
        VU0_SET_ONES_XYZ(vf10);
        VU0_SCALAR_OP(unit->unk50, "vmulx.xyzw vf10, vf10, vf2x");
        inner = unit->effectObject->inner;
        inner->flags = (inner->flags | 1) & ~2;
        VU0_STORE_VF(vf10, inner->scale);
        mdlStoreTertiaryVectorVU(model);
        if (unit->unk50 != 1.0f) {
            mdlSetAmountOnAllContextResources(model, unit->unk50);
        }
        dds3ClearObjectFlags(unit->effectObject, 0x400);
        unit->status.flags |= 8;
        unit->unk328 = (s32)sndAcquireSlotOwner(kind, index);
        unit->status.flags |= 2;
    }
    btlSetUnitPosition(unit, unit->position);
    btlSetUnitRotation(unit, (s128 *)unit->rotation);
    unit->updateFlags = 0;
    unit->unkF8 = 0;
    unit->unkEC = -1;
    unit->unkFA = 0;
    btlRefreshUnitMotionSelection(unit);
    if (unit->effectIndex != 0xB) {
        if ((battle->commandRestrictFlags & 0x40000) != 0 && (unit->status.flags & 0x400) != 0) {
            btlApplyScaledUnitEffectParameter(unit, 2, 1, 1.0f);
        } else {
            btlApplyScaledUnitEffectParameter(unit, unit->effectIndex, 1, 1.0f);
        }
    } else {
        btlApplyScaledUnitEffectParameter(unit, 0xB, 2, 1.0f);
    }
    if (unit->link31C != NULL) {
        btlMarkTaskReady(unit->link31C);
    }
    unit->status.flags |= 0x40000000;
    if (unit->status.flags & 0x20) {
        model = unit->ext->owner;
        unit->ext->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        unit->ext->flags &= ~0xA0;
        mdlAddEntryPlain(model, 0, 0xB);
        unit->unkEC = 0xB;
        sdfMotionSampleAtFrame(model->first, (f32)model->first->frameCount);
        unit->status.flags = unit->status.flags & 0x7FFFFFFF & 0xBFFFFFFF;
    } else if (btlTestActorStatusPredicate(unit)) {
        unit->ext->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        model = unit->ext->owner;
        unit->ext->flags &= ~0xA0;
        status = (BtlActorStatusRecord *)btlGetSideIndexedActorStatusTable(kind, index);
        mdlAddEntryPlain(model, 0, 1);
        unit->unkEC = 1;
        sdfMotionSampleAtFrame(model->first, (f32)status->model);
        btlResetUnitModelProgress(unit);
        unit->status.flags = (unit->status.flags | 0x2000) & 0x7FFFFFFF & 0xBFFFFFFF;
    }
    unit->status.flags |= 0x80004;
    if (battle->afterActorModelReady != NULL) {
        battle->afterActorModelReady(unit);
    }
}

extern void sdfReleaseDevSlot(s32, s32, s32);

extern char D_00417940[]; /* "btl:unit transparency delete[%p]\n" */

void btlReleaseActorModelResources(BtlUnit *unit) {
    if (unit->unkCC == 0) {
        if (unit->unk328 != 0) {
            sndReleaseSlotOwner((struct SoundSlotOwner *)unit->unk328);
            unit->unk328 = 0;
        }
        if (unit->unk344 != 0) {
            sdfReleaseDevSlot(unit->unk344, 1, 1);
            unit->unk344 = 0;
            btlBossDebugPrintf(D_00417940, unit);
        }
        if (unit->effectObject != 0) {
            dds3RemoveWorldObjectNode(unit->effectObject);
            unit->effectObject = 0;
            unit->ext = 0;
        }
    } else {
        unit->unk328 = 0;
        unit->unk344 = 0;
        unit->effectObject = 0;
        unit->ext = 0;
    }
    unit->gunResourceFlags &= ~1;
    unit->status.flags &= ~2;
    unit->gunResourceFlags &= ~2;
}

void btlRequestModelAssetByMode(u32 unused, u32 effect, u32 model) {
    s64 available;

    available = mdlFlagTest(0xc0f);
    if (available != 0) {
        func_0022CD60(effect, model);
        return;
    }
    mdlRequestAsset(effect, model, 0);
}

void btlReleaseModelAssetByMode(u32 unused, u32 effect, u32 model) {
    s64 available;

    available = mdlFlagTest(0xc0f);
    if (available != 0) {
        btlReleaseFoundModelEntry(effect, model);
        return;
    }
}

s32 btlCheckModelAssetByMode(u8 *object, u32 effect, u32 model) {
    if (mdlFlagTest(0xC0F) != 0) {
        if (func_0022CD60(effect, model, 0) != 0) {
            return 1;
        }
    } else {
        if (mdlRequestAsset(effect, model, 0) != 0 && mdlRequestAsset(effect, model, 0) != -1) {
            return 1;
        }
    }
    return 0;
}

void btlFlagUnitDefeatCandidate(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = ((BtlState *)btlGetRuntime())->unk69C;
    if (hook == 0 || hook(unit) != 0) {
        unit->status.flags |= 4;
        if (!(unit->status.flags & 0x8000000)) {
            unit->status.flags |= 8;
            if (unit->status.flags & 2) {
                unit->ext->owner->flags &= ~MDL_SKIP_TRANSFORMS;
            }
        }
    }
}

void btlClearUnitDefeatCandidate(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = ((BtlState *)btlGetRuntime())->unk6A0;
    if (hook == 0 || hook(unit) != 0) {
        unit->status.flags &= ~4;
        unit->status.flags &= ~8;
        if (unit->status.flags & 2) {
            unit->ext->owner->flags |= MDL_SKIP_TRANSFORMS;
        }
    }
}

u32 btlIsUnitInfoFlagOneEligible(BtlUnit *unit) {
    u32 flags = unit->status.flags;
    u8 modelFlags;

    if (flags & 0x08000000) {
        return 0;
    }
    if (!(flags & 1)) {
        return 0;
    }
    if (!(flags & 2)) {
        return 0;
    }
    modelFlags = unit->ext->owner->flags;
    return modelFlags & MDL_SKIP_TRANSFORMS;
}

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417940);

void btlApplyUnitMotionSelection(BtlUnit *unit, u32 index, s32 mode, f32 rate) {
    BtlState *work;
    BtlEffectResource *table;
    BtlRuntimeTask *task;
    MdlCtx *model;
    s32 selected;
    s32 node;
    s32 start;
    s32 end;
    u16 frameCount;
    f32 scale;
    u32 color;
    s32 (*chooseMotion)(BtlUnit *, s32, s32);
    s32 (*keepRange)(BtlUnit *, s32);
    s32 (*chooseMode)(BtlUnit *, s32, s32);

    if ((unit->status.flags & 2) == 0) {
        return;
    }
    if (unit->updateFlags & 1) {
        if (unit->status.flags & 0x2000) {
            switch (index) {
            case 1:
            case 11:
            case 18:
                task = btlCreateStiffenDamageShakeTask(unit, 8.0f);
                task->startDelay = 1;
                task->ownerId = 0;
                btlStartTask(task);
                break;
            }
        }
        return;
    }
    work = (BtlState *)btlGetRuntime();
    if (unit->updateFlags & 2) {
        color = (unit->overlayColor & 0xFFFFFF) | 0x80000000;
        evtSetUnitRgbTransition(unit->ext, 0, color);
        evtSetUnitAlphaTransition(unit->ext, 0, color);
        unit->overlayColor = color;
        unit->updateFlags &= ~4;
        unit->updateFlags &= ~2;
    }
    table = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(unit->resourceKind,
                                                                unit->resourceIndex);
    if (table->nodes[index].rateKind == 2) {
        unit->updateFlags |= 6;
    }
    chooseMotion = work->chooseMotion;
    if (chooseMotion != 0) {
        selected = chooseMotion(unit, index, 0);
        if (selected == -1) {
            return;
        }
        if (index != selected) {
            scale = 1.0f;
            if (table->nodes[index].scale > 0.0f) {
                scale = rate / table->nodes[index].scale;
            }
            index = selected;
            mode = btlGetSlotRateKind(unit, index);
            rate = scale * table->nodes[index].scale;
        }
    }
    if (unit->unkEC == -1) {
        start = 0;
        end = 0;
    } else {
        switch (index) {
        case 11:
            mode = 2;
        case 0: case 2: case 9: case 10:
            start = unit->unkF8;
            end = unit->unkFA;
            break;
        case 1: case 18:
            start = 0;
            end = 1;
            break;
        case 3: case 4: case 5: case 6: case 7: case 8:
        case 12: case 16: case 17: case 19: case 20: case 21:
        case 22: case 23: case 24:
            node = mdlGetNodeMotionIndex(unit->ext->owner, 0);
            switch (node) {
            case 0: case 2: case 9: case 10: case 11:
                start = 0;
                end = 5;
                break;
            default:
                start = 0;
                end = 0;
                break;
            }
            break;
        case 15:
            start = 0;
            end = 0;
            break;
        default:
            start = 0;
            end = 5;
            break;
        }
    }
    keepRange = work->unk5DC;
    if (keepRange != 0 && keepRange(unit, index) != 0) {
        start = unit->unkF8;
        end = unit->unkFA;
    }
    if (mode & 0x100) {
        end = 8;
        mode &= ~0x100;
    }
    chooseMode = work->unk6FC;
    if (chooseMode != 0) {
        mode = chooseMode(unit, index, mode);
    }
    unit->fF4 = rate;
    unit->unkEC = index;
    unit->effectState = mode;
    rate = rate * (30.0f / work->unk4C4);
    rate *= work->unk4C8;
    if (work->unk6F0 != 0) {
        work->unk6F0(unit, index, start, end, mode, rate);
    } else {
        evtPrepareUnitMotionState(unit->ext, index, start, end, mode);
        model = unit->ext->owner;
        model->first->frameStep = rate;
        if (end == 0) {
            mdlAddEntryFlagged(model, 0, index);
            sdfMotionSampleAtFrame(unit->ext->owner->first, 0.0f);
        }
    }
    unit->unkF8 = 0;
    frameCount = table->nodes[index].frameCount;
    unit->unkFA = frameCount;
    if (mode != 0 && mode != 3) {
        return;
    }
    if (work->unk6F8 != 0) {
        work->unk6F8(unit, 0, (s16)frameCount);
    } else {
        evtStoreUnitMotionShortParameters(unit->ext, 0, (s16)frameCount);
    }
    unit->unkF8 = 0;
    unit->unkFA = table->nodes[unit->effectIndex].frameCount;
}

void btlRefreshUnitMotionSelection(BtlUnit *unit) {
    s32 entryFlags;
    BtlState *work;
    s32 index;
    s32 selected;
    s32 mode;
    BtlEffectResource *resource;
    f32 rate;
    f32 speed;
    u32 color;
    s32 (*chooseStatus)(BtlUnit *);
    s32 (*chooseMotion)(BtlUnit *, s32, s32);
    void (*setMotion)(BtlUnit *, s32, f32);
    s32 (*chooseMode)(BtlUnit *, s32, s32);

    if ((unit->status.flags & 2) == 0) {
        return;
    }
    entryFlags = btlGetEntryFlagsUnlessDisabled(&unit->partyRecord);
    work = (BtlState *)btlGetRuntime();
    if (unit->updateFlags & 2) {
        color = (unit->overlayColor & 0xFFFFFF) | 0x80000000;
        evtSetUnitRgbTransition(unit->ext, 0, color);
        evtSetUnitAlphaTransition(unit->ext, 0, color);
        unit->overlayColor = color;
        unit->updateFlags &= ~4;
        unit->updateFlags &= ~2;
    }
    index = 0;
    if (btlIsCurrentValueBelowQuarterThreshold(unit) != 0 &&
        ((unit->status.flags & 0x200) || (entryFlags & 0x200))) {
        index = 10;
    }
    if (unit->selectedEntryIndex > 0 &&
        ((unit->status.flags & 0x200) || (entryFlags & 0x200))) {
        index = 9;
    }
    switch (unit->partyRecord.status & 0x7FFF) {
    case 1: case 8: case 0x10: case 0x20: case 0x40:
    case 0x80: case 0x100: case 0x200: case 0x400: case 0x2000:
        index = 2;
        break;
    }
    if (btlTestActorStatusPredicate(unit) != 0) {
        if ((unit->status.flags & 0x2000) == 0) {
            unit->status.flags |= 0x80002000;
        }
    } else if (unit->status.flags & 0x2000) {
        btlApplyUnitModelScaledValue((u8 *)unit);
        unit->status.flags &= 0x7FFFFFFF;
        unit->status.flags &= ~0x2000;
    }
    chooseStatus = work->unk5D8;
    if (chooseStatus != 0) {
        selected = chooseStatus(unit);
        if (selected >= 0) {
            index = selected;
        }
    }
    if (btlIsUnitDefeatTriggeredByValueDelta(unit, 0) != 0 &&
        ((unit->status.flags & 0x200) || (entryFlags & 0x200)) &&
        ((unit->status.stateFlags & 0x40) == 0)) {
        index = 11;
        btlApplyUnitModelScaledValue((u8 *)unit);
        unit->status.flags &= 0x7FFFFFFF;
        unit->status.flags &= ~0x2000;
    }
    resource = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(
        unit->resourceKind, unit->resourceIndex);
    rate = resource->nodes[index].scale;
    chooseMotion = work->chooseMotion;
    if (chooseMotion != 0) {
        selected = chooseMotion(unit, index, 1);
        if (selected == -1) {
            return;
        }
        if (index != selected) {
            index = selected;
            rate = resource->nodes[index].scale;
        }
    }
    unit->effectScale = rate;
    speed = rate * (30.0f / work->unk4C4);
    speed *= work->unk4C8;
    setMotion = work->unk6F4;
    unit->effectIndex = index;
    if (setMotion != 0) {
        setMotion(unit, index, speed);
    } else {
        evtUnitSetStoredParameter(unit->ext, index);
        evtSetTransitionMotionScale(unit->ext, speed);
    }
    chooseMode = work->unk6FC;
    mode = index != 11 ? 1 : 2;
    if (chooseMode != 0) {
        mode = chooseMode(unit, index, mode);
    }
    unit->effectParameter = mode;
    if (unit->unkEC != 11 &&
        (btlIsActorModeAcceptedByBattleHook(unit) != 0 || index == 11) &&
        unit->unkEC != index) {
        btlApplyUnitMotionSelection(unit, index, mode, rate);
    }
}

s32 btlIsActorModeAcceptedByBattleHook(BtlUnit *unit) {
    BtlState *work;
    if (!(unit->status.flags & 2)) {
        return 0;
    }
    work = (BtlState *)btlGetRuntime();
    if (work->unk5D8 != 0 && work->unk5D8(0) == unit->unkEC) {
        return 1;
    }
    switch (unit->unkEC) {
    case 0:
    case 2:
    case 9:
    case 10:
    case 11:
        return 1;
    default:
        return 0;
    }
}

extern s32 btlGetSideIndexedActorStatusTable(s32, s32);

extern void btlApplyUnitMotionSelection(BtlUnit *, u32, s32, f32);

void btlApplyScaledUnitEffectParameter(BtlUnit *unit, s32 index, s32 option, f32 scale) {
    u8 *table = (u8 *)btlGetSideIndexedActorStatusTable(((BtlUnit *)unit)->resourceKind, ((BtlUnit *)unit)->resourceIndex);
    btlApplyUnitMotionSelection(unit, index, option, ((BtlEffectResource *)table)->nodes[index].scale * scale);
}

s32 btlGetSlotRateKind(u8 *unit, s32 index) {
    u8 *table = (u8 *)btlGetSideIndexedActorStatusTable(((BtlUnit *)unit)->resourceKind, ((BtlUnit *)unit)->resourceIndex);
    s32 value = ((BtlEffectResource *)table)->nodes[index].rateKind;
    switch (value) {
    case 0:
        return 0;
    case 1:
    case 2:
    case 3:
        return 2;
    default:
        return 0;
    }
}

void btlUpdateUnitEffects(void) {
    s32 context = btlGetRuntime();
    BtlUnit *object = ((BtlState *)context)->units;

    while (object != 0) {
        if (object->status.flags & 2) {
            u8 *resource = (u8 *)btlGetSideIndexedActorStatusTable(object->resourceKind,
                                                  object->resourceIndex);
            MdlCtx *model = object->ext->owner;
            s32 node = mdlGetNodeMotionIndex(model, 0);
            if (((BtlEffectResource *)resource)->nodes[node].rateKind == 1 &&
                btlIsActorModeAcceptedByBattleHook(object) == 0) {
                btlRefreshUnitMotionSelection(object);
                btlApplyUnitMotionSelection(object, object->effectIndex,
                              object->effectParameter,
                              object->effectScale);
            }
        }
        object = object->nextActor;
    }
}

void btlApplyUnitModelScaledValue(u8 *object) {
    s32 context;
    f32 volume;
    if ((((BtlUnit *)object)->status.flags & 2) == 0) {
        return;
    }
    context = btlGetRuntime();
    ((BtlUnit *)object)->updateFlags &= ~1;
    volume = ((BtlUnit *)object)->fF4;
    ((BtlUnit *)object)->ext->owner->first->frameStep =
        volume * (30.0f / (f32)((BtlState *)context)->unk4C4);
}

void btlResetUnitModelProgress(BtlUnit *unit) {
    if (unit->status.flags & 2) {
        unit->updateFlags |= 1;
        unit->ext->owner->first->frameStep = 0.0f;
    }
}

s32 func_001E2E58(BtlUnit *unit, s32 motionIndex) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlActorStatusRecord *status = (BtlActorStatusRecord *)
        btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->resourceIndex);

    return (s32)(status->motions[motionIndex].frameCount /
        (status->motions[motionIndex].alphaFrameScale * state->unk4C8));
}

f32 btlGetUnitModelValue1C(BtlUnit *unit) {
    f32 value = 0.0f;
    if (unit->status.flags & 2) {
        value = unit->ext->owner->first->currentFrame;
    }
    return value;
}

void btlAdvanceUnitModelFrame(BtlUnit *unit, f32 frame) {
    if ((unit->status.flags & 2) != 0) {
        sdfMotionSampleAtFrame(unit->ext->owner->first, frame);
        return;
    }
}

s32 btlGetUnitModelFrameCount(BtlUnit *unit) {
    if (!(unit->status.flags & 2)) {
        return 0;
    }
    return unit->ext->owner->first->frameCount;
}

void btlSeekUnitModelFrameZero(BtlUnit *unit) {
    if (unit->status.flags & 2) {
        sdfMotionSampleAtFrame(unit->ext->owner->first, 0.0f);
    }
}

extern u32 effMiscRandMod(void *state, u32 modulus);

void btlSeekRandomModelFrame(BtlUnit *object) {
    s32 duration;
    u32 randomFrame;
    f32 frame;

    if ((object->status.flags & 2) == 0) {
        return;
    }
    duration = btlGetUnitModelFrameCount(object);
    if (duration > 0) {
        randomFrame = effMiscRandMod(0, duration);
        frame = (f32)randomFrame;
        sdfMotionSampleAtFrame(object->ext->owner->first, frame);
    }
}

s32 btlIsUnitModelStateFive(BtlUnit *unit) {
    if (!(unit->status.flags & 2)) {
        return 1;
    }
    if (unit->effectState != 2) {
        return 1;
    }
    return unit->ext->owner->first->state == SDF_MOTION_STATE_TERMINAL;
}

void btlSetUnitPosition(BtlUnit *unit, f32 *vec) {
    f32 pos[4];
    if (!(unit->status.stateFlags & 0x80)) {
        u8 *work = (u8 *)btlGetRuntime();
        VU0_LOAD_VF(vf10, vec);
        VU0_STORE_VF_UNCLOBBERED(vf10, unit->currentPosition);
        VU0_LOAD_VF(vf11, work);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        if (unit->status.flags & 2) {
            pos[2] += unit->positionZOffset;
            effObjSetInnerPosition(unit->effectObject, (u128 *)pos);
        }
    }
}

void func_001E3108(void *object, f32 *dst) {
    PCP_COPY_VECTOR(dst, ((BtlUnit *)object)->currentPosition);
}

void btlGetUnitWorldPos(BtlUnit *unit, f32 *dst) {
    u8 *work = (u8 *)btlGetRuntime();
    VU0_LOAD_VF(vf10, unit->currentPosition);
    VU0_LOAD_VF(vf11, work);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, dst);
}

extern s32 sdfLoadMapRecordPositionVector(SdfModel *, s32);





extern void sdfModelUpdateCurrentFrameTransforms(SdfModel *);

extern void btlRefreshUnitFxVectors(BtlUnit *);

s8 btlSetActorEffectParameter(BtlUnit *unit, s32 mode) {
    s32 (*hook)(BtlUnit *, s32);
    if (!(unit->status.flags & 2)) {
        return 0;
    }
    hook = ((BtlState *)btlGetRuntime())->effectParameterCallback;
    if (hook != 0) {
        mode = hook(unit, mode);
    }
    btlRefreshUnitFxVectors(unit);
    return sdfLoadMapRecordPositionVector(unit->ext->owner->inner, mode);
}

void btlSetActorEffectParameterOrMuzzlePosition(BtlUnit *unit, s32 mode) {
    if (btlSetActorEffectParameter(unit, mode) == 0) {
        btlUnitGetMuzzlePosVU(unit);
    }
}

/* vu0 routine: preserve the actor's primary position and rotation quaternion while
 * evaluating the requested model record; return the sampled vector in vf10. */
s32 func_001E3230(BtlUnit *unit, s32 value) {
    f32 currentVector[4] __attribute__((aligned(16)));
    f32 primaryVector[4] __attribute__((aligned(16)));
    f32 rotationQuaternion[4] __attribute__((aligned(16)));
    s32 (*callback)(BtlUnit *, s32);
    s8 result;

    if ((unit->status.flags & 2) == 0) {
        return 0;
    }
    callback = ((BtlState *)btlGetRuntime())->effectParameterCallback;
    if (callback != NULL) {
        value = callback(unit, value);
    }
    mdlLoadPrimaryVectorVU(unit->ext->owner);
    VU0_STORE_VF_UNCLOBBERED(vf10, primaryVector);
    mdlLoadRotationQuaternionVU(unit->ext->owner);
    VU0_STORE_VF_UNCLOBBERED(vf10, rotationQuaternion);
    btlRefreshUnitFxVectors(unit);
    result = sdfLoadMapRecordPositionVector(unit->ext->owner->inner, value);
    VU0_STORE_VF_UNCLOBBERED(vf10, currentVector);
    VU0_LOAD_VF(vf10, primaryVector);
    mdlStorePrimaryVectorVU(unit->ext->owner);
    VU0_LOAD_VF(vf10, rotationQuaternion);
    mdlUpdateContextRotationBasisFromQuaternion(unit->ext->owner);
    sdfModelUpdateCurrentFrameTransforms(unit->ext->owner->inner);
    VU0_LOAD_VF(vf10, currentVector);
    return result;
}

extern s32 sdfLoadMapRecordLookAtBasis(SdfModel *, s32);

s8 btlSetActorAlternateEffectParameter(unit, mode)
    BtlUnit *unit;
    s32 mode;
{
    s32 (*hook)(BtlUnit *, s32);
    if (!(unit->status.flags & 2)) {
        return 0;
    }
    hook = ((BtlState *)btlGetRuntime())->effectParameterCallback;
    if (hook != 0) {
        mode = hook(unit, mode);
    }
    btlRefreshUnitFxVectors(unit);
    return sdfLoadMapRecordLookAtBasis(unit->ext->owner->inner, mode);
}

void btlSetAlternateEffectParameterOrMuzzlePosition(BtlUnit *unit, s32 mode) {
    if (btlSetActorAlternateEffectParameter(unit, mode) == 0) {
        VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
    }
}

s32 btlIsUnitAtStoredPosition(u8 *object) {
    f32 position[4];
    func_001E3108(object, position);
    if (((BtlUnit *)object)->position[0] == position[0] &&
        ((BtlUnit *)object)->position[1] == position[1] &&
        ((BtlUnit *)object)->position[2] == position[2]) {
        return 1;
    }
    return 0;
}

extern u8 D_004179E0[];

extern void effMiscQuatMultiplyVU(void);

void btlSetUnitRotation(BtlUnit *unit, s128 *quat) {
    f32 result[4];
    if (!(unit->status.stateFlags & 0x100)) {
        VU0_LOAD_VF(vf10, quat);
        if (unit->status.flags & 0x10) {
            VU0_LOAD_VF(vf11, D_004179E0);
            effMiscQuatMultiplyVU();
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, unit->orientation);
        VU0_LOAD_VF(vf11, D_004179E0);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF_UNCLOBBERED(vf10, result);
        if (unit->status.flags & 2) {
            effObjSetInnerRotation(unit->effectObject, (u128 *)result);
        }
    }
}

void btlCopyUnitRotationQuaternion(BtlUnit *unit, void *dst) {
    PCP_COPY_VECTOR(dst, unit->orientation);
}

void btlSetUnitColor(BtlUnit *unit, u32 color, s32 mode) {
    if (unit->status.flags & 2) {
        color = (color & 0xFFFFFF) | 0x80000000;
        unit->baseColor = (unit->baseColor & 0xFF000000) | (color & 0xFFFFFF);
        evtSetUnitRgbTransition(unit->ext, mode, color);
    }
}

void btlBlendUnitColor(BtlUnit *unit, u32 color, s32 mode) {
    u32 base;
    u32 blended;
    if (unit->status.flags & 2) {
        color = (color & 0xFFFFFF) | 0x80000000;
        base = (unit->baseColor & 0xFFFFFF) | 0x80000000;
        blended = (base & color) + (((base ^ color) & 0xFEFEFEFE) >> 1);
        unit->overlayColor = (unit->overlayColor & 0xFF000000) | (color & 0xFFFFFF);
        evtSetUnitRgbTransition(unit->ext, mode, blended);
    }
}

extern void mdlReleaseInnerResourceHandle(MdlCtx *, s32, f32);

/* Forward the packed model-color word and its scalar to the inner resource list. */
void btlReleaseUnitModelColorResource(BtlUnit *unit, u32 value, f32 scalar) {
    mdlReleaseInnerResourceHandle(unit->ext->owner, (value & 0xFFFFFF) | 0x80000000, scalar);
}

void btlRefreshUnitFxVectors(BtlUnit *unit) {
    if (!(unit->status.flags & 2)) {
        return;
    }
    effObjFetchInnerPosition(unit->effectObject);
    mdlStorePrimaryVectorVU(unit->ext->owner);
    effObjFetchInnerRotationNormalized(unit->effectObject);
    mdlUpdateContextRotationBasisFromQuaternion(unit->ext->owner);
    sdfModelUpdateCurrentFrameTransforms(unit->ext->owner->inner);
}

extern s32 btlAimHorizontalDirectionVU(f32 *, f32 *);

extern void btlUnitGetBodyPosVU(BtlUnit *);

extern void btlSetUnitRotation(BtlUnit *, s128 *);

void btlUnitFaceTarget(BtlUnit *unit, BtlUnit *target) {
    s128 from;
    s128 to;
    s128 hit;
    if (unit->status.flags & 0x80000) {
        btlUnitGetBodyPosVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, &from);
        btlUnitGetBodyPosVU(target);
        VU0_STORE_VF_UNCLOBBERED(vf10, &to);
        if (btlAimHorizontalDirectionVU((f32 *)&from, (f32 *)&to) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, &hit);
            btlSetUnitRotation(unit, &hit);
        }
    }
}

extern s32 btlAimHorizontalDirectionClampedVU(f32 *, f32 *, f32);

void btlUnitFaceTargetScaled(BtlUnit *unit, BtlUnit *target, f32 scale) {
    s128 from;
    s128 to;
    s128 hit;
    if (unit->status.flags & 0x80000) {
        btlUnitGetBodyPosVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, &from);
        btlUnitGetBodyPosVU(target);
        VU0_STORE_VF_UNCLOBBERED(vf10, &to);
        btlAimHorizontalDirectionClampedVU((f32 *)&from, (f32 *)&to, scale);
        VU0_STORE_VF_UNCLOBBERED(vf10, &hit);
        btlSetUnitRotation(unit, &hit);
    }
}

typedef struct {
    u8 unk00[0x110];
    u32 flags;
    u8 unk114[0x10];
    u16 objectId;
} BattleEntryHeader;

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004179E0);

s32 btlClassifySpecialEntryObject(BtlUnit *entry) {
    if (!(entry->status.flags & 0x400)) {
        return 0;
    }
    switch (entry->partyRecord.unitId) {
    case 0x109: case 0x10A: case 0x110: case 0x111: case 0x112:
    case 0x119: case 0x11D: case 0x11E: case 0x11F: case 0x120:
    case 0x121: case 0x127: case 0x12E: case 0x12F: case 0x131:
    case 0x132: case 0x133: case 0x134: case 0x135: case 0x136:
        return 2;
    default:
        return (btlGetEntryFlagsUnlessDisabled(&entry->partyRecord) >> 14) & 1;
    }
}

void btlCopyUnitStats(BtlUnit *unit, DatPartyRecord *source) {
    DatPartyRecord *stats = &unit->partyRecord;
    *stats = *source;
    btlRefreshUnitMaximumHpAndClampCurrentHp(stats);
    btlRefreshUnitMaximumMpAndClampCurrentMp(stats);
}

extern s32 sdfAllocPacketAligned(s32);

extern void func_003325F8(SdfModel *, SdfModel *);

extern void func_003320E8(SdfPoolNode **, SdfModel *);

extern void mdlDispatchViewerAnchorRecord(MdlCtx *, MdlResourceItem *);

extern u64 D_003B6BB0[4];

void func_001E38F0(BtlUnit *unit, MdlCtx *model, SdfModel *overlay,
                   SdfPoolNode **surfaces, u32 frame) {
    SdfListHead *list;
    u64 *packet;
    MdlResourceItem *item;
    u16 savedFlags;
    s32 i;

    if (model->flags & MDL_SKIP_TRANSFORMS) {
        return;
    }
    mdlBroadcastMasked(model, frame);
    for (i = 0; i != 4; i++) {
        list = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        packet = (u64 *)sdfAllocPacketAligned(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = 0x72801;
        packet[5] = 0x47;
        sdfAppendPacket(list, (u32)packet);
        surfaces[i]->append(surfaces[i], list);
    }
    savedFlags = model->inner->unk1A;
    model->flags |= MDL_SKIP_ANCHORS;
    model->inner->unk1A = 0x2000;
    mdlProcessContextNodesAndTransforms(model, surfaces);
    model->flags &= ~MDL_SKIP_ANCHORS;
    model->inner->unk1A = savedFlags;
    for (i = 0; i != 4; i++) {
        list = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        packet = (u64 *)sdfAllocPacketAligned(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = 0x51801;
        packet[5] = 0x47;
        sdfAppendPacket(list, (u32)packet);
        surfaces[i]->append(surfaces[i], list);
    }
    func_003325F8(overlay, model->inner);
    if (unit->status.flags & 2) {
        overlay->lighting = unit->ext->endpointWork;
    } else {
        overlay->lighting = NULL;
    }
    func_003320E8(surfaces, overlay);
    for (i = 0; i != 4; i++) {
        list = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        packet = (u64 *)sdfAllocPacketAligned(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = D_003B6BB0[i];
        packet[5] = 0x47;
        sdfAppendPacket(list, (u32)packet);
        surfaces[i]->append(surfaces[i], list);
    }
    mdlSetAllResourceFrames(model, frame);
    for (item = model->resourceItems; item != NULL; item = item->next) {
        mdlDispatchViewerAnchorRecord(model, item);
    }
}

extern void dds3SetObjectFlags(void *, u32);

void btlCreateUnitTransparency(BtlUnit *unit) {
    BattleGroupNode *shape;
    if ((unit->status.flags & 2) == 0) {
        return;
    }
    if (unit->unk344 != 0) {
        return;
    }
    if (unit->unkCC != 0) {
        return;
    }
    shape = unit->ext->owner->sub;
    unit->unk344 = (s32)sdfModelCreateWithItems(shape->resourceList, shape->itemList);
    dds3SetObjectFlags(unit->effectObject, 1);
    btlBossDebugPrintf("btl:unit transparency create[%p]\n", unit);
}

void btlUpdateUnitTransparency(BtlUnit *unit) {
    u32 flags = unit->status.flags;
    u32 color;
    MdlCtx *info;
    u32 alpha;
    if (flags & 2) {
        if (unit->unkCC == 0) {
            color = unit->overlayColor;
            info = unit->ext->owner;
            alpha = color >> 24;
            if (!(flags & 0x20000)) {
                if (unit->unk344 != 0) {
                    sdfReleaseDevSlot(unit->unk344, 1, 1);
                    unit->unk344 = 0;
                    if (unit->status.flags & 2) {
                        info->inner->lighting = unit->ext->endpointWork;
                    } else {
                        info->inner->lighting = 0;
                    }
                    mdlBroadcastMasked(info, color);
                    mdlProcessContextNodesAndTransforms(info, D_00380788[0]);
                    dds3ClearObjectFlags(unit->effectObject, 1);
                    btlBossDebugPrintf(D_00417940, unit);
                }
            } else if (alpha == 0) {
                dds3SetObjectFlags(unit->effectObject, 1);
            } else if (unit->unk344 == 0) {
                btlCreateUnitTransparency(unit);
            } else {
                func_001E38F0(unit, info, (SdfModel *)unit->unk344, D_003B6BD0, color);
            }
        }
    }
}

extern SdfGraphObj D_0040B290;

extern SdfPoolNode *D_003B6BE0[];

extern SdfPoolNode *D_003B6BF0[];

extern s32 sdfAllocPacketAligned(s32);

void func_001E3E20(BtlUnit *unit) {
    MdlCtx *info;
    SdfListHead *list;

    if ((unit->status.flags & 2) == 0) {
        return;
    }
    if (unit->unkCC != 0) {
        return;
    }
    unit->mirror->unk34C = sdfAllocPacketAligned(0x70000);
    list = sdfAllocatePacketList(0);
    sdfCreateResourcePacket(list, D_0040B290.buffers[2],
                            0, 0, 0x200, 0xE0, unit->mirror->unk34C, 0, 0, 0);
    D_003B6BE0[0]->append(D_003B6BE0[0], list);
    info = unit->ext->owner;
    if (unit->unk344 == 0) {
        unit->unk344 = (s32)sdfModelCreateWithItems(info->sub->resourceList, info->sub->itemList);
        dds3SetObjectFlags(unit->effectObject, 1);
        return;
    }
    func_001E38F0(unit, info, (SdfModel *)unit->unk344, D_003B6BE0, unit->overlayColor);
    info = unit->mirror->ext->owner;
    if (unit->mirror->unk344 == 0) {
        unit->mirror->unk344 = (s32)sdfModelCreateWithItems(info->sub->resourceList, info->sub->itemList);
        dds3SetObjectFlags(unit->mirror->effectObject, 1);
        return;
    }
    list = sdfAllocatePacketList(0);
    sdfCreateDescriptorPacket(list, D_0040B290.buffers[2],
                              0, 0, 0x200, 0xE0, unit->mirror->unk34C, 0);
    D_003B6BF0[0]->append(D_003B6BF0[0], list);
    func_001E38F0(unit->mirror, info, (SdfModel *)unit->mirror->unk344, D_003B6BF0, unit->mirror->overlayColor);
}

extern char D_00436A28[];

s32 btlFormatUnitBedName(BtlUnit *unit, char *name) {
    btlGetRuntime();
    if (unit->status.flags & 0x200) {
        if (unit->partyRecord.flags & 0x10) {
            func_0035C860(name, "%s%03X_%02X.BED", D_00436A28, 0, unit->partyRecord.unitId + 0x20);
        } else if (unit->status.flags & 0x1000) {
            func_0035C860(name, "%s%03X_%02X.BED", D_00436A28, 0, unit->partyRecord.unitId);
        } else {
            func_0035C860(name, "%s%03X_%02X.BED", D_00436A28, btlGetActorBedAssetIdFromIndex(unit->partyRecord.menuValue), unit->partyRecord.unitId);
        }
        return 1;
    }
    return 0;
}

extern void effMiscQuaternionToMatrixVU(void);

const char D_00417AE0[16] = "btl:warp[%p]\n";

void func_001E40F0(BtlUnit *unit, BtlUnit *target, s32 index) {
    f32 bodyPos[4] __attribute__((aligned(16)));
    f32 muzzlePos[4] __attribute__((aligned(16)));
    f32 world[4] __attribute__((aligned(16)));
    BtlEffectResource *table;
    f32 reach;
    f32 margin;
    f32 distance;
    f32 scale;

    if (index < 0) {
        return;
    }
    if ((unit->status.stateFlags & 0x8000) != 0) {
        return;
    }
    if ((unit->status.flags & 0x200) != 0) {
        if (unit->partyRecord.unitId == 2 || unit->partyRecord.unitId == 8) {
            return;
        }
    }
    table = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->resourceIndex);
    if (table->nodes[index].triggerKind != 2) {
        return;
    }
    scale = unit->scale;
    reach = table->nodes[index].reachOffset;
    reach *= scale;
    margin = unit->reach;
    margin *= scale;
    if (reach <= scale * 100.0f || reach <= margin) {
        return;
    }
    reach -= margin;
    btlUnitGetBodyPosVU((u8 *)unit);
    VU0_STORE_VF(vf10, bodyPos);
    if (target != NULL) {
        btlUnitGetMuzzlePosVU(target);
        VU0_STORE_VF_UNCLOBBERED(vf10, muzzlePos);
        muzzlePos[1] = bodyPos[1];
        VU0_LOAD_VF(vf10, muzzlePos);
        VU0_LOAD_VF(vf11, bodyPos);
        VU0_SUB(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();
    } else {
        VU0_LOAD_VF(vf10, unit->orientation);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_003E9130);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_LOAD_VF(vf11, bodyPos);
    }
    VU0_SCALAR_OP(reach, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, world);
    VU0_LOAD_VF(vf10, unit->orientation);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, unit->bodyOffset);
    VU0_SCALAR_OP(unit->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_NEGATE_XYZ(vf10);
    VU0_LOAD_VF(vf11, world);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, world);
    if ((unit->status.flags & 2) != 0) {
        world[1] = 0;
        if (sdfLoadMapRecordPositionVector(unit->ext->owner->inner, 0) == 0) {
            effObjFetchInnerPosition(unit->effectObject);
        }
        VU0_SCALAR_OP(0.0f, "vaddx.y vf10, vf0, vf2x");
        VU0_LOAD_VF(vf11, world);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(distance);
        if (distance < 2.0f * (unit->unkBC * unit->scale)) {
            effObjSetInnerPosition(unit->effectObject, (u128 *)world);
            unit->status.stateFlags |= 0x8000;
            btlBossDebugPrintf(D_00417AE0, unit);
        }
    }
}

void btlRefreshUnitEffectMotionAndEntry(BtlUnit *unit) {
    BtlState *work;
    if (!(unit->status.flags & 2)) {
        return;
    }
    work = (BtlState *)btlGetRuntime();
    if (unit->unkEC != unit->effectIndex) {
        btlRefreshUnitMotionSelection(unit);
        unit->unkF8 = 0;
        unit->unkFA = 0;
        btlApplyUnitMotionSelection(unit, unit->effectIndex, unit->effectParameter, unit->effectScale);
    }
    if (unit->status.flags & 0x2000) {
        return;
    }
    if (work->unk6F0 != 0) {
        if (unit->effectIndex != 0xB) {
            work->unk6F0(unit, unit->effectIndex, 0, 0, 1, 1.0f);
        } else {
            work->unk6F0(unit, unit->effectIndex, 0, 0, 2, 1.0f);
        }
    } else {
        if (unit->effectIndex != 0xB) {
            mdlAddEntryFlagged(unit->ext->owner, 0, unit->effectIndex);
        } else {
            mdlAddEntryPlain(unit->ext->owner, 0, unit->effectIndex);
        }
        sdfMotionSampleAtFrame(unit->ext->owner->first, 0.0f);
    }
}

u32 btlApplyIndexedUnitEffectTask(u8 *arguments) {
    s32 index = ((SoundTaskArgs *)arguments)->option;
    if (index >= 0) {
        btlApplyScaledUnitEffectParameter(((SoundTaskArgs *)arguments)->actor, index, ((SoundTaskArgs *)arguments)->unk_08,
                        ((SoundTaskArgs *)arguments)->scale2);
    }
    return 1;
}

BtlRuntimeTask *btlAllocateIndexedUnitEffectTask(BtlUnit *unit, s32 index, s32 value, f32 scale) {
    BtlRuntimeTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 9;
    task->callback = btlApplyIndexedUnitEffectTask;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    args->option = index;
    args->unk_08 = value;
    args->scale2 = scale;
    return task;
}

u32 btlApplyScaledUnitModelTask(u32 *taskArgs) {
    btlApplyUnitModelScaledValue(*taskArgs);
    return 1;
}

BtlRuntimeTask *btlCreateScaledUnitModelTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlApplyScaledUnitModelTask;
    task->taskId = 0xA;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    return task;
}

u32 btlPollThresholdTask(s32 *arguments) {
    if ((s32)btlGetUnitModelValue1C((BtlUnit *)arguments[0]) >= arguments[1]) {
        if ((((BtlUnit *)arguments[0])->updateFlags & 1) == 0) {
            btlResetUnitModelProgress((BtlUnit *)arguments[0]);
        }
        return 1;
    }
    return 0;
}

BtlRuntimeTask *btlScheduleThresholdTask(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0xB;
    task->ownerId = actor->owner;
    task->callback = btlPollThresholdTask;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = option;
    return task;
}

typedef struct BtlApproachTaskArgs {
    BtlUnit *unit;
    BtlUnit *target;
    f32 offset;
    f32 scale;
    s32 unk10;
    s32 count;
} BtlApproachTaskArgs;

/* vu0 routine: move the unit along the line to the target's muzzle, offset by reach */
s32 btlApproachTargetTask(BtlApproachTaskArgs *args) {
    BtlUnit *unit = args->unit;
    BtlUnit *target = args->target;
    f32 scale;
    f32 reach;
    f32 dist;
    f32 pos[4];
    s128 fromPos;
    s128 toPos;
    scale = args->scale == 0.0f ? 1.0f : args->scale;
    if (args->count == 0) {
        BtlEffectResource *table = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->resourceIndex);
        args->offset = table->nodes[unit->unkEC].reachOffset * unit->scale;
    }
    reach = args->offset + target->reach * target->scale;
    btlUnitGetMuzzlePosVU(unit);
    VU0_STORE_VF_UNCLOBBERED(vf10, &fromPos);
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF_UNCLOBBERED(vf10, &toPos);
    ((f32 *)&toPos)[1] = ((f32 *)&fromPos)[1];
    VU0_LOAD_VF(vf10, &fromPos);
    VU0_LOAD_VF(vf11, &toPos);
    VU0_MOVE_VF(vf12, vf10);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_SCALAR_OP(reach, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf12);
    VU0_LERP_VF10(0.8f / scale);
    VU0_STORE_VF(vf10, pos);
    VU0_LOAD_VF(vf11, &fromPos);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(dist);
    VU0_LOAD_VF(vf10, unit->orientation);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, unit->bodyOffset);
    VU0_SCALAR_OP(unit->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_NEGATE_XYZ(vf10);
    VU0_LOAD_VF(vf11, pos);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, pos);
    pos[2] -= unit->positionZOffset;
    btlSetUnitPosition(args->unit, pos);
    btlUnitFaceTarget(unit, target);
    if (dist < 1.0f) {
        return 1;
    }
    args->count++;
    return 0;
}

BtlRuntimeTask *btlAllocateApproachTargetTask(BtlUnit *unit, s32 index, f32 scale) {
    BtlRuntimeTask *task = btlAllocTask(24);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlApproachTargetTask;
    task->taskId = 0xE;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    args->option = index;
    args->scale2 = scale;
    args->unk_08 = 0;
    args->unk_14 = 0;
    return task;
}

typedef struct BtlPosLerpTaskArgs {
    s128 from;
    s128 to;
    f32 rate;
    f32 t;
    s32 count;
    BtlUnit *unit;
} BtlPosLerpTaskArgs;

s32 btlUpdateUnitPositionInterpolationTask(BtlPosLerpTaskArgs *args) {
    s128 pos;
    f32 t = args->t;
    f32 rate;
    BtlUnit *unit = args->unit;
    if (t < 1.0f && args->rate > 0.0f && args->rate < 1.0f) {
        rate = args->rate;
        if (args->count == 0) {
            PCP_COPY_VECTOR(&args->from, unit->currentPosition);
        }
        args->t = t + (1.0f - t) * rate;
        if (args->t > 0.999f) {
            args->t = 1.0f;
        }
        VU0_LOAD_VF(vf10, &args->from);
        VU0_LOAD_VF(vf11, &args->to);
        VU0_LERP_VF10(args->t);
        VU0_STORE_VF(vf10, &pos);
        btlSetUnitPosition(unit, (f32 *)&pos);
    } else {
        btlSetUnitPosition(unit, (f32 *)&args->to);
        return 1;
    }
    args->count++;
    return 0;
}

BtlRuntimeTask *btlCreateUnitPositionLerpTowardTargetTask(BtlUnit *unit, f32 *target, f32 scale) {
    BtlRuntimeTask *task = btlAllocTask(0x30);
    u8 *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0xC;
    task->ownerId = unit->owner;
    task->callback = btlUpdateUnitPositionInterpolationTask;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    ((BtlVectorTaskArgs *)args)->scale = scale;
    ((BtlVectorTaskArgs *)args)->unit2C = (u32)unit;
    ((BtlVectorTaskArgs *)args)->state24 = 0;
    ((BtlVectorTaskArgs *)args)->state28 = 0;
    PCP_COPY_VECTOR(args, unit->currentPosition);
    PCP_COPY_VECTOR(args + 0x10, target);
    return task;
}

typedef struct BtlSlerpTaskArgs {
    s128 from;
    s128 to;
    f32 rate;
    f32 t;
    s32 count;
    s8 mode;
    BtlUnit *unit;
} BtlSlerpTaskArgs;

s32 btlStepUnitRotationNlerp(BtlSlerpTaskArgs *args) {
    s128 quat;
    f32 t;
    f32 rate;
    BtlUnit *unit = args->unit;
    if (args->mode == 0 && !(unit->status.flags & 0x80000)) {
        return 1;
    }
    t = args->t;
    if (t < 1.0f) {
        rate = args->rate;
        if (rate > 0.0f && rate < 1.0f) {
            if (args->count == 0) {
                PCP_COPY_VECTOR(&args->from, unit->orientation);
            }
            args->t = t + (1.0f - t) * rate;
            if (args->t > 0.999f) {
                args->t = 1.0f;
            }
            VU0_LOAD_VF(vf10, &args->from);
            VU0_LOAD_VF(vf11, &args->to);
            effMiscQuaternionNlerpVU(args->t);
            VU0_STORE_VF(vf10, &quat);
            btlSetUnitRotation(unit, &quat);
            return 0;
        }
    }
    btlSetUnitRotation(unit, &args->to);
    return 1;
}

BtlRuntimeTask *btlCreateUnitRotationInterpolationTask(BtlUnit *unit, f32 *target, s8 mode, f32 scale) {
    BtlRuntimeTask *task = btlAllocTask(0x34);
    u8 *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0xD;
    task->ownerId = unit->owner;
    task->callback = btlStepUnitRotationNlerp;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    ((BtlVectorTaskArgs *)args)->scale = scale;
    ((BtlVectorTaskArgs *)args)->mode2C = mode;
    ((BtlVectorTaskArgs *)args)->unit30 = (u32)unit;
    ((BtlVectorTaskArgs *)args)->state24 = 0;
    ((BtlVectorTaskArgs *)args)->state28 = 0;
    PCP_COPY_VECTOR(args, unit->orientation);
    PCP_COPY_VECTOR(args + 0x10, target);
    return task;
}

void btlRequestModelOrReuse(u32 *arguments) {
    u8 *object = (u8 *)arguments[0];
    u32 effect = arguments[1];
    u32 model = arguments[2];
    if ((((BtlUnit *)object)->status.flags & 2) != 0) {
        return;
    }
    if (btlHasMatchingModel(effect, model)) {
        btlBindUnitModel(object, effect, model);
        if (*(char *)(arguments + 3) == 0) {
            btlClearUnitDefeatCandidate(object);
            evtSetUnitAlphaTransition(((BtlUnit *)object)->ext, 0, 0);
            ((BtlUnit *)object)->overlayColor = ((BtlUnit *)object)->baseColor & 0xFFFFFF;
        }
        btlBossDebugPrintf(D_00417AF0, effect, model);
    } else {
        btlRequestModelAssetByMode(object, effect, model);
        ((BtlUnit *)object)->gunResourceFlags |= 1;
        btlBossDebugPrintf(D_00417B10, effect, model);
    }
}

u32 btlPollModelLoadCompletion(u32 *arguments) {
    u8 *object = (u8 *)arguments[0];
    u32 effect = arguments[1];
    u32 model = arguments[2];
    if ((((BtlUnit *)object)->status.flags & 2) == 0) {
        if (!btlCheckModelAssetByMode(object, effect, model)) {
            return 0;
        }
        btlBindUnitModel(object, effect, model);
        btlReleaseModelAssetByMode(object, effect, model);
        btlBossDebugPrintf(D_00417B30, effect, model, object);
    }
    if (*(s8 *)(arguments + 3) == 0) {
        btlClearUnitDefeatCandidate(object);
        evtSetUnitAlphaTransition(((BtlUnit *)object)->ext, 0, 0);
        ((BtlUnit *)object)->overlayColor = ((BtlUnit *)object)->baseColor & 0xFFFFFF;
    }
    ((BtlUnit *)object)->gunResourceFlags = (((BtlUnit *)object)->gunResourceFlags & ~1) | 2;
    return 1;
}

BtlRuntimeTask *btlCreateModelLoadPollTask(BtlUnit *unit, u32 index, u32 value, s8 mode) {
    BtlRuntimeTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x18;
    task->flags &= ~BTL_TASK_FLAG_REGISTERED;
    task->ownerId = unit->owner;
    task->onStart = btlRequestModelOrReuse;
    task->callback = btlPollModelLoadCompletion;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    args->option = index;
    args->unk_08 = value;
    args->mode = mode;
    return task;
}

u32 btlReleaseUnitModelTask(u32 *taskArgs) {
    btlClearUnitDefeatCandidate(*taskArgs);
    btlReleaseActorModelResources(*taskArgs);
    return 1;
}

BtlRuntimeTask *btlScheduleRefreshTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlReleaseUnitModelTask;
    task->taskId = 0x19;
    task->ownerId = unit->owner;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    return task;
}

/* Task-start callbacks do not return a status to the scheduler. */
/* Exact 0x1C argument allocation owned by the model-change task creator. */
typedef struct BtlModelChangeArgs {
    BtlUnit *unit;
    u32 resourceKind;
    u32 resourceId;
    s32 delay;
    u32 duration;
    s32 elapsed;
    u8 phase;
    u8 transitionMode;
    u8 pad1A[2];
} BtlModelChangeArgs;

typedef char BtlModelChangeArgsSizeCheck[sizeof(BtlModelChangeArgs) == 0x1C ? 1 : -1];

typedef char BtlModelChangeArgsElapsedOffsetCheck[((u32)&((BtlModelChangeArgs *)0)->elapsed == 0x14) ? 1 : -1];

typedef char BtlModelChangeArgsPhaseOffsetCheck[((u32)&((BtlModelChangeArgs *)0)->phase == 0x18) ? 1 : -1];

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417AF0);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417B10);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417B30);

void btlBeginModelChange(u32 argumentsAddress) {
    BtlModelChangeArgs *arguments = (BtlModelChangeArgs *)argumentsAddress;
    BtlUnit *unit = arguments->unit;
    u32 model = arguments->resourceKind;
    u32 variant = arguments->resourceId;
    s32 status = btlHasMatchingModel(model, variant);

    if (status == 0) {
        btlRequestModelAssetByMode((u32)unit, model, variant);
        unit->gunResourceFlags = (unit->gunResourceFlags | 1) & ~2;
        btlBossDebugPrintf("btl:model change start[%X,%X]\n", model, variant);
    }
}

extern void func_001E1B80(BtlUnit *, BtlUnit *);

extern void btlDestroyUnit(BtlUnit *);

/* Complete model loading, cross-fade the retained actor, and release it. */
u32 func_001E50E0(BtlModelChangeArgs *args) {
    BtlUnit *unit = args->unit;
    u32 resourceKind = args->resourceKind;
    u32 resourceId = args->resourceId;
    u32 packedStart[4];
    u32 packedEnd[4];
    u32 alpha;
    u32 color;
    s32 entryFlags;

    switch (args->phase) {
    case 0:
        if (args->delay > args->elapsed) {
            break;
        }
        if (!btlCheckModelAssetByMode((u8 *)unit, resourceKind, resourceId)) {
            break;
        }
        if (args->duration != 0) {
            unit->mirror = btlCreateUnit();
            unit->mirror->status.flags |= 0x40000;
            func_001E1B80(unit->mirror, unit);
            if (unit->unkCC == 0) {
                unit->mirror->unkCC = 0;
                unit->unkCC = 1;
            }
            btlSetUnitPosition(unit->mirror, unit->currentPosition);
            btlSetUnitRotation(unit->mirror, (s128 *)unit->orientation);
            if (!(unit->status.stateFlags & 0x100000)) {
                btlSetUnitColor(unit->mirror, unit->baseColor, 0);
            } else {
                color = mdlGetBroadcastValue(unit->mirror->ext->owner);
                btlSetUnitColor(unit->mirror, (color & 0xFFFFFF) | 0x80000000, 0);
            }
            unit->mirror->status.flags |= 8;
            args->phase = 1;
            args->elapsed = 0;
        } else {
            args->phase = 2;
        }
        btlReleaseActorModelResources(unit);
        btlRefreshUnitMaximumHpAndClampCurrentHp(&unit->partyRecord);
        btlRefreshUnitMaximumMpAndClampCurrentMp(&unit->partyRecord);
        btlBindUnitModel((u8 *)unit, resourceKind, resourceId);
        btlReleaseModelAssetByMode((u32)unit, resourceKind, resourceId);
        if (args->duration == 0) {
            kwlnDrawControlFlags |= 0x2000000;
        }
        btlSetUnitPosition(unit, unit->currentPosition);
        btlSetUnitRotation(unit, (s128 *)unit->orientation);
        btlSetUnitColor(unit, unit->baseColor, 0);
        if (unit->status.stateFlags & 0x10) {
            u32 firstColor;
            u32 secondColor;

            VU0_LOAD_VF(vf10, unit->colorStart);
            EE_MMI_RGBA_PACK_UNIT(packedStart[0], 128.0f);
            firstColor = packedStart[0];
            VU0_LOAD_VF(vf10, unit->colorEnd);
            EE_MMI_RGBA_PACK_UNIT(packedEnd[0], 128.0f);
            secondColor = packedEnd[0];
            evtInitializeUnitColorTransition(unit->ext, 0, firstColor, secondColor);
        }
        unit->gunResourceFlags = (unit->gunResourceFlags & ~1) | 2;
        if (args->phase == 1) {
            u32 baseRgb = unit->baseColor & 0xFFFFFF;

            unit->overlayColor = baseRgb;
            unit->mirror->overlayColor = baseRgb | 0x80000000;
            evtSetUnitAlphaTransition(unit->ext, 0, 0);
        }
        break;
    case 1:
        if (args->elapsed == 1 && args->duration != 0) {
            entryFlags = btlGetEntryFlagsUnlessDisabled(&unit->partyRecord);
            if (unit->status.flags & 0x20) {
                btlApplyScaledUnitEffectParameter(unit, 0xB,
                    btlGetSlotRateKind((u8 *)unit, 0xB), 1.0f);
            } else if (args->transitionMode == 2) {
                unit->unkEC = -1;
                btlApplyScaledUnitEffectParameter(unit, 0xE,
                    btlGetSlotRateKind((u8 *)unit, 0xE), 1.0f);
            } else if (args->transitionMode != 3 &&
                       ((unit->status.flags & 0x200) || (entryFlags & 0x200)) &&
                       args->resourceId != 0x1F) {
                unit->unkEC = -1;
                if (unit->status.flags & 0x1000) {
                    btlApplyScaledUnitEffectParameter(unit, 0x10,
                        btlGetSlotRateKind((u8 *)unit, 0x10), 1.0f);
                } else {
                    btlApplyScaledUnitEffectParameter(unit, 0x11,
                        btlGetSlotRateKind((u8 *)unit, 0x11), 1.0f);
                }
            }
        }
        if ((u32)args->elapsed < args->duration) {
            if (args->transitionMode == 0) {
                alpha = (u32)((f32)args->elapsed / (f32)args->duration * 128.0f);
                color = unit->baseColor & 0xFFFFFF;
                unit->overlayColor = (alpha << 24) | color;
                if (!(unit->status.stateFlags & 0x100000)) {
                    unit->mirror->overlayColor = ((128 - alpha) << 24) | color;
                } else {
                    unit->mirror->overlayColor = ((128 - alpha) << 24) |
                        (mdlGetBroadcastValue(unit->mirror->ext->owner) & 0xFFFFFF);
                }
                unit->status.flags |= 0x10000;
            } else {
                if (args->elapsed == 1) {
                    btlFlagUnitDefeatCandidate(unit->mirror);
                    evtSetUnitRgbTransition(unit->mirror->ext, args->duration >> 2, 0x80000000);
                } else if ((u32)args->elapsed == (args->duration >> 2)) {
                    evtSetUnitAlphaTransition(unit->mirror->ext, args->elapsed, 0);
                    unit->mirror->status.flags |= 0x200000;
                    btlFlagUnitDefeatCandidate(unit);
                    alpha = unit->baseColor & 0xFF000000;
                    evtSetUnitRgbTransition(unit->ext, 0, 0);
                    evtSetUnitAlphaTransition(unit->ext, args->duration >> 2, alpha);
                    unit->status.flags |= 0x100000;
                } else if ((u32)args->elapsed == (args->duration >> 1)) {
                    unit->overlayColor = unit->baseColor;
                    evtSetUnitRgbTransition(unit->ext, args->duration >> 1, unit->baseColor);
                }
            }
        } else {
            if (args->transitionMode == 0) {
                unit->status.flags &= ~0x10000;
            }
            unit->overlayColor = unit->baseColor;
            unit->mirror->overlayColor = 0;
            args->phase = 2;
        }
        break;
    case 2:
        if (unit->mirror != NULL) {
            btlDestroyUnit(unit->mirror);
            unit->mirror = NULL;
            if (unit->unk350 != 0) {
                sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)unit->unk350);
                unit->unk350 = 0;
                unit->unk34C = 0;
            }
        }
        btlBossDebugPrintf("btl:model change end[%X,%X]\n", resourceKind, resourceId);
        return 1;
    }
    args->elapsed++;
    return 0;
}

extern u32 func_001E50E0(BtlModelChangeArgs *);

BtlRuntimeTask *btlCreateModelChangeTask(BtlUnit *unit, s32 option, s32 value08, s32 value0C, s32 value10, u8 flag19) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(BtlModelChangeArgs));
    BtlModelChangeArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x1A;
    task->flags &= ~BTL_TASK_FLAG_REGISTERED;
    task->ownerId = unit->owner;
    task->onStart = btlBeginModelChange;
    task->callback = func_001E50E0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    args->resourceKind = option;
    args->resourceId = value08;
    args->delay = value0C;
    args->duration = value10;
    args->transitionMode = flag19;
    args->phase = 0;
    args->elapsed = 0;
    return task;
}

void btlApplyLinkedUnitStatusWhenActorActive(s32 taskArgs) {
    if ((((BtlUnit *)((SoundTaskArgs *)taskArgs)->unk_0C)->status.flags & 2) != 0) {
        evtSetUnitStatusFlags(((BtlUnit *)((SoundTaskArgs *)taskArgs)->unk_0C)->ext);
        return;
    }
}

u32 btlApplyUnitFxWhenLoaded(u32 *taskArgs) {
    if ((btlUnitStatusPair((BtlUnit *)taskArgs[3]) & 0x1000000002) == 0x1000000002) {
        evtInitializeUnitColorTransition(((BtlUnit *)taskArgs[3])->ext, taskArgs[2], *taskArgs, taskArgs[1]);
    }
    return 1;
}

BtlRuntimeTask *btlCreateUnitTask0F(BtlUnit *unit, s32 value, s32 option, s32 value08) {
    BtlRuntimeTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0xF;
    task->ownerId = unit->owner;
    task->onStart = btlApplyLinkedUnitStatusWhenActorActive;
    task->callback = btlApplyUnitFxWhenLoaded;
    args = btlGetTaskArguments(task);
    args->unk_0C = (u32)unit;
    args->value = value;
    args->option = option;
    args->unk_08 = value08;
    return task;
}

void btlPrepareUnitStatusFxOnStart(s32 taskArgs) {
    if ((((FxTask *)taskArgs)->unit->status.flags & 2) != 0) {
        evtSetUnitStatusFlags(((FxTask *)taskArgs)->unit->ext);
        return;
    }
}

s32 btlApplyUnitVectorFxWhenLoaded(FxTask *task) {
    BtlUnit *unit = task->unit;
    if (unit->status.flags & 2) {
        VU0_LOAD_VF_MEMORY(vf10, task);
        evtSetUnitNormalizedDirection(unit->ext, task->unk10);
    }
    return 1;
}

BtlRuntimeTask *btlCreateUnitTask10(BtlUnit *unit, f32 *vec, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(0x18);
    FxTask *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x10;
    task->ownerId = unit->owner;
    task->onStart = btlPrepareUnitStatusFxOnStart;
    task->callback = btlApplyUnitVectorFxWhenLoaded;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    args->unk10 = option;
    PCP_COPY_VECTOR(args, vec);
    return task;
}

typedef struct BtlFadeArgs {
    BtlUnit *unit;
    s32 fadeIn;
    s32 fadeOut;
    u32 count;
    u32 color;
} BtlFadeArgs;

s32 btlUnitFadeInTask(BtlFadeArgs *args) {
    u32 total = args->fadeIn + args->fadeOut;
    BtlUnit *unit = args->unit;
    if (total != 0) {
        if (args->count == 0) {
            args->color = (unit->overlayColor & 0xFFFFFF) | 0x80000000;
            btlFlagUnitDefeatCandidate(unit);
            mdlBroadcastMasked(unit->ext->owner, 0);
            evtSetUnitRgbTransition(unit->ext, 0, 0);
            evtSetUnitAlphaTransition(unit->ext, 0, 0);
            evtSetUnitAlphaTransition(unit->ext, args->fadeIn, args->color);
            unit->status.flags |= 0x100000;
        }
        if (args->count == args->fadeIn - 1) {
            unit->overlayColor = args->color;
            evtSetUnitRgbTransition(unit->ext, args->fadeOut, args->color);
        }
    } else {
        unit->overlayColor = args->color;
    }
    if (!(unit->status.flags & 0x100000)) {
        if (args->count >= total) {
            evtSetUnitRgbTransition(unit->ext, 0, args->color);
            evtSetUnitAlphaTransition(unit->ext, 0, args->color);
            return 1;
        }
    }
    args->count++;
    return 0;
}

BtlRuntimeTask *btlCreateUnitFadeInTask(BtlUnit *unit, u32 value, u32 variant) {
    BtlRuntimeTask *task = btlAllocTask(20);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlUnitFadeInTask;
    task->taskId = 0x11;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    args->unk_10 = 0x80808080;
    args->option = value;
    args->unk_08 = variant;
    args->unk_0C = 0;
    return task;
}

s32 btlUnitFadeOutTask(BtlFadeArgs *args) {
    u32 total = args->fadeIn + args->fadeOut;
    BtlUnit *unit = args->unit;
    if (total != 0) {
        if (args->count == 0) {
            btlFlagUnitDefeatCandidate(unit);
            evtSetUnitRgbTransition(unit->ext, args->fadeOut, 0x80000000);
            unit->status.flags |= 0x200000;
        }
        if (args->count == args->fadeOut - 1) {
            unit->overlayColor = 0x80000000;
            evtSetUnitRgbTransition(unit->ext, 0, 0);
            evtSetUnitAlphaTransition(unit->ext, args->fadeIn, 0);
        }
    } else {
        unit->overlayColor = 0x80000000;
    }
    if (!(unit->status.flags & 0x200000)) {
        if (args->count >= total) {
            return 1;
        }
    }
    args->count++;
    return 0;
}

BtlRuntimeTask *btlCreateUnitFadeOutTask(BtlUnit *unit, u32 value, u32 variant) {
    BtlRuntimeTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlUnitFadeOutTask;
    task->taskId = 0x12;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    args->option = value;
    args->unk_08 = variant;
    args->unk_0C = 0;
    return task;
}

s32 btlStepUnitDefeatFadeIn(u32 *arguments) {
    BtlUnit *unit = (BtlUnit *)arguments[0];
    u32 alpha;

    if ((s32)arguments[1] == -1) {
        unit->overlayColor = (unit->baseColor & 0xFFFFFF) | 0x80000000;
        evtSetUnitRgbTransition(unit->ext, 0, unit->overlayColor);
        evtSetUnitAlphaTransition(unit->ext, 0, unit->overlayColor);
        btlClearUnitDefeatCandidate(unit);
        return 1;
    }
    if (arguments[2] == 0) {
        btlFlagUnitDefeatCandidate(unit);
    }
    if (arguments[1] == 0) {
        evtSetUnitRgbTransition(unit->ext, 0, unit->overlayColor);
        evtSetUnitAlphaTransition(unit->ext, 0,
                                  (unit->overlayColor & 0xFFFFFF) | 0x80000000);
    }
    if ((s32)arguments[2] >= (s32)arguments[1]) {
        unit->status.flags &= ~0x20000;
        unit->overlayColor = (unit->baseColor & 0xFFFFFF) | 0x80000000;
        return 1;
    }
    unit->status.flags |= 0x20000;
    alpha = (u32)((f32)(s32)arguments[2] * 128.0f / (f32)(s32)arguments[1]);
    alpha <<= 24;
    unit->overlayColor = alpha | (unit->baseColor & 0xFFFFFF);
    arguments[2]++;
    return 0;
}

BtlRuntimeTask *func_001E5FF8(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    SoundTaskArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x13;
    task->ownerId = actor->owner;
    task->callback = btlStepUnitDefeatFadeIn;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = option;
    args->unk_08 = 0;
    return task;
}

s32 btlStepUnitDefeatFadeOut(u32 *arguments) {
    BtlUnit *unit = (BtlUnit *)arguments[0];
    u32 alpha;

    if (arguments[2] == 0) {
        btlFlagUnitDefeatCandidate(unit);
    }
    if ((s32)arguments[2] >= (s32)arguments[1]) {
        unit->status.flags &= ~0x20000;
        unit->overlayColor = unit->baseColor & 0xFFFFFF;
        return 1;
    }
    unit->status.flags |= 0x20000;
    alpha = (u32)((1.0f - (f32)(s32)arguments[2] / (f32)(s32)arguments[1]) * 128.0f);
    alpha <<= 24;
    unit->overlayColor = alpha | (unit->baseColor & 0xFFFFFF);
    arguments[2]++;
    return 0;
}

BtlRuntimeTask *func_001E61A0(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    SoundTaskArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x14;
    task->ownerId = actor->owner;
    task->callback = btlStepUnitDefeatFadeOut;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = option;
    args->unk_08 = 0;
    return task;
}

u32 func_001E6228(u32 *arguments) {
    BtlUnit *unit = (BtlUnit *)arguments[0];
    s32 finished = 0;
    u32 alpha;

    if (arguments[2] == 0) {
        unit->status.flags |= 0x80;
        btlFlagUnitDefeatCandidate(unit);
    }

    switch (arguments[1]) {
    case 0:
        if (arguments[2] == 1) {
            btlFlagUnitDefeatCandidate(unit);
            evtSetUnitRgbTransition(unit->ext, 10, 0x80000000);
        } else if (arguments[2] == 10) {
            evtSetUnitRgbTransition(unit->ext, 0, 0x80000000);
            evtSetUnitAlphaTransition(unit->ext, 6, 0);
            unit->status.flags |= 0x200000;
        }
        if ((unit->status.flags & 0x200000) == 0 && (s32)arguments[2] >= 16) {
            finished = 1;
        }
        break;

    case 1:
        if ((s32)arguments[2] >= 8) {
            unit->status.flags &= ~0x20000;
            unit->overlayColor = unit->baseColor & 0xFFFFFF;
            finished = 1;
        } else {
            unit->status.flags |= 0x20000;
            alpha = (u32)((1.0f - (f32)(s32)arguments[2] * 0.125f) * 128.0f);
            alpha <<= 24;
            unit->overlayColor = alpha | (unit->baseColor & 0xFFFFFF);
        }
        break;
    }

    arguments[2]++;
    if (finished != 0) {
        unit->status.flags = (unit->status.flags & ~0x80) | 0x40;
        return 1;
    }
    return 0;
}

BtlRuntimeTask *func_001E6428(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    SoundTaskArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x15;
    task->ownerId = actor->owner;
    task->callback = func_001E6228;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = option;
    args->unk_08 = 0;
    return task;
}

typedef struct UnitEffectTaskArgs {
    BtlUnit *unit;
    SoundMixer *mixer;
    BattleEffect *effect;
    s32 duration;
    s32 counter;
} UnitEffectTaskArgs;

extern void func_00168978(BattleEffect *);

/* Start from the selected-unit SYSEFF source, then update through its duration.
 * Return one for an ineligible unit or expiry, zero while updating. */
s32 btlUpdateSelectedUnitEffect(UnitEffectTaskArgs *args) {
    BtlUnit *unit = args->unit;
    if (!(unit->status.flags & 2)) {
        return 1;
    }
    {
        BtlState *battle = (BtlState *)btlGetRuntime();
        if (args->effect == 0) {
            void *handle = battle->resources[BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT]->sourceHandle;
            unit->status.flags |= 0x80;
            args->mixer = sndMixerClone(handle);
            args->effect = func_00168548(args->mixer, 2, unit, 0);
            args->duration = 0xE;
            args->effect->flags &= 0xFFF9;
            effBattleUpdateSelectedValue(args->effect, 0xE);
            unit->status.flags &= ~8;
            if (unit->status.flags & 2) {
                unit->ext->owner->flags |= MDL_SKIP_TRANSFORMS;
            }
        }
        args->counter = args->counter + 1;
        if (args->counter >= args->duration) {
            unit->status.flags = (unit->status.flags & ~0x80) | 0x40;
            return 1;
        }
        func_00168978(args->effect);
        return 0;
    }
}

void btlFinishSelectedUnitEffect(UnitEffectTaskArgs *arguments) {
    BattleEffect *voice = arguments->effect;
    if (voice != 0) {
        effReleaseBattleVoiceOwner(voice);
    }
    if (arguments->mixer != 0) {
        sndReleaseAllVoices(arguments->mixer);
    }
    btlClearUnitDefeatCandidate(arguments->unit);
    arguments->unit->status.flags |= 0x40;
}

BtlRuntimeTask *btlCreateSelectedEffectUpdateTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(0x14);
    UnitEffectTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x16;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->ownerId = unit->owner;
    task->callback = btlUpdateSelectedUnitEffect;
    task->onFinish = btlFinishSelectedUnitEffect;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    args->effect = 0;
    args->counter = 0;
    args->duration = 0;
    return task;
}

u32 btlUpdateCommandSoundTask(void) {
    btlUpdateUnitActors();
    return 1;
}

BtlRuntimeTask *btlCreateCommandSoundUpdateTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlUpdateCommandSoundTask;
    task->taskId = 0x1B;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

u32 btlUpdateCommandSoundTaskSecondary(void) {
    func_00209078();
    return 1;
}

BtlRuntimeTask *btlCreateSecondaryCommandSoundTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x1C;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->onStart = 0;
    task->callback = btlUpdateCommandSoundTaskSecondary;
    return task;
}

u32 func_001E6790(void) {
    return 1;
}

BtlRuntimeTask *func_001E6798(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = func_001E6790;
    task->taskId = 0x20;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

typedef struct BtlUnitBaseLightArgs {
    BtlUnit *unit;
    s32 delay;
} BtlUnitBaseLightArgs;

/* vu0 routine: SDK quadword copies restore source colours and light direction. */
u32 btlUnitBaseLightTask(BtlUnitBaseLightArgs *work) {
    BtlUnit *unit = work->unit;
    EvtUnit *ext;
    EvtTargetInfo *info;
    EffWorldNode *target;

    if (unit->status.flags & 2) {
        if (work->delay >= 2) {
            ext = unit->ext;
            evtSetUnitStatusFlags(ext);
            target = (EffWorldNode *)ext->currentTransitionValue;
            if (target != NULL && (ext->flags & 0x40000)) {
                info = target->data;
                PCP_COPY_VECTOR(unit->colorStart, info->firstColor);
                PCP_COPY_VECTOR(unit->colorEnd, info->secondColor);
                PCP_COPY_VECTOR(unit->lightDirection, info->direction);
            } else {
                f32 *defaultLight = D_0037F770[0];
                PCP_COPY_VECTOR(unit->colorStart, defaultLight);
                PCP_COPY_VECTOR(unit->colorEnd, kwlnDefaultColorVector);
                PCP_COPY_VECTOR(unit->lightDirection, defaultLight + 4);
                btlBossDebugPrintf("btl:base light error[%p]\n", unit);
            }
            unit->status.stateFlags |= 0x10;
            btlBossDebugPrintf("btl:base light set[%p]\n", unit);
            return 1;
        }
        work->delay++;
    }
    return 0;
}

BtlRuntimeTask *btlCreateUnitBaseLightTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(8);
    BtlUnitBaseLightArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlUnitBaseLightTask;
    task->taskId = 0x21;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    args->delay = 0;
    return task;
}

typedef struct BtlStiffenTaskArgs {
    BtlUnit *unit;
    f32 scale;
    s32 count;
} BtlStiffenTaskArgs;

u32 btlStiffenDamageShakeStep(BtlStiffenTaskArgs *args) {
    f32 pos[4];
    f32 scale;
    s32 node;

    if (!(args->unit->status.flags & 2)) {
        return 1;
    }
    if (args->unit->status.stateFlags & 0x200000) {
        return 1;
    }
    if (args->count == 0) {
        node = mdlGetNodeMotionIndex(args->unit->ext->owner, 0);
        if (node < 0x1D) {
            u8 *resource = (u8 *)btlGetSideIndexedActorStatusTable(args->unit->resourceKind, args->unit->resourceIndex);
            if (((BtlEffectResource *)resource)->nodes[node].rateKind == 2) {
                btlRefreshUnitEffectMotionAndEntry(args->unit);
                btlBossDebugPrintf("btl:stiffen damage motion wait\n");
            }
        }
    }
    if (0.5f < args->scale) {
        BtlUnit *actor;
        scale = args->scale * (effMiscRandUnitFloat(effSharedRandomState) * 0.5f + 0.5f);
        if (args->count & 1) {
            scale = -scale;
        }
        actor = args->unit;
        if (btlUnitStatusPair(actor) & 0x808000000000) {
            effObjFetchInnerPosition(actor->effectObject);
            VU0_STORE_VF(vf10, pos);
            pos[0] += scale;
        } else {
            func_001E3108(actor, pos);
            pos[0] += scale;
            pos[2] += args->unit->positionZOffset;
        }
        effObjSetInnerPosition(args->unit->effectObject, (u128 *)pos);
        args->scale *= 0.85f;
    } else {
        BtlUnit *actor = args->unit;
        if (btlUnitStatusPair(actor) & 0x808000000000) {
            effObjFetchInnerPosition(actor->effectObject);
            VU0_STORE_VF(vf10, pos);
        } else {
            func_001E3108(actor, pos);
            pos[2] += args->unit->positionZOffset;
        }
        effObjSetInnerPosition(args->unit->effectObject, (u128 *)pos);
        return 1;
    }
    args->count += 1;
    return 0;
}

BtlRuntimeTask *btlCreateStiffenDamageShakeTask(BtlUnit *unit, f32 value) {
    BtlRuntimeTask *task = btlAllocTask(12);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x1D;
    task->callback = btlStiffenDamageShakeStep;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    args->scale = value;
    args->unk_08 = 0;
    return task;
}

typedef struct BtlPositionEffectArgs {
    BtlUnit *unit;
    s32 tick;
    f32 amount;
    f32 velocity;
} BtlPositionEffectArgs;

u32 btlAdvanceActorPositionEffectTask(BtlPositionEffectArgs *task) {
    BtlState *runtime = (BtlState *)btlGetRuntime();
    BtlUnit *unit = task->unit;
    s32 flags;
    s32 approved;
    s32 parameter;
    f32 amplitude;
    f32 offset;
    f32 velocity;
    f32 delta;
    f32 position[4];

    if (!(unit->status.flags & 2)) {
        return 1;
    }
    flags = btlGetEntryFlagsUnlessDisabled(&unit->partyRecord);
    if ((unit->status.flags & 0x200) || (flags & 0x200)) {
        approved = 1;
        if (runtime->unitLiftPredicate != NULL) {
            approved = runtime->unitLiftPredicate(unit);
        }
        if (approved) {
            parameter = 13;
            if (runtime->chooseMotion != NULL) {
                parameter = runtime->chooseMotion(unit, 13, 0);
            }
            if (parameter != -1) {
                btlApplyScaledUnitEffectParameter(unit, parameter, 0, 1.0f);
                return 1;
            }
        }
    }
    amplitude = unit->unkBC * unit->scale * 0.8f;
    if (amplitude > 100.0f) {
        amplitude = 100.0f;
    }
    if (task->tick == 0) {
        task->amount = 0.0f;
        task->velocity = 0.3f;
    }
    velocity = task->velocity;
    if (velocity >= 0.0f) {
        offset = amplitude * task->amount;
        task->velocity = velocity + 0.02f;
        delta = (1.0f - task->amount) * velocity;
        task->amount += delta;
        if (task->amount >= 0.99f) {
            task->velocity = -0.17999998f;
        }
    } else {
        offset = amplitude * task->amount;
        task->velocity = velocity - 0.01f;
        delta = task->amount * -velocity;
        task->amount -= delta;
        if (task->amount <= 0.01f) {
            func_001E3108((u8 *)task->unit, position);
            position[2] += task->unit->positionZOffset;
            effObjSetInnerPosition(task->unit->effectObject, (u128 *)position);
            return 1;
        }
    }
    func_001E3108((u8 *)task->unit, position);
    position[0] += offset;
    position[2] += task->unit->positionZOffset;
    effObjSetInnerPosition(task->unit->effectObject, (u128 *)position);
    task->tick++;
    return 0;
}

BtlRuntimeTask *func_001E6E18(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(16);
    BtlPositionEffectArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlAdvanceActorPositionEffectTask;
    task->taskId = 0x1E;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    args->tick = 0;
    return task;
}

u32 func_001E6E90(s32 *taskArgs) {
    s32 unit;

    unit = *taskArgs;
    ((BtlUnit *)unit)->status.flags = ((BtlUnit *)unit)->status.flags & 0xffffffef;
    btlSetUnitRotation(unit, unit + 0x40);
    return 1;
}

BtlRuntimeTask *btlScheduleActorUpdate(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = func_001E6E90;
    task->taskId = 0x1F;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    return task;
}

u32 btlRefreshUnitFxVectorTask(u32 *taskArgs) {
    btlRefreshUnitFxVectors(*taskArgs);
    return 1;
}

BtlRuntimeTask *btlCreateUnitFxVectorRefreshTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlRefreshUnitFxVectorTask;
    task->taskId = 0x22;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    return task;
}

extern s32 btlFormatUnitBedName(BtlUnit *, char *);

void btlStartGunFinishLoad(s32 *task) {
    char filename[0x70];
    BtlUnit *unit = *(BtlUnit **)task;
    if (unit->status.flags & 0x400) {
        return;
    }
    if (unit->gunResource != 0) {
        sdfFreeMemoryFromEitherHeap(unit->gunResource);
        unit->gunResource = 0;
    }
    if (btlFormatUnitBedName(unit, filename)) {
        s32 handle = (s32)fileQueueAlternateCallbackRequest(filename);
        task[1] = handle;
        btlBossDebugPrintf("btl:gun & finish load start[%s][%p]\n", filename, handle);
    }
    unit->gunResourceFlags = (unit->gunResourceFlags | 4) & ~8;
}

typedef struct GunLoadArgs {
    BtlUnit *unit;
    s32 handle;
} GunLoadArgs;

u32 btlPollGunLoad(s32 arg) {
    GunLoadArgs *args = (GunLoadArgs *)arg;
    BtlUnit *unit = args->unit;
    if (args->handle == 0) {
        return 1;
    }
    if (fileIsRequestReadyInCurrentMode((struct FileRequest *)args->handle) == 0) {
        return 0;
    }
    btlBossDebugPrintf("btl:gun & finish load end[%p]\n", args->handle);
    unit->gunResource = (void *)sdfResourceRetainAddress((struct SdfMemBlock *)(fileGetResourceHandle((struct FileRequest *)args->handle)));
    filePollEntryCleanup((struct FileRequest *)(u32)args->handle);
    unit->gunResourceFlags = (unit->gunResourceFlags & ~4) | 8;
    return 1;
}

BtlRuntimeTask *btlCreateGunLoadPollTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x23;
    task->flags &= ~BTL_TASK_FLAG_REGISTERED;
    task->ownerId = unit->owner;
    task->onStart = btlStartGunFinishLoad;
    task->callback = btlPollGunLoad;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    args->option = 0;
    return task;
}

u32 btlUpdateUnitEffectsTask(void) {
    btlUpdateUnitEffects();
    return 1;
}

BtlRuntimeTask *btlCreateUpdateUnitEffectsTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlUpdateUnitEffectsTask;
    task->taskId = 0x24;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

typedef struct BtlActorTransparencyArgs {
    BtlUnit *unit;
} BtlActorTransparencyArgs;

u32 btlCreateActorTransparency(BtlActorTransparencyArgs *args) {
    BtlUnit *unit = args->unit;
    if (!(unit->status.flags & 2)) {
        return 0;
    }
    btlCreateUnitTransparency(unit);
    args->unit->status.flags |= 0x20000;
    return 1;
}

BtlRuntimeTask *btlCreateActorTransparencyTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    BtlActorTransparencyArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x25;
    task->callback = btlCreateActorTransparency;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    return task;
}


s32 func_001E72B0(BtlActorModelBlendArgs *args) {
    BtlUnit *unit;
    BtlUnit *target;

    if (args->index < 0) {
        return 1;
    }
    unit = args->unit;
    target = args->target;
    if (args->stage == 0) {
        if (unit->status.flags & 2) {
            args->previousModelValue = mdlGetNodeMotionIndex(unit->ext->owner, 0);
        } else {
            args->previousModelValue = unit->unkEC;
        }
        unit->unkF8 = 0;
        unit->unkFA = 1;
        btlApplyScaledUnitEffectParameter(unit, args->index, args->value, args->scale);
    } else {
        if (unit != target) {
            func_001E40F0(unit, target, args->previousModelValue);
        }
        return 1;
    }
    args->stage++;
    return 0;
}

BtlRuntimeTask *btlCreateActorModelBlendTask(BtlUnit *unit, BtlUnit *target, s32 index, s32 value, f32 scale) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(BtlActorModelBlendArgs));
    BtlActorModelBlendArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = func_001E72B0;
    task->taskId = 0x26;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    args->target = target;
    args->index = index;
    args->value = value;
    args->scale = scale;
    args->previousModelValue = -1;
    args->stage = 0;
    return task;
}

typedef struct BtlFaceBodyTaskArgs {
    BtlUnit *actor;
    BtlUnit *target;
} BtlFaceBodyTaskArgs;

u32 btlRotateUnitTowardOtherBody(BtlFaceBodyTaskArgs *taskArgs) {
    s128 hit[1];
    s128 from;
    s128 to;
    btlUnitGetBodyPosVU(taskArgs->actor);
    VU0_STORE_VF_UNCLOBBERED(vf10, &from);
    btlUnitGetBodyPosVU(taskArgs->target);
    VU0_STORE_VF_UNCLOBBERED(vf10, &to);
    if (btlAimHorizontalDirectionVU((f32 *)&from, (f32 *)&to) != 0) {
        VU0_STORE_VF_UNCLOBBERED(vf10, hit);
        btlSetUnitRotation(taskArgs->actor, hit);
    }
    return 1;
}

BtlRuntimeTask *btlCreateUnitFaceBodyTask(BtlUnit *actor, BtlUnit *target) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(BtlFaceBodyTaskArgs));
    BtlFaceBodyTaskArgs *args;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x27;
    task->ownerId = actor->owner;
    task->callback = btlRotateUnitTowardOtherBody;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->target = target;
    return task;
}

u32 btlFlagDefeatCandidateTask(u32 *taskArgs) {
    btlFlagUnitDefeatCandidate((BtlUnit *)*taskArgs);
    return 1;
}

BtlRuntimeTask *btlCreateDefeatCandidateTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlFlagDefeatCandidateTask;
    task->taskId = 0x28;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    return task;
}

u32 btlClearDefeatCandidateTask(u32 *taskArgs) {
    btlClearUnitDefeatCandidate(*taskArgs);
    return 1;
}

BtlRuntimeTask *btlCreateDefeatCandidateClearTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlClearDefeatCandidateTask;
    task->taskId = 0x29;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    return task;
}

/* Update motion completion, alpha transitions, and the selected-unit color pulse. */
void btlUpdateActiveActorModelState(void) {
    BtlState *runtime = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    BtlActorStatusRecord *status;
    s32 parameter;
    s32 frame;
    s32 index;
    f32 pulse;
    u32 packed[4];
    /* Three boss installers publish no-argument hooks in this opaque slot. */
    void (*beforeMotionUpdate)(void) = *(void (**)(void))runtime->pad610;

    if (beforeMotionUpdate != NULL) {
        beforeMotionUpdate();
    }
    for (unit = runtime->units; unit != NULL; unit = unit->nextActor) {
        if (!(unit->status.flags & 0x600) || !(unit->status.flags & 2)) {
            continue;
        }
        status = (BtlActorStatusRecord *)btlGetSideIndexedActorStatusTable(
            unit->resourceKind, unit->resourceIndex);
        if (unit->status.flags & 0x40000000) {
            btlSeekRandomModelFrame(unit);
            unit->status.flags &= ~0x40000000;
        }
        if (unit->status.flags & 0x80000000) {
            parameter = 1;
            if (runtime->chooseMotion != NULL) {
                parameter = runtime->chooseMotion(unit, 1, 0);
            }
            if (unit->unkEC != parameter && parameter != -1) {
                btlApplyScaledUnitEffectParameter(unit, parameter, 0, 1.0f);
            } else {
                frame = (s32)btlGetUnitModelValue1C(unit);
                if (frame >= status->model) {
                    sdfMotionSampleAtFrame(unit->ext->owner->first, (f32)status->model);
                    btlResetUnitModelProgress(unit);
                    unit->status.flags &= ~0x80000000;
                }
            }
        }
        if (unit->updateFlags & 2) {
            if (unit->updateFlags & 4) {
                frame = (s32)btlGetUnitModelValue1C(unit);
                index = unit->unkEC;
                if (index == mdlGetNodeMotionIndex(unit->ext->owner, 0)) {
                    if (frame >= status->motions[index].alphaStartFrame) {
                        evtSetUnitAlphaTransition(unit->ext,
                            (s32)((f32)status->motions[index].alphaDuration /
                                (status->motions[index].alphaFrameScale * runtime->unk4C8)),
                            unit->overlayColor & 0xFFFFFF);
                        unit->updateFlags &= ~4;
                    }
                }
            }
            unit->overlayColor = mdlGetBroadcastValue(unit->ext->owner);
        }
        if (unit->status.flags & 0x8000) {
            /* The pulse uses the signed global counter at +0x214, not scene frame +0x234. */
            pulse = (f32)(*(s32 *)runtime->pad214 % 30) / 15.0f;
            if (pulse > 1.0f) {
                pulse = 2.0f - pulse;
            }
            VU0_SET_ONES_XYZ(vf10);
            VU0_SCALAR_OP(pulse * 1.6f + 0.3f, "vmulx.xyzw vf10, vf10, vf2x");
            EE_MMI_RGBA_PACK(packed[0]);
            btlBlendUnitColor(unit, (packed[0] & 0xFFFFFF) | 0x80000000, 0);
        }
    }
}

extern void func_001E3E20(BtlUnit *);

extern void func_002034A8(struct SoundResourceLink *);

extern s32 btlGetSelectedUnitProperty(BtlUnit *);

extern void btlDrawActorGroundDisc(BtlUnit *);

void btlUpdateActorModelColorAndLinks(void) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit = work->units;
    s32 color;

    for (; unit != 0; unit = unit->nextActor) {
        if (unit->status.flags & 2) {
            MdlCtx *model = unit->ext->owner;
            if (!(unit->status.stateFlags & 0x20000)) {
                unit->unkEC = mdlGetNodeMotionIndex(model, 0);
            }
            if (!(unit->status.flags & 0x40000)) {
                if (unit->status.flags & 0x100000) {
                    color = mdlGetBroadcastValue(unit->ext->owner);
                    unit->overlayColor = color;
                    if ((color & 0xFF000000) == 0x80000000) {
                        btlFlagUnitDefeatCandidate(unit);
                        unit->status.flags &= ~0x100000;
                    }
                } else if (unit->status.flags & 0x200000) {
                    color = mdlGetBroadcastValue(unit->ext->owner);
                    unit->overlayColor = color;
                    if ((color & 0xFF000000) == 0) {
                        if (!(unit->status.flags & 0xC0)) {
                            btlClearUnitDefeatCandidate(unit);
                        }
                        unit->status.flags &= ~0x200000;
                    }
                }
                if (unit->status.flags & 0x10000) {
                    func_001E3E20(unit);
                } else {
                    btlUpdateUnitTransparency(unit);
                }
            }
            func_002034A8(unit->link31C);
            btlUpdateUnitCommandEffect(unit->link320);
            if (!(work->commandRestrictFlags & 0x10000)) {
                btlDrawActorGroundDisc(unit);
            }
        }
    }
}

void btlResetUnitLinks(BtlUnit *unit) {
    unit->selectedEntryIndex = -1;
    unit->unk314 = -1;
    unit->status.flags = 0;
    unit->status.stateFlags = 0;
    unit->gunResourceFlags = 0;
    unit->effectLink.flags = 0;
    btlClearAllActorEntrySlots(unit);
    unit->link31C = sndAllocResourceLink(unit);
    unit->link320 = sndAllocLink(unit);
}

extern u64 btlAdvanceRuntimeSequenceCounter(void);

extern void *memset(void *, s32, u32);

BtlUnit *btlCreateUnit(void) {
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(0x368);
    BtlUnit *unit = (BtlUnit *)sdfResourceRetainAddress(allocation);
    BtlState *work;
    memset(unit, 0, 0x368);
    unit->handle35C = (u32)allocation;
    unit->owner = btlAdvanceRuntimeSequenceCounter();
    unit->status.flags = 0;
    unit->status.stateFlags = 0;
    unit->lookupId = unit->selectedEntryIndex = -1;
    unit->unk2E4 = 6;
    unit->gunResourceFlags = 0;
    unit->effectLink.referenceCount = 0;
    unit->node318 = 0;
    unit->gunResource = 0;
    unit->effectObject = 0;
    unit->ext = 0;
    btlInitUnitFxDefaults(unit);
    btlInitFxLights(unit);
    btlResetUnitLinks(unit);
    work = (BtlState *)btlGetRuntime();
    unit->previousActor = 0;
    if (work->units != 0) {
        work->units->previousActor = unit;
        unit->nextActor = work->units;
    } else {
        unit->nextActor = 0;
    }
    work->units = unit;
    btlBossDebugPrintf("btl:unit create[%p]\n", unit);
    return unit;
}

extern void sndFreeResourceNode(struct SoundResourceNode *);

extern void btlReleaseActorModelResources(BtlUnit *);

extern void sndFreeListNode(struct ActiveSoundNode *);

void btlReleaseUnitResources(BtlUnit *unit) {
    btlBossDebugPrintf("btl:unit data free[%p]\n", unit);
    if (unit->node318 != 0) {
        sndFreeResourceNode(unit->node318);
        unit->node318 = 0;
    }
    if (unit->link31C != 0) {
        sndFreeResourceLink(unit->link31C);
        unit->link31C = 0;
    }
    if (unit->link320 != 0) {
        sndFreeLink(unit->link320);
        unit->link320 = 0;
    }
    if (unit->gunResource != 0) {
        sdfFreeMemoryFromEitherHeap(unit->gunResource);
        unit->gunResource = 0;
        unit->gunResourceFlags &= ~4;
        unit->gunResourceFlags &= ~8;
    }
    if (unit->node324 != 0) {
        sndFreeListNode(unit->node324);
        unit->node324 = 0;
    }
    btlReleaseActorModelResources(unit);
    if (unit->unk350 != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)unit->unk350);
        unit->unk350 = 0;
        unit->unk34C = 0;
    }
}

void btlDestroyUnit(BtlUnit *unit) {
    btlBossDebugPrintf("btl:unit delete[%p]\n", unit);
    btlReleaseUnitResources(unit);
    if (unit->nextActor != 0) {
        unit->nextActor->previousActor = unit->previousActor;
    }
    if (unit->previousActor != 0) {
        unit->previousActor->nextActor = unit->nextActor;
    } else {
        ((BtlState *)btlGetRuntime())->units = unit->nextActor;
    }
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(unit->handle35C));
}

void btlDestroyAllUnits(void) {
    BtlUnit *unit;
    BtlUnit *next;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = next) {
        next = unit->nextActor;
        btlDestroyUnit(unit);
    }
}

void btlRemoveActorsWithFlags(u32 mask) {
    s32 actor = (s32)((BtlState *)btlGetRuntime())->units;
    s32 next;
    while (actor != 0) {
        next = (s32)((BtlUnit *)actor)->nextActor;
        if (((BtlUnit *)actor)->status.flags & mask) {
            btlDestroyUnit(actor);
        }
        actor = next;
    }
}

BtlUnit *btlFindActorForOwner(u64 owner) {
    BtlUnit *unit;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->nextActor) {
        if (unit->owner == owner) {
            return unit;
        }
    }
    return 0;
}

s32 btlIsActiveActor(BtlUnit *actor) {
    BtlUnit *unit;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->nextActor) {
        if (unit == actor) {
            return 1;
        }
    }
    return 0;
}

BtlUnit *btlFindUnitByModeClear(s32 mode) {
    BtlUnit *unit;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->nextActor) {
        if (!(unit->partyRecord.flags & 0x20) && unit->partyRecord.unitId == mode) {
            return unit;
        }
    }
    return 0;
}

BtlUnit *btlFindUnitByModeFlagged(s32 mode) {
    BtlUnit *unit;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->nextActor) {
        if ((unit->partyRecord.flags & 0x20) && unit->partyRecord.unitId == mode) {
            return unit;
        }
    }
    return 0;
}

/* Allocate a native header followed by capacity pointer entries. */
BtlIndexList *btlAllocateIndexList(s32 capacity) {
    BtlIndexList *list = sdfAllocAndClearQuadwords(capacity * 4 + 12);
    list->capacity = capacity;
    list->entries = (void **)(list + 1);
    list->count = 0;
    return list;
}

void btlFreeIndexList(BtlIndexList *list) {
    sdfReleaseChipBlock(list);
}

/* The caller keeps the live count within the allocated capacity. */
void btlAppendIndexListEntry(BtlIndexList *list, void *entry) {
    s32 index;

    index = list->count;
    list->count = index + 1;
    list->entries[index] = entry;
}

/* Clear the live count without changing storage or its existing entries. */
void btlClearIndexList(BtlIndexList *list) {
    list->count = 0;
}

/* Return the number of live entries, not the allocated capacity. */
u32 btlGetIndexListCount(BtlIndexList *list) {
    return list->count;
}

/* The caller supplies an in-range index. */
void *btlGetIndexListEntry(BtlIndexList *list, s32 index) {
    return list->entries[index];
}

void btlCopyIndexList(BtlIndexList *destination, BtlIndexList *source) {
    u32 count;
    u32 index;

    btlClearIndexList(destination);
    count = btlGetIndexListCount(source);
    for (index = 0; index < count; index++) {
        btlAppendIndexListEntry(destination, btlGetIndexListEntry(source, index));
    }
}

/* Swap two entries without changing the live count. */
void btlSwapIndexListEntries(BtlIndexList *list, s32 firstIndex, s32 secondIndex) {
    void **entries;
    void *first;
    void *second;

    if (firstIndex == secondIndex) {
        return;
    }
    entries = list->entries;
    first = entries[firstIndex];
    second = entries[secondIndex];
    entries[firstIndex] = second;
    entries[secondIndex] = first;
}

u32 btlFindListIndex(BtlIndexList *list, void *entry) {
    u32 count = btlGetIndexListCount(list);
    u32 i;

    for (i = 0; i < count; i++) {
        if (entry == btlGetIndexListEntry(list, i)) {
            return i;
        }
    }
    return -1;
}

void btlApplyUnitEffectScale(BtlUnit *unit) {
    ObjectTransform *inner;
    if (unit->status.flags & 2) {
        btlInitializeEffectVectorsFromSourceRecords(unit, unit->resourceKind, unit->resourceIndex);
        VU0_SET_ONES_XYZ(vf10);
        VU0_SCALAR_OP(unit->unk50, "vmulx.xyzw vf10, vf10, vf2x");
        inner = unit->effectObject->inner;
        inner->flags |= OBJECT_TRANSFORM_FLAG_UPDATE_PENDING;
        inner->flags &= ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
        VU0_STORE_VF(vf10, inner->scale);
        mdlStoreTertiaryVectorVU(unit->ext->owner);
        mdlSetAmountOnAllContextResources(unit->ext->owner, unit->unk50);
        btlSetUnitPosition(unit, unit->currentPosition);
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E8258);

void btlNormalizeActionCameraKeyScales(BtlLinkedCommand *action) {
    f32 *key = (f32 *)action;
    u32 flags = action->flags;
    u32 i;
    if (flags & 2) {
        for (i = 0; i < 4; i++, key += 12) {
            f32 *pos = key + 12;
            VU0_LOAD_VF(vf10, key + 16);
            VU0_NEGATE_XYZ(vf10);
            VU0_LOAD_VF(vf11, pos);
            VU0_SCALAR_OP(key[20] - 1.0f, "vmulx.xyzw vf10, vf10, vf2x");
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, pos);
            key[20] = 1.0f;
        }
    } else if (flags & 4) {
        f32 *src = key + 12;
        f32 *dst = key + 24;
        for (i = 1; i < 4; i++, dst += 12) {
            f32 delta = dst[8] - src[8];
            dst[0] -= dst[4] * delta;
            dst[1] -= dst[5] * delta;
            dst[2] -= dst[6] * delta;
            dst[8] = src[8];
        }
    }
}

void btlInterpolateVectorStep(f32 *src) {
    f32 vec[4];
    f32 step = -src[8];
    vec[3] = 0.0f;
    vec[0] = src[4] * step + src[0];
    vec[1] = src[5] * step + src[1];
    vec[2] = src[6] * step + src[2];
    VU0_LOAD_VF_MEMORY(vf10, vec);
}

u32 func_001E8568(BtlLinkedCommand *command) {
    return 1;
}

u32 func_001E8570(BtlLinkedCommand *command) {
    return 1;
}

u32 func_001E8578(BtlLinkedCommand *command) {
    return 1;
}

void func_001E8580(BtlCamState *dst, BtlCamState *current, BtlCamState *target, f32 blend) {
    f32 delta;
    f32 value;

    dst->position[0] = current->position[0] + (target->position[0] - current->position[0]) * blend;
    dst->position[1] = current->position[1] + (target->position[1] - current->position[1]) * blend;
    dst->position[2] = current->position[2] + (target->position[2] - current->position[2]) * blend;
    dst->position[3] = 0.0f;
    dst->direction[0] = current->direction[0] + (target->direction[0] - current->direction[0]) * blend;
    dst->direction[1] = current->direction[1] + (target->direction[1] - current->direction[1]) * blend;
    value = current->direction[2];
    dst->direction[2] = value + (target->direction[2] - value) * blend;
    dst->direction[3] = 0.0f;
    delta = target->distance - current->distance;
    dst->distance = current->distance + delta * blend;
    delta = target->fov - current->fov;
    dst->fov = current->fov + delta * blend;
}

extern void func_001E8580(BtlCamState *, BtlCamState *, BtlCamState *, f32);

s32 btlStepPoseBlendHalf(BtlLinkedCommand *command) {
    f32 t;
    if (command->state == 0) {
        command->progressBits = 0;
        btlScalarRangeInitQuadratic(&command->quadraticRange, (f32)(command->durationFrames * 2));
        btlCopyMotionTransform(&command->camera, &command->frontCamera);
        return 0;
    }
    t = btlScalarRangeStepQuadratic(&command->quadraticRange, 1.0f);
    if (t > 0.5f) {
        t = 0.5f;
    }
    func_001E8580(&command->camera, &command->frontCamera, &command->backCamera, t + t);
    command->progress = t;
    if (0.5f <= t) {
        return 1;
    }
    return 0;
}

extern void func_001E8580(BtlCamState *, BtlCamState *, BtlCamState *, f32);

s32 btlStepPoseBlend(BtlLinkedCommand *command) {
    BtlExponentialRange *timer = &command->exponentialRange;
    BtlCamState *pose = &command->frontCamera;
    f32 t;
    if (command->state == 0) {
        btlScalarRangeSetStartClearEnd(timer, command->motionParameter);
        btlCopyMotionTransform(&command->camera, pose);
    }
    t = btlScalarRangeStepExponential(timer);
    func_001E8580(&command->camera, pose, &command->backCamera, t);
    command->progress = t;
    if (0.999999f <= t) {
        return 1;
    }
    return 0;
}

extern void func_001E8580(BtlCamState *, BtlCamState *, BtlCamState *, f32);

s32 btlStepPoseBlendFrame(BtlLinkedCommand *command) {
    f32 t;
    if (command->state == 0) {
        command->progressBits = 0;
        btlScalarRangeInitQuadratic(&command->quadraticRange, (f32)command->durationFrames);
        btlCopyMotionTransform(&command->camera, &command->frontCamera);
        return 0;
    }
    t = btlScalarRangeStepQuadratic(&command->quadraticRange, 1.0f);
    func_001E8580(&command->camera, &command->frontCamera, &command->backCamera, t);
    command->progress = t;
    if (0.999999f <= t) {
        return 1;
    }
    return 0;
}

s32 btlStepPoseBlendRatio(BtlLinkedCommand *command) {
    f32 ratio = (f32)command->state / (f32)command->durationFrames;
    if (ratio <= 1.0f) {
        func_001E8580(&command->camera, &command->frontCamera, &command->backCamera, ratio);
        return 0;
    }
    btlCopyMotionTransform(&command->camera, &command->backCamera);
    return 0;
}

/* Adjust the pose direction when projected camera height is above Y = -20. */
s32 btlAdjustCameraDirectionForDefaultPlane(BtlCamState *state) {
    f32 vector[4];
    f32 direction[4];
    f32 length = state->distance;
    f32 y;
    f32 scale;
    f32 delta;
    s32 changed = 0;

    if (!(length <= 1.0f)) {
        scale = -length;
        vector[0] = state->direction[0] * scale + state->position[0];
        y = state->position[1];
        vector[1] = state->direction[1] * scale + y;
        vector[2] = state->direction[2] * scale + state->position[2];
        vector[3] = 0.0f;
        if (-20.0f < vector[1]) {
            VU0_LOAD_VF(vf10, state->position);
            VU0_SET_VF10_COMPONENT(y, 0.0f);
            VU0_MOVE_VF(vf11, vf10);
            VU0_LOAD_VF(vf10, vector);
            VU0_SET_VF10_COMPONENT(y, 0.0f);
            VU0_SUB(vf10, vf10, vf11);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, direction);
            delta = y - (-20.0f);
            scale = fsqrtf(delta * delta + length * length);
            vector[0] = -direction[0] * scale;
            vector[1] = delta;
            vector[2] = -direction[2] * scale;
            VU0_LOAD_VF(vf10, vector);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, state->direction);
            changed = 1;
        }
    }
    return changed;
}

/* Adjust the pose direction using the supplied horizontal height plane. */
s32 btlAdjustCameraDirectionForPlane(BtlCamState *state, f32 height) {
    f32 vector[4];
    f32 direction[4];
    f32 length = state->distance;
    f32 y;
    f32 scale;
    f32 delta;
    s32 changed = 0;

    if (!(length <= 1.0f)) {
        scale = -length;
        vector[0] = state->direction[0] * scale + state->position[0];
        y = state->position[1];
        vector[1] = state->direction[1] * scale + y;
        vector[2] = state->direction[2] * scale + state->position[2];
        vector[3] = 0.0f;
        if (height < vector[1]) {
            VU0_LOAD_VF(vf10, state->position);
            VU0_SET_VF10_COMPONENT(y, 0.0f);
            VU0_MOVE_VF(vf11, vf10);
            VU0_LOAD_VF(vf10, vector);
            VU0_SET_VF10_COMPONENT(y, 0.0f);
            VU0_SUB(vf10, vf10, vf11);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, direction);
            delta = y - height;
            scale = fsqrtf(delta * delta + length * length);
            vector[0] = -direction[0] * scale;
            vector[1] = delta;
            vector[2] = -direction[2] * scale;
            VU0_LOAD_VF(vf10, vector);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, state->direction);
            changed = 1;
        }
    }
    return changed;
}

u32 btlExecuteCommandSoundTask(u32 *taskArgs) {
    func_001E8258(taskArgs[3], *taskArgs, taskArgs[1], taskArgs[2], taskArgs[4]);
    return 1;
}

BtlRuntimeTask *btlCreateCommandSoundTask(s32 actor, s32 mode) {
    BtlRuntimeTask *task = btlAllocTask(0x14);
    SoundTaskArgs *args;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x2A;
    if (actor != 0 && ((ActionStateLink *)actor)->unit != 0) {
        task->ownerId = ((ActionStateLink *)actor)->unit->owner;
    }
    task->callback = btlExecuteCommandSoundTask;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = (void *)actor;
    args->unk_0C = mode;
    args->option = 0;
    args->unk_08 = 0;
    args->unk_10 = 0;
    return task;
}

BtlRuntimeTask *btlCreateTargetedCommandSoundTask(s32 actor, s32 mode, u32 command) {
    BtlRuntimeTask *task = btlCreateCommandSoundTask(actor, mode);
    SoundTaskArgs *args = btlGetTaskArguments(task);

    args->unk_10 = command;
    return task;
}

BtlRuntimeTask *btlCreateCommandSoundWithArguments(s32 actor, s32 option, s32 flag, s32 mode, s32 command) {
    BtlRuntimeTask *task = btlCreateCommandSoundTask(actor, mode);
    SoundTaskArgs *args = btlGetTaskArguments(task);

    args->unk_10 = command;
    args->option = option;
    args->unk_08 = flag;
    return task;
}

u32 btlInitializeMotionTransformFromTaskArguments(u8 *arguments) {
    u8 *context = (u8 *)btlGetRuntime();
    func_001E8258(1, *(u32 *)arguments, 0, 0, 0);
    btlInitMotionTransformFromComponents(&((BtlState *)context)->cameraCommand.camera, ((BtlCameraTaskArgs *)arguments)->component[0], ((BtlCameraTaskArgs *)arguments)->component[1],
                    ((BtlCameraTaskArgs *)arguments)->component[2], ((BtlCameraTaskArgs *)arguments)->component[3],
                    ((BtlCameraTaskArgs *)arguments)->component[4], ((BtlCameraTaskArgs *)arguments)->component[5],
                    ((BtlCameraTaskArgs *)arguments)->component[6], ((BtlCameraTaskArgs *)arguments)->component[7]);
    return 1;
}

BtlRuntimeTask *btlCreateFloatTask28(ActionStateLink *actor, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h) {
    BtlRuntimeTask *task = btlAllocTask(0x24);
    f32 *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x2B;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    if (actor != 0 && actor->unit != 0) {
        task->ownerId = actor->unit->owner;
    }
    task->callback = btlInitializeMotionTransformFromTaskArguments;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    *(ActionStateLink **)args = actor;
    args[1] = a;
    args[2] = b;
    args[3] = c;
    args[4] = d;
    args[5] = e;
    args[6] = f;
    args[7] = g;
    args[8] = h;
    return task;
}

s32 btlApplyEffectCameraKeyframes(f32 *args) {
    s32 context = btlGetRuntime();
    func_001E8258(1, *(s32 *)args, 0, 0, 0);
    btlSetEffectCameraKeys(context + 0x70, args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8],
                  args[9], args[10], args[11], args[12], args[13], args[14], args[15], args[16]);
    return 1;
}

BtlRuntimeTask *btlCreateFloatTask29(ActionStateLink *actor, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, f32 a10, f32 a11, f32 a12, f32 a13, f32 a14, f32 a15, f32 a16) {
    BtlRuntimeTask *task = btlAllocTask(0x44);
    f32 *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x2C;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    if (actor != 0 && actor->unit != 0) {
        task->ownerId = actor->unit->owner;
    }
    task->callback = btlApplyEffectCameraKeyframes;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    *(ActionStateLink **)args = actor;
    args[1] = a1;
    args[2] = a2;
    args[3] = a3;
    args[4] = a4;
    args[5] = a5;
    args[6] = a6;
    args[7] = a7;
    args[8] = a8;
    args[9] = a9;
    args[10] = a10;
    args[11] = a11;
    args[12] = a12;
    args[13] = a13;
    args[14] = a14;
    args[15] = a15;
    args[16] = a16;
    return task;
}

u32 btlApplyCameraKeysAndMarkRuntimeChange(u32 taskArgs) {
    s32 work;

    work = btlGetRuntime();
    btlApplyEffectCameraKeyframes(taskArgs);
    ((BtlState *)work)->cameraCommand.flags |= 0x80000;
    return 1;
}

BtlRuntimeTask *btlCreateNotifyingCameraKeyframeTask(ActionStateLink *actor, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, f32 a10, f32 a11, f32 a12, f32 a13, f32 a14, f32 a15, f32 a16) {
    BtlRuntimeTask *task = btlCreateFloatTask29(actor, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
    task->callback = btlApplyCameraKeysAndMarkRuntimeChange;
    return task;
}

u32 btlRunCameraMotionResetTask(void) {
    BtlState *work = (BtlState *)btlGetRuntime();

    btlResetCameraMotion(&work->cameraCommand);
    return 1;
}

BtlRuntimeTask *btlScheduleContextReset(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlRunCameraMotionResetTask;
    task->taskId = 0x2D;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E9130);

extern f32 D_003B6D70[4], D_003B6D80[4];

extern f32 D_003B6D50[4], D_003B6D60[4];

extern u32 D_00436A98;

extern EffWorldNode *dds3CreateCameraObject(s32, void *, void *);

extern void dds3SetCameraFieldOfView(EffWorldNode *, f32);

extern void effObjSetInnerFloat(EffWorldNode *, f32);

extern void dds3EnsureSlotData(void *);

extern void func_001129C8(EffWorldNode *, s32);

/* vu0 routine: add the battle origin to the default camera position. */
void btlInitializeWorldCamera(void) {
    f32 position[4];
    EffWorldNode *camera;
    CameraData *data;
    BtlState *battle = (BtlState *)btlGetRuntime();

    battle->cameraCommand.camera.fov = 0.6981317f;
    VU0_LOAD_VF(vf10, D_003B6D70);
    VU0_LOAD_VF(vf11, battle->position);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, position);
    camera = dds3GetWorldCameraObject(dds3GetWorldObject());
    if (camera != NULL) {
        effObjSetInnerPosition(camera, (u128 *)position);
        effObjSetInnerRotation(camera, (u128 *)D_003B6D80);
        data = camera->data;
        dds3SetCameraFieldOfView(camera, 0.6981317f);
        data->fovUpdatePending |= 1;
    }
    camera = dds3CreateCameraObject(dds3AdvanceWorldCounter(), D_003B6D50, D_003B6D60);
    camera->value = (const char *)D_00436A98;
    effObjSetInnerFloat(camera, 10.0f);
    dds3EnsureSlotData(camera);
    func_001129C8(camera, 0);
    dds3SetCameraFieldOfView(camera, 0.6981317f);
    dds3SetWorldCameraObject(dds3GetWorldObject(), camera);
    battle->cameraObject = camera;
    battle->cameraCommand.targetList = btlAllocateIndexList(13);
    battle->battleFlags |= 0x10;
}

void btlClearPendingSoundList(void) {
    s32 context = btlGetRuntime();
    BtlIndexList *list = ((BtlState *)context)->cameraCommand.targetList;

    if (list != 0) {
        btlFreeIndexList(list);
        ((BtlState *)context)->cameraCommand.targetList = 0;
    }
    ((BtlState *)context)->battleFlags &= ~0x10;
}

void btlCopyMotionTransform(BtlCamState *dst, BtlCamState *src) {
    PCP_COPY_VECTOR(dst->position, src->position);
    PCP_COPY_VECTOR(dst->direction, src->direction);
    dst->distance = src->distance;
    dst->fov = src->fov;
}

void btlSetMotionTransformFieldOfView(BtlCamState *object, f32 fovRadians) {
    object->fov = fovRadians;
}

void btlInitMotionTransformFromVectors(BtlCamState *object, f32 *origin, f32 *direction) {
    VU0_LOAD_VF(vf10, direction);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9130);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, object->direction);
    VU0_SET_VF2X(1.0f);
    VU0_MUL_VF2X(vf10, vf10);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, origin);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, object->position);
    object->distance = 1.0f;
    object->fov = 0.6981317f;
    btlClearRuntimeFlag2000();
}

void btlInitMotionTransformFromComponents(BtlCamState *object, f32 x, f32 y, f32 z, f32 vx, f32 vy,
                    f32 vz, f32 vw, f32 fovDegrees) {
    f32 origin[4];
    f32 direction[4];
    origin[0] = x;
    origin[1] = y;
    origin[2] = z;
    direction[0] = vx;
    direction[1] = vy;
    direction[2] = vz;
    direction[3] = vw;
    origin[3] = 0.0f;
    btlInitMotionTransformFromVectors(object, origin, direction);
    object->fov = fovDegrees * 0.017453293f;
}

void btlSetEffectCameraKeys(s32 fx, f32 x0, f32 y0, f32 z0, f32 vx0, f32 vy0, f32 vz0, f32 vw0, f32 x1, f32 y1, f32 z1,
                   f32 vx1, f32 vy1, f32 vz1, f32 vw1, f32 scale, f32 f154) {
    btlInitMotionTransformFromComponents(&((BtlLinkedCommand *)fx)->frontCamera, x0, y0, z0, vx0, vy0, vz0, vw0, scale);
    btlInitMotionTransformFromComponents(&((BtlLinkedCommand *)fx)->backCamera, x1, y1, z1, vx1, vy1, vz1, vw1, scale);
    ((BtlLinkedCommand *)fx)->motionParameter = f154;
    ((BtlLinkedCommand *)fx)->flags |= 0x41;
}

u32 btlGetActiveUnitId(void) {
    s32 workAddress;

    workAddress = btlGetRuntime();
    return ((BtlState *)workAddress)->cameraCommand.status;
}

f32 btlGetPoseBlendProgress(u8 *unit) {
    return ((BtlLinkedCommand *)unit)->progress;
}

s32 btlIsUnitInActiveList(void *unit) {
    u8 *work = (u8 *)btlGetRuntime();
    u8 *slot = (u8 *)((BtlState *)work)->cameraCommand.link;
    u32 count;
    u32 i;
    if (slot != 0 && ((BtlActiveSlot *)slot)->unit == unit) {
        return 1;
    }
    count = btlGetIndexListCount(((BtlState *)work)->cameraCommand.targetList);
    for (i = 0; i < count; i++) {
        if (btlGetIndexListEntry(((BtlState *)work)->cameraCommand.targetList, i) == unit) {
            return 1;
        }
    }
    return 0;
}

void btlResetActiveUnitList(void) {
    s32 workAddress;

    workAddress = btlGetRuntime();
    ((BtlState *)workAddress)->cameraCommand.link = 0;
    ((BtlState *)workAddress)->cameraCommand.flags = ((BtlState *)workAddress)->cameraCommand.flags | 0x400;
    btlClearIndexList(((BtlState *)workAddress)->cameraCommand.targetList);
}

void btlClearRuntimeFlag2000(void) {
    s32 workAddress;

    workAddress = btlGetRuntime();
    ((BtlState *)workAddress)->cameraCommand.flags = ((BtlState *)workAddress)->cameraCommand.flags & 0xffffdfff;
}

void btlSetRuntimeFlag2000(void) {
    s32 workAddress;

    workAddress = btlGetRuntime();
    ((BtlState *)workAddress)->cameraCommand.flags = ((BtlState *)workAddress)->cameraCommand.flags | 0x2000;
}

u32 btlIsRuntimeFlag2000Clear(void) {
    s32 workAddress;

    workAddress = btlGetRuntime();
    return ((((s32)((BtlState *)workAddress)->cameraCommand.flags >> 0xd)) ^ 1U) & 1;
}

typedef struct WorldObjectSub {
    u8 pad0[0x34];
    s32 handle;
} WorldObjectSub;

typedef struct WorldObjectHead {
    u8 pad0[8];
    WorldObjectSub *sub;
} WorldObjectHead;

typedef struct WorldObj {
    u8 pad0[0x18];
    WorldObjectHead *head;
} WorldObj;

extern f32 dds3GetCameraFieldOfView(EffWorldNode *);

extern void sdfSetViewFieldOfView(f32);

void btlRefreshWorldCameraHandle(void) {
    WorldObj *object;
    EffWorldNode *handle;
    if (((BtlState *)btlGetRuntime())->battleFlags & 2) {
        object = dds3GetWorldObject();
        if (object != NULL) {
            handle = dds3GetWorldCameraObject((EffWorldNode *)object);
            if (handle != 0) {
                if (handle->next != 0) {
                    handle = handle->next;
                } else {
                    handle = (EffWorldNode *)object->head->sub->handle;
                }
                dds3SetWorldCameraObject((EffWorldNode *)object, handle);
                sdfSetViewFieldOfView(dds3GetCameraFieldOfView(handle));
            }
        }
    }
}

extern s32 D_00436A9C;

extern s32 D_00436AA0;

s32 btlGetWorldObjectDefault(void) {
    EffWorldNode *data;
    if (!(((BtlState *)btlGetRuntime())->battleFlags & 2)) {
        return D_00436AA0;
    }
    data = dds3GetWorldCameraObject(dds3GetWorldObject());
    if (data == 0) {
        return D_00436AA0;
    }
    if (data->value == 0) {
        return D_00436A9C;
    }
    return (s32)data->value;
}

s32 btlIsWorldMotionIdle(void) {
    EffWorldNode *data;
    if (!(((BtlState *)btlGetRuntime())->battleFlags & 2)) {
        return 0;
    }
    data = dds3GetWorldCameraObject(dds3GetWorldObject());
    if (data == 0) {
        return 0;
    }
    return data->value == 0;
}

s32 btlGetCameraVectorWork(void) {
    s32 work;

    work = btlGetRuntime();
    return work + 0x70;
}

void btlFlagAllUnitDefeatCandidatesTask(void) {
    btlFlagAllUnitsDefeatCandidate();
}

void btlClearAllUnitDefeatCandidatesTask(void) {
    btlClearAllUnitDefeatCandidates();
}

void btlFlagLinkedGroupDefeatCandidatesTask(s32 action) {
    btlFlagMatchingUnitsDefeatCandidate(((BtlLinkedCommand *)action)->link->unit->status.flags & 0x600);
}

void btlApplyCombinedActorFlags(u8 *fx) {
    u32 i = 0;
    s32 bits = 0;
    u32 count = btlGetIndexListCount(((BtlLinkedCommand *)fx)->targetList);
    if (count != 0) {
        do {
            bits |= ((BtlUnit *)btlGetIndexListEntry(((BtlLinkedCommand *)fx)->targetList, i++))->status.flags & 0x600;
        } while (i < count);
    }
    if (bits != 0) {
        btlFlagMatchingUnitsDefeatCandidate(bits);
    }
}

extern f32 D_003B6D90[];

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417C78);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417C88);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417C98);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417CA8);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417CB8);

void btlResetCameraMotion(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    f32 current;
    f32 limit;
    if (work->cameraCommand.status == 1 || btlHasSingleLinkedResource(action) != 0) {
        for (unit = work->units; unit != 0; unit = unit->nextActor) {
            if (unit->status.flags & 1) {
                if (unit->status.flags & 0x200) {
                    if (unit->status.flags & 2) {
                        if (unit->ext != 0) {
                            s32 node = mdlGetNodeMotionIndex(unit->ext->owner, 0);
                            if (node == 0xD || node == 0x12) {
                                current = btlGetUnitModelValue1C(unit);
                                limit = (f32)btlGetUnitModelFrameCount(unit);
                                if (unit->partyRecord.unitId < 0xA) {
                                    limit = limit * D_003B6D90[unit->partyRecord.unitId];
                                } else {
                                    limit = limit * 0.7f;
                                }
                                if (current < limit) {
                                    sdfMotionSampleAtFrame(unit->ext->owner->first, limit);
                                    btlBossDebugPrintf("btl:camera mot reset[%p]\n", unit);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

s32 btlPositionActorIndexUnits(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    u32 count;
    f32 pos[4];
    if (work->unk268 != 3) {
        return 0;
    }
    count = btlGetIndexListCount(action->targetList);
    if (count != 1) {
        return 0;
    }
    unit = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
    if (!(unit->status.flags & 0x200)) {
        return 0;
    }
    if (unit->lookupId != count) {
        return 0;
    }
    for (unit = work->units; unit != 0; unit = unit->nextActor) {
        if ((unit->status.flags & 0x100) && unit->lookupId != 1) {
            func_001E3108(unit, pos);
            pos[2] = unit->position[2] - 90.0f;
            btlSetUnitPosition(unit, pos);
        }
    }
    return 1;
}

void btlDebugPrintWorldTransform(s32 arg0, u8 *arg1) {
    EffWorldNode *object;
    if (((BtlState *)btlGetRuntime())->battleFlags & 2) {
        object = dds3GetWorldCameraObject(dds3GetWorldObject());
        if (object != 0) {
            btlBossDebugPrintfN(arg0, (s32)arg1, 0, "P:%.1f %.1f %.1f", (double)object->inner->position[0],
                                (double)object->inner->position[1], (double)object->inner->position[2]);
            btlBossDebugPrintfN(arg0, (s32)(arg1 + 0xC), 0, "R:%.3f %.3f %.3f %.3f",
                                (double)object->inner->rotation[0], (double)object->inner->rotation[1],
                                (double)object->inner->rotation[2], (double)object->inner->rotation[3]);
        }
    }
}

extern f32 func_00208750(BtlIndexList *, f32 *, f32 *);

void btlFaceActionParticipantsTowardLinkedTarget(BtlLinkedCommand *action) {
    s128 vec[3];
    s128 *pos;
    BtlUnit *target;
    BtlUnit *first;
    u32 i;
    u32 count = btlGetIndexListCount(action->targetList);
    if (count != 0) {
        target = action->link->unit;
        if (count == 1) {
            first = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
            if (target != 0 && (target->status.flags & 0x600) == (first->status.flags & 0x600)) {
                return;
            }
            btlUnitGetMuzzlePosVU(first);
        } else {
            func_00208750(action->targetList, 0, 0);
        }
        pos = &vec[1];
        VU0_STORE_VF(vf10, pos);
        if (target != 0) {
            btlUnitGetMuzzlePosVU(target);
            VU0_STORE_VF(vf10, &vec[0]);
            if (target->status.flags & 0x80000) {
                if (btlAimHorizontalDirectionVU((f32 *)&vec[0], pos) != 0) {
                    VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
                    btlSetUnitRotation(target, &vec[2]);
                }
            }
        }
        for (i = 0; i < count; i++) {
            btlUnitFaceTarget((BtlUnit *)btlGetIndexListEntry(action->targetList, i), target);
        }
    }
}

void btlAimLinkedUnitAtMuzzle(BtlLinkedCommand *action) {
    s128 vec[3];
    s128 *pos;
    BtlUnit *target;
    u32 count = btlGetIndexListCount(action->targetList);
    if (count != 0) {
        target = action->link->unit;
        if (count == 1) {
            btlUnitGetMuzzlePosVU((BtlUnit *)btlGetIndexListEntry(action->targetList, 0));
        } else {
            func_00208750(action->targetList, 0, 0);
        }
        pos = &vec[1];
        VU0_STORE_VF(vf10, pos);
        if (target != 0) {
            btlUnitGetMuzzlePosVU(target);
            VU0_STORE_VF(vf10, &vec[0]);
            if (target->status.flags & 0x80000) {
                if (btlAimHorizontalDirectionVU((f32 *)&vec[0], pos) != 0) {
                    VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
                    btlSetUnitRotation(target, &vec[2]);
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", btlMatchFirstLinkedActorFlags);

/* Return whether a live linked group has its byte at 0x14 marked. */
s32 btlHasMarkedEntry14(s32 actor) {
    ActionStateLink *linked = ((BtlLinkedCommand *)actor)->link;
    u32 count;
    BtlOperandGroup *entry;
    u32 i;

    if (linked == 0) {
        return 0;
    }
    count = btlGetIndexListCount(linked->indexWork.indices);
    entry = linked->indexWork.groups;
    for (i = 0; i < count; i++, entry++) {
        if (entry->reflected != 0) {
            return 1;
        }
    }
    return 0;
}

s32 btlCanUseLinkedActor(s32 actor) {
    u32 status = ((BtlLinkedCommand *)actor)->status;
    s32 linked;
    s32 category;

    switch (status) {
    case 5:
    case 6:
    case 7:
    case 8:
        break;
    default:
        return 1;
    }
    linked = (s32)((BtlLinkedCommand *)actor)->link;
    if (linked == 0) {
        return 1;
    }
    if (btlHasMarkedEntry14(actor)) {
        return 0;
    }
    if (((ActionStateLink *)linked)->unit->partyRecord.status & 0x480) {
        return 0;
    }
    category = ((BtlLinkedCommand *)actor)->actionCode;
    if (category != 0 && (((BtlActionTableEntry *)datActionAnimationRecords)[category].flags & 1)) {
        return 0;
    }
    return 1;
}

/* Return whether a live linked group has its byte at 0x10 marked. */
s32 btlHasMarkedEntry10(s32 actor) {
    ActionStateLink *linked = ((BtlLinkedCommand *)actor)->link;
    u32 count;
    BtlOperandGroup *entry;
    u32 i;

    if (linked == 0) {
        return 0;
    }
    count = btlGetIndexListCount(linked->indexWork.indices);
    entry = linked->indexWork.groups;
    for (i = 0; i < count; i++, entry++) {
        if (entry->inactive != 0) {
            return 1;
        }
    }
    return 0;
}

extern f32 btlUnitGetTopY(BtlUnit *);

s32 btlCheckActorDistanceLimit(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;

    while (unit != NULL) {
        u32 flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (btlUnitGetTopY(unit) > 400.0f) {
                    return 0;
                }
            }
        }
        unit = unit->nextActor;
    }
    return 1;
}

s32 btlIsEntryHeightWithinLimit(void) {
    if (func_00208000(0x400, 0, 0) > 600.0f) {
        return 0;
    }
    return 1;
}

/* Find an unmarked linked kind-two slot whose unit is not disabled. */
s32 btlHasIdleLinkedSlotKindTwo(u8 *actor) {
    ActionStateLink *linked = ((BtlLinkedCommand *)actor)->link;
    u32 count;
    u32 i;
    BtlOperandGroup *entry;
    if (linked == NULL) {
        return 0;
    }
    i = 0;
    count = btlGetIndexListCount(linked->indexWork.indices);
    entry = linked->indexWork.groups;
    for (; i < count; i++, entry++) {
        if (entry->inactive == 0 && entry->kind == 1 && entry->parameter == 2 &&
            !(((BtlUnit *)btlGetIndexListEntry(linked->indexWork.indices, i))->status.flags & 0x80002000)) {
            return 1;
        }
    }
    return 0;
}

/* Find a type-two linked group whose associated unit is not disabled. */
s32 btlHasEligibleLinkedEntryTypeTwo(u8 *actor) {
    ActionStateLink *linked = ((BtlLinkedCommand *)actor)->link;
    u32 count;
    u32 i;
    BtlOperandGroup *entry;
    if (linked == NULL) {
        return 0;
    }
    i = 0;
    count = btlGetIndexListCount(linked->indexWork.indices);
    entry = linked->indexWork.groups;
    for (; i < count; i++, entry++) {
        if (entry->kind == 2 &&
            !(((BtlUnit *)btlGetIndexListEntry(linked->indexWork.indices, i))->status.flags & 0x80002000)) {
            return 1;
        }
    }
    return 0;
}

s32 btlHasLinkedEffectNodeTrigger(BtlLinkedCommand *fx) {
    ActionStateLink *task;
    BtlUnit *owner;
    s32 index;
    BtlEffectResource *table;
    if (fx->link == 0) {
        return 0;
    }
    if (btlHasSingleLinkedResource(fx) == 0) {
        return 0;
    }
    task = fx->link;
    owner = task->unit;
    index = task->indexWork.slot;
    table = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(owner->resourceKind, owner->resourceIndex);
    if (fx->actionCode == 0x91) {
        return 0;
    }
    return table->nodes[index].triggerKind == 2;
}

static inline s32 btlHasFlag(u32 flags, u32 mask) {
    return (flags & mask) != 0;
}

s32 btlHasActorCategoryFlag100(BtlLinkedCommand *action) {
    s32 category = action->actionCode;

    if (category == 0) {
        return 0;
    }
    return btlHasFlag(((BtlActionTableEntry *)datActionAnimationRecords)[category].flags, 0x100);
}

s32 btlIsActorCategoryTypeTwo(BtlLinkedCommand *action) {
    s32 category = action->actionCode;

    if (category == 0) {
        return 0;
    }
    return datCommandRecords[category].unk30 == 2;
}

extern s32 btlIsActorCategoryMarked(s32);

s32 btlCanUseActorCategoryFlag2(s32 actor) {
    s32 category;

    if (btlIsActorCategoryMarked(actor)) {
        return 1;
    }
    if (!btlCanUseLinkedActor(actor)) {
        return 0;
    }
    category = ((BtlLinkedCommand *)actor)->actionCode;
    if (category == 0) {
        return 0;
    }
    return btlHasFlag(((BtlActionTableEntry *)datActionAnimationRecords)[category].flags, 2);
}

s32 btlHasSingleLinkedResource(BtlLinkedCommand *actor) {
    s32 category = actor->actionCode;

    if (category != 0 && datCommandRecords[category].targetType != 0) {
        return 0;
    }
    return btlGetIndexListCount(actor->targetList) == 1;
}

s32 btlCanUseActorCategoryFlag4(s32 actor) {
    s32 category = ((BtlLinkedCommand *)actor)->actionCode;

    if (category == 0) {
        return 0;
    }
    if ((datCommandRecords[category].options & 1) == 0) {
        if (!btlCanUseLinkedActor(actor)) {
            return 0;
        }
    }
    return btlHasFlag(((BtlActionTableEntry *)datActionAnimationRecords)[((BtlLinkedCommand *)actor)->actionCode].flags, 4);
}

s32 btlIsActorCategoryMarked(s32 actor) {
    s32 category = ((BtlLinkedCommand *)actor)->actionCode;

    if (category == 0) {
        return 0;
    }
    return datCommandRecords[category].unk30 == 1;
}

s32 btlHasActorCategoryFlag40(BtlLinkedCommand *action) {
    s32 category = action->actionCode;

    if (category == 0) {
        return 0;
    }
    return btlHasFlag(((BtlActionTableEntry *)datActionAnimationRecords)[category].flags, 0x40);
}

s32 btlMatchLinkedActorFlags(s32 actor) {
    s32 linked;
    BtlUnit *entry;

    switch (((BtlLinkedCommand *)actor)->status) {
    case 4:
    case 5:
    case 6:
        break;
    default:
        return 0;
    }
    linked = (s32)((BtlLinkedCommand *)actor)->link;
    if (linked == 0) {
        return 0;
    }
    if (btlGetIndexListCount(((ActionStateLink *)linked)->indexWork.indices) >= 2) {
        return 0;
    }
    entry = btlGetIndexListEntry(((ActionStateLink *)linked)->indexWork.indices, 0);
    return ((((ActionStateLink *)linked)->unit->status.flags ^ entry->status.flags) & 0x600) == 0;
}

s32 btlHasFirstLinkedCategoryFlag1000(BtlLinkedCommand *actor) {
    ActionStateLink *linked = actor->link;
    BtlUnit *entry;
    u32 category;

    if (linked == 0) {
        return 0;
    }
    if (btlGetIndexListCount(linked->indexWork.indices) >= 2) {
        return 0;
    }
    entry = btlGetIndexListEntry(linked->indexWork.indices, 0);
    if ((entry->status.flags & 0x400) == 0) {
        return 0;
    }
    category = entry->resourceIndex;
    if (category >= 0x180) {
        return 0;
    }
    return btlHasFlag(datEnemyRecords[category].flags, 0x1000);
}

u8 func_001EA940(BtlLinkedCommand *action) {
    return action->actionCode == 0x5f;
}

s32 btlMapActorCategory(s32 actor) {
    switch ((u32)((BtlLinkedCommand *)actor)->actionCode) {
    case 0x09:
        return 0x29;
    case 0x12:
        return 0x51;
    case 0x1B:
        return 0x2B;
    case 0x24:
        return 0x4D;
    case 0x2D:
        return 0x3C;
    case 0x5B:
        return 0x45;
    case 0x5C:
        return 0x6E;
    case 0x5D:
        return 0xAA;
    default:
        return 0;
    }
}

s32 btlIsSpecialActorCategory(s32 actor) {
    switch ((u32)((BtlLinkedCommand *)actor)->actionCode) {
    case 0x5B:
    case 0x5C:
    case 0x5D:
        return 1;
    default:
        return 0;
    }
}

u32 func_001EAA00(BtlLinkedCommand *action) {
    return 0;
}

u8 func_001EAA08(BtlLinkedCommand *action) {
    return action->actionCode == 0x1a0;
}

void func_001EAA18(void) {
}

void func_001EAA20(void) {
}

extern void btlPrepareActionCameraPoseWithActorClearance(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void func_001F20B0(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void func_001F20C8(BtlCamState *, BtlCamState *, BtlCamState *);

/* Choose the action's camera pose from active ally and enemy height maxima. */
void btlChooseCameraPoseByActorHeights(BtlLinkedCommand *action) {
    BtlState *work;
    BtlUnit *unit;
    s32 enemyCount;
    f32 enemyHeight;
    f32 allyHeight;
    f32 height;

    work = (BtlState *)btlGetRuntime();
    if (work->unk670 != NULL) {
        if (work->unk670(action)) {
            return;
        }
    }
    enemyCount = 0;
    enemyHeight = 0.0f;
    allyHeight = 0.0f;
    unit = work->units;
    for (; unit != NULL; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            height = btlUnitGetTopY(unit);
            if (unit->status.flags & 0x200) {
                if (allyHeight < height) {
                    allyHeight = height;
                }
            } else if (unit->status.flags & 0x400) {
                if (enemyHeight < height) {
                    enemyHeight = height;
                }
                enemyCount++;
            }
        }
    }
    if (enemyCount == 1 && allyHeight + 100.0f < enemyHeight) {
        switch (effMiscRandMod(0, 4)) {
        case 0:
        case 1:
            if (enemyHeight <= 500.0f) {
                func_001F20C8(&action->camera, &action->frontCamera, &action->backCamera);
            } else {
                btlPrepareActionCameraPoseWithActorClearance(action, &action->frontCamera, &action->backCamera);
            }
            break;
        case 2:
            btlPrepareActionCameraPoseWithActorClearance(action, &action->frontCamera, &action->backCamera);
            break;
        case 3:
            func_001F20B0(action, &action->frontCamera, &action->backCamera);
            break;
        }
    } else {
        switch (effMiscRandMod(0, 2)) {
        case 0:
            btlPrepareActionCameraPoseWithActorClearance(action, &action->frontCamera, &action->backCamera);
            break;
        case 1:
            func_001F20B0(action, &action->frontCamera, &action->backCamera);
            break;
        }
    }
    action->motionParameter = 100.0f;
    action->flags |= 0x41;
}

void func_001EAC08(void) {
}

void func_001EAC10(BtlLinkedCommand *action) {
    func_001ECBF8(action, &action->camera);
}

void func_001EAC28(void) {
}

extern void btlInitTargetCursorAndFacing(BtlLinkedCommand *, BtlCamState *);

extern void btlPrepareUnitPoseWithTiltRotation(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void btlFlagUserAndTargetDefeat(BtlLinkedCommand *, BtlLinkedCommand *);

extern void btlSetupActionCameraPair(BtlLinkedCommand *);

/* Select the linked command's initial cursor step and prepare its camera. */
void btlInitializeLinkedCommandCursor(BtlLinkedCommand *action) {
    s32 (*hook)(BtlUnit *) = ((BtlState *)btlGetRuntime())->unk648;
    ActionStateLink *link;
    u32 flags;

    action->stepKind = 0;
    link = action->link;
    if (hook != NULL && hook((BtlUnit *)action) != 0) {
        return;
    }
    flags = link->unit->status.flags;
    if (flags & 0x200) {
        if (!(flags & 0x1000) && !(link->unit->partyRecord.flags & 0x10)) {
            action->stepKind = 0xB;
            func_001FF5D8(action, &action->camera);
        } else if (btlHasSingleLinkedResource(action) != 0) {
            if ((link->unit->partyRecord.flags & 0x10) && link->indexWork.slot == 0x17) {
                action->stepKind = 0xC;
                func_001F3C30(action);
            } else {
                action->stepKind = 9;
                btlFlagUserAndTargetDefeat(action, action);
            }
        } else {
            btlInitTargetCursorAndFacing(action, &action->camera);
        }
    } else {
        if (btlMatchLinkedActorFlags((s32)action) != 0) {
            func_001F0968(action);
        } else if (btlHasSingleLinkedResource(action) != 0) {
            btlPositionActorIndexUnits(action);
            action->stepKind = 0xA;
            btlSetupActionCameraPair(action);
        } else {
            btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
            btlAimLinkedUnitAtMuzzle(action);
            action->motionParameter = 200.0f;
            action->flags |= 0x41;
        }
        btlResetCameraMotion(action);
    }
}

void btlDispatchActionCursorStepByKind(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (work->unk64C != 0 && work->unk64C((BtlUnit *)action) != 0) {
        return;
    }
    switch (action->stepKind) {
    case 9:
        btlBuildApproachCamera(action, &action->camera);
        break;
    case 0xA:
        btlUpdateActionTargetCameraPose(action);
        break;
    case 0xB:
        btlAdvanceCursorForUnmarkedUnit(action, &action->camera);
        break;
    case 0xC:
        func_001F3E48(action);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EAE88);

extern void btlAdvanceUnblockedPlayerCursorAnimation(BtlLinkedCommand *);

extern void btlRefreshActionPoseBlendSnapshot();

extern void btlAimEffectPoseAtUnit();

extern void func_001ED9A0();

extern void func_001F34E0(BtlLinkedCommand *, BtlCamState *);

extern void btlBuildHeightClampedApproachCamera(BtlLinkedCommand *, BtlCamState *);

/* Dispatch camera-step work unless a runtime override handles it. */
void btlDispatchActionCameraStep(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();

    if (func_001EAA00(action) != 0) {
        btlAdvanceUnblockedPlayerCursorAnimation(action);
        return;
    }
    if (work->actionCameraStepHook != 0 && work->actionCameraStepHook(action) != 0) {
        return;
    }
    switch (action->stepKind) {
    case 2:
        btlRefreshActionPoseBlendSnapshot(action, &action->camera);
        break;
    case 9:
        btlBuildApproachCamera(action, &action->camera);
        break;
    case 4:
        btlAimEffectPoseAtUnit((u8 *)action, (u8 *)&action->camera);
        break;
    case 5:
        func_001ED9A0(action, &action->camera);
        break;
    case 7:
        func_001F34E0(action, &action->camera);
        break;
    case 8:
        btlBuildHeightClampedApproachCamera(action, &action->camera);
        break;
    }
}
INCLUDE_SDATA(const s32, "game/code_001DD390", btlDeferredTaskHead);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A28);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A30);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A38);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A40);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A48);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A50);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A58);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A60);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A68);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A70);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A78);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A80);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A88);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A90);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A98);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A9C);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436AA0);

