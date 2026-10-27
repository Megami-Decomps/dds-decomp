#include "sdk_ps2.h"
#include "dds3_path.h"
#include "sdf_model_packet_callback.h"
#include "btl_motion_transform.h"
#include "btl_actor_transform_parameters.h"
#include "btl_select_transform_parameters_api.h"
#include "common.h"
#include "btl_horizontal_aim_api.h"
#include "btl_actor_command_selector_value_api.h"
#include "dds3_camera_update_flags.h"
#include "file_config_option_api.h"
#include "btl_command_use_error.h"
#include "btl_action_reaction_motion.h"
#include "btl_action_press_result.h"
#include "btl_hunt_mp_recovery_api.h"
#include "btl_action_state_selection.h"
#include "btl_scene_counter_mode.h"
#include "btl_command_target_policy.h"
#include "btl_command_target_eligibility.h"
#include "dds3_object_flags.h"
#include "sdf_vu0.h"
#include "btl_aim_horizontal_direction_clamped_api.h"
#include "btl_set_actor_effect_parameter_api.h"
#include "btl_raise_party_record_status_value.h"
#include "btl_actor_status_table_api.h"
#include "btl_unit_fade_task_args.h"
#include "btl_unit_color_fade_args.h"
#include "btl_special_ability.h"
#include "eff_random_mod.h"
#include "snd_sequence_volume_pan.h"
#include "evt_world.h"
#include "snd_pad.h"
#include "eff_random.h"
#include "btl_stage_task_cleanup.h"
#include "mdl_motion_api.h"
#include "sdf_motion.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "btl_effect_position.h"
#include "btl_actor_entry_code_api.h"
#include "eff_offset_if_owner_flag_clear.h"
#include "sdf_chip.h"
#include "snd_slot.h"
#include "kwln.h"
#include "btl_task_state.h"
#include "btl_task_poll_result.h"
#include "btl_task_condition.h"
#include "sdf_resource.h"
#include "sdf_model.h"
#include "sdf_model_release_api.h"
#include "btl.h"
#include "btl_index_list_search_api.h"
#include "btl_skill_name_index.h"
#include "btl_model_state_ownership.h"
#include "btl_deferred_unit_status_args.h"
#include "btl_position_effect_task_args.h"
#include "btl_position_lerp_task_args.h"
#include "btl_actor_work.h"
#include "btl_find_actor_for_owner_api.h"
#include "sdf_texture_offset_list.h"
#include "btl_state.h"
#include "btl_model_record.h"
#include "btl_task_args.h"
#include "btl_sound.h"
#include "btl_selected_unit_effect_task_args.h"
#include "btl_approach_task_args.h"
#include "btl_unit_base_light_task_args.h"
#include "btl_effect_node.h"
#include "eff_field_color.h"
#include "dds3obj.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "fpu.h"
#include "dat_state.h"
#include "evt_unit.h"
#include "evt_unit_motion_api.h"
#include "evt_unit_color_transition.h"
#include "mdl.h"
#include "mdl_asset_request.h"
#include "btl_action.h"
#include "btl_unit_tasks.h"
#include "sdf.h"
#include "file.h"
#include "dat_command.h"
#include "btl_command_id.h"
#include "file_request_api.h"
#include "dat_affinity.h"
#include "mdl_flag_api.h"
#include "dds3_world_counter.h"
#include "sdf_vu_matrix.h"

/* SYSEFF metadata and runtime registrations share these indices. */
enum { BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT = 0x26 };



extern BtlUnit *btlSelectUnitAtExtremeX(BtlUnit *, BtlIndexList *);

void btlPrepareSavedCommandTargets(BtlTask *, BattleIndexWork *);

extern s32 btlIsUnitInActiveList(BtlUnit *unit);

extern void btlBossDebugPrintf(const char *format, ...);


extern u64 btlStartTask(BtlRuntimeTask *);

extern void btlDispatchStateHandler(void *, s32);

extern s32 btlHasSingleLinkedResource(BtlLinkedCommand *);

extern void btlResetCameraMotion(BtlLinkedCommand *);

extern s32 btlCountTasksForOwner(u64);

extern s32 btlUpdateUnitPositionInterpolationTask(BtlPosLerpTaskArgs *);

typedef struct BtlRotationTaskArgs BtlRotationTaskArgs;

extern s32 btlStepUnitRotationNlerp(BtlRotationTaskArgs *);

extern u64 btlAdvanceRuntimeSequenceCounter(void);

extern BtlRuntimeTask *btlAllocateIndexedUnitEffectTask(BtlUnit *, s32, s32, f32);




extern SndPad dds3PadButtonState;

extern u32 dds3DrawFlag;

extern void btlDestroyUnit(BtlUnit *);

extern void btlInheritActorModelState(BtlUnit *, BtlUnit *);




extern BtlRuntimeTask *btlDeferredTaskTail;

extern BtlRuntimeTask *btlDeferredTaskHead;

extern DatEnemyRecord *datEnemyRecords;

extern void btlApplyScaledUnitEffectParameter(BtlUnit *, s32, s32, f32);
extern BtlActionAnimationRecord *datActionAnimationRecords;

extern EffRandState effSharedRandomState[];

extern void btlResetDeferredTaskQueue(void);

s32 btlAdjustUnitHp(datUnitWork_t *object, s32 delta);

s32 btlAdjustUnitMp(datUnitWork_t *object, s32 delta);

u16 btlRefreshUnitMaximumHpAndClampCurrentHp(datUnitWork_t *object);

u16 btlRefreshUnitMaximumMpAndClampCurrentMp(datUnitWork_t *object);


extern EffRandState effSharedRandomState[];

extern SndPad dds3PadButtonState;

extern void btlInitializeWorldCamera(void);

/* No selected entry is represented by -1. */ void btlClearActorSelectedEntryIndex(BtlUnit *actor);

void btlClearAllActorEntrySlots(BtlUnit *unit);

s32 btlGetActorBedAssetIdFromIndex(s32 itemIndex);

s32 btlGetEntryFlagsUnlessDisabled(datUnitWork_t *entry);

struct evtUnitHandle_t;

extern void evtSetWaitMotionSpeed(struct evtUnitHandle_t *, f32);

extern s32 btlIsCurrentValueBelowQuarterThreshold(BtlUnit *);

extern s32 btlTestActorStatusPredicate(BtlUnit *);

extern void btlApplyUnitModelScaledValue(BtlUnit *);

extern s32 btlIsActorModeAcceptedByBattleHook(BtlUnit *object);

extern void btlApplyUnitMotionSelection(BtlUnit *, u32, s32, f32);

extern void btlRefreshUnitMotionSelection(BtlUnit *unit);

extern void btlRefreshUnitEffectMotionAndEntry(BtlUnit *unit);

extern void evtSetWaitHokanFrame(struct evtUnitHandle_t *, s32, s32);

extern s32 btlGetSlotRateKind(BtlUnit *, s32);

extern BtlRuntimeTask *btlCreateStiffenDamageShakeTask(BtlUnit *, f32);

s32 btlGetLoggedIndexedCommandItem(s32 index);



s32 btlIsUnitDefeatTriggeredByValueDelta(BtlUnit *actor, s32 delta);

/* Set the actor's selected entry index. */ void btlSetActorSelectedEntryIndex(BtlUnit *actor, u32 index);

void btlSetTrackedTaskDisplayMode(s32 mode);

void btlClearPartyRecordStatusBits(datUnitWork_t *record, s32 mask);

void btlApplyMaskedActorEntryAdjustments(BtlUnit *unit, u32 flags, s16 delta);

typedef struct BtlRotationTaskArgs {
    s128 from;
    s128 to;
    f32 rate;
    f32 t;
    s32 count;
    BtlUnit *unit;
} BtlRotationTaskArgs;

typedef char BtlRotationTaskArgsSizeCheck[
    sizeof(BtlRotationTaskArgs) == 0x30 ? 1 : -1];
typedef char BtlRotationTaskArgsRateOffsetCheck[
    (u32)&((BtlRotationTaskArgs *)0)->rate == 0x20 ? 1 : -1];
typedef char BtlRotationTaskArgsTOffsetCheck[
    (u32)&((BtlRotationTaskArgs *)0)->t == 0x24 ? 1 : -1];
typedef char BtlRotationTaskArgsCountOffsetCheck[
    (u32)&((BtlRotationTaskArgs *)0)->count == 0x28 ? 1 : -1];
typedef char BtlRotationTaskArgsUnitOffsetCheck[
    (u32)&((BtlRotationTaskArgs *)0)->unit == 0x2C ? 1 : -1];

extern s32 btlCountTasksByKind(u16 kind);

BtlRuntimeTask *btlCreateActorParameterDeltaTask(BtlUnit *owner, BtlOperandEntry *spec);

void btlResolveActionTargetResults(BtlTask *task, BattleIndexWork *work);

extern BtlRuntimeTask *btlCreateModelChangeTask(BtlUnit *unit, s32 resourceKind, s32 resourceId, s32 delay, s32 duration, u8 transitionMode);

extern s32 btlCountTasksByKind(u16 kind);

extern BtlRuntimeTask *btlCreateGunLoadPollTask(BtlUnit *);

extern BtlRuntimeTask *btlCreateUnitDefeatFadeInTask(BtlUnit *, s32);

extern void btlResetUnitLinks(BtlUnit *);

extern void btlReleaseUnitResources(BtlUnit *);

extern s32 btlCountTasksForOwner(u64);

extern s32 btlCountTasksForOwner(u64);

extern BtlRuntimeTask *btlCreateSelectedEffectUpdateTask(BtlUnit *);

extern BtlRuntimeTask *btlCreateUnitExitFadeTask(BtlUnit *, s32);
extern void btlBossDebugPrintfN(s32, s32, s32, s32, ...);

BtlTask *btlFindUnitByActor(BtlUnit *target);

extern void scrSetGlobalSeenBit(u32 flagId);

void btlDispatchCommandViaHookOrDefault(BtlTask *command, BattleIndexWork *argument) {
    BtlActionStateSelectionResult (*handler)(s32, s32) =
        *(BtlActionStateSelectionResult (**)(s32, s32))((u8 *)btlGetRuntime() + 0x604);

    if (handler != 0) {
        BtlActionStateSelectionResult result = handler((s32)command, (s32)argument);
        if (result != BTL_ACTION_STATE_SELECTION_USE_DEFAULT) {
            btlDispatchStateHandler(command, result);
            return;
        }
    }
    switch (argument->phase) {
    case 1:
        btlDispatchStateHandler(command, 0xD);
        break;
    case 4:
        argument->skillId = btlGetLoggedIndexedCommandItem(argument->reference);
        /* fallthrough */
    case 2:
    case 3:
    case 7:
    case 8:
        if (datCommandSelectors[argument->skillId].kind != 1) {
            btlDispatchStateHandler(command, 0xE);
        } else {
            if ((u32)command->unit->status.flags & 0x200) {
                scrSetGlobalSeenBit((u16)argument->skillId);
            }
            btlDispatchStateHandler(command, 0xF);
        }
        break;
    case 5:
        if ((u32)command->unit->status.flags & 0x1000) {
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
        if ((u32)command->indexWork.linkedUnit->status.flags & 1) {
            btlDispatchStateHandler(command, 0x16);
        } else {
            btlDispatchStateHandler(command, 0x15);
        }
        break;
    case 12:
        btlDispatchStateHandler(command, 0x15);
        break;
    case 6: {
        u32 flags = (u32)command->unit->status.flags;
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

s32 btlIsSupportedCommandKind(const s32 *phase) {
    switch (*phase) {
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

BtlCommandId btlResolveActionOperand(BtlUnit *unit, BattleIndexWork *work) {
    switch (work->phase) {
    case 1:
        if ((btlUnitStatusPair(unit) & 0x1200) == 0x200) {
            return btlGetActorBedAssetIdFromIndex(unit->partyRecord.menuValue);
        }
        if (work->skillId > 0) {
            return work->skillId;
        }
        return 0;
    case 4:
        return btlGetLoggedIndexedCommandItem(work->reference);
    case 2:
    case 3:
    case 7:
    case 8:
        return work->skillId;
    default:
        return BTL_COMMAND_ID_INVALID;
    }
}

u32 btlClassifyActionOperand(BtlUnit *actor, BattleIndexWork *work) {
    switch (work->phase) {
    case 1: {
        u32 count = btlGetIndexListCount(work->indices);
        if ((btlUnitStatusPair(actor) & 0x1200) == 0x1200 &&
            (actor->partyRecord.status & 0x1000) == 0 &&
            count == 1) {
            BtlOperandGroup *group = work->groups;
            if (group->parameter == 2 && group->reflected == 0) {
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
        return datActionAnimationRecords[work->skillId].cameraKind;
    default:
        return 0;
    }
}

BtlActionReactionMotionIndex btlClassifyActionResult(
    BtlUnit *actor, u32 actionKind, BtlActionPressResult press,
    u32 elementMask, s32 hitCount, u8 groupInactive, s32 commandIndex) {
    BtlActionReactionMotionIndex motionIndex;

    btlGetEntryFlagsUnlessDisabled(&actor->partyRecord);
    if (commandIndex >= 0) {
        switch (datCommandRecords[commandIndex].unk30) {
        case 1:
        case 2:
        case 9:
        case 10:
            return BTL_ACTION_REACTION_MOTION_NONE;
        }
    }
    if ((datCommandRecords[commandIndex].attribute.bits & 0x400000FF) == 0x40000002) {
        return BTL_ACTION_REACTION_MOTION_NONE;
    }
    if (actionKind & 0x50004) {
        return BTL_ACTION_REACTION_MOTION_NONE;
    }
    if (elementMask & 0xE0001) {
        motionIndex = BTL_ACTION_REACTION_MOTION_NONE;
    } else if ((actor->status.flags & 0x200) != 0 &&
               press == BTL_ACTION_PRESS_RESULT_CRITICAL && hitCount == 1 && groupInactive == 0) {
        motionIndex = 0x12;
    } else {
        motionIndex = 1;
    }
    if (groupInactive != 0 && (btlUnitStatusPair(actor) & 0x4000000200) == 0x200) {
        motionIndex = 0xB;
    }
    if ((actionKind & 0x20001) == 0) {
        motionIndex = BTL_ACTION_REACTION_MOTION_NONE;
    }
    return motionIndex;
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

/* Return whether any grouped operand has flag bit 0; the selector is unused. */
s32 btlHasAnyOperandLowFlag(s32 unused, BattleIndexWork *state) {
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

extern void btlBuildCombinedActorUnit(BtlUnit *, BtlUnit *, BtlUnit *, BtlUnit *);

extern s32 btlShouldDistributeRandomTargetHits(BtlUnit *, s32);

extern void btlDistributeRandomTargetHits(BtlUnit *, BtlIndexList *, BtlOperandGroup *, s32);

extern void btlSortActorMuzzleDirections(BtlUnit *, BtlIndexList *, s32);

extern u32 btlEncodeActorIndexAsSelectionMask(u32);

extern s32 btlApplyCommandAbilityMultiplier(datUnitWork_t *battler, s32 command);

extern s32 btlGetCombinedPartyCommandPower(datUnitWork_t *, BtlUnit *, BtlUnit *, BtlUnit *, s32);

extern s32 btlGetUnitMotionDurationFrames(BtlUnit *, s32);

extern s32 btlSelectedEntryHitsElement(BtlUnit *, BtlUnit *, s32);

extern s32 btlGetActionRecordLookupValue(s32);

extern s32 btlResolveCommandHitResult(BtlUnit *, BtlUnit *, s32);

extern u8 btlSelectCommandHitCount(BtlUnit *, s32);

extern s32 btlGetCommandAddedStatus(BtlUnit *, BtlUnit *, s32, BtlActionPressResult, s32);

extern s32 btlQueryUnitChannelFlags(BtlUnit *, BtlUnit *, s32, BtlActionPressResult, s32);

extern s32 btlSelectActorAction(BtlUnit *actor);

extern s32 btlCalculateHuntEpReward(BtlUnit *acquirer, BtlUnit *enemy);

/* Native hunt interface passes actor and target; HP recovery is zero in this title. */
extern u32 btlGetHuntHpRecoveryAmount(BtlUnit *actor, BtlUnit *target);

/* Native caller supplies actor,target; concrete provider consumes only actor. */
extern u32 btlGetHuntPenaltyFlags(BtlUnit *unit, BtlUnit *enemy);

/* The damage/restore result is signed; keep this cross-unit declaration aligned with its provider. */
extern s32 btlCalculateHpAttackOrRestoreAmount(
    BtlUnit *, BtlUnit *, s32, u32, BtlActionPressResult, s32, u32);

extern s32 btlCalculateMpAttackOrRestoreAmount(
    BtlUnit *, BtlUnit *, s32, u32, BtlActionPressResult, s32, u32);

extern f32 btlGetActionCategoryMultiplier(BtlUnit *, s32, s32);

extern f32 btlGetActionCategoryGateAsFloat(s32, s32, s32);

/* Native resolver hook passes kind and press; both are unused by this title's no-op. */
extern void btlFindSoundTaskByWorkValue(s32 kind, BtlActionPressResult press);

extern s32 btlRollAllFearChance(s32, BtlUnit *, u32, s32, u8);

extern s32 btlRollFearChance(s32, BtlUnit *, u32, BtlActionPressResult);

extern s32 btlRollStoneDeathChance(BtlUnit *, s32);

extern s32 btlRollActorEligibilityWithAbilityOverride(BtlUnit *);

extern s32 btlCalculateEnemyExperienceReward(BtlUnit *, BtlUnit *);

extern s32 btlGetEnemyMoney(BtlUnit *, BtlUnit *);

extern u8 btlGetActorDisplayByteWithDefault(BtlUnit *object, s32 index);

extern s32 btlRollPassiveAbilityAction(BtlUnit *);

extern void fldAppendSceneGroupHandle(BtlTask *);

extern s16 btlGetCommandEffectId(BtlTask *, s32);

extern void btlClearActorEntrySlot(BtlUnit *, s32);

extern s32 btlCompareSkippedAndActiveTargetCounts(BtlIndexList *, BtlOperandGroup *);

extern u32 btlAdjustPointsForCombatFlags(u32, u32, u32, s32);

extern BtlSceneCounterMode btlGetCommandResultKindFromFlags(u32, u32);

void btlResolveActionTargetResults(BtlTask *action, BattleIndexWork *work) {
    BtlUnit combinedUnit;
    BtlState *runtime;
    BtlUnit *source;
    BtlUnit *target;
    BtlUnit *recipient;
    BtlOperandGroup *group;
    s32 hpDelta;
    s32 huntHpRecovery;
    BtlHuntMpRecoveryAmount huntMpRecovery;
    s32 cumulativeHp;
    BtlCommandId command;
    u32 costError;
    s32 helperResult;
    s32 counterCommand;
    s32 reflectedHp;
    s32 targetIndex;
    s32 hitIndex;
    u32 targetCount;
    u32 hitCount;
    u32 maxHits;
    u32 addedStatus;
    u32 removedStatus;
    u32 deferredStatus;
    u32 operandFlags;
    BtlActionPressResult press;
    s32 emptyHits;
    s32 fearCount;
    u32 combinedKinds;
    BtlActionPressResult combinedPress;
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
    command = btlResolveActionOperand(action->unit, work);
    if (command == BTL_COMMAND_ID_INVALID) {
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
        btlBuildCombinedActorUnit(&combinedUnit, action->unit, action->indexWork.companionA,
                     action->indexWork.companionB);
        source = &combinedUnit;
    }
    group = work->groups;
    randomTargets = 0;
    if (btlShouldDistributeRandomTargetHits(action->unit, command)) {
        randomTargets = 1;
        btlDistributeRandomTargetHits(action->unit, work->indices, group, command);
    }
    btlSortActorMuzzleDirections(action->unit, work->indices, command);
    targetCount = btlGetIndexListCount(work->indices);
    btlBossDebugPrintf("btl:skillID=%X\n", command);
    element = btlGetActorIndexedSignedValue(source, command);
    elementMask = btlEncodeActorIndexAsSelectionMask(element);
    if (action->indexWork.phase != 4) {
        if (datCommandSelectors[command].kind != 1) {
            costError = btlGetCommandFailureReason(source, command);
        } else {
            costError = btlGetLinkedPartyCommandFailureReason(source, action->unit, action->indexWork.companionA,
                                    action->indexWork.companionB, command);
        }
        if (costError == BTL_COMMAND_USE_ERROR_NONE) {
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
            work->costValidationFailed = 1;
            switch (costError) {
            case BTL_COMMAND_USE_ERROR_INSUFFICIENT_HP:
                work->stage = 3;
                work->parameter = effOffsetIfOwnerFlagClear(source, 0x14);
                action->unit->status.stateFlags |= 0x1000;
                break;
            case BTL_COMMAND_USE_ERROR_INSUFFICIENT_MP:
                work->stage = 3;
                work->parameter = effOffsetIfOwnerFlagClear(source, 0x12);
                action->unit->status.stateFlags |= 0x1000;
                break;
            case BTL_COMMAND_USE_ERROR_ACTOR_STATE:
                work->stage = 3;
                work->parameter = effOffsetIfOwnerFlagClear(source, 0x80);
                break;
            }
        }
    }
    helperResult = btlGetUnitMotionDurationFrames(action->unit, datActionAnimationRecords[command].cameraKind);
    if (datCommandRecords[command].commandEffectId != 0) {
        counterCommand = command;
        btlBossDebugPrintf("btl:counter=%d\n", command);
    } else {
        counterCommand = 0;
    }
    if (command == 0) {
        work->precedence = 8;
    } else {
        if (helperResult >= 21) {
            work->precedence = helperResult;
        } else {
            work->precedence = 20;
        }
        work->precedence -= 8;
        btlBossDebugPrintf("btl:precede=%d\n", work->precedence);
    }
    combinedKinds = 0;
    combinedPress = BTL_ACTION_PRESS_RESULT_NONE;
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
        if (btlSelectedEntryHitsElement(source, target, command)) {
            kind = btlGetActionRecordLookupValue(target->selectedEntryIndex);
            group->targetSpecialHit = 1;
        } else {
            kind = btlResolveCommandHitResult(source, target, command);
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
                    hitCount = btlSelectCommandHitCount(source, command);
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
            if (btlSelectedEntryHitsElement(action->unit, recipient, command)) {
                kind = btlGetActionRecordLookupValue(recipient->selectedEntryIndex);
                group->sourceSpecialHit = 1;
            } else {
                kind = btlResolveCommandHitResult(action->unit, recipient, command);
                group->sourceSpecialHit = 0;
            }
            if (datCommandRecords[command].flags & 2) {
                kind = 4;
            } else if (runtime->runtimeFlags & 0x10) {
                kind = 1;
            } else {
                switch (kind) {
                case 0x20000:
                    kind = 0x10000;
                    break;
                case 2:
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
        press = BTL_ACTION_PRESS_RESULT_NORMAL;
        if (!group->reflected) {
            switch (kind) {
            case 2:
            case 4:
            case 0x10000:
            case 0x20000:
            case 0x40000:
                press = BTL_ACTION_PRESS_RESULT_NORMAL;
                break;
            default:
                press = btlDetermineActionPressResult(source, recipient, command);
                if (action->options & 1) {
                    BtlActionPressResult adjustedPress = BTL_ACTION_PRESS_RESULT_CRITICAL;
                    if (press == BTL_ACTION_PRESS_RESULT_WEAKNESS) {
                        adjustedPress = press;
                    }
                    press = adjustedPress;
                }
                break;
            }
        }
        addedStatus = btlGetCommandAddedStatus(source, recipient, command, press, (s32)kind);
        removedStatus = btlQueryUnitChannelFlags(source, recipient, command, press, (s32)kind);
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
            work->flags = btlSelectActorAction(target);
            target->status.flags |= 0x4000000;
        }
        switch (datCommandRecords[command].unk30) {
        case 12: operandFlags |= 0x10; break;
        case 13: operandFlags |= 0x20; addedStatus = 0x4000; break;
        case 14: operandFlags |= 0x40; break;
        case 15: operandFlags |= 0x80; break;
        }
        if (datCommandRecords[command].effectType == 2) {
            if (kind == 1) {
                operandFlags |= 1;
                addedStatus = 0x4000;
                work->huntEpReward += btlCalculateHuntEpReward(action->unit, recipient);
                work->stage = 5;
                work->parameter = effOffsetIfOwnerFlagClear(target, 0x9C);
                if (!huntRecovered) {
                    huntRecovered = 1;
                    huntHpRecovery = btlGetHuntHpRecoveryAmount(action->unit, target);
                    huntMpRecovery = btlGetHuntMpRecoveryAmount(action->unit, target);
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
            entryChange = (u16)(s8)datCommandRecords[command].entryChange;
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

            hpAttack = btlCalculateHpAttackOrRestoreAmount(source, recipient, command, maxHits, press, 1, calculationFlags);
            hpRestore = btlCalculateHpAttackOrRestoreAmount(source, recipient, command, maxHits, press, 0, calculationFlags);
            hpAttack = hpAttack * ratio;
            mpAttack = btlCalculateMpAttackOrRestoreAmount(source, recipient, command, maxHits, press, 1, calculationFlags);
            mpRestore = btlCalculateMpAttackOrRestoreAmount(source, recipient, command, maxHits, press, 0, calculationFlags);
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
                    work->refreshEligibleActors = 1;
                    allFear = 1;
                }
            }
            if (!allFear && !btlIsUnitDefeatTriggeredByValueDelta(recipient, cumulativeHp) &&
                (source->status.flags & 0x200) && (recipient->status.flags & 0x400) &&
                cumulativeHp < 0 && kind == 1 && !group->inactive && !group->reflected &&
                !group->targetSpecialHit && !absorbed && addedStatus == 0 &&
                !(datEnemyRecords[recipient->partyRecord.unitId].flags & 0x440)) {
                if (btlRollFearChance((s32)source, recipient, 1, press)) {
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
            press = BTL_ACTION_PRESS_RESULT_NORMAL;
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
                work->huntEpReward += btlCalculateHuntEpReward(action->unit, recipient);
                work->stage = 5;
                work->parameter = effOffsetIfOwnerFlagClear(target, 0x9C);
                if (!huntRecovered) {
                    huntRecovered = 1;
                    group->entries[hitCount - 1].hpRecovery += btlGetHuntHpRecoveryAmount(action->unit, target);
                    group->entries[hitCount - 1].mpRecovery += btlGetHuntMpRecoveryAmount(action->unit, target);
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
                work->battleExperienceReward += btlCalculateEnemyExperienceReward(action->unit, recipient);
                recipient->status.stateFlags |= 2;
            }
            work->moneyReward += btlGetEnemyMoney(action->unit, recipient);
            runtime->defeatedLevelTotal += recipient->partyRecord.level;
            runtime->defeatedLevelCount++;
            recipient->status.stateFlags |= 0x400;
        }
        work->groups[targetIndex].interval = btlGetActorDisplayByteWithDefault(source, command);
        work->groups[targetIndex].count = hitCount;
        if (element < 2 &&
            kind == 1 && !group->inactive && !group->reflected && !group->targetSpecialHit &&
            !absorbed && !(action->flags & 0x40) && !(target->partyRecord.status & 0x90F) &&
            !(addedStatus & 0x90F)) {
            s32 counter = btlRollPassiveAbilityAction(target);
            if (counter > 0) {
                BtlTask *reaction = btlFindUnitByActor(target);
                fldAppendSceneGroupHandle(reaction);
                reaction->indexWork.phase = 1;
                reaction->indexWork.skillId = counter;
                btlAppendIndexListEntry(reaction->indexWork.indices, action->unit);
                btlBossDebugPrintf("btl:counter[%p][%X]\n", target, counter);
            }
        }
        if (press == BTL_ACTION_PRESS_RESULT_CRITICAL) {
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
        work->allTargetSideActorsSkipped = 1;
    }
    work->slot = btlClassifyActionOperand(action->unit, work);
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
        if (!work->costValidationFailed && (!allEmpty || (combinedKinds & 0x60000))) {
            work->adjustedValue = btlAdjustPointsForCombatFlags(combinedKinds, combinedPress, points, command);
            work->resultKind = btlGetCommandResultKindFromFlags(combinedKinds, combinedPress);
        } else {
            if (overrideMiss) {
                points += 100;
            }
            work->resultKind = BTL_SCENE_COUNTER_MODE_NORMAL;
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
    work->summonId = -1;
    work->stageValue = 0;
    work->precedence = 8;
    work->stage = 0;
    work->parameter = 0;
    work->wait = -1;
    work->unk3C = 0;
    work->costValidationFailed = 0;
    work->allTargetSideActorsSkipped = 0;
    work->huntEpReward = 0;
    work->battleExperienceReward = 0;
    work->moneyReward = 0;
    work->flags = 0;
    work->refreshEligibleActors = 0;
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
    struct sdfMemHandle_t *allocation;
    u32 value;
    object->indices = btlAllocateIndexList(13);
    allocation = sdfMemAllocDirect(0x836C);
    value = sdfMemLock(allocation);
    object->allocationHandle = allocation;
    object->groups = (BtlOperandGroup *)value;
    object->ownerId = 0;
    btlResetIndexWork(object);
}

/* Release each owned buffer once. The cached group address is deliberately not cleared. */
void btlReleaseObjectBuffers(BattleIndexWork *object) {
    struct sdfMemHandle_t *handle = object->allocationHandle;
    if (handle != 0) {
        sdfMemFreeDirect(handle);
        object->allocationHandle = 0;
    }
    if (object->indices != 0) {
        btlFreeIndexList(object->indices);
        object->indices = 0;
    }
}

extern void btlHighlightActorStatPanel(BtlUnit *, s8);

/* Apply the copied operand, then refresh the actor's status and defeat state. */
s32 btlApplyActorParameterDelta(BtlOperandTaskArgs *arguments) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit = arguments->unit;
    u16 previousStatus;
    s32 defeated;

    if (battle->battleFlags & 0x80) {
        btlAdjustUnitHp(&unit->partyRecord, arguments->operand.hpDelta);
        if (arguments->operand.hpDelta < 0) {
            btlHighlightActorStatPanel(unit, 0);
        }
        btlAdjustUnitMp(&unit->partyRecord, arguments->operand.mpDelta);
        if (arguments->operand.mpDelta < 0) {
            btlHighlightActorStatPanel(unit, 1);
        }
    }
    if (arguments->operand.addedStatus & 0x8000) {
        if (unit->status.flags & 0x1000) {
            unit->status.flags |= 0x400000;
        }
        arguments->operand.addedStatus &= ~0x8000;
    }
    if ((arguments->operand.addedStatus & 0x1000) && unit->partyRecord.status < 0x1000) {
        unit->status.flags |= 0x10000000;
    }
    if ((arguments->operand.addedStatus & 0x2000) && unit->partyRecord.status < 0x2000 &&
        !(unit->status.flags & 0x1000)) {
        unit->status.flags |= 0x20000000;
    }
    previousStatus = unit->partyRecord.status;
    btlRaisePartyRecordStatusValue(&unit->partyRecord, arguments->operand.addedStatus);
    btlClearPartyRecordStatusBits(&unit->partyRecord, arguments->operand.removedStatus);
    defeated = 0;
    if (unit->partyRecord.status != 0 && unit->partyRecord.status != previousStatus) {
        unit->status.stateFlags |= 4;
    }
    if (battle->actorParameterDeltaCallback != NULL) {
        battle->actorParameterDeltaCallback(unit, &arguments->operand.hpDelta);
    }
    btlRefreshUnitMotionSelection(unit);
    if (arguments->operand.flags & 1) {
        unit->status.flags |= 0x800020;
        defeated = 1;
    } else if (btlIsUnitDefeatTriggeredByValueDelta(unit, 0)) {
        unit->status.flags |= 0x20;
        defeated = 1;
    } else if (arguments->operand.flags & 2) {
        unit->status.flags |= 0x8000020;
        defeated = 1;
    }
    if (defeated) {
        unit->partyRecord.hp = 0;
    }
    if ((unit->status.flags & 0x20) && (unit->partyRecord.status & 0x1000)) {
        if (unit->species == 0x1F) {
            unit->status.flags = (unit->status.flags & ~0x10400000) | 0x20000000;
        }
        unit->status.flags &= ~0x10000000;
        btlClearPartyRecordStatusBits(&unit->partyRecord, 0x1000);
    }
    return 1;
}

extern BtlRuntimeTask *btlAllocTask(s32);

BtlRuntimeTask *btlCreateActorParameterDeltaTask(BtlUnit *owner, BtlOperandEntry *spec) {
    BtlRuntimeTask *task = btlAllocTask(0x2C);
    BtlOperandTaskArgs *arguments;

    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x45;
    task->callback = btlApplyActorParameterDelta;
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
    datUnitWork_t *resource;
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

BtlRuntimeTask *btlCreateDeferredActorStatsTask(BtlUnit *owner, BtlOperandEntry *spec) {
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

u32 btlApplyDeferredUnitStatus(BtlDeferredUnitStatusArgs *arguments) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit = arguments->unit;
    if ((work->battleFlags & 0x80) == 0) {
        return 1;
    }
    btlRaisePartyRecordStatusValue(&unit->partyRecord, arguments->minimumValue);
    btlRefreshUnitMotionSelection(unit);
    btlIsUnitDefeatTriggeredByValueDelta(unit, 0);
    return 1;
}

extern BtlRuntimeTask *btlAllocTask(s32);

extern u32 btlApplyDeferredUnitStatus(BtlDeferredUnitStatusArgs *);

BtlRuntimeTask *btlCreateDeferredUnitStatusTask(BtlUnit *owner, u32 value) {
    BtlRuntimeTask *task = btlAllocTask(8);
    u8 *taskBytes = (u8 *)task;
    s64 data;
    BtlDeferredUnitStatusArgs *arguments;

    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)((u8 *)task + 0x20) = 0x47;
    *(void **)((u8 *)task + 0x4C) = btlApplyDeferredUnitStatus;
    data = owner->identity;
    *(s64 *)((u8 *)task + 0x40) = data;
    *(u32 *)((u8 *)task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = owner;
    arguments->minimumValue = value;
    return task;
}

u32 btlApplyCategoryStatDamage(BtlCategoryStatDamageArgs *args) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit = args->unit;
    if ((work->battleFlags & 0x80) == 0) {
        return 1;
    }
    if (datCommandRecords[args->category].flags & 8) {
        btlAdjustUnitHp(&unit->partyRecord, -0x7FFF);
        btlRaisePartyRecordStatusValue(&unit->partyRecord, 0x4000);
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

extern u32 btlApplyCategoryStatDamage(BtlCategoryStatDamageArgs *);

BtlRuntimeTask *btlCreateCategoryStatDamageTask(BtlUnit *owner, s32 category, s32 amount) {
    BtlRuntimeTask *task = btlAllocTask(12);
    u8 *taskBytes = (u8 *)task;
    BtlCategoryStatDamageArgs *arguments;

    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)((u8 *)task + 0x4C) = btlApplyCategoryStatDamage;
    *(u16 *)((u8 *)task + 0x20) = 0x48;
    *(s64 *)((u8 *)task + 0x40) = *(s64 *)((u8 *)owner + 0x108);
    *(u32 *)((u8 *)task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = owner;
    arguments->category = category;
    arguments->amount = amount;
    return task;
}

/* Apply the operand's masked entry changes; complete immediately. */
s32 btlApplyMaskedActorEntryUpdate(BtlOperandTaskArgs *args) {
    btlApplyMaskedActorEntryAdjustments(args->unit, args->operand.entryChangeMask, args->operand.entryChange);
    return 1;
}

/* Copy the operand into an owner-bound masked-entry-update task. */
BtlRuntimeTask *btlCreateMaskedActorEntryUpdateTask(BtlUnit *owner, BtlOperandEntry *spec) {
    BtlRuntimeTask *task = btlAllocTask(0x2C);
    BtlOperandTaskArgs *arguments;

    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x49;
    task->callback = btlApplyMaskedActorEntryUpdate;
    task->ownerId = owner->identity;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = owner;
    memcpy(&arguments->operand, spec, sizeof(*spec));
    return task;
}

u32 btlApplyQueuedActorEntrySelection(BtlOperandTaskArgs *taskArgs) {
    if (0 < taskArgs->operand.entrySelection) {
        btlSetActorSelectedEntryIndex(taskArgs->unit, taskArgs->operand.entrySelection);
        btlRefreshUnitMotionSelection(taskArgs->unit);
    }
    return 1;
}

/* Queue the operand's actor entry selection in an owner-bound task. */
BtlRuntimeTask *btlCreateQueuedActorEntrySelectionTask(BtlUnit *owner, BtlOperandEntry *spec) {
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

u32 btlClearQueuedActorEntrySelection(BtlUnit **taskArgs) {
    btlClearActorSelectedEntryIndex(*taskArgs);
    btlRefreshUnitMotionSelection(*taskArgs);
    return 1;
}

/* Create an owner-bound task to clear the actor's queued entry selection. */
BtlRuntimeTask *btlCreateClearActorEntrySelectionTask(BtlUnit *owner) {
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

extern datUnitWork_t *btlGetIndexedPartyEntryRecord(s32);

/* Award hunt experience; the sharing ability also rewards eligible party records. */
u32 btlAwardHuntExperience(BtlHuntExpArgs *args) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    BtlUnit *head;
    datUnitWork_t *record;
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
        record = btlGetIndexedPartyEntryRecord(args->actor->partyIndex);
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
            u16 flags = DDS3_GBWK->party[index].flags;
            if (flags & 1) {
                if (!(flags & 2) && !(DDS3_GBWK->party[index].status & 0x4040)) {
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
                        record = btlGetIndexedPartyEntryRecord(unit->partyIndex);
                        record->huntExp += share;
                    }
                }
            }
        }
        for (index = 0; index < 5; index++) {
            u16 flags = DDS3_GBWK->party[index].flags;
            if (flags & 1) {
                if (!(flags & 2) && !(DDS3_GBWK->party[index].status & 0x4040)) {
                    DDS3_GBWK->party[index].huntExp += share;
                }
            }
        }
        btlBossDebugPrintf("btl:AUTO ep=%d[%d],count=%d\n", share, args->amount, count);
    } else {
        record = btlGetIndexedPartyEntryRecord(args->actor->partyIndex);
        record->huntExp += args->amount;
        btlBossDebugPrintf("btl:hunt ep=%d[%p]\n", args->amount, record);
    }
    return 1;
}

extern u32 btlAwardHuntExperience(BtlHuntExpArgs *);

BtlRuntimeTask *btlCreateActorSoundOptionTask(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    u8 *taskBytes = (u8 *)task;
    BtlHuntExpArgs *data;

    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)((u8 *)task + 0x20) = 0x4C;
    *(void **)((u8 *)task + 0x4C) = btlAwardHuntExperience;
    *(u64 *)((u8 *)task + 0x40) = actor->identity;
    *(s32 *)((u8 *)task + 0x48) = 0;
    data = btlGetTaskArguments(task);
    data->actor = actor;
    data->amount = (u32)option;
    return task;
}

extern void ptyAdjustItemQuantity(s32 itemId, s32 quantityDelta);

/* Consume one item when its table flag 4 is set, then signal task completion. */
typedef struct {
    BtlUnit *actor;
    s32 itemId;
} BtlCommandItemWork;

u32 btlConsumeFlaggedCommandItem(BtlCommandItemWork *work) {
    s32 itemId = work->itemId;
    if ((datItemSkillRecords[itemId].useFlags & 4) != 0) {
        ptyAdjustItemQuantity(itemId, 0xffffffffffffffff);
    }
    return 1;
}

/* Queue an actor/item argument pair for conditional inventory consumption. */
BtlRuntimeTask *btlCreateFlaggedCommandItemConsumeTask(BtlUnit *actor, s32 itemId) {
    BtlRuntimeTask *task = btlAllocTask(8);
    u8 *taskBytes = (u8 *)task;
    BtlCommandItemWork *data;

    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)((u8 *)task + 0x20) = 0x4D;
    *(void **)((u8 *)task + 0x4C) = btlConsumeFlaggedCommandItem;
    *(u64 *)((u8 *)task + 0x40) = actor->identity;
    *(s32 *)((u8 *)task + 0x48) = 0;
    data = (BtlCommandItemWork *)btlGetTaskArguments(task);
    data->actor = actor;
    data->itemId = itemId;
    return task;
}

/* Add one to the argument's 16-bit item ID quantity, then complete the task. */
u32 btlIncrementCommandItemQuantity(BtlRuntimeTaskArgs *taskArgs) {
    ptyAdjustItemQuantity(taskArgs->optionId, 1);
    return 1;
}

/* Queue an owner-bound task that increments this item's inventory quantity. */
BtlRuntimeTask *btlCreateItemQuantityIncrementTask(BtlUnit *actor, u16 itemId) {
    BtlRuntimeTask *task = btlAllocTask(8);
    u8 *taskBytes = (u8 *)task;
    u8 *actorBytes = (u8 *)actor;
    BtlRuntimeTaskArgs *data;

    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)((u8 *)task + 0x20) = 0x4E;
    *(void **)((u8 *)task + 0x4C) = btlIncrementCommandItemQuantity;
    *(u64 *)((u8 *)task + 0x40) = *(u64 *)(actorBytes + 0x108);
    *(s32 *)((u8 *)task + 0x48) = 0;
    data = btlGetTaskArguments(task);
    data->actor = actor;
    data->optionId = itemId;
    return task;
}

u32 btlAddEpFromPacket(BtlRewardPacket *packet) {
    BtlState *work = (BtlState *)btlGetRuntime();

    if (packet->amount == 0) {
        return 1;
    }
    if ((u32)packet->actor->status.flags & 0x400) {
        return 1;
    }
    work->epEarned += packet->amount;
    btlBossDebugPrintf("btl:epall=%d[%d](packet)\n", work->epEarned, packet->amount);
    return 1;
}

extern u32 btlAddEpFromPacket(BtlRewardPacket *);

BtlRuntimeTask *btlScheduleEpPacketTask(BtlUnit *actor, s32 amount) {
    BtlRuntimeTask *task = btlAllocTask(8);
    u8 *taskBytes = (u8 *)task;
    BtlRewardPacket *arguments;

    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)((u8 *)task + 0x20) = 0x4F;
    *(void **)((u8 *)task + 0x4C) = btlAddEpFromPacket;
    *(u64 *)((u8 *)task + 0x40) = actor->identity;
    *(u32 *)((u8 *)task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments->actor = actor;
    arguments->amount = amount;
    return task;
}

u32 btlAddMoneyFromPacket(BtlRewardPacket *packet) {
    BtlState *work = (BtlState *)btlGetRuntime();

    if (packet->amount == 0) {
        return 1;
    }
    if ((u32)packet->actor->status.flags & 0x400) {
        return 1;
    }
    work->moneyEarned += packet->amount;
    btlBossDebugPrintf("btl:money=%d[%d](packet)\n", work->moneyEarned, packet->amount);
    return 1;
}

extern u32 btlAddMoneyFromPacket(BtlRewardPacket *);

BtlRuntimeTask *btlScheduleMoneyPacketTask(BtlUnit *actor, s32 amount) {
    BtlRuntimeTask *task = btlAllocTask(8);
    u8 *taskBytes = (u8 *)task;
    s64 data;
    BtlRewardPacket *arguments;

    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)((u8 *)task + 0x20) = 0x50;
    *(void **)((u8 *)task + 0x4C) = btlAddMoneyFromPacket;
    data = actor->identity;
    *(s64 *)((u8 *)task + 0x40) = data;
    *(u32 *)((u8 *)task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments->actor = actor;
    arguments->amount = amount;
    return task;
}

u32 btlRefreshEligibleActors(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *actor = state->units;
    while (actor != 0) {
        u32 flags = (u32)actor->status.flags;
        if (flags & 0x400) {
            if (flags & 1) {
                if ((flags & 0xE0) == 0 &&
                    (u16)(actor->partyRecord.unitId - 1) < 0x17F) {
                    u32 entry = datEnemyRecords[actor->partyRecord.unitId].flags;
                    if ((entry & 0x40) == 0) {
                        if ((entry & 0x400) == 0) {
                            if ((actor->status.stateFlags & 8) == 0) {
                                u16 prior = actor->partyRecord.status;
                                btlRaisePartyRecordStatusValue(&actor->partyRecord, 1);
                                btlRefreshUnitMotionSelection(actor);
                                if (actor->partyRecord.status == 1 &&
                                    prior != actor->partyRecord.status) {
                                    actor->status.stateFlags |= 4;
                                    state->commandRestrictFlags |= 0x100;
                                }
                            }
                        }
                    }
                }
            }
        }
        actor = actor->next;
    }
    return 1;
}

BtlRuntimeTask *btlCreateRefreshEligibleActorsTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    u8 *taskBytes = (u8 *)task;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    *(void **)((u8 *)task + 0x4C) = btlRefreshEligibleActors;
    *(u16 *)((u8 *)task + 0x20) = 0x51;
    *(u32 *)((u8 *)task + 0x48) = 0;
    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    return task;
}

extern char D_003A3A40[];

extern char D_003A3A50[];

void btlUpdateAutoMusic(void) {
    BtlState *context = (BtlState *)btlGetRuntime();
    u32 flags = context->battleFlags;
    if ((flags & 0x100000) == 0 || (flags & 0x6000000) == 0x6000000 ||
        (flags & 0x800) != 0) {
        return;
    }
    if (flags & 0x8000) {
        if (dds3PadButtonState.primaryTriangle < 0 || dds3PadButtonState.primaryCancel < 0) {
            context->battleFlags = flags & ~0x8000;
            p2sdPlaySE(6, 0x7F, 0x3F);
            btlSetTrackedTaskDisplayMode(0);
            btlBossDebugPrintf(D_003A3A40);
        }
    } else if (dds3PadButtonState.primaryTriangle < 0) {
        context->battleFlags = flags | 0x8000;
        p2sdPlaySE(5, 0x7F, 0x3F);
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
    BtlCommandTargetEligibilityResult reason;

    if (fileTestSavedSlotFlags(FILE_CONFIG_OPTION_RESTORE_BATTLE_COMMAND)) {
        if ((s16)unit->partyRecord.savedCommandPhase == 10) {
            work->phase = (s16)unit->partyRecord.savedCommandPhase;
            return;
        }
        if (((s16)unit->partyRecord.savedCommandPhase == 2 ||
             (s16)unit->partyRecord.savedCommandPhase == 7) &&
            (btlUnitStatusPair(unit) & 0x1400)) {
            if ((s16)unit->partyRecord.actionSlot == 0) {
                work->phase = 1;
            } else if ((unit->status.flags & 0x400) || (battle->battleFlags & 0x1000000) ||
                       btlCheckSpecialAbility(&unit->partyRecord, (s16)unit->partyRecord.actionSlot)) {
                work->phase = (s16)unit->partyRecord.savedCommandPhase;
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
        if ((u32)work->skillId >= 0x260) {
            work->skillId = 0;
            work->phase = 1;
        }
        if (btlGetCommandFailureReason(unit, work->skillId) != BTL_COMMAND_USE_ERROR_NONE) {
            work->skillId = 0;
            work->phase = 1;
        }
        unit->partyRecord.savedCommandPhase = work->phase;
        unit->partyRecord.actionSlot = work->skillId;
        list = btlAllocateIndexList(13);
        kind = btlSelectCommandTargetPolicy(task, list, 0);
        reason = btlGetCommandTargetEligibility(list, work->skillId);
        if (reason == BTL_COMMAND_TARGET_ELIGIBILITY_RESULT_ENTRY_FLAG_40 ||
            reason == BTL_COMMAND_TARGET_ELIGIBILITY_RESULT_COMMAND_RESTRICTED) {
            work->skillId = 0;
            work->phase = 1;
            unit->partyRecord.actionSlot = work->skillId;
            unit->partyRecord.savedCommandPhase = work->phase;
            btlClearIndexList(list);
            kind = btlSelectCommandTargetPolicy(task, list, 0);
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
        kind = btlSelectCommandTargetPolicy(task, list, 0);
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
void btlFindSoundTaskByWorkValue(s32 kind, BtlActionPressResult press) {
}

/* Return the oldest task matching this handle; unstarted tasks may have handle 0. */
BtlRuntimeTask *btlFindTaskByHandle(u64 handle) {
    BtlRuntimeTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    while (task != 0) {
        if (task->handle == handle) {
            return task;
        }
        task = task->next;
    }
    return 0;
}

/* Return the oldest task with this owner, or zero. */
BtlRuntimeTask *btlFindTaskByOwner(u64 owner) {
    BtlRuntimeTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    while (task != 0) {
        if (task->ownerId == owner) {
            return task;
        }
        task = task->next;
    }
    return 0;
}

/* Return the oldest registered task of this kind, or zero. */
BtlRuntimeTask *btlFindTaskByKind(u16 kind) {
    BtlRuntimeTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    while (task != 0) {
        if (task->taskId == kind) {
            return task;
        }
        task = task->next;
    }
    return 0;
}

/* Count all registrations, including tasks awaiting startup or release. */
s32 btlCountRegisteredTasks(void) {
    s32 address;
    s32 count;

    address = (s32)(u32)btlGetRuntime();
    count = 0;
    for (address = (s32)((BtlActorWork *)address)->taskHead; address != 0; address = (s32)((BtlRuntimeTask *)address)->next) {
        count = count + 1;
    }
    return count;
}

/* Count registrations with this full-width owner key. */
s32 btlCountTasksForOwner(u64 owner) {
    BtlRuntimeTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    s32 count = 0;
    while (task != 0) {
        u64 taskOwner = task->ownerId;
        task = task->next;
        if (taskOwner == owner) {
            count++;
        }
    }
    return count;
}

/* Count registrations of the requested task kind. */
s32 btlCountTasksByKind(u16 kind) {
    BtlRuntimeTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
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
    BtlRuntimeTask *task = ((BtlActorWork *)btlGetRuntime())->taskTail;
    while (task != 0) {
        u16 flags = task->flags;
        BtlRuntimeTask *next = task->prev;
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

u8 btlEvalTaskCondition(BtlTaskCondition *condition, s32 currentCount) {
    u8 result = 0;
    BtlRuntimeTask *task;

    switch (condition->kind) {
    case BTL_TASK_CONDITION_NEVER:
        break;
    case BTL_TASK_CONDITION_ALWAYS:
        result = 1;
        break;
    case BTL_TASK_CONDITION_COUNT_REACHED:
        if (!(currentCount < condition->value.count)) {
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
    BtlActorWork *context;
    BtlRuntimeTask *tail;

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
void *btlGetTaskArguments(BtlRuntimeTask *task) {
    return task->args;
}

/* Invoke the finish hook before unlinking, then release the task block. */
void btlFreeTask(BtlRuntimeTask *task) {
    BtlActorWork *context;
    BtlRuntimeTask *next;
    BtlRuntimeTask *previous;
    void (*cleanup)(u32 *);
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
    sdfChipFreeDirect(task);
}

/* Install a fresh handle/reset phase counters, invoke startup, then reread handle. */
u64 btlStartTask(BtlRuntimeTask *taskObject) {
    u64 value = btlAdvanceRuntimeSequenceCounter();
    BtlRuntimeTask *task = taskObject;
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
        } else if ((BtlTaskPollResult)task->callback(task->args) != BTL_TASK_POLL_CONTINUE) {
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
    BtlRuntimeTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    BtlRuntimeTask *next;
    while (task != 0) {
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
        task = next;
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
    BtlRuntimeTask *task = ((BtlActorWork *)btlGetRuntime())->taskTail;
    while (task != 0) {
        BtlRuntimeTask *next = task->prev;
        btlFreeTask(task);
        task = next;
    }
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

/* Process callback for the zero-argument immediate-completion task. */
u32 btlCompleteTaskImmediately(void) {
    return 1;
}

BtlRuntimeTask *btlCreateImmediateCompletionTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlCompleteTaskImmediately;
    task->taskId = 0x63;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

void btlDumpTaskQueue(void) {
    s32 context = (s32)(u32)btlGetRuntime();
    s32 node = *(s32 *)(context + 0x230);
    while (node != 0) {
        btlBossDebugPrintf("btl:packet[%d]\n", *(u16 *)(node + 0x20));
        node = *(s32 *)(node + 0x58);
    }
    btlBossDebugPrintf("btl:packet head[%p]\n", *(void **)(context + 0x22C));
    btlBossDebugPrintf("btl:packet tail[%p]\n", *(void **)(context + 0x230));
}

extern u128 btlDefaultUnitBodyOffset;

void btlInitUnitFxDefaults(BtlUnit *fx) {
    SDF_VU0_COPYVECTOR(fx->bodyOffset, &btlDefaultUnitBodyOffset);
    fx->height = 220.0f;
    fx->reach = 80.0f;
    fx->cameraRadius = 75.0f;
}

extern u128 btlDefaultUnitPosition;

extern u128 btlDefaultUnitRotation;

void btlInitFxLights(BtlUnit *fx) {
    SDF_VU0_COPYVECTOR(fx->position, &btlDefaultUnitPosition);
    SDF_VU0_COPYVECTOR(fx->rotation, &btlDefaultUnitRotation);
    fx->unk58Bits = 0;
    fx->actorScale = 1.0f;
    fx->baseColor = 0x80808080;
    SDF_VU0_COPYVECTOR(fx->currentPosition, &btlDefaultUnitPosition);
    SDF_VU0_COPYVECTOR(fx->orientation, &btlDefaultUnitRotation);
    fx->scale = 1.0f;
    fx->overlayColor = 0x80808080;
    fx->positionZOffsetBits = 0;
}

void btlInitializeEffectVectorsFromSourceRecords(
    BtlUnit *fx, s32 resourceKind, s32 resourceIndex) {
    BtlActorTransformParameters *alt =
        btlSelectSharedOrIndexedTransformParameters(resourceKind, resourceIndex);
    BtlEffectResource *base = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(
        resourceKind, resourceIndex);

    if (alt->overrideEnabled == 0.0f) {
        SDF_VU0_COPYVECTOR(fx->bodyOffset, base);
        fx->reach = base->reach;
        fx->height = base->height;
        fx->cameraRadius = base->cameraRadius;
    } else {
        fx->bodyOffset[0] = alt->position[0];
        fx->bodyOffset[1] = alt->position[1];
        fx->bodyOffset[2] = alt->position[2];
        fx->bodyOffset[3] = 0.0f;
        fx->reach = alt->reach;
        fx->height = alt->height;
    }
    SDF_VU0_COPYVECTOR(fx->muzzleOffset, base);
    fx->bodyRadius = base->reach;
    fx->geometryHeight = base->height;
    fx->cameraRadius = base->cameraRadius;
    fx->scale = base->actorScale;
    fx->actorScale = base->actorScale;
    fx->positionZOffset = base->zOffset;
    fx->unk58 = base->zOffset;
}

s32 btlHasMatchingModel(s32 effect, s32 model) {
    s32 context = (s32)(u32)btlGetRuntime();
    BtlUnit *node = ((BtlState *)context)->units;
    while (node != 0) {
        if ((node->status.flags & 2) != 0 &&
            node->ext != 0 &&
            node->soundSlotOwner != 0 &&
            mdlGetMajorNum(node->ext->owner) == effect &&
            mdlGetMinorNum(node->ext->owner) == model) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

/* Inherit the source unit's sound owner, transparency model, world node and
 * event work, and mark the destination as an inheriting unit. */
void btlInheritActorModelState(BtlUnit *dst, BtlUnit *src) {
    dst->soundSlotOwner = src->soundSlotOwner;
    dst->transparencyModel = src->transparencyModel;
    dst->effectObject = src->effectObject;
    dst->ext = src->ext;
    dst->modelBorrowed = BTL_MODEL_STATE_BORROWED;
    dst->status.flags |= 2;
}

/* Kind-5 world-node payload attaches the event unit at +8. */
typedef struct EventUnitData {
    u8 pad00[8];
    evtUnitHandle_t *unit;
} EventUnitData;

extern s32 mdlSpawnCameraSlotViewerObject(s32, s32);

#include "dds3_world_primary.h"



extern void mdlSetEffectScale(dds3ModelHandle_t *, f32);

extern struct SoundSlotOwner *sndAcquireSlotOwner(s32 category, s32 id);

extern void btlResetUnitModelProgress(BtlUnit *);

extern void btlSetUnitPosition(BtlUnit *object, const f32 *position);

extern void btlSetUnitRotation(BtlUnit *object, const f32 rotation[4]);

/* vu0 routine: initialize the actor world transform with the SDK unit vector. */
void btlBindActorModel(BtlUnit *unit, u32 kind, u32 id) {
    BtlUnit *reused = NULL;
    BtlState *battle = (BtlState *)btlGetRuntime();
    dds3ModelHandle_t *model;
    BtlActorStatusRecord *status;
    EventUnitData *data;
    s32 key;

    unit->resourceKind = kind;
    unit->species = id;
    unit->modelBorrowed = BTL_MODEL_STATE_OWNED;
    btlInitializeEffectVectorsFromSourceRecords(unit, kind, id);
    if (battle->findReusableUnit) {
        reused = battle->findReusableUnit(kind, id);
        if (reused) {
            if (reused->resourceKind != kind || reused->species != id) {
                key = mdlSpawnCameraSlotViewerObject(kind, id);
                dds3DeleteSceneBasicData(dds3SearchSceneBasic(dds3GetActiveScene(), key, 5));
            }
            btlInheritActorModelState(unit, reused);
        }
    }
    if (battle->prepareModelUnit) {
        battle->prepareModelUnit(unit);
    }
    if (reused == NULL) {
        key = mdlSpawnCameraSlotViewerObject(kind, id);
        unit->effectObject = dds3SearchSceneBasic(dds3GetActiveScene(), key, 5);
        data = unit->effectObject->data;
        unit->ext = data->unit;
        model = unit->ext->owner;
        unit->ext->flags |= 0x200000;
        SDF_VU0_SETSCALE1VECTOR();
        VU0_SCALAR_OP(unit->actorScale, "vmulx.xyzw vf10, vf10, vf2x");
        unit->effectObject->inner->flags = (unit->effectObject->inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
            ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
        SDF_VU0_STOREVECTOR(unit->effectObject->inner->scale);
        mdlSetScale(model);
        if (unit->actorScale != 1.0f) {
            mdlSetEffectScale(model, unit->actorScale);
        }
        dds3ObjectBaseOffFlag(unit->effectObject, 0x400);
        unit->status.flags |= 8;
        unit->soundSlotOwner = sndAcquireSlotOwner(kind, id);
        unit->status.flags |= 2;
    }
    btlSetUnitPosition(unit, unit->position);
    btlSetUnitRotation(unit, unit->rotation);
    unit->updateFlags = 0;
    unit->motionStartFrame = 0;
    unit->motionId = -1;
    unit->motionEndFrame = 0;
    btlRefreshUnitMotionSelection(unit);
    if (unit->effectIndex != 0xB) {
        btlApplyScaledUnitEffectParameter(unit, unit->effectIndex, 1, 1.0f);
    } else {
        btlApplyScaledUnitEffectParameter(unit, 0xB, 2, 1.0f);
    }
    if (unit->resourceLink) {
        btlMarkTaskReady(unit->resourceLink);
    }
    unit->status.flags |= 0x40000000;
    if (unit->status.flags & 0x20) {
        model = unit->ext->owner;
        unit->ext->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        unit->ext->flags &= ~0xA0;
        mdlSingleMotion(model, 0, 0xB);
        unit->motionId = 0xB;
        sdfMotionSeek(model->first, model->first->frameCount);
        unit->status.flags = unit->status.flags & 0x7FFFFFFF & 0xBFFFFFFF;
    } else if (btlTestActorStatusPredicate(unit)) {
        unit->ext->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        model = unit->ext->owner;
        unit->ext->flags &= ~0xA0;
        status = (BtlActorStatusRecord *)btlGetSideIndexedActorStatusTable(kind, id);
        mdlSingleMotion(model, 0, 1);
        unit->motionId = 1;
        sdfMotionSeek(model->first, status->model);
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
    sdfModelRootNode_t *load;
    dds3Basic_t *model;
    u32 state;
    u32 flags;

    if (object->modelBorrowed == BTL_MODEL_STATE_OWNED) {
        sound = object->soundSlotOwner;
        if (sound != 0) {
            sndReleaseSlotOwner(sound);
            object->soundSlotOwner = 0;
        }
        load = object->transparencyModel;
        if (load != 0) {
            sdfModelRootFree(load,
                             SDF_MODEL_ROOT_FREE_DEFERRED,
                             SDF_MODEL_MATERIAL_RESOURCES_RELEASE_DEFERRED);
            object->transparencyModel = 0;
            btlBossDebugPrintf(D_003A3AD0, object);
        }
        model = object->effectObject;
        if (model != 0) {
            dds3DeleteSceneBasicData(model);
            object->effectObject = 0;
            object->ext = 0;
        }
    } else {
        object->soundSlotOwner = 0;
        object->transparencyModel = 0;
        object->effectObject = 0;
        object->ext = 0;
    }
    state = object->gunResourceFlags & ~1;
    flags = object->status.flags & ~2;
    state &= ~2;
    object->status.flags = flags;
    object->gunResourceFlags = state;
}

extern s32 btlEnsureModelAndSoundReady(s32 kind, s32 id, s32 input);

void btlRequestModelAssetByMode(u32 unused, u32 effect, u32 model) {
    s32 available;

    available = evtBitCheck(0xc0f);
    if (available != 0) {
        btlEnsureModelAndSoundReady(effect, model, 0);
        return;
    }
    mdlPrepare(effect, model, MDL_ASSET_REQUEST_NONBLOCKING);
}

extern void btlReleaseFoundModelEntry(s32 modelKind, s32 modelId);

void btlReleaseModelAssetByMode(u32 unused, u32 effect, u32 model) {
    s32 available;

    available = evtBitCheck(0xc0f);
    if (available != 0) {
        btlReleaseFoundModelEntry(effect, model);
        return;
    }
}

u8 btlCheckModelAssetByMode(u8 *object, u32 effect, u32 model) {
    if (evtBitCheck(0xC0F) != 0) {
        if (btlEnsureModelAndSoundReady(effect, model, 0) != 0) {
            return 1;
        }
    } else {
        if (mdlPrepare(effect, model, MDL_ASSET_REQUEST_NONBLOCKING) != MDL_ASSET_REQUEST_QUEUED &&
            mdlPrepare(effect, model, MDL_ASSET_REQUEST_NONBLOCKING) != MDL_ASSET_REQUEST_ALREADY_PENDING) {
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

    callback = ((BtlState *)btlGetRuntime())->allowDefeatCandidateClear;
    if (callback != 0 && callback(object) == 0) {
        return;
    }
    flags = object->status.flags;
    masked = flags & ~4;
    masked &= ~8;
    object->status.flags = masked;
    if ((flags & 2) != 0) {
        dds3ModelHandle_t *resource = object->ext->owner;
        resource->flags |= MDL_SKIP_TRANSFORMS;
    }
}

s32 btlIsUnitInfoFlagOneEligible(BtlUnit *unit) {
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
    BtlState *context;
    BtlEffectResource *table;
    BtlRuntimeTask *task;
    dds3ModelHandle_t *model;
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
                task->startDelay = 1;
                task->ownerId = 0;
                btlStartTask(task);
                break;
            }
        }
        return;
    }
    context = btlGetRuntime();
    if (unit->updateFlags & 2) {
        color = (unit->overlayColor & 0xFFFFFF) | 0x80000000;
        evtSetUnitColorRGB(unit->ext, 0, color);
        evtSetUnitColorA(unit->ext, 0, color);
        unit->overlayColor = color;
        unit->updateFlags &= ~4;
        unit->updateFlags &= ~2;
    }
    table = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(unit->resourceKind,
                                                                unit->species);
    if (table->nodes[index].rateKind == 2) {
        unit->updateFlags |= 6;
    }
    chooseMotion = context->chooseMotion;
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
    if (unit->motionId == -1) {
        start = 0;
        end = 0;
    } else {
        switch (index) {
        case 11:
            mode = 2;
        case 0: case 2: case 9: case 10:
            start = unit->motionStartFrame;
            end = unit->motionEndFrame;
            break;
        case 1: case 18:
            start = 0;
            end = 1;
            break;
        case 3: case 4: case 5: case 6: case 7: case 8:
        case 12: case 19: case 20: case 21: case 22: case 23: case 24:
            node = mdlGetMotionNum(unit->ext->owner, 0);
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
    keepRange = context->preserveMotionRange;
    if (keepRange != 0 && keepRange(unit, index) != 0) {
        start = unit->motionStartFrame;
        end = unit->motionEndFrame;
    }
    if (mode & 0x100) {
        end = 8;
        mode &= ~0x100;
    }
    unit->motionRate = rate;
    unit->motionId = index;
    unit->effectState = mode;
    rate = rate * (30.0f / context->timingRate);
    rate *= context->modelFrameScale;
    evtSetUnitMotion(unit->ext, index, start, end, mode);
    model = unit->ext->owner;
    model->first->frameStep = rate;
    if (end == 0) {
        mdlLoopMotion(model, 0, index);
        sdfMotionSeek(unit->ext->owner->first, 0.0f);
    }
    unit->motionStartFrame = 0;
    frameCount = table->nodes[index].frameCount;
    unit->motionEndFrame = frameCount;
    if (mode != 0 && mode != 3) {
        return;
    }
    evtSetWaitHokanFrame(unit->ext, 0, (s16)frameCount);
    unit->motionStartFrame = 0;
    unit->motionEndFrame = table->nodes[unit->effectIndex].frameCount;
}

void btlRefreshUnitMotionSelection(BtlUnit *unit) {
    s32 entryFlags;
    BtlState *context;
    s32 index;
    s32 selected;
    s32 mode;
    BtlActorStatusRecord *statusTable;
    f32 rate;
    f32 speed;
    u32 color;
    s32 (*chooseStatus)(BtlUnit *);
    s32 (*chooseMotion)(BtlUnit *, s32, s32);

    if (((u32)unit->status.flags & 2) == 0) {
        return;
    }
    entryFlags = btlGetEntryFlagsUnlessDisabled(&unit->partyRecord);
    context = btlGetRuntime();
    if (unit->updateFlags & 2) {
        color = (unit->overlayColor & 0xFFFFFF) | 0x80000000;
        evtSetUnitColorRGB(unit->ext, 0, color);
        evtSetUnitColorA(unit->ext, 0, color);
        unit->overlayColor = color;
        unit->updateFlags &= ~4;
        unit->updateFlags &= ~2;
    }
    index = 0;
    if (btlIsCurrentValueBelowQuarterThreshold(unit) != 0 &&
        (((u32)unit->status.flags & 0x200) || (entryFlags & 0x200))) {
        index = 10;
    }
    if (unit->selectedEntryIndex > 0 &&
        (((u32)unit->status.flags & 0x200) || (entryFlags & 0x200))) {
        index = 9;
    }
    switch (unit->partyRecord.status & 0x7FFF) {
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
    chooseStatus = context->statusMotionOverride;
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
    statusTable = (BtlActorStatusRecord *)btlGetSideIndexedActorStatusTable(
        unit->resourceKind, unit->species);
    rate = statusTable->motions[index].alphaFrameScale;
    chooseMotion = context->chooseMotion;
    if (chooseMotion != 0) {
        selected = chooseMotion(unit, index, 1);
        if (selected == -1) {
            return;
        }
        if (index != selected) {
            index = selected;
            rate = statusTable->motions[index].alphaFrameScale;
        }
    }
    unit->effectScale = rate;
    speed = rate * (30.0f / context->timingRate);
    speed *= context->modelFrameScale;
    unit->effectIndex = index;
    evtSetWaitMotion(unit->ext, index);
    evtSetWaitMotionSpeed(unit->ext, speed);
    mode = 1;
    if (index == 11) {
        mode = 2;
    }
    unit->effectParameter = mode;
    if (unit->motionId != 11 &&
        (btlIsActorModeAcceptedByBattleHook(unit) != 0 || index == 11) &&
        unit->motionId != index) {
        btlApplyUnitMotionSelection(unit, index, mode, rate);
    }
}

s32 btlIsActorModeAcceptedByBattleHook(BtlUnit *object) {
    s32 value;
    if (((u32)object->status.flags & 2) == 0) {
        return 0;
    }
    {
        s32 (*callback)(BtlUnit *) = ((BtlState *)btlGetRuntime())->statusMotionOverride;
        if (callback != 0 && callback(0) == object->motionId) {
            return 1;
        }
    }
    value = object->motionId;
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

void btlApplyScaledUnitEffectParameter(BtlUnit *unit, s32 index, s32 argument, f32 scale) {
    BtlEffectResource *resource = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(
        unit->resourceKind, unit->species);
    btlApplyUnitMotionSelection(unit, index, argument, resource->nodes[index].scale * scale);
}

s32 btlGetSlotRateKind(BtlUnit *unit, s32 index) {
    BtlEffectResource *resource = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(
        unit->resourceKind, unit->species);
    s32 value = resource->nodes[index].rateKind;

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
    BtlState *context = btlGetRuntime();
    BtlUnit *object = context->units;

    while (object != 0) {
        if ((u32)object->status.flags & 2) {
            BtlEffectResource *resource = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(
                object->resourceKind, object->species);
            dds3ModelHandle_t *model = object->ext->owner;
            s32 node = mdlGetMotionNum(model, 0);
            if (resource->nodes[node].rateKind == 1 &&
                btlIsActorModeAcceptedByBattleHook(object) == 0) {
                btlRefreshUnitMotionSelection(object);
                btlApplyUnitMotionSelection(object, object->effectIndex, object->effectParameter,
                                            object->effectScale);
            }
        }
        object = object->next;
    }
}

void btlApplyUnitModelScaledValue(BtlUnit *object) {
    s32 context;
    f32 volume;
    if ((object->status.flags & 2) == 0) {
        return;
    }
    context = (s32)(u32)btlGetRuntime();
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

s32 btlGetUnitMotionDurationFrames(BtlUnit *unit, s32 motionIndex) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlActorStatusRecord *status = (BtlActorStatusRecord *)
        btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->species);

    return (s32)(status->motions[motionIndex].frameCount /
        (status->motions[motionIndex].alphaFrameScale * state->modelFrameScale));
}

/* Return the unit model's current frame, or zero while its model flag is clear. */
f32 btlGetUnitModelCurrentFrame(BtlUnit *unit) {
    if ((unit->status.flags & 2) == 0) {
        return 0.0f;
    }
    return unit->ext->owner->first->currentFrame;
}

/* The caller's frame argument is forwarded unchanged to the sampler. */
void btlAdvanceUnitModelFrame(BtlUnit *unit, f32 frame) {
    if ((unit->status.flags & 2) != 0) {
        sdfMotionSeek(unit->ext->owner->first, frame);
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
    sdfMotionSeek(unit->ext->owner->first, 0.0f);
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
        randomFrame = dds3GetRandIntA(0, duration);
        frame = (f32)randomFrame;
        sdfMotionSeek(object->ext->owner->first, frame);
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

void btlSetUnitPosition(BtlUnit *object, const f32 *position) {
    f32 world[4] __attribute__((aligned(16)));
    s32 context;
    if ((object->status.stateFlags & 0x80) != 0) {
        return;
    }
    context = (s32)(u32)btlGetRuntime();
    SDF_VU0_LOADVECTOR(position);
    SDF_VU0_STOREVECTOR((u8 *)object + 0x60);
    SDF_VU0_LOADVECTOR_B(context);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(world);
    if (((u32)object->status.flags & 2) != 0) {
        world[2] += *(f32 *)((u8 *)object + 0x88);
        dds3SetPosPoseSet(object->effectObject, (u128 *)world);
    }
}

/* Copy the stored current-position vector, without the runtime world offset. */
void btlCopyUnitCurrentPosition(BtlUnit *unit, f32 *destination) {
    SDF_VU0_COPYVECTOR(destination, (u8 *)unit + 0x60);
}

void btlGetUnitWorldPos(BtlUnit *unit, f32 *worldPosition) {
    s32 context = (s32)(u32)btlGetRuntime();
    SDF_VU0_LOADVECTOR(unit->currentPosition);
    SDF_VU0_LOADVECTOR_B(context);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(worldPosition);
}

extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *, s32);


extern void btlRefreshUnitFxVectors(BtlUnit *);

BtlActorEffectPositionResult btlSetActorEffectParameter(object, value)
BtlUnit *object;
s32 value;
{
    s32 (*callback)(BtlUnit *, s32);
    if ((object->status.flags & 2) == 0) {
        return BTL_ACTOR_EFFECT_POSITION_UNAVAILABLE;
    }
    callback = ((BtlState *)btlGetRuntime())->effectParameterCallback;
    if (callback != 0) {
        value = callback(object, value);
    }
    btlRefreshUnitFxVectors(object);
    {
        dds3ModelHandle_t *owner = object->ext->owner;
        return (s8)sdfModelGetMarkPos(owner->inner, value);
    }
}

void effBTLModelGetMarkPosition2(BtlUnit *unit, s32 mode) {
    BtlActorEffectPositionResult positionResult;

    positionResult = btlSetActorEffectParameter(unit, mode);
    if (positionResult == BTL_ACTOR_EFFECT_POSITION_UNAVAILABLE) {
        btlUnitGetMuzzlePosVU(unit);
        return;
    }
}

/* vu0 routine: preserve the actor's primary position and rotation quaternion while
 * evaluating the requested model record; return the sampled vector in vf10. */
s32 btlSampleActorModelPositionVU(BtlUnit *unit, s32 value) {
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
    mdlGetPosition(unit->ext->owner);
    SDF_VU0_STOREVECTOR_NOMEM(primaryVector);
    mdlGetRotate(unit->ext->owner);
    SDF_VU0_STOREVECTOR_NOMEM(rotationQuaternion);
    btlRefreshUnitFxVectors(unit);
    result = sdfModelGetMarkPos(unit->ext->owner->inner, value);
    SDF_VU0_STOREVECTOR_NOMEM(currentVector);
    SDF_VU0_LOADVECTOR(primaryVector);
    mdlSetPosition(unit->ext->owner);
    SDF_VU0_LOADVECTOR(rotationQuaternion);
    mdlSetRotate(unit->ext->owner);
    sdfModelEvalTm(unit->ext->owner->inner);
    SDF_VU0_LOADVECTOR(currentVector);
    return result;
}

extern s32 sdfModelGetMarkPosMatrix(sdfModelRootNode_t *, s32);

s8 btlSetActorAlternateEffectParameter(object, value)
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
        dds3ModelHandle_t *owner = object->ext->owner;
        return (s8)sdfModelGetMarkPosMatrix(owner->inner, value);
    }
}

void effBTLModelGetMarkMatrix2(BtlUnit *unit, s32 mode) {
    if (btlSetActorAlternateEffectParameter(unit, mode) != 0) {
        return;
    }
    SDF_VU0_LOADIDENTITY();
}

s32 btlIsUnitAtStoredPosition(u8 *object) {
    f32 position[4];
    btlCopyUnitCurrentPosition((BtlUnit *)object, position);
    if (*(f32 *)(object + 0x30) == position[0] &&
        *(f32 *)(object + 0x34) == position[1] &&
        *(f32 *)(object + 0x38) == position[2]) {
        return 1;
    }
    return 0;
}

extern u8 D_003A3B70[];

void btlSetUnitRotation(BtlUnit *object, const f32 rotation[4]) {
    u8 vector[16];
    if ((object->status.stateFlags & 0x100) != 0) {
        return;
    }
    SDF_VU0_LOADVECTOR(rotation);
    if (((u32)object->status.flags & 0x10) != 0) {
        SDF_VU0_LOADVECTOR_B(D_003A3B70);
        effMiscQuatMultiplyVU();
    }
    SDF_VU0_STOREVECTOR_NOMEM((u8 *)object + 0x70);
    SDF_VU0_LOADVECTOR_B(D_003A3B70);
    effMiscQuatMultiplyVU();
    SDF_VU0_STOREVECTOR_NOMEM(vector);
    if (((u32)object->status.flags & 2) != 0) {
        dds3SetRotPoseSet(object->effectObject, (u128 *)vector);
    }
}

void btlCopyUnitRotationQuaternion(BtlUnit *unit, f32 dst[4]) {
    SDF_VU0_COPYVECTOR(dst, unit->orientation);
}

void btlSetUnitColor(BtlUnit *unit, u32 color, s32 mode) {
    if ((u32)unit->status.flags & 2) {
        color = (color & 0xFFFFFF) | 0x80000000;
        unit->baseColor = (unit->baseColor & 0xFF000000) | (color & 0xFFFFFF);
        evtSetUnitColorRGB(unit->ext, mode, color);
    }
}

void btlBlendUnitColor(BtlUnit *unit, u32 color, s32 mode) {
    u32 base;
    u32 blended;
    if ((u32)unit->status.flags & 2) {
        color = (color & 0xFFFFFF) | 0x80000000;
        base = (unit->baseColor & 0xFFFFFF) | 0x80000000;
        blended = (base & color) + (((base ^ color) & 0xFEFEFEFE) >> 1);
        unit->overlayColor = (unit->overlayColor & 0xFF000000) | (color & 0xFFFFFF);
        evtSetUnitColorRGB(unit->ext, mode, blended);
    }
}

extern void mdlSetClutColor(dds3ModelHandle_t *, s32, f32);

void btlReleaseUnitModelColorResource(BtlUnit *unit, s32 value, f32 scalar) {
    mdlSetClutColor(unit->ext->owner, (value & 0xffffff) | 0x80000000, scalar);
}

void btlRefreshUnitFxVectors(BtlUnit *unit) {
    if (!(unit->status.flags & 2)) {
        return;
    }
    dds3GetPosPoseSet(unit->effectObject);
    mdlSetPosition(unit->ext->owner);
    dds3GetRotPoseSet(unit->effectObject);
    mdlSetRotate(unit->ext->owner);
    sdfModelEvalTm(unit->ext->owner->inner);
}

extern void btlUnitGetBodyPosVU(u8 *);


void btlUnitFaceTarget(BtlUnit *object, BtlUnit *target) {
    u8 first[16];
    u8 second[16];
    u8 result[16];
    if (((u32)object->status.flags & 0x80000) != 0) {
        btlUnitGetBodyPosVU(object);
        SDF_VU0_STOREVECTOR_NOMEM(first);
        btlUnitGetBodyPosVU(target);
        SDF_VU0_STOREVECTOR_NOMEM(second);
        if (btlAimHorizontalDirectionVU((f32 *)first, (f32 *)second) != 0) {
            SDF_VU0_STOREVECTOR_NOMEM(result);
            btlSetUnitRotation(object, (const f32 *)result);
        }
    }
}

void btlUnitFaceTargetScaled(BtlUnit *object, BtlUnit *target, f32 scale) {
    f32 first[4] __attribute__((aligned(16)));
    f32 second[4] __attribute__((aligned(16)));
    u8 result[16];
    if (((u32)object->status.flags & 0x80000) != 0) {
        btlUnitGetBodyPosVU(object);
        SDF_VU0_STOREVECTOR_NOMEM(first);
        btlUnitGetBodyPosVU(target);
        SDF_VU0_STOREVECTOR_NOMEM(second);
        btlAimHorizontalDirectionClampedVU(first, second, scale);
        SDF_VU0_STOREVECTOR_NOMEM(result);
        btlSetUnitRotation(object, (const f32 *)result);
    }
}

void btlCopyUnitStats(BtlUnit *unit, datUnitWork_t *source) {
    datUnitWork_t *stats = &unit->partyRecord;
    *stats = *source;
    btlRefreshUnitMaximumHpAndClampCurrentHp(stats);
    btlRefreshUnitMaximumMpAndClampCurrentMp(stats);
}

extern void mdlLinkEffect(dds3ModelHandle_t *, dds3ModelEffectCtrlr_t *);

#include "sdf_packet_alloc.h"

extern void sdfModelCopyStat(sdfModelRootNode_t *, sdfModelRootNode_t *);


extern u64 btlTransparencySurfaceTestValues[4];

/* Draw the model into four surfaces in three GS TEST passes, then update its anchors. */
void btlDrawUnitTransparencyPasses(BtlUnit *unit, dds3ModelHandle_t *model, sdfModelRootNode_t *overlay, SdfPoolNode **surfaces, u32 color) {
    sdfDrawTag_t *list;
    u64 *packet;
    dds3ModelEffectCtrlr_t *item;
    u16 savedFlags;
    s32 i;

    if (model->flags & MDL_SKIP_TRANSFORMS) {
        return;
    }
    mdlSetColor(model, color);
    for (i = 0; i != 4; i++) {
        list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        packet = (u64 *)sdfGrAllocPacketBuff(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = 0x72801;
        packet[5] = 0x47;
        sdfDrawTagLink(list, (u32)packet);
        surfaces[i]->append(surfaces[i], list);
    }
    savedFlags = model->inner->drawFlags;
    model->flags |= MDL_SKIP_ANCHORS;
    model->inner->drawFlags = 0x2000;
    mdlLink(model, surfaces);
    model->flags &= ~MDL_SKIP_ANCHORS;
    model->inner->drawFlags = savedFlags;
    for (i = 0; i != 4; i++) {
        list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        packet = (u64 *)sdfGrAllocPacketBuff(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = 0x51801;
        packet[5] = 0x47;
        sdfDrawTagLink(list, (u32)packet);
        surfaces[i]->append(surfaces[i], list);
    }
    sdfModelCopyStat(overlay, model->inner);
    if (unit->status.flags & 2) {
        overlay->lighting = (struct SdfDmaTagHeader *)unit->ext->endpointWork;
    } else {
        overlay->lighting = NULL;
    }
    sdfBuildModelPacketsVia(sdfModelBuildNodeResourcePackets, surfaces, overlay);
    for (i = 0; i != 4; i++) {
        list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        packet = (u64 *)sdfGrAllocPacketBuff(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = btlTransparencySurfaceTestValues[i];
        packet[5] = 0x47;
        sdfDrawTagLink(list, (u32)packet);
        surfaces[i]->append(surfaces[i], list);
    }
    mdlSetEffectColor(model, color);
    for (item = model->resourceItems; item != NULL; item = item->next) {
        mdlLinkEffect(model, item);
    }
}

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3B70);

void btlCreateUnitTransparency(BtlUnit *unit) {
    dds3ModelResrc_t *shape;
    if ((unit->status.flags & 2) == 0) {
        return;
    }
    if (unit->transparencyModel != 0) {
        return;
    }
    if (unit->modelBorrowed != BTL_MODEL_STATE_OWNED) {
        return;
    }
    shape = unit->ext->owner->sub;
    unit->transparencyModel = sdfModelMake(shape->resourceList, shape->itemList);
    dds3ObjectBaseOnFlag(unit->effectObject, 1);
    btlBossDebugPrintf("btl:unit transparency create[%p]\n", unit);
}

extern struct SdfPoolNode *kwlnSceneSurfaceSets[13][4];

extern SdfPoolNode *btlUnitTransparencySurfaces[];

void btlUpdateUnitTransparency(BtlUnit *unit) {
    u32 flags = unit->status.flags;
    u32 color;
    dds3ModelHandle_t *info;
    u32 alpha;

    if (flags & 2) {
        if (unit->modelBorrowed == BTL_MODEL_STATE_OWNED) {
            color = unit->overlayColor;
            info = unit->ext->owner;
            alpha = color >> 24;
            if (!(flags & 0x20000)) {
                if (unit->transparencyModel != 0) {
                    sdfModelRootFree(unit->transparencyModel,
                                     SDF_MODEL_ROOT_FREE_DEFERRED,
                                     SDF_MODEL_MATERIAL_RESOURCES_RELEASE_DEFERRED);
                    unit->transparencyModel = 0;
                    if (unit->status.flags & 2) {
                        info->inner->lighting = (struct SdfDmaTagHeader *)unit->ext->endpointWork;
                    } else {
                        info->inner->lighting = 0;
                    }
                    mdlSetColor(info, color);
                    mdlLink(info, kwlnSceneSurfaceSets[0]);
                    dds3ObjectBaseOffFlag(unit->effectObject, 1);
                    btlBossDebugPrintf(D_003A3AD0, unit);
                }
            } else if (alpha == 0) {
                dds3ObjectBaseOnFlag(unit->effectObject, 1);
            } else if (unit->transparencyModel == 0) {
                btlCreateUnitTransparency(unit);
            } else {
                btlDrawUnitTransparencyPasses(unit, info, unit->transparencyModel, btlUnitTransparencySurfaces, color);
            }
        }
    }
}

extern sdfGrFrameSet_t D_003980E0;

extern SdfPoolNode *btlMirrorSourceTransparencySurfaces[];

extern SdfPoolNode *btlMirrorTargetTransparencySurfaces[];


/* Draw the unit's transparency model into its mirror's packet buffer, then draw the mirror from it. Either
 * model is created (and the update ends) on the first frame it is missing. */
/* Emit the mirror model's transparency packets using its overlay color. */
void btlDrawUnitMirrorTransparency(BtlUnit *unit) {
    dds3ModelHandle_t *info;
    sdfDrawTag_t *list;

    if ((unit->status.flags & 2) == 0) {
        return;
    }
    if (unit->modelBorrowed != BTL_MODEL_STATE_OWNED) {
        return;
    }
    unit->mirror->transparencyBuffer = sdfGrAllocPacketBuff(0x70000);
    list = sdfPrimAllocDrawTag(0);
    sdfLinkStoreImage(list, D_003980E0.buffers[2],
                            0, 0, 0x200, 0xE0, unit->mirror->transparencyBuffer, 0, 0, 0);
    btlMirrorSourceTransparencySurfaces[0]->append(btlMirrorSourceTransparencySurfaces[0], list);
    info = unit->ext->owner;
    if (unit->transparencyModel == 0) {
        unit->transparencyModel = sdfModelMake(info->sub->resourceList, info->sub->itemList);
        dds3ObjectBaseOnFlag(unit->effectObject, 1);
        return;
    }
    btlDrawUnitTransparencyPasses(unit, info, unit->transparencyModel, btlMirrorSourceTransparencySurfaces, unit->overlayColor);
    info = unit->mirror->ext->owner;
    if (unit->mirror->transparencyModel == 0) {
        unit->mirror->transparencyModel = sdfModelMake(info->sub->resourceList, info->sub->itemList);
        dds3ObjectBaseOnFlag(unit->mirror->effectObject, 1);
        return;
    }
    list = sdfPrimAllocDrawTag(0);
    sdfLinkLoadImage(list, D_003980E0.buffers[2],
                              0, 0, 0x200, 0xE0, unit->mirror->transparencyBuffer, 0);
    btlMirrorTargetTransparencySurfaces[0]->append(btlMirrorTargetTransparencySurfaces[0], list);
    btlDrawUnitTransparencyPasses(unit->mirror, info, unit->mirror->transparencyModel, btlMirrorTargetTransparencySurfaces, unit->mirror->overlayColor);
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
        sprintf(filename, D_003A3BA8, D_003BB5F8, 0,
                      actor->partyRecord.unitId);
    } else {
        sprintf(filename, D_003A3BA8, D_003BB5F8,
                      btlGetActorBedAssetIdFromIndex(actor->partyRecord.menuValue),
                      actor->partyRecord.unitId);
    }
    return 1;
}

extern u8 D_0037E110[];

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3BA8);

const char btlUnitWarpFormat[16] = "btl:warp[%p]\n";

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
    SDF_VU0_STOREVECTOR(bodyPos);
    if (target != NULL) {
        btlUnitGetMuzzlePosVU(target);
        SDF_VU0_STOREVECTOR_NOMEM(muzzlePos);
        muzzlePos[1] = bodyPos[1];
        SDF_VU0_LOADVECTOR(muzzlePos);
        SDF_VU0_LOADVECTOR_B(bodyPos);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
    } else {
        SDF_VU0_LOADVECTOR(unit->orientation);
        effMiscQuaternionToMatrixVU();
        SDF_VU0_LOADVECTOR(D_0037E110);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_LOADVECTOR_B(bodyPos);
    }
    VU0_SCALAR_OP(reach, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(world);
    SDF_VU0_LOADVECTOR(unit->orientation);
    effMiscQuaternionToMatrixVU();
    SDF_VU0_LOADVECTOR(unit->bodyOffset);
    VU0_SCALAR_OP(unit->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_NEGVECTOR_XYZ();
    SDF_VU0_LOADVECTOR_B(world);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(world);
    if ((unit->status.flags & 2) != 0) {
        world[1] = 0;
        if (sdfModelGetMarkPos(unit->ext->owner->inner, 0) == 0) {
            dds3GetPosPoseSet(unit->effectObject);
        }
        VU0_SCALAR_OP(0.0f, "vaddx.y vf10, vf0, vf2x");
        SDF_VU0_LOADVECTOR_B(world);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_GETLENGTH(distance);
        if (distance < 2.0f * (unit->bodyRadius * unit->scale)) {
            dds3SetPosPoseSet(unit->effectObject, (u128 *)world);
            unit->status.stateFlags |= 0x8000;
            btlBossDebugPrintf(btlUnitWarpFormat, unit);
        }
    }
}

void btlRefreshUnitEffectMotionAndEntry(BtlUnit *unit) {
    u32 flags = (u32)unit->status.flags;
    if ((flags & 2) == 0) {
        return;
    }
    if (unit->motionId != unit->effectIndex) {
        btlRefreshUnitMotionSelection(unit);
        unit->motionStartFrame = 0;
        unit->motionEndFrame = 0;
        btlApplyUnitMotionSelection(unit, unit->effectIndex, unit->effectParameter,
                                    unit->effectScale);
        flags = (u32)unit->status.flags;
    }
    if ((flags & 0x2000) == 0) {
        s32 index = unit->effectIndex;
        if (index != 11) {
            mdlLoopMotion(unit->ext->owner, 0, index);
        } else {
            mdlSingleMotion(unit->ext->owner, 0, 11);
        }
        sdfMotionSeek(unit->ext->owner->first, 0.0f);
    }
}

u32 btlApplyIndexedUnitEffectTask(BtlRuntimeTaskArgs *taskArgs) {
    s32 index = taskArgs->option;
    if (index >= 0) {
        btlApplyScaledUnitEffectParameter(taskArgs->actor, index, taskArgs->effectParameterOption, taskArgs->scale2);
    }
    return 1;
}

BtlRuntimeTask *btlAllocateIndexedUnitEffectTask(BtlUnit *owner, s32 index, s32 value, f32 scale) {
    BtlRuntimeTask *task = btlAllocTask(16);
    u8 *taskBytes = (u8 *)task;
    u8 *ownerBytes = (u8 *)owner;
    BtlRuntimeTaskArgs *arguments;

    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u16 *)((u8 *)task + 0x20) = 9;
    *(void **)((u8 *)task + 0x4C) = btlApplyIndexedUnitEffectTask;
    *(u64 *)((u8 *)task + 0x40) = *(u64 *)(ownerBytes + 0x108);
    *(u32 *)((u8 *)task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments->actor = owner;
    arguments->option = index;
    arguments->effectParameterOption = value;
    arguments->scale2 = scale;
    return task;
}

u32 btlApplyScaledUnitModelTask(BtlUnit **taskArgs) {
    btlApplyUnitModelScaledValue(*taskArgs);
    return 1;
}

BtlRuntimeTask *btlCreateScaledUnitModelTask(BtlUnit *owner) {
    BtlRuntimeTask *task = btlAllocTask(4);
    u8 *taskBytes = (u8 *)task;
    u8 *ownerBytes = (u8 *)owner;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)((u8 *)task + 0x4C) = btlApplyScaledUnitModelTask;
    *(u16 *)((u8 *)task + 0x20) = 10;
    *(s64 *)((u8 *)task + 0x40) = *(s64 *)(ownerBytes + 0x108);
    *(u32 *)((u8 *)task + 0x48) = 0;
    *(BtlUnit **)btlGetTaskArguments(task) = owner;
    return task;
}

u32 btlPollThresholdTask(BtlThresholdTaskArgs *arguments) {
    if ((s32)btlGetUnitModelCurrentFrame(arguments->unit) >= arguments->frameThreshold) {
        if ((arguments->unit->updateFlags & 1) == 0) {
            btlResetUnitModelProgress(arguments->unit);
        }
        return 1;
    }
    return 0;
}

BtlRuntimeTask *btlScheduleThresholdTask(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    u8 *taskBytes = (u8 *)task;
    u8 *actorBytes = (u8 *)actor;
    BtlThresholdTaskArgs *arguments;

    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)((u8 *)task + 0x20) = 0xB;
    *(void **)((u8 *)task + 0x4C) = btlPollThresholdTask;
    *(s64 *)((u8 *)task + 0x40) = *(s64 *)(actorBytes + 0x108);
    *(u32 *)((u8 *)task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = actor;
    arguments->frameThreshold = option;
    return task;
}


s32 btlApproachTargetTask(BtlApproachTaskArgs *args) {
    BtlUnit *unit = args->unit;
    BtlUnit *target = args->target;
    f32 scale;
    f32 reach;
    f32 dist;
    f32 pos[4] __attribute__((aligned(16)));
    f32 fromPos[4] __attribute__((aligned(16)));
    f32 toPos[4] __attribute__((aligned(16)));
    scale = args->scale == 0.0f ? 1.0f : args->scale;
    if (args->count == 0) {
        BtlEffectResource *table = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->species);
        args->motionReachOffset = table->nodes[unit->motionId].reachOffset * unit->scale;
    }
    reach = args->motionReachOffset + target->reach * target->scale;
    btlUnitGetMuzzlePosVU(unit);
    SDF_VU0_STOREVECTOR(fromPos);
    btlUnitGetMuzzlePosVU(target);
    SDF_VU0_STOREVECTOR_NOMEM(toPos);
    toPos[1] = fromPos[1];
    SDF_VU0_LOADVECTOR(fromPos);
    VU0_LOAD_VF_PTR(vf11, toPos);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    VU0_SCALAR_OP(reach, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_ADDVECTOR();
    SDF_VU0_COPYVECTOR_C_TO_B();
    VU0_LERP_VF10(0.8f / scale);
    SDF_VU0_STOREVECTOR(pos);
    SDF_VU0_LOADVECTOR_B(fromPos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(dist);
    SDF_VU0_LOADVECTOR(&unit->orientation);
    effMiscQuaternionToMatrixVU();
    SDF_VU0_LOADVECTOR(&unit->bodyOffset);
    VU0_SCALAR_OP(unit->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_NEGVECTOR_XYZ();
    SDF_VU0_LOADVECTOR_B(pos);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(pos);
    pos[2] -= unit->positionZOffset;
    btlSetUnitPosition(args->unit, pos);
    btlUnitFaceTarget(unit, target);
    if (dist < 1.0f) {
        return 1;
    }
    args->count++;
    return 0;
}

extern s32 btlApproachTargetTask(BtlApproachTaskArgs *);

BtlRuntimeTask *btlAllocateApproachTargetTask(BtlUnit *unit, BtlUnit *target, f32 scale) {
    BtlRuntimeTask *task = btlAllocTask(24);
    BtlApproachTaskArgs *arguments;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlApproachTargetTask;
    task->taskId = 0xE;
    task->ownerId = unit->identity;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = unit;
    arguments->target = target;
    arguments->scale = scale;
    arguments->motionReachOffset = 0.0f;
    arguments->count = 0;
    return task;
}

s32 btlUpdateUnitPositionInterpolationTask(BtlPosLerpTaskArgs *args) {
    s128 pos;
    f32 t = args->t;
    f32 rate;
    BtlUnit *unit = args->unit;

    if (t < 1.0f && args->rate > 0.0f && args->rate < 1.0f) {
        rate = args->rate;
        if (args->count == 0) {
            SDF_VU0_COPYVECTOR(&args->from, unit->currentPosition);
        }
        args->t = t + (1.0f - t) * rate;
        if (args->t > 0.999f) {
            args->t = 1.0f;
        }
        SDF_VU0_LOADVECTOR(&args->from);
        SDF_VU0_LOADVECTOR_B(&args->to);
        VU0_LERP_VF10(args->t);
        SDF_VU0_STOREVECTOR(&pos);
        btlSetUnitPosition(unit, (f32 *)&pos);
    } else {
        btlSetUnitPosition(unit, (f32 *)&args->to);
        return 1;
    }
    args->count++;
    return 0;
}

BtlRuntimeTask *btlCreateUnitPositionLerpTowardTargetTask(BtlUnit *unit, const f32 target[4], f32 scale) {
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
    SDF_VU0_COPYVECTOR(&args->from, unit->currentPosition);
    SDF_VU0_COPYVECTOR(&args->to, target);
    return task;
}

extern void Lerp(f32);

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
                SDF_VU0_COPYVECTOR(&args->from, unit->orientation);
            }
            args->t = t + (1.0f - t) * rate;
            if (args->t > 0.999f) {
                args->t = 1.0f;
            }
            SDF_VU0_LOADVECTOR(&args->from);
            SDF_VU0_LOADVECTOR_B(&args->to);
            Lerp(args->t);
            SDF_VU0_STOREVECTOR(&quat);
            btlSetUnitRotation(unit, (const f32 *)&quat);
            return 0;
        }
    }
    btlSetUnitRotation(unit, (const f32 *)&args->to);
    return 1;
}

BtlRuntimeTask *btlCreateUnitRotationInterpolationTask(BtlUnit *unit, const f32 targetOrientation[4], f32 interpolationRate) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(BtlRotationTaskArgs));
    BtlRotationTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0xD;
    task->ownerId = unit->identity;
    task->callback = btlStepUnitRotationNlerp;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->rate = interpolationRate;
    args->unit = unit;
    args->t = 0.0f;
    args->count = 0;
    SDF_VU0_COPYVECTOR(&args->from, unit->orientation);
    SDF_VU0_COPYVECTOR(&args->to, targetOrientation);
    return task;
}

extern char D_003A3BC8[];

extern char D_003A3BE8[];

void btlRequestModelOrReuse(BtlRuntimeTaskArgs *arguments) {
    BtlUnit *object = arguments->actor;
    u32 effect = arguments->option;
    u32 model = arguments->unk_08;
    if (((u32)object->status.flags & 2) != 0) {
        return;
    }
    if (btlHasMatchingModel(effect, model)) {
        btlBindActorModel(object, effect, model);
        if (arguments->mode == 0) {
            btlClearUnitDefeatCandidate(object);
            evtSetUnitColorA(object->ext, 0, 0);
            object->overlayColor = object->baseColor & 0xFFFFFF;
        }
        btlBossDebugPrintf(D_003A3BC8, effect, model);
    } else {
        btlRequestModelAssetByMode(object, effect, model);
        object->gunResourceFlags |= 1;
        btlBossDebugPrintf(D_003A3BE8, effect, model);
    }
}

extern char D_003A3C08[];

extern u8 btlCheckModelAssetByMode(u8 *, u32, u32);

u32 btlPollModelLoadCompletion(BtlRuntimeTaskArgs *arguments) {
    BtlUnit *object = arguments->actor;
    u32 effect = arguments->option;
    u32 model = arguments->unk_08;
    if (((u32)object->status.flags & 2) == 0) {
        if (!btlCheckModelAssetByMode(object, effect, model)) {
            return 0;
        }
        btlBindActorModel(object, effect, model);
        btlReleaseModelAssetByMode(object, effect, model);
        btlBossDebugPrintf(D_003A3C08, effect, model, object);
    }
    if (arguments->mode == 0) {
        btlClearUnitDefeatCandidate(object);
        evtSetUnitColorA(object->ext, 0, 0);
        object->overlayColor = object->baseColor & 0xFFFFFF;
    }
    object->gunResourceFlags = (object->gunResourceFlags & ~1) | 2;
    return 1;
}

BtlRuntimeTask *btlCreateModelLoadPollTask(BtlUnit *owner, u32 effectId, u32 modelId, s8 preserveDefeatCandidate) {
    BtlRuntimeTask *task = btlAllocTask(16);
    BtlRuntimeTaskArgs *arguments;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x18;
    task->flags &= ~BTL_TASK_FLAG_REGISTERED;
    task->ownerId = owner->identity;
    task->onStart = btlRequestModelOrReuse;
    task->callback = btlPollModelLoadCompletion;
    arguments = btlGetTaskArguments(task);
    arguments->actor = owner;
    arguments->option = effectId;
    arguments->unk_08 = modelId;
    arguments->mode = preserveDefeatCandidate;
    return task;
}

u32 btlReleaseUnitModelTask(BtlUnit **taskArgs) {
    btlClearUnitDefeatCandidate(*taskArgs);
    btlReleaseActorModelResources(*taskArgs);
    return 1;
}

BtlRuntimeTask *btlScheduleRefreshTask(BtlUnit *owner) {
    u8 *ownerBytes = (u8 *)owner;
    BtlRuntimeTask *task = btlAllocTask(4);
    u8 *taskBytes = (u8 *)task;
    BtlUnit **arguments;

    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)((u8 *)task + 0x4C) = btlReleaseUnitModelTask;
    *(u16 *)((u8 *)task + 0x20) = 0x19;
    *(s64 *)((u8 *)task + 0x40) = *(s64 *)(ownerBytes + 0x108);
    arguments = btlGetTaskArguments(task);
    *arguments = owner;
    return task;
}

/* Task-start callbacks do not return a status to the scheduler. */
INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3BC8);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3BE8);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3C08);

void btlBeginModelChange(BtlModelChangeArgs *arguments) {
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
/* Advance model replacement and mirror crossfade; return one after cleanup. */
u32 btlUpdateModelChangeTask(BtlModelChangeArgs *args) {
    BtlUnit *unit = args->unit;
    u32 resourceKind = args->resourceKind;
    u32 resourceId = args->resourceId;
    u32 packedStart[4];
    u32 packedEnd[4];
    u32 alpha;
    u32 color;
    s32 entryFlags;

    switch (args->phase) {
    case BTL_MODEL_CHANGE_WAITING_FOR_ASSET:
        if (args->delay > args->elapsed) {
            break;
        }
        if (!btlCheckModelAssetByMode((u8 *)unit, resourceKind, resourceId)) {
            break;
        }
        if (args->duration != 0) {
            unit->mirror = btlCreateUnit();
            unit->mirror->status.flags |= 0x40000;
            btlInheritActorModelState(unit->mirror, unit);
            if (unit->modelBorrowed == BTL_MODEL_STATE_OWNED) {
                unit->mirror->modelBorrowed = BTL_MODEL_STATE_OWNED;
                unit->modelBorrowed = BTL_MODEL_STATE_BORROWED;
            }
            btlSetUnitPosition(unit->mirror, unit->currentPosition);
            btlSetUnitRotation(unit->mirror, unit->orientation);
            btlSetUnitColor(unit->mirror, unit->baseColor, 0);
            unit->mirror->status.flags |= 8;
            args->phase = BTL_MODEL_CHANGE_CROSSFADE;
            args->elapsed = 0;
        } else {
            args->phase = BTL_MODEL_CHANGE_CLEANUP;
        }
        btlReleaseActorModelResources(unit);
        btlRefreshUnitMaximumHpAndClampCurrentHp(&unit->partyRecord);
        btlRefreshUnitMaximumMpAndClampCurrentMp(&unit->partyRecord);
        btlBindActorModel(unit, resourceKind, resourceId);
        btlReleaseModelAssetByMode((u32)unit, resourceKind, resourceId);
        if (args->duration == 0) {
            dds3DrawFlag |= 0x2000000;
        }
        btlSetUnitPosition(unit, unit->currentPosition);
        btlSetUnitRotation(unit, unit->orientation);
        btlSetUnitColor(unit, unit->baseColor, 0);
        if (unit->status.stateFlags & 0x10) {
            u32 firstColor;
            u32 secondColor;

            SDF_VU0_LOADVECTOR(unit->colorStart);
            EE_MMI_RGBA_PACK_UNIT(packedStart[0], 128.0f);
            firstColor = packedStart[0];
            SDF_VU0_LOADVECTOR(unit->colorEnd);
            EE_MMI_RGBA_PACK_UNIT(packedEnd[0], 128.0f);
            secondColor = packedEnd[0];
            evtSetUnitLightAndAmbient(unit->ext, 0, firstColor, secondColor);
        }
        unit->gunResourceFlags = (unit->gunResourceFlags & ~1) | 2;
        if (args->phase == BTL_MODEL_CHANGE_CROSSFADE) {
            u32 baseRgb = unit->baseColor & 0xFFFFFF;

            unit->overlayColor = baseRgb;
            unit->mirror->overlayColor = baseRgb | 0x80000000;
            evtSetUnitColorA(unit->ext, 0, 0);
        }
        break;
    case BTL_MODEL_CHANGE_CROSSFADE:
        if (args->elapsed == 1 && args->duration != 0) {
            entryFlags = btlGetEntryFlagsUnlessDisabled(&unit->partyRecord);
            if (unit->status.flags & 0x20) {
                btlApplyScaledUnitEffectParameter(unit, 0xB,
                    btlGetSlotRateKind(unit, 0xB), 1.0f);
            } else if (args->transitionMode == 2) {
                unit->motionId = -1;
                btlApplyScaledUnitEffectParameter(unit, 0xE,
                    btlGetSlotRateKind(unit, 0xE), 1.0f);
            } else if (args->transitionMode != 3 &&
                       ((unit->status.flags & 0x200) || (entryFlags & 0x200)) &&
                       args->resourceId != 0x1F &&
                       !(unit->partyRecord.status & 0x2000)) {
                unit->motionId = -1;
                if (unit->status.flags & 0x1000) {
                    btlApplyScaledUnitEffectParameter(unit, 0x10,
                        btlGetSlotRateKind(unit, 0x10), 1.0f);
                } else {
                    btlApplyScaledUnitEffectParameter(unit, 0x11,
                        btlGetSlotRateKind(unit, 0x11), 1.0f);
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
                    evtSetUnitColorRGB(unit->mirror->ext, args->duration >> 2, 0x80000000);
                } else if ((u32)args->elapsed == (args->duration >> 2)) {
                    evtSetUnitColorA(unit->mirror->ext, args->elapsed, 0);
                    unit->mirror->status.flags |= 0x200000;
                    btlFlagUnitDefeatCandidate(unit);
                    alpha = unit->baseColor & 0xFF000000;
                    evtSetUnitColorRGB(unit->ext, 0, 0);
                    evtSetUnitColorA(unit->ext, args->duration >> 2, alpha);
                    unit->status.flags |= 0x100000;
                } else if ((u32)args->elapsed == (args->duration >> 1)) {
                    unit->overlayColor = unit->baseColor;
                    evtSetUnitColorRGB(unit->ext, args->duration >> 1, unit->baseColor);
                }
            }
        } else {
            if (args->transitionMode == 0) {
                unit->status.flags &= ~0x10000;
            }
            unit->overlayColor = unit->baseColor;
            unit->mirror->overlayColor = 0;
            args->phase = BTL_MODEL_CHANGE_CLEANUP;
        }
        break;
    case BTL_MODEL_CHANGE_CLEANUP:
        if (unit->mirror != NULL) {
            btlDestroyUnit(unit->mirror);
            unit->mirror = NULL;
            if (unit->transparencyBufferAllocation != 0) {
                sdfMemFreeDelayedDirect(unit->transparencyBufferAllocation);
                unit->transparencyBufferAllocation = 0;
                unit->transparencyBuffer = 0;
            }
        }
        btlBossDebugPrintf("btl:model change end[%X,%X]\n", resourceKind, resourceId);
        return 1;
    }
    args->elapsed++;
    return 0;
}

extern u32 btlUpdateModelChangeTask(BtlModelChangeArgs *);

BtlRuntimeTask *btlCreateModelChangeTask(BtlUnit *unit, s32 resourceKind, s32 resourceId, s32 delay, s32 duration, u8 transitionMode) {
    BtlRuntimeTask *task = btlAllocTask(0x1C);
    u8 *taskBytes = (u8 *)task;
    BtlModelChangeArgs *args;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u16 *)((u8 *)task + 0x20) = 0x1A;
    *(u16 *)((u8 *)task + 0x24) &= ~BTL_TASK_FLAG_REGISTERED;
    *(u64 *)((u8 *)task + 0x40) = *(u64 *)((u8 *)unit + 0x108);
    *(void **)((u8 *)task + 0x48) = btlBeginModelChange;
    *(void **)((u8 *)task + 0x4C) = btlUpdateModelChangeTask;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    args->resourceKind = resourceKind;
    args->resourceId = resourceId;
    args->delay = delay;
    args->duration = duration;
    args->transitionMode = transitionMode;
    args->phase = BTL_MODEL_CHANGE_WAITING_FOR_ASSET;
    args->elapsed = 0;
    return task;
}

void btlApplyLinkedUnitStatusWhenActorActive(BtlUnitLightAndAmbientTaskArgs *taskArgs) {
    if ((taskArgs->unit->status.flags & 2) != 0) {
        evtUnitLightOn(taskArgs->unit->ext);
        return;
    }
}

u32 btlApplyUnitFxWhenLoaded(BtlUnitLightAndAmbientTaskArgs *taskArgs) {
    if ((btlUnitStatusPair(taskArgs->unit) & 0x1000000002) == 0x1000000002) {
        evtSetUnitLightAndAmbient(taskArgs->unit->ext, taskArgs->duration, taskArgs->firstColor, taskArgs->secondColor);
    }
    return 1;
}

u8 *btlCreateUnitTask0F(BtlUnit *unit, s32 firstColor, s32 secondColor, s32 duration) {
    BtlRuntimeTask *task = btlAllocTask(16);
    u8 *taskBytes = (u8 *)task;
    BtlUnitLightAndAmbientTaskArgs *args;
    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)((u8 *)task + 0x20) = 0xF;
    *(u64 *)((u8 *)task + 0x40) = unit->identity;
    *(void **)((u8 *)task + 0x48) = btlApplyLinkedUnitStatusWhenActorActive;
    *(void **)((u8 *)task + 0x4C) = btlApplyUnitFxWhenLoaded;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    args->firstColor = firstColor;
    args->secondColor = secondColor;
    args->duration = duration;
    return (u8 *)task;
}

typedef struct BtlUnitLightDirectionTaskArgs {
    f32 direction[4];
    s32 directionMode;
    BtlUnit *unit;
} BtlUnitLightDirectionTaskArgs;

typedef char BtlUnitLightDirectionTaskArgs_size_check[
    sizeof(BtlUnitLightDirectionTaskArgs) == 0x18 ? 1 : -1];
typedef char BtlUnitLightDirectionTaskArgs_directionMode_offset_check[
    (u32)&((BtlUnitLightDirectionTaskArgs *)0)->directionMode == 0x10 ? 1 : -1];
typedef char BtlUnitLightDirectionTaskArgs_unit_offset_check[
    (u32)&((BtlUnitLightDirectionTaskArgs *)0)->unit == 0x14 ? 1 : -1];

void btlPrepareUnitStatusFxOnStart(BtlUnitLightDirectionTaskArgs *task) {
    if (((u32)task->unit->status.flags & 2) != 0) {
        evtUnitLightOn(task->unit->ext);
        return;
    }
}

s32 btlApplyUnitVectorFxWhenLoaded(BtlUnitLightDirectionTaskArgs *task) {
    if (((u32)task->unit->status.flags & 2) != 0) {
        SDF_VU0_LOADVECTOR(task->direction);
        evtSetUnitLightDir(task->unit->ext, task->directionMode);
    }
    return 1;
}

BtlRuntimeTask *btlCreateUnitTask10(BtlUnit *unit, f32 *direction, s32 directionMode) {
    BtlRuntimeTask *task = btlAllocTask(0x18);
    BtlUnitLightDirectionTaskArgs *args;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x10;
    task->ownerId = unit->identity;
    task->onStart = btlPrepareUnitStatusFxOnStart;
    task->callback = btlApplyUnitVectorFxWhenLoaded;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    args->directionMode = directionMode;
    SDF_VU0_COPYVECTOR(args->direction, direction);
    return task;
}

u32 btlUnitFadeInTask(BtlUnitColorFadeArgs *args) {
    u32 total = args->fadeIn + args->fadeOut;
    BtlUnit *unit = args->unit;
    if (total != 0) {
        if (args->count == 0) {
            args->color = (unit->overlayColor & 0xFFFFFF) | 0x80000000;
            btlFlagUnitDefeatCandidate(unit);
            mdlSetColor(unit->ext->owner, 0);
            evtSetUnitColorRGB(unit->ext, 0, 0);
            evtSetUnitColorA(unit->ext, 0, 0);
            evtSetUnitColorA(unit->ext, args->fadeIn, args->color);
            unit->status.flags |= 0x100000;
        }
        if (args->count == args->fadeIn - 1) {
            unit->overlayColor = args->color;
            evtSetUnitColorRGB(unit->ext, args->fadeOut, args->color);
        }
    } else {
        unit->overlayColor = args->color;
    }
    if (!(unit->status.flags & 0x100000)) {
        if (args->count >= total) {
            evtSetUnitColorRGB(unit->ext, 0, args->color);
            evtSetUnitColorA(unit->ext, 0, args->color);
            return 1;
        }
    }
    args->count++;
    return 0;
}

extern u32 btlUnitFadeInTask(BtlUnitColorFadeArgs *);

BtlRuntimeTask *btlCreateUnitFadeInTask(BtlUnit *owner, u32 value, u32 variant) {
    u8 *ownerBytes = (u8 *)owner;
    BtlRuntimeTask *task = btlAllocTask(20);
    u8 *taskBytes = (u8 *)task;
    BtlUnitColorFadeArgs *arguments;

    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)((u8 *)task + 0x4C) = btlUnitFadeInTask;
    *(u16 *)((u8 *)task + 0x20) = 0x11;
    *(u64 *)((u8 *)task + 0x40) = *(u64 *)(ownerBytes + 0x108);
    *(u32 *)((u8 *)task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = owner;
    arguments->color = 0x80808080;
    arguments->fadeIn = value;
    arguments->fadeOut = variant;
    arguments->count = 0;
    return task;
}

u32 btlUnitFadeOutTask(BtlUnitColorFadeArgs *args) {
    u32 total = args->fadeIn + args->fadeOut;
    BtlUnit *unit = args->unit;
    if (total != 0) {
        if (args->count == 0) {
            btlFlagUnitDefeatCandidate(unit);
            evtSetUnitColorRGB(unit->ext, args->fadeOut, 0x80000000);
            unit->status.flags |= 0x200000;
        }
        if (args->count == args->fadeOut - 1) {
            unit->overlayColor = 0x80000000;
            evtSetUnitColorRGB(unit->ext, 0, 0);
            evtSetUnitColorA(unit->ext, args->fadeIn, 0);
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

extern u32 btlUnitFadeOutTask(BtlUnitColorFadeArgs *);

BtlRuntimeTask *btlCreateUnitFadeOutTask(BtlUnit *owner, u32 value, u32 variant) {
    BtlRuntimeTask *task = btlAllocTask(16);
    u8 *taskBytes = (u8 *)task;
    BtlUnitColorFadeArgs *arguments;

    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)((u8 *)task + 0x4C) = btlUnitFadeOutTask;
    *(u16 *)((u8 *)task + 0x20) = 0x12;
    *(u64 *)((u8 *)task + 0x40) = *(u64 *)((u8 *)owner + 0x108);
    *(u32 *)((u8 *)task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = owner;
    arguments->fadeIn = value;
    arguments->fadeOut = variant;
    arguments->count = 0;
    return task;
}

u32 btlStepUnitDefeatFadeIn(BtlUnitFadeTaskArgs *taskArgs) {
    BtlUnit *unit = taskArgs->unit;
    u32 alpha;

    if (taskArgs->durationOrMode == -1) {
        unit->overlayColor = (unit->baseColor & 0xFFFFFF) | 0x80000000;
        evtSetUnitColorRGB(unit->ext, 0, unit->overlayColor);
        evtSetUnitColorA(unit->ext, 0, unit->overlayColor);
        btlClearUnitDefeatCandidate(unit);
        return 1;
    }
    if (taskArgs->elapsed == 0) {
        btlFlagUnitDefeatCandidate(unit);
    }
    if (taskArgs->durationOrMode == 0) {
        evtSetUnitColorRGB((struct evtUnitHandle_t *)unit->ext, 0, unit->overlayColor);
        evtSetUnitColorA((struct evtUnitHandle_t *)unit->ext, 0,
                                  (unit->overlayColor & 0xFFFFFF) | 0x80000000);
    }
    if (taskArgs->elapsed >= taskArgs->durationOrMode) {
        unit->status.flags &= ~0x20000;
        unit->overlayColor = (unit->baseColor & 0xFFFFFF) | 0x80000000;
        return 1;
    }
    unit->status.flags |= 0x20000;
    alpha = (u32)((f32)taskArgs->elapsed * 128.0f / (f32)taskArgs->durationOrMode);
    alpha <<= 24;
    unit->overlayColor = alpha | (unit->baseColor & 0xFFFFFF);
    taskArgs->elapsed++;
    return 0;
}

BtlRuntimeTask *btlCreateUnitDefeatFadeInTask(BtlUnit *owner, s32 value) {
    BtlRuntimeTask *task = (BtlRuntimeTask *)btlAllocTask(12);
    BtlUnitFadeTaskArgs *arguments;

    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x13;
    task->callback = btlStepUnitDefeatFadeIn;
    task->ownerId = owner->identity;
    task->onStart = 0;
    arguments = (BtlUnitFadeTaskArgs *)btlGetTaskArguments(task);
    arguments->unit = owner;
    arguments->durationOrMode = value;
    arguments->elapsed = 0;
    return task;
}

u32 btlStepUnitDefeatFadeOut(BtlUnitFadeTaskArgs *taskArgs) {
    BtlUnit *unit = taskArgs->unit;
    u32 alpha;

    if (taskArgs->elapsed == 0) {
        btlFlagUnitDefeatCandidate(unit);
    }
    if (taskArgs->elapsed >= taskArgs->durationOrMode) {
        unit->status.flags = (u32)unit->status.flags & (~0x20000);
        unit->overlayColor = unit->baseColor & 0xFFFFFF;
        return 1;
    }
    unit->status.flags = (u32)unit->status.flags | (0x20000);
    alpha = (u32)((1.0f - (f32)taskArgs->elapsed / (f32)taskArgs->durationOrMode) * 128.0f);
    alpha <<= 24;
    unit->overlayColor = alpha | (unit->baseColor & 0xFFFFFF);
    taskArgs->elapsed++;
    return 0;
}

/* Create the owner-bound defeat fade-out task with a zero elapsed count. */
BtlRuntimeTask *btlCreateUnitDefeatFadeOutTask(BtlUnit *unit, s32 durationOrMode) {
    u8 *unitBytes = (u8 *)unit;
    BtlRuntimeTask *task = btlAllocTask(12);
    u8 *taskBytes = (u8 *)task;
    BtlUnitFadeTaskArgs *arguments;

    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)((u8 *)task + 0x20) = 0x14;
    *(void **)((u8 *)task + 0x4C) = btlStepUnitDefeatFadeOut;
    *(u64 *)((u8 *)task + 0x40) = *(u64 *)(unitBytes + 0x108);
    *(u32 *)((u8 *)task + 0x48) = 0;
    arguments = (BtlUnitFadeTaskArgs *)btlGetTaskArguments(task);
    arguments->unit = unit;
    arguments->durationOrMode = durationOrMode;
    arguments->elapsed = 0;
    return task;
}

/* Arguments hold unit, mode and elapsed ticks; return one after the exit fade. */
u32 btlStepUnitExitFade(BtlUnitFadeTaskArgs *taskArgs) {
    BtlUnit *unit = taskArgs->unit;
    s32 finished = 0;
    u32 alpha;

    if (taskArgs->elapsed == 0) {
        unit->status.flags = (u32)unit->status.flags | (0x80);
        btlFlagUnitDefeatCandidate(unit);
    }

    switch (taskArgs->durationOrMode) {
    case 0:
        if (taskArgs->elapsed == 1) {
            btlFlagUnitDefeatCandidate(unit);
            evtSetUnitColorRGB(unit->ext,
                                    10, 0x80000000);
        } else if (taskArgs->elapsed == 10) {
            evtSetUnitColorRGB(unit->ext,
                                    0, 0x80000000);
            evtSetUnitColorA(unit->ext, 6, 0);
            unit->status.flags = (u32)unit->status.flags | (0x200000);
        }
        if (((u32)unit->status.flags & 0x200000) == 0 &&
            taskArgs->elapsed >= 16) {
            finished = 1;
        }
        break;

    case 1:
        if (taskArgs->elapsed >= 8) {
            unit->status.flags = (u32)unit->status.flags & (~0x20000);
            unit->overlayColor = unit->baseColor & 0xFFFFFF;
            finished = 1;
        } else {
            unit->status.flags = (u32)unit->status.flags | (0x20000);
            alpha = (u32)((1.0f - (f32)taskArgs->elapsed * 0.125f) * 128.0f);
            alpha <<= 24;
            unit->overlayColor = alpha | (unit->baseColor & 0xFFFFFF);
        }
        break;
    }

    taskArgs->elapsed++;
    if (finished != 0) {
        unit->status.flags = ((u32)unit->status.flags & ~0x80) | 0x40;
        return 1;
    }
    return 0;
}

/* Queue the selected exit-fade mode for this owner, starting at tick zero. */
BtlRuntimeTask *btlCreateUnitExitFadeTask(BtlUnit *owner, s32 value) {
    BtlRuntimeTask *task = (BtlRuntimeTask *)btlAllocTask(12);
    BtlUnitFadeTaskArgs *arguments;

    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x15;
    task->callback = btlStepUnitExitFade;
    task->ownerId = owner->identity;
    task->onStart = 0;
    arguments = (BtlUnitFadeTaskArgs *)btlGetTaskArguments(task);
    arguments->unit = owner;
    arguments->durationOrMode = value;
    arguments->elapsed = 0;
    return task;
}

extern void effBattleUpdate(effBTLCutHandle_t *);

/* Start from the selected-unit SYSEFF source, then update through its duration. */
BtlSelectedUnitEffectTaskResult btlUpdateSelectedUnitEffect(BtlSelectedUnitEffectTaskArgs *arguments) {
    BtlUnit *unit = arguments->unit;
    SoundResourceNode *work;
    SoundMixer *handle;

    if (!(unit->status.flags & 2)) {
        return BTL_SELECTED_UNIT_EFFECT_TASK_COMPLETE;
    }
    {
        BtlState *battle = (BtlState *)btlGetRuntime();
        if (arguments->effect == 0) {
            work = battle->resources[BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT];
            handle = work->sourceHandle;
            unit->status.flags |= 0x80;
            arguments->mixer = sndMixerClone(handle);
            arguments->effect = effCreateBattleVoiceOwner(arguments->mixer, 2, unit, 0);
            arguments->duration = 0xE;
            arguments->effect->flags &= 0xFFF9;
            effBTLCutSetDrawEndFrame(arguments->effect, 0xE);
            unit->status.flags &= ~8;
            if (unit->status.flags & 2) {
                unit->ext->owner->flags |= MDL_SKIP_TRANSFORMS;
            }
        }
        arguments->counter = arguments->counter + 1;
        if (arguments->counter >= arguments->duration) {
            unit->status.flags = (unit->status.flags & ~0x80) | 0x40;
            return BTL_SELECTED_UNIT_EFFECT_TASK_COMPLETE;
        }
        effBattleUpdate(arguments->effect);
        return BTL_SELECTED_UNIT_EFFECT_TASK_CONTINUE;
    }
}

void btlFinishSelectedUnitEffect(BtlSelectedUnitEffectTaskArgs *arguments) {
    effBTLCutHandle_t *voice = arguments->effect;
    if (voice != 0) {
        effBTLCutDelete(voice);
    }
    if (arguments->mixer != 0) {
        sndMixerDestroy(arguments->mixer);
    }
    btlClearUnitDefeatCandidate(arguments->unit);
    arguments->unit->status.flags |= 0x40;
}

BtlRuntimeTask *btlCreateSelectedEffectUpdateTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(20);
    u8 *taskBytes = (u8 *)task;
    BtlSelectedUnitEffectTaskArgs *arguments;

    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u16 *)((u8 *)task + 0x20) = 0x16;
    *(u16 *)((u8 *)task + 0x24) |= BTL_TASK_FLAG_DEFERRED;
    *(u64 *)((u8 *)task + 0x40) = unit->identity;
    *(void **)((u8 *)task + 0x4C) = btlUpdateSelectedUnitEffect;
    *(void **)((u8 *)task + 0x50) = btlFinishSelectedUnitEffect;
    *(u32 *)((u8 *)task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = unit;
    arguments->effect = 0;
    arguments->counter = 0;
    arguments->duration = 0;
    return task;
}

extern void btlUpdateUnitActors(void);

u32 btlUpdateCommandSoundTask(void) {
    btlUpdateUnitActors();
    return 1;
}

BtlRuntimeTask *btlCreateCommandSoundUpdateTask(void) {
    BtlRuntimeTask *task = (BtlRuntimeTask *)btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlUpdateCommandSoundTask;
    task->taskId = 0x1B;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

extern void btlRefreshUnitEffects(void);

u32 btlUpdateCommandSoundTaskSecondary(void) {
    btlRefreshUnitEffects();
    return 1;
}

BtlRuntimeTask *btlCreateSecondaryCommandSoundTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    u8 *taskBytes = (u8 *)task;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)((u8 *)task + 0x20) = 0x1C;
    *(u16 *)((u8 *)task + 0x24) |= BTL_TASK_FLAG_DEFERRED;
    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u32 *)((u8 *)task + 0x48) = 0;
    *(void **)((u8 *)task + 0x4C) = btlUpdateCommandSoundTaskSecondary;
    return task;
}

/* Complete the empty task on its first process call; no sound operation is performed. */
u32 btlFinishNoOpTask(void) {
    return 1;
}

/* Allocate the zero-argument, kind-0x20 task with an unconditional completion callback. */
BtlRuntimeTask *btlCreateNoOpTask(void) {
    BtlRuntimeTask *task = (BtlRuntimeTask *)btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlFinishNoOpTask;
    task->taskId = 0x20;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

extern f32 *D_00324770[];

extern u8 kwlnDefaultColorVector[];

u32 btlUnitBaseLightTask(BtlUnitBaseLightArgs *work) {
    BtlUnit *unit = work->unit;
    evtUnitHandle_t *ext;
    EvtTargetInfo *info;
    dds3Basic_t *target;
    if (unit->status.flags & 2) {
        if (work->delay >= 2) {
            ext = unit->ext;
            evtUnitLightOn(ext);
            target = ext->currentTransitionTarget;
            if (target != NULL && (ext->flags & 0x40000)) {
                info = target->data;
                SDF_VU0_COPYVECTOR(unit->colorStart, info->firstColor);
                SDF_VU0_COPYVECTOR(unit->colorEnd, info->secondColor);
                SDF_VU0_COPYVECTOR(unit->lightDirection, info->direction);
            } else {
                f32 *defaultLight = D_00324770[0];
                SDF_VU0_COPYVECTOR(unit->colorStart, defaultLight);
                SDF_VU0_COPYVECTOR(unit->colorEnd, kwlnDefaultColorVector);
                SDF_VU0_COPYVECTOR(unit->lightDirection, defaultLight + 4);
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
        node = mdlGetMotionNum(task->unit->ext->owner, 0);
        if (node < 0x1D) {
            BtlActorStatusRecord *resource = (BtlActorStatusRecord *)btlGetSideIndexedActorStatusTable(task->unit->resourceKind,
                                                 task->unit->species);
            if (resource->motions[node].endBehavior == BTL_MOTION_END_OUT) {
                btlRefreshUnitEffectMotionAndEntry(task->unit);
                btlBossDebugPrintf(D_003A3CA0);
            }
        }
    }
    if (0.5f < task->amplitude) {
        BtlUnit *actor;
        scale = task->amplitude * (dds3GetRandFloatA(effSharedRandomState) * 0.5f + 0.5f);
        if (task->tick & 1) {
            scale = -scale;
        }
        actor = task->unit;
        if (btlUnitStatusPair(actor) & 0x808000000000) {
            dds3GetPosPoseSet(actor->effectObject);
            SDF_VU0_STOREVECTOR_NOMEM(pos);
            pos[0] += scale;
        } else {
            btlCopyUnitCurrentPosition(actor, pos);
            pos[0] += scale;
            pos[2] += task->unit->positionZOffset;
        }
        dds3SetPosPoseSet(task->unit->effectObject, (u128 *)pos);
        task->amplitude *= 0.85f;
    } else {
        BtlUnit *actor = task->unit;
        if (btlUnitStatusPair(actor) & 0x808000000000) {
            dds3GetPosPoseSet(actor->effectObject);
            SDF_VU0_STOREVECTOR_NOMEM(pos);
        } else {
            btlCopyUnitCurrentPosition(actor, pos);
            pos[2] += task->unit->positionZOffset;
        }
        dds3SetPosPoseSet(task->unit->effectObject, (u128 *)pos);
        return 1;
    }
    task->tick += 1;
    return 0;
}

extern u32 btlStiffenDamageShakeStep(BtlDamageShakeArgs *);

BtlRuntimeTask *btlCreateStiffenDamageShakeTask(BtlUnit *owner, f32 value) {
    BtlRuntimeTask *task = (BtlRuntimeTask *)btlAllocTask(sizeof(BtlDamageShakeArgs));
    BtlDamageShakeArgs *arguments;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x1D;
    task->callback = btlStiffenDamageShakeStep;
    task->ownerId = owner->identity;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = owner;
    arguments->amplitude = value;
    arguments->tick = 0;
    return task;
}

/* Request the position-effect motion or advance its fallback position curve. */
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
        if (runtime->allowPositionEffect != NULL) {
            approved = runtime->allowPositionEffect(unit);
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
    amplitude = unit->bodyRadius * unit->scale * 0.8f;
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
            btlCopyUnitCurrentPosition(task->unit, position);
            position[2] += task->unit->positionZOffset;
            dds3SetPosPoseSet(task->unit->effectObject, (u128 *)position);
            return 1;
        }
    }
    btlCopyUnitCurrentPosition(task->unit, position);
    position[0] += offset;
    position[2] += task->unit->positionZOffset;
    dds3SetPosPoseSet(task->unit->effectObject, (u128 *)position);
    task->tick++;
    return 0;
}

extern u32 btlAdvanceActorPositionEffectTask(BtlPositionEffectArgs *);

/* Queue the actor's position effect with its tick counter reset. */
BtlRuntimeTask *btlCreateActorPositionEffectTask(BtlUnit *unit) {
    BtlRuntimeTask *task = (BtlRuntimeTask *)btlAllocTask(0x10);
    BtlPositionEffectArgs *args;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlAdvanceActorPositionEffectTask;
    task->taskId = 0x1E;
    task->ownerId = unit->identity;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    args->tick = 0;
    return task;
}

/* Apply the queued actor's stored orientation and complete immediately. */
u32 btlApplyQueuedActorRotation(BtlRuntimeTaskArgs *taskArgs) {
    BtlUnit *unit;

    unit = taskArgs->actor;
    unit->status.flags = (u32)unit->status.flags & 0xffffffef;
    btlSetUnitRotation(unit, unit->rotation);
    return 1;
}

BtlRuntimeTask *btlScheduleActorUpdate(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    BtlRuntimeTaskArgs *arguments;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlApplyQueuedActorRotation;
    task->taskId = 0x1F;
    task->ownerId = unit->identity;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    arguments->actor = unit;
    return task;
}

u32 btlRefreshUnitFxVectorTask(BtlUnit **taskArgs) {
    btlRefreshUnitFxVectors(*taskArgs);
    return 1;
}

BtlRuntimeTask *btlCreateUnitFxVectorRefreshTask(BtlUnit *owner) {
    BtlRuntimeTask *task = btlAllocTask(4);
    u8 *taskBytes = (u8 *)task;
    u8 *ownerBytes = (u8 *)owner;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)((u8 *)task + 0x4C) = btlRefreshUnitFxVectorTask;
    *(u16 *)((u8 *)task + 0x20) = 0x22;
    *(s64 *)((u8 *)task + 0x40) = *(s64 *)(ownerBytes + 0x108);
    *(u32 *)((u8 *)task + 0x48) = 0;
    *(BtlUnit **)btlGetTaskArguments(task) = owner;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3CA0);

typedef struct GunLoadArgs {
    BtlUnit *unit;
    struct FileRequest *request;
} GunLoadArgs;

void btlStartGunFinishLoad(GunLoadArgs *taskArgs) {
    char filename[0x70];
    BtlUnit *actor = taskArgs->unit;
    if (((u32)actor->status.flags & 0x400) != 0) {
        return;
    }
    if (actor->gunResource != 0) {
        sdfFreeAuto(actor->gunResource);
        actor->gunResource = 0;
    }
    if (btlFormatUnitBedName(actor, filename)) {
        taskArgs->request = fileLoadUseTail(filename);
        btlBossDebugPrintf("btl:gun & finish load start[%s][%p]\n", filename, taskArgs->request);
    }
    actor->gunResourceFlags = (actor->gunResourceFlags | 4) & ~8;
}

u32 btlPollGunLoad(GunLoadArgs *loadArgs) {
    BtlUnit *unit = loadArgs->unit;
    if (loadArgs->request == NULL) {
        return 1;
    }
    if (pacFileCheckEnd(loadArgs->request) == 0) {
        return 0;
    }
    btlBossDebugPrintf("btl:gun & finish load end[%p]\n", loadArgs->request);
    unit->gunResource = (void *)sdfMemLock(fileGetMemHandle(loadArgs->request));
    fileRequestFreeIfComplete(loadArgs->request);
    unit->gunResourceFlags = (unit->gunResourceFlags & ~4) | 8;
    return 1;
}

extern void btlStartGunFinishLoad(GunLoadArgs *);

extern u32 btlPollGunLoad(GunLoadArgs *);

BtlRuntimeTask *btlCreateGunLoadPollTask(BtlUnit *owner) {
    BtlRuntimeTask *task = btlAllocTask(8);
    u8 *taskBytes = (u8 *)task;
    u8 *ownerBytes = (u8 *)owner;
    GunLoadArgs *arguments;

    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u16 *)((u8 *)task + 0x20) = 0x23;
    *(u16 *)((u8 *)task + 0x24) &= ~BTL_TASK_FLAG_REGISTERED;
    *(u64 *)((u8 *)task + 0x40) = *(u64 *)(ownerBytes + 0x108);
    *(void **)((u8 *)task + 0x48) = btlStartGunFinishLoad;
    *(void **)((u8 *)task + 0x4C) = btlPollGunLoad;
    arguments = btlGetTaskArguments(task);
    arguments->unit = owner;
    arguments->request = NULL;
    return task;
}

u32 btlUpdateUnitEffectsTask(void) {
    btlUpdateUnitEffects();
    return 1;
}

BtlRuntimeTask *btlCreateUpdateUnitEffectsTask(void) {
    BtlRuntimeTask *task = (BtlRuntimeTask *)btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlUpdateUnitEffectsTask;
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

BtlRuntimeTask *btlCreateActorTransparencyTask(BtlUnit *actor) {
    BtlRuntimeTask *task = btlAllocTask(4);
    u8 *taskBytes = (u8 *)task;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)((u8 *)task + 0x20) = 0x25;
    *(void **)((u8 *)task + 0x4C) = btlCreateActorTransparency;
    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u32 *)((u8 *)task + 0x48) = 0;
    ((BtlActorTransparencyArgs *)btlGetTaskArguments(task))->unit = actor;
    return (u8 *)task;
}

/* Advance the queued model-motion blend in two stages; return one when done. */
s32 btlAdvanceActorModelBlendTask(BtlActorModelBlendArgs *args) {
    BtlUnit *unit;
    BtlUnit *target;

    if (args->index < 0) {
        return 1;
    }
    unit = args->unit;
    target = args->target;
    if (args->stage == 0) {
        if (unit->status.flags & 2) {
            args->previousMotionIndex = mdlGetMotionNum(unit->ext->owner, 0);
        } else {
            args->previousMotionIndex = unit->motionId;
        }
        unit->motionStartFrame = 0;
        unit->motionEndFrame = 1;
        btlApplyScaledUnitEffectParameter(unit, args->index, args->value, args->scale);
    } else {
        if (unit != target) {
            btlWarpUnitToMotionReach(unit, target, args->previousMotionIndex);
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
    task->callback = btlAdvanceActorModelBlendTask;
    task->taskId = 0x26;
    task->ownerId = actor->identity;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = actor;
    arguments->target = target;
    arguments->index = index;
    arguments->value = value;
    arguments->scale = scale;
    arguments->previousMotionIndex = -1;
    arguments->stage = 0;
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
            if (unit->motionId != parameter && parameter != -1) {
                btlApplyScaledUnitEffectParameter(unit, parameter, 0, 1.0f);
            } else {
                frame = (s32)btlGetUnitModelCurrentFrame(unit);
                if (frame >= status->model) {
                    sdfMotionSeek(unit->ext->owner->first, (f32)status->model);
                    btlResetUnitModelProgress(unit);
                    unit->status.flags &= ~0x80000000;
                }
            }
        }
        if (unit->updateFlags & 2) {
            if (unit->updateFlags & 4) {
                frame = (s32)btlGetUnitModelCurrentFrame(unit);
                index = unit->motionId;
                if (index == mdlGetMotionNum(unit->ext->owner, 0)) {
                    if (frame >= status->motions[index].alphaStartFrame) {
                        evtSetUnitColorA(unit->ext,
                            (s32)((f32)status->motions[index].alphaDuration /
                                (status->motions[index].alphaFrameScale * runtime->modelFrameScale)),
                            unit->overlayColor & 0xFFFFFF);
                        unit->updateFlags &= ~4;
                    }
                }
            }
            unit->overlayColor = mdlGetColor(unit->ext->owner);
        }
        if (unit->status.flags & 0x8000) {
            pulse = (f32)(runtime->frame % 30) / 15.0f;
            if (pulse > 1.0f) {
                pulse = 2.0f - pulse;
            }
            SDF_VU0_SETSCALE1VECTOR();
            VU0_SCALAR_OP(pulse * 1.6f + 0.3f, "vmulx.xyzw vf10, vf10, vf2x");
            EE_MMI_RGBA_PACK(packed[0]);
            btlBlendUnitColor(unit, (packed[0] & 0xFFFFFF) | 0x80000000, 0);
        }
    }
}

extern void btlDrawUnitStatusShadowDisc(BtlUnit *unit);

void btlUpdateActorModelColorAndLinks(void) {
    BtlUnit *unit = ((BtlActorWork *)btlGetRuntime())->actorList;
    s32 color;

    for (; unit != 0; unit = unit->next) {
        if (unit->status.flags & 2) {
            unit->motionId = mdlGetMotionNum(unit->ext->owner, 0);
            if (!(unit->status.flags & 0x40000)) {
                if (unit->status.flags & 0x100000) {
                    color = mdlGetColor(unit->ext->owner);
                    unit->overlayColor = color;
                    if ((color & 0xFF000000) == 0x80000000) {
                        btlFlagUnitDefeatCandidate(unit);
                        unit->status.flags &= ~0x100000;
                    }
                } else if (unit->status.flags & 0x200000) {
                    color = mdlGetColor(unit->ext->owner);
                    unit->overlayColor = color;
                    if ((color & 0xFF000000) == 0) {
                        if (!(unit->status.flags & 0xC0)) {
                            btlClearUnitDefeatCandidate(unit);
                        }
                        unit->status.flags &= ~0x200000;
                    }
                }
                if (unit->status.flags & 0x10000) {
                    btlDrawUnitMirrorTransparency(unit);
                } else {
                    btlUpdateUnitTransparency(unit);
                }
            }
            func_001F2818(unit->resourceLink);
            btlUpdateUnitCommandEffect(unit->link);
            btlDrawUnitStatusShadowDisc(unit);
        }
    }
}

void btlResetUnitLinks(BtlUnit *actor) {
    actor->selectedEntryIndex = -1;
    actor->pendingSkillNameIndex = BTL_SKILL_NAME_INDEX_NONE;
    actor->status.flags = 0;
    actor->status.stateFlags = 0;
    actor->gunResourceFlags = 0;
    actor->effectLink.flags = 0;
    btlClearAllActorEntrySlots(actor);
    actor->resourceLink = sndAllocResourceLink(actor);
    actor->link = sndAllocLink(actor);
}


BtlUnit *btlCreateUnit(void) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(0x348);
    BtlUnit *unit = (BtlUnit *)sdfMemLock(allocation);
    BtlActorWork *work;
    memset(unit, 0, 0x348);
    unit->handle = allocation;
    unit->identity = btlAdvanceRuntimeSequenceCounter();
    unit->status.flags = 0;
    unit->status.stateFlags = 0;
    unit->lookupId = unit->selectedEntryIndex = -1;
    unit->partyIndex = 6;
    unit->gunResourceFlags = 0;
    unit->effectLink.referenceCount = 0;
    unit->resourceNode = 0;
    unit->gunResource = 0;
    unit->effectObject = 0;
    unit->ext = 0;
    btlInitUnitFxDefaults(unit);
    btlInitFxLights(unit);
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
        sdfFreeAuto(unit->gunResource);
        unit->gunResource = 0;
        unit->gunResourceFlags &= ~4;
        unit->gunResourceFlags &= ~8;
    }
    if (unit->listNode != 0) {
        sndFreeListNode(unit->listNode);
        unit->listNode = 0;
    }
    btlReleaseActorModelResources(unit);
    if (unit->transparencyBufferAllocation != 0) {
        sdfMemFreeDelayedDirect(unit->transparencyBufferAllocation);
        unit->transparencyBufferAllocation = 0;
        unit->transparencyBuffer = 0;
    }
}

extern char D_003A3D38[]; /* "btl:unit delete[%p]\n" */

void btlDestroyUnit(BtlUnit *unit) {
    btlBossDebugPrintf(D_003A3D38, unit);
    btlReleaseUnitResources(unit);
    if (unit->next != 0) {
        unit->next->previousActor = unit->previousActor;
    }
    if (unit->previousActor != 0) {
        unit->previousActor->next = unit->next;
    } else {
        ((BtlActorWork *)btlGetRuntime())->actorList = unit->next;
    }
    sdfMemFreeDirect(unit->handle);
}

void btlDestroyAllUnits(void) {
    BtlUnit *unit;
    BtlUnit *next;
    for (unit = ((BtlActorWork *)btlGetRuntime())->actorList; unit != 0; unit = next) {
        next = unit->next;
        btlDestroyUnit(unit);
    }
}

void btlRemoveActorsWithFlags(u32 mask) {
    BtlUnit *actor = ((BtlActorWork *)btlGetRuntime())->actorList;
    while (actor != 0) {
        BtlUnit *next = actor->next;
        if ((u32)actor->status.flags & mask) {
            btlDestroyUnit(actor);
        }
        actor = next;
    }
}

BtlUnit *btlFindActorForOwner(BtlUnitIdentity identity) {
    BtlState *context = (BtlState *)btlGetRuntime();
    BtlUnit *actor = context->units;
    while (actor != 0) {
        if (actor->identity == identity) {
            return actor;
        }
        actor = actor->next;
    }
    return 0;
}

s32 btlIsUnitInActorList(const BtlUnit *candidate) {
    BtlState *context = (BtlState *)btlGetRuntime();
    BtlUnit *actor = context->units;
    while (actor != 0) {
        if (actor == candidate) {
            return 1;
        }
        actor = actor->next;
    }
    return 0;
}

BtlUnit *btlFindUnitByModeClear(s32 unitId) {
    BtlUnit *unit;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->next) {
        if (!(unit->partyRecord.flags & 0x20) && unit->partyRecord.unitId == unitId) {
            return unit;
        }
    }
    return 0;
}

BtlUnit *btlFindUnitByModeFlagged(s32 unitId) {
    BtlUnit *unit;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->next) {
        if ((unit->partyRecord.flags & 0x20) && unit->partyRecord.unitId == unitId) {
            return unit;
        }
    }
    return 0;
}

/* Allocate a native header followed by capacity pointer entries. */
BtlIndexList *btlAllocateIndexList(s32 capacity) {
    BtlIndexList *list = sdfAllocAndClearQuadwords(capacity * 4 + 12);
    list->capacity = capacity;
    list->entries = (struct BtlUnit **)(list + 1);
    list->count = 0;
    return list;
}

void btlFreeIndexList(BtlIndexList *list) {
    sdfChipFreeDirect(list);
}

/* The caller keeps the live count within the allocated capacity. */
void btlAppendIndexListEntry(BtlIndexList *list, BtlUnit *entry) {
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
BtlUnit *btlGetIndexListEntry(BtlIndexList *list, s32 index) {
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
    BtlUnit **entries;
    BtlUnit *first;
    BtlUnit *second;

    if (firstIndex == secondIndex) {
        return;
    }
    entries = list->entries;
    first = entries[firstIndex];
    second = entries[secondIndex];
    entries[firstIndex] = second;
    entries[secondIndex] = first;
}

BtlIndexListEntryIndex btlFindListIndex(BtlIndexList *list, BtlUnit *entry) {
    u32 count = btlGetIndexListCount(list);
    u32 index;

    for (index = 0; index < count; index++) {
        if (entry == btlGetIndexListEntry(list, index)) {
            return index;
        }
    }
    return BTL_INDEX_LIST_ENTRY_NOT_FOUND;
}

extern void mdlSetEffectScale(dds3ModelHandle_t *, f32);

void btlApplyUnitEffectScale(BtlUnit *unit) {
    dds3PoseSet_t *inner;
    if (unit->status.flags & 2) {
        btlInitializeEffectVectorsFromSourceRecords(unit, unit->resourceKind, unit->species);
        SDF_VU0_SETSCALE1VECTOR();
        VU0_SCALAR_OP(unit->actorScale, "vmulx.xyzw vf10, vf10, vf2x");
        inner = unit->effectObject->inner;
        inner->flags |= OBJECT_TRANSFORM_FLAG_UPDATE_PENDING;
        inner->flags &= ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
        SDF_VU0_STOREVECTOR(inner->scale);
        mdlSetScale(unit->ext->owner);
        mdlSetEffectScale(unit->ext->owner, unit->actorScale);
        btlSetUnitPosition(unit, unit->currentPosition);
    }
}

#include "btl_camera_mode_definition.h"

extern BtlCameraModeDefinition btlCameraModeDefinitions[];

extern void mdlPropagateObjectFlag(void);
extern void mdlSetListedObjectFlag(void);

void btlSetCameraCommandMode(BtlCameraModeIndex cameraMode, BtlTask *link, BtlUnit *valueA,
                             BtlUnit *valueB, BtlActionCode actionCode) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    BtlCameraModeDefinition *definition;
    BtlCameraModeIndex previousCameraMode = scene->cameraCommand.status;
    u32 flags;
    s32 accepted;
    if (previousCameraMode == cameraMode && btlCameraModeDefinitions[cameraMode].allowRepeat == 0) {
        return;
    }
    if (cameraMode == 6 && (scene->cameraCommand.flags & 0x800)) {
        return;
    }
    flags = scene->cameraCommand.flags;
    scene->cameraCommand.linkedA = valueA;
    scene->cameraCommand.linkedB = valueB;
    scene->cameraCommand.actionKind = previousCameraMode;
    scene->cameraCommand.actionCode = actionCode;
    scene->cameraCommand.task = link;
    scene->cameraCommand.selectedUnit = NULL;
    scene->cameraCommand.status = cameraMode;
    if (cameraMode != 10 || !(flags & 0x1000)) {
        scene->cameraCommand.state = 0;
        scene->cameraCommand.flags = 0;
    }
    scene->cameraCommand.camera.fov = 0.6981317f; /* 40 degrees. */
    if (cameraMode - 9 < 2) {
        scene->cameraCommand.flags |= flags & 0x40000;
    }
    if (link != NULL && link->indexWork.indices != NULL) {
        btlCopyIndexList(scene->cameraCommand.targetList, link->indexWork.indices);
    } else {
        btlClearIndexList(scene->cameraCommand.targetList);
    }
    definition = &btlCameraModeDefinitions[cameraMode];
    if (definition->initialize != NULL) {
        accepted = 1;
        if (scene->cameraModeInitializePredicate != NULL) {
            accepted = scene->cameraModeInitializePredicate(&scene->cameraCommand);
        }
        if (accepted != 0) {
            definition->initialize(&scene->cameraCommand);
        }
    }
    switch (cameraMode) {
    case 6:
    case 8:
    case 11:
    case 14:
        break;
    default:
        mdlSetListedObjectFlag();
        mdlPropagateObjectFlag();
        break;
    }
    scene->cameraCommand.flags &= ~0x400;
    scene->cameraCommand.flags &= ~0x2000;
}

void btlNormalizeActionCameraKeyScales(BtlLinkedCommand *action) {
    f32 *key = (f32 *)action;
    u32 flags = action->flags;
    u32 i;
    if (flags & 2) {
        for (i = 0; i < 4; i++, key += 12) {
            f32 *pos = key + 12;
            SDF_VU0_LOADVECTOR(key + 16);
            SDF_VU0_NEGVECTOR_XYZ();
            SDF_VU0_LOADVECTOR_B(pos);
            VU0_SCALAR_OP(key[20] - 1.0f, "vmulx.xyzw vf10, vf10, vf2x");
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(pos);
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

void btlInterpolateVectorStep(BtlCamState *camera) {
    f32 vec[4];
    f32 step = -camera->distance;
    vec[3] = 0.0f;
    vec[0] = camera->direction[0] * step + camera->position[0];
    vec[1] = camera->direction[1] * step + camera->position[1];
    vec[2] = camera->direction[2] * step + camera->position[2];
    SDF_VU0_LOADVECTOR_BARRIER(vec);
}

/* Complete native camera flag-0x08 mode without changing command state. */
u32 btlCompleteCameraFlag08Mode(BtlLinkedCommand *command) {
    return 1;
}

/* Complete native camera flag-0x20 mode without changing command state. */
u32 btlCompleteCameraFlag20Mode(BtlLinkedCommand *command) {
    return 1;
}

/* Complete native camera flag-0x80 mode without changing command state. */
u32 btlCompleteCameraFlag80Mode(BtlLinkedCommand *command) {
    return 1;
}

/* Blend position, direction, distance and FOV; both vector W lanes become zero. */
void btlBlendActionCameraState(BtlCamState *dst, BtlCamState *current, BtlCamState *target, f32 blend) {
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

extern void btlBlendActionCameraState(BtlCamState *, BtlCamState *, BtlCamState *, f32);

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
    btlBlendActionCameraState(&command->camera, &command->frontCamera,
                  &command->backCamera, 2.0f * blend);
    command->progress = blend;
    if (blend >= 0.5f) {
        return 1;
    }
    return 0;
}

extern void btlBlendActionCameraState(BtlCamState *, BtlCamState *, BtlCamState *, f32);

s32 btlStepPoseBlend(BtlLinkedCommand *command) {
    BtlExponentialRange *motion = &command->exponentialRange;
    BtlCamState *from = &command->frontCamera;
    f32 value;
    if (command->state == 0) {
        btlScalarRangeSetStartClearEnd(motion, command->motionParameter);
        btlCopyMotionTransform(&command->camera, from);
    }
    value = btlScalarRangeStepExponential(motion);
    btlBlendActionCameraState(&command->camera, from,
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
    btlBlendActionCameraState(&command->camera, &command->frontCamera,
                  &command->backCamera, value);
    command->progress = value;
    return 0.9999990f <= value;
}

s32 btlStepPoseBlendRatio(BtlLinkedCommand *command) {
    f32 ratio = (f32)command->state / (f32)command->durationFrames;
    if (ratio <= 1.0f) {
        btlBlendActionCameraState(&command->camera, &command->frontCamera,
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
            SDF_VU0_LOADVECTOR(state->position);
            VU0_SET_VF10_COMPONENT(y, 0.0f);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(vector);
            VU0_SET_VF10_COMPONENT(y, 0.0f);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(direction);
            delta = y - (-20.0f);
            scale = fsqrtf(delta * delta + length * length);
            vector[0] = -direction[0] * scale;
            vector[1] = delta;
            vector[2] = -direction[2] * scale;
            SDF_VU0_LOADVECTOR(vector);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(state->direction);
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
            SDF_VU0_LOADVECTOR(state->position);
            VU0_SET_VF10_COMPONENT(y, 0.0f);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(vector);
            VU0_SET_VF10_COMPONENT(y, 0.0f);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(direction);
            delta = y - height;
            scale = fsqrtf(delta * delta + length * length);
            vector[0] = -direction[0] * scale;
            vector[1] = delta;
            vector[2] = -direction[2] * scale;
            SDF_VU0_LOADVECTOR(vector);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(state->direction);
            changed = 1;
        }
    }
    return changed;
}

u32 btlExecuteCommandSoundTask(BtlCameraModeTaskArgs *arguments) {
    btlSetCameraCommandMode(arguments->cameraMode, arguments->actor, arguments->unitA, arguments->unitB, arguments->actionCode);
    return 1;
}

BtlRuntimeTask *btlCreateCommandSoundTask(BtlTask *actor, BtlCameraModeIndex cameraMode) {
    BtlRuntimeTask *task = (BtlRuntimeTask *)btlAllocTask(20);
    BtlCameraModeTaskArgs *arguments;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x27;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    if (actor != NULL && actor->unit != NULL) {
        task->ownerId = actor->unit->identity;
    }
    task->callback = btlExecuteCommandSoundTask;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    arguments->actor = actor;
    arguments->cameraMode = cameraMode;
    arguments->unitA = NULL;
    arguments->unitB = NULL;
    arguments->actionCode = 0;
    return task;
}

BtlRuntimeTask *btlCreateTargetedCommandSoundTask(BtlTask *actor, BtlCameraModeIndex cameraMode, BtlActionCode actionCode) {
    BtlRuntimeTask *task = btlCreateCommandSoundTask(actor, cameraMode);
    BtlCameraModeTaskArgs *arguments = btlGetTaskArguments(task);
    arguments->actionCode = actionCode;
    return task;
}

BtlRuntimeTask *btlCreateCommandSoundWithArguments(BtlTask *actor, BtlUnit *unitA, BtlUnit *unitB, BtlCameraModeIndex cameraMode, BtlActionCode actionCode) {
    BtlRuntimeTask *task = btlCreateCommandSoundTask(actor, cameraMode);
    BtlCameraModeTaskArgs *arguments = btlGetTaskArguments(task);
    arguments->actionCode = actionCode;
    arguments->unitA = unitA;
    arguments->unitB = unitB;
    return task;
}

u32 btlInitializeMotionTransformFromTaskArguments(BtlCameraTransformTaskArgs *arguments) {
    u8 *context = (u8 *)btlGetRuntime();
    btlSetCameraCommandMode(1, arguments->actor, 0, 0, 0);
    btlInitMotionTransformFromComponents(&((BtlState *)context)->cameraCommand.camera,
                    arguments->position[0], arguments->position[1], arguments->position[2],
                    arguments->direction[0], arguments->direction[1], arguments->direction[2],
                    arguments->direction[3], arguments->fovDegrees);
    return 1;
}

BtlRuntimeTask *btlCreateFloatTask28(BtlTask *actor, f32 x, f32 y, f32 z,
                                     f32 directionX, f32 directionY, f32 directionZ,
                                     f32 directionW, f32 fovDegrees) {
    BtlRuntimeTask *task = btlAllocTask(0x24);
    u8 *taskBytes = (u8 *)task;
    BtlCameraTransformTaskArgs *args;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)((u8 *)task + 0x20) = 0x28;
    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    if (actor != NULL && actor->unit != NULL) {
        *(u64 *)((u8 *)task + 0x40) = actor->unit->identity;
    }
    *(void **)((u8 *)task + 0x4C) = btlInitializeMotionTransformFromTaskArguments;
    *(u32 *)((u8 *)task + 0x48) = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->position[0] = x;
    args->position[1] = y;
    args->position[2] = z;
    args->direction[0] = directionX;
    args->direction[1] = directionY;
    args->direction[2] = directionZ;
    args->direction[3] = directionW;
    args->fovDegrees = fovDegrees;
    return task;
}

extern void btlSetEffectCameraKeys(u8 *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

s32 btlApplyEffectCameraKeyframes(BtlCameraKeyframesTaskArgs *args) {
    s32 context = (s32)(u32)btlGetRuntime();
    btlSetCameraCommandMode(1, args->actor, 0, 0, 0);
    btlSetEffectCameraKeys((u8 *)(context + 0x70),
                  args->frontPosition[0], args->frontPosition[1], args->frontPosition[2],
                  args->frontDirection[0], args->frontDirection[1],
                  args->frontDirection[2], args->frontDirection[3],
                  args->backPosition[0], args->backPosition[1], args->backPosition[2],
                  args->backDirection[0], args->backDirection[1],
                  args->backDirection[2], args->backDirection[3],
                  args->scale, args->motionParameter);
    return 1;
}

BtlRuntimeTask *btlCreateFloatTask29(BtlTask *actor, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, f32 a10, f32 a11, f32 a12, f32 a13, f32 a14, f32 a15, f32 a16) {
    BtlRuntimeTask *task = btlAllocTask(0x44);
    u8 *taskBytes = (u8 *)task;
    BtlCameraKeyframesTaskArgs *args;
    taskBytes[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)((u8 *)task + 0x20) = 0x29;
    taskBytes[0x10] = BTL_TASK_CONDITION_NEVER;
    if (actor != 0 && *(u8 **)((u8 *)actor + 0x18) != 0) {
        *(u64 *)((u8 *)task + 0x40) = *(u64 *)(*(u8 **)((u8 *)actor + 0x18) + 0x108);
    }
    *(void **)((u8 *)task + 0x4C) = btlApplyEffectCameraKeyframes;
    *(u32 *)((u8 *)task + 0x48) = 0;
    args = btlGetTaskArguments(task);
    args->actor = (BtlTask *)actor;
    args->frontPosition[0] = a1;
    args->frontPosition[1] = a2;
    args->frontPosition[2] = a3;
    args->frontDirection[0] = a4;
    args->frontDirection[1] = a5;
    args->frontDirection[2] = a6;
    args->frontDirection[3] = a7;
    args->backPosition[0] = a8;
    args->backPosition[1] = a9;
    args->backPosition[2] = a10;
    args->backDirection[0] = a11;
    args->backDirection[1] = a12;
    args->backDirection[2] = a13;
    args->backDirection[3] = a14;
    args->scale = a15;
    args->motionParameter = a16;
    return task;
}

u32 btlRunCameraMotionResetTask(void) {
    BtlState *work = (BtlState *)btlGetRuntime();

    btlResetCameraMotion(&work->cameraCommand);
    return 1;
}

BtlRuntimeTask *btlScheduleContextReset(void) {
    BtlRuntimeTask *task = (BtlRuntimeTask *)btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlRunCameraMotionResetTask;
    task->taskId = 0x2A;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

extern void dds3CameraObjectBasicSetFovy(dds3Basic_t *, f32);
extern f32 btlCameraLocalUpVector[4], btlCameraOutputRotation[4];

/* Per-frame battle camera update: run the active camera mode's update hook,
 * advance the command's pose blend, then push the pose into the world camera. */
void btlUpdateBattleCameraFrame(void) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    BtlCameraModeIndex cameraMode;
    dds3Basic_t *camera;
    CameraData *data;
    u32 flags;
    s32 result;
    f32 position[4];

    if (!(scene->battleFlags & 0x10)) {
        return;
    }
    cameraMode = scene->cameraCommand.status;
    if (cameraMode == 0) {
        return;
    }
    if (btlCameraModeDefinitions[cameraMode].update != NULL && !(scene->cameraCommand.flags & 0x400)) {
        result = 1;
        if (scene->cameraUpdatePredicate != NULL) {
            result = scene->cameraUpdatePredicate(&scene->cameraCommand);
        }
        if (result != 0) {
            btlCameraModeDefinitions[cameraMode].update(&scene->cameraCommand);
        }
    }
    camera = dds3GetActiveCameraSceneBasic(dds3GetActiveScene());
    if (camera == NULL || camera->value == NULL) {
        return;
    }
    flags = scene->cameraCommand.flags;
    data = camera->data;
    if (flags & 1) {
        if (scene->cameraCommand.state == 0) {
            btlNormalizeActionCameraKeyScales(&scene->cameraCommand);
            flags = scene->cameraCommand.flags;
        }
        result = 0;
        if (flags & 8) {
            result = btlCompleteCameraFlag08Mode(&scene->cameraCommand);
        } else if (flags & 0x10) {
            result = btlStepPoseBlendHalf(&scene->cameraCommand);
            flags = scene->cameraCommand.flags;
        } else if (flags & 0x20) {
            result = btlCompleteCameraFlag20Mode(&scene->cameraCommand);
        } else if (flags & 0x40) {
            result = btlStepPoseBlend(&scene->cameraCommand);
            flags = scene->cameraCommand.flags;
        } else if (flags & 0x80) {
            result = btlCompleteCameraFlag80Mode(&scene->cameraCommand);
        } else if (flags & 0x100) {
            result = btlStepPoseBlendFrame(&scene->cameraCommand);
            flags = scene->cameraCommand.flags;
        } else if (flags & 0x20000) {
            result = btlStepPoseBlendRatio(&scene->cameraCommand);
            flags = scene->cameraCommand.flags;
        }
        if (result == 1) {
            scene->cameraCommand.progressBits = 0;
            scene->cameraCommand.flags = flags & ~1;
            flags = scene->cameraCommand.flags;
        }
    }
    if (flags & 0x2000) {
        return;
    }
    SDF_VU0_LOADVECTOR(scene->cameraCommand.camera.direction);
    SDF_VU0_STOREVECTOR(&data->localEyeOffset);
    VU0_SCALAR_OP(-scene->cameraCommand.camera.distance, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_LOADVECTOR_B(scene->cameraCommand.camera.position);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_B(&data->worldEye);
    SDF_VU0_STOREVECTOR(position);
    dds3SetPosPoseSet(camera, (u128 *)position);
    dds3CameraObjectBasicSetFovy(camera, scene->cameraCommand.camera.fov);
    data->fovUpdatePending |= DDS3_CAMERA_UPDATE_FOV_BIT;
    dds3SetRotPoseSet(camera, (u128 *)btlCameraOutputRotation);
    SDF_VU0_COPYVECTOR(&data->localUp, btlCameraLocalUpVector);
    scene->cameraCommand.state++;
}

extern f32 btlWorldCameraStartOffset[4], btlWorldCameraStartRotation[4];

extern f32 btlCreatedCameraTarget[4], btlCreatedCameraRotation[4];

extern u32 D_003BB660;

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32, u128 *, u128 *);


extern void dds3GetRadiusPoseSet(dds3Basic_t *, f32);

extern void dds3ObjectBaseCreateMover(dds3Basic_t *);

/* vu0 routine: add the battle origin to the default camera position. */
void btlInitializeWorldCamera(void) {
    f32 position[4];
    dds3Basic_t *camera;
    CameraData *data;
    BtlState *battle = (BtlState *)btlGetRuntime();

    battle->cameraCommand.camera.fov = 0.6981317f;
    SDF_VU0_LOADVECTOR(btlWorldCameraStartOffset);
    SDF_VU0_LOADVECTOR_B(battle->position);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(position);
    camera = dds3GetActiveCameraSceneBasic(dds3GetActiveScene());
    if (camera != NULL) {
        dds3SetPosPoseSet(camera, (u128 *)position);
        dds3SetRotPoseSet(camera, (u128 *)btlWorldCameraStartRotation);
        data = camera->data;
        dds3CameraObjectBasicSetFovy(camera, 0.6981317f);
        data->fovUpdatePending |= DDS3_CAMERA_UPDATE_FOV_BIT;
    }
    camera = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), (u128 *)btlCreatedCameraTarget, (u128 *)btlCreatedCameraRotation);
    camera->value = (const char *)D_003BB660;
    dds3GetRadiusPoseSet(camera, 10.0f);
    dds3ObjectBaseCreateMover(camera);
    dds3ObjectBaseSetPadControl(camera, 0);
    dds3CameraObjectBasicSetFovy(camera, 0.6981317f);
    dds3ReplaceActiveCameraSceneBasic(dds3GetActiveScene(), camera);
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

void btlCopyMotionTransform(BtlCamState *dst, const BtlCamState *src) {
    SDF_VU0_COPYVECTOR(dst->position, src->position);
    SDF_VU0_COPYVECTOR(dst->direction, src->direction);
    dst->distance = src->distance;
    dst->fov = src->fov;
}

void btlSetMotionTransformFieldOfView(BtlCamState *object, f32 fovRadians) {
    object->fov = fovRadians;
}


extern void btlClearRuntimeFlag2000(void);

extern u8 D_0037E110[];

void btlInitMotionTransformFromVectors(BtlCamState *object, const f32 *origin, const f32 *direction) {
    SDF_VU0_LOADVECTOR(direction);
    effMiscQuaternionToMatrixVU();
    SDF_VU0_LOADVECTOR(D_0037E110);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(object->direction);
    VU0_SET_VF2X(1.0f);
    VU0_MUL_VF2X(vf10, vf10);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(origin);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(object->position);
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

    temp_v0 = (s32)(u32)btlGetRuntime();
    return *(u32 *)(temp_v0 + 0x174);
}

f32 btlGetPoseBlendProgress(BtlLinkedCommand *command) {
    return command->progress;
}

s32 btlIsUnitInActiveList(BtlUnit *unit) {
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

    temp_v0 = (s32)(u32)btlGetRuntime();
    *(u32 *)(temp_v0 + 0x160) = *(u32 *)(temp_v0 + 0x160) & 0xffffdfff;
}

void btlSetRuntimeFlag2000(void) {
    s32 temp_v0;

    temp_v0 = (s32)(u32)btlGetRuntime();
    *(u32 *)(temp_v0 + 0x160) = *(u32 *)(temp_v0 + 0x160) | 0x2000;
}

u32 btlIsRuntimeFlag2000Clear(void) {
    s32 temp_v0;

    temp_v0 = (s32)(u32)btlGetRuntime();
    return ((*(s32 *)(temp_v0 + 0x160) >> 0xd) ^ 1U) & 1;
}

extern f32 dds3CameraObjectBasicGetFovy(const dds3Basic_t *);

extern void dds3SetScreenFovy(f32);

void btlRefreshWorldCameraHandle(void) {
    dds3Basic_t *object;
    dds3Basic_t *handle;
    if (((BtlState *)btlGetRuntime())->battleFlags & 2) {
        object = dds3GetActiveScene();
        if (object != NULL) {
            handle = dds3GetActiveCameraSceneBasic(object);
            if (handle != 0) {
                if (handle->next != NULL) {
                    handle = handle->next;
                } else {
                    handle = ((EvtWorldTable *)object->data)->slots[EFF_WORLD_KIND_CAMERA].head;
                }
                dds3ReplaceActiveCameraSceneBasic(object, handle);
                dds3SetScreenFovy(dds3CameraObjectBasicGetFovy(handle));
            }
        }
    }
}

extern s32 D_003BB668;

extern s32 D_003BB664;

s32 btlGetWorldObjectDefault(void) {
    dds3Basic_t *camera;
    if (!(((BtlState *)btlGetRuntime())->battleFlags & 2)) {
        return D_003BB668;
    }
    camera = dds3GetActiveCameraSceneBasic(dds3GetActiveScene());
    if (camera == NULL) {
        return D_003BB668;
    }
    if (camera->value == 0) {
        return D_003BB664;
    }
    return (s32)camera->value;
}

s32 btlIsWorldMotionIdle(void) {
    s32 context = (s32)(u32)btlGetRuntime();
    if ((*(u32 *)(context + 0x1F4) & 2) == 0) {
        return 0;
    }
    {
        dds3Basic_t *camera = dds3GetActiveCameraSceneBasic(dds3GetActiveScene());
        if (camera == NULL) {
            return 0;
        }
        return camera->value == 0;
    }
}

s32 btlGetCameraVectorWork(void) {
    s32 temp_v0;

    temp_v0 = (s32)(u32)btlGetRuntime();
    return temp_v0 + 0x70;
}

extern void btlFlagAllUnitsDefeatCandidate(void);

void btlFlagAllUnitDefeatCandidatesTask(void) {
    btlFlagAllUnitsDefeatCandidate();
}

extern void btlClearAllUnitDefeatCandidates(void);

void btlClearAllUnitDefeatCandidatesTask(void) {
    btlClearAllUnitDefeatCandidates();
}

extern void btlFlagMatchingUnitsDefeatCandidate(s32 mask);

void btlFlagLinkedGroupDefeatCandidatesTask(BtlLinkedCommand *command) {
    btlFlagMatchingUnitsDefeatCandidate((u32)command->task->unit->status.flags & 0x600);
}

void btlApplyCombinedActorFlags(BtlLinkedCommand *command) {
    u32 flags = 0;
    u32 count = btlGetIndexListCount(command->targetList);
    u32 index;
    for (index = 0; index < count; index++) {
        BtlUnit *actor = (BtlUnit *)btlGetIndexListEntry(command->targetList, index);
        flags |= (u32)actor->status.flags & 0x600;
    }
    if (flags != 0) {
        btlFlagMatchingUnitsDefeatCandidate(flags);
    }
}

extern f32 btlCameraMotionResetFrameScales[];

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
                                s32 node = mdlGetMotionNum(unit->ext->owner, 0);
                                if (node == 0xD || node == 0x12) {
                                    current = btlGetUnitModelCurrentFrame(unit);
                                    limit = (f32)btlGetUnitModelFrameCount(unit);
                                    if (unit->partyRecord.unitId < 8) {
                                        limit = limit * btlCameraMotionResetFrameScales[unit->partyRecord.unitId];
                                    } else {
                                        limit = limit * fallbackScale;
                                    }
                                    if (current < limit) {
                                        sdfMotionSeek(unit->ext->owner->first, limit);
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

void btlDebugPrintWorldTransform(s32 x, s32 y) {
    dds3Basic_t *object;

    if (((BtlState *)btlGetRuntime())->battleFlags & 2) {
        object = dds3GetActiveCameraSceneBasic(dds3GetActiveScene());
        if (object != 0) {
            btlBossDebugPrintfN(x, y, 0, (s32)D_003A3DF0,
                                (double)object->inner->position[0],
                                (double)object->inner->position[1],
                                (double)object->inner->position[2]);
            btlBossDebugPrintfN(x, y + 12, 0, (s32)D_003A3E08,
                                (double)object->inner->rotation[0],
                                (double)object->inner->rotation[1],
                                (double)object->inner->rotation[2],
                                (double)object->inner->rotation[3]);
        }
    }
}

extern f32 btlMeasureIndexListBounds(BtlIndexList *, f32 *, f32 *);

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
            btlMeasureIndexListBounds(action->targetList, 0, 0);
        }
        pos = &vec[1];
        SDF_VU0_STOREVECTOR(pos);
        if (target != 0) {
            btlUnitGetMuzzlePosVU(target);
            SDF_VU0_STOREVECTOR(&vec[0]);
            if (target->status.flags & 0x80000) {
                if (btlAimHorizontalDirectionVU((f32 *)&vec[0], (f32 *)pos) != 0) {
                    SDF_VU0_STOREVECTOR_NOMEM(&vec[2]);
                    btlSetUnitRotation(target, (const f32 *)&vec[2]);
                }
            }
        }
        for (i = 0; i < count; i++) {
            btlUnitFaceTarget(btlGetIndexListEntry(action->targetList, i), target);
        }
    }
}

void btlAimLinkedUnitAtMuzzle(BtlLinkedCommand *action) {
    s128 vec[3];
    s128 *pos;
    BtlUnit *target;
    s32 count = btlGetIndexListCount(action->targetList);
    if (count != 0) {
        target = action->task->unit;
        if (count == 1) {
            btlUnitGetMuzzlePosVU((BtlUnit *)btlGetIndexListEntry(action->targetList, 0));
        } else {
            btlMeasureIndexListBounds(action->targetList, 0, 0);
        }
        pos = &vec[1];
        SDF_VU0_STOREVECTOR(pos);
        if (target != 0) {
            btlUnitGetMuzzlePosVU(target);
            SDF_VU0_STOREVECTOR(&vec[0]);
            if ((u32)target->status.flags & 0x80000) {
                if (btlAimHorizontalDirectionVU((f32 *)&vec[0], (f32 *)pos) != 0) {
                    SDF_VU0_STOREVECTOR_NOMEM(&vec[2]);
                    btlSetUnitRotation(target, (const f32 *)&vec[2]);
                }
            }
        }
    }
}

/* Let the runtime hook handle the actor before dispatching its camera step. */
/* Validate retained command fields beside their native initialization. */
typedef char BattleIndexWork_summonId_offset_check[
    ((u32)&((BattleIndexWork *)0)->summonId == 0x18) ? 1 : -1];
typedef char BattleIndexWork_battleExperienceReward_offset_check[
    ((u32)&((BattleIndexWork *)0)->battleExperienceReward == 0x54) ? 1 : -1];
typedef char BattleIndexWork_moneyReward_offset_check[
    ((u32)&((BattleIndexWork *)0)->moneyReward == 0x58) ? 1 : -1];

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3D38);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3D50);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3D60);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3D70);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3D80);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3D90);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3DD0);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3DF0);

INCLUDE_RODATA(const s32, "game/code_001D0BA0", D_003A3E08);

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
