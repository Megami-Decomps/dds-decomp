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
#include "kwln.h"
#include "btl_task_state.h"
#include "btl_task_condition.h"
#include "sdf_resource.h"
#include "sdf_model.h"
#include "btl.h"
#include "sdf_texture_offset_list.h"
#include "btl_state.h"
#include "btl_model_record.h"
#include "btl_task_args.h"
#include "btl_sound.h"
#include "eff_field_color.h"
#include "dds3obj.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "fpu.h"
#include "dat_state.h"
#include "evt_unit.h"
#include "mdl.h"
#include "mdl_asset_request.h"
#include "btl_action.h"
#include "btl_unit_tasks.h"
#include "sdf.h"
#include "file.h"
#include "dat_command.h"
#include "file_request_api.h"
#include "dat_affinity.h"

enum {
    BTL_SOUND_ENTRY_COUNT = 0x31,
    BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT = 0x26
};

extern s32 func_003014F0(char *dst, const char *format, ...);

extern s32 fileTestSavedSlotFlags(u32);

extern s32 btlGetCommandFailureReason(BtlUnit *, s32);

extern u32 func_001A3360(void *, BtlIndexList *, s32);

extern BtlUnit *btlSelectUnitAtExtremeX(BtlUnit *, BtlIndexList *);

extern BtlUnit *btlFindActorForOwner(s64);

void btlPrepareSavedCommandTargets(BtlTask *, BattleIndexWork *);

extern s32 btlIsUnitInActiveList(void *unit);

extern void btlBossDebugPrintf(const char *format, ...);

extern u32 effMiscRandMod(void *state, u32 modulus);

extern u64 btlStartTask(void *);

extern void btlDispatchStateHandler(void *, s32);

extern s32 btlHasSingleLinkedResource(BtlLinkedCommand *);

extern void btlResetCameraMotion(BtlLinkedCommand *);

extern s32 btlCountTasksForOwner(s64);

typedef struct BtlPosLerpTaskArgs BtlPosLerpTaskArgs;

extern s32 btlUpdateUnitPositionInterpolationTask(BtlPosLerpTaskArgs *);

extern void btlUnitGetMuzzlePosVU(void *);

typedef struct BtlRotationTaskArgs BtlRotationTaskArgs;

extern s32 btlStepUnitRotationNlerp(BtlRotationTaskArgs *);

extern u64 btlAdvanceRuntimeSequenceCounter();

extern u8 *btlAllocateIndexedUnitEffectTask(u8 *, s32, s32, f32);

/* Battle runtime prefix: actor/task/sound registrations and packed battle tint. */
typedef struct BtlActorWork {
    u8 pad_00[0x208];
    s32 unk208;
    u8 pad20C[0x1C];
    BtlUnit *actorList;
    struct SoundTask *taskTail; /* Newest registration; reverse traversal. */
    struct SoundTask *taskHead; /* Oldest registration; forward traversal. */
    struct SoundResourceNode *soundResourceHead; /* Allocated resource nodes. */
    struct ActiveSoundNode *soundList;           /* Independent active-node list. */
    struct SoundSlotOwner *soundSlotOwners;     /* Shared category/id owners. */
    u8 pad240[0x24];
    u32 unk264;
    s32 unk268;
    u8 pad26C[0x38];
    s32 unk2A4;
    u8 pad2A8[0x210];
    struct SoundResourceNode *soundResourceSlots[BTL_SOUND_ENTRY_COUNT];
    u8 pad57C[8];
    u8 fadeEnabled; /* 0 raises the tint, 1 lowers it; refreshed by the frame updater. */
    u8 pad585[3];
    u32 fadeColor; /* Packed tint; retain the original whole-word arithmetic. */
    u8 pad58C[0x58];
    s32 (*hook5E4)(BtlUnit *); /* Actor-state predicate consulted by the defeat transition. */
    s32 (*hook5E8)(BtlUnit *); /* Fallback predicate consulted when hook5E4 returns 0. */
} BtlActorWork;

/* Tagged scheduler predicate, embedded for task entry and exit. */
typedef struct TaskCondition {
    u8 kind; /* 0 never, 1 always, 2 counter threshold, 3-10 task queries. */
    u8 pad01[7];
    union {
        s32 count;    /* Kind 2: signed threshold for the supplied counter. */
        u64 handle;   /* Kinds 3-5: task handle. */
        u64 owner;    /* Kinds 6-8: task owner; queries select its oldest task. */
        u16 taskKind; /* Kinds 9-10: registered task kind. */
    } value;
} TaskCondition;

/* Generic scheduler header; task-specific arguments follow at byte 0x70.
 * next/prev link registration order, independently of the deferred queue. */
typedef struct SoundTask {
    TaskCondition startCondition;
    TaskCondition endCondition;
    u16 taskId;
    u16 state; /* 0 waiting, 1 start delay, 2 running, 3 end delay. */
    u16 flags;
    u8 unk_26[2];
    s32 startDelay;
    s32 endDelay;
    u32 pollCount; /* Eligible scheduler polls, including wait/delay phases. */
    u32 runCount;  /* Callback updates that continued the running phase. */
    u64 handle; /* Installed by btlStartTask; independent of owner. */
    u64 owner;
    void (*onStart)(u32);
    union {
        void (*update)(void);
        s32 (*playSound)(u32 *);
        u32 (*process)(void);
        u32 (*commandSound)(u32 *);
        u32 (*playCustomSound)(u8 *);
        s32 (*releaseSound)(u16 *);
        s32 (*acquireSound)(u32 *);
        s32 (*run)(void *);
    } callback;
    void (*onFinish)(u32 *);
    void *args;
    struct SoundTask *next;
    struct SoundTask *prev;
    struct SoundTask *deferNext;
    struct SoundTask *deferPrev;
    u8 pad68[8];
} SoundTask;

typedef struct ActiveSoundNode {
    u32 flags;
    u8 unk_04[8];
    struct ActiveSoundNode *previous;
    struct ActiveSoundNode *next;
} ActiveSoundNode;

typedef struct SndPad {
    u8 pad00[0x21];
    s8 confirm;
    s8 edge22;
    s8 edge23;
    u8 pad24[2];
    s8 prev;
    s8 next;
} SndPad;

extern SndPad D_00324510;

extern void sndSetSequenceVolumePan(s32 arg0, s32 arg1, s32 arg2);

extern void evtInitializeUnitColorTransition(EvtUnit *, s32, u32, u32);

extern u32 kwlnDrawControlFlags;

extern void btlDestroyUnit(u8 *);

extern void func_001D4E60(BtlUnit *, BtlUnit *);

extern u32 dds3AdvanceWorldCounter(void);

extern s32 btlSetActorEffectParameter();

extern s32 mdlFlagTest(u32);

extern SoundTask *btlDeferredTaskTail;

extern SoundTask *btlDeferredTaskHead;

extern DatEnemyRecord *datEnemyRecords;

extern void func_001DF358(BtlLinkedCommand *, BtlCamState *);

extern void btlPrepareUnitPoseWithTiltRotation(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void btlApplyScaledUnitEffectParameter(u8 *, s32, s32, f32);

extern s32 btlGetRuntime(void);

extern s32 datActionAnimationRecords;

extern s8 effSharedRandomState[];

extern void btlResetDeferredTaskQueue(void);

s32 btlAdjustUnitHp(DatPartyRecord *object, s32 value);

s32 btlAdjustUnitMp(DatPartyRecord *object, s32 value);

u16 btlRefreshUnitMaximumHpAndClampCurrentHp(DatPartyRecord *object);

u16 btlRefreshUnitMaximumMpAndClampCurrentMp(DatPartyRecord *object);

void func_001A1948();

extern s8 effSharedRandomState[];

extern SndPad D_00324510;

extern void btlInitializeWorldCamera(void);

/* No selected entry is represented by -1. */ void btlClearActorSelectedEntryIndex(BtlUnit *actor);

void btlClearAllActorEntrySlots(BtlUnit *unit);

s32 btlGetActorBedAssetIdFromIndex(s32 arg0);

s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *entry);

struct EvtUnit;

extern void evtSetTransitionMotionScale(struct EvtUnit *, f32);

extern s32 btlIsCurrentValueBelowQuarterThreshold(void *);

extern s32 btlTestActorStatusPredicate(BtlUnit *);

extern void btlApplyUnitModelScaledValue(BtlUnit *);

extern s32 btlIsActorModeAcceptedByBattleHook(BtlUnit *object);

extern void btlApplyUnitMotionSelection(BtlUnit *, u32, s32, f32);

extern void btlRefreshUnitMotionSelection(BtlUnit *unit);

extern void btlRefreshUnitEffectMotionAndEntry(BtlUnit *unit);

extern void evtPrepareUnitMotionState(struct EvtUnit *, s32, s32, s32, s32);

extern void evtStoreUnitMotionShortParameters(struct EvtUnit *, s32, s32);

extern s32 btlGetSlotRateKind(u8 *, s32);

extern u8 *btlCreateStiffenDamageShakeTask(u8 *, f32);

s32 btlGetLoggedIndexedCommandItem(s32 index);

s32 btlGetSideIndexedActorStatusTable(s32 arg0, s32 arg1);

extern s32 btlCheckSpecialAbility(DatPartyRecord *, s32);

s32 btlIsUnitDefeatTriggeredByValueDelta(BtlUnit *actor, s32 delta);

/* Set the actor's selected entry index. */ void btlSetActorSelectedEntryIndex(BtlUnit *actor, u32 index);

void btlSetTrackedTaskDisplayMode(s32 mode);

void func_001A1960(DatPartyRecord *record, s32 mask);

void func_001A4C68(BtlUnit *unit, u32 flags, s16 delta);

typedef struct BtlStatArgs {
    BtlUnit *unit;
    s32 amount;
    s32 category;
} BtlStatArgs;

typedef struct BtlFxSrcA {
    f32 f0;
    f32 f4;
    f32 f8;
    f32 fC;
    f32 f10;
    f32 f14;
} BtlFxSrcA;

/* Shared effect-resource records also provide approach reach and frame limits. */
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
    u8 pad24[8];
    BtlEffectNode nodes[1];
} BtlEffectResource;

typedef struct BtlApproachTaskArgs {
    BtlUnit *unit;
    BtlUnit *target;
    f32 offset;
    f32 scale;
    s32 unk10;
    s32 count;
} BtlApproachTaskArgs;

typedef struct BtlRotationTaskArgs {
    s128 from;
    s128 to;
    f32 rate;
    f32 t;
    s32 count;
    BtlUnit *unit;
} BtlRotationTaskArgs;

/* Constructor-owned model-change arguments; btlAllocTask reserves 0x1C bytes. */
typedef struct BtlModelChangeArgs {
    BtlUnit *unit;
    u32 resourceKind;
    u32 resourceId;
    s32 delay;
    u32 duration;
    s32 elapsed;
    u8 phase;
    u8 transitionMode;
    u8 reserved1A[2];
} BtlModelChangeArgs;

typedef struct BtlFadeArgs {
    BtlUnit *unit;
    s32 fadeIn;
    s32 fadeOut;
    u32 count;
    u32 color;
} BtlFadeArgs;

extern s32 effOffsetIfOwnerFlagClear();

extern void sndFreeResourceNode(SoundResourceNode *);

extern void sndFreeListNode(ActiveSoundNode *);

extern s32 btlCountTasksByKind(u16 kind);

void *btlCreateActorParameterDeltaTask(BtlUnit *owner, BtlOperandEntry *spec);

extern s32 btlGetCommandTargetEligibility(BtlIndexList *indices, s32 selection);

void func_001D12A0(BtlTask *task, BattleIndexWork *work);

extern u8 *btlCreateModelChangeTask(u8 *, s32, s32, s32, s32, u8);

extern s32 btlCountTasksByKind(u16 kind);

extern BtlRuntimeTask *btlCreateModelLoadPollTask(BtlUnit *, u32, u32, s8);

extern u8 *btlCreateGunLoadPollTask(u8 *);

extern BtlRuntimeTask *btlCreateUnitPositionLerpTowardTargetTask(BtlUnit *, f32 *, f32);

extern BtlRuntimeTask *btlCreateUnitRotationInterpolationTask(BtlUnit *, f32 *, f32);

extern u8 *func_001D9038(u8 *, u32);

extern BtlRuntimeTask *btlCreateUnitBaseLightTask(BtlUnit *);

extern void btlResetUnitLinks(BtlUnit *);

extern void btlReleaseUnitResources(BtlUnit *);

extern s32 btlCountTasksForOwner(s64);


extern s32 btlCountTasksForOwner(s64);


extern u8 *btlCreateSelectedEffectUpdateTask(u8 *);

extern u8 *func_001D9468(u8 *, u32);

extern s32 btlGetRuntime(void);

extern void btlBossDebugPrintfN(s32, s32, s32, s32, ...);

extern char D_003BB5E0[];

extern char D_003BB5E8[];

BtlTask *btlFindUnitByActor(BtlUnit *target);

void btlDispatchCommandViaHookOrDefault(u8 *command, u8 *argument) {
    s32 (*handler)(s32, s32) = *(s32 (**)(s32, s32))(btlGetRuntime() + 0x604);

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
            if ((u32)((BtlTask *)command)->unit->status.flags & 0x200) {
                scrSetGlobalSeenBit(*(u16 *)(argument + 4));
            }
            btlDispatchStateHandler(command, 0xF);
        }
        break;
    case 5:
        if ((u32)((BtlTask *)command)->unit->status.flags & 0x1000) {
            btlDispatchStateHandler(command, 0x10);
        } else {
            btlDispatchStateHandler(command, 0x11);
        }
        break;
    case 10:
    case 13:
    case 14:
        btlDispatchStateHandler(command, 0x13);
        break;
    case 9:
        if ((u32)((BtlTask *)command)->indexWork.linkedUnit->status.flags & 1) {
            btlDispatchStateHandler(command, 0x16);
        } else {
            btlDispatchStateHandler(command, 0x15);
        }
        break;
    case 12:
        btlDispatchStateHandler(command, 0x15);
        break;
    case 6: {
        u32 flags = (u32)((BtlTask *)command)->unit->status.flags;
        if (flags & 0x200) {
            btlDispatchStateHandler(command, 0x17);
        } else if (flags & 0x400) {
            btlDispatchStateHandler(command, 0x14);
        }
        break;
    }
    case 11:
        btlDispatchStateHandler(command, 0x14);
        break;
    case 15:
        btlDispatchStateHandler(command, 0x18);
        break;
    case 16:
        btlDispatchStateHandler(command, 0x19);
        break;
    case 17:
        btlDispatchStateHandler(command, 0x1F);
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
    switch (argument[0]) {
    case 1:
        if ((btlUnitStatusPair(unit) & 0x1200) == 0x200) {
            return btlGetActorBedAssetIdFromIndex(unit->partyRecord.menuValue);
        }
        if (argument[1] > 0) {
            return argument[1];
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

u32 btlClassifyActionOperand(BtlUnit *actor, u8 *argument) {
    switch (*(s32 *)argument) {
    case 1: {
        u32 count = btlGetIndexListCount(*(struct BtlIndexList **)(argument + 0x40));
        if ((btlUnitStatusPair(actor) & 0x1200) == 0x1200 &&
            (*(u16 *)((u8 *)actor + 0x12E) & 0x1000) == 0 &&
            count == 1) {
            u8 *option = *(u8 **)(argument + 0x60);
            if (*(s32 *)(option + 0xC) == 2 && option[0x14] == 0) {
                return 0x17;
            }
        }
        return 3;
    }
    case 4:
        return ((u32)actor->status.flags & 0x200) ? 0xC : 4;
    case 2:
    case 3:
    case 7:
    case 8:
        return *(u8 *)(datActionAnimationRecords + *(s32 *)(argument + 4) * 0x20);
    default:
        return 0;
    }
}

s32 btlClassifyActionResult(BtlUnit *actor, u32 arg1, s32 arg2, u32 arg3, s32 arg4, u8 arg5, s32 commandIndex) {
    s32 resultCode;

    btlGetEntryFlagsUnlessDisabled(&actor->partyRecord);
    if (commandIndex >= 0) {
        switch ((u32)datCommandRecords[commandIndex].unk30) {
        case 1:
        case 2:
        case 9:
        case 10:
            return -1;
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
        switch (datCommandRecords[index].unk30) {
        case 1:
        case 2:
        case 9:
        case 10:
        case 12:
        case 13:
        case 14:
        case 15:
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
s32 func_001D1218(s32 unused, BattleIndexWork *state) {
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

extern void func_001AA130(BtlUnit *, BtlUnit *, BtlUnit *, BtlUnit *);

extern s32 func_001A6EC0(BtlUnit *, s32);

extern void btlDistributeRandomTargetHits(BtlUnit *, BtlIndexList *, BtlOperandGroup *, s32);

extern void func_001A6BE0(BtlUnit *, BtlIndexList *, s32);

extern s8 btlGetActorIndexedSignedValue(BtlUnit *, s32);

extern u32 btlEncodeActorIndexAsSelectionMask(u32);

extern s32 func_001A2DE8(BtlUnit *, BtlUnit *, BtlUnit *, BtlUnit *, s32);

extern s32 btlApplyCommandAbilityMultiplier(DatPartyRecord *, s32);

extern s32 btlGetCombinedPartyCommandPower(DatPartyRecord *, BtlUnit *, BtlUnit *, BtlUnit *, s32);

extern s32 func_001D6050(BtlUnit *, s32);

extern s32 btlSelectedEntryHitsElement(s32, BtlUnit *, s32);

extern s32 btlGetActionRecordLookupValue(s32);

extern s32 func_001A7548(BtlUnit *, BtlUnit *, s32);

extern u8 func_001A6968(BtlUnit *, s32);

extern s32 func_001A5958(BtlUnit *, BtlUnit *, s32);

extern s32 func_001A7180(BtlUnit *, BtlUnit *, s32, s32, s32);

extern s32 btlQueryUnitChannelFlags(s32, s32, s32, s32, s32);

extern s32 btlSelectActorAction(s32);

extern s32 btlCalculateHuntEpReward(u8 *, u8 *);

/* Native hunt interface passes actor and target; HP recovery is zero in this title. */
extern u32 func_001A7ED8(BtlUnit *actor, BtlUnit *target);

/* Native caller supplies actor,target; concrete provider consumes only actor. */
extern s32 btlCalculateAbilityRecoveryAmount(BtlUnit *actor, BtlUnit *target);

extern u32 btlGetHuntPenaltyFlags(BtlUnit *, BtlUnit *);

/* ASM-only definition; align the current pinned declaration in code_001A9780.c. */
extern u32 func_001A5C40(BtlUnit *, BtlUnit *, s32, s32, s32, s32, s32);

extern s32 func_001A6118(BtlUnit *, BtlUnit *, s32, s32, s32, s32, u32);

extern f32 btlGetActionCategoryMultiplier(BtlUnit *, s32, s32);

extern f32 btlGetActionCategoryGateAsFloat(s32, s32, s32);

/* Native resolver hook passes kind and press; both are unused by this title's no-op. */
extern void btlFindSoundTaskByWorkValue(s32 kind, s32 press);

extern s32 btlRollAllFearChance(s32, BtlUnit *, u32, s32, u8);

extern s32 btlRollFearChance(s32, u8 *, u32, u32);

extern s32 btlRollStoneDeathChance(BtlUnit *, s32);

extern s32 btlRollActorEligibilityWithAbilityOverride(BtlUnit *);

extern s32 btlCalculateEnemyExperienceReward(u8 *, u8 *);

extern s32 btlGetEnemyMoney(u8 *, u8 *);

extern u8 btlGetActorDisplayByteWithDefault(s32, s32);

extern s32 btlRollPassiveAbilityAction(BtlUnit *);

extern void fldAppendSceneGroupHandle(s32);

extern s16 btlGetCommandEffectId(BtlTask *, s32);

extern s32 btlActorEntryIsExpired(BtlUnit *, s32);

extern void btlClearActorEntrySlot(BtlUnit *, s32);

extern s32 btlCompareSkippedAndActiveTargetCounts(BtlIndexList *, BtlOperandGroup *);

extern u32 btlAdjustPointsForCombatFlags(u32, u32, u32, s32);

extern s32 btlGetCommandResultKindFromFlags(u32, u32);

void func_001D12A0(BtlTask *action, BattleIndexWork *work) {
    BtlUnit combinedUnit;
    BtlState *runtime;
    BtlUnit *source;
    BtlUnit *target;
    BtlUnit *recipient;
    BtlOperandGroup *group;
    s32 hpDelta;
    s32 huntHpRecovery;
    s32 huntMpRecovery;
    s32 cumulativeHp;
    s32 command;
    u32 costError;
    s32 helperResult;
    s32 counterCommand;
    s32 reflectedHp;
    s32 targetIndex;
    s32 hitIndex;
    u32 targetCount;
    u32 hitCount;
    s32 maxHits;
    u32 addedStatus;
    u32 removedStatus;
    u32 deferredStatus;
    u32 operandFlags;
    s32 press;
    s32 emptyHits;
    s32 fearCount;
    u32 combinedKinds;
    u32 combinedPress;
    u32 element;
    u32 elementMask;
    s32 entryChangeMask;
    u32 calculationFlags;
    s32 entryChange;
    s32 absorbed;
    s32 randomTargets;
    s32 fearPending;
    s32 allFear;
    s32 allEmpty;
    s32 overrideMiss;
    s32 reflectedSpecial;
    s32 huntRecovered;
    u32 kind;
    f32 ratio;

    btlBossDebugPrintf("btl:act[%p][%p][%p]\n", action->unit,
                      action->indexWork.companionA, action->indexWork.companionB);
    runtime = (BtlState *)btlGetRuntime();
    action->options &= ~2;
    command = btlResolveActionOperand(action->unit, &work->phase);
    if (command == -1) {
        return;
    }
    reflectedHp = 0;
    calculationFlags = 0;
    if (work->phase == 4) {
        calculationFlags = work->reference != -1;
    }
    if (datCommandSelectors[command].kind != 1) {
        source = action->unit;
    } else {
        func_001AA130(&combinedUnit, action->unit, action->indexWork.companionA,
                     action->indexWork.companionB);
        source = &combinedUnit;
    }
    group = work->groups;
    randomTargets = 0;
    if (func_001A6EC0(action->unit, command)) {
        randomTargets = 1;
        btlDistributeRandomTargetHits(action->unit, work->indices, group, command);
    }
    func_001A6BE0(action->unit, work->indices, command);
    targetCount = btlGetIndexListCount(work->indices);
    btlBossDebugPrintf("btl:skillID=%X\n", command);
    element = btlGetActorIndexedSignedValue(source, command);
    elementMask = btlEncodeActorIndexAsSelectionMask(element);
    if (action->indexWork.phase != 4) {
        if (datCommandSelectors[command].kind != 1) {
            costError = btlGetCommandFailureReason(source, command);
        } else {
            costError = func_001A2DE8(source, action->unit, action->indexWork.companionA,
                                    action->indexWork.companionB, command);
        }
        if (costError == 0) {
            if (datCommandSelectors[command].kind != 1) {
                work->stageValue = btlApplyCommandAbilityMultiplier(&source->partyRecord, command);
            } else {
                work->stageValue = btlGetCombinedPartyCommandPower(&source->partyRecord,
                    action->unit, action->indexWork.companionA, action->indexWork.companionB, command);
            }
            action->unit->status.stateFlags &= ~0x1000;
            btlBossDebugPrintf("btl:cost=%d\n", work->stageValue);
        } else {
            btlBossDebugPrintf("btl:cost error[%d][hp=%d,mp=%d]\n", costError,
                              source->partyRecord.hp, source->partyRecord.mp);
            work->unk2D = 1;
            switch (costError) {
            case 1:
                work->stage = 3;
                work->parameter = effOffsetIfOwnerFlagClear(source, 0x14);
                action->unit->status.stateFlags |= 0x1000;
                break;
            case 2:
                work->stage = 3;
                work->parameter = effOffsetIfOwnerFlagClear(source, 0x12);
                action->unit->status.stateFlags |= 0x1000;
                break;
            case 3:
                work->stage = 3;
                work->parameter = effOffsetIfOwnerFlagClear(source, 0x80);
                break;
            }
        }
    }
    helperResult = func_001D6050(action->unit, ((BtlActionAnimationRecord *)datActionAnimationRecords)[command].cameraKind);
    if (datCommandRecords[command].unk2E != 0) {
        counterCommand = command;
        btlBossDebugPrintf("btl:counter=%d\n", command);
    } else {
        counterCommand = 0;
    }
    if (command == 0) {
        work->unk20 = 8;
    } else {
        if (helperResult >= 21) {
            work->unk20 = helperResult;
        } else {
            work->unk20 = 20;
        }
        work->unk20 -= 8;
        btlBossDebugPrintf("btl:precede=%d\n", work->unk20);
    }
    combinedKinds = 0;
    combinedPress = 0;
    fearPending = 0;
    allFear = 0;
    overrideMiss = 0;
    allEmpty = 1;
    reflectedSpecial = 0;
    huntRecovered = 0;
    fearCount = 0;
    for (targetIndex = 0; targetIndex < targetCount; targetIndex++, group++) {
        huntHpRecovery = 0;
        huntMpRecovery = 0;
        deferredStatus = 0;
        emptyHits = 0;
        ratio = 1.0f;
        operandFlags = 0;
        absorbed = 0;
        switch (element) {
        case 1:
            operandFlags = 8;
            break;
        case 0:
            break;
        case 6:
            operandFlags = 0x100;
        default:
            operandFlags |= 4;
            break;
        }
        target = btlGetIndexListEntry(work->indices, targetIndex);
        btlBossDebugPrintf("btl:target=%p\n", target);
        if (btlSelectedEntryHitsElement((s32)source, target, command)) {
            kind = btlGetActionRecordLookupValue(target->selectedEntryIndex);
            group->targetSpecialHit = 1;
        } else {
            kind = func_001A7548(source, target, command);
            if (runtime->actionHitOverride != NULL) {
                helperResult = runtime->actionHitOverride(source, target, command, kind);
                if (helperResult != 0) {
                    kind = helperResult;
                    if (kind & 4) {
                        overrideMiss = 1;
                    }
                }
            }
            group->targetSpecialHit = 0;
        }
        {
            s32 initialReflection = kind == 0x20000;
            hitCount = 1;
            maxHits = 1;
            if (!initialReflection) {
                if (randomTargets) {
                    hitCount = group->count;
                } else {
                    hitCount = func_001A6968(source, command);
                }
                maxHits = datCommandRecords[command].rangeMax;
            }
        }
        if (kind == 0x20000) {
            recipient = action->unit;
            if (btlCheckSpecialAbility(&target->partyRecord, 0x218)) {
                ratio *= datAbilityParameters[0x18].value;
            } else if (btlCheckSpecialAbility(&target->partyRecord, 0x217)) {
                ratio *= datAbilityParameters[0x17].value;
            }
            if (btlSelectedEntryHitsElement((s32)action->unit, recipient, command)) {
                kind = btlGetActionRecordLookupValue(recipient->selectedEntryIndex);
                group->sourceSpecialHit = 1;
            } else {
                kind = func_001A7548(action->unit, recipient, command);
                group->sourceSpecialHit = 0;
            }
            if (datCommandRecords[command].flags & 2) {
                kind = 4;
            } else if (runtime->unk_1FC & 0x10) {
                goto normalReflectedHit;
            } else {
                switch (kind) {
                case 0x20000:
                    kind = 0x10000;
                    break;
                case 2:
                normalReflectedHit:
                    kind = 1;
                    break;
                }
            }
            if (runtime->reflectedHitOverride != NULL) {
                s32 result = runtime->reflectedHitOverride(action->unit, recipient, command);
                if (result != 0) {
                    kind = result;
                }
            }
            if (reflectedSpecial) {
                kind = 4;
                operandFlags |= 0x1000;
            } else if (datCommandRecords[command].hpType == 8 || datCommandRecords[command].hpType == 10) {
                reflectedSpecial = 1;
            }
            combinedKinds |= 0x20000;
            group->reflected = 1;
            btlBossDebugPrintf("btl:HANSYA\n");
        } else {
            group->sourceSpecialHit = 0;
            recipient = target;
            group->reflected = 0;
        }
        if (kind == 0x40000) {
            if (btlCheckSpecialAbility(&recipient->partyRecord, 0x23B)) {
                ratio *= datAbilityParameters[0x3B].value;
            }
            absorbed = 1;
        }
        press = 1;
        if (!group->reflected) {
            switch (kind) {
            case 2:
            case 4:
            case 0x10000:
            case 0x20000:
            case 0x40000:
                press = 1;
                break;
            default:
                press = func_001A5958(source, recipient, command);
                if (action->options & 1) {
                    s32 adjustedPress = 2;
                    if (press == 4) {
                        adjustedPress = press;
                    }
                    press = adjustedPress;
                }
                break;
            }
        }
        addedStatus = func_001A7180(source, recipient, command, press, (s32)kind);
        removedStatus = btlQueryUnitChannelFlags((s32)source, (s32)recipient, command, press, (s32)kind);
        if (group->reflected) {
            addedStatus &= ~6;
        }
        if ((source->status.flags & 0x600) == (recipient->status.flags & 0x600)) {
            addedStatus &= ~6;
        }
        if (action->flags & 0x40) {
            addedStatus = 0;
        }
        if (addedStatus & ~removedStatus & 1) {
            if (!(recipient->status.stateFlags & 8)) {
                fearPending = 1;
            } else {
                addedStatus &= ~1;
            }
        }
        if (datCommandRecords[command].unk30 == 10) {
            work->flags = btlSelectActorAction((s32)target);
            target->status.flags |= 0x4000000;
        }
        switch ((u32)datCommandRecords[command].unk30) {
        case 12: operandFlags |= 0x10; break;
        case 13: operandFlags |= 0x20; addedStatus = 0x4000; break;
        case 14: operandFlags |= 0x40; break;
        case 15: operandFlags |= 0x80; break;
        }
        if (datCommandRecords[command].effectType == 2) {
            if (kind == 1) {
                operandFlags |= 1;
                addedStatus = 0x4000;
                work->unk50 += btlCalculateHuntEpReward((u8 *)action->unit, (u8 *)recipient);
                work->stage = 5;
                work->parameter = effOffsetIfOwnerFlagClear(target, 0x9C);
                if (!huntRecovered) {
                    huntRecovered = 1;
                    huntHpRecovery = func_001A7ED8(action->unit, target);
                    huntMpRecovery = btlCalculateAbilityRecoveryAmount(action->unit, target);
                }
                switch (btlGetHuntPenaltyFlags(action->unit, target)) {
                case 16: deferredStatus = 0x40; break;
                case 8: deferredStatus = 0x80; break;
                }
            } else {
                if (kind != 0x10000) {
                    kind = 4;
                }
                if (datCommandRecords[command].targetType == 0) {
                    work->stage = 5;
                    work->parameter = effOffsetIfOwnerFlagClear(target, 0x9E);
                }
            }
            recipient->status.flags |= 0x1000000;
        }
        if (kind == 2 && targetCount == 1 && work->stage == 0) {
            work->stage = 1;
            work->parameter = effOffsetIfOwnerFlagClear(target, 10);
        }
        group->kind = kind;
        group->statusChanged = (addedStatus & ~removedStatus) != 0;
        cumulativeHp = 0;
        group->parameter = press;
        group->inactive = 0;
        if (addedStatus == 0x4000) {
            group->inactive = 1;
        }
        if (!(kind & 6)) {
            entryChangeMask = datCommandRecords[command].requirementBits;
            entryChange = (u16)(s8)datCommandRecords[command].unk_2C;
        } else {
            entryChangeMask = 0;
            entryChange = 0;
        }
        for (hitIndex = 0; hitIndex < hitCount; hitIndex++) {
            s32 hpAttack;
            s32 hpRestore;
            s32 mpAttack;
            s32 mpRestore;
            s32 mpDelta;
            f32 multiplier;

            hpAttack = (s32)func_001A5C40(source, recipient, command, maxHits, press, 1, (s32)calculationFlags);
            hpRestore = (s32)func_001A5C40(source, recipient, command, maxHits, press, 0, (s32)calculationFlags);
            hpAttack = hpAttack * ratio;
            mpAttack = func_001A6118(source, recipient, command, maxHits, press, 1, calculationFlags);
            mpRestore = func_001A6118(source, recipient, command, maxHits, press, 0, calculationFlags);
            group->entries[hitIndex].addedStatus = addedStatus;
            group->entries[hitIndex].deferredStatus = deferredStatus;
            group->entries[hitIndex].removedStatus = removedStatus;
            if (kind == 0x40000) {
                hpRestore = hpAttack;
                hpAttack = -1;
                mpRestore = mpAttack;
                mpAttack = -1;
            } else if (kind == 0x10000 || kind == 2 || kind == 4) {
                hpAttack = -1;
                mpAttack = -1;
                hpRestore = -1;
                mpRestore = -1;
            }
            if (hpAttack > 0) {
                multiplier = btlGetActionCategoryMultiplier(source, (s32)recipient, command);
            } else {
                multiplier = 0.0f;
            }
            group->entries[hitIndex].hpRecovery = (s32)(hpAttack * multiplier);
            group->entries[hitIndex].hpRecovery += huntHpRecovery;
            if (mpAttack > 0) {
                multiplier = btlGetActionCategoryGateAsFloat((s32)source, (s32)recipient, command);
            } else {
                multiplier = 0.0f;
            }
            group->entries[hitIndex].mpRecovery = (s32)(mpAttack * multiplier);
            group->entries[hitIndex].mpRecovery += huntMpRecovery;
            group->entries[hitIndex].entryChangeMask = entryChangeMask;
            group->entries[hitIndex].entryChange = entryChange;
            group->entries[hitIndex].entrySelection = counterCommand;
            hpDelta = 0;
            if (hpAttack * hpRestore <= 0) {
                hpDelta = hpAttack > 0 ? -hpAttack : hpRestore;
            }
            if (mpAttack * mpRestore > 0) {
                mpDelta = 0;
            } else {
                mpDelta = mpAttack > 0 ? -mpAttack : mpRestore;
            }
            group->entries[hitIndex].hpDelta = hpDelta;
            group->entries[hitIndex].mpDelta = mpDelta;
            cumulativeHp += hpDelta;
            btlBossDebugPrintf("btl:[%d,%d]hp=%d,mp=%d,badD=%X,badR=%X[ratio=%.2f]\n",
                targetIndex, hitIndex, hpDelta, mpDelta, addedStatus, removedStatus, ratio);
            btlFindSoundTaskByWorkValue(kind, press);
            if (group->reflected) {
                reflectedHp += hpDelta;
            }
            group->entries[hitIndex].flags = operandFlags;
            if ((source->status.flags & 0x400) && (target->status.flags & 0x200)) {
                if (btlRollAllFearChance((s32)source, target, kind, press, group->targetSpecialHit)) {
                    work->unk5E = 1;
                    allFear = 1;
                }
            }
            if (!allFear && !btlIsUnitDefeatTriggeredByValueDelta(recipient, cumulativeHp) &&
                (source->status.flags & 0x200) && (recipient->status.flags & 0x400) &&
                cumulativeHp < 0 && kind == 1 && !group->inactive && !group->reflected &&
                !group->targetSpecialHit && !absorbed && addedStatus == 0 &&
                !(datEnemyRecords[recipient->partyRecord.unitId].flags & 0x440)) {
                if (btlRollFearChance((s32)source, (u8 *)recipient, 1, (u32)press)) {
                    group->entries[hitIndex].addedStatus = 1;
                    addedStatus = 1;
                    fearPending = 1;
                    fearCount++;
                }
            }
            if (btlIsUnitDefeatTriggeredByValueDelta(recipient, cumulativeHp)) {
                hitCount = hitIndex + 1;
                if ((recipient->status.stateFlags & 0x20) ||
                    (command == 0 && btlCheckSpecialAbility(&source->partyRecord, 0x24B) && recipient->partyRecord.hp >= 2)) {
                    group->entries[hitIndex].hpDelta = -recipient->partyRecord.hp - (cumulativeHp - hpDelta) + 1;
                } else {
                    fearPending = 0;
                    group->inactive = 1;
                    allEmpty = 0;
                }
                break;
            }
            if (kind == 1 && btlRollStoneDeathChance(recipient, command)) {
                hitCount = hitIndex + 1;
                fearPending = 0;
                allEmpty = 0;
                group->entries[hitIndex].flags |= 2;
                group->inactive = 1;
                break;
            }
            if (!btlActionEntryIsEmpty(command, group, &group->entries[hitIndex])) {
                allEmpty = 0;
            } else {
                emptyHits++;
            }
            if (kind == 4) {
                group->entries[hitIndex].flags |= 0x800;
            }
        }
        if (emptyHits == hitCount) {
            press = 1;
            btlBossDebugPrintf("btl:press miss\n");
        }
        combinedKinds |= kind;
        combinedPress |= press;
        if (group->inactive || (group->reflected && btlIsUnitDefeatTriggeredByValueDelta(recipient, reflectedHp))) {
            if (!(recipient->partyRecord.status & 0x7C0E) &&
                (btlUnitStatusPair(recipient) & 0x4000001200) == 0x1200 &&
                btlRollActorEligibilityWithAbilityOverride(recipient)) {
                recipient->status.stateFlags |= 0x40;
            }
        }
        if (datCommandRecords[command].effectType != 2 && (datCommandRecords[command].flags & 0x20)) {
            if (group->inactive && (recipient->status.flags & 0x400)) {
                operandFlags |= 1;
                group->entries[hitCount - 1].flags |= 1;
                group->entries[hitCount - 1].addedStatus = 0x4000;
                work->unk50 += btlCalculateHuntEpReward((u8 *)action->unit, (u8 *)recipient);
                work->stage = 5;
                work->parameter = effOffsetIfOwnerFlagClear(target, 0x9C);
                if (!huntRecovered) {
                    huntRecovered = 1;
                    group->entries[hitCount - 1].hpRecovery += func_001A7ED8(action->unit, target);
                    group->entries[hitCount - 1].mpRecovery += btlCalculateAbilityRecoveryAmount(action->unit, target);
                }
                switch (btlGetHuntPenaltyFlags(action->unit, target)) {
                case 16: group->entries[hitCount - 1].deferredStatus |= 0x40; break;
                case 8: group->entries[hitCount - 1].deferredStatus |= 0x80; break;
                }
            }
            recipient->status.flags |= 0x1000000;
        }
        if (group->inactive && (action->unit->status.flags & 0x200) && (recipient->status.flags & 0x400)) {
            if (!(operandFlags & 1)) {
                work->unk54 += btlCalculateEnemyExperienceReward((u8 *)action->unit, (u8 *)recipient);
                recipient->status.stateFlags |= 2;
            }
            work->unk58 += btlGetEnemyMoney((u8 *)action->unit, (u8 *)recipient);
            runtime->unk26E += recipient->partyRecord.level;
            runtime->unk26C++;
            recipient->status.stateFlags |= 0x400;
        }
        work->groups[targetIndex].interval = btlGetActorDisplayByteWithDefault((s32)source, command);
        work->groups[targetIndex].count = hitCount;
        if (element < 2 &&
            kind == 1 && !group->inactive && !group->reflected && !group->targetSpecialHit &&
            !absorbed && !(action->flags & 0x40) && !(target->partyRecord.status & 0x90F) &&
            !(addedStatus & 0x90F)) {
            s32 counter = btlRollPassiveAbilityAction(target);
            if (counter > 0) {
                BtlTask *reaction = btlFindUnitByActor(target);
                fldAppendSceneGroupHandle((s32)reaction);
                reaction->indexWork.phase = 1;
                reaction->indexWork.skillId = counter;
                btlAppendIndexListEntry(reaction->indexWork.indices, action->unit);
                btlBossDebugPrintf("btl:counter[%p][%X]\n", target, counter);
            }
        }
        if (press == 2) {
            action->options |= 2;
        }
        group->reactionCode = btlClassifyActionResult(recipient, kind, press, elementMask,
                                                     hitCount, group->inactive, command);
    }
    if (fearPending && !allFear) {
        work->wait = fearCount == 1 ? 0 : 2;
    }
    if (!allEmpty && work->stage == 0) {
        s32 effect = btlGetCommandEffectId(action, command);
        if (effect > 0) {
            work->parameter = effect;
            work->stage = 1;
        }
    }
    if (datCommandRecords[command].effectType == 0 && !(calculationFlags & 1) &&
        datCommandSelectors[command].kind != 1 && !(u8)(datCommandRecords[command].flags & 1)) {
        if (btlActorEntryIsExpired(action->unit, 5)) {
            btlClearActorEntrySlot(action->unit, 5);
        }
    }
    if (datCommandRecords[command].effectType == 1 && !(calculationFlags & 1) &&
        datCommandSelectors[command].kind != 1 && !(u8)(datCommandRecords[command].flags & 1)) {
        if (btlActorEntryIsExpired(action->unit, 6)) {
            btlClearActorEntrySlot(action->unit, 6);
        }
    }
    if (btlCompareSkippedAndActiveTargetCounts(work->indices, work->groups)) {
        work->unk2E = 1;
    }
    work->slot = btlClassifyActionOperand(action->unit, (u8 *)work);
    {
        s32 points;
        if (runtime->actionPointsOverride != NULL) {
            points = runtime->actionPointsOverride(combinedKinds, combinedPress, command);
        } else {
            points = 100;
        }
        if ((u32)(command - 0x1AB) < 0x55) {
            points *= datAffinityRecords[command - 0x1AB].slotCost;
        }
        if (!work->unk2D && (!allEmpty || (combinedKinds & 0x60000))) {
            work->adjustedValue = btlAdjustPointsForCombatFlags(combinedKinds, combinedPress, points, command);
            work->resultKind = btlGetCommandResultKindFromFlags(combinedKinds, combinedPress);
        } else {
            if (overrideMiss) {
                points += 100;
            }
            work->resultKind = 1;
            work->adjustedValue = points;
        }
    }
    btlBossDebugPrintf("btl:press=%d[type=%d]\n", work->adjustedValue, (u8)work->resultKind);
}

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
    work->unk5E = 0;
    for (i = 0; i < 13; i++) {
        work->groups[i].count = 0;
        work->groups[i].kind = 0;
        work->groups[i].reflected = 0;
        work->groups[i].inactive = 0;
    }
    btlClearIndexList(work->indices);
}

/* Allocate the index list and retained groups, then initialize their headers. */
void btlInitBattleIndexWork(BattleIndexWork *object) {
    struct SdfMemBlock *allocation;
    u32 value;
    object->indices = btlAllocateIndexList(13);
    allocation = sdfAllocGeneralBlock(0x836C);
    value = sdfResourceRetainAddress(allocation);
    object->allocationHandle = allocation;
    object->groups = (BtlOperandGroup *)value;
    object->ownerId = 0;
    btlResetIndexWork(object);
}

/* Release each owned buffer once. The cached group address is deliberately not cleared. */
void btlReleaseObjectBuffers(BattleIndexWork *object) {
    struct SdfMemBlock *handle = object->allocationHandle;
    if (handle != 0) {
        sdfReleaseResourceAllocation(handle);
        object->allocationHandle = 0;
    }
    if (object->indices != 0) {
        btlFreeIndexList(object->indices);
        object->indices = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001D0BA0", func_001D2C78);

extern u32 func_001D2C78(void *);

extern void *btlAllocTask(s32);

void *btlCreateActorParameterDeltaTask(BtlUnit *owner, BtlOperandEntry *spec) {
    BtlRuntimeTask *task = btlAllocTask(0x2C);
    BtlOperandTaskArgs *arguments;

    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x45;
    task->callback = func_001D2C78;
    task->ownerId = owner->identity;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = owner;
    memcpy(&arguments->operand, spec, sizeof(*spec));
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

void *btlCreateDeferredActorStatsTask(BtlUnit *owner, BtlOperandEntry *spec) {
    BtlRuntimeTask *task = btlAllocTask(0x2C);
    BtlOperandTaskArgs *arguments;

    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x46;
    task->callback = btlApplyDeferredActorStats;
    task->ownerId = owner->identity;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = owner;
    memcpy(&arguments->operand, spec, sizeof(*spec));
    return task;
}

u32 btlApplyDeferredUnitStatus(void *argument) {
    u32 *args = (u32 *)argument;
    s32 context = btlGetRuntime();
    BtlUnit *owner = (BtlUnit *)args[0];
    if ((*(u32 *)(context + 0x1F4) & 0x80) == 0) {
        return 1;
    }
    func_001A1948(&owner->partyRecord, args[1]);
    btlRefreshUnitMotionSelection(owner);
    btlIsUnitDefeatTriggeredByValueDelta(owner, 0);
    return 1;
}

extern void *btlAllocTask(s32);

extern u32 btlApplyDeferredUnitStatus(void *);

void *btlCreateDeferredUnitStatusTask(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(8);
    s64 data;
    u32 *arguments;

    task[0x10] = BTL_TASK_CONDITION_NEVER;
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x47;
    *(void **)(task + 0x4C) = btlApplyDeferredUnitStatus;
    data = *(s64 *)(owner + 0x108);
    *(s64 *)(task + 0x40) = data;
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    return task;
}

u32 btlApplyCategoryStatDamage(BtlStatArgs *args) {
    s32 context = btlGetRuntime();
    BtlUnit *unit = args->unit;
    if ((*(u32 *)(context + 0x1F4) & 0x80) == 0) {
        return 1;
    }
    if (datCommandRecords[args->category].flags & 8) {
        btlAdjustUnitHp(&unit->partyRecord, -0x7FFF);
        func_001A1948(&unit->partyRecord, 0x4000);
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

extern u32 btlApplyCategoryStatDamage(BtlStatArgs *);

void *btlCreateCategoryStatDamageTask(u8 *owner, u32 value, u32 extra) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)(task + 0x4C) = btlApplyCategoryStatDamage;
    *(u16 *)(task + 0x20) = 0x48;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[2] = value;
    arguments[1] = extra;
    return task;
}

s32 func_001D33D0(BtlOperandTaskArgs *args) {
    func_001A4C68(args->unit, args->operand.entryChangeMask, args->operand.entryChange);
    return 1;
}

void *func_001D3400(BtlUnit *owner, BtlOperandEntry *spec) {
    BtlRuntimeTask *task = btlAllocTask(0x2C);
    BtlOperandTaskArgs *arguments;

    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x49;
    task->callback = func_001D33D0;
    task->ownerId = owner->identity;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = owner;
    memcpy(&arguments->operand, spec, sizeof(*spec));
    return task;
}

u32 btlApplyQueuedActorEntrySelection(BtlOperandTaskArgs *arg0) {
    if (0 < arg0->operand.entrySelection) {
        btlSetActorSelectedEntryIndex(arg0->unit, arg0->operand.entrySelection);
        btlRefreshUnitMotionSelection(arg0->unit);
    }
    return 1;
}

void *func_001D3510(BtlUnit *owner, BtlOperandEntry *spec) {
    BtlRuntimeTask *task = btlAllocTask(0x2C);
    BtlOperandTaskArgs *arguments;

    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x4A;
    task->callback = btlApplyQueuedActorEntrySelection;
    task->ownerId = owner->identity;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = owner;
    memcpy(&arguments->operand, spec, sizeof(*spec));
    return task;
}

u32 btlClearQueuedActorEntrySelection(BtlUnit **arg0) {
    btlClearActorSelectedEntryIndex(*arg0);
    btlRefreshUnitMotionSelection(*arg0);
    return 1;
}

void *func_001D3618(BtlUnit *owner) {
    BtlRuntimeTask *task = btlAllocTask(4);
    BtlUnit **arguments;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlClearQueuedActorEntrySelection;
    task->taskId = 0x4B;
    task->ownerId = owner->identity;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    *arguments = owner;
    return task;
}

/* Complete eight-byte argument allocation owned by the hunt-EP task. */
typedef struct BtlHuntExpArgs {
    BtlUnit *actor;
    u32 amount;
} BtlHuntExpArgs;

typedef char BtlHuntExpArgsSizeCheck[sizeof(BtlHuntExpArgs) == 8 ? 1 : -1];

extern DatPartyRecord *btlGetIndexedPartyEntryRecord(s32);

u32 func_001D3688(s32 address) {
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
    if (btlCheckSpecialAbility(&args->actor->partyRecord, 0x24C)) {
        count = 0;
        record = btlGetIndexedPartyEntryRecord(args->actor->unk2C4);
        record->huntExp += args->amount;
        head = battle->units;
        for (unit = head; unit != NULL; unit = unit->next) {
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
        for (unit = head; unit != NULL; unit = unit->next) {
            u32 flags = unit->status.flags;
            if (flags & 0x200) {
                if (flags & 1) {
                    if (args->actor != unit && !(flags & 0x20) && !(unit->partyRecord.status & 0x40)) {
                        record = btlGetIndexedPartyEntryRecord(unit->unk2C4);
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
        record = btlGetIndexedPartyEntryRecord(args->actor->unk2C4);
        record->huntExp += args->amount;
        btlBossDebugPrintf("btl:hunt ep=%d[%p]\n", args->amount, record);
    }
    return 1;
}

extern u32 func_001D3688(s32);

u8 *btlCreateActorSoundOptionTask(BtlUnit *arg0, s32 arg1) {
    u8 *task = btlAllocTask(8);
    BtlHuntExpArgs *data;

    task[0x10] = BTL_TASK_CONDITION_NEVER;
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x4C;
    *(void **)(task + 0x4C) = func_001D3688;
    *(u64 *)(task + 0x40) = arg0->identity;
    *(s32 *)(task + 0x48) = 0;
    data = btlGetTaskArguments(task);
    data->actor = arg0;
    data->amount = (u32)arg1;
    return task;
}

u32 func_001D3A20(s32 arg0) {
    if ((datItemSkillRecords[*(s32 *)(arg0 + 4)].unk01 & 4) != 0) {
        ptyAdjustItemQuantity(*(s32 *)(arg0 + 4), 0xffffffffffffffff);
    }
    return 1;
}

u8 *btlCreatePermittedBattleVoiceTask(s32 arg0, s32 arg1) {
    u8 *task = btlAllocTask(8);
    u32 *data;

    task[0x10] = BTL_TASK_CONDITION_NEVER;
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x4D;
    *(void **)(task + 0x4C) = func_001D3A20;
    *(u64 *)(task + 0x40) = *(u64 *)(arg0 + 0x108);
    *(s32 *)(task + 0x48) = 0;
    data = btlGetTaskArguments(task);
    data[0] = (u32)arg0;
    data[1] = (u32)arg1;
    return task;
}

u32 btlPlayQueuedBattleVoice(s32 arg0) {
    ptyAdjustItemQuantity(*(u16 *)(arg0 + 4), 1);
    return 1;
}

u8 *btlCreateQueuedBattleVoiceTask(u8 *arg0, u16 arg1) {
    u8 *task = btlAllocTask(8);
    u32 *data;

    task[0x10] = BTL_TASK_CONDITION_NEVER;
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x4E;
    *(void **)(task + 0x4C) = btlPlayQueuedBattleVoice;
    *(u64 *)(task + 0x40) = *(u64 *)(arg0 + 0x108);
    *(s32 *)(task + 0x48) = 0;
    data = btlGetTaskArguments(task);
    data[0] = (u32)arg0;
    ((u16 *)data)[2] = arg1;
    return task;
}

u32 btlAddEpFromPacket(s32 arg0) {
    s32 context = btlGetRuntime();

    if (((s32 *)arg0)[1] == 0) {
        return 1;
    }
    if ((u32)((BtlUnit *)((u32 *)arg0)[0])->status.flags & 0x400) {
        return 1;
    }
    *(s32 *)(context + 0x2CC) += ((s32 *)arg0)[1];
    btlBossDebugPrintf("btl:epall=%d[%d](packet)\n", *(s32 *)(context + 0x2CC), ((s32 *)arg0)[1]);
    return 1;
}

extern u32 btlAddEpFromPacket(s32);

u8 *btlScheduleEpPacketTask(u8 *owner, s32 value) {
    u8 *task = btlAllocTask(8);
    u32 *arguments;

    task[0x10] = BTL_TASK_CONDITION_NEVER;
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x4F;
    *(void **)(task + 0x4C) = btlAddEpFromPacket;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    return task;
}

u32 btlAddMoneyFromPacket(void *arg0) {
    s32 context = btlGetRuntime();

    if (((s32 *)arg0)[1] == 0) {
        return 1;
    }
    if ((u32)((BtlUnit *)((u32 *)arg0)[0])->status.flags & 0x400) {
        return 1;
    }
    *(s32 *)(context + 0x2C0) += ((s32 *)arg0)[1];
    btlBossDebugPrintf("btl:money=%d[%d](packet)\n", *(s32 *)(context + 0x2C0), ((s32 *)arg0)[1]);
    return 1;
}

extern u32 btlAddMoneyFromPacket(void *);

void *btlScheduleMoneyPacketTask(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(8);
    s64 data;
    u32 *arguments;

    task[0x10] = BTL_TASK_CONDITION_NEVER;
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x50;
    *(void **)(task + 0x4C) = btlAddMoneyFromPacket;
    data = *(s64 *)(owner + 0x108);
    *(s64 *)(task + 0x40) = data;
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    return task;
}

u32 btlRefreshEligibleActors(void) {
    u8 *context = (u8 *)btlGetRuntime();
    BtlUnit *actor = *(BtlUnit **)(context + 0x228);
    while (actor != 0) {
        u32 flags = (u32)actor->status.flags;
        if (flags & 0x400) {
            if (flags & 1) {
                if ((flags & 0xE0) == 0 &&
                    (u16)(*(u16 *)((u8 *)actor + 0x124) - 1) < 0x17F) {
                    u32 entry = datEnemyRecords[*(u16 *)((u8 *)actor + 0x124)].flags;
                    if ((entry & 0x40) == 0) {
                        if ((entry & 0x400) == 0) {
                            if ((actor->status.stateFlags & 8) == 0) {
                                u16 prior = *(u16 *)((u8 *)actor + 0x12E);
                                func_001A1948(&actor->partyRecord, 1);
                                btlRefreshUnitMotionSelection(actor);
                                if (*(u16 *)((u8 *)actor + 0x12E) == 1 &&
                                    prior != *(u16 *)((u8 *)actor + 0x12E)) {
                                    actor->status.stateFlags |= 4;
                                    *(u32 *)(context + 0x1F8) |= 0x100;
                                }
                            }
                        }
                    }
                }
            }
        }
        actor = *(BtlUnit **)((u8 *)actor + 0x344);
    }
    return 1;
}

void *btlCreateRefreshEligibleActorsTask(void) {
    u8 *task = btlAllocTask(0);
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(void **)(task + 0x4C) = btlRefreshEligibleActors;
    *(u16 *)(task + 0x20) = 0x51;
    *(u32 *)(task + 0x48) = 0;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    return task;
}

extern char D_003A3A40[];

extern char D_003A3A50[];

void btlUpdateAutoMusic(void) {
    u8 *context = (u8 *)btlGetRuntime();
    u32 flags = *(u32 *)(context + 0x1F4);
    if ((flags & 0x100000) == 0 || (flags & 0x6000000) == 0x6000000 ||
        (flags & 0x800) != 0) {
        return;
    }
    if (flags & 0x8000) {
        if (D_00324510.edge22 < 0 || D_00324510.edge23 < 0) {
            *(u32 *)(context + 0x1F4) = flags & ~0x8000;
            sndSetSequenceVolumePan(6, 0x7F, 0x3F);
            btlSetTrackedTaskDisplayMode(0);
            btlBossDebugPrintf(D_003A3A40);
        }
    } else if (D_00324510.edge22 < 0) {
        *(u32 *)(context + 0x1F4) = flags | 0x8000;
        sndSetSequenceVolumePan(5, 0x7F, 0x3F);
        btlSetTrackedTaskDisplayMode(1);
        btlBossDebugPrintf(D_003A3A50);
    }
}

void btlPrepareSavedCommandTargets(BtlTask *task, BattleIndexWork *work) {
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
        if ((s16)unit->partyRecord.unk18C == 10) {
            work->phase = (s16)unit->partyRecord.unk18C;
            return;
        }
        if (((s16)unit->partyRecord.unk18C == 2 ||
             (s16)unit->partyRecord.unk18C == 7) &&
            (btlUnitStatusPair(unit) & 0x1400)) {
            if ((s16)unit->partyRecord.unk190 == 0) {
                work->phase = 1;
            } else if ((unit->status.flags & 0x400) || (battle->battleFlags & 0x1000000) ||
                       btlCheckSpecialAbility(&unit->partyRecord, (s16)unit->partyRecord.unk190)) {
                work->phase = (s16)unit->partyRecord.unk18C;
                work->skillId = (s16)unit->partyRecord.unk190;
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
        if ((u32)work->skillId >= 0x260) {
            work->skillId = 0;
            work->phase = 1;
        }
        if (btlGetCommandFailureReason(unit, work->skillId)) {
            work->skillId = 0;
            work->phase = 1;
        }
        unit->partyRecord.unk18C = work->phase;
        unit->partyRecord.unk190 = work->skillId;
        list = btlAllocateIndexList(13);
        kind = func_001A3360(task, list, 0);
        reason = btlGetCommandTargetEligibility(list, work->skillId);
        if (reason == 5 || reason == 8) {
            work->skillId = 0;
            work->phase = 1;
            unit->partyRecord.unk190 = work->skillId;
            unit->partyRecord.unk18C = work->phase;
            btlClearIndexList(list);
            kind = func_001A3360(task, list, 0);
        }
        count = btlGetIndexListCount(list);
        sides = 0;
        for (i = 0; i < count; i++) {
            target = btlGetIndexListEntry(list, i);
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
        kind = func_001A3360(task, list, 0);
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

/* Resolver hook receives kind and press; both are unused by this title's no-op. */
void btlFindSoundTaskByWorkValue(s32 kind, s32 press) {
}

/* Return the oldest matching handle, or zero; unstarted tasks may have handle 0. */
s32 btlFindTaskByHandle(s64 handle) {
    SoundTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    while (task != 0) {
        if ((s64)task->handle == handle) {
            return (s32)task;
        }
        task = task->next;
    }
    return 0;
}

/* Return the oldest task with this owner, or zero. */
s32 btlFindTaskByOwner(s64 owner) {
    SoundTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    while (task != 0) {
        if ((s64)task->owner == owner) {
            return (s32)task;
        }
        task = task->next;
    }
    return 0;
}

/* Return the oldest registered task of this kind, or zero. */
s32 btlFindTaskByKind(u16 kind) {
    SoundTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    while (task != 0) {
        if (task->taskId == kind) {
            return (s32)task;
        }
        task = task->next;
    }
    return 0;
}

/* Count all registrations, including tasks awaiting startup or release. */
s32 btlCountRegisteredTasks(void) {
    s32 address;
    s32 count;

    address = btlGetRuntime();
    count = 0;
    for (address = (s32)((BtlActorWork *)address)->taskHead; address != 0; address = (s32)((SoundTask *)address)->next) {
        count = count + 1;
    }
    return count;
}

/* Count registrations with this full-width owner key. */
s32 btlCountTasksForOwner(s64 key) {
    SoundTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    s32 count = 0;
    while (task != 0) {
        s64 owner = task->owner;
        task = task->next;
        if (owner == key) {
            count++;
        }
    }
    return count;
}

/* Count registrations of the requested task kind. */
s32 btlCountTasksByKind(u16 kind) {
    SoundTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    s32 count = 0;
    while (task != 0) {
        u16 taskKind = task->taskId;
        task = task->next;
        if (taskKind == kind) {
            count++;
        }
    }
    return count;
}

/* Walk newest first and request release for tasks carrying allocation bit 1. */
void btlFlagTasksForUpdate(void) {
    SoundTask *task = ((BtlActorWork *)btlGetRuntime())->taskTail;
    while (task != 0) {
        u16 flags = task->flags;
        SoundTask *next = task->prev;
        if ((flags & 1) != 0) {
            task->flags = flags | 4;
        }
        task = next;
    }
}

/* Return whether the predicate is satisfied by value or registered tasks.
 * Kinds 5/8 accept running (phase 2) or absent, not an existing finishing task. */
INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3A40);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3A50);

s32 btlEvalTaskCondition(TaskCondition *condition, s32 value) {
    s32 result = 0;
    SoundTask *task;

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
        task = (SoundTask *)btlFindTaskByHandle(condition->value.handle);
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
        task = (SoundTask *)btlFindTaskByOwner(condition->value.owner);
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
void *btlAllocTask(s32 size) {
    SoundTask *task = sdfAllocAndClearQuadwords(size + 0x70);
    BtlActorWork *context;
    SoundTask *tail;

    if (size > 0) {
        task->args = (u8 *)task + 0x70;
    } else {
        task->args = 0;
    }
    context = (BtlActorWork *)btlGetRuntime();
    task->next = 0;
    tail = context->taskTail;
    if (tail != 0) {
        tail->next = task;
        task->prev = context->taskTail;
    } else {
        context->taskHead = task;
        task->prev = 0;
    }
    context->taskTail = task;
    task->flags |= BTL_TASK_FLAG_REGISTERED;
    return task;
}

/* Return the argument address recorded by allocation (zero for no arguments). */
void *btlGetTaskArguments(void *task) {
    return ((SoundTask *)task)->args;
}

/* Invoke the finish hook before unlinking, then release the task block. */
void btlFreeTask(s32 taskAddress) {
    BtlActorWork *context;
    SoundTask *next;
    SoundTask *previous;
    void (*cleanup)(u32 *);
    SoundTask *task = (SoundTask *)taskAddress;
    cleanup = task->onFinish;
    if (cleanup != 0) {
        cleanup(task->args);
    }
    context = (BtlActorWork *)btlGetRuntime();
    previous = task->prev;
    if (previous != 0) {
        previous->next = task->next;
    } else {
        context->taskHead = task->next;
    }
    next = task->next;
    if (next != 0) {
        next->prev = task->prev;
    } else {
        context->taskTail = task->prev;
    }
    sdfReleaseChipBlock((void *)taskAddress);
}

/* Install a fresh handle/reset phase counters, invoke startup, then reread handle. */
u64 btlStartTask(void *taskObject) {
    u64 value = btlAdvanceRuntimeSequenceCounter();
    SoundTask *task = taskObject;
    void (*callback)(u32) = task->onStart;
    task->flags |= BTL_TASK_FLAG_STARTED;
    task->handle = value;
    task->pollCount = 0;
    task->runCount = 0;
    task->state = BTL_TASK_PHASE_WAITING;
    task->deferNext = 0;
    task->deferPrev = 0;
    if (callback != 0) {
        callback((u32)task->args);
    }
    return task->handle;
}

void btlResetDeferredTaskQueue(void) {
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

/* Advance a started task through wait/delay/update/release.
 * Fallthrough is intentional: zero delays permit all phases in one poll. */
void btlRunTask(s32 taskAddress) {
    SoundTask *task = (SoundTask *)taskAddress;
    u32 counter;

    if (!(task->flags & BTL_TASK_FLAG_STARTED)) {
        return;
    }
    if (task->flags & BTL_TASK_FLAG_RELEASE_REQUESTED) {
        btlFreeTask(taskAddress);
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
        } else if (task->callback.run(task->args) != 0) {
            task->state = BTL_TASK_PHASE_END_DELAY;
        } else {
            task->runCount = task->runCount + 1;
            break;
        }
    case BTL_TASK_PHASE_END_DELAY:
        if (task->endDelay <= 0) {
            btlFreeTask(taskAddress);
        } else {
            task->endDelay = task->endDelay - 1;
        }
        break;
    }
}

/* Run ordinary registrations now; queue deferred registrations for the later pass. */
void btlSweepFinishedTasks(void) {
    SoundTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    SoundTask *next;
    while (task != 0) {
        next = task->next;
        if (!(task->flags & BTL_TASK_FLAG_DEFERRED)) {
            btlRunTask((s32)task);
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
        task = next;
    }
}

/* Run deferred tasks, saving the next link before callbacks may free the task. */
void btlClearDeferredTasks(void) {
    SoundTask *node = btlDeferredTaskHead;
    while (node != 0) {
        SoundTask *next = node->deferNext;
        btlRunTask((s32)node);
        node = next;
    }
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

/* Release newest first; cache the previous registration before its block is freed. */
void btlClearTaskLists(void) {
    SoundTask *task = ((BtlActorWork *)btlGetRuntime())->taskTail;
    while (task != 0) {
        SoundTask *next = task->prev;
        btlFreeTask((s32)task);
        task = next;
    }
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

u32 func_001D4B28(void) {
    return 1;
}

void *btlCreateImmediateCompletionTask(void) {
    u8 *task = btlAllocTask(0);
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(void **)(task + 0x4C) = func_001D4B28;
    *(u16 *)(task + 0x20) = 0x63;
    *(u32 *)(task + 0x48) = 0;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    return task;
}

void btlDumpTaskQueue(void) {
    s32 context = btlGetRuntime();
    s32 node = *(s32 *)(context + 0x230);
    while (node != 0) {
        btlBossDebugPrintf("btl:packet[%d]\n", *(u16 *)(node + 0x20));
        node = *(s32 *)(node + 0x58);
    }
    btlBossDebugPrintf("btl:packet head[%p]\n", *(void **)(context + 0x22C));
    btlBossDebugPrintf("btl:packet tail[%p]\n", *(void **)(context + 0x230));
}

extern u128 D_00359CC0;

void btlInitUnitFxDefaults(u8 *fx) {
    PCP_COPY_VECTOR(fx + 0x90, &D_00359CC0);
    *(f32 *)(fx + 0xB0) = 220.0f;
    *(f32 *)(fx + 0xB4) = 80.0f;
    *(f32 *)(fx + 0xC0) = 75.0f;
}

extern u128 D_00359CD0;

extern u128 D_00359CE0;

void btlInitFxLights(u8 *fx) {
    PCP_COPY_VECTOR(fx + 0x30, &D_00359CD0);
    PCP_COPY_VECTOR(fx + 0x40, &D_00359CE0);
    *(u32 *)(fx + 0x58) = 0;
    *(f32 *)(fx + 0x50) = 1.0f;
    *(u32 *)(fx + 0x54) = 0x80808080;
    PCP_COPY_VECTOR(fx + 0x60, &D_00359CD0);
    PCP_COPY_VECTOR(fx + 0x70, &D_00359CE0);
    *(f32 *)(fx + 0x80) = 1.0f;
    *(u32 *)(fx + 0x84) = 0x80808080;
    *(u32 *)(fx + 0x88) = 0;
}

void btlInitializeEffectVectorsFromSourceRecords(BtlUnit *fx, s32 kind, s32 index) {
    BtlFxSrcA *alt = (BtlFxSrcA *)btlSelectSharedOrIndexedTransformParameters(kind, index);
    BtlEffectResource *base = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(kind, index);

    if (alt->fC == 0.0f) {
        PCP_COPY_VECTOR(fx->bodyOffset, base);
        fx->reach = base->f18;
        fx->height = base->f1C;
        fx->cameraRadius = base->f20;
    } else {
        fx->bodyOffset[0] = alt->f0;
        fx->bodyOffset[1] = alt->f4;
        fx->bodyOffset[2] = alt->f8;
        fx->bodyOffset[3] = 0.0f;
        fx->reach = alt->f10;
        fx->height = alt->f14;
    }
    PCP_COPY_VECTOR(fx->muzzleOffset, base);
    fx->unkBC = base->f18;
    fx->unkB8 = base->f1C;
    fx->cameraRadius = base->f20;
    fx->scale = base->f10;
    fx->effectScale = base->f10;
    fx->zOffset = base->f14;
    fx->unk58 = base->f14;
}

s32 btlHasMatchingModel(s32 effect, s32 model) {
    s32 context = btlGetRuntime();
    BtlUnit *node = ((BtlState *)context)->units;
    while (node != 0) {
        if ((node->status.flags & 2) != 0 &&
            node->ext != 0 &&
            node->soundSlotOwner != 0 &&
            mdlGetContextResourceGroup(node->ext->owner) == effect &&
            mdlGetContextResourceId(node->ext->owner) == model) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001D0BA0", func_001D4E60);

/* Kind-5 world-node payload attaches the event unit at +8. */
typedef struct EventUnitData {
    u8 pad00[8];
    EvtUnit *unit;
} EventUnitData;

extern s32 mdlSpawnCameraSlotViewerObject(s32, s32);

extern void *dds3GetWorldObject(void);

extern void dds3ClearObjectFlags(void *, s32);

extern void dds3SetObjectFlags(void *, s32);

extern void mdlSetAmountOnAllContextResources(MdlCtx *, f32);

extern struct SoundSlotOwner *sndAcquireSlotOwner(s32, s32);

extern void btlResetUnitModelProgress(BtlUnit *);

extern void btlSetUnitPosition(BtlUnit *object, void *position);

extern void btlSetUnitRotation(BtlUnit *object, void *rotation);

/* vu0 routine: initialize the actor world transform with the SDK unit vector. */
void btlBindActorModel(BtlUnit *unit, u32 kind, u32 id) {
    BtlUnit *reused = NULL;
    BtlState *battle = (BtlState *)btlGetRuntime();
    MdlCtx *model;
    BtlActorStatusRecord *status;
    EventUnitData *data;
    s32 key;

    unit->resourceKind = kind;
    unit->species = id;
    unit->unkCC = 0;
    btlInitializeEffectVectorsFromSourceRecords(unit, kind, id);
    if (battle->findReusableUnit) {
        reused = battle->findReusableUnit(kind, id);
        if (reused) {
            if (reused->resourceKind != kind || reused->species != id) {
                key = mdlSpawnCameraSlotViewerObject(kind, id);
                dds3RemoveWorldObjectNode(dds3FindWorldObjectNodeByKey(dds3GetWorldObject(), key, 5));
            }
            func_001D4E60(unit, reused);
        }
    }
    if (battle->prepareModelUnit) {
        battle->prepareModelUnit(unit);
    }
    if (reused == NULL) {
        key = mdlSpawnCameraSlotViewerObject(kind, id);
        unit->effectObject = dds3FindWorldObjectNodeByKey(dds3GetWorldObject(), key, 5);
        data = unit->effectObject->data;
        unit->ext = data->unit;
        model = unit->ext->owner;
        unit->ext->flags |= 0x200000;
        VU0_SET_ONES_XYZ(vf10);
        VU0_SCALAR_OP(unit->effectScale, "vmulx.xyzw vf10, vf10, vf2x");
        unit->effectObject->inner->flags = (unit->effectObject->inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
            ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
        VU0_STORE_VF(vf10, unit->effectObject->inner->scale);
        mdlStoreTertiaryVectorVU(model);
        if (unit->effectScale != 1.0f) {
            mdlSetAmountOnAllContextResources(model, unit->effectScale);
        }
        dds3ClearObjectFlags(unit->effectObject, 0x400);
        unit->status.flags |= 8;
        unit->soundSlotOwner = sndAcquireSlotOwner(kind, id);
        unit->status.flags |= 2;
    }
    btlSetUnitPosition(unit, unit->position);
    btlSetUnitRotation(unit, unit->rotation);
    unit->updateFlags = 0;
    unit->effectTimerA = 0;
    unit->unkEC = -1;
    unit->effectTimerB = 0;
    btlRefreshUnitMotionSelection(unit);
    if (unit->effectArgA != 0xB) {
        btlApplyScaledUnitEffectParameter((u8 *)unit, unit->effectArgA, 1, 1.0f);
    } else {
        btlApplyScaledUnitEffectParameter((u8 *)unit, 0xB, 2, 1.0f);
    }
    if (unit->resourceLink) {
        btlMarkTaskReady(unit->resourceLink);
    }
    unit->status.flags |= 0x40000000;
    if (unit->status.flags & 0x20) {
        model = unit->ext->owner;
        unit->ext->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        unit->ext->flags &= ~0xA0;
        mdlAddEntryPlain(model, 0, 0xB);
        unit->unkEC = 0xB;
        sdfMotionSampleAtFrame(model->first, model->first->frameCount);
        unit->status.flags = unit->status.flags & 0x7FFFFFFF & 0xBFFFFFFF;
    } else if (btlTestActorStatusPredicate(unit)) {
        unit->ext->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        model = unit->ext->owner;
        unit->ext->flags &= ~0xA0;
        status = (BtlActorStatusRecord *)btlGetSideIndexedActorStatusTable(kind, id);
        mdlAddEntryPlain(model, 0, 1);
        unit->unkEC = 1;
        sdfMotionSampleAtFrame(model->first, status->model);
        btlResetUnitModelProgress(unit);
        unit->status.flags = (unit->status.flags | 0x2000) & 0x7FFFFFFF & 0xBFFFFFFF;
    }
    unit->status.flags |= 0x80004;
    if (battle->finishModelUnit) {
        battle->finishModelUnit(unit);
    }
}

extern const char D_003A3AD0[];

void btlReleaseActorModelResources(BtlUnit *object) {
    SoundSlotOwner *sound;
    s32 load;
    EffWorldNode *model;
    u32 state;
    u32 flags;
    if (((u8 *)object)[0xCC] == 0) {
        sound = *(SoundSlotOwner **)((u8 *)object + 0x308);
        if (sound != 0) {
            sndReleaseSlotOwner(sound);
            *(SoundSlotOwner **)((u8 *)object + 0x308) = 0;
        }
        load = *(s32 *)((u8 *)object + 0x324);
        if (load != 0) {
            sdfReleaseDevSlot(load, 1, 1);
            *(s32 *)((u8 *)object + 0x324) = 0;
            btlBossDebugPrintf(D_003A3AD0, object);
        }
        model = *(EffWorldNode **)((u8 *)object + 0x31C);
        if (model != 0) {
            dds3RemoveWorldObjectNode(model);
            *(s32 *)((u8 *)object + 0x31C) = 0;
            *(s32 *)((u8 *)object + 0x320) = 0;
        }
    } else {
        *(s32 *)((u8 *)object + 0x308) = 0;
        *(s32 *)((u8 *)object + 0x324) = 0;
        *(s32 *)((u8 *)object + 0x31C) = 0;
        *(s32 *)((u8 *)object + 0x320) = 0;
    }
    state = *(u32 *)((u8 *)object + 0x118) & ~1;
    flags = (u32)object->status.flags & ~2;
    state &= ~2;
    object->status.flags = flags;
    *(u32 *)((u8 *)object + 0x118) = state;
}

void btlRequestModelAssetByMode(u32 arg0, u32 arg1, u32 arg2) {
    s64 temp_v0;

    temp_v0 = mdlFlagTest(0xc0f);
    if (temp_v0 != 0) {
        btlEnsureModelAndSoundReady(arg1, arg2);
        return;
    }
    mdlRequestAsset(arg1, arg2, 0);
}

void btlReleaseModelAssetByMode(u32 arg0, u32 arg1, u32 arg2) {
    s64 temp_v0;

    temp_v0 = mdlFlagTest(0xc0f);
    if (temp_v0 != 0) {
        btlReleaseFoundModelEntry(arg1, arg2);
        return;
    }
}

s32 btlCheckModelAssetByMode(u8 *object, u32 effect, u32 model) {
    if (mdlFlagTest(0xC0F) != 0) {
        if (btlEnsureModelAndSoundReady(effect, model, 0) != 0) {
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
    s32 (*hook)(BtlUnit *) = ((BtlState *)btlGetRuntime())->allowDefeatCandidate;
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

void btlClearUnitDefeatCandidate(BtlUnit *object) {
    s32 (*callback)(BtlUnit *);
    u32 flags;
    u32 masked;

    callback = ((BtlState *)btlGetRuntime())->unk658;
    if (callback != 0 && callback(object) == 0) {
        return;
    }
    flags = object->status.flags;
    masked = flags & ~4;
    masked &= ~8;
    object->status.flags = masked;
    if ((flags & 2) != 0) {
        MdlCtx *resource = object->ext->owner;
        resource->flags |= MDL_SKIP_TRANSFORMS;
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

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3AD0);

void btlApplyUnitMotionSelection(BtlUnit *unit, u32 index, s32 mode, f32 rate) {
    u8 *context;
    BtlEffectResource *table;
    u8 *task;
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
                *(s32 *)(task + 0x28) = 1;
                *(u64 *)(task + 0x40) = 0;
                btlStartTask(task);
                break;
            }
        }
        return;
    }
    context = (u8 *)btlGetRuntime();
    if (unit->updateFlags & 2) {
        color = (unit->overlayColor & 0xFFFFFF) | 0x80000000;
        evtSetUnitRgbTransition(unit->ext, 0, color);
        evtSetUnitAlphaTransition(unit->ext, 0, color);
        unit->overlayColor = color;
        unit->updateFlags &= ~4;
        unit->updateFlags &= ~2;
    }
    table = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(unit->resourceKind,
                                                                unit->species);
    if (table->nodes[index].rateKind == 2) {
        unit->updateFlags |= 6;
    }
    chooseMotion = *(s32 (**)(BtlUnit *, s32, s32))(context + 0x5A0);
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
            start = unit->effectTimerA;
            end = unit->effectTimerB;
            break;
        case 1: case 18:
            start = 0;
            end = 1;
            break;
        case 3: case 4: case 5: case 6: case 7: case 8:
        case 12: case 19: case 20: case 21: case 22: case 23: case 24:
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
    keepRange = *(s32 (**)(BtlUnit *, s32))(context + 0x5A8);
    if (keepRange != 0 && keepRange(unit, index) != 0) {
        start = unit->effectTimerA;
        end = unit->effectTimerB;
    }
    if (mode & 0x100) {
        end = 8;
        mode &= ~0x100;
    }
    unit->motionRate = rate;
    unit->unkEC = index;
    unit->effectState = mode;
    rate = rate * (30.0f / ((BtlState *)context)->timingRate);
    rate *= *(f32 *)(context + 0x494);
    evtPrepareUnitMotionState(unit->ext, index, start, end, mode);
    model = unit->ext->owner;
    model->first->frameStep = rate;
    if (end == 0) {
        mdlAddEntryFlagged(model, 0, index);
        sdfMotionSampleAtFrame(unit->ext->owner->first, 0.0f);
    }
    unit->effectTimerA = 0;
    frameCount = table->nodes[index].frameCount;
    unit->effectTimerB = frameCount;
    if (mode != 0 && mode != 3) {
        return;
    }
    evtStoreUnitMotionShortParameters(unit->ext, 0, (s16)frameCount);
    unit->effectTimerA = 0;
    unit->effectTimerB = table->nodes[unit->effectArgA].frameCount;
}

void btlRefreshUnitMotionSelection(BtlUnit *unit) {
    s32 entryFlags;
    u8 *context;
    s32 index;
    s32 selected;
    s32 mode;
    u8 *rates;
    f32 rate;
    f32 speed;
    u32 color;
    s32 (*chooseStatus)(BtlUnit *);
    s32 (*chooseMotion)(BtlUnit *, s32, s32);

    if (((u32)unit->status.flags & 2) == 0) {
        return;
    }
    entryFlags = btlGetEntryFlagsUnlessDisabled(&unit->partyRecord);
    context = (u8 *)btlGetRuntime();
    if (*(u32 *)((u8 *)unit + 0xE8) & 2) {
        color = (*(u32 *)((u8 *)unit + 0x84) & 0xFFFFFF) | 0x80000000;
        evtSetUnitRgbTransition(*(struct EvtUnit **)((u8 *)unit + 0x320), 0, color);
        evtSetUnitAlphaTransition(*(struct EvtUnit **)((u8 *)unit + 0x320), 0, color);
        *(u32 *)((u8 *)unit + 0x84) = color;
        *(u32 *)((u8 *)unit + 0xE8) &= ~4;
        *(u32 *)((u8 *)unit + 0xE8) &= ~2;
    }
    index = 0;
    if (btlIsCurrentValueBelowQuarterThreshold(unit) != 0 &&
        (((u32)unit->status.flags & 0x200) || (entryFlags & 0x200))) {
        index = 10;
    }
    if (*(s32 *)((u8 *)unit + 0x2F0) > 0 &&
        (((u32)unit->status.flags & 0x200) || (entryFlags & 0x200))) {
        index = 9;
    }
    switch (*(u16 *)((u8 *)unit + 0x12E) & 0x7FFF) {
    case 1: case 8: case 0x10: case 0x20: case 0x40:
    case 0x80: case 0x100: case 0x200: case 0x400: case 0x2000:
        index = 2;
        break;
    }
    if (btlTestActorStatusPredicate(unit) != 0) {
        if (((u32)unit->status.flags & 0x2000) == 0) {
            unit->status.flags = (u32)unit->status.flags | (0x80002000);
        }
    } else if ((u32)unit->status.flags & 0x2000) {
        btlApplyUnitModelScaledValue(unit);
        unit->status.flags = (u32)unit->status.flags & (0x7FFFFFFF);
        unit->status.flags = (u32)unit->status.flags & (~0x2000);
    }
    chooseStatus = *(s32 (**)(BtlUnit *))(context + 0x5A4);
    if (chooseStatus != 0) {
        selected = chooseStatus(unit);
        if (selected >= 0) {
            index = selected;
        }
    }
    if (btlIsUnitDefeatTriggeredByValueDelta(unit, 0) != 0 &&
        (((u32)unit->status.flags & 0x200) || (entryFlags & 0x200)) &&
        ((unit->status.stateFlags & 0x40) == 0)) {
        index = 11;
        btlApplyUnitModelScaledValue(unit);
        unit->status.flags = (u32)unit->status.flags & (0x7FFFFFFF);
        unit->status.flags = (u32)unit->status.flags & (~0x2000);
    }
    rates = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)((u8 *)unit + 0xC4),
                                                  *(s32 *)((u8 *)unit + 0xC8)) + 0x14;
    rate = *(f32 *)(rates + index * 20 + 0x20);
    chooseMotion = ((BtlState *)context)->chooseMotion;
    if (chooseMotion != 0) {
        selected = chooseMotion(unit, index, 1);
        if (selected == -1) {
            return;
        }
        if (index != selected) {
            index = selected;
            rate = *(f32 *)(rates + index * 20 + 0x20);
        }
    }
    *(f32 *)((u8 *)unit + 0x104) = rate;
    speed = rate * (30.0f / ((BtlState *)context)->timingRate);
    speed *= *(f32 *)(context + 0x494);
    *(s32 *)((u8 *)unit + 0xFC) = index;
    evtUnitSetStoredParameter(*(struct EvtUnit **)((u8 *)unit + 0x320), index);
    evtSetTransitionMotionScale(*(struct EvtUnit **)((u8 *)unit + 0x320), speed);
    mode = 1;
    if (index == 11) {
        mode = 2;
    }
    *(s32 *)((u8 *)unit + 0x100) = mode;
    if (*(s32 *)((u8 *)unit + 0xEC) != 11 &&
        (btlIsActorModeAcceptedByBattleHook(unit) != 0 || index == 11) &&
        *(s32 *)((u8 *)unit + 0xEC) != index) {
        btlApplyUnitMotionSelection(unit, index, mode, rate);
    }
}

s32 btlIsActorModeAcceptedByBattleHook(BtlUnit *object) {
    s32 value;
    if (((u32)object->status.flags & 2) == 0) {
        return 0;
    }
    {
        s32 (*callback)(BtlUnit *) = *(s32 (**)(BtlUnit *))(btlGetRuntime() + 0x5A4);
        if (callback != 0 && callback(0) == *(s32 *)((u8 *)object + 0xEC)) {
            return 1;
        }
    }
    value = *(s32 *)((u8 *)object + 0xEC);
    switch (value) {
    case 0:
    case 2:
    case 9:
    case 10:
    case 11:
        return 1;
    }
    return 0;
}

extern void btlApplyUnitMotionSelection(BtlUnit *, u32, s32, f32);

void btlApplyScaledUnitEffectParameter(u8 *object, s32 index, s32 argument, f32 scale) {
    u8 *resource = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)(object + 0xC4), *(s32 *)(object + 0xC8));
    f32 value = *(f32 *)(resource + index * 20 + 0x34);
    btlApplyUnitMotionSelection(object, index, argument, value * scale);
}

s32 btlGetSlotRateKind(u8 *object, s32 index) {
    u8 *resource = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)(object + 0xC4), *(s32 *)(object + 0xC8));
    s32 value = *(s16 *)(resource + index * 20 + 0x30);

    switch (value) {
    case 0:
        return 0;
    case 1:
    case 2:
    case 3:
        return 2;
    }
    return 0;
}

void btlUpdateUnitEffects(void) {
    s32 context = btlGetRuntime();
    BtlUnit *object = *(BtlUnit **)(context + 0x228);

    while (object != 0) {
        if ((u32)object->status.flags & 2) {
            u8 *resource = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)((u8 *)object + 0xC4),
                                                  *(s32 *)((u8 *)object + 0xC8));
            MdlCtx *model = object->ext->owner;
            s32 node = mdlGetNodeMotionIndex(model, 0);
            if (*(s16 *)(resource + node * 20 + 0x30) == 1 &&
                btlIsActorModeAcceptedByBattleHook(object) == 0) {
                btlRefreshUnitMotionSelection(object);
                btlApplyUnitMotionSelection(object, *(s32 *)((u8 *)object + 0xFC),
                              *(s32 *)((u8 *)object + 0x100),
                              *(f32 *)((u8 *)object + 0x104));
            }
        }
        object = *(BtlUnit **)((u8 *)object + 0x344);
    }
}

void btlApplyUnitModelScaledValue(BtlUnit *object) {
    s32 context;
    f32 volume;
    if ((object->status.flags & 2) == 0) {
        return;
    }
    context = btlGetRuntime();
    object->updateFlags &= ~1;
    volume = object->motionRate;
    object->ext->owner->first->frameStep =
        volume * (30.0f / (f32)((BtlState *)context)->timingRate);
}

void btlResetUnitModelProgress(BtlUnit *object) {
    if ((object->status.flags & 2) != 0) {
        object->updateFlags |= 1;
        object->ext->owner->first->frameStep = 0.0f;
    }
}

s32 func_001D6050(BtlUnit *unit, s32 motionIndex) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlActorStatusRecord *status = (BtlActorStatusRecord *)
        btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->species);

    return (s32)(status->motions[motionIndex].frameCount /
        (status->motions[motionIndex].alphaFrameScale * state->modelFrameScale));
}

f32 btlGetUnitModelValue1C(BtlUnit *unit) {
    if ((unit->status.flags & 2) == 0) {
        return 0.0f;
    }
    return unit->ext->owner->first->currentFrame;
}

/* The caller's frame argument is forwarded unchanged to the sampler. */
void btlAdvanceUnitModelFrame(BtlUnit *unit, f32 frame) {
    if ((unit->status.flags & 2) != 0) {
        sdfMotionSampleAtFrame(unit->ext->owner->first, frame);
        return;
    }
}

s32 btlGetUnitModelFrameCount(BtlUnit *unit) {
    if ((unit->status.flags & 2) == 0) {
        return 0;
    }
    return unit->ext->owner->first->frameCount;
}

void btlSeekUnitModelFrameZero(BtlUnit *unit) {

    if ((unit->status.flags & 2) == 0) {
        return;
    }
    sdfMotionSampleAtFrame(unit->ext->owner->first, 0.0f);
}

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

u32 btlIsUnitModelStateFive(BtlUnit *object) {
    if ((object->status.flags & 2) == 0) {
        return 1;
    }
    if (object->effectState != 2) {
        return 1;
    }
    return object->ext->owner->first->state == SDF_MOTION_STATE_TERMINAL;
}

void btlSetUnitPosition(BtlUnit *object, void *position) {
    f32 world[4] __attribute__((aligned(16)));
    s32 context;
    if ((object->status.stateFlags & 0x80) != 0) {
        return;
    }
    context = btlGetRuntime();
    VU0_LOAD_VF(vf10, position);
    VU0_STORE_VF(vf10, (u8 *)object + 0x60);
    VU0_LOAD_VF(vf11, context);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, world);
    if (((u32)object->status.flags & 2) != 0) {
        world[2] += *(f32 *)((u8 *)object + 0x88);
        effObjSetInnerPosition(*(EffWorldNode **)((u8 *)object + 0x31C), (u128 *)world);
    }
}

void func_001D6300(u8 *object, void *position) {
    PCP_COPY_VECTOR(position, object + 0x60);
}

void btlGetUnitWorldPos(u8 *object, void *worldPosition) {
    s32 context = btlGetRuntime();
    VU0_LOAD_VF(vf10, object + 0x60);
    VU0_LOAD_VF(vf11, context);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, worldPosition);
}

extern s32 sdfLoadMapRecordPositionVector(SdfModel *, s32);

extern void sdfModelUpdateCurrentFrameTransforms(SdfModel *);

extern void btlRefreshUnitFxVectors(BtlUnit *);

s32 btlSetActorEffectParameter(object, value)
BtlUnit *object;
s32 value;
{
    s32 (*callback)(BtlUnit *, s32);
    if ((object->status.flags & 2) == 0) {
        return 0;
    }
    callback = ((BtlState *)btlGetRuntime())->effectParameterCallback;
    if (callback != 0) {
        value = callback(object, value);
    }
    btlRefreshUnitFxVectors(object);
    {
        MdlCtx *owner = object->ext->owner;
        return (s8)sdfLoadMapRecordPositionVector(owner->inner, value);
    }
}

void btlSetActorEffectParameterOrMuzzlePosition(BtlUnit *unit, s32 mode) {
    s64 temp_v0;

    temp_v0 = btlSetActorEffectParameter(unit, mode);
    if (temp_v0 == 0) {
        btlUnitGetMuzzlePosVU(unit);
        return;
    }
}

/* vu0 routine: preserve the actor's primary position and rotation quaternion while
 * evaluating the requested model record; return the sampled vector in vf10. */
s32 func_001D6428(BtlUnit *unit, s32 value) {
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

s32 btlSetActorAlternateEffectParameter(object, value)
BtlUnit *object;
s32 value;
{
    s32 (*callback)(BtlUnit *, s32);
    if ((object->status.flags & 2) == 0) {
        return 0;
    }
    callback = ((BtlState *)btlGetRuntime())->effectParameterCallback;
    if (callback != 0) {
        value = callback(object, value);
    }
    btlRefreshUnitFxVectors(object);
    {
        MdlCtx *owner = object->ext->owner;
        return (s8)sdfLoadMapRecordLookAtBasis(owner->inner, value);
    }
}

void btlSetAlternateEffectParameterOrMuzzlePosition(BtlUnit *unit, s32 mode) {
    if (btlSetActorAlternateEffectParameter(unit, mode) != 0) {
        return;
    }
    VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
}

s32 btlIsUnitAtStoredPosition(u8 *object) {
    f32 position[3];
    func_001D6300(object, position);
    if (*(f32 *)(object + 0x30) == position[0] &&
        *(f32 *)(object + 0x34) == position[1] &&
        *(f32 *)(object + 0x38) == position[2]) {
        return 1;
    }
    return 0;
}

extern u8 D_003A3B70[];

extern void effMiscQuatMultiplyVU(void);

void btlSetUnitRotation(BtlUnit *object, void *rotation) {
    u8 vector[16];
    if ((object->status.stateFlags & 0x100) != 0) {
        return;
    }
    VU0_LOAD_VF(vf10, rotation);
    if (((u32)object->status.flags & 0x10) != 0) {
        VU0_LOAD_VF(vf11, D_003A3B70);
        effMiscQuatMultiplyVU();
    }
    VU0_STORE_VF_UNCLOBBERED(vf10, (u8 *)object + 0x70);
    VU0_LOAD_VF(vf11, D_003A3B70);
    effMiscQuatMultiplyVU();
    VU0_STORE_VF_UNCLOBBERED(vf10, vector);
    if (((u32)object->status.flags & 2) != 0) {
        effObjSetInnerRotation(*(EffWorldNode **)((u8 *)object + 0x31C), (u128 *)vector);
    }
}

void btlCopyUnitRotationQuaternion(BtlUnit *unit, void *dst) {
    PCP_COPY_VECTOR(dst, unit->orientation);
}

void btlSetUnitColor(BtlUnit *unit, u32 color, s32 mode) {
    if ((u32)unit->status.flags & 2) {
        color = (color & 0xFFFFFF) | 0x80000000;
        *(u32 *)((u8 *)unit + 0x54) = (*(u32 *)((u8 *)unit + 0x54) & 0xFF000000) | (color & 0xFFFFFF);
        evtSetUnitRgbTransition((EvtUnit *)*(u32 *)((u8 *)unit + 0x320), mode, color);
    }
}

void btlBlendUnitColor(BtlUnit *unit, u32 color, s32 mode) {
    u32 base;
    u32 blended;
    if ((u32)unit->status.flags & 2) {
        color = (color & 0xFFFFFF) | 0x80000000;
        base = (*(u32 *)((u8 *)unit + 0x54) & 0xFFFFFF) | 0x80000000;
        blended = (base & color) + (((base ^ color) & 0xFEFEFEFE) >> 1);
        *(u32 *)((u8 *)unit + 0x84) = (*(u32 *)((u8 *)unit + 0x84) & 0xFF000000) | (color & 0xFFFFFF);
        evtSetUnitRgbTransition((EvtUnit *)*(u32 *)((u8 *)unit + 0x320), mode, blended);
    }
}

extern void mdlReleaseInnerResourceHandle(MdlCtx *, s32, f32);

void btlReleaseUnitModelColorResource(BtlUnit *unit, s32 value, f32 scalar) {
    mdlReleaseInnerResourceHandle(unit->ext->owner, (value & 0xffffff) | 0x80000000, scalar);
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

extern void btlUnitGetBodyPosVU(u8 *);

extern s32 btlAimHorizontalDirectionVU(void *, void *);

void btlUnitFaceTarget(BtlUnit *object, BtlUnit *target) {
    u8 first[16];
    u8 second[16];
    u8 result[16];
    if (((u32)object->status.flags & 0x80000) != 0) {
        btlUnitGetBodyPosVU(object);
        VU0_STORE_VF_UNCLOBBERED(vf10, first);
        btlUnitGetBodyPosVU(target);
        VU0_STORE_VF_UNCLOBBERED(vf10, second);
        if (btlAimHorizontalDirectionVU(first, second) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, result);
            btlSetUnitRotation(object, result);
        }
    }
}

extern s32 btlAimHorizontalDirectionClampedVU(void *, void *, f32);

void btlUnitFaceTargetScaled(BtlUnit *object, BtlUnit *target, f32 scale) {
    u8 first[16];
    u8 second[16];
    u8 result[16];
    if (((u32)object->status.flags & 0x80000) != 0) {
        btlUnitGetBodyPosVU(object);
        VU0_STORE_VF_UNCLOBBERED(vf10, first);
        btlUnitGetBodyPosVU(target);
        VU0_STORE_VF_UNCLOBBERED(vf10, second);
        btlAimHorizontalDirectionClampedVU(first, second, scale);
        VU0_STORE_VF_UNCLOBBERED(vf10, result);
        btlSetUnitRotation(object, result);
    }
}

void btlCopyUnitStats(s32 arg0, s32 arg1) {
    DatPartyRecord *record = &((BtlUnit *)arg0)->partyRecord;
    *record = *(DatPartyRecord *)arg1;
    btlRefreshUnitMaximumHpAndClampCurrentHp(record);
    btlRefreshUnitMaximumMpAndClampCurrentMp(record);
}

extern void mdlDispatchViewerAnchorRecord(MdlCtx *, MdlResourceItem *);

extern s32 sdfAllocPacketAligned(s32 size);

extern void func_002D9748(SdfModel *, SdfModel *);

extern void func_002D9238(SdfPoolNode **, SdfModel *);

extern u64 D_00359CF0[4];

/* Draw the model into four surfaces in three GS TEST passes, then update its anchors. */
void func_001D6A80(BtlUnit *unit, MdlCtx *model, SdfModel *overlay, SdfPoolNode **surfaces, u32 frame) {
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
    func_002D9748(overlay, model->inner);
    if (unit->status.flags & 2) {
        overlay->lighting = unit->ext->endpointWork;
    } else {
        overlay->lighting = NULL;
    }
    func_002D9238(surfaces, overlay);
    for (i = 0; i != 4; i++) {
        list = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        packet = (u64 *)sdfAllocPacketAligned(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = D_00359CF0[i];
        packet[5] = 0x47;
        sdfAppendPacket(list, (u32)packet);
        surfaces[i]->append(surfaces[i], list);
    }
    mdlSetAllResourceFrames(model, frame);
    for (item = model->resourceItems; item != NULL; item = item->next) {
        mdlDispatchViewerAnchorRecord(model, item);
    }
}

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3B70);

void btlCreateUnitTransparency(BtlUnit *unit) {
    BattleGroupNode *shape;
    if ((unit->status.flags & 2) == 0) {
        return;
    }
    if (unit->transparencyModel != 0) {
        return;
    }
    if (unit->unkCC != 0) {
        return;
    }
    shape = unit->ext->owner->sub;
    unit->transparencyModel = (s32)sdfModelCreateWithItems(shape->resourceList, shape->itemList);
    dds3SetObjectFlags(unit->effectObject, 1);
    btlBossDebugPrintf("btl:unit transparency create[%p]\n", unit);
}

extern void sdfReleaseDevSlot(s32, s32, s32);

extern struct SdfPoolNode *D_00325788[13][4];

extern SdfPoolNode *D_00359D10[];

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
                if (unit->transparencyModel != 0) {
                    sdfReleaseDevSlot(unit->transparencyModel, 1, 1);
                    unit->transparencyModel = 0;
                    if (unit->status.flags & 2) {
                        info->inner->lighting = unit->ext->endpointWork;
                    } else {
                        info->inner->lighting = 0;
                    }
                    mdlBroadcastMasked(info, color);
                    mdlProcessContextNodesAndTransforms(info, D_00325788[0]);
                    dds3ClearObjectFlags(unit->effectObject, 1);
                    btlBossDebugPrintf(D_003A3AD0, unit);
                }
            } else if (alpha == 0) {
                dds3SetObjectFlags(unit->effectObject, 1);
            } else if (unit->transparencyModel == 0) {
                btlCreateUnitTransparency(unit);
            } else {
                func_001D6A80(unit, info, (SdfModel *)unit->transparencyModel, D_00359D10, color);
            }
        }
    }
}

extern SdfGraphObj D_003980E0;

extern SdfPoolNode *D_00359D20[];

extern SdfPoolNode *D_00359D30[];

extern s32 sdfAllocPacketAligned(s32 size);

/* Draw the unit's transparency model into its mirror's packet buffer, then draw the mirror from it. Either
 * model is created (and the update ends) on the first frame it is missing. */
void func_001D6FB0(BtlUnit *unit) {
    MdlCtx *info;
    SdfListHead *list;

    if ((unit->status.flags & 2) == 0) {
        return;
    }
    if (unit->unkCC != 0) {
        return;
    }
    unit->mirror->unk32C = sdfAllocPacketAligned(0x70000);
    list = sdfAllocatePacketList(0);
    sdfCreateResourcePacket(list, D_003980E0.buffers[2],
                            0, 0, 0x200, 0xE0, unit->mirror->unk32C, 0, 0, 0);
    D_00359D20[0]->append(D_00359D20[0], list);
    info = unit->ext->owner;
    if (unit->transparencyModel == 0) {
        unit->transparencyModel = (s32)sdfModelCreateWithItems(info->sub->resourceList, info->sub->itemList);
        dds3SetObjectFlags(unit->effectObject, 1);
        return;
    }
    func_001D6A80(unit, info, (SdfModel *)unit->transparencyModel, D_00359D20, unit->overlayColor);
    info = unit->mirror->ext->owner;
    if (unit->mirror->transparencyModel == 0) {
        unit->mirror->transparencyModel = (s32)sdfModelCreateWithItems(info->sub->resourceList, info->sub->itemList);
        dds3SetObjectFlags(unit->mirror->effectObject, 1);
        return;
    }
    list = sdfAllocatePacketList(0);
    sdfCreateDescriptorPacket(list, D_003980E0.buffers[2],
                              0, 0, 0x200, 0xE0, unit->mirror->unk32C, 0);
    D_00359D30[0]->append(D_00359D30[0], list);
    func_001D6A80(unit->mirror, info, (SdfModel *)unit->mirror->transparencyModel, D_00359D30, unit->mirror->overlayColor);
}

extern char D_003A3BA8[];

extern char D_003BB5F8[];

s32 btlFormatUnitBedName(BtlUnit *actor, char *filename) {
    u32 flags;
    btlGetRuntime();
    flags = (u32)actor->status.flags;
    if (!(flags & 0x200)) {
        return 0;
    }
    if (flags & 0x1000) {
        func_003014F0(filename, D_003A3BA8, D_003BB5F8, 0,
                      *(u16 *)((u8 *)actor + 0x124));
    } else {
        func_003014F0(filename, D_003A3BA8, D_003BB5F8,
                      btlGetActorBedAssetIdFromIndex(actor->partyRecord.menuValue),
                      *(u16 *)((u8 *)actor + 0x124));
    }
    return 1;
}

extern u8 D_0037E110[];

extern void effMiscQuaternionToMatrixVU(void);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3BA8);

const char D_003A3BB8[16] = "btl:warp[%p]\n";

void btlWarpUnitToMotionReach(BtlUnit *unit, BtlUnit *target, s32 index) {
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
    table = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->species);
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
        VU0_LOAD_VF(vf10, D_0037E110);
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
            btlBossDebugPrintf(D_003A3BB8, unit);
        }
    }
}

void btlRefreshUnitEffectMotionAndEntry(BtlUnit *unit) {
    u32 flags = (u32)unit->status.flags;
    if ((flags & 2) == 0) {
        return;
    }
    if (*(s32 *)((u8 *)unit + 0xEC) != *(s32 *)((u8 *)unit + 0xFC)) {
        btlRefreshUnitMotionSelection(unit);
        *(u16 *)((u8 *)unit + 0xF8) = 0;
        *(u16 *)((u8 *)unit + 0xFA) = 0;
        btlApplyUnitMotionSelection(unit, *(s32 *)((u8 *)unit + 0xFC),
                      *(s32 *)((u8 *)unit + 0x100), *(f32 *)((u8 *)unit + 0x104));
        flags = (u32)unit->status.flags;
    }
    if ((flags & 0x2000) == 0) {
        s32 index = *(s32 *)((u8 *)unit + 0xFC);
        if (index != 11) {
            mdlAddEntryFlagged(unit->ext->owner, 0, index);
        } else {
            mdlAddEntryPlain(unit->ext->owner, 0, 11);
        }
        sdfMotionSampleAtFrame(unit->ext->owner->first, 0.0f);
    }
}

u32 btlApplyIndexedUnitEffectTask(u8 *arguments) {
    s32 index = *(s32 *)(arguments + 4);
    if (index >= 0) {
        btlApplyScaledUnitEffectParameter(*(u8 **)arguments, index, *(s32 *)(arguments + 8),
                        *(f32 *)(arguments + 0xC));
    }
    return 1;
}

u8 *btlAllocateIndexedUnitEffectTask(u8 *owner, s32 index, s32 value, f32 scale) {
    u8 *task = btlAllocTask(16);
    u32 *arguments;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u16 *)(task + 0x20) = 9;
    *(void **)(task + 0x4C) = btlApplyIndexedUnitEffectTask;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = index;
    arguments[2] = value;
    *(f32 *)(arguments + 3) = scale;
    return task;
}

u32 btlApplyScaledUnitModelTask(u32 *arg0) {
    btlApplyUnitModelScaledValue(*arg0);
    return 1;
}

void *btlCreateScaledUnitModelTask(u8 *owner) {
    u8 *task = btlAllocTask(4);
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)(task + 0x4C) = btlApplyScaledUnitModelTask;
    *(u16 *)(task + 0x20) = 10;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)btlGetTaskArguments(task) = (u32)owner;
    return task;
}

u32 btlPollThresholdTask(s32 *arguments) {
    if ((s32)btlGetUnitModelValue1C((BtlUnit *)arguments[0]) >= arguments[1]) {
        if ((*(u32 *)(arguments[0] + 0xE8) & 1) == 0) {
            btlResetUnitModelProgress((BtlUnit *)arguments[0]);
        }
        return 1;
    }
    return 0;
}

void *btlScheduleThresholdTask(u8 *owner, u32 threshold) {
    u8 *task = btlAllocTask(8);
    u32 *arguments;

    task[0x10] = BTL_TASK_CONDITION_NEVER;
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0xB;
    *(void **)(task + 0x4C) = btlPollThresholdTask;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = threshold;
    return task;
}

extern void effMiscQuaternionToMatrixVU(void);

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
        BtlEffectResource *table = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->species);
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
    VU0_LOAD_VF(vf10, &unit->orientation);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, &unit->bodyOffset);
    VU0_SCALAR_OP(unit->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_NEGATE_XYZ(vf10);
    VU0_LOAD_VF(vf11, pos);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, pos);
    pos[2] -= unit->zOffset;
    btlSetUnitPosition(args->unit, pos);
    btlUnitFaceTarget(unit, target);
    if (dist < 1.0f) {
        return 1;
    }
    args->count++;
    return 0;
}

extern s32 btlApproachTargetTask(BtlApproachTaskArgs *);

u8 *btlAllocateApproachTargetTask(u8 *owner, s32 index, f32 scale) {
    u8 *task = btlAllocTask(24);
    u32 *arguments;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)(task + 0x4C) = btlApproachTargetTask;
    *(u16 *)(task + 0x20) = 0xE;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = index;
    *(f32 *)(arguments + 3) = scale;
    arguments[2] = 0;
    arguments[5] = 0;
    return task;
}

struct BtlPosLerpTaskArgs {
    s128 from;
    s128 to;
    f32 rate;
    f32 t;
    s32 count;
    BtlUnit *unit;
};

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
        btlSetUnitPosition(unit, &pos);
    } else {
        btlSetUnitPosition(unit, &args->to);
        return 1;
    }
    args->count++;
    return 0;
}

BtlRuntimeTask *btlCreateUnitPositionLerpTowardTargetTask(BtlUnit *unit, f32 *target, f32 scale) {
    BtlRuntimeTask *task = btlAllocTask(0x30);
    BtlPosLerpTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0xC;
    task->ownerId = unit->identity;
    task->callback = btlUpdateUnitPositionInterpolationTask;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->rate = scale;
    args->unit = unit;
    args->t = 0.0f;
    args->count = 0;
    PCP_COPY_VECTOR(&args->from, unit->currentPosition);
    PCP_COPY_VECTOR(&args->to, target);
    return task;
}

extern void effMiscQuaternionNlerpVU(f32);

s32 btlStepUnitRotationNlerp(BtlRotationTaskArgs *args) {
    s128 quat;
    f32 t;
    f32 rate;
    BtlUnit *unit = args->unit;
    if (!(unit->status.flags & 0x80000)) {
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

BtlRuntimeTask *btlCreateUnitRotationInterpolationTask(BtlUnit *unit, f32 *target, f32 scale) {
    BtlRuntimeTask *task = btlAllocTask(0x30);
    BtlRotationTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0xD;
    task->ownerId = unit->identity;
    task->callback = btlStepUnitRotationNlerp;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->rate = scale;
    args->unit = unit;
    args->t = 0.0f;
    args->count = 0;
    PCP_COPY_VECTOR(&args->from, unit->orientation);
    PCP_COPY_VECTOR(&args->to, target);
    return task;
}

extern char D_003A3BC8[];

extern char D_003A3BE8[];

void btlRequestModelOrReuse(u32 *arguments) {
    BtlUnit *object = (BtlUnit *)arguments[0];
    u32 effect = arguments[1];
    u32 model = arguments[2];
    if (((u32)object->status.flags & 2) != 0) {
        return;
    }
    if (btlHasMatchingModel(effect, model)) {
        btlBindActorModel(object, effect, model);
        if (*(char *)(arguments + 3) == 0) {
            btlClearUnitDefeatCandidate(object);
            evtSetUnitAlphaTransition((EvtUnit *)*(u32 *)((u8 *)object + 0x320), 0, 0);
            *(u32 *)((u8 *)object + 0x84) = *(u32 *)((u8 *)object + 0x54) & 0xFFFFFF;
        }
        btlBossDebugPrintf(D_003A3BC8, effect, model);
    } else {
        btlRequestModelAssetByMode(object, effect, model);
        *(u32 *)((u8 *)object + 0x118) |= 1;
        btlBossDebugPrintf(D_003A3BE8, effect, model);
    }
}

extern char D_003A3C08[];

extern s32 btlCheckModelAssetByMode(u8 *, u32, u32);

u32 btlPollModelLoadCompletion(u32 *arguments) {
    BtlUnit *object = (BtlUnit *)arguments[0];
    u32 effect = arguments[1];
    u32 model = arguments[2];
    if (((u32)object->status.flags & 2) == 0) {
        if (!btlCheckModelAssetByMode(object, effect, model)) {
            return 0;
        }
        btlBindActorModel(object, effect, model);
        btlReleaseModelAssetByMode(object, effect, model);
        btlBossDebugPrintf(D_003A3C08, effect, model, object);
    }
    if (*(s8 *)(arguments + 3) == 0) {
        btlClearUnitDefeatCandidate(object);
        evtSetUnitAlphaTransition((EvtUnit *)*(u32 *)((u8 *)object + 0x320), 0, 0);
        *(u32 *)((u8 *)object + 0x84) = *(u32 *)((u8 *)object + 0x54) & 0xFFFFFF;
    }
    *(u32 *)((u8 *)object + 0x118) = (*(u32 *)((u8 *)object + 0x118) & ~1) | 2;
    return 1;
}

BtlRuntimeTask *btlCreateModelLoadPollTask(BtlUnit *owner, u32 index, u32 value, s8 mode) {
    BtlRuntimeTask *task = btlAllocTask(16);
    u32 *arguments;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x18;
    task->flags &= ~BTL_TASK_FLAG_REGISTERED;
    task->ownerId = owner->identity;
    task->onStart = btlRequestModelOrReuse;
    task->callback = btlPollModelLoadCompletion;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = index;
    arguments[2] = value;
    *(s8 *)(arguments + 3) = mode;
    return task;
}

u32 btlReleaseUnitModelTask(u32 *arg0) {
    btlClearUnitDefeatCandidate(*arg0);
    btlReleaseActorModelResources((BtlUnit *)*arg0);
    return 1;
}

void *btlScheduleRefreshTask(u8 *owner) {
    u8 *task = btlAllocTask(4);
    u32 *arguments;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)(task + 0x4C) = btlReleaseUnitModelTask;
    *(u16 *)(task + 0x20) = 0x19;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    return task;
}

/* Task-start callbacks do not return a status to the scheduler. */
INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3BC8);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3BE8);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3C08);

void btlBeginModelChange(u32 argumentsAddress) {
    BtlModelChangeArgs *arguments = (BtlModelChangeArgs *)argumentsAddress;
    BtlUnit *owner = arguments->unit;
    u32 model = arguments->resourceKind;
    u32 variant = arguments->resourceId;
    s32 status = btlHasMatchingModel(model, variant);

    if (status == 0) {
        btlRequestModelAssetByMode((u32)owner, model, variant);
        owner->gunResourceFlags = (owner->gunResourceFlags | 1) & ~2;
        btlBossDebugPrintf("btl:model change start[%X,%X]\n", model, variant);
    }
}

/* Complete model loading, cross-fade the retained actor, and release it. */
u32 func_001D8190(BtlModelChangeArgs *args) {
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
            func_001D4E60(unit->mirror, unit);
            if (unit->unkCC == 0) {
                unit->mirror->unkCC = 0;
                unit->unkCC = 1;
            }
            btlSetUnitPosition(unit->mirror, unit->currentPosition);
            btlSetUnitRotation(unit->mirror, unit->orientation);
            btlSetUnitColor(unit->mirror, unit->baseColor, 0);
            unit->mirror->status.flags |= 8;
            args->phase = 1;
            args->elapsed = 0;
        } else {
            args->phase = 2;
        }
        btlReleaseActorModelResources(unit);
        btlRefreshUnitMaximumHpAndClampCurrentHp(&unit->partyRecord);
        btlRefreshUnitMaximumMpAndClampCurrentMp(&unit->partyRecord);
        btlBindActorModel(unit, resourceKind, resourceId);
        btlReleaseModelAssetByMode((u32)unit, resourceKind, resourceId);
        if (args->duration == 0) {
            kwlnDrawControlFlags |= 0x2000000;
        }
        btlSetUnitPosition(unit, unit->currentPosition);
        btlSetUnitRotation(unit, unit->orientation);
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
                btlApplyScaledUnitEffectParameter((u8 *)unit, 0xB,
                    btlGetSlotRateKind((u8 *)unit, 0xB), 1.0f);
            } else if (args->transitionMode == 2) {
                unit->unkEC = -1;
                btlApplyScaledUnitEffectParameter((u8 *)unit, 0xE,
                    btlGetSlotRateKind((u8 *)unit, 0xE), 1.0f);
            } else if (args->transitionMode != 3 &&
                       ((unit->status.flags & 0x200) || (entryFlags & 0x200)) &&
                       args->resourceId != 0x1F &&
                       !(unit->partyRecord.status & 0x2000)) {
                unit->unkEC = -1;
                if (unit->status.flags & 0x1000) {
                    btlApplyScaledUnitEffectParameter((u8 *)unit, 0x10,
                        btlGetSlotRateKind((u8 *)unit, 0x10), 1.0f);
                } else {
                    btlApplyScaledUnitEffectParameter((u8 *)unit, 0x11,
                        btlGetSlotRateKind((u8 *)unit, 0x11), 1.0f);
                }
            }
        }
        if ((u32)args->elapsed < args->duration) {
            if (args->transitionMode == 0) {
                alpha = (u32)((f32)args->elapsed / (f32)args->duration * 128.0f);
                color = unit->baseColor & 0xFFFFFF;
                unit->overlayColor = (alpha << 24) | color;
                unit->mirror->overlayColor = ((128 - alpha) << 24) | color;
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
            btlDestroyUnit((u8 *)unit->mirror);
            unit->mirror = NULL;
            if (unit->unk330 != 0) {
                sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)unit->unk330);
                unit->unk330 = 0;
                unit->unk32C = 0;
            }
        }
        btlBossDebugPrintf("btl:model change end[%X,%X]\n", resourceKind, resourceId);
        return 1;
    }
    args->elapsed++;
    return 0;
}

extern u32 func_001D8190(BtlModelChangeArgs *);

u8 *btlCreateModelChangeTask(u8 *unit, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5) {
    u8 *task = btlAllocTask(0x1C);
    BtlModelChangeArgs *args;
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u16 *)(task + 0x20) = 0x1A;
    *(u16 *)(task + 0x24) &= ~BTL_TASK_FLAG_REGISTERED;
    *(u64 *)(task + 0x40) = *(u64 *)(unit + 0x108);
    *(void **)(task + 0x48) = btlBeginModelChange;
    *(void **)(task + 0x4C) = func_001D8190;
    args = btlGetTaskArguments(task);
    args->unit = (BtlUnit *)unit;
    args->resourceKind = arg1;
    args->resourceId = arg2;
    args->delay = arg3;
    args->duration = arg4;
    args->transitionMode = arg5;
    args->phase = 0;
    args->elapsed = 0;
    return task;
}

void btlApplyLinkedUnitStatusWhenActorActive(s32 arg0) {
    if (((u32)((BtlUnit *)*(u32 *)(arg0 + 0xc))->status.flags & 2) != 0) {
        evtSetUnitStatusFlags(((BtlUnit *)*(u32 *)(arg0 + 0xc))->ext);
        return;
    }
}

u32 btlApplyUnitFxWhenLoaded(u32 *arg0) {
    if ((btlUnitStatusPair(((BtlUnit *)arg0[3])) & 0x1000000002) == 0x1000000002) {
        evtInitializeUnitColorTransition(((BtlUnit *)arg0[3])->ext, arg0[2], *arg0, arg0[1]);
    }
    return 1;
}

u8 *btlCreateUnitTask0F(u8 *unit, s32 arg1, s32 arg2, s32 arg3) {
    u8 *task = btlAllocTask(16);
    u32 *args;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0xF;
    *(u64 *)(task + 0x40) = *(u64 *)(unit + 0x108);
    *(void **)(task + 0x48) = btlApplyLinkedUnitStatusWhenActorActive;
    *(void **)(task + 0x4C) = btlApplyUnitFxWhenLoaded;
    args = btlGetTaskArguments(task);
    args[3] = (u32)unit;
    args[0] = arg1;
    args[1] = arg2;
    args[2] = arg3;
    return task;
}

void btlPrepareUnitStatusFxOnStart(s32 arg0) {
    if (((u32)((BtlUnit *)*(u32 *)(arg0 + 0x14))->status.flags & 2) != 0) {
        evtSetUnitStatusFlags(((BtlUnit *)*(u32 *)(arg0 + 0x14))->ext);
        return;
    }
}

u32 btlApplyUnitVectorFxWhenLoaded(u8 *arguments) {
    BtlUnit *object = *(BtlUnit **)(arguments + 0x14);
    if (((u32)object->status.flags & 2) != 0) {
        VU0_LOAD_VF(vf10, arguments);
        evtSetUnitNormalizedDirection(object->ext, *(s32 *)(arguments + 0x10));
    }
    return 1;
}

u8 *btlCreateUnitTask10(u8 *unit, f32 *spawnPosition, s32 value) {
    u8 *task = btlAllocTask(0x18);
    u32 *args;
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u16 *)(task + 0x20) = 0x10;
    *(u64 *)(task + 0x40) = *(u64 *)(unit + 0x108);
    *(void **)(task + 0x48) = btlPrepareUnitStatusFxOnStart;
    *(void **)(task + 0x4C) = btlApplyUnitVectorFxWhenLoaded;
    args = btlGetTaskArguments(task);
    args[5] = (u32)unit;
    args[4] = value;
    PCP_COPY_VECTOR(args, spawnPosition);
    return task;
}

u32 btlUnitFadeInTask(BtlFadeArgs *args) {
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

extern u32 btlUnitFadeInTask(BtlFadeArgs *);

u8 *btlCreateUnitFadeInTask(u8 *owner, u32 value, u32 variant) {
    u8 *task = btlAllocTask(20);
    u32 *arguments;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)(task + 0x4C) = btlUnitFadeInTask;
    *(u16 *)(task + 0x20) = 0x11;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[4] = 0x80808080;
    arguments[1] = value;
    arguments[2] = variant;
    arguments[3] = 0;
    return task;
}

u32 btlUnitFadeOutTask(BtlFadeArgs *args) {
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

extern u32 btlUnitFadeOutTask(BtlFadeArgs *);

u8 *btlCreateUnitFadeOutTask(u8 *owner, u32 value, u32 variant) {
    u8 *task = btlAllocTask(16);
    u32 *arguments;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)(task + 0x4C) = btlUnitFadeOutTask;
    *(u16 *)(task + 0x20) = 0x12;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = variant;
    arguments[3] = 0;
    return task;
}

u32 btlStepUnitDefeatFadeIn(u32 *arguments) {
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
        evtSetUnitRgbTransition((struct EvtUnit *)unit->ext, 0, unit->overlayColor);
        evtSetUnitAlphaTransition((struct EvtUnit *)unit->ext, 0,
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

u8 *func_001D9038(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0x10] = BTL_TASK_CONDITION_NEVER;
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x13;
    *(void **)(task + 0x4C) = btlStepUnitDefeatFadeIn;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = 0;
    return task;
}

u32 btlStepUnitDefeatFadeOut(u32 *arguments) {
    BtlUnit *unit = (BtlUnit *)arguments[0];
    u32 alpha;

    if (arguments[2] == 0) {
        btlFlagUnitDefeatCandidate(unit);
    }
    if ((s32)arguments[2] >= (s32)arguments[1]) {
        unit->status.flags = (u32)unit->status.flags & (~0x20000);
        *(u32 *)((u8 *)unit + 0x84) = *(u32 *)((u8 *)unit + 0x54) & 0xFFFFFF;
        return 1;
    }
    unit->status.flags = (u32)unit->status.flags | (0x20000);
    alpha = (u32)((1.0f - (f32)(s32)arguments[2] / (f32)(s32)arguments[1]) * 128.0f);
    alpha <<= 24;
    *(u32 *)((u8 *)unit + 0x84) = alpha | (*(u32 *)((u8 *)unit + 0x54) & 0xFFFFFF);
    arguments[2]++;
    return 0;
}

u8 *func_001D91E0(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0x10] = BTL_TASK_CONDITION_NEVER;
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x14;
    *(void **)(task + 0x4C) = btlStepUnitDefeatFadeOut;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = 0;
    return task;
}

u32 func_001D9268(u32 *arguments) {
    BtlUnit *unit = (BtlUnit *)arguments[0];
    s32 finished = 0;
    u32 alpha;

    if (arguments[2] == 0) {
        unit->status.flags = (u32)unit->status.flags | (0x80);
        btlFlagUnitDefeatCandidate(unit);
    }

    switch (arguments[1]) {
    case 0:
        if (arguments[2] == 1) {
            btlFlagUnitDefeatCandidate(unit);
            evtSetUnitRgbTransition(*(struct EvtUnit **)((u8 *)unit + 0x320),
                                    10, 0x80000000);
        } else if (arguments[2] == 10) {
            evtSetUnitRgbTransition(*(struct EvtUnit **)((u8 *)unit + 0x320),
                                    0, 0x80000000);
            evtSetUnitAlphaTransition(*(struct EvtUnit **)((u8 *)unit + 0x320), 6, 0);
            unit->status.flags = (u32)unit->status.flags | (0x200000);
        }
        if (((u32)unit->status.flags & 0x200000) == 0 &&
            (s32)arguments[2] >= 16) {
            finished = 1;
        }
        break;

    case 1:
        if ((s32)arguments[2] >= 8) {
            unit->status.flags = (u32)unit->status.flags & (~0x20000);
            *(u32 *)((u8 *)unit + 0x84) = *(u32 *)((u8 *)unit + 0x54) & 0xFFFFFF;
            finished = 1;
        } else {
            unit->status.flags = (u32)unit->status.flags | (0x20000);
            alpha = (u32)((1.0f - (f32)(s32)arguments[2] * 0.125f) * 128.0f);
            alpha <<= 24;
            *(u32 *)((u8 *)unit + 0x84) = alpha | (*(u32 *)((u8 *)unit + 0x54) & 0xFFFFFF);
        }
        break;
    }

    arguments[2]++;
    if (finished != 0) {
        unit->status.flags = ((u32)unit->status.flags & ~0x80) | 0x40;
        return 1;
    }
    return 0;
}

u8 *func_001D9468(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0x10] = BTL_TASK_CONDITION_NEVER;
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x15;
    *(void **)(task + 0x4C) = func_001D9268;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = 0;
    return task;
}

typedef struct UnitEffectTaskArgs {
    BtlUnit *unit;
    SoundMixer *mixer;
    BattleEffect *effect;
    s32 duration;
    s32 counter;
} UnitEffectTaskArgs;

extern void func_00160D88(BattleEffect *);

/* Start from the selected-unit SYSEFF source, then update through its duration.
 * Return one for an ineligible unit or expiry, zero while updating. */
u32 btlUpdateSelectedUnitEffect(UnitEffectTaskArgs *arguments) {
    BtlUnit *unit = arguments->unit;
    SoundResourceNode *work;
    void *handle;

    if (!(unit->status.flags & 2)) {
        return 1;
    }
    {
        BtlState *battle = (BtlState *)btlGetRuntime();
        if (arguments->effect == 0) {
            work = battle->resources[BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT];
            handle = work->sourceHandle;
            unit->status.flags |= 0x80;
            arguments->mixer = sndMixerClone(handle);
            arguments->effect = func_00160958(arguments->mixer, 2, unit, 0);
            arguments->duration = 0xE;
            arguments->effect->flags &= 0xFFF9;
            effBattleUpdateSelectedValue(arguments->effect, 0xE);
            unit->status.flags &= ~8;
            if (unit->status.flags & 2) {
                unit->ext->owner->flags |= MDL_SKIP_TRANSFORMS;
            }
        }
        arguments->counter = arguments->counter + 1;
        if (arguments->counter >= arguments->duration) {
            unit->status.flags = (unit->status.flags & ~0x80) | 0x40;
            return 1;
        }
        func_00160D88(arguments->effect);
        return 0;
    }
}

extern u32 btlUpdateSelectedUnitEffect(UnitEffectTaskArgs *);

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

u8 *btlCreateSelectedEffectUpdateTask(u8 *owner) {
    u8 *task = btlAllocTask(20);
    UnitEffectTaskArgs *arguments;
    BtlUnit *unit = (BtlUnit *)owner;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u16 *)(task + 0x20) = 0x16;
    *(u16 *)(task + 0x24) |= BTL_TASK_FLAG_DEFERRED;
    *(u64 *)(task + 0x40) = unit->identity;
    *(void **)(task + 0x4C) = btlUpdateSelectedUnitEffect;
    *(void **)(task + 0x50) = btlFinishSelectedUnitEffect;
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = unit;
    arguments->effect = 0;
    arguments->counter = 0;
    arguments->duration = 0;
    return task;
}

u32 btlUpdateCommandSoundTask(void) {
    btlUpdateUnitActors();
    return 1;
}

SoundTask *btlCreateCommandSoundUpdateTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback.process = btlUpdateCommandSoundTask;
    task->taskId = 0x1B;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

u32 btlUpdateCommandSoundTaskSecondary(void) {
    btlRefreshUnitEffects();
    return 1;
}

u8 *btlCreateSecondaryCommandSoundTask(void) {
    u8 *task = btlAllocTask(0);
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x1C;
    *(u16 *)(task + 0x24) |= BTL_TASK_FLAG_DEFERRED;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u32 *)(task + 0x48) = 0;
    *(void **)(task + 0x4C) = btlUpdateCommandSoundTaskSecondary;
    return task;
}

u32 func_001D97D0(void) {
    return 1;
}

SoundTask *func_001D97D8(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback.process = func_001D97D0;
    task->taskId = 0x20;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

typedef struct BtlUnitBaseLightArgs {
    BtlUnit *unit;
    s32 delay;
} BtlUnitBaseLightArgs;

extern f32 *D_00324770[];

extern u8 kwlnDefaultColorVector[];

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
                f32 *defaultLight = D_00324770[0];
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

BtlRuntimeTask *btlCreateUnitBaseLightTask(BtlUnit *owner) {
    BtlRuntimeTask *task = btlAllocTask(8);
    BtlUnitBaseLightArgs *arguments;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlUnitBaseLightTask;
    task->taskId = 0x21;
    task->ownerId = owner->identity;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = owner;
    arguments->delay = 0;
    return task;
}

extern char D_003A3CA0[];

extern f32 effMiscRandUnitFloat(void *);

typedef struct BtlDamageShakeArgs {
    BtlUnit *unit;
    f32 amplitude;
    s32 tick;
} BtlDamageShakeArgs;

u32 btlStiffenDamageShakeStep(BtlDamageShakeArgs *task) {
    f32 pos[4] __attribute__((aligned(16)));
    f32 scale;
    s32 node;

    if (!(task->unit->status.flags & 2)) {
        return 1;
    }
    if (task->tick == 0) {
        node = mdlGetNodeMotionIndex(task->unit->ext->owner, 0);
        if (node < 0x1D) {
            BtlActorStatusRecord *resource = (BtlActorStatusRecord *)btlGetSideIndexedActorStatusTable(task->unit->resourceKind,
                                                 task->unit->species);
            if (resource->motions[node].kind == 2) {
                btlRefreshUnitEffectMotionAndEntry(task->unit);
                btlBossDebugPrintf(D_003A3CA0);
            }
        }
    }
    if (0.5f < task->amplitude) {
        BtlUnit *actor;
        scale = task->amplitude * (effMiscRandUnitFloat(effSharedRandomState) * 0.5f + 0.5f);
        if (task->tick & 1) {
            scale = -scale;
        }
        actor = task->unit;
        if (btlUnitStatusPair(actor) & 0x808000000000) {
            effObjFetchInnerPosition(actor->effectObject);
            VU0_STORE_VF_UNCLOBBERED(vf10, pos);
            pos[0] += scale;
        } else {
            func_001D6300((u8 *)actor, pos);
            pos[0] += scale;
            pos[2] += task->unit->zOffset;
        }
        effObjSetInnerPosition(task->unit->effectObject, (u128 *)pos);
        task->amplitude *= 0.85f;
    } else {
        BtlUnit *actor = task->unit;
        if (btlUnitStatusPair(actor) & 0x808000000000) {
            effObjFetchInnerPosition(actor->effectObject);
            VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        } else {
            func_001D6300((u8 *)actor, pos);
            pos[2] += task->unit->zOffset;
        }
        effObjSetInnerPosition(task->unit->effectObject, (u128 *)pos);
        return 1;
    }
    task->tick += 1;
    return 0;
}

extern u32 btlStiffenDamageShakeStep(BtlDamageShakeArgs *);

u8 *btlCreateStiffenDamageShakeTask(u8 *owner, f32 value) {
    u8 *task = btlAllocTask(sizeof(BtlDamageShakeArgs));
    BtlDamageShakeArgs *arguments;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u16 *)(task + 0x20) = 0x1D;
    *(void **)(task + 0x4C) = btlStiffenDamageShakeStep;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = (BtlUnit *)owner;
    arguments->amplitude = value;
    arguments->tick = 0;
    return task;
}

typedef struct BtlPositionEffectArgs {
    BtlUnit *unit;
    s32 tick;
    f32 amount;
    f32 velocity;
} BtlPositionEffectArgs;

u32 func_001D9C28(BtlPositionEffectArgs *task) {
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
        if (runtime->allowPositionEffect != NULL) {
            approved = runtime->allowPositionEffect(unit);
        }
        if (approved) {
            parameter = 13;
            if (runtime->chooseMotion != NULL) {
                parameter = runtime->chooseMotion(unit, 13, 0);
            }
            if (parameter != -1) {
                btlApplyScaledUnitEffectParameter((u8 *)unit, parameter, 0, 1.0f);
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
            func_001D6300((u8 *)task->unit, position);
            position[2] += task->unit->zOffset;
            effObjSetInnerPosition(task->unit->effectObject, (u128 *)position);
            return 1;
        }
    }
    func_001D6300((u8 *)task->unit, position);
    position[0] += offset;
    position[2] += task->unit->zOffset;
    effObjSetInnerPosition(task->unit->effectObject, (u128 *)position);
    task->tick++;
    return 0;
}

extern u32 func_001D9C28(BtlPositionEffectArgs *);

u8 *func_001D9E48(u8 *arg0) {
    u8 *task = btlAllocTask(0x10);
    u32 *data;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)(task + 0x4C) = func_001D9C28;
    *(u16 *)(task + 0x20) = 0x1E;
    *(u64 *)(task + 0x40) = *(u64 *)(arg0 + 0x108);
    *(s32 *)(task + 0x48) = 0;
    data = btlGetTaskArguments(task);
    data[0] = (u32)arg0;
    data[1] = 0;
    return task;
}

u32 func_001D9EC0(s32 *arg0) {
    BtlUnit *temp_v0;

    temp_v0 = (BtlUnit *)(u32)*arg0;
    temp_v0->status.flags = (u32)temp_v0->status.flags & 0xffffffef;
    btlSetUnitRotation(temp_v0, (u8 *)temp_v0 + 0x40);
    return 1;
}

void *btlScheduleActorUpdate(u8 *owner) {
    u8 *task = btlAllocTask(4);
    u32 *arguments;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)(task + 0x4C) = func_001D9EC0;
    *(u16 *)(task + 0x20) = 0x1F;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    return task;
}

u32 btlRefreshUnitFxVectorTask(u32 *arg0) {
    btlRefreshUnitFxVectors(*arg0);
    return 1;
}

void *btlCreateUnitFxVectorRefreshTask(u8 *owner) {
    u8 *task = btlAllocTask(4);
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)(task + 0x4C) = btlRefreshUnitFxVectorTask;
    *(u16 *)(task + 0x20) = 0x22;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)btlGetTaskArguments(task) = (u32)owner;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3CA0);

void btlStartGunFinishLoad(s32 task) {
    char filename[0x70];
    BtlUnit *actor = *(BtlUnit **)task;
    if (((u32)actor->status.flags & 0x400) != 0) {
        return;
    }
    if (*(s32 *)((u8 *)actor + 0x30C) != 0) {
        sdfFreeMemoryFromEitherHeap((void *)(u32)(*(u32 *)((u8 *)actor + 0x30C)));
        *(s32 *)((u8 *)actor + 0x30C) = 0;
    }
    if (btlFormatUnitBedName(actor, filename)) {
        s32 handle = (s32)fileQueueAlternateCallbackRequest(filename);
        *(s32 *)(task + 4) = handle;
        btlBossDebugPrintf("btl:gun & finish load start[%s][%p]\n", filename, handle);
    }
    *(u32 *)((u8 *)actor + 0x118) = (*(u32 *)((u8 *)actor + 0x118) | 4) & ~8;
}

typedef struct GunLoadArgs {
    BtlUnit *unit;
    s32 handle;
} GunLoadArgs;

u32 btlPollGunLoad(u32 *arg) {
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

extern void btlStartGunFinishLoad(s32);

extern u32 btlPollGunLoad(u32 *);

u8 *btlCreateGunLoadPollTask(u8 *owner) {
    u8 *task = btlAllocTask(8);
    u32 *arguments;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u16 *)(task + 0x20) = 0x23;
    *(u16 *)(task + 0x24) &= ~BTL_TASK_FLAG_REGISTERED;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = btlStartGunFinishLoad;
    *(void **)(task + 0x4C) = btlPollGunLoad;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = 0;
    return task;
}

u32 btlUpdateUnitEffectsTask(void) {
    btlUpdateUnitEffects();
    return 1;
}

SoundTask *btlCreateUpdateUnitEffectsTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback.process = btlUpdateUnitEffectsTask;
    task->taskId = 0x24;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

extern void btlCreateUnitTransparency(BtlUnit *);

typedef struct BtlActorTransparencyArgs {
    BtlUnit *unit;
} BtlActorTransparencyArgs;

s32 btlCreateActorTransparency(BtlActorTransparencyArgs *arguments) {
    BtlUnit *actor = arguments->unit;
    if ((actor->status.flags & 2) == 0) {
        return 0;
    }
    btlCreateUnitTransparency(actor);
    arguments->unit->status.flags |= 0x20000;
    return 1;
}

void *btlCreateActorTransparencyTask(BtlUnit *actor) {
    u8 *task = btlAllocTask(4);
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x25;
    *(void **)(task + 0x4C) = btlCreateActorTransparency;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u32 *)(task + 0x48) = 0;
    ((BtlActorTransparencyArgs *)btlGetTaskArguments(task))->unit = actor;
    return task;
}

s32 func_001DA2E0(BtlActorModelBlendArgs *args) {
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
        unit->effectTimerA = 0;
        unit->effectTimerB = 1;
        btlApplyScaledUnitEffectParameter(unit, args->index, args->value, args->scale);
    } else {
        if (unit != target) {
            btlWarpUnitToMotionReach(unit, target, args->previousModelValue);
        }
        return 1;
    }
    args->stage++;
    return 0;
}

BtlRuntimeTask *btlCreateActorModelBlendTask(BtlUnit *actor, BtlUnit *target, s32 index, s32 value, f32 scale) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(BtlActorModelBlendArgs));
    BtlActorModelBlendArgs *arguments;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = func_001DA2E0;
    task->taskId = 0x26;
    task->ownerId = actor->identity;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = actor;
    arguments->target = target;
    arguments->index = index;
    arguments->value = value;
    arguments->scale = scale;
    arguments->previousModelValue = -1;
    arguments->stage = 0;
    return task;
}

/* Update motion completion, alpha transitions, and the selected-unit color pulse. */
void func_001DA468(void) {
    BtlState *runtime = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    BtlActorStatusRecord *status;
    s32 parameter;
    s32 frame;
    s32 index;
    f32 pulse;
    u32 packed[4]; /* SDK color workspace; the low word holds RGBA8888. */

    if (runtime->beforeMotionUpdate != NULL) {
        runtime->beforeMotionUpdate();
    }
    for (unit = runtime->units; unit != NULL; unit = unit->next) {
        if (!(unit->status.flags & 0x600) || !(unit->status.flags & 2)) {
            continue;
        }
        status = (BtlActorStatusRecord *)btlGetSideIndexedActorStatusTable(
            unit->resourceKind, unit->species);
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
                btlApplyScaledUnitEffectParameter((u8 *)unit, parameter, 0, 1.0f);
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
                                (status->motions[index].alphaFrameScale * runtime->modelFrameScale)),
                            unit->overlayColor & 0xFFFFFF);
                        unit->updateFlags &= ~4;
                    }
                }
            }
            unit->overlayColor = mdlGetBroadcastValue(unit->ext->owner);
        }
        if (unit->status.flags & 0x8000) {
            pulse = (f32)(runtime->frame % 30) / 15.0f;
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

void btlUpdateActorModelColorAndLinks(void) {
    BtlUnit *unit = ((BtlActorWork *)btlGetRuntime())->actorList;
    s32 color;

    for (; unit != 0; unit = unit->next) {
        if (unit->status.flags & 2) {
            unit->unkEC = mdlGetNodeMotionIndex(unit->ext->owner, 0);
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
                    func_001D6FB0(unit);
                } else {
                    btlUpdateUnitTransparency(unit);
                }
            }
            func_001F2818(unit->resourceLink);
            btlUpdateUnitCommandEffect(unit->link);
            func_001FC998(unit);
        }
    }
}

void btlResetUnitLinks(BtlUnit *actor) {
    actor->selectedEntryIndex = -1;
    actor->unk2F4 = -1;
    actor->status.flags = 0;
    actor->status.stateFlags = 0;
    actor->gunResourceFlags = 0;
    actor->effectLink.flags = 0;
    btlClearAllActorEntrySlots(actor);
    actor->resourceLink = sndAllocResourceLink(actor);
    actor->link = sndAllocLink(actor);
}

BtlUnit *btlCreateUnit(void) {
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(0x348);
    BtlUnit *unit = (BtlUnit *)sdfResourceRetainAddress(allocation);
    BtlActorWork *work;
    memset(unit, 0, 0x348);
    unit->handle = (u32)allocation;
    unit->identity = btlAdvanceRuntimeSequenceCounter();
    unit->status.flags = 0;
    unit->status.stateFlags = 0;
    unit->lookupId = unit->selectedEntryIndex = -1;
    unit->unk2C4 = 6;
    unit->gunResourceFlags = 0;
    unit->effectLink.referenceCount = 0;
    unit->resourceNode = 0;
    unit->gunResource = 0;
    unit->effectObject = 0;
    unit->ext = 0;
    btlInitUnitFxDefaults((u8 *)unit);
    btlInitFxLights((u8 *)unit);
    btlResetUnitLinks(unit);
    work = (BtlActorWork *)btlGetRuntime();
    unit->previousActor = 0;
    if (work->actorList != 0) {
        work->actorList->previousActor = unit;
        unit->next = work->actorList;
    } else {
        unit->next = 0;
    }
    work->actorList = unit;
    btlBossDebugPrintf("btl:unit create[%p]\n", unit);
    return unit;
}

void btlReleaseUnitResources(BtlUnit *unit) {
    btlBossDebugPrintf("btl:unit data free[%p]\n", unit);
    if (unit->resourceNode != 0) {
        sndFreeResourceNode(unit->resourceNode);
        unit->resourceNode = 0;
    }
    if (unit->resourceLink != 0) {
        sndFreeResourceLink(unit->resourceLink);
        unit->resourceLink = 0;
    }
    if (unit->link != 0) {
        sndFreeLink(unit->link);
        unit->link = 0;
    }
    if (unit->gunResource != 0) {
        sdfFreeMemoryFromEitherHeap(unit->gunResource);
        unit->gunResource = 0;
        unit->gunResourceFlags &= ~4;
        unit->gunResourceFlags &= ~8;
    }
    if (unit->listNode != 0) {
        sndFreeListNode(unit->listNode);
        unit->listNode = 0;
    }
    btlReleaseActorModelResources(unit);
    if (unit->unk330 != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)unit->unk330);
        unit->unk330 = 0;
        unit->unk32C = 0;
    }
}

extern char D_003A3D38[]; /* "btl:unit delete[%p]\n" */

void btlDestroyUnit(u8 *actor) {
    u8 *next;
    u8 *previous;

    btlBossDebugPrintf(D_003A3D38, actor);
    btlReleaseUnitResources(actor);
    next = *(u8 **)(actor + 0x344);
    if (next != 0) {
        *(u8 **)(next + 0x340) = *(u8 **)(actor + 0x340);
    }
    previous = *(u8 **)(actor + 0x340);
    if (previous != 0) {
        *(u8 **)(previous + 0x344) = *(u8 **)(actor + 0x344);
    } else {
        *(u8 **)(btlGetRuntime() + 0x228) = *(u8 **)(actor + 0x344);
    }
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(*(s32 *)(actor + 0x33C)));
}

void btlDestroyAllUnits(void) {
    BtlUnit *unit;
    BtlUnit *next;
    for (unit = ((BtlActorWork *)btlGetRuntime())->actorList; unit != 0; unit = next) {
        next = unit->next;
        btlDestroyUnit((u8 *)unit);
    }
}

void btlRemoveActorsWithFlags(u32 mask) {
    s32 context = btlGetRuntime();
    BtlUnit *actor = *(BtlUnit **)(context + 0x228);
    while (actor != 0) {
        BtlUnit *next = *(BtlUnit **)((u8 *)actor + 0x344);
        if ((u32)actor->status.flags & mask) {
            btlDestroyUnit(actor);
        }
        actor = next;
    }
}

BtlUnit *btlFindActorForOwner(s64 target) {
    BtlState *context = (BtlState *)btlGetRuntime();
    BtlUnit *actor = context->units;
    while (actor != 0) {
        if (actor->identity == target) {
            return actor;
        }
        actor = actor->next;
    }
    return 0;
}

s32 btlIsActiveActor(s32 candidate) {
    s32 context = btlGetRuntime();
    s32 actor = *(s32 *)(context + 0x228);
    while (actor != 0) {
        if (actor == candidate) {
            return 1;
        }
        actor = *(s32 *)(actor + 0x344);
    }
    return 0;
}

s32 btlFindUnitByModeClear(s32 arg0) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);

    while (node != 0) {
        if (((*(u16 *)(node + 0x120) & 0x20) == 0) && (*(u16 *)(node + 0x124) == arg0)) {
            return node;
        }
        node = *(s32 *)(node + 0x344);
    }
    return 0;
}

s32 btlFindUnitByModeFlagged(s32 arg0) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);

    while (node != 0) {
        if (((*(u16 *)(node + 0x120) & 0x20) != 0) && (*(u16 *)(node + 0x124) == arg0)) {
            return node;
        }
        node = *(s32 *)(node + 0x344);
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
    u32 index;

    for (index = 0; index < count; index++) {
        if (entry == btlGetIndexListEntry(list, index)) {
            return index;
        }
    }
    return -1;
}

extern void mdlSetAmountOnAllContextResources(MdlCtx *, f32);

void btlApplyUnitEffectScale(BtlUnit *unit) {
    ObjectTransform *inner;
    if (unit->status.flags & 2) {
        btlInitializeEffectVectorsFromSourceRecords(unit, unit->resourceKind, unit->species);
        VU0_SET_ONES_XYZ(vf10);
        VU0_SCALAR_OP(unit->effectScale, "vmulx.xyzw vf10, vf10, vf2x");
        inner = unit->effectObject->inner;
        inner->flags |= OBJECT_TRANSFORM_FLAG_UPDATE_PENDING;
        inner->flags &= ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
        VU0_STORE_VF(vf10, inner->scale);
        mdlStoreTertiaryVectorVU(unit->ext->owner);
        mdlSetAmountOnAllContextResources(unit->ext->owner, unit->effectScale);
        btlSetUnitPosition(unit, (u8 *)unit->currentPosition);
    }
}

INCLUDE_ASM(const s32, "game/code_001D0BA0", func_001DB048);

void btlNormalizeActionCameraKeyScales(s32 action) {
    f32 *key = (f32 *)action;
    u32 flags = *(u32 *)(action + 0xF0);
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

u32 func_001DB358(BtlLinkedCommand *command) {
    return 1;
}

u32 func_001DB360(BtlLinkedCommand *command) {
    return 1;
}

u32 func_001DB368(BtlLinkedCommand *command) {
    return 1;
}

void func_001DB370(BtlCamState *dst, BtlCamState *current, BtlCamState *target, f32 blend) {
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

extern void func_001DB370(BtlCamState *, BtlCamState *, BtlCamState *, f32);

s32 btlStepPoseBlendHalf(BtlLinkedCommand *command) {
    f32 blend;
    if (command->state == 0) {
        command->progress = 0.0f;
        btlScalarRangeInitQuadratic(&command->quadraticRange, (f32)(command->durationFrames * 2));
        btlCopyMotionTransform(&command->camera,
                               &command->frontCamera);
        return 0;
    }
    blend = btlScalarRangeStepQuadratic(&command->quadraticRange, 1.0f);
    if (blend > 0.5f) {
        blend = 0.5f;
    }
    func_001DB370(&command->camera, &command->frontCamera,
                  &command->backCamera, 2.0f * blend);
    command->progress = blend;
    if (blend >= 0.5f) {
        return 1;
    }
    return 0;
}

extern void func_001DB370(BtlCamState *, BtlCamState *, BtlCamState *, f32);

s32 btlStepPoseBlend(BtlLinkedCommand *command) {
    BtlExponentialRange *motion = &command->exponentialRange;
    BtlCamState *from = &command->frontCamera;
    f32 value;
    if (command->state == 0) {
        btlScalarRangeSetStartClearEnd(motion, command->motionParameter);
        btlCopyMotionTransform(&command->camera, from);
    }
    value = btlScalarRangeStepExponential(motion);
    func_001DB370(&command->camera, from,
                  &command->backCamera, value);
    command->progress = value;
    return 0.9999990f <= value;
}

s32 btlStepPoseBlendFrame(BtlLinkedCommand *command) {
    f32 value;
    if (command->state == 0) {
        command->progressBits = 0;
        btlScalarRangeInitQuadratic(&command->quadraticRange, (f32)command->durationFrames);
        btlCopyMotionTransform(&command->camera,
                               &command->frontCamera);
        return 0;
    }
    value = btlScalarRangeStepQuadratic(&command->quadraticRange, 1.0f);
    func_001DB370(&command->camera, &command->frontCamera,
                  &command->backCamera, value);
    command->progress = value;
    return 0.9999990f <= value;
}

s32 btlStepPoseBlendRatio(BtlLinkedCommand *command) {
    f32 ratio = (f32)command->state / (f32)command->durationFrames;
    if (ratio <= 1.0f) {
        func_001DB370(&command->camera, &command->frontCamera,
                      &command->backCamera, ratio);
        return 0;
    }
    btlCopyMotionTransform(&command->camera,
                           &command->backCamera);
    return 0;
}

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

u32 btlExecuteCommandSoundTask(u32 *arg0) {
    func_001DB048(arg0[3], *arg0, arg0[1], arg0[2], arg0[4]);
    return 1;
}

void *btlCreateCommandSoundTask(s32 owner, s32 variant) {
    SoundTask *task = (SoundTask *)btlAllocTask(20);
    u32 *arguments;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x27;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    if (owner != 0 && *(s32 *)(owner + 0x18) != 0) {
        task->owner = ((BtlTask *)owner)->unit->identity;
    }
    task->callback.commandSound = btlExecuteCommandSoundTask;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = owner;
    arguments[3] = variant;
    arguments[1] = 0;
    arguments[2] = 0;
    arguments[4] = 0;
    return task;
}

void *btlCreateTargetedCommandSoundTask(s32 owner, s32 variant, u32 target) {
    void *task = btlCreateCommandSoundTask(owner, variant);
    u32 *arguments = btlGetTaskArguments(task);
    arguments[4] = target;
    return task;
}

void *btlCreateCommandSoundWithArguments(s32 owner, s32 variant, u32 first, u32 second, u32 third) {
    void *task = btlCreateCommandSoundTask(owner, second);
    u32 *arguments = btlGetTaskArguments(task);
    arguments[4] = third;
    arguments[1] = variant;
    arguments[2] = first;
    return task;
}

u32 btlInitializeMotionTransformFromTaskArguments(u8 *arguments) {
    u8 *context = (u8 *)btlGetRuntime();
    func_001DB048(1, *(u32 *)arguments, 0, 0, 0);
    btlInitMotionTransformFromComponents(&((BtlState *)context)->cameraCommand.camera, *(f32 *)(arguments + 4), *(f32 *)(arguments + 8),
                    *(f32 *)(arguments + 0xC), *(f32 *)(arguments + 0x10),
                    *(f32 *)(arguments + 0x14), *(f32 *)(arguments + 0x18),
                    *(f32 *)(arguments + 0x1C), *(f32 *)(arguments + 0x20));
    return 1;
}

u8 *btlCreateFloatTask28(u8 *actor, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h) {
    u8 *task = btlAllocTask(0x24);
    f32 *args;
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x28;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    if (actor != 0 && *(u8 **)(actor + 0x18) != 0) {
        *(u64 *)(task + 0x40) = *(u64 *)(*(u8 **)(actor + 0x18) + 0x108);
    }
    *(void **)(task + 0x4C) = btlInitializeMotionTransformFromTaskArguments;
    *(u32 *)(task + 0x48) = 0;
    args = btlGetTaskArguments(task);
    *(u8 **)args = actor;
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

extern void func_001DB048(s32, s32, s32, s32, s32);

extern void btlSetEffectCameraKeys(u8 *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

s32 btlApplyEffectCameraKeyframes(f32 *args) {
    s32 context = btlGetRuntime();
    func_001DB048(1, *(s32 *)args, 0, 0, 0);
    btlSetEffectCameraKeys((u8 *)(context + 0x70), args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8],
                  args[9], args[10], args[11], args[12], args[13], args[14], args[15], args[16]);
    return 1;
}

u8 *btlCreateFloatTask29(u8 *actor, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, f32 a10, f32 a11, f32 a12, f32 a13, f32 a14, f32 a15, f32 a16) {
    u8 *task = btlAllocTask(0x44);
    f32 *args;
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x29;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    if (actor != 0 && *(u8 **)(actor + 0x18) != 0) {
        *(u64 *)(task + 0x40) = *(u64 *)(*(u8 **)(actor + 0x18) + 0x108);
    }
    *(void **)(task + 0x4C) = btlApplyEffectCameraKeyframes;
    *(u32 *)(task + 0x48) = 0;
    args = btlGetTaskArguments(task);
    *(u8 **)args = actor;
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

u32 btlRunCameraMotionResetTask(void) {
    BtlState *work = (BtlState *)btlGetRuntime();

    btlResetCameraMotion(&work->cameraCommand);
    return 1;
}

SoundTask *btlScheduleContextReset(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback.process = btlRunCameraMotionResetTask;
    task->taskId = 0x2A;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001D0BA0", func_001DBE68);

extern f32 D_00359EA0[4], D_00359EB0[4];

extern f32 D_00359E80[4], D_00359E90[4];

extern u32 D_003BB660;

extern EffWorldNode *dds3CreateCameraObject(s32, void *, void *);

extern void dds3SetCameraFieldOfView(EffWorldNode *, f32);

extern void effObjSetInnerFloat(EffWorldNode *, f32);

extern void dds3EnsureSlotData(void *);

extern void func_001127A0(EffWorldNode *, s32);

/* vu0 routine: add the battle origin to the default camera position. */
void btlInitializeWorldCamera(void) {
    f32 position[4];
    EffWorldNode *camera;
    CameraData *data;
    BtlState *battle = (BtlState *)btlGetRuntime();

    battle->cameraCommand.camera.fov = 0.6981317f;
    VU0_LOAD_VF(vf10, D_00359EA0);
    VU0_LOAD_VF(vf11, battle->position);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, position);
    camera = dds3GetWorldCameraObject(dds3GetWorldObject());
    if (camera != NULL) {
        effObjSetInnerPosition(camera, (u128 *)position);
        effObjSetInnerRotation(camera, (u128 *)D_00359EB0);
        data = camera->data;
        dds3SetCameraFieldOfView(camera, 0.6981317f);
        data->fovUpdatePending |= 1;
    }
    camera = dds3CreateCameraObject(dds3AdvanceWorldCounter(), D_00359E80, D_00359E90);
    camera->value = (const char *)D_003BB660;
    effObjSetInnerFloat(camera, 10.0f);
    dds3EnsureSlotData(camera);
    func_001127A0(camera, 0);
    dds3SetCameraFieldOfView(camera, 0.6981317f);
    dds3SetWorldCameraObject(dds3GetWorldObject(), camera);
    battle->cameraObject = camera;
    battle->cameraCommand.targetList = btlAllocateIndexList(13);
    battle->battleFlags |= 0x10;
}

void btlClearPendingSoundList(void) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlIndexList *list = work->cameraCommand.targetList;
    if (list != 0) {
        btlFreeIndexList(list);
        work->cameraCommand.targetList = 0;
    }
    work->battleFlags &= ~0x10;
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

extern void effMiscQuaternionToMatrixVU(void);

extern void btlClearRuntimeFlag2000(void);

extern u8 D_0037E110[];

void btlInitMotionTransformFromVectors(BtlCamState *object, f32 *origin, f32 *direction) {
    VU0_LOAD_VF(vf10, direction);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E110);
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

void btlSetEffectCameraKeys(u8 *fx, f32 x0, f32 y0, f32 z0, f32 vx0, f32 vy0, f32 vz0, f32 vw0,
                            f32 x1, f32 y1, f32 z1, f32 vx1, f32 vy1, f32 vz1, f32 vw1,
                            f32 scale, f32 f154) {
    btlInitMotionTransformFromComponents(&((BtlLinkedCommand *)fx)->frontCamera, x0, y0, z0, vx0, vy0, vz0, vw0, scale);
    btlInitMotionTransformFromComponents(&((BtlLinkedCommand *)fx)->backCamera, x1, y1, z1, vx1, vy1, vz1, vw1, scale);
    *(f32 *)(fx + 0x130) = f154;
    *(u32 *)(fx + 0xF0) |= 0x41;
}

u32 btlGetActiveUnitId(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    return *(u32 *)(temp_v0 + 0x174);
}

f32 btlGetPoseBlendProgress(s32 arg0) {
    return *(f32 *)(arg0 + 0x128);
}

s32 btlIsUnitInActiveList(void *unit) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlTask *slot = work->cameraCommand.task;
    u32 count;
    u32 i;
    if (slot != 0 && slot->unit == unit) {
        return 1;
    }
    count = btlGetIndexListCount(work->cameraCommand.targetList);
    for (i = 0; i < count; i++) {
        if (btlGetIndexListEntry(work->cameraCommand.targetList, i) == unit) {
            return 1;
        }
    }
    return 0;
}

void btlResetActiveUnitList(void) {
    BtlState *work;

    work = (BtlState *)btlGetRuntime();
    work->cameraCommand.task = 0;
    work->cameraCommand.flags = work->cameraCommand.flags | 0x400;
    btlClearIndexList(work->cameraCommand.targetList);
}

void btlClearRuntimeFlag2000(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 0x160) = *(u32 *)(temp_v0 + 0x160) & 0xffffdfff;
}

void btlSetRuntimeFlag2000(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 0x160) = *(u32 *)(temp_v0 + 0x160) | 0x2000;
}

u32 btlIsRuntimeFlag2000Clear(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    return ((*(s32 *)(temp_v0 + 0x160) >> 0xd) ^ 1U) & 1;
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
                if (((EffWorldNode *)handle)->next != NULL) {
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

extern s32 D_003BB668;

extern s32 D_003BB664;

s32 btlGetWorldObjectDefault(void) {
    EffWorldNode *camera;
    if (!(((BtlState *)btlGetRuntime())->battleFlags & 2)) {
        return D_003BB668;
    }
    camera = dds3GetWorldCameraObject(dds3GetWorldObject());
    if (camera == NULL) {
        return D_003BB668;
    }
    if (camera->value == 0) {
        return D_003BB664;
    }
    return (s32)camera->value;
}

s32 btlIsWorldMotionIdle(void) {
    s32 context = btlGetRuntime();
    if ((*(u32 *)(context + 0x1F4) & 2) == 0) {
        return 0;
    }
    {
        EffWorldNode *camera = dds3GetWorldCameraObject(dds3GetWorldObject());
        if (camera == NULL) {
            return 0;
        }
        return camera->value == 0;
    }
}

s32 btlGetCameraVectorWork(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    return temp_v0 + 0x70;
}

void btlFlagAllUnitDefeatCandidatesTask(void) {
    btlFlagAllUnitsDefeatCandidate();
}

void btlClearAllUnitDefeatCandidatesTask(void) {
    btlClearAllUnitDefeatCandidates();
}

void btlFlagLinkedGroupDefeatCandidatesTask(BtlLinkedCommand *command) {
    btlFlagMatchingUnitsDefeatCandidate((u32)command->task->unit->status.flags & 0x600);
}

void btlApplyCombinedActorFlags(u8 *resource) {
    u32 flags = 0;
    u32 count = btlGetIndexListCount(*(struct BtlIndexList **)(resource + 0x118));
    u32 index;
    for (index = 0; index < count; index++) {
        BtlUnit *actor = (BtlUnit *)btlGetIndexListEntry(*(struct BtlIndexList **)(resource + 0x118), index);
        flags |= (u32)actor->status.flags & 0x600;
    }
    if (flags != 0) {
        btlFlagMatchingUnitsDefeatCandidate(flags);
    }
}

extern f32 D_00359EC0[];

extern char D_003A3DD0[];

void btlResetCameraMotion(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    f32 current;
    f32 limit;

    if (work->cameraCommand.status == 1 || btlHasSingleLinkedResource(action) != 0) {
        unit = work->units;
        if (unit != 0) {
            f32 fallbackScale = 0.7f;
            for (; unit != 0; unit = unit->next) {
                if (unit->status.flags & 1) {
                    if (unit->status.flags & 0x200) {
                        if (unit->status.flags & 2) {
                            if (unit->ext != 0) {
                                s32 node = mdlGetNodeMotionIndex(unit->ext->owner, 0);
                                if (node == 0xD || node == 0x12) {
                                    current = btlGetUnitModelValue1C(unit);
                                    limit = (f32)btlGetUnitModelFrameCount(unit);
                                    if (unit->partyRecord.unitId < 8) {
                                        limit = limit * D_00359EC0[unit->partyRecord.unitId];
                                    } else {
                                        limit = limit * fallbackScale;
                                    }
                                    if (current < limit) {
                                        sdfMotionSampleAtFrame(unit->ext->owner->first, limit);
                                        btlBossDebugPrintf(D_003A3DD0, unit);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

extern char D_003A3DF0[];

extern char D_003A3E08[];

extern void btlBossDebugPrintfN(s32, s32, s32, s32, ...);

void btlDebugPrintWorldTransform(s32 arg0, u8 *arg1) {
    EffWorldNode *object;

    if (((BtlState *)btlGetRuntime())->battleFlags & 2) {
        object = dds3GetWorldCameraObject(dds3GetWorldObject());
        if (object != 0) {
            btlBossDebugPrintfN(arg0, (s32)arg1, 0, (s32)D_003A3DF0,
                                (double)object->inner->position[0],
                                (double)object->inner->position[1],
                                (double)object->inner->position[2]);
            btlBossDebugPrintfN(arg0, (s32)(arg1 + 0xC), 0, (s32)D_003A3E08,
                                (double)object->inner->rotation[0],
                                (double)object->inner->rotation[1],
                                (double)object->inner->rotation[2],
                                (double)object->inner->rotation[3]);
        }
    }
}

extern f32 func_001F6E28(BtlIndexList *, f32 *, f32 *);

void btlFaceActionParticipantsTowardLinkedTarget(BtlLinkedCommand *action) {
    s128 vec[3];
    s128 *pos;
    BtlUnit *target;
    BtlUnit *first;
    u32 i;
    u32 count = btlGetIndexListCount(action->targetList);
    if (count != 0) {
        target = action->task->unit;
        if (count == 1) {
            first = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
            if (target != 0 && (target->status.flags & 0x600) == (first->status.flags & 0x600)) {
                return;
            }
            btlUnitGetMuzzlePosVU(first);
        } else {
            func_001F6E28(action->targetList, 0, 0);
        }
        pos = &vec[1];
        VU0_STORE_VF(vf10, pos);
        if (target != 0) {
            btlUnitGetMuzzlePosVU(target);
            VU0_STORE_VF(vf10, &vec[0]);
            if (target->status.flags & 0x80000) {
                if (btlAimHorizontalDirectionVU(&vec[0], pos) != 0) {
                    VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
                    btlSetUnitRotation(target, &vec[2]);
                }
            }
        }
        for (i = 0; i < count; i++) {
            btlUnitFaceTarget(btlGetIndexListEntry(action->targetList, i), target);
        }
    }
}

void btlAimLinkedUnitAtMuzzle(u8 *action) {
    s128 vec[3];
    s128 *pos;
    BtlUnit *target;
    u32 count = btlGetIndexListCount(*(struct BtlIndexList **)(action + 0x118));
    if (count != 0) {
        target = *(BtlUnit **)(*(s32 *)(action + 0xF4) + 0x18);
        if (count == 1) {
            btlUnitGetMuzzlePosVU(btlGetIndexListEntry(*(struct BtlIndexList **)(action + 0x118), 0));
        } else {
            func_001F6E28(*(BtlIndexList **)(action + 0x118), 0, 0);
        }
        pos = &vec[1];
        VU0_STORE_VF(vf10, pos);
        if (target != 0) {
            btlUnitGetMuzzlePosVU(target);
            VU0_STORE_VF(vf10, &vec[0]);
            if ((u32)target->status.flags & 0x80000) {
                if (btlAimHorizontalDirectionVU(&vec[0], pos) != 0) {
                    VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
                    btlSetUnitRotation(target, &vec[2]);
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001D0BA0", btlMatchFirstLinkedActorFlags);

extern s32 btlIsActorCategoryMarked(BtlLinkedCommand *);

/* Check actor status, linked-group marks, and owner restrictions before use. */
s32 btlCanUseLinkedActor(BtlLinkedCommand *actor) {
    u32 status = actor->status;
    BtlTask *linked;
    s32 category;
    u32 count;
    BtlOperandGroup *entry;
    u32 i;

    switch (status) {
    case 5:
    case 6:
    case 7:
    case 8:
        break;
    default:
        return 1;
    }
    linked = actor->task;
    if (linked == 0) {
        return 1;
    }
    count = btlGetIndexListCount(linked->indexWork.indices);
    entry = linked->indexWork.groups;
    for (i = 0; i < count; i++, entry++) {
        if (entry->reflected != 0) {
            return 0;
        }
    }
    if (linked->unit->partyRecord.status & 0x480) {
        return 0;
    }
    category = actor->actionCode;
    if (category != 0 && (*(u16 *)(datActionAnimationRecords + category * 32 + 0x1C) & 1)) {
        return 0;
    }
    return 1;
}

/* Return whether a live linked group has its byte at 0x10 marked. */
u32 btlHasMarkedEntry10(u8 *object) {
    BtlTask *resource = ((BtlLinkedCommand *)object)->task;
    u32 count;
    u32 index;
    BtlOperandGroup *entry;
    if (resource == 0) {
        return 0;
    }
    count = btlGetIndexListCount(resource->indexWork.indices);
    entry = resource->indexWork.groups;
    for (index = 0; index < count; index++, entry++) {
        if (entry->inactive != 0) {
            return 1;
        }
    }
    return 0;
}

extern s32 btlGetRuntime(void);

extern f32 btlUnitGetTopY(BtlUnit *);

s32 btlCheckActorDistanceLimit(void) {
    BtlUnit *actor = *(BtlUnit **)(btlGetRuntime() + 0x228);

    while (actor != 0) {
        u32 flags = (u32)actor->status.flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (btlUnitGetTopY(actor) > 400.0f) {
                    return 0;
                }
            }
        }
        actor = *(BtlUnit **)((u8 *)actor + 0x344);
    }
    return 1;
}

/* With a qualifying unit, the span query leaves its last muzzle point in VF10. */
extern f32 func_001F66D8(s32, f32 *, f32 *);

s32 btlIsEntryHeightWithinLimit(void) {
    if (func_001F66D8(0x400, 0, 0) > 600.0f) {
        return 0;
    }
    return 1;
}

/* Find an unmarked linked kind-two slot whose unit is not disabled. */
s32 btlHasIdleLinkedSlotKindTwo(u8 *actor) {
    BtlTask *linked = ((BtlLinkedCommand *)actor)->task;
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
    BtlTask *linked = ((BtlLinkedCommand *)actor)->task;
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
    BtlTask *task;
    BtlUnit *owner;
    s32 index;
    BtlEffectResource *table;
    if (fx->task == 0) {
        return 0;
    }
    if (btlHasSingleLinkedResource(fx) == 0) {
        return 0;
    }
    task = fx->task;
    owner = task->unit;
    index = task->indexWork.slot;
    table = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(owner->resourceKind, owner->species);
    return table->nodes[index].triggerKind == 2;
}

s32 btlHasActorCategoryFlag100(BtlLinkedCommand *action) {
    s32 index = action->actionCode;
    if (index == 0) {
        return 0;
    }
    if ((((BtlActionAnimationRecord *)datActionAnimationRecords)[index].flags & 0x100) == 0) {
        return 0;
    }
    return 1;
}

s32 btlIsActorCategoryTypeTwo(BtlLinkedCommand *action) {
    s32 index;

    index = action->actionCode;
    if (index == 0) {
        return 0;
    }
    return datCommandRecords[index].unk30 == 2;
}

u32 btlCanUseActorCategoryFlag2(BtlLinkedCommand *actor) {
    s32 index;
    if (btlIsActorCategoryMarked(actor) != 0) {
        return 1;
    }
    if (btlCanUseLinkedActor(actor) == 0) {
        return 0;
    }
    index = actor->actionCode;
    if (index == 0) {
        return 0;
    }
    if ((*(u16 *)(datActionAnimationRecords + index * 0x20 + 0x1C) & 2) != 0) {
        return 1;
    }
    return 0;
}

s32 btlHasSingleLinkedResource(BtlLinkedCommand *actor) {
    s32 index = actor->actionCode;
    if (index != 0 && datCommandRecords[index].targetType != 0) {
        return 0;
    }
    return btlGetIndexListCount(actor->targetList) == 1;
}

u32 func_001DD2C0(BtlLinkedCommand *actor) {
    s32 index;
    if (btlCanUseLinkedActor(actor) == 0) {
        return 0;
    }
    index = actor->actionCode;
    if (index == 0) {
        return 0;
    }
    if ((*(u16 *)(datActionAnimationRecords + index * 0x20 + 0x1C) & 4) != 0) {
        return 1;
    }
    return 0;
}

s32 btlIsActorCategoryMarked(BtlLinkedCommand *actor) {
    s32 index = actor->actionCode;
    if (index == 0) {
        return 0;
    }
    return datCommandRecords[index].unk30 == 1;
}

s32 btlHasActorCategoryFlag40(BtlLinkedCommand *action) {
    s32 index = action->actionCode;
    if (index == 0) {
        return 0;
    }
    if ((((BtlActionAnimationRecord *)datActionAnimationRecords)[index].flags & 0x40) == 0) {
        return 0;
    }
    return 1;
}

s32 btlMatchLinkedActorFlags(s32 actor) {
    s32 linked;
    BtlUnit *entry;

    switch (*(u32 *)(actor + 0x104)) {
    case 4:
    case 5:
    case 6:
        break;
    default:
        return 0;
    }
    linked = *(s32 *)(actor + 0xF4);
    if (linked == 0) {
        return 0;
    }
    if (btlGetIndexListCount(*(struct BtlIndexList **)(linked + 0x60)) >= 2) {
        return 0;
    }
    entry = btlGetIndexListEntry(*(struct BtlIndexList **)(linked + 0x60), 0);
    return (((u32)((BtlTask *)linked)->unit->status.flags ^ (u32)entry->status.flags) & 0x600) == 0;
}

s32 btlHasFirstLinkedCategoryFlag1000(BtlLinkedCommand *node) {
    BtlTask *resource = node->task;
    BtlUnit *actor;
    u32 id;
    if (resource == 0) return 0;
    if (btlGetIndexListCount(resource->indexWork.indices) >= 2) return 0;
    actor = btlGetIndexListEntry(resource->indexWork.indices, 0);
    if ((actor->status.flags & 0x400) == 0) return 0;
    id = actor->species;
    if (id >= 0x180) return 0;
    if (datEnemyRecords[id].flags & 0x1000) return 1;
    return 0;
}

u8 func_001DD488(BtlLinkedCommand *action) {
    return action->actionCode == 0x5f;
}

u8 func_001DD498(BtlLinkedCommand *action) {
    return action->actionCode == 0x1a0;
}

void func_001DD4A8(void) {
}

void func_001DD4B0(void) {
}

extern void btlPrepareActionCameraPoseWithActorClearance(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void func_001E4708(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void func_001E4720(BtlCamState *, BtlCamState *, BtlCamState *);

/* Choose the action's camera pose from active ally and enemy height maxima. */
void btlChooseCameraPoseByActorHeights(BtlLinkedCommand *action) {
    BtlUnit *unit;
    s32 enemyCount = 0;
    f32 enemyHeight = 0.0f;
    f32 allyHeight = 0.0f;
    f32 height;

    unit = ((BtlState *)btlGetRuntime())->units;
    for (; unit != NULL; unit = unit->next) {
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
                func_001E4720(&action->camera, &action->frontCamera, &action->backCamera);
            } else {
                btlPrepareActionCameraPoseWithActorClearance(action, &action->frontCamera, &action->backCamera);
            }
            break;
        case 2:
            btlPrepareActionCameraPoseWithActorClearance(action, &action->frontCamera, &action->backCamera);
            break;
        case 3:
            func_001E4708(action, &action->frontCamera, &action->backCamera);
            break;
        }
    } else {
        switch (effMiscRandMod(0, 2)) {
        case 0:
            btlPrepareActionCameraPoseWithActorClearance(action, &action->frontCamera, &action->backCamera);
            break;
        case 1:
            func_001E4708(action, &action->frontCamera, &action->backCamera);
            break;
        }
    }
    action->motionParameter = 100.0f;
    action->flags |= 0x41;
}

void func_001DD678(void) {
}

void func_001DD680(BtlLinkedCommand *command) {
    func_001DF358(command, &command->camera);
}

void func_001DD698(void) {
}

extern void btlFlagUserAndTargetDefeat(BtlLinkedCommand *, BtlLinkedCommand *);

extern void btlInitTargetCursorAndFacing(BtlLinkedCommand *, BtlCamState *);

extern void btlInitializeCameraCursorForActorMode(BtlLinkedCommand *, BtlCamState *);

extern void func_001E2FF8(BtlLinkedCommand *);

extern void btlSetupActionCameraPair(BtlLinkedCommand *);

void btlInitializeLinkedActionCamera(BtlLinkedCommand *action) {
    s32 (*hook)(BtlLinkedCommand *) = ((BtlState *)btlGetRuntime())->cameraStateChangePredicate;
    BtlTask *link;

    action->stepKind = 0;
    link = action->task;
    if (hook != NULL && hook(action) != 0) {
        return;
    }
    if (link->unit->status.flags & 0x200) {
        if (link->unit->status.flags & 0x1000) {
            if (btlHasSingleLinkedResource(action)) {
                action->stepKind = 9;
                btlFlagUserAndTargetDefeat(action, action);
            } else {
                btlInitTargetCursorAndFacing(action, &action->camera);
            }
        } else {
            action->stepKind = 11;
            btlInitializeCameraCursorForActorMode(action, &action->camera);
        }
    } else {
        if (btlMatchLinkedActorFlags((s32)action)) {
            func_001E2FF8(action);
        } else if (btlHasSingleLinkedResource(action)) {
            action->stepKind = 10;
            btlSetupActionCameraPair(action);
        } else {
            btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
            btlAimLinkedUnitAtMuzzle((u8 *)action);
            action->motionParameter = 200.0f;
            action->flags |= 0x41;
        }
        btlResetCameraMotion(action);
    }
}

extern void btlBuildApproachCamera(BtlLinkedCommand *, BtlCamState *);

extern void btlUpdateActionTargetCameraPose(BtlLinkedCommand *);

extern void btlAdvanceCursorForUnmarkedUnit(BtlLinkedCommand *, BtlCamState *);

void func_001DD7E8(BtlLinkedCommand *actor) {
    s32 (*callback)(BtlLinkedCommand *) = *(s32 (**)(BtlLinkedCommand *))(btlGetRuntime() + 0x614);

    if (callback != 0 && callback(actor) != 0) {
        return;
    }

    switch (actor->stepKind) {
    case 9:
        btlBuildApproachCamera(actor, &actor->camera);
        break;
    case 10:
        btlUpdateActionTargetCameraPose(actor);
        break;
    case 11:
        btlAdvanceCursorForUnmarkedUnit(actor, &actor->camera);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001D0BA0", func_001DD890);

extern void btlRefreshActionPoseBlendSnapshot();

extern void btlAimEffectPoseAtUnit();

extern void func_001E0100();

extern void func_001E5718(BtlLinkedCommand *, BtlCamState *);

extern void btlBuildHeightClampedApproachCamera(BtlLinkedCommand *, BtlCamState *);

/* Let the runtime hook handle the actor before dispatching its camera step. */
INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3D38);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3D50);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3D60);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3D70);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3D80);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3D90);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3DD0);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3DF0);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3E08);

void btlDispatchActionCameraStep(u8 *actor) {
    BtlState *runtime = (BtlState *)btlGetRuntime();
    s32 (*callback)(u8 *) = runtime->actionCameraStepHook;

    if (callback != 0 && callback(actor) != 0) {
        return;
    }
    switch (*(u16 *)(actor + 0x10C)) {
    case 2:
        btlRefreshActionPoseBlendSnapshot(actor, actor);
        break;
    case 9:
        btlBuildApproachCamera((BtlLinkedCommand *)actor, &((BtlLinkedCommand *)actor)->camera);
        break;
    case 4:
        btlAimEffectPoseAtUnit(actor, actor);
        break;
    case 5:
        func_001E0100(actor, actor);
        break;
    case 7:
        func_001E5718((BtlLinkedCommand *)actor, &((BtlLinkedCommand *)actor)->camera);
        break;
    case 8:
        btlBuildHeightClampedApproachCamera((BtlLinkedCommand *)actor, &((BtlLinkedCommand *)actor)->camera);
        break;
    }
}

INCLUDE_SDATA(const s32, "game/code_001D0BA0", btlDeferredTaskHead);

INCLUDE_SDATA(const s32, "game/code_001D0BA0", D_003BB5F8);

INCLUDE_SDATA(const s32, "game/code_001D0BA0", D_003BB600);

INCLUDE_SDATA(const s32, "game/code_001D0BA0", D_003BB608);

INCLUDE_SDATA(const s32, "game/code_001D0BA0", D_003BB610);

INCLUDE_SDATA(const s32, "game/code_001D0BA0", D_003BB618);

INCLUDE_SDATA(const s32, "game/code_001D0BA0", D_003BB620);

INCLUDE_SDATA(const s32, "game/code_001D0BA0", D_003BB628);

INCLUDE_SDATA(const s32, "game/code_001D0BA0", D_003BB630);

INCLUDE_SDATA(const s32, "game/code_001D0BA0", D_003BB638);

INCLUDE_SDATA(const s32, "game/code_001D0BA0", D_003BB640);

INCLUDE_SDATA(const s32, "game/code_001D0BA0", D_003BB648);

INCLUDE_SDATA(const s32, "game/code_001D0BA0", D_003BB650);

INCLUDE_SDATA(const s32, "game/code_001D0BA0", D_003BB658);

INCLUDE_SDATA(const s32, "game/code_001D0BA0", D_003BB660);

INCLUDE_SDATA(const s32, "game/code_001D0BA0", D_003BB664);

INCLUDE_SDATA(const s32, "game/code_001D0BA0", D_003BB668);

