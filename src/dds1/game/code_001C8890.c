#include "sdk_ps2.h"
#include "evt_run_context_api.h"
#include "btl_motion_transform.h"
#include "btl_unit_motion_duration_api.h"
#include "common.h"
#include "btl_unit_defeat_by_delta_result.h"
#include "btl_action_state_api.h"
#include "btl_action_entry_is_empty_api.h"
#include "btl_command_id.h"
#include "btl_unit_geometry_api.h"
#include "btl_horizontal_aim_api.h"
#include "btl_action_reaction_motion.h"
#include "btl_action_eligibility.h"
#include "sdf_vu0.h"
#include "btl_actor_resource_bank_api.h"
#include "btl_special_ability.h"
#include "btl_scene_light_params.h"
#include "btl_stage_task_cleanup.h"
#include "mdl_motion_api.h"
#include "sdf_motion.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "btl_effect_position.h"
#include "btl_raise_party_record_status_value.h"
#include "eff_offset_if_owner_flag_clear.h"
#include "sdf_chip.h"
#include "snd_slot.h"
#include "active_sound_node.h"
#include "kwln.h"
#include "kwln_process_registration.h"
#include "btl_task_state.h"
#include "btl_task_condition.h"
#include "sdf_resource.h"
#include "sdf_model.h"
#include "btl.h"
#include "btl_skill_name_index.h"
#include "btl_command_block_reason.h"
#include "btl_command_target_eligibility.h"
#include "btl_actor_work.h"
#include "sdf_texture_offset_list.h"
#include "btl_state.h"
#include "btl_battle_record_eligibility.h"
#include "btl_action_state_handler.h"
#include "btl_action_state_selection.h"
#include "btl_action_state_selection_slot.h"
#include "btl_model_record.h"
#include "btl_task_args.h"
#include "btl_sound.h"
#include "btl_sound_resource_use_result.h"
#include "btl_referenced_battle_effect_task.h"
#include "btl_actor_resource_cleanup_api.h"
#include "btl_enemy_spawn_capacity_api.h"
#include "btl_sound_node_api.h"
#include "btl_sound_release_task.h"
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
#include "btl_unit_fade_task_types.h"
#include "sdf.h"
#include "file.h"
#include "dat_command.h"
#include "file_request_api.h"
#include "dat_affinity.h"
#include "btl_ai_table.h"
#include "btl_ui.h"
#include "btl_target_selection_state_api.h"
/* Declared before first use (were implicit int). */
extern BtlRuntimeTask *btlScheduleRefreshTask(BtlUnit *owner);
extern BtlRuntimeTask *btlCreateUnitFadeInTask(BtlUnit *owner, u32 value, u32 variant);
extern BtlRuntimeTask * btlCreateCommandSoundTask(BtlTask *, BtlCameraModeIndex cameraMode);
extern BtlRuntimeTask *btlCreateSecondaryCommandSoundTask(void);
extern void btlDispatchCommandViaHookOrDefault(BtlTask *command, BattleIndexWork *argument);
extern BtlCameraModeIndex btlGetCameraCommandMode(void);
extern s32 btlGetStatusActionEffectId(BtlTask *task);
extern void btlInitUnitFxDefaults(BtlUnit *fx);
extern void btlInitializeBattleDebugMenu(void);
extern s32 btlIsUnitInActorList(const BtlUnit *candidate);
extern s32 btlIsSupportedCommandKind(const s32 *phase);
extern void btlRemovePartyActorAndShiftEntries(BtlUnit *unit);
extern void btlResetActiveUnitList(void);
extern s32 btlSetEligibleActorPanelTransitionState(void);
extern void btlSetCommandEffectLinkFlags(BtlTask *task, BattleIndexWork *work);
extern void btlSelectAndRunModeAiAction(BtlTask *task, u32 mode);
extern s32 sndHasActiveActor(void);
/* Original-style implicit int, kept: matched code depends on it: btlCreateCommandSoundUpdateTask. */
/* btlFlagUnitDefeatCandidate is void in its provider TU; preserve C89 call
 * scope here. An early void declaration moves the $16/$17/$31 restores before
 * the final dispatcher call (retail JAL +0xE4 becomes tail J +0xF4). */

enum { BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT = 0x26 };

void btlPrepareSavedCommandTargets(BtlTask *, BattleIndexWork *);

extern s32 btlIsUnitInActiveList(BtlUnit *unit);

extern AiSpecies *datEnemyAiRecords;

extern s32 btlAllocAndCheck(BtlTask *task);

extern u32 btlAssignTaskResultAndArgument(BtlTask *task);

extern void btlBindActorSlot(BtlTask *, s32);

extern s32 btlRunRandomWeightedAiTableAction(BtlTask *);

extern void btlDebugPrintf(const char *, ...);

extern void btlBossDebugPrintf(const char *format, ...);

extern u64 btlStartTask(BtlRuntimeTask *);


extern s32 btlCountTasksForOwner(u64);

extern u64 btlAdvanceRuntimeSequenceCounter(void);

extern BtlRuntimeTask *btlAllocateIndexedUnitEffectTask(BtlUnit *, s32, s32, f32);



extern BtlRuntimeTask *btlCreateMoveOtherUnitsTask(BtlUnit *, u32);

extern s32 btlUpdateBattleModelDebugEditor(void);

u8 *fldCreateSceneGroupAction(u8 *, u32, s32);

extern DatEnemyRecord *datEnemyRecords;

extern void btlSetActionActorPanelPresentation(BtlTask *, s8, s8);
extern BtlActionAnimationRecord *datActionAnimationRecords;


extern void sndSetStationedSeVolume(u32);

extern s32 btlRepositionPartyAroundBattleCenter(void);

extern s32 btlMarkInactiveActorCandidates(void);

s32 btlBothSidesActive(BtlUnit *unit);

void btlClearAllActorEntrySlots(BtlUnit *unit);

void btlClearSceneTaskActiveFlag(BtlTask *task);

s32 btlGetEntryFlagsUnlessDisabled(datUnitWork_t *entry);

extern s32 btlTestActorStatusPredicate(BtlUnit *);

extern void btlRefreshUnitMotionSelection(BtlUnit *unit);

extern s32 btlGetSlotRateKind(BtlUnit *, s32);

extern BtlRuntimeTask *btlCreateStiffenDamageShakeTask(BtlUnit *, f32);

s32 btlGetLoggedIndexedCommandItem(s32 index);


void btlSyncPlayerWork(BtlUnit *actor);

void btlCreateTargetSelectionTask(BtlTask *sourceTask);

void btlClearPartyRecordStatusBits(datUnitWork_t *record, s32 mask);

void btlActionSeqStateSelect(BtlTask *task) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit = task->unit;
    BtlActionStateSelectionHook hook;
    BtlActionStateSelectionResult next;
    u32 flags;
    task->flags &= ~0x20;
    if (task->actionNumber == 0) {
        btlDispatchStateHandler(task, 0x1A);
        btlBossDebugPrintf("btl:actnum 0 [%p]\n", task);
        return;
    }
    hook = ((BtlDds1ActionStateSelectionCallbackWindow *)work->pad5F4)->actionStateSelectionHook;
    if (hook != NULL) {
        next = hook(task);
        if (next != BTL_ACTION_STATE_SELECTION_USE_DEFAULT) {
            btlDispatchStateHandler(task, next);
            return;
        }
    }
    if (task->flags & 0x40) {
        btlDispatchStateHandler(task, 0xA);
    } else {
        flags = (u32)unit->status.flags;
        if (flags & 0x200) {
            if (work->battleFlags & 0x8000) {
                btlDispatchStateHandler(task, 9);
            } else {
                btlDispatchStateHandler(task, 6);
            }
        } else if (flags & 0x400) {
            if (!(work->runtimeFlags & 1)) {
                btlDispatchStateHandler(task, 8);
            } else {
                btlDispatchStateHandler(task, 6);
            }
        }
    }
}

void btlUnitTurnEndStateSelect(BtlTask *task) {
    BtlUnit *unit = task->unit;
    u32 flags = (u32)unit->status.flags;
    if (flags & 0x200) {
        if (flags & 0x1000) {
            if ((unit->status.stateFlags & 0x40) && !(unit->partyRecord.status & 0x7C0E) &&
                !(task->flags & 0x100)) {
                task->indexWork.stage = 4;
                task->indexWork.parameter = effOffsetIfOwnerFlagClear(unit, 0xA4);
                unit->status.flags = ((u32)unit->status.flags & ~0x20) | 0x400000;
                unit->partyRecord.flags |= 0x4000;
                unit->status.stateFlags |= 0x2000;
                btlDispatchStateHandler(task, 0x10);
            } else {
                btlDispatchStateHandler(task, 0x1D);
            }
            unit->status.stateFlags &= ~0x40;
        } else {
            btlDispatchStateHandler(task, 0x1D);
        }
    } else {
        btlDispatchStateHandler(task, 0x1D);
    }
}

extern s32 btlCountTasksByKind(u16 kind);

/* Frees inactive resources; PENDING means a resource or task still blocks scene progress. */
BtlBattleResourceCleanupResult btlPollBattleResourceCleanup(BtlUnit *self) {
    BtlActorWork *scene = (BtlActorWork *)btlGetRuntime();
    BtlUnit *actor;

    for (actor = scene->actorList; actor != 0; actor = actor->next) {
        if (actor->resourceNode != 0) {
            if (sndIsResourceNodeReferencedOrActive(actor->resourceNode) != BTL_SOUND_RESOURCE_NOT_IN_USE) {
                if (actor == self) {
                    return BTL_BATTLE_RESOURCE_CLEANUP_PENDING;
                }
                sndGetResourceStatus(actor->resourceNode);
                return BTL_BATTLE_RESOURCE_CLEANUP_PENDING;
            }
            sndFreeResourceNode(actor->resourceNode);
            actor->resourceNode = 0;
        }
    }
    if (self->listNode != 0) {
        if (sndHasResourceFlagsOneOrEight(self->listNode) != 0) {
            return BTL_BATTLE_RESOURCE_CLEANUP_PENDING;
        }
        sndFreeListNode(self->listNode);
        self->listNode = 0;
    }
    for (actor = scene->actorList; actor != 0; actor = actor->next) {
        if (actor->status.flags & 0x200) {
            if (actor->status.flags & 2) {
                if ((actor->gunResourceFlags & 8) == 0) {
                    return BTL_BATTLE_RESOURCE_CLEANUP_PENDING;
                }
            }
        }
    }
    if (btlCountTasksByKind(0x23) != 0) return BTL_BATTLE_RESOURCE_CLEANUP_PENDING;
    if (btlCountTasksByKind(0x3D) != 0) return BTL_BATTLE_RESOURCE_CLEANUP_PENDING;
    if (btlCountTasksByKind(0x3E) != 0) return BTL_BATTLE_RESOURCE_CLEANUP_PENDING;
    if (btlCountTasksByKind(0x3C) != 0) return BTL_BATTLE_RESOURCE_CLEANUP_PENDING;
    if (btlCountTasksByKind(0x33) != 0) return BTL_BATTLE_RESOURCE_CLEANUP_PENDING;
    if (btlCountTasksByKind(0x34) != 0) return BTL_BATTLE_RESOURCE_CLEANUP_PENDING;
    if (btlCountTasksByKind(0x24) != 0) return BTL_BATTLE_RESOURCE_CLEANUP_PENDING;
    return btlCountTasksByKind(0x2B) == 0
        ? BTL_BATTLE_RESOURCE_CLEANUP_COMPLETE
        : BTL_BATTLE_RESOURCE_CLEANUP_PENDING;
}

BtlActorResourceCleanupResult btlPollActorResourceCleanup(BtlUnit *actor) {
    if (actor->resourceNode != 0) {
        if (sndIsResourceNodeReferencedOrActive(actor->resourceNode) != BTL_SOUND_RESOURCE_NOT_IN_USE) {
            return BTL_ACTOR_RESOURCE_CLEANUP_PENDING;
        }
        sndFreeResourceNode(actor->resourceNode);
        actor->resourceNode = 0;
    }
    if (actor->listNode != 0) {
        if (sndHasResourceFlagsOneOrEight(actor->listNode) != 0) {
            return BTL_ACTOR_RESOURCE_CLEANUP_PENDING;
        }
        sndFreeListNode(actor->listNode);
        actor->listNode = 0;
    }
    if (actor->effectLink.referenceCount != 0) {
        return BTL_ACTOR_RESOURCE_CLEANUP_PENDING;
    }
    if (btlIsUnitInActiveList(actor) != 0) {
        btlResetActiveUnitList();
        return BTL_ACTOR_RESOURCE_CLEANUP_PENDING;
    }
    if (btlCountTasksForOwner(actor->identity) != 0) {
        return BTL_ACTOR_RESOURCE_CLEANUP_PENDING;
    }
    return btlCountTasksByKind(0x2B) == 0 ? BTL_ACTOR_RESOURCE_CLEANUP_COMPLETE : BTL_ACTOR_RESOURCE_CLEANUP_PENDING;
}

/* Empty entry callback for the retail NON action state. */
void btlEnterNonActionState(void) {
}

/* The retail NON state's update callback performs no work. */
void btlUpdateNonActionState(void) {
}

/* Enter STANDBY DEAD by clearing this action's pre-status-processing flag. */
void btlEnterStandbyDeadState(BtlTask *task) {
    task->flags = task->flags & 0xfffffdff;
}

void btlReleaseIdleUnitSoundAndAdvanceTask(BtlTask *task) {
    BtlUnit *unit = task->unit;
    u32 flags;
    u32 masked;
    SoundResourceNode *resource = unit->resourceNode;

    task->flags &= ~0x100;
    if (resource != 0) {
        if (sndIsResourceNodeReferencedOrActive(resource) == BTL_SOUND_RESOURCE_NOT_IN_USE) {
            sndFreeResourceNode(unit->resourceNode);
            unit->resourceNode = 0;
        }
    }
    flags = unit->status.flags;
    if ((flags & 0x400) == 0 && (unit->partyRecord.status & 0x4000) == 0) {
        if (unit->species == 0x1F) {
            btlRaisePartyRecordStatusValue(&unit->partyRecord, 0x1000);
            flags = unit->status.flags;
        }
        masked = flags & ~0x20;
        masked &= ~0x08000000;
        unit->status.flags = masked;
        btlFlagUnitDefeatCandidate(unit);
        btlRefreshUnitMotionSelection(unit);
        btlStartTask(btlAllocateIndexedUnitEffectTask(unit, 0xE, 0, 1.0f));
        fldAppendTaskToGroup(task);
        btlDispatchStateHandler(task, 2);
    }
    return;
}

/* Initialize the retail STANDBY action flags without changing their bit meanings. */
void btlEnterStandbyState(BtlTask *task) {
    task->flags = (task->flags | 0x10) & ~0x200;
}

void btlTaskUpdateFlags(BtlTask *task) {
    BtlUnit *unit = task->unit;
    u32 flags;
    if (!(unit->status.flags & 0x20)) {
        task->flags &= ~0x100;
    }
    if (unit->resourceNode != 0 && sndIsResourceNodeReferencedOrActive(unit->resourceNode) == BTL_SOUND_RESOURCE_NOT_IN_USE) {
        sndFreeResourceNode(unit->resourceNode);
        unit->resourceNode = 0;
    }
    flags = unit->status.flags;
    if (flags & 0x20000000) {
        btlDispatchStateHandler(task, 0x11);
    } else if (flags & 0x400000) {
        btlDispatchStateHandler(task, 0x10);
    } else if (flags & 0x10000000) {
        btlDispatchStateHandler(task, 0x12);
    } else if (flags & 0x20) {
        btlUnitTurnEndStateSelect(task);
    }
}

/* Enter the retail START state by clearing action flag 0x10. */
void btlEnterStartActionState(BtlTask *task) {
    task->flags = task->flags & 0xffffffef;
}

void btlActionSeqCheckDispatch(BtlTask *task) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    u32 flags = scene->battleFlags;
    BtlUnit *unit = task->unit;
    BtlUnit *actor;
    if (!(flags & 0x20)) {
        for (actor = scene->units; actor != 0; actor = actor->next) {
            u32 actorFlags = actor->status.flags;
            if (actorFlags & 0x4000) {
                return;
            }
            if (actorFlags & 0x30400000) {
                return;
            }
        }
        if (!(flags & 0x8000) ||
            btlPollBattleResourceCleanup(unit) != BTL_BATTLE_RESOURCE_CLEANUP_PENDING) {
            if (btlBothSidesActive(unit) == 0) {
                btlDispatchStateHandler(task, 0x1C);
                return;
            }
            if (btlCheckStatusActionEligibility(task) != BTL_ACTION_NOT_ELIGIBLE) {
                btlDispatchStateHandler(task, 5);
            } else {
                btlActionSeqStateSelect(task);
            }
        }
    }
}

/* Empty entry callback for the retail BAD RECOVERY action state. */
void btlEnterStatusRecoveryState(void) {
}

BtlRuntimeTask *btlCreateActorParameterDeltaTask(BtlUnit *owner, BtlOperandEntry *spec);


extern s32 btlRollAiBucket(void);

extern BtlRuntimeTask *btlCreateEffObjA(BtlUnit *, s32);

extern BtlRuntimeTask *btlCreateEffObjB(BtlUnit *, s32);

extern BtlRuntimeTask *btlCreateEffObjD(BtlUnit *, BtlSkillNameIndex);

/* Try to clear the unit's condition: 2 and 4 always clear, 1 needs battle mode 2, and the
 * others roll a script-supplied chance (capped at 70, scaled by ability 0x232). */
extern BtlRuntimeTask * btlCreateCommandSoundUpdateTask(void);

/* Roll ailment recovery, queue status removal and advance the acting unit's state. */
void btlProcessStatusRecoveryAction(BtlTask *link) {
    BtlOperandEntry spec;
    BtlUnit *unit;
    s32 chance;
    f32 scale;
    BtlRuntimeTask *task;

    if (btlCountTasksByKind(0x45) != 0) {
        return;
    }
    unit = link->unit;
    unit->status.flags |= 0x4000;
    switch (unit->partyRecord.status & 0x7FFF) {
    case 8:
    case 0x20:
    case 0x200:
    case 0x1000:
        if (unit->status.stateFlags & 4) {
            unit->status.stateFlags &= ~4;
            break;
        }
        /* fallthrough */
    case 1:
        unit->status.stateFlags &= ~4;
        switch (unit->partyRecord.status & 0x7FFF) {
        case 0x1000:
            chance = evtRunContextWithArguments(0xE, evtScriptArgumentFromPointer(&unit->partyRecord),
                evtScriptArgumentFromBits(0), 0, 0);
            break;
        case 0x200:
            chance = evtRunContextWithArguments(0xF, evtScriptArgumentFromPointer(&unit->partyRecord),
                evtScriptArgumentFromBits(0), 0, 0);
            break;
        case 0x20:
            chance = evtRunContextWithArguments(0x10, evtScriptArgumentFromPointer(&unit->partyRecord),
                evtScriptArgumentFromBits(0), 0, 0);
            break;
        case 8:
            chance = evtRunContextWithArguments(0x11, evtScriptArgumentFromPointer(&unit->partyRecord),
                evtScriptArgumentFromBits(0), 0, 0);
            break;
        case 1:
            chance = ((BtlState *)btlGetRuntime())->mode == 2 ? 100 : 0;
            break;
        default:
            chance = 0;
            break;
        }
        scale = 1.0f;
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x232)) {
            scale = datAbilityParameters[0x232 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        chance = chance * scale;
        if ((unit->partyRecord.status & 0x7FFF) != 1 && chance > 70) {
            chance = 70;
        }
        btlBossDebugPrintf("btl:bad recovery=%d%%[ratio=%.2f]\n", chance, scale);
        if (btlRollAiBucket() >= chance) {
            break;
        }
        /* fallthrough */
    case 2:
    case 4:
        memset(&spec, 0, sizeof(spec));
        spec.removedStatus = 0x122F;
        btlStartTask(btlCreateActorParameterDeltaTask(unit, &spec));
        if (unit->partyRecord.status & 0x1000) {
            unit->status.flags |= 0x20000000;
            task = btlCreateEffObjB(link->unit, 0xCA);
            task->ownerId = btlAdvanceRuntimeSequenceCounter();
            btlStartTask(task);
            btlStartTask(btlCreateCommandSoundUpdateTask());
            btlStartTask(btlCreateSecondaryCommandSoundTask());
            btlStartTask(btlCreateCommandSoundTask(link, 3));
        }
        break;
    }
    btlDispatchStateHandler(link, 0x1B);
}

/* Empty entry callback for BAD_BEFORE, the pre-action status-processing state. */
void btlEnterPreActionStatusState(void) {
}

void btlStartCommandSoundAndEffectTasks(u8 *battleTask) {
    BtlUnit *unit;
    u8 *task;
    u64 value;
    s32 count;

    if (sndHasActiveActor() != 0) {
        return;
    }
    if (btlCountTasksByKind(0x45) != 0) {
        return;
    }
    unit = *(BtlUnit **)(battleTask + 0x18);
    value = btlAdvanceRuntimeSequenceCounter();
    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    if ((u32)unit->status.flags & 0x200) {
        btlStartTask(btlCreateCommandSoundTask((BtlTask *)battleTask, 9));
    } else {
        btlStartTask(btlCreateCommandSoundTask((BtlTask *)battleTask, 3));
    }
    switch (unit->partyRecord.status & 0x7FFF) {
    case 0x200:
        btlSelectAndRunModeAiAction(battleTask, 0);
        btlDispatchStateHandler(battleTask, 0xC);
        break;
    case 0x2000:
        btlSelectAndRunModeAiAction(battleTask, 1);
        btlDispatchStateHandler(battleTask, 0xC);
        break;
    case 0x20:
        if ((u32)((BtlTask *)battleTask)->unit->status.flags & 0x200) {
            btlSelectAndRunModeAiAction(battleTask, 2);
        } else {
            btlSelectAndRunModeAiAction(battleTask, 3);
        }
        btlDispatchStateHandler(battleTask, 0xC);
        break;
    case 0x40:
        btlSelectAndRunModeAiAction(battleTask, 4);
        btlDispatchStateHandler(battleTask, 0xC);
        break;
    case 1:
        *(s32 *)(battleTask + 0x20) = 0xD;
        btlDispatchStateHandler(battleTask, 0xC);
        break;
    case 8:
        task = fldCreateSceneGroupAction(battleTask, 0x64, 1);
        *task = 7;
        *(u64 *)(task + 8) = value;
        *(u64 *)(task + 0x40) = unit->identity;
        btlStartTask(task);
        btlDispatchStateHandler(battleTask, 0x1A);
        break;
    case 0x800:
        task = fldCreateSceneGroupAction(battleTask, 0x64, 1);
        *task = 7;
        *(u64 *)(task + 8) = value;
        *(u64 *)(task + 0x40) = unit->identity;
        btlStartTask(task);
        btlDispatchStateHandler(battleTask, 0x1A);
        break;
    }
    count = btlGetStatusActionEffectId(battleTask);
    if (count > 0) {
        task = (u8 *)btlCreateEffObjB(unit, count);
        *(u64 *)(task + 0x40) = value;
        btlStartTask(task);
    }
    *(u32 *)(battleTask + 8) |= 0x200;
}

extern void btlOpenBattleCommandPanel(BtlTask *task);

/* Enter COMMAND by clearing its one-shot audio flag and opening the command panel. */
void btlEnterCommandSelectionState(BtlTask *task) {
    btlGetRuntime();
    task->flags = task->flags & 0xfffffffb;
    btlOpenBattleCommandPanel(task);
}

extern void btlRequestCommandPanelClose(void);

/* Poll command-panel selection and dispatch the selected action state. */
void btlUpdateCommandSelectionState(BtlTask *task) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    s32 state;

    if (scene->battleFlags & 0x20) {
        return;
    }
    if (!(task->flags & 4) && sndHasActiveActor() == 0 &&
        btlCountTasksByKind(0x2B) == 0) {
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
        btlStartTask(btlCreateCommandSoundTask(task, 9));
        task->flags |= 4;
    }

    state = fldGetSceneObjectState();
    if (state == 3 || state == 8) {
        if (btlIsSupportedCommandKind(&task->indexWork.phase) != 0) {
            btlDispatchStateHandler(task, 7);
        } else {
            btlRequestCommandPanelClose();
            if (btlAiCheckStatusRollEligibility(task) != BTL_ACTION_NOT_ELIGIBLE) {
                btlDispatchStateHandler(task, 0xB);
            } else {
                btlDispatchStateHandler(task, 0xC);
            }
        }
    } else if (scene->battleFlags & 0x8000) {
        btlRequestCommandPanelClose();
        btlDispatchStateHandler(task, 9);
    }
}

void btlCommandResultEffectSelect(BtlTask *task) {
    BtlCommandId commandId;
    BtlCommandBlockReason reason;
    s32 state;

    task->flags &= ~4;
    state = task->indexWork.phase;
    switch (state) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
        if (state == 4) {
            commandId = btlGetLoggedIndexedCommandItem(task->indexWork.reference);
        } else {
            commandId = task->indexWork.skillId;
        }
        reason = btlGetCommandBlockReason(task, commandId);
        switch (reason) {
        case BTL_COMMAND_BLOCK_NO_RESTRICTED_UNIT:
            btlStartTask(btlCreateEffObjB(task->unit, 0x82));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        case BTL_COMMAND_BLOCK_EVENT_61_NOT_ACTIVE:
            btlStartTask(btlCreateEffObjB(task->unit, 0xB0));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        case 7:
            btlStartTask(btlCreateEffObjB(task->unit, 0xB2));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        case BTL_COMMAND_BLOCK_EMPTY_OR_ALL_FLAGGED_TARGETS:
            btlStartTask(btlCreateEffObjB(task->unit, 0xD0));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        }
        break;
    }
    btlCreateTargetSelectionTask(task);
}

extern BtlIndexList *btlGetSelectedTargetList(void);

extern void btlRequestTargetSelectionClose(void);

extern s32 btlSetTaskPhase2(void);

void btlInitializeActorCommandState(BtlTask *task) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlTargetSelectionState state;
    BtlIndexList *selected;
    BtlCommandId commandId;
    BtlCommandTargetEligibilityResult reason;

    if (work->battleFlags & 0x20) {
        return;
    }
    state = btlGetTargetSelectionState();
    if ((task->flags & 4) == 0 && state != BTL_TARGET_SELECTION_STATE_COMMITTED &&
        sndHasActiveActor() == 0) {
        if (btlGetCameraCommandMode() != 9) {
            btlStartTask(btlCreateCommandSoundUpdateTask());
            btlStartTask(btlCreateSecondaryCommandSoundTask());
        }
        btlStartTask(btlCreateCommandSoundTask(task, 0xA));
        task->flags |= 4;
    }
    if (state == BTL_TARGET_SELECTION_STATE_COMMITTED) {
        selected = btlGetSelectedTargetList();
        switch (task->indexWork.phase) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 7:
        case 8:
            if (task->indexWork.phase == 4) {
                commandId = btlGetLoggedIndexedCommandItem(task->indexWork.reference);
            } else {
                commandId = task->indexWork.skillId;
            }
            reason = btlGetCommandTargetEligibility(selected, commandId);
            switch (reason) {
            case BTL_COMMAND_TARGET_ELIGIBILITY_RESULT_REQUIREMENTS_MET:
            case 4:
                btlStartTask(btlCreateEffObjB(task->unit, 0x6A));
                sndSetStationedSeVolume(0xD);
                btlSetTaskPhase2();
                return;
            case BTL_COMMAND_TARGET_ELIGIBILITY_RESULT_ENTRY_FLAG_40:
                btlStartTask(btlCreateEffObjB(task->unit, 0xA6));
                sndSetStationedSeVolume(0xD);
                btlSetTaskPhase2();
                return;
            case BTL_COMMAND_TARGET_ELIGIBILITY_RESULT_COMMAND_RESTRICTED:
                btlStartTask(btlCreateEffObjB(task->unit, 0xCC));
                sndSetStationedSeVolume(0xD);
                btlSetTaskPhase2();
                return;
            }
            break;
        }
        if (task->indexWork.phase != 9) {
            sndSetStationedSeVolume(8);
        }
        btlCopyIndexList(task->indexWork.indices, selected);
        btlRequestCommandPanelClose();
        btlRequestTargetSelectionClose();
        if (btlAiCheckStatusRollEligibility(task) != BTL_ACTION_NOT_ELIGIBLE) {
            btlDispatchStateHandler(task, 0xB);
        } else {
            btlDispatchStateHandler(task, 0xC);
        }
    } else if (state == BTL_TARGET_SELECTION_STATE_CANCELLED) {
        btlRequestTargetSelectionClose();
        btlDispatchStateHandler(task, 6);
    } else if (work->battleFlags & 0x8000) {
        btlRequestCommandPanelClose();
        btlRequestTargetSelectionClose();
        btlDispatchStateHandler(task, 9);
    }
}

/* Allow the AI updater to bind this acting unit's script slot once again. */
void btlEnterAiActionState(BtlTask *task) {
    task->flags = task->flags & ~BTL_AI_TASK_FLAG_ACTION_DISPATCHED;
}

/* Bind the acting unit's AI slot once, then wait for its script task. */
s32 btlAiTaskUpdate(BtlTask *task) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    u16 index;

    if (!(scene->battleFlags & 0x20)) {
        if (sndHasActiveActor() == 0) {
            if (btlPollBattleResourceCleanup(task->unit) != BTL_BATTLE_RESOURCE_CLEANUP_PENDING) {
                if (!(task->flags & BTL_AI_TASK_FLAG_ACTION_DISPATCHED)) {
                    index = task->unit->partyRecord.unitId;
                    scene->boundTask = 0;
                    if (datEnemyAiRecords[index].kind != 1 && btlAllocAndCheck(task) != 0) {
                        btlAssignTaskResultAndArgument(task);
                    } else if (datEnemyAiRecords[index].sceneSlot != 0) {
                        btlBindActorSlot(task, datEnemyAiRecords[index].sceneSlot);
                    } else {
                        btlRunRandomWeightedAiTableAction(task);
                    }
                    task->flags |= BTL_AI_TASK_FLAG_ACTION_DISPATCHED;
                    scene->battleFlags &= ~0x100000;
                }
                if (scene->boundTask == 0) {
                    scene->battleFlags |= 0x100000;
                    if (btlAiCheckStatusRollEligibility(task) != BTL_ACTION_NOT_ELIGIBLE) {
                        btlDispatchStateHandler(task, 0xB);
                    } else {
                        btlDispatchStateHandler(task, 0xC);
                    }
                } else if (dds3ExistProcess(scene->boundTask) == KWLN_PROCESS_NOT_REGISTERED) {
                    if (task->indexWork.phase == -1) {
                        btlBossDebugPrintf("btl:AI script return NULL[%p]\n", task);
                        btlDebugPrintf("AI script return NULL\n");
                        btlRunRandomWeightedAiTableAction(task);
                    }
                    scene->battleFlags |= 0x100000;
                    if (btlAiCheckStatusRollEligibility(task) != BTL_ACTION_NOT_ELIGIBLE) {
                        btlDispatchStateHandler(task, 0xB);
                    } else {
                        btlDispatchStateHandler(task, 0xC);
                    }
                    scene->boundTask = 0;
                }
            }
        }
    }
}

void btlMarkSceneTaskAfterReset(BtlTask *task) {
    btlSetActionActorPanelPresentation(task, 0, 0);
    task->flags |= 0x20;
}

s32 btlCommandStateSelectB(BtlTask *task) {
    if (sndHasActiveActor() == 0) {
        btlPrepareSavedCommandTargets(task, &task->indexWork);
        if (btlAiCheckStatusRollEligibility(task) != BTL_ACTION_NOT_ELIGIBLE) {
            btlDispatchStateHandler(task, 0xB);
        } else {
            btlDispatchStateHandler(task, 0xC);
        }
    }
}

/* Empty entry callback for the retail AUTO action state. */
void btlEnterAutoActionState(void) {
}

void btlChooseActorStateFromFirstLinkedUnit(u8 *actor) {
    BtlUnit *model;
    if (sndHasActiveActor() != 0) {
        return;
    }
    model = (BtlUnit *)btlGetIndexListEntry(*(struct BtlIndexList **)(actor + 0x60), 0);
    if (*(u32 *)(actor + 0x20) == 1 &&
        (btlIsUnitInActorList(model) == 0 ||
         (btlUnitStatusPair(model) & 0xE1) != 1)) {
        btlDispatchStateHandler(actor, 26);
    } else {
        btlDispatchStateHandler(actor, 12);
    }
}

void btlResetCommandIndexWork(BtlTask *task) {
    btlSetEligibleActorPanelTransitionState();
    btlResetIndexWork(&task->indexWork);
    task->unit->pendingSkillNameIndex = BTL_SKILL_NAME_INDEX_NONE;
}

void btlCommandStartSoundTasks(BtlTask *task) {
    BtlUnit *unit;
    s64 ownerId;
    s32 effect;
    BtlRuntimeTask *object;
    if (sndHasActiveActor() == 0 && btlCountTasksByKind(0x45) == 0) {
        unit = task->unit;
        ownerId = btlAdvanceRuntimeSequenceCounter();
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
        if ((u32)unit->status.flags & 0x200) {
            btlStartTask(btlCreateCommandSoundTask(task, 9));
        } else {
            btlStartTask(btlCreateCommandSoundTask(task, 3));
        }
        if ((unit->partyRecord.status & 0x7FFF) == 0x20) {
            if ((u32)task->unit->status.flags & 0x200) {
                btlSelectAndRunModeAiAction(task, 2);
            } else {
                btlSelectAndRunModeAiAction(task, 3);
            }
            btlDispatchStateHandler(task, 0xC);
        }
        effect = btlGetStatusActionEffectId(task);
        if (effect > 0) {
            object = btlCreateEffObjB(unit, effect);
            object->ownerId = ownerId;
            btlStartTask(object);
        }
        task->flags |= 0x200;
    }
}

extern char D_003A3648[]; /* "btl:command=%d\n" */


void btlResolveActionTargetResults(BtlTask *task, BattleIndexWork *work);

void btlCommandPrintAndFetchOwner(BtlTask *task) {
    BtlTask *link = task;
    BattleIndexWork *commandWork;
    s32 command;
    btlBossDebugPrintf(D_003A3648, link->indexWork.phase);
    commandWork = &link->indexWork;
    btlResolveActionTargetResults(task, commandWork);
    command = commandWork->phase;
    if (command <= 0) {
        return;
    }
    if (command >= 4) {
        if (command >= 9) {
            return;
        }
        if (command < 7) {
            return;
        }
    }
    if (btlGetIndexListCount(link->indexWork.indices) == 1) {
        link->indexWork.ownerId = ((BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, 0))->identity;
    }
}

void btlDispatchEffectCommandWhenActorReady(u8 *command) {
    u8 *actor = *(u8 **)(command + 0x18);
    if (btlPollBattleResourceCleanup((BtlUnit *)actor) == BTL_BATTLE_RESOURCE_CLEANUP_PENDING) {
        return;
    }
    if (*(u16 *)(command + 0x50) == 2) {
        btlStartTask(btlCreateEffObjB((BtlUnit *)actor, *(u32 *)(command + 0x54)));
    }
    btlSetCommandEffectLinkFlags(command, command + 0x20);
    btlDispatchCommandViaHookOrDefault((BtlTask *)command, (BattleIndexWork *)(command + 0x20));
}

/* Empty entry callback for the retail ATTACK action state. */
void btlEnterAttackActionState(void) {
}

/* ATTACK command scheduling; descriptive names inferred from retail consumers. */
extern EventRosterStat *datRosterDetails;
extern f32 btlGetActorEffectScale(BtlTask *);
extern BtlRuntimeTask *btlCreateTargetedCommandSoundTask(BtlTask *, BtlCameraModeIndex cameraMode, BtlActionCode actionCode);
extern BtlRuntimeTask *btlAllocateApproachTargetTask(BtlUnit *, BtlUnit *, f32);
extern SoundResourceNode *sndCreateResourceNode(SoundMixer *sourceMixer);
extern const char D_003A3658[];
extern const char D_003A3670[];
extern s32 btlGetActionAnimationDelay(BtlUnit *, s32);
extern s32 btlResolveSkillCategory(s32, u32);
extern BtlRuntimeTask *btlCreateDeferredActorStatsTask(BtlUnit *, BtlOperandEntry *);
extern BtlRuntimeTask *btlCreateActorSoundOptionTask(BtlUnit *, s32);
extern BtlRuntimeTask *btlScheduleEpPacketTask(BtlUnit *, s32);
extern BtlRuntimeTask *btlScheduleMoneyPacketTask(BtlUnit *, s32);
extern BtlRuntimeTask *btlCreateRefreshEligibleActorsTask(void);
extern BtlRuntimeTask *btlCreateImmediateCompletionTask(void);
extern BtlRuntimeTask *btlCreateUpdateUnitEffectsTask(void);
extern BtlRuntimeTask *btlCreateWaitUnitListIdleTask(u32);
extern BtlRuntimeTask *btlCreateApplyToActiveActorsTask(u32);
extern BtlRuntimeTask *btlCreateFadeStateResetTask(void);
extern BtlRuntimeTask *btlCreateEffectCounterTask(BtlUnit *, s32);
extern BtlRuntimeTask *btlCreateEffectTask3E(BtlUnit *, u16);
extern BtlRuntimeTask *btlCreateEffObjC(BtlUnit *, s32);
extern BtlRuntimeTask *sndCreateTimedUnitEffectTask(SoundResourceNode *, BtlUnit *, EffBattleVoiceKind, s32, u32);
extern BtlRuntimeTask *sndCreateSetStateTask(void);
extern BtlRuntimeTask *sndCreateClearStateTask(void);
extern BtlRuntimeTask *btlScheduleActorUpdate(BtlUnit *unit);
extern BtlRuntimeTask *btlCreateClearActorEntrySelectionTask(BtlUnit *);
extern BtlRuntimeTask *btlCreateActorPositionEffectTask(BtlUnit *);
extern BtlRuntimeTask *sndCreateStationedSeTask(u32);
extern BtlRuntimeTask *btlCreateLinkedEffectTask(BtlUnit *, s32, u8);

void btlScheduleAttackActionTasks(BtlTask *action) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *indexedTarget;
    BtlUnit *resultTarget = NULL;
    u8 *statusTable;
    BtlIndexList *indices;
    SoundResourceNode *resource;
    BtlOperandGroup *group;
    u64 masterOwner, operandOwner, commandOwner, moveHandle, lastOperandHandle;
    BtlRuntimeTask *operandTask;
    BtlRuntimeTask *task;
    BtlRuntimeTask *groupTask;
    s32 spacing;
    s32 repeatCount;
    u32 elapsed;
    BtlUnit *currentTarget;
    s32 reflectedStarted;
    s32 resourceNormal;
    s32 resourceTimed;
    s32 skipMovement;
    s32 categoryKind, extraKind;
    u32 targetCount, groupIndex, operandIndex, operandCount;
    s32 operandDelay, commandMap, actionFrames, groupDelay, initialDelay;
    s32 endDelay;
    s32 effectFrameOffset;
    f32 scale = btlGetActorEffectScale(action);

    resourceNormal = 0;
    resourceTimed = 0;
    endDelay = 0;
    skipMovement = 0;
    if (scale != 1.0f) {
        if ((btlUnitStatusPair(action->unit) & 0x1200) == 0x1200)
            action->indexWork.slot = 0x19;
        skipMovement = 1;
    }
    indices = action->indexWork.indices;
    targetCount = btlGetIndexListCount(indices);
    group = action->indexWork.groups;
    masterOwner = btlAdvanceRuntimeSequenceCounter();
    commandOwner = btlAdvanceRuntimeSequenceCounter();
    operandOwner = btlAdvanceRuntimeSequenceCounter();
    lastOperandHandle = btlAdvanceRuntimeSequenceCounter();
    statusTable = (u8 *)btlGetActorResourceBankEntry(action->unit->resourceKind, action->unit->species);
    if (skipMovement == 0) {
        operandTask = btlCreateMoveOtherUnitsTask(action->unit, action->indexWork.slot);
        btlStartTask(operandTask);
        moveHandle = operandTask->handle;
    } else {
        moveHandle = btlAdvanceRuntimeSequenceCounter();
    }
    task = (BtlRuntimeTask *)sndCreateSetStateTask();
    task->startCondition.kind = 4;
    task->startCondition.value.handle = moveHandle;
    btlStartTask(task);
    task = (BtlRuntimeTask *)btlCreateCommandSoundUpdateTask();
    task->startCondition.kind = 4;
    task->startCondition.value.handle = moveHandle;
    btlStartTask(task);
    task = btlCreateSecondaryCommandSoundTask();
    task->startCondition.kind = 4;
    task->startCondition.value.handle = moveHandle;
    btlStartTask(task);
    task = btlAllocateIndexedUnitEffectTask(action->unit, action->indexWork.slot,
        btlGetSlotRateKind(action->unit, action->indexWork.slot), scale);
    task->startCondition.kind = 4;
    task->startCondition.value.handle = moveHandle;
    btlStartTask(task);
    task = btlCreateTargetedCommandSoundTask(action, 4, 0);
    task->startCondition.kind = 4;
    task->startCondition.value.handle = moveHandle;
    btlStartTask(task);
    if (action->unit->pendingSkillNameIndex == BTL_SKILL_NAME_INDEX_NONE)
        task = btlCreateEffObjD(action->unit, action->indexWork.skillId);
    else
        task = btlCreateEffObjD(action->unit, action->unit->pendingSkillNameIndex);
    task->startCondition.kind = 4;
    task->startCondition.value.handle = moveHandle;
    task->ownerId = btlAdvanceRuntimeSequenceCounter();
    btlStartTask(task);
    if (targetCount == 1) {
        indexedTarget = (BtlUnit *)btlGetIndexListEntry(indices, 0);
        if (btlIsBattleRecordEligible(action->unit, indexedTarget,
                action->indexWork.slot, action->indexWork.skillId) != 0) {
            if ((action->unit->status.flags & indexedTarget->status.flags & 0x600) == 0 ||
                    (indexedTarget->status.flags & 0x200) != 0) {
                task = btlAllocateApproachTargetTask(action->unit, indexedTarget,
                    state->modelFrameScale);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = moveHandle;
                btlStartTask(task);
            }
        }
    }
    resource = NULL;
    groupDelay = (s32)(btlGetUnitMotionDurationFrames(action->unit, action->indexWork.slot) / scale);
    initialDelay = groupDelay;
    btlBossDebugPrintf(D_003A3658, groupDelay);
    if ((action->unit->status.flags & 0x200) != 0 &&
            ((action->unit->status.flags & 0x1000) == 0 || action->indexWork.slot == 0x17)) {
        resource = action->unit->resourceNode = sndCreateResourceNode((SoundMixer *)action->unit->gunResource);
        if ((action->unit->status.flags & 0x1000) == 0) resourceNormal = 1;
        else resourceTimed = 1;
    }
    currentTarget = action->unit;
    reflectedStarted = 0;
    actionFrames = btlGetActionAnimationDelay(action->unit, action->indexWork.skillId);
    commandMap = btlResolveSkillCategory((s32)action->unit, action->indexWork.skillId);
    for (groupIndex = 0; groupIndex < targetCount; groupIndex++, group++) {
        if (resourceNormal == 0 && resourceTimed == 0) {
            s32 resourceIndex = group->parameter != 2 ? 26 : 27;
            resource = state->resources[resourceIndex];
        }
        operandCount = group->count;
        indexedTarget = (BtlUnit *)btlGetIndexListEntry(indices, groupIndex);
        currentTarget = indexedTarget;
        if (((btlUnitStatusPair(action->unit) & 0x1200) == 0x1200 && action->indexWork.slot == 3) ||
                (action->unit->status.flags & 0x400) != 0) {
            if ((action->unit->status.flags & 0x200) != 0) {
                repeatCount = datRosterDetails[action->unit->partyRecord.unitId].attackCount;
                spacing = datRosterDetails[action->unit->partyRecord.unitId].attackSpacing;
            } else {
                repeatCount = datEnemyRecords[action->unit->species].tickCount;
                spacing = datEnemyRecords[action->unit->species].attackSpacing;
                if (repeatCount <= 0) repeatCount = 1;
            }
            spacing = (s32)(spacing / ((BtlActorStatusRecord *)statusTable)->motions[
                action->indexWork.slot].alphaFrameScale);
            spacing = (s32)(spacing / scale);
            if (spacing <= 0) spacing = 1;
            elapsed = 0xFFFFFF;
            if (group->kind != 2 && group->kind != 0x40000 && group->reflected == 0) {
                for (operandIndex = 1; operandIndex < repeatCount; operandIndex++) {
                    elapsed += spacing;
                    if (elapsed >= 3) {
                        elapsed = 0;
                        groupTask = btlCreateReferencedBattleEffectTask(resource, action->unit, currentTarget, 2);
                        groupTask->startCondition.kind = 4;
                        groupTask->startCondition.value.handle = moveHandle;
                        groupTask->startDelay = groupDelay - spacing * operandIndex;
                        groupTask->ownerId = masterOwner;
                        btlStartTask(groupTask);
                        groupTask = sndCreateStationedSeTask(group->parameter != 2 ? 0x1000A : 0x1000B);
                        groupTask->startCondition.kind = 4;
                        groupTask->startCondition.value.handle = moveHandle;
                        groupTask->startDelay = groupDelay - spacing * operandIndex;
                        groupTask->ownerId = masterOwner;
                        btlStartTask(groupTask);
                        operandTask = btlAllocateIndexedUnitEffectTask(currentTarget,
                            group->reactionCode, 0, 1.0f);
                        operandTask->startCondition.kind = 4;
                        operandTask->startCondition.value.handle = moveHandle;
                        operandTask->startDelay = groupDelay - spacing * operandIndex;
                        btlStartTask(operandTask);
                    }
                }
            } else if (group->kind == 2) {
                groupDelay -= spacing * (repeatCount - 1);
            }
        }
        if (group->reflected != 0) currentTarget = action->unit;
        if (group->targetSpecialHit != 0) {
            task = btlCreateClearActorEntrySelectionTask(indexedTarget);
            task->startDelay = groupDelay;
            btlStartTask(task);
        }
        if (group->sourceSpecialHit != 0) {
            task = btlCreateClearActorEntrySelectionTask(currentTarget);
            task->startDelay = groupDelay;
            btlStartTask(task);
        }
        if (group->inactiveOrStatusChanged != 0 && (state->battleFlags & 0x80) != 0) {
            endDelay = 18;
            btlBossDebugPrintf(D_003A3670, currentTarget);
            {
                u64 owner = action->unit->identity;
                endDelay = action->indexWork.allTargetSideActorsSkipped != 0 ? endDelay : 0;
                commandOwner = owner;
            }
        } else {
            endDelay = 18;
            commandOwner = btlAdvanceRuntimeSequenceCounter();
            endDelay = skipMovement == 0 ? endDelay : 0;
        }
        if (group->kind != 0x40000) {
            if (group->kind != 2 && (group->reflected == 0 || reflectedStarted == 0)) {
                groupTask = btlCreateReferencedBattleEffectTask(resource, action->unit, currentTarget, 2);
                groupTask->startCondition.kind = 4;
                groupTask->startCondition.value.handle = moveHandle;
                groupTask->startDelay = groupDelay;
                groupTask->ownerId = masterOwner;
                btlStartTask(groupTask);
                if (resourceNormal == 0 && resourceTimed == 0) {
                    groupTask = sndCreateStationedSeTask(group->parameter != 2 ? 0x1000A : 0x1000B);
                    groupTask->startCondition.kind = 4;
                    groupTask->startCondition.value.handle = moveHandle;
                    groupTask->startDelay = groupDelay;
                    groupTask->ownerId = masterOwner;
                    btlStartTask(groupTask);
                }
                if (group->reflected != 0) reflectedStarted = 1;
            } else {
                groupTask = btlCreateImmediateCompletionTask();
                groupTask->startCondition.kind = 4;
                groupTask->startCondition.value.handle = moveHandle;
                groupTask->startDelay = groupDelay;
                groupTask->ownerId = masterOwner;
                btlStartTask(groupTask);
            }
        } else {
            groupTask = btlCreateReferencedBattleEffectTask(state->resources[30], action->unit, currentTarget, 2);
            groupTask->startCondition.kind = 4;
            groupTask->startCondition.value.handle = moveHandle;
            groupTask->startDelay = groupDelay;
            groupTask->ownerId = masterOwner;
            btlStartTask(groupTask);
            groupTask = sndCreateStationedSeTask(0x1000D);
            groupTask->startCondition.kind = 4;
            groupTask->startCondition.value.handle = moveHandle;
            groupTask->startDelay = groupDelay;
            groupTask->ownerId = masterOwner;
            btlStartTask(groupTask);
        }
        if (group->reflected != 0) {
            groupTask = btlCreateReferencedBattleEffectTask(state->resources[28], action->unit, indexedTarget, 2);
            groupTask->startCondition.kind = 4;
            groupTask->startCondition.value.handle = moveHandle;
            groupTask->startDelay = groupDelay;
            groupTask->ownerId = masterOwner;
            btlStartTask(groupTask);
            groupTask = sndCreateStationedSeTask(0x1000C);
            groupTask->startCondition.kind = 4;
            groupTask->startCondition.value.handle = moveHandle;
            groupTask->startDelay = groupDelay;
            groupTask->ownerId = masterOwner;
            btlStartTask(groupTask);
        }
        for (operandIndex = 0, operandDelay = 0, effectFrameOffset = 0; operandIndex < operandCount; operandIndex++) {
            BtlActionReactionMotionIndex selection;
            if (btlActionEntryIsEmpty(action->indexWork.skillId, group, &group->entries[operandIndex]) != 0) {
                selection = BTL_ACTION_REACTION_MOTION_NONE;
                categoryKind = 4;
                extraKind = 1;
            } else {
                selection = group->reactionCode;
                categoryKind = group->kind;
                extraKind = group->parameter;
            }
            if (resourceTimed == 0) {
                operandTask = btlAllocateIndexedUnitEffectTask(currentTarget, selection, 0, 1.0f);
                operandTask->startCondition.kind = 4;
                operandTask->startCondition.value.handle = moveHandle;
                operandTask->startDelay = groupTask->startDelay + operandDelay;
                btlStartTask(operandTask);
            } else {
                operandTask = sndCreateTimedUnitEffectTask(resource, currentTarget, 2, selection, 0);
                operandTask->startCondition.kind = 5;
                operandTask->startCondition.value.handle = groupTask->handle;
                operandTask->startDelay = operandDelay;
                btlStartTask(operandTask);
            }
            lastOperandHandle = operandTask->handle;
            if (group->kind == 2 && operandIndex == 0) {
                task = btlCreateActorPositionEffectTask(currentTarget);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                btlStartTask(task);
            }
            if (action->indexWork.stage == 1 && groupIndex == 0 && operandIndex == 0) {
                BtlRuntimeTask *stageTask;
                effectFrameOffset += 0xC;
                stageTask = btlCreateEffObjB(action->unit, action->indexWork.parameter);
                stageTask->startCondition.kind = 4;
                stageTask->startCondition.value.handle = operandTask->handle;
                btlStartTask(stageTask);
            }
            if (action->indexWork.huntEpReward != 0 && groupIndex == 0 && operandIndex == operandCount - 1) {
                task = btlCreateEffectTask3E(action->unit, (u16)action->indexWork.huntEpReward);
                task->startCondition.kind = 4;
                task->startDelay = effectFrameOffset + 0xC;
                task->startCondition.value.handle = operandTask->handle;
                btlStartTask(task);
            }
            if ((indexedTarget->status.flags & 0x10) != 0 && operandIndex == 0) {
                task = btlScheduleActorUpdate(indexedTarget);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                btlStartTask(task);
            }
            if (groupIndex == 0 && operandIndex == operandCount - 1) {
                task = (BtlRuntimeTask *)fldCreateSceneGroupAction((u8 *)action, action->indexWork.adjustedValue, (u8)action->indexWork.resultKind);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                task->endDelay = endDelay;
                task->ownerId = action->unit->identity;
                btlStartTask(task);
            }
            if (groupIndex == targetCount - 1 && operandIndex == operandCount - 1) {
                task = btlCreateActorSoundOptionTask(action->unit, action->indexWork.huntEpReward);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = commandOwner;
                btlStartTask(task);
            }
            if (groupIndex == 0 && operandIndex == operandCount - 1) {
                task = btlScheduleEpPacketTask(action->unit, action->indexWork.battleExperienceReward);
                task->startCondition.kind = 4;
                                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = commandOwner;
                btlStartTask(task);
                task = btlScheduleMoneyPacketTask(action->unit, action->indexWork.moneyReward);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = commandOwner;
                btlStartTask(task);
            }

            task = btlCreateActorParameterDeltaTask(currentTarget, &group->entries[operandIndex]);
            task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = commandOwner;
            btlStartTask(task);
            task = btlCreateDeferredActorStatsTask(action->unit, &group->entries[operandIndex]);
            task->startCondition.kind = 4;
            task->startDelay = commandMap;
            task->startCondition.value.handle = operandTask->handle;
            task->ownerId = commandOwner;
            btlStartTask(task);

            if (group->entries[operandIndex].hpDelta != 0) {
                task = btlCreateLinkedEffectTask(currentTarget, group->entries[operandIndex].hpDelta, 0);
                task->startCondition.kind = 4;
                task->startDelay = 1;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = operandOwner;
                btlStartTask(task);
            }
            if (group->entries[operandIndex].mpDelta != 0) {
                task = btlCreateLinkedEffectTask(currentTarget, group->entries[operandIndex].mpDelta, 1);
                task->startCondition.kind = 4;
                task->startDelay = 1;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = operandOwner;
                btlStartTask(task);
            }
            if (group->entries[operandIndex].hpRecovery != 0) {
                task = btlCreateLinkedEffectTask(action->unit, group->entries[operandIndex].hpRecovery, 0);
                task->startCondition.kind = 4;
                task->startDelay = commandMap;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = operandOwner;
                btlStartTask(task);
            }
            if (group->entries[operandIndex].mpRecovery != 0) {
                task = btlCreateLinkedEffectTask(action->unit, group->entries[operandIndex].mpRecovery, 1);
                task->startCondition.kind = 4;
                task->startDelay = commandMap;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = operandOwner;
                btlStartTask(task);
            }

            if ((group->entries[operandIndex].flags & 0x1000) == 0) {
                if (categoryKind == 0x10000 || categoryKind == 4) {
                    if (categoryKind == 0x10000) task = btlCreateEffectCounterTask(currentTarget, 4);
                    else task = btlCreateEffectCounterTask(currentTarget, 3);
                    task->startCondition.kind = 4;
                    task->startDelay = 1;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = operandOwner;
                    btlStartTask(task);
                }
                if (extraKind == 2 || extraKind == 4) {
                    if (extraKind == 2) task = btlCreateEffectCounterTask(currentTarget, 1);
                    else task = btlCreateEffectCounterTask(currentTarget, 2);
                    task->startCondition.kind = 4;
                    task->startDelay = 1;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = operandOwner;
                    btlStartTask(task);
                }
            }
            if ((group->entries[operandIndex].addedStatus & 1) != 0 && currentTarget->partyRecord.status == 0) {
                resultTarget = currentTarget;
            }
            operandDelay += group->interval;
        }
        if (state->postTargetHook != NULL)
            state->postTargetHook(action, action->indexWork.skillId, indexedTarget, masterOwner, lastOperandHandle, -1);
        groupDelay += actionFrames;
    }
    if (action->indexWork.refreshEligibleActors != 0) {
        task = btlCreateRefreshEligibleActorsTask();
        task->startCondition.kind = 7;
        task->startCondition.value.handle = commandOwner;
        btlStartTask(task);
    }
    if (resourceNormal != 0 && resource != NULL) {
        task = btlCreateReferencedBattleEffectTask(resource, action->unit, currentTarget, 1);
        task->startCondition.kind = 4;
        task->startCondition.value.handle = moveHandle;
        task->startDelay = initialDelay;
        task->ownerId = masterOwner;
        btlStartTask(task);
    }
    if (resultTarget != NULL && action->indexWork.wait >= 0 && (action->flags & 0x20) == 0) {
        task = btlCreateEffObjC(resultTarget, action->indexWork.wait);
        task->startCondition.kind = 7;
        task->startCondition.value.handle = masterOwner;
        btlStartTask(task);
    }
    if (skipMovement == 0) {
        task = (BtlRuntimeTask *)btlCreateUpdateUnitEffectsTask();
        task->startCondition.kind = 7;
        task->startCondition.value.handle = masterOwner;
        btlStartTask(task);
    }
    task = btlCreateWaitUnitListIdleTask(12);
    task->startCondition.kind = 7;
    task->startCondition.value.handle = masterOwner;
    task->startDelay = endDelay;
    btlStartTask(task);
    task = btlCreateApplyToActiveActorsTask(12);
    task->startCondition.kind = 7;
    task->startCondition.value.handle = masterOwner;
    task->startDelay = endDelay;
    btlStartTask(task);
    task = (BtlRuntimeTask *)sndCreateClearStateTask();
    task->startCondition.kind = 7;
    task->startCondition.value.handle = masterOwner;
    task->startDelay = endDelay;
    btlStartTask(task);
    task = (BtlRuntimeTask *)btlCreateFadeStateResetTask();
    task->startCondition.kind = 7;
    task->startCondition.value.handle = masterOwner;
    task->startDelay = endDelay;
    btlStartTask(task);
    if ((action->unit->partyRecord.status & 0x480) != 0) btlDispatchStateHandler(action, 24);
    else btlDispatchStateHandler(action, 26);
}

/* Empty entry callback for the retail SKILL action state. */
void btlEnterSkillActionState(void) {
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3648);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3658);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3670);

#ifdef NON_MATCHING
#include "btl_motion_transform.h"
#include "common.h"
#include "btl_actor_resource_bank_api.h"
#include "btl_special_ability.h"
#include "btl_scene_light_params.h"
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
#include "btl_actor_work.h"
#include "sdf_texture_offset_list.h"
#include "btl_state.h"
#include "btl_model_record.h"
#include "btl_task_args.h"
#include "btl_sound.h"
#include "btl_sound_node_api.h"
#include "btl_sound_release_task.h"
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
#include "btl_ai_table.h"
#include "btl_ui.h"
#include "btl_enemy_spawn_capacity_api.h"
#include "btl_boss_debug_api.h"
#include "btl_battle_record_eligibility.h"
#include "btl_referenced_battle_effect_task.h"
#include "btl_action_state_api.h"
extern void *btlCreateDeferredUnitStatusTask(u8 *owner, u32 value);
extern u8 *btlCreateItemQuantityIncrementTask(u8 *actor, u16 itemId);


extern BtlRuntimeTask *btlCreateSceneLightingRestoreTask(u32 value);
extern u8 *fldCreateSceneActorAction(u8 *actorTask, s32 slotCount);
extern u8 *fldCreateSceneGroupAction(u8 *actorTask, u32 counterAmount, s32 counterMode);
extern void btlBuildActionLightParameters(s32 skillIndex, BtlSceneLightParams *light);


extern BtlRuntimeTask *btlCreateCommandSoundUpdateTask(void);
extern BtlRuntimeTask *btlCreateEffectTask44(BtlUnit *owner);

extern BtlRuntimeTask *btlCreateEffectWaitTask(BtlUnit *owner, u16 mode);

extern BtlRuntimeTask *btlCreateOwnerLinkedTimedTask(BtlUnit *owner, s32 skillNameIndex);
extern s32 btlFormatActionEventFilename(s32 skillIndex, char *output);
extern s32 btlGetActionDefaultOrOverride(s32 skillIndex);
extern s32 btlHasAnyOperandLowFlag(s32 unused, BattleIndexWork *state);
extern BtlRuntimeTask *sndCreateActorEffectTask(SoundResourceNode *source, BtlUnit *owner, u32 duration);
extern BtlRuntimeTask *sndCreateEffectLoadTask(SoundResourceNode *effect, const char *filename);






extern u64 btlAdvanceRuntimeSequenceCounter();


extern BtlActionAnimationRecord *datActionAnimationRecords;

extern BtlRuntimeTask *btlCreateEffObjB(BtlUnit *, s32);
extern BtlRuntimeTask *btlCreateEffObjD(BtlUnit *, s32);


extern const char D_003A3658[];
extern const char D_003A3670[];
extern s32 btlGetActionAnimationDelay(BtlUnit *, s32);
extern s32 btlResolveSkillCategory(s32, u32);
extern s32 btlActionEntryIsEmpty(s32, BtlOperandGroup *, BtlOperandEntry *);





extern BtlRuntimeTask *btlCreateImmediateCompletionTask(void);
extern BtlRuntimeTask *btlCreateUpdateUnitEffectsTask(void);


extern BtlRuntimeTask *btlCreateFadeStateResetTask(void);
extern BtlRuntimeTask *btlCreateEffectCounterTask(BtlUnit *, s32);
extern BtlRuntimeTask *btlCreateEffectTask3E(BtlUnit *, u16);
extern BtlRuntimeTask *btlCreateEffObjC(BtlUnit *, s32);
extern BtlRuntimeTask *sndCreateTimedUnitEffectTask(SoundResourceNode *, BtlUnit *, u16, s32, u32);
extern BtlRuntimeTask *sndCreateSetStateTask(void);
extern BtlRuntimeTask *sndCreateClearStateTask(void);



extern BtlRuntimeTask *btlCreateLinkedEffectTask(BtlUnit *, s32, u8);

/* Schedule the whole battle action sequence for one command on its actor:
 * movement, sounds, effect load, approach, per-target hit effects with damage,
 * status and reward tasks (reflected hits and kind-specific effects included),
 * the trailing effects and the scene actions, then hand the actor to the next
 * state handler (state 0x15 for kind 2 commands, 0x18 when stunned or dead,
 * otherwise 0x1A). Upstream: INCLUDE_ASM in game/code_001C8890; written from
 * the retail routine. It differs from retail only in register allocation,
 * scheduling, one constant load placement, a nop and the jump table address,
 * and keeps the retail call order throughout. */
void func_001CB410(BtlTask *actor) {
    BtlState *runtime;
    BtlUnit *unit;
    BtlUnit *targetUnit;
    BtlUnit *effectUnit;
    BtlUnit *sceneUnit = NULL;
    BtlIndexList *indexList;
    BtlOperandGroup *group;
    BtlRuntimeTask *taskWork;
    BtlRuntimeTask *initialTask;
    SoundResourceNode *effect;
    ActiveSoundNode *listNode;
    BtlSceneLightParams light;
    char resourceName[0x70];
    u64 effectLoadSequence;
    u64 actionSequence;
    u64 spareSequence;
    u64 damageSequence;
    u64 finalSequence;
    s32 reflectedEffectStarted;
    u64 categoryEnabled = 1;
    s32 soundEnabled;
    s32 trailingSound;
    s32 trailingEffect;
    s32 pendingModelBlend;
    u32 targetIndex;
    u32 entryIndex;
    u32 targetCount;
    u32 entryCount;
    s32 entryDelay;
    s32 category;
    s32 skillId;
    s32 groupDelay;
    u32 modelPolicy;
    s32 hitKind;
    s32 hitParameter;
    s32 extraDelay;
    s32 selector;
    s32 phase;
    s32 slot;
    s32 resolvedAction;
    s32 resourceType;
    s32 animationLength;
    BtlUnit *currentUnit;
    BtlOperandEntry *operand;
    BtlRuntimeTask *effectTask;
    BtlRuntimeTask *phaseTask;
    BtlRuntimeTask *sceneGroupTask;
    s32 reaction;
    s32 specialModel;

    unit = actor->unit;
    runtime = (BtlState *)btlGetRuntime();
    indexList = actor->indexWork.indices;
    targetCount = btlGetIndexListCount(indexList);
    effectUnit = unit;
    group = actor->indexWork.groups;
    skillId = actor->indexWork.skillId;
    selector = -1;
    pendingModelBlend = 0;
    trailingSound = 0;
    trailingEffect = 0;
    soundEnabled = 1;

    effectLoadSequence = btlAdvanceRuntimeSequenceCounter();
    actionSequence = btlAdvanceRuntimeSequenceCounter();
    damageSequence = btlAdvanceRuntimeSequenceCounter();
    spareSequence = btlAdvanceRuntimeSequenceCounter();
    finalSequence = btlAdvanceRuntimeSequenceCounter();

    if (datCommandSelectors[skillId].kind == 2) {
        soundEnabled = 0;
        if (btlHasAnyOperandLowFlag(skillId, &actor->indexWork) != 0) {
            trailingSound = 1;
            trailingEffect = 1;
        } else {
            categoryEnabled = 0;
        }
    }

    slot = actor->indexWork.slot;
    (void)btlGetActorResourceBankEntry(unit->resourceKind, unit->species);

    if (actor->indexWork.costValidationFailed == 0) {
        initialTask = (BtlRuntimeTask *)btlCreateMoveOtherUnitsTask((u8 *)unit, slot);
        btlStartTask(initialTask);
    } else {
        initialTask = btlCreateImmediateCompletionTask();
        btlStartTask(initialTask);
    }

    taskWork = (BtlRuntimeTask *)sndCreateSetStateTask();
    taskWork->startCondition.kind = 4;
    taskWork->startCondition.value.handle = initialTask->handle;
    btlStartTask(taskWork);

    taskWork = (BtlRuntimeTask *)btlCreateCommandSoundUpdateTask();
    taskWork->startCondition.kind = 4;
    taskWork->flags |= 2;
    taskWork->startCondition.value.handle = initialTask->handle;
    btlStartTask(taskWork);

    taskWork = (BtlRuntimeTask *)btlCreateSecondaryCommandSoundTask();
    taskWork->startCondition.kind = 4;
    taskWork->startCondition.value.handle = initialTask->handle;
    btlStartTask(taskWork);

    btlBuildActionLightParameters(skillId, &light);
    taskWork = (BtlRuntimeTask *)btlCreateEffectTaskWithSourceParams((u8 *)&light, 0xC);
    taskWork->startCondition.kind = 4;
    taskWork->startCondition.value.handle = initialTask->handle;
    btlStartTask(taskWork);

    resolvedAction = btlGetActionDefaultOrOverride(skillId);
    taskWork = (BtlRuntimeTask *)sndCreateAcquireTask((u32)resolvedAction, 0xC);
    taskWork->startCondition.kind = 4;
    taskWork->startCondition.value.handle = initialTask->handle;
    btlStartTask(taskWork);

    phase = actor->indexWork.phase;
    if (phase != 4) {
        taskWork = btlCreateEffObjD(unit, skillId);
        taskWork->startCondition.kind = 4;
        taskWork->startCondition.value.handle = initialTask->handle;
        taskWork->ownerId = actionSequence;
        btlStartTask(taskWork);
    } else {
        taskWork = btlCreateOwnerLinkedTimedTask(unit, actor->indexWork.reference);
        taskWork->startCondition.kind = 4;
        taskWork->startCondition.value.handle = initialTask->handle;
        taskWork->ownerId = actionSequence;
        btlStartTask(taskWork);
    }

    taskWork = (BtlRuntimeTask *)btlAllocateIndexedUnitEffectTask((u8 *)unit, slot,
        btlGetSlotRateKind((u8 *)unit, slot), 1.0f);
    taskWork->startCondition.kind = 4;
    taskWork->flags |= 2;
    taskWork->startCondition.value.handle = initialTask->handle;
    btlStartTask(taskWork);

    taskWork = (BtlRuntimeTask *)btlCreateTargetedCommandSoundTask((s32)actor, 5, (u32)skillId);
    taskWork->startCondition.kind = 4;
    taskWork->flags |= 2;
    taskWork->startCondition.value.handle = initialTask->handle;
    btlStartTask(taskWork);

    if (targetCount == 1) {
        targetUnit = btlGetIndexListEntry(indexList, 0);
        if (btlIsBattleRecordEligible(unit, targetUnit, slot, skillId) != 0) {
            taskWork = (BtlRuntimeTask *)btlAllocateApproachTargetTask((u8 *)unit,
                (s32)targetUnit, runtime->modelFrameScale);
            taskWork->startCondition.kind = 4;
            taskWork->startCondition.value.handle = initialTask->handle;
            taskWork->startDelay = targetCount;
            btlStartTask(taskWork);
        }
    }

    btlStartTask((BtlRuntimeTask *)btlCreateCategoryStatDamageTask((u8 *)unit, (u32)skillId, (u32)actor->indexWork.stageValue));
    if (actor->indexWork.phase == 4) {
        btlStartTask((BtlRuntimeTask *)btlCreateFlaggedCommandItemConsumeTask((s32)unit,
            actor->indexWork.reference));
    }

    animationLength = btlGetUnitMotionDurationFrames(unit, slot);
    if (animationLength < 0x14) {
        animationLength = 0x14;
    }
    btlBossDebugPrintf(D_003A3658, animationLength);

    btlFormatActionEventFilename(skillId, resourceName);
    if (runtime->actionResourceNameHook != NULL) {
        runtime->actionResourceNameHook(actor, skillId, resourceName);
    }

    effect = sndAllocResourceNode();
    unit->resourceNode = effect;
    taskWork = sndCreateEffectLoadTask(effect, resourceName);
    taskWork->startCondition.kind = 4;
    taskWork->startDelay = animationLength > 0 ? animationLength - 1 : 0;
    taskWork->startCondition.value.handle = initialTask->handle;
    taskWork->ownerId = effectLoadSequence;
    btlStartTask(taskWork);

    sndFormatResourceNameFromIndex(skillId, resourceName);
    listNode = sndAllocListNode();
    unit->listNode = listNode;
    taskWork = (BtlRuntimeTask *)sndCreateFileLoadTask((struct SoundLoadNode *)listNode,
        (u32)skillId, resourceName);
    taskWork->startCondition.kind = 4;
    taskWork->startCondition.value.handle = initialTask->handle;
    taskWork->ownerId = effectLoadSequence;
    btlStartTask(taskWork);

    resourceType = sndLookupResourceType((s32)unit, skillId);
    if (resourceType != 0) {
        s32 soundId;
        taskWork = sndCreateActorEffectTask(runtime->resources[resourceType], unit,
            (u32)(animationLength + 6));
        taskWork->startCondition.kind = 4;
        taskWork->startCondition.value.handle = initialTask->handle;
        btlStartTask(taskWork);

        soundId = sndMapResourceType((s32)unit, skillId);
        if (soundId != -1) {
            taskWork = (BtlRuntimeTask *)sndCreateStationedSeTask((u32)soundId);
            taskWork->startCondition.kind = 4;
            taskWork->startCondition.value.handle = initialTask->handle;
            btlStartTask(taskWork);
        }
    }

    if (actor->indexWork.stage == 3) {
        taskWork = btlCreateEffObjB(unit, actor->indexWork.parameter);
        taskWork->startCondition.kind = 7;
        taskWork->startCondition.value.handle = effectLoadSequence;
        if (actor->indexWork.costValidationFailed == 0) {
            taskWork->ownerId = actionSequence;
        }
        btlStartTask(taskWork);
    }

    if (datCommandRecords[skillId].unk30 == 2) {
        if ((actor->unit->status.flags & 0x200) == 0 && btlCanSpawnAnotherEnemy(actor->unit) != 0) {
            currentUnit = btlGetIndexListEntry(actor->indexWork.indices, 0);
            effectTask = btlCreateReferencedBattleEffectTask(unit->resourceNode, unit, currentUnit, 2);
            effectTask->startCondition.kind = 7;
            effectTask->startCondition.value.handle = effectLoadSequence;
            effectTask->ownerId = actionSequence;
            btlStartTask(effectTask);

            if (soundEnabled != 0) {
                taskWork = (BtlRuntimeTask *)sndCreateSkillSeTask((s32)unit->listNode, 2);
                taskWork->startCondition.kind = 7;
                taskWork->startCondition.value.handle = effectLoadSequence;
                taskWork->ownerId = actionSequence;
                btlStartTask(taskWork);
            }

            effectTask = btlCreateReferencedBattleEffectTask(unit->resourceNode, unit, currentUnit, 1);
            effectTask->startCondition.kind = 7;
            effectTask->startCondition.value.handle = effectLoadSequence;
            effectTask->endCondition.kind = 7;
            effectTask->endCondition.value.handle = actionSequence;
            effectTask->ownerId = btlAdvanceRuntimeSequenceCounter();
            btlStartTask(effectTask);

            taskWork = (BtlRuntimeTask *)sndCreateSkillSeTask((s32)unit->listNode, 1);
            taskWork->startCondition.kind = 7;
            taskWork->startCondition.value.handle = effectLoadSequence;
            taskWork->endCondition.kind = 7;
            taskWork->endCondition.value.handle = actionSequence;
            taskWork->ownerId = btlAdvanceRuntimeSequenceCounter();
            btlStartTask(taskWork);

            taskWork = btlCreateImmediateCompletionTask();
            taskWork->startCondition.kind = 7;
            taskWork->startCondition.value.handle = effectLoadSequence;
            taskWork->ownerId = actionSequence;
            btlStartTask(taskWork);
            taskWork = btlCreateImmediateCompletionTask();
            taskWork->startCondition.kind = 7;
            taskWork->startCondition.value.handle = effectLoadSequence;
            taskWork->ownerId = damageSequence;
            btlStartTask(taskWork);
        }
    } else if (actor->indexWork.costValidationFailed == 0) {
        {
            taskWork = (BtlRuntimeTask *)btlCreateTargetedCommandSoundTask((s32)actor, 6, (u32)skillId);
            taskWork->startCondition.kind = 7;
            taskWork->flags |= 2;
            taskWork->startCondition.value.handle = effectLoadSequence;
            btlStartTask(taskWork);
        }

        /* Command hooks use the releaseMode actor and genuine scheduler sequence IDs. */
        if (runtime->preActionHook != NULL) {
            runtime->preActionHook(actor, skillId, actionSequence, initialTask->handle, effectLoadSequence);
        }
        category = categoryEnabled ? btlResolveSkillCategory((s32)unit, skillId) : 0;
        modelPolicy = btlGetActionAnimationDelay(unit, skillId);
        reflectedEffectStarted = 0;
        groupDelay = 0;
        for (targetIndex = 0; targetIndex < targetCount; targetIndex++, group++) {
            currentUnit = unit;
            entryCount = group->count;
            targetUnit = btlGetIndexListEntry(indexList, targetIndex);
            if (!group->reflected) {
                currentUnit = targetUnit;
            }
            if (group->reflected || group->kind == 0x40000) {
                selector = ((BtlActionAnimationRecord *)datActionAnimationRecords)[skillId].effectKind;
            }
            if (selector == 1) {
                category = 0;
            }
            effectUnit = selector == 3 ? targetUnit : currentUnit;
            if (group->inactiveOrStatusChanged && (runtime->battleFlags & 0x80)) {
                btlBossDebugPrintf(D_003A3670, currentUnit);
                damageSequence = unit->identity;
            } else {
                damageSequence = btlAdvanceRuntimeSequenceCounter();
            }
            if (group->targetSpecialHit) {
                taskWork = btlCreateClearActorEntrySelectionTask(targetUnit);
                taskWork->startCondition.kind = 7;
                taskWork->startCondition.value.handle = damageSequence;
                btlStartTask(taskWork);
            }
            if (group->sourceSpecialHit) {
                taskWork = btlCreateClearActorEntrySelectionTask(currentUnit);
                taskWork->startCondition.kind = 7;
                taskWork->startCondition.value.handle = damageSequence;
                btlStartTask(taskWork);
            }
            if (!(group->reflected && reflectedEffectStarted) && selector != 1) {
                effectTask = btlCreateReferencedBattleEffectTask(unit->resourceNode, unit, effectUnit, 2);
                effectTask->startCondition.kind = 7;
                effectTask->startCondition.value.handle = effectLoadSequence;
                effectTask->startDelay = groupDelay;
                effectTask->ownerId = actionSequence;
                btlStartTask(effectTask);
                if (soundEnabled) {
                    taskWork = (BtlRuntimeTask *)sndCreateSkillSeTask((s32)unit->listNode, 2);
                    taskWork->startCondition.kind = 7;
                    taskWork->startCondition.value.handle = effectLoadSequence;
                    taskWork->startDelay = groupDelay;
                    taskWork->ownerId = actionSequence;
                    btlStartTask(taskWork);
                }
                if (group->reflected && selector != 3) {
                    reflectedEffectStarted = 1;
                }
            } else {
                effectTask = btlCreateImmediateCompletionTask();
                effectTask->startCondition.kind = 7;
                effectTask->startCondition.value.handle = effectLoadSequence;
                effectTask->startDelay = groupDelay;
                effectTask->ownerId = actionSequence;
                btlStartTask(effectTask);
            }
            entryDelay = 0;
            extraDelay = 0;
            for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
                BtlRuntimeTask *stageTask;
                operand = &group->entries[entryIndex];
                if (btlActionEntryIsEmpty(skillId, group, operand)) {
                    reaction = -1;
                    hitKind = 4;
                    hitParameter = 1;
                } else {
                    reaction = group->reactionCode;
                    hitKind = group->kind;
                    hitParameter = group->parameter;
                }
                specialModel = (u32)(selector - 3) < 2;
                initialTask = sndCreateTimedUnitEffectTask(unit->resourceNode,
                    currentUnit, 2, specialModel ? -1 : reaction, 0);
                initialTask->startCondition.kind = 5;
                initialTask->startDelay = entryDelay;
                initialTask->startCondition.value.handle = effectTask->handle;
                btlStartTask(initialTask);
                if (group->reflected && specialModel && entryIndex == entryCount - 1) {
                    if ((u32)modelPolicy >= 2 || !pendingModelBlend) {
                        if (targetCount == 1) {
                            taskWork = btlCreateActorModelBlendTask(currentUnit, targetUnit, reaction, 0, 1.0f);
                        } else {
                            taskWork = btlCreateActorModelBlendTask(currentUnit, NULL, reaction, 0, 1.0f);
                        }
                        taskWork->startCondition.kind = 4;
                        pendingModelBlend = 1;
                        taskWork->startCondition.value.handle = initialTask->handle;
                        btlStartTask(taskWork);
                    }
                }
                finalSequence = initialTask->handle;
                if (group->kind == 2 && entryIndex == 0) {
                    taskWork = (BtlRuntimeTask *)btlCreateActorPositionEffectTask((u8 *)currentUnit);
                    taskWork->startCondition.kind = 4;
                    taskWork->startCondition.value.handle = initialTask->handle;
                    btlStartTask(taskWork);
                }
                if (datCommandRecords[skillId].unk30 == 1 && entryIndex == 0) {
                    phaseTask = btlCreateEffectTask44(currentUnit);
                    phaseTask->startCondition.kind = 4;
                    phaseTask->startCondition.value.handle = initialTask->handle;
                    phaseTask->ownerId = unit->identity;
                    btlStartTask(phaseTask);
                    taskWork = btlCreateImmediateCompletionTask();
                    taskWork->startCondition.kind = 4;
                    taskWork->startCondition.value.handle = phaseTask->handle;
                    taskWork->ownerId = actionSequence;
                    btlStartTask(taskWork);
                }
                if (actor->indexWork.stage == 1 && targetIndex == 0 && entryIndex == 0) {
                    extraDelay += 12;
                    stageTask = btlCreateEffObjB(currentUnit, actor->indexWork.parameter);
                    stageTask->startCondition.kind = 4;
                    stageTask->startCondition.value.handle = initialTask->handle;
                    btlStartTask(stageTask);
                }
                if (actor->indexWork.stage == 5 && targetIndex == 0 && entryIndex == 0) {
                    extraDelay += 12;
                    stageTask = btlCreateEffObjB(unit, actor->indexWork.parameter);
                    stageTask->startCondition.kind = 4;
                    stageTask->startCondition.value.handle = initialTask->handle;
                    btlStartTask(stageTask);
                }
                if (actor->indexWork.huntEpReward && targetIndex == 0 && entryIndex == entryCount - 1) {
                    taskWork = btlCreateEffectTask3E(unit, (u16)actor->indexWork.huntEpReward);
                    taskWork->startCondition.kind = 4;
                    taskWork->startDelay = extraDelay + 12;
                    taskWork->startCondition.value.handle = initialTask->handle;
                    btlStartTask(taskWork);
                }
                if (actor->indexWork.flags && targetIndex == 0 && entryIndex == entryCount - 1) {
                    taskWork = btlCreateEffectWaitTask(unit, actor->indexWork.flags);
                    taskWork->startCondition.kind = 4;
                    taskWork->startCondition.value.handle = initialTask->handle;
                    btlStartTask(taskWork);
                }
                if ((targetUnit->status.flags & 0x10) && entryIndex == 0) {
                    taskWork = btlScheduleActorUpdate((u8 *)targetUnit);
                    taskWork->startCondition.kind = 4;
                    taskWork->startCondition.value.handle = initialTask->handle;
                    btlStartTask(taskWork);
                }
                if (targetIndex == targetCount - 1 && entryIndex == 0) {
                    taskWork = btlCreateImmediateCompletionTask();
                    taskWork->startCondition.kind = 4;
                    taskWork->startCondition.value.handle = initialTask->handle;
                    taskWork->startDelay = actor->indexWork.allTargetSideActorsSkipped == 0 ? 10 : 18;
                    taskWork->ownerId = unit->identity;
                    btlStartTask(taskWork);
                }
                taskWork = btlCreateActorParameterDeltaTask(currentUnit, operand);
                taskWork->startCondition.kind = 4;
                taskWork->startCondition.value.handle = initialTask->handle;
                taskWork->ownerId = damageSequence;
                btlStartTask(taskWork);
                taskWork = btlCreateDeferredActorStatsTask(unit, operand);
                taskWork->startCondition.kind = 4;
                taskWork->startDelay = category;
                taskWork->startCondition.value.handle = initialTask->handle;
                taskWork->ownerId = damageSequence;
                btlStartTask(taskWork);
                if (operand->deferredStatus) {
                    taskWork = btlCreateDeferredUnitStatusTask((u8 *)unit, operand->deferredStatus);
                    taskWork->startCondition.kind = 7;
                    taskWork->startCondition.value.handle = actionSequence;
                    taskWork->ownerId = damageSequence;
                    btlStartTask(taskWork);
                    if ((operand->deferredStatus & 0x40) && unit->partyRecord.status < 0x40) {
                        taskWork = btlCreateEffObjB(unit, 0xC6);
                        taskWork->startCondition.kind = 7;
                        taskWork->startCondition.value.handle = actionSequence;
                        taskWork->ownerId = damageSequence;
                        btlStartTask(taskWork);
                    }
                }
                taskWork = btlCreateMaskedActorEntryUpdateTask(currentUnit, operand);
                taskWork->startCondition.kind = 4;
                taskWork->startCondition.value.handle = initialTask->handle;
                taskWork->ownerId = damageSequence;
                btlStartTask(taskWork);
                taskWork = btlCreateQueuedActorEntrySelectionTask(currentUnit, operand);
                taskWork->startCondition.kind = 4;
                taskWork->startCondition.value.handle = initialTask->handle;
                taskWork->ownerId = damageSequence;
                btlStartTask(taskWork);
                if (targetIndex == targetCount - 1 && entryIndex == entryCount - 1) {
                    taskWork = (BtlRuntimeTask *)btlCreateActorSoundOptionTask(unit, actor->indexWork.huntEpReward);
                    taskWork->startCondition.kind = 4;
                    taskWork->startCondition.value.handle = initialTask->handle;
                    taskWork->ownerId = damageSequence;
                    btlStartTask(taskWork);
                }
                if (targetIndex == 0 && entryIndex == entryCount - 1) {
                    taskWork = (BtlRuntimeTask *)btlScheduleEpPacketTask((u8 *)unit, actor->indexWork.battleExperienceReward);
                    taskWork->startCondition.kind = 4;
                    taskWork->startCondition.value.handle = initialTask->handle;
                    taskWork->ownerId = damageSequence;
                    btlStartTask(taskWork);
                    taskWork = btlScheduleMoneyPacketTask((u8 *)unit, actor->indexWork.moneyReward);
                    taskWork->startCondition.kind = 4;
                    taskWork->startCondition.value.handle = initialTask->handle;
                    taskWork->ownerId = damageSequence;
                    btlStartTask(taskWork);
                }
                if (actor->indexWork.flags && targetIndex == 0 && entryIndex == entryCount - 1) {
                    taskWork = (BtlRuntimeTask *)btlCreateItemQuantityIncrementTask((u8 *)unit, actor->indexWork.flags);
                    taskWork->startCondition.kind = 4;
                    taskWork->startCondition.value.handle = initialTask->handle;
                    taskWork->ownerId = damageSequence;
                    btlStartTask(taskWork);
                }
                if (group->entries[entryIndex].hpDelta) {
                    taskWork = btlCreateLinkedEffectTask(currentUnit, group->entries[entryIndex].hpDelta, 0);
                    taskWork->startCondition.kind = 4;
                    taskWork->startDelay = 1;
                    taskWork->startCondition.value.handle = initialTask->handle;
                    taskWork->ownerId = spareSequence;
                    btlStartTask(taskWork);
                }
                if (group->entries[entryIndex].mpDelta) {
                    taskWork = btlCreateLinkedEffectTask(currentUnit, group->entries[entryIndex].mpDelta, 1);
                    taskWork->startCondition.kind = 4;
                    taskWork->startDelay = 1;
                    taskWork->startCondition.value.handle = initialTask->handle;
                    taskWork->ownerId = spareSequence;
                    btlStartTask(taskWork);
                }
                if (group->entries[entryIndex].hpRecovery) {
                    taskWork = btlCreateLinkedEffectTask(unit, group->entries[entryIndex].hpRecovery, 0);
                    taskWork->startCondition.kind = 4;
                    taskWork->startDelay = category;
                    taskWork->startCondition.value.handle = initialTask->handle;
                    taskWork->ownerId = spareSequence;
                    btlStartTask(taskWork);
                }
                if (group->entries[entryIndex].mpRecovery) {
                    taskWork = btlCreateLinkedEffectTask(unit, group->entries[entryIndex].mpRecovery, 1);
                    taskWork->startCondition.kind = 4;
                    taskWork->startDelay = category;
                    taskWork->startCondition.value.handle = initialTask->handle;
                    taskWork->ownerId = spareSequence;
                    btlStartTask(taskWork);
                }
                if (!(group->entries[entryIndex].flags & 0x1000)) {
                    if (hitKind == 0x10000 || hitKind == 4) {
                        if (hitKind == 0x10000) taskWork = btlCreateEffectCounterTask(currentUnit, 4);
                        else taskWork = btlCreateEffectCounterTask(currentUnit, 3);
                        taskWork->startCondition.kind = 4;
                        taskWork->startDelay = 1;
                        taskWork->startCondition.value.handle = initialTask->handle;
                        taskWork->ownerId = spareSequence;
                        btlStartTask(taskWork);
                    }
                    if (hitParameter == 2 || hitParameter == 4) {
                        if (hitParameter == 2) taskWork = btlCreateEffectCounterTask(currentUnit, 1);
                        else taskWork = btlCreateEffectCounterTask(currentUnit, 2);
                        taskWork->startCondition.kind = 4;
                        taskWork->startDelay = 1;
                        taskWork->startCondition.value.handle = initialTask->handle;
                        taskWork->ownerId = spareSequence;
                        btlStartTask(taskWork);
                    }
                }
                if ((group->entries[entryIndex].addedStatus & 1) && currentUnit->partyRecord.status == 0) {
                    sceneUnit = currentUnit;
                }
                entryDelay += group->interval;
            }
            if (runtime->postTargetHook != NULL) {
                runtime->postTargetHook(actor, skillId, targetUnit, effectLoadSequence, finalSequence, selector);
            }
            if (group->kind == 0x40000) {
                phaseTask = btlCreateReferencedBattleEffectTask(runtime->resources[0x1E],
                    unit, currentUnit, 2);
                switch (selector) {
                case 1:
                    phaseTask->startCondition.kind = 5;
                    phaseTask->startCondition.value.handle = effectTask->handle;
                    break;
                case 2:
                case 3:
                case 4:
                    phaseTask->startCondition.kind = 4;
                    phaseTask->startCondition.value.handle = finalSequence;
                    break;
                case 0: default:
                    phaseTask->startCondition.kind = 5;
                    phaseTask->startCondition.value.handle = effectTask->handle;
                    break;
                }
                phaseTask->ownerId = actionSequence;
                btlStartTask(phaseTask);
                taskWork = (BtlRuntimeTask *)sndCreateStationedSeTask(0x1000D);
                taskWork->startCondition.kind = 5;
                taskWork->startCondition.value.handle = phaseTask->handle;
                taskWork->ownerId = actionSequence;
                btlStartTask(taskWork);
            }
            if (group->reflected) {
                phaseTask = btlCreateReferencedBattleEffectTask(runtime->resources[0x1D],
                    unit, targetUnit, 2);
                switch (selector) {
                case 1:
                    phaseTask->startCondition.kind = 5;
                    phaseTask->startCondition.value.handle = effectTask->handle;
                    break;
                case 2:
                case 3:
                case 4:
                    phaseTask->startCondition.kind = 4;
                    phaseTask->startCondition.value.handle = finalSequence;
                    break;
                case 0: default:
                    phaseTask->startCondition.kind = 5;
                    phaseTask->startCondition.value.handle = effectTask->handle;
                    break;
                }
                phaseTask->ownerId = actionSequence;
                btlStartTask(phaseTask);
                taskWork = (BtlRuntimeTask *)sndCreateStationedSeTask(0x1000C);
                taskWork->startCondition.kind = 5;
                taskWork->startCondition.value.handle = phaseTask->handle;
                taskWork->ownerId = actionSequence;
                btlStartTask(taskWork);
            }
            groupDelay += modelPolicy;
        }
        if (selector != 1) {
            effectTask = btlCreateReferencedBattleEffectTask(unit->resourceNode, unit, effectUnit, 1);
            effectTask->startCondition.kind = 7;
            effectTask->startCondition.value.handle = effectLoadSequence;
            effectTask->endCondition.kind = 7;
            effectTask->endCondition.value.handle = actionSequence;
            effectTask->ownerId = btlAdvanceRuntimeSequenceCounter();
            btlStartTask(effectTask);

            taskWork = (BtlRuntimeTask *)sndCreateSkillSeTask((s32)unit->listNode, 1);
            taskWork->startCondition.kind = 7;
            taskWork->startCondition.value.handle = effectLoadSequence;
            taskWork->endCondition.kind = 7;
            taskWork->endCondition.value.handle = actionSequence;
            taskWork->ownerId = btlAdvanceRuntimeSequenceCounter();
            btlStartTask(taskWork);
        }

        if (trailingSound) {
            taskWork = (BtlRuntimeTask *)sndCreateSkillSeTask((s32)unit->listNode, 3);
            taskWork->startCondition.kind = 7;
            taskWork->startCondition.value.handle = effectLoadSequence;
            taskWork->endCondition.kind = 7;
            taskWork->endCondition.value.handle = actionSequence;
            taskWork->ownerId = btlAdvanceRuntimeSequenceCounter();
            btlStartTask(taskWork);
        }

        if (trailingEffect) {
            s32 rateKind = btlGetSlotRateKind((u8 *)unit, 0x1A);

            taskWork = (BtlRuntimeTask *)btlAllocateIndexedUnitEffectTask(
                (u8 *)unit, 0x1A, rateKind, 1.0f);
            taskWork->startCondition.kind = 7;
            taskWork->startDelay = 10;
            taskWork->startCondition.value.handle = effectLoadSequence;
            taskWork->endCondition.kind = 7;
            taskWork->endCondition.value.handle = actionSequence;
            taskWork->ownerId = btlAdvanceRuntimeSequenceCounter();
            btlStartTask(taskWork);
        }

        if (actor->indexWork.refreshEligibleActors != 0) {
            taskWork = (BtlRuntimeTask *)btlCreateRefreshEligibleActorsTask();
            taskWork->startCondition.kind = 7;
            taskWork->startCondition.value.handle = damageSequence;
            btlStartTask(taskWork);
        }
    }

    sceneGroupTask = (BtlRuntimeTask *)fldCreateSceneGroupAction(
        (u8 *)actor, actor->indexWork.adjustedValue,
        (u8)actor->indexWork.resultKind);
    switch (selector) {
    case 1:
        sceneGroupTask->startCondition.kind = 7;
        sceneGroupTask->startCondition.value.handle = effectLoadSequence;
        break;
    case -1: case 2: case 3: case 4:
        sceneGroupTask->startCondition.kind = 4;
        sceneGroupTask->startCondition.value.handle = finalSequence;
        break;
    case 0: default:
        sceneGroupTask->startCondition.kind = 7;
        sceneGroupTask->startCondition.value.handle = effectLoadSequence;
        break;
    }
    sceneGroupTask->ownerId = unit->identity;
    btlStartTask(sceneGroupTask);

    if (datCommandRecords[skillId].unk30 == 9 && actor->indexWork.costValidationFailed == 0) {
        taskWork = (BtlRuntimeTask *)fldCreateSceneActorAction(
            (u8 *)actor, (s8)datCommandRecords[skillId].entryChange);
        taskWork->startCondition.kind = 4;
        taskWork->startDelay = 16;
        taskWork->startCondition.value.handle = sceneGroupTask->handle;
        taskWork->ownerId = unit->identity;
        btlStartTask(taskWork);
    }

    if (sceneUnit != 0 && actor->indexWork.wait >= 0 &&
        (actor->flags & 0x20) == 0) {
        currentUnit = sceneUnit;
        taskWork = btlCreateEffObjC(currentUnit, actor->indexWork.wait);
        taskWork->startCondition.kind = 7;
        taskWork->startCondition.value.handle = actionSequence;
        btlStartTask(taskWork);
    }

    taskWork = (BtlRuntimeTask *)btlCreateUpdateUnitEffectsTask();
    taskWork->startCondition.kind = 7;
    taskWork->startCondition.value.handle = damageSequence;
    taskWork->startDelay = 30;
    btlStartTask(taskWork);

    taskWork = (BtlRuntimeTask *)sndCreateReleaseTask(0xC);
    taskWork->startCondition.kind = 7;
    taskWork->startCondition.value.handle = actionSequence;
    btlStartTask(taskWork);

    taskWork = btlCreateSceneLightingRestoreTask(0xC);
    taskWork->startCondition.kind = 7;
    taskWork->startCondition.value.handle = actionSequence;
    btlStartTask(taskWork);

    taskWork = (BtlRuntimeTask *)btlCreateWaitUnitListIdleTask(0xC);
    taskWork->startCondition.kind = 7;
    taskWork->startCondition.value.handle = actionSequence;
    btlStartTask(taskWork);

    taskWork = (BtlRuntimeTask *)btlCreateApplyToActiveActorsTask(0xC);
    taskWork->startCondition.kind = 7;
    taskWork->startCondition.value.handle = actionSequence;
    btlStartTask(taskWork);

    taskWork = (BtlRuntimeTask *)sndCreateClearStateTask();
    taskWork->startCondition.kind = 7;
    taskWork->startCondition.value.handle = actionSequence;
    btlStartTask(taskWork);

    taskWork = (BtlRuntimeTask *)btlCreateFadeStateResetTask();
    taskWork->startCondition.kind = 7;
    taskWork->startCondition.value.handle = actionSequence;
    btlStartTask(taskWork);

    if (datCommandRecords[skillId].unk30 == 2) {
        btlDispatchStateHandler(actor, 0x15);
    } else if ((actor->unit->partyRecord.status & 0x480) != 0) {
        btlDispatchStateHandler(actor, 0x18);
    } else {
        btlDispatchStateHandler(actor, 0x1A);
    }

}
#else
INCLUDE_ASM(const s32, "game/code_001C8890", func_001CB410);
#endif

/* Empty entry callback for the retail LINKAGE action state. */
void btlEnterLinkageActionState(void) {
}

extern char D_003A36A8[];
extern BtlRuntimeTask *btlCreateSceneLightingRestoreTask(u32);
extern u64 btlAdvanceRuntimeSequenceCounter(void);
extern BtlRuntimeTask *sndCreateTimedUnitEffectTask(
    SoundResourceNode *, BtlUnit *, EffBattleVoiceKind, s32, u32);

void btlBuildActionLightParameters(s32 skillIndex, BtlSceneLightParams *light);
extern BtlRuntimeTask *btlCreateEffectTaskWithSourceParams(const void *, u32);
s32 btlGetActionDefaultOrOverride(s32 skillIndex);
extern BtlRuntimeTask *btlCreateLinkageFormationTask(BtlTask *, BtlUnit *,
        BtlUnit *, u32, u32);
extern BtlRuntimeTask *btlCreateCommandSoundWithArguments(BtlTask *, BtlUnit *, BtlUnit *, BtlCameraModeIndex cameraMode, BtlActionCode actionCode);
s32 sndLookupResourceType(BtlUnit *actor, s32 resourceIndex);
BtlRuntimeTask *sndCreateActorEffectTask(SoundResourceNode *source,
        BtlUnit *owner, u32 duration);
s32 sndMapResourceType(BtlUnit *actor, s32 resourceIndex);
extern BtlRuntimeTask *btlCreateCategoryStatDamageTask(BtlUnit *, s32, s32);
extern BtlRuntimeTask *btlCreateFlaggedCommandItemConsumeTask(BtlUnit *, s32);
s32 btlFormatActionEventFilename(s32 skillIndex, char *output);
extern BtlRuntimeTask *sndCreateEffectLoadTask(SoundResourceNode *, const char *);
extern BtlRuntimeTask *sndCreateEffectWithTargets(SoundResourceNode *, BtlUnit *, s32, s32, BtlUnit *, EffBattleVoiceKind);
extern BtlRuntimeTask *sndCreateSkillSeTask(const ActiveSoundNode *, u16);
extern BtlRuntimeTask *btlCreateMaskedActorEntryUpdateTask(BtlUnit *, BtlOperandEntry *);
extern BtlRuntimeTask *btlCreateQueuedActorEntrySelectionTask(BtlUnit *, BtlOperandEntry *);


/* Schedule the LINKAGE action task chain (inferred role). */
INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A36A8);

void btlScheduleLinkageActionTasks(BtlTask *action) {
    BtlUnit *actors[3];
    BtlSceneLightParams light;
    s32 actorFrames[3];
    char resourceName[0x70];
    BtlState *state;
    BtlUnit *unit = action->unit;
    BtlUnit *companionA = action->indexWork.companionA;
    BtlUnit *companionB = action->indexWork.companionB;
    BtlUnit *indexedTarget;
    BtlUnit *currentTarget;
    BtlOperandGroup *groups;
    BtlUnit *effectTarget = unit;
    BtlUnit *resultTarget = NULL;
    BtlIndexList *indices;
    BtlRuntimeTask *task;
    BtlRuntimeTask *groupTask;
    u64 masterOwner, actionOwner, operandOwner, commandOwner, soundHandle, latestOperandHandle;
    s32 slot, maxFrames;
    s32 reflectedStarted, blendStarted;
    u32 groupIndex, operandIndex, targetCount, operandCount;
    s32 operandDelay, commandMap, skill, groupDelay, actionFrames;
    s32 categoryKind, extraKind, effectFrameOffset, mode;
    actors[0] = unit;
    actors[1] = companionA;
    actors[2] = companionB;
    if ((unit->status.flags & 0x1000) != 0) {
        if (companionA != NULL && (companionA->status.flags & 0x1000) == 0) {
            actors[0] = companionA;
            actors[1] = unit;
        } else if (companionB != NULL && (companionB->status.flags & 0x1000) == 0) {
            actors[0] = companionB;
            actors[1] = unit;
            actors[2] = companionA;
        }
    }
    state = (BtlState *)btlGetRuntime();
    indices = action->indexWork.indices;
    targetCount = btlGetIndexListCount(indices);
    maxFrames = 0;
    mode = -1;
    groups = action->indexWork.groups;
    slot = action->indexWork.slot;
    skill = action->indexWork.skillId;
    masterOwner = btlAdvanceRuntimeSequenceCounter();
    actionOwner = btlAdvanceRuntimeSequenceCounter();
    commandOwner = btlAdvanceRuntimeSequenceCounter();
    operandOwner = btlAdvanceRuntimeSequenceCounter();
    soundHandle = btlAdvanceRuntimeSequenceCounter();
    latestOperandHandle = btlAdvanceRuntimeSequenceCounter();
    btlStartTask(sndCreateSetStateTask());
    for (groupIndex = 0; groupIndex < 3; groupIndex++) {
        if (actors[groupIndex] != NULL) {
            actorFrames[groupIndex] = btlGetUnitMotionDurationFrames(actors[groupIndex], slot);
            if (actorFrames[groupIndex] < 20) actorFrames[groupIndex] = 20;
            if (maxFrames < actorFrames[groupIndex]) maxFrames = actorFrames[groupIndex];
        }
    }
    btlBossDebugPrintf(D_003A36A8, maxFrames);
    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    btlBuildActionLightParameters(skill, &light);
    btlStartTask(btlCreateEffectTaskWithSourceParams(&light, 12));
    btlStartTask(sndCreateAcquireTask(btlGetActionDefaultOrOverride(skill), 12));
    if ((unit->status.flags & 0x400) != 0) {
        btlStartTask(btlCreateEffObjB(unit, 0xC9));
        task = btlCreateEffObjD(unit, skill);
        task->startDelay = action->indexWork.costValidationFailed == 0 ? 24 : 12;
        task->ownerId = actionOwner;
        btlStartTask(task);
    } else {
        task = btlCreateEffObjD(unit, skill);
        task->ownerId = actionOwner;
        btlStartTask(task);
    }
    btlStartTask(btlCreateLinkageFormationTask(action, companionA, companionB, skill, 0));
    btlStartTask(btlCreateCommandSoundWithArguments(action, companionA, companionB, 7, skill));
    for (groupIndex = 0; groupIndex < 3; groupIndex++) {
        if (actors[groupIndex] != NULL) {
            s32 resourceIndex;
            if (action->indexWork.costValidationFailed == 0) {
                BtlRuntimeTask *soundTask = btlCreateMoveOtherUnitsTask(actors[groupIndex], slot);
                soundTask->startDelay = maxFrames - actorFrames[groupIndex];
                btlStartTask(soundTask);
                soundHandle = soundTask->handle;
            }
            task = btlAllocateIndexedUnitEffectTask(actors[groupIndex], slot,
                btlGetSlotRateKind(actors[groupIndex], slot), 1.0f);
            task->startCondition.kind = 4;
            task->startCondition.value.handle = soundHandle;
            btlStartTask(task);
            resourceIndex = sndLookupResourceType(actors[groupIndex], skill);
            if (resourceIndex != 0) {
                btlStartTask(sndCreateActorEffectTask(state->resources[resourceIndex], actors[groupIndex], maxFrames + 6));
                if (groupIndex == 0) {
                    s32 sound = sndMapResourceType(actors[0], skill);
                    if (sound != -1) btlStartTask(sndCreateStationedSeTask(sound));
                }
            }
            btlStartTask(btlCreateCategoryStatDamageTask(actors[groupIndex], skill, action->indexWork.stageValue));
            if (action->indexWork.phase == 4) {
                btlStartTask(btlCreateFlaggedCommandItemConsumeTask(actors[groupIndex], action->indexWork.reference));
            }
        }
    }
    btlFormatActionEventFilename(skill, resourceName);
    unit->resourceNode = sndAllocResourceNode();
    task = sndCreateEffectLoadTask(unit->resourceNode, resourceName);
    task->startCondition.kind = 4;
    task->startDelay = maxFrames > 0 ? maxFrames - 1 : 0;
    task->startCondition.value.handle = soundHandle;
    task->ownerId = masterOwner;
    btlStartTask(task);
    sndFormatResourceNameFromIndex(skill, resourceName);
    unit->listNode = sndAllocListNode();
    task = sndCreateFileLoadTask(unit->listNode, skill, resourceName);
    task->startCondition.kind = 4;
    task->startCondition.value.handle = soundHandle;
    task->ownerId = masterOwner;
    btlStartTask(task);
    if (action->indexWork.stage == 3) {
        task = btlCreateEffObjB(unit, action->indexWork.parameter);
        task->startCondition.kind = 7;
        task->startCondition.value.handle = masterOwner;
        if (action->indexWork.costValidationFailed == 0) task->ownerId = actionOwner;
        btlStartTask(task);
    }
    if (action->indexWork.costValidationFailed == 0) {
        task = btlCreateCommandSoundWithArguments(action, companionA, companionB, 8, skill);
        task->startCondition.kind = 7;
        task->startCondition.value.handle = masterOwner;
        task->flags |= 2;
        if (state->commandSoundDelay != NULL) task->startDelay = state->commandSoundDelay(skill);
        btlStartTask(task);
        if (state->preActionHook != NULL) state->preActionHook(action, skill, actionOwner, soundHandle, masterOwner);
        actionFrames = btlGetActionAnimationDelay(unit, skill);
        commandMap = btlResolveSkillCategory((s32)unit, skill);
        blendStarted = 0;
        reflectedStarted = 0;
        groupDelay = 0;
        for (groupIndex = 0; groupIndex < targetCount; groupIndex++, groups++) {
            BtlOperandGroup *group = groups;
            currentTarget = unit;
            operandCount = group->count;
            indexedTarget = (BtlUnit *)btlGetIndexListEntry(indices, groupIndex);
            if (group->reflected == 0) currentTarget = indexedTarget;
            if (group->reflected != 0 || group->kind == 0x40000)
                mode = datActionAnimationRecords[skill].effectKind;
            effectTarget = mode == BTL_ACTION_EFFECT_KIND_TARGET_EFFECT_WITH_ACTOR_OWNER
                ? indexedTarget : currentTarget;
            if (group->inactiveOrStatusChanged != 0 && (state->battleFlags & 0x80) != 0) {
                btlBossDebugPrintf(D_003A3670, currentTarget);
                commandOwner = unit->identity;
            } else commandOwner = btlAdvanceRuntimeSequenceCounter();
            if (group->targetSpecialHit != 0) {
                task = btlCreateClearActorEntrySelectionTask(indexedTarget);
                task->startCondition.kind = 7;
                task->startCondition.value.handle = commandOwner;
                btlStartTask(task);
            }
            if (group->sourceSpecialHit != 0) {
                task = btlCreateClearActorEntrySelectionTask(currentTarget);
                task->startCondition.kind = 7;
                task->startCondition.value.handle = commandOwner;
                btlStartTask(task);
            }
            if ((group->reflected == 0 || reflectedStarted == 0) && mode != 1) {
                groupTask = sndCreateEffectWithTargets(unit->resourceNode, actors[0], (s32)actors[1], (s32)actors[2], effectTarget, 2);
                groupTask->startCondition.kind = 7;
                groupTask->startCondition.value.handle = masterOwner;
                groupTask->startDelay = groupDelay;
                groupTask->ownerId = actionOwner;
                btlStartTask(groupTask);
                task = (BtlRuntimeTask *)sndCreateSkillSeTask(unit->listNode, 2);
                task->startCondition.kind = 7;
                task->startCondition.value.handle = masterOwner;
                task->startDelay = groupDelay;
                task->ownerId = actionOwner;
                btlStartTask(task);
                if (group->reflected != 0 && mode != 3) reflectedStarted = 1;
            } else {
                groupTask = btlCreateImmediateCompletionTask();
                groupTask->startCondition.kind = 7;
                groupTask->startCondition.value.handle = masterOwner;
                groupTask->startDelay = groupDelay;
                groupTask->ownerId = actionOwner;
                btlStartTask(groupTask);
            }
            for (operandIndex = 0, operandDelay = 0, effectFrameOffset = 0; operandIndex < operandCount; operandIndex++) {
                BtlActionReactionMotionIndex selection;
                BtlRuntimeTask *operandTask;
                if (btlActionEntryIsEmpty(skill, group, &group->entries[operandIndex]) != 0) {
                    selection = BTL_ACTION_REACTION_MOTION_NONE;
                    categoryKind = 4;
                    extraKind = 1;
                } else {
                    selection = group->reactionCode;
                    categoryKind = group->kind;
                    extraKind = group->parameter;
                }
                operandTask = sndCreateTimedUnitEffectTask(unit->resourceNode, currentTarget, 2, (u32)(mode - 3) < 2 ? -1 : selection, 0);
                operandTask->startCondition.kind = 5;
                operandTask->startDelay = operandDelay;
                operandTask->startCondition.value.handle = groupTask->handle;
                btlStartTask(operandTask);
                if (group->reflected != 0 && (u32)(mode - 3) < 2 && operandIndex == operandCount - 1 && ((u32)actionFrames >= 2 || blendStarted == 0)) {
                    if (targetCount == 1) {
                        task = btlCreateActorModelBlendTask(currentTarget, indexedTarget, selection, 0, 1.0f);
                    } else {
                        task = btlCreateActorModelBlendTask(currentTarget, 0, selection, 0, 1.0f);
                    }
                    task->startCondition.kind = 4;
                    blendStarted = 1;
                    task->startCondition.value.handle = operandTask->handle;
                    btlStartTask(task);
                }
                latestOperandHandle = operandTask->handle;
                if (group->kind == 2 && operandIndex == 0) {
                    task = btlCreateActorPositionEffectTask(currentTarget);
                    task->startCondition.kind = 4;
                    task->startCondition.value.handle = operandTask->handle;
                    btlStartTask(task);
                }
                if (action->indexWork.stage == 1 && groupIndex == 0 && operandIndex == 0) {
                    BtlRuntimeTask *stageTask;
                    effectFrameOffset += 0xC;
                    stageTask = btlCreateEffObjB(unit, action->indexWork.parameter);
                    stageTask->startCondition.kind = 4;
                    stageTask->startCondition.value.handle = operandTask->handle;
                    btlStartTask(stageTask);
                }
                if (action->indexWork.huntEpReward != 0 && groupIndex == 0 && operandIndex == operandCount - 1) {
                    task = btlCreateEffectTask3E(unit, (u16)action->indexWork.huntEpReward);
                    task->startCondition.kind = 4;
                    task->startDelay = effectFrameOffset + 0xC;
                    task->startCondition.value.handle = operandTask->handle;
                    btlStartTask(task);
                }
                if ((indexedTarget->status.flags & 0x10) != 0 && operandIndex == 0) {
                    task = btlScheduleActorUpdate(indexedTarget);
                    task->startCondition.kind = 4;
                    task->startCondition.value.handle = operandTask->handle;
                    btlStartTask(task);
                }
                if (groupIndex == targetCount - 1 && operandIndex == 0) {
                    task = btlCreateImmediateCompletionTask();
                    task->startCondition.kind = 4;
                    task->startCondition.value.handle = operandTask->handle;
                    task->startDelay = action->indexWork.allTargetSideActorsSkipped == 0 ? 10 : 18;
                    task->ownerId = unit->identity;
                    btlStartTask(task);
                }
                task = btlCreateActorParameterDeltaTask(currentTarget, &group->entries[operandIndex]);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = commandOwner;
                btlStartTask(task);
                task = btlCreateDeferredActorStatsTask(unit, &group->entries[operandIndex]);
                task->startCondition.kind = 4;
                task->startDelay = commandMap;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = commandOwner;
                btlStartTask(task);

                task = btlCreateMaskedActorEntryUpdateTask(currentTarget, &group->entries[operandIndex]);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = commandOwner;
                btlStartTask(task);
                task = btlCreateQueuedActorEntrySelectionTask(currentTarget, &group->entries[operandIndex]);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = commandOwner;
                btlStartTask(task);
                if (groupIndex == targetCount - 1 && operandIndex == 0) {
                    task = btlCreateActorSoundOptionTask(unit, action->indexWork.huntEpReward);
                    task->startCondition.kind = 4;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = commandOwner;
                    btlStartTask(task);
                }
                if (groupIndex == 0 && operandIndex == operandCount - 1) {
                    task = btlScheduleEpPacketTask(unit, action->indexWork.battleExperienceReward);
                    task->startCondition.kind = 4;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = commandOwner;
                    btlStartTask(task);
                    task = btlScheduleMoneyPacketTask(unit, action->indexWork.moneyReward);
                    task->startCondition.kind = 4;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = commandOwner;
                    btlStartTask(task);
                }

                if (group->entries[operandIndex].hpDelta != 0) {
                    task = btlCreateLinkedEffectTask(currentTarget, group->entries[operandIndex].hpDelta, 0);
                    task->startCondition.kind = 4;
                    task->startDelay = 1;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = operandOwner;
                    btlStartTask(task);
                }
                if (group->entries[operandIndex].mpDelta != 0) {
                    task = btlCreateLinkedEffectTask(currentTarget, group->entries[operandIndex].mpDelta, 1);
                    task->startCondition.kind = 4;
                    task->startDelay = 1;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = operandOwner;
                    btlStartTask(task);
                }
                if (group->entries[operandIndex].hpRecovery != 0) {
                    task = btlCreateLinkedEffectTask(unit, group->entries[operandIndex].hpRecovery, 0);
                    task->startCondition.kind = 4;
                    task->startDelay = commandMap;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = operandOwner;
                    btlStartTask(task);
                }
                if (group->entries[operandIndex].mpRecovery != 0) {
                    task = btlCreateLinkedEffectTask(unit, group->entries[operandIndex].mpRecovery, 1);
                    task->startCondition.kind = 4;
                    task->startDelay = commandMap;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = operandOwner;
                    btlStartTask(task);
                }

                if ((group->entries[operandIndex].flags & 0x1000) == 0) {
                    if (categoryKind == 0x10000 || categoryKind == 4) {
                        if (categoryKind == 0x10000) task = btlCreateEffectCounterTask(currentTarget, 4);
                        else task = btlCreateEffectCounterTask(currentTarget, 3);
                        task->startCondition.kind = 4;
                        task->startDelay = 1;
                        task->startCondition.value.handle = operandTask->handle;
                        task->ownerId = operandOwner;
                        btlStartTask(task);
                    }
                    if (extraKind == 2 || extraKind == 4) {
                        if (extraKind == 2) task = btlCreateEffectCounterTask(currentTarget, 1);
                        else task = btlCreateEffectCounterTask(currentTarget, 2);
                        task->startCondition.kind = 4;
                        task->startDelay = 1;
                        task->startCondition.value.handle = operandTask->handle;
                        task->ownerId = operandOwner;
                        btlStartTask(task);
                    }
                }
                if ((group->entries[operandIndex].addedStatus & 1) != 0 && currentTarget->partyRecord.status == 0) {
                    resultTarget = currentTarget;
                }
                operandDelay += group->interval;
            }
            if (state->postTargetHook != NULL) state->postTargetHook(action, skill, indexedTarget, masterOwner, latestOperandHandle, mode);
            if (group->kind == 0x40000) {
                BtlRuntimeTask *effect = sndCreateEffectWithTargets(state->resources[30], actors[0], (s32)actors[1], (s32)actors[2], currentTarget, 2);
                switch (mode) {
                case 1:
                    effect->startCondition.kind = 5;
                    effect->startCondition.value.handle = groupTask->handle;
                    break;
                case 2: case 3: case 4:
                    effect->startCondition.kind = 4;
                    effect->startCondition.value.handle = latestOperandHandle;
                    break;
                case 0: default:
                    effect->startCondition.kind = 5;
                    effect->startCondition.value.handle = groupTask->handle;
                    break;
                }
                effect->ownerId = actionOwner;
                btlStartTask(effect);
                task = sndCreateStationedSeTask(0x1000D);
                task->startCondition.kind = 5;
                task->startCondition.value.handle = effect->handle;
                task->ownerId = actionOwner;
                btlStartTask(task);
            }
            if (group->reflected != 0) {
                BtlRuntimeTask *effect = sndCreateEffectWithTargets(state->resources[29], actors[0], (s32)actors[1], (s32)actors[2], indexedTarget, 2);
                switch (mode) {
                case 1:
                    effect->startCondition.kind = 5;
                    effect->startCondition.value.handle = groupTask->handle;
                    break;
                case 2: case 3: case 4:
                    effect->startCondition.kind = 4;
                    effect->startCondition.value.handle = latestOperandHandle;
                    break;
                case 0: default:
                    effect->startCondition.kind = 5;
                    effect->startCondition.value.handle = groupTask->handle;
                    break;
                }
                effect->ownerId = actionOwner;
                btlStartTask(effect);
                task = sndCreateStationedSeTask(0x1000C);
                task->startCondition.kind = 5;
                task->startCondition.value.handle = effect->handle;
                task->ownerId = actionOwner;
                btlStartTask(task);
            }
            groupDelay += actionFrames;
        }
        if (mode != 1) {
            groupTask = sndCreateEffectWithTargets(unit->resourceNode, actors[0], (s32)actors[1], (s32)actors[2], effectTarget, 1);
            groupTask->startCondition.kind = 7;
            groupTask->startCondition.value.handle = masterOwner;
            groupTask->endCondition.kind = 7;
            groupTask->endCondition.value.handle = actionOwner;
            groupTask->ownerId = btlAdvanceRuntimeSequenceCounter();
            btlStartTask(groupTask);
            task = (BtlRuntimeTask *)sndCreateSkillSeTask(unit->listNode, 1);
            task->startCondition.kind = 7;
            task->startCondition.value.handle = masterOwner;
            task->endCondition.kind = 7;
            task->endCondition.value.handle = actionOwner;
            task->ownerId = btlAdvanceRuntimeSequenceCounter();
            btlStartTask(task);
        }
        if (action->indexWork.refreshEligibleActors != 0) {
            task = btlCreateRefreshEligibleActorsTask();
            task->startCondition.kind = 7;
            task->startCondition.value.handle = commandOwner;
            btlStartTask(task);
        }
    }
    task = (BtlRuntimeTask *)fldCreateSceneGroupAction((u8 *)action, action->indexWork.adjustedValue, (u8)action->indexWork.resultKind);
    switch (mode) {
    case 1:
        task->startCondition.kind = 7;
        task->startCondition.value.handle = masterOwner;
        break;
    case -1: case 2: case 3: case 4:
        task->startCondition.kind = 4;
        task->startCondition.value.handle = latestOperandHandle;
        break;
    case 0: default:
        task->startCondition.kind = 7;
        task->startCondition.value.handle = masterOwner;
        break;
    }
    task->ownerId = unit->identity;
    btlStartTask(task);
    if (resultTarget != NULL && action->indexWork.wait >= 0 && (action->flags & 0x20) == 0) {
        currentTarget = resultTarget;
        task = btlCreateEffObjC(currentTarget, action->indexWork.wait);
        task->startCondition.kind = 7;
        task->startCondition.value.handle = actionOwner;
        btlStartTask(task);
    }
    task = (BtlRuntimeTask *)btlCreateUpdateUnitEffectsTask();
    task->startCondition.kind = 7;
    task->startCondition.value.handle = commandOwner;
    task->startDelay = 30;
    btlStartTask(task);
    task = sndCreateReleaseTask(12);
    task->startCondition.kind = 7;
    task->startCondition.value.handle = actionOwner;
    btlStartTask(task);
    task = btlCreateSceneLightingRestoreTask(12);
    task->startCondition.kind = 7;
    task->startCondition.value.handle = actionOwner;
    btlStartTask(task);
    task = btlCreateWaitUnitListIdleTask(12);
    task->startCondition.kind = 7;
    task->startCondition.value.handle = actionOwner;
    btlStartTask(task);
    task = btlCreateApplyToActiveActorsTask(12);
    task->startCondition.kind = 7;
    task->startCondition.value.handle = actionOwner;
    btlStartTask(task);
    task = (BtlRuntimeTask *)sndCreateClearStateTask();
    task->startCondition.kind = 7;
    task->startCondition.value.handle = actionOwner;
    btlStartTask(task);
    task = (BtlRuntimeTask *)btlCreateFadeStateResetTask();
    task->startCondition.kind = 7;
    task->startCondition.value.handle = actionOwner;
    btlStartTask(task);
    if ((action->unit->partyRecord.status & 0x480) != 0) {
        btlDispatchStateHandler(action, 0x18);
        return;
    }
    btlDispatchStateHandler(action, 0x1A);
}

void btlMarkLinkedActorStatusFlag(s32 task) {
    ((BtlTask *)task)->unit->status.flags = (u32)((BtlTask *)task)->unit->status.flags | 0x4000
    ;
}

extern BtlRuntimeTask *btlCreateGunLoadPollTask(BtlUnit *);
extern BtlRuntimeTask *btlCreateModelChangeTask(BtlUnit *unit, s32 resourceKind, s32 resourceId, s32 delay, s32 duration, u8 transitionMode);

/* State-handler table entry (0x359BEC); the table holds s32 handlers (btlCommandStateSelectB) and this one
 * returns without a value, as retail's missing sibling calls show (DDS1 twin of dds2 func_001DA210).
 * Gun-change command (slot 0x11): once no blocking tasks remain, swap the unit to its gun model, refresh linked
 * allies and reload their models, start the gun effects, and continue to state 0x18/0x1A; a unit already
 * holding the gun only restores its motion, swaps back and continues to 0x1B/0x1A. */
s32 btlCommandGunChangeStart(BtlTask *task) {
    BtlUnit *unit;
    BtlState *state;
    BtlUnit *other;
    BtlRuntimeTask *sound;
    BtlRuntimeTask *change;
    BtlRuntimeTask *spawned;
    BtlRuntimeTask *load;
    u16 *status;
    s32 motion;

    if (btlCountTasksByKind(0x1A) != 0 || btlCountTasksByKind(0x18) != 0 || btlCountTasksByKind(0x23) != 0) {
        return;
    }
    state = (BtlState *)btlGetRuntime();
    unit = task->unit;
    status = (u16 *)btlGetActorResourceBankEntry(unit->resourceKind, unit->species);
    if (!(unit->status.flags & 0x20)) {
        motion = btlGetUnitMotionDurationFrames(unit, 0x11) + 0x14;
    } else {
        motion = status[0x15];
    }
    if (!(unit->status.flags & 0x400020)) {
        unit->status.flags &= ~0x1000;
        unit->partyRecord.flags &= ~0x1000;
        sound = btlCreateMoveOtherUnitsTask(unit, 0x11);
        btlStartTask(sound);
        for (other = state->units; other != NULL; other = other->next) {
            if (other != unit && (btlUnitStatusPair(other) & 0x202) == 0x202) {
                spawned = btlScheduleRefreshTask(other);
                spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                spawned->startDelay = 1;
                spawned->startCondition.value.handle = sound->handle;
                spawned->ownerId = unit->identity;
                btlStartTask(spawned);
            }
        }
        change = btlCreateModelChangeTask(unit, unit->modelId, unit->modelVariant, motion, 0x18,
                                                       0);
        change->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        change->startCondition.value.handle = sound->handle;
        btlStartTask(change);
        for (other = state->units; other != NULL; other = other->next) {
            if (other != unit && (btlUnitStatusPair(other) & 0x202) == 0x202) {
                load = btlCreateModelLoadPollTask(other, other->resourceKind, other->species, 0);
                load->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                load->startCondition.value.handle = change->handle;
                load->ownerId = unit->identity;
                btlStartTask(load);
                spawned = btlCreateUnitFadeInTask(other, 0, 0);
                spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                spawned->startCondition.value.handle = load->handle;
                spawned->ownerId = unit->identity;
                btlStartTask(spawned);
            }
        }
        btlStartTask(btlCreateGunLoadPollTask(unit));
        btlStartTask(sndCreateEffectSourceTask(state->resources[45], unit, change->handle));
        btlStartTask(sndCreateStationedSeTask(0x1000F));
        spawned = (BtlRuntimeTask *)btlCreateEffObjA(unit, task->indexWork.phase);
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlCreateCommandSoundUpdateTask();
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlCreateSecondaryCommandSoundTask();
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlCreateCommandSoundTask(task, 0xE);
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlAllocateIndexedUnitEffectTask(unit, 0x11,
                                                                btlGetSlotRateKind(unit, 0x11), 1.0f);
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (BtlRuntimeTask *)fldCreateSceneGroupAction((u8 *)task, 0x64, 1);
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        spawned->ownerId = unit->identity;
        btlStartTask(spawned);
        if (task->unit->partyRecord.status & 0x480) {
            btlDispatchStateHandler(task, 0x18);
        } else {
            btlDispatchStateHandler(task, 0x1A);
        }
    } else {
        if ((unit->partyRecord.status & 0x7FFF) != 0x4000) {
            unit->status.flags &= ~0x20;
            if (unit->partyRecord.hp == 0) {
                unit->partyRecord.hp = 1;
                btlRefreshUnitMotionSelection(unit);
            }
        }
        if (task->indexWork.stage == 4) {
            if (!btlCheckSpecialAbility(&unit->partyRecord, 0x231)) {
                btlStartTask(btlCreateEffObjB(unit, task->indexWork.parameter));
                task->indexWork.stage = 0;
            } else {
                btlStartTask(btlCreateEffObjD(unit, 0x231));
            }
        }
        unit->status.flags &= ~0x1000;
        unit->partyRecord.flags &= ~0x1000;
        change = btlCreateModelChangeTask(unit, unit->modelId, unit->modelVariant, motion, 0x12,
                                                       3);
        btlStartTask(change);
        btlStartTask(btlCreateGunLoadPollTask(unit));
        btlStartTask(sndCreateEffectSourceTask(state->resources[47], unit, change->handle));
        btlStartTask(sndCreateStationedSeTask(0x1000F));
        unit->status.flags &= ~0x400000;
        if (task->flags & 0x10) {
            btlDispatchStateHandler(task, 0x1B);
        } else {
            btlDispatchStateHandler(task, 0x1A);
        }
    }
}

/* Mark the acting unit in progress on entry to the retail DEVIL state. */
void btlEnterDevilActionState(BtlTask *task) {
    task->unit->status.flags |= 0x4000;
}

/* Slot-0x10 model-change command: the same flow as btlCommandGunChangeStart, swapping to the unit's
 * modelResourceKind/displaySpecies model and back; the return path only clears flag 0x20000000. */
/* Retail DEVIL state: schedule the alternate model tuple and its dependent tasks. */
void btlScheduleDevilStateModelChange(BtlTask *task) {
    BtlUnit *unit;
    BtlState *state;
    BtlUnit *other;
    BtlRuntimeTask *sound;
    BtlRuntimeTask *change;
    BtlRuntimeTask *spawned;
    BtlRuntimeTask *load;
    u16 *status;
    s32 motion;

    if (btlCountTasksByKind(0x1A) != 0 || btlCountTasksByKind(0x18) != 0 || btlCountTasksByKind(0x23) != 0) {
        return;
    }
    state = (BtlState *)btlGetRuntime();
    unit = task->unit;
    status = (u16 *)btlGetActorResourceBankEntry(unit->resourceKind, unit->species);
    if (!(unit->status.flags & 0x20)) {
        motion = btlGetUnitMotionDurationFrames(unit, 0x10) + 0x14;
    } else {
        motion = status[0x15];
    }
    if (!(unit->status.flags & 0x20000020)) {
        unit->status.flags |= 0x1000;
        unit->partyRecord.flags |= 0x1000;
        sound = btlCreateMoveOtherUnitsTask(unit, 0x10);
        btlStartTask(sound);
        for (other = state->units; other != NULL; other = other->next) {
            if (other != unit && (btlUnitStatusPair(other) & 0x202) == 0x202) {
                spawned = btlScheduleRefreshTask(other);
                spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                spawned->startDelay = 1;
                spawned->startCondition.value.handle = sound->handle;
                spawned->ownerId = unit->identity;
                btlStartTask(spawned);
            }
        }
        change = btlCreateModelChangeTask(unit, unit->modelResourceKind, unit->displaySpecies, motion, 0x18,
                                                       0);
        change->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        change->startCondition.value.handle = sound->handle;
        btlStartTask(change);
        for (other = state->units; other != NULL; other = other->next) {
            if (other != unit && (btlUnitStatusPair(other) & 0x202) == 0x202) {
                load = btlCreateModelLoadPollTask(other, other->resourceKind, other->species, 0);
                load->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                load->startCondition.value.handle = change->handle;
                load->ownerId = unit->identity;
                btlStartTask(load);
                spawned = btlCreateUnitFadeInTask(other, 0, 0);
                spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                spawned->startCondition.value.handle = load->handle;
                spawned->ownerId = unit->identity;
                btlStartTask(spawned);
            }
        }
        btlStartTask(btlCreateGunLoadPollTask(unit));
        btlStartTask(sndCreateEffectSourceTask(state->resources[45], unit, change->handle));
        btlStartTask(sndCreateStationedSeTask(0x1000F));
        spawned = (BtlRuntimeTask *)btlCreateEffObjA(unit, task->indexWork.phase);
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlCreateCommandSoundUpdateTask();
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlCreateSecondaryCommandSoundTask();
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlCreateCommandSoundTask(task, 0xE);
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlAllocateIndexedUnitEffectTask(unit, 0x10,
                                                                btlGetSlotRateKind(unit, 0x10), 1.0f);
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (BtlRuntimeTask *)fldCreateSceneGroupAction((u8 *)task, 0x64, 1);
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        spawned->ownerId = unit->identity;
        btlStartTask(spawned);
        if (task->unit->partyRecord.status & 0x480) {
            btlDispatchStateHandler(task, 0x18);
        } else {
            btlDispatchStateHandler(task, 0x1A);
        }
    } else {
        unit->status.flags |= 0x1000;
        unit->partyRecord.flags |= 0x1000;
        change = btlCreateModelChangeTask(unit, unit->modelResourceKind, unit->displaySpecies, motion, 0x12,
                                                       3);
        btlStartTask(change);
        btlStartTask(btlCreateGunLoadPollTask(unit));
        btlStartTask(sndCreateEffectSourceTask(state->resources[47], unit, change->handle));
        btlStartTask(sndCreateStationedSeTask(0x1000F));
        unit->status.flags &= ~0x20000000;
        if (task->flags & 0x10) {
            btlDispatchStateHandler(task, 0x1B);
        } else {
            btlDispatchStateHandler(task, 0x1A);
        }
    }
}

/* Mark the acting unit in progress on entry to the retail BAT state. */
void btlEnterBatActionState(BtlTask *task) {
    task->unit->status.flags |= 0x4000;
}


/* Retail BAT state: schedule the status-table model; BAT's meaning is not expanded. */
void btlScheduleBatStateModelChange(BtlActionStateOwner *task) {
    BtlUnit *actor;
    u8 *effectTask;
    u8 *modelTask;
    BtlActorStatusRecord *statusTable;
    s32 model;

    if (btlCountTasksByKind(0x1A) != 0 ||
        btlCountTasksByKind(0x18) != 0 ||
        btlCountTasksByKind(0x23) != 0) {
        return;
    }

    btlGetRuntime();
    actor = task->unit;
    statusTable = (BtlActorStatusRecord *)btlGetActorResourceBankEntry(
        actor->resourceKind, actor->species);
    model = statusTable->model;
    effectTask = (u8 *)btlCreateEffObjB(actor, 0x7E);
    btlStartTask(effectTask);
    modelTask = btlCreateModelChangeTask(actor, 0, 0x1F, model, 0x12, 1);
    btlStartTask(modelTask);

    actor->status.flags = ((u32)actor->status.flags | 0x1000) & 0xEFFFFFFF;
    actor->partyRecord.flags |= 0x1000;
    if (task->flags & 0x10) {
        btlDispatchStateHandler(task, 0x1B);
    } else {
        btlDispatchStateHandler(task, 0x1A);
    }
}

/* Empty entry callback for the retail WAIT action state. */
void btlEnterWaitActionState(void) {
}

void btlCommandTaskStartEffects(u8 *task) {
    s32 countdown;
    s32 effectId;

    if (((u32)((BtlTask *)task)->unit->status.flags & 0x200) != 0 &&
        (*(u32 *)(task + 8) & 0x20) != 0) {
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
        btlStartTask(btlCreateCommandSoundTask((BtlTask *)task, 9));
    }
    switch (*(s32 *)(task + 0x20)) {
    case 14:
        ((BtlTask *)task)->unit->status.flags = (u32)((BtlTask *)task)->unit->status.flags | (0x2000000);
        effectId = 0xF;
        countdown = 0x64;
        break;
    case 10:
        if (((u32)((BtlTask *)task)->unit->status.flags & 0x400) != 0) {
            btlStartTask(btlCreateCommandSoundUpdateTask());
            btlStartTask(btlCreateSecondaryCommandSoundTask());
            btlStartTask(btlCreateCommandSoundTask((BtlTask *)task, 3));
        }
        effectId = ((u32)((BtlTask *)task)->unit->status.flags & 0x200) ? 0xF : 0x1E;
        countdown = 0x32;
        break;
    case 13:
        effectId = 0xF;
        countdown = 0x64;
        break;
    default:
        effectId = 0;
        countdown = 0;
        break;
    }

    btlStartTask(btlCreateEffObjA(0, *(s32 *)(task + 0x20)));
    {
        u8 *object = fldCreateSceneGroupAction(task, countdown, 1);
        *(s32 *)(object + 0x28) = effectId;
        *(s64 *)(object + 0x40) = *(s64 *)(*(u8 **)(task + 0x18) + 0x108);
        btlStartTask(object);
    }
    if ((((BtlTask *)task)->unit->partyRecord.status & 0x480) != 0) {
        btlDispatchStateHandler(task, 0x18);
    } else {
        btlDispatchStateHandler(task, 0x1A);
    }
}

extern BtlRuntimeTask *btlCreateUnitExitFadeTask(BtlUnit *, BtlUnitExitFadeMode);

u64 btlCommandTaskReturnStart(u8 *task) {
    BtlUnit *linked = *(BtlUnit **)(task + 0x34);
    BtlUnit *actor;
    u8 *object;

    if (linked != 0) {
        actor = linked;
    } else {
        actor = *(BtlUnit **)(task + 0x18);
    }
    if ((u32)actor->status.flags & 0x200) {
        btlBossDebugPrintf("return:player=%X[%X]\n", actor->partyIndex, actor->partyRecord.unitId);
    } else {
        btlBossDebugPrintf("return:enemy=%X\n", actor->partyRecord.unitId);
    }
    if ((u32)actor->status.flags & 0x400) {
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
    }
    if ((u32)actor->status.flags & 0x200) {
        btlStartTask(btlCreateCommandSoundTask((BtlTask *)task, 9));
        btlStartTask(btlCreateEffObjA(actor, *(u32 *)(task + 0x20)));
    } else if ((*(u32 *)(task + 8) & 0x200) == 0) {
        btlStartTask(btlCreateCommandSoundTask((BtlTask *)task, 0x10));
        btlStartTask(btlCreateEffObjB(actor, 0xF));
    }
    if ((u32)actor->status.flags & 0x200) {
        btlSyncPlayerWork(actor);
    }
    object = fldCreateSceneGroupAction(task, 0x64, 1);
    *(s32 *)(object + 0x28) = 0xF;
    *(u64 *)(object + 0x40) = *(u64 *)(*(u8 **)(task + 0x18) + 0x108);
    btlStartTask(object);
    object = (u8 *)btlCreateUnitExitFadeTask(actor, BTL_UNIT_EXIT_FADE_MODE_FRAME_ALPHA);
    *(s32 *)(object + 0x28) = 0xF;
    return btlStartTask(object);
}


extern void fldUpdateSceneGroupTask(s32 task);

extern void btlRemoveTaskFromSceneGroup(BtlTask *task);

void btlCommandTaskReturnUpdate(BtlTask *task) {
    BtlUnit *unit = task->unit;
    u32 flags = (u32)unit->status.flags;

    unit->status.flags = flags & ~1;
    if (flags & 0x200) {
        btlRepositionPartyAroundBattleCenter();
        btlMarkInactiveActorCandidates();
    }
    if (btlCountTasksByKind(0x3C) != 0) {
        return;
    }
    if (btlCountTasksByKind(0x42) != 0) {
        return;
    }
    if (((u32)unit->status.flags & 0x40) == 0) {
        return;
    }
    if (btlPollActorResourceCleanup(task->unit) != BTL_ACTOR_RESOURCE_CLEANUP_PENDING) {
        if ((u32)unit->status.flags & 0x200) {
            btlClearPartyRecordStatusBits(&unit->partyRecord, 8);
            btlRemovePartyActorAndShiftEntries(unit);
        }
        fldUpdateSceneGroupTask((s32)task);
        btlRemoveTaskFromSceneGroup(task);
        btlDispatchStateHandler(task, 0x1E);
    }
}

void btlStartLinkedActorEffectTask(BtlTask *task) {
    BtlRuntimeTask *entry;
    if (((u32)task->unit->status.flags & 0x200) == 0 &&
        btlCanSpawnAnotherEnemy(task->unit) == BTL_ENEMY_SPAWN_CAPACITY_EXHAUSTED) {
        entry = btlCreateEffObjB(task->unit, 0x67);
        entry->startCondition.kind = BTL_TASK_CONDITION_KIND_ABSENT;
        entry->startCondition.value.taskKind = 0x40;
        btlStartTask(entry);
        if ((task->unit->partyRecord.status & 0x480) != 0) {
            btlDispatchStateHandler(task, 0x18);
        } else {
            btlDispatchStateHandler(task, 0x1A);
        }
    }
}

extern void btlResetUnitLinks(BtlUnit *);
extern void btlInitializePartyRecordFromEnemy(datUnitWork_t *, s32);
extern void btlInsertPartyActorByPriority(BtlUnit *, u8, u8);
extern struct BtlRuntimeTask *btlCreateControlObject(void);
extern BtlTask *btlCreateActionSeq(void);
extern void btlBindActorTaskAndSelectActionNumber(BtlTask *, BtlUnit *);
extern void btlSetSceneTaskActiveFlag(BtlTask *);
extern BtlRuntimeTask *btlCreateUnitDefeatFadeInTask(BtlUnit *, s32);
extern f32 btlMeasureMaskedUnitBounds(s32, f32 *, f32 *);

void btlScheduleSummonActionTasks(BtlTask *action) {
    f32 center[4] __attribute__((aligned(16)));
    f32 bodyPosition[4] __attribute__((aligned(16)));
    BtlState *battle;
    BtlUnit *unit;
    BtlRuntimeTask *task;
    BtlRuntimeTask *load;
    BtlRuntimeTask *controlTask;
    BtlRuntimeTask *fadeTask;
    BtlRuntimeTask *completionTask;
    BtlTask *summonedAction;
    s32 modelId;
    s32 resourceKind;
    s32 duration;
    s32 effectId;
    s32 effectDelay;
    s32 fadeEnabled;
    s32 repositioned;
    u64 completionHandle;
    f32 rate;
    u16 partyId;
    u32 playerFlags;

    battle = btlGetRuntime();
    btlGetIndexListCount(action->indexWork.indices);
    unit = btlGetIndexListEntry(action->indexWork.indices, 0);
    btlResetUnitLinks(unit);
    if (action->unit->status.flags & 0x200) {
        btlBossDebugPrintf("summon:player=%X\n", action->indexWork.summonId);
        unit->status.flags |= 0x301;
        btlInsertPartyActorByPriority(unit, (u8)action->indexWork.summonId, unit->battleSideSlotIndex);
        if (unit->partyRecord.flags & 0x1000) {
            unit->status.flags |= 0x1000;
        }
        if (unit->partyRecord.flags & 0x4000) {
            unit->status.stateFlags |= 0x2000;
        }
        if (unit->partyRecord.status & 0x4000) {
            unit->status.flags |= 0x20;
        }
        partyId = unit->partyRecord.unitId;
        playerFlags = unit->status.flags;
        unit->modelId = 0;
        unit->modelVariant = partyId;
        unit->modelResourceKind = 0;
        unit->displaySpecies = partyId + 0x10;
        if (playerFlags & 0x1000) {
            resourceKind = 0;
            modelId = partyId + 0x10;
        } else {
            resourceKind = 0;
            modelId = partyId;
        }
        repositioned = btlRepositionPartyAroundBattleCenter();
        btlMarkInactiveActorCandidates();
        if (repositioned != 0) {
            btlStartTask(btlCreateCommandSoundUpdateTask());
            btlStartTask(btlCreateCommandSoundTask(action, 0xB));
        }
        btlStartTask(btlCreateEffObjA(action->unit, action->indexWork.phase));
    } else {
        s32 controlDelay = 0;
        btlBossDebugPrintf("summon:enemy=%X\n", action->indexWork.summonId);
        resourceKind = 1;
        btlInitializePartyRecordFromEnemy(&unit->partyRecord, action->indexWork.summonId);
        unit->status.flags |= 0x501;
        unit->modelResourceKind = resourceKind;
        unit->partyRecord.flags |= 0x23;
        modelId = action->indexWork.summonId;
        unit->displaySpecies = modelId;
        btlInitializeEffectVectorsFromSourceRecords(
            unit, BTL_ACTOR_RESOURCE_TABLE_OPPOSING, modelId);
        if (action->indexWork.skillId < 0x260 && action->indexWork.skillId != -1) {
            if (datActionAnimationRecords[action->indexWork.skillId].flags & 0x100) {
                controlDelay = 2;
            }
        }
        controlTask = btlCreateControlObject();
        controlTask->startDelay = controlDelay;
        controlTask->startCondition.kind = 10;
        controlTask->startCondition.value.taskKind = 0x40;
        btlStartTask(controlTask);
        task = btlCreateTargetedCommandSoundTask(action, 0xB, action->indexWork.skillId);
        task->startCondition.kind = 10;
        task->startCondition.value.taskKind = 0x40;
        btlStartTask(task);
    }

    summonedAction = btlCreateActionSeq();
    btlBindActorTaskAndSelectActionNumber(summonedAction, unit);
    fldAppendTaskToGroup(summonedAction);
    btlDispatchStateHandler(summonedAction, 2);
    btlSetSceneTaskActiveFlag(summonedAction);
    btlDispatchStateHandler(summonedAction, 0x1B);
    summonedAction->flags |= 0x100;
    duration = 0;
    if (!(action->unit->status.flags & 0x200)) {
        duration = btlGetUnitMotionDurationFrames(action->unit, action->indexWork.slot);
        if (duration < 20) {
            duration = 20;
        }
    }
    if (!(unit->partyRecord.status & 0x1000)) {
        load = btlCreateModelLoadPollTask(unit, resourceKind, modelId, 0);
    } else {
        load = btlCreateModelLoadPollTask(unit, 0, 0x1F, 0);
    }
    if (action->unit->status.flags & 0x200) {
        load->startCondition.kind = 10;
        load->startCondition.value.taskKind = 0x40;
    } else {
        load->startDelay = duration;
    }
    load->ownerId = action->unit->identity;
    btlStartTask(load);
    task = btlCreateUnitBaseLightTask(unit);
    task->startCondition.kind = 4;
    task->startCondition.value.handle = load->handle;
    btlStartTask(task);
    task = btlCreateGunLoadPollTask(unit);
    task->startCondition.kind = 10;
    task->startCondition.value.taskKind = 0x40;
    task->ownerId = action->unit->identity;
    btlStartTask(task);
    if (action->unit->status.flags & 0x200) {
        if (unit->battleSideSlotIndex != 1 && battle->cameraPresetMode == 3) {
            unit->currentPosition[2] = unit->position[2] - 45.0f;
        }
        if (!(unit->status.flags & 0xE0)) {
            /* Audited SDK VU pipeline, as in btlUnitFaceTarget: only the
             * external aim routine consumes the two stored vectors. */
            btlMeasureMaskedUnitBounds(0x400, 0, 0);
            SDF_VU0_STOREVECTOR_NOMEM(center);
            btlUnitGetBodyPosVU(unit);
            SDF_VU0_STOREVECTOR_NOMEM(bodyPosition);
            if (btlAimHorizontalDirectionVU(bodyPosition, center) != 0) {
                SDF_VU0_STOREVECTOR_NOMEM(unit->orientation);
            }
        }
        rate = 1.0f;
        task = btlCreateUnitPositionLerpTowardTargetTask(unit, unit->currentPosition, rate);
        task->startCondition.kind = 4;
        task->startCondition.value.handle = load->handle;
        btlStartTask(task);
        task = btlCreateUnitRotationInterpolationTask(unit, unit->orientation, rate);
        task->startCondition.kind = 4;
        task->startCondition.value.handle = load->handle;
        btlStartTask(task);
    }
    fadeEnabled = 1;
    if (!(action->unit->status.flags & 0x200) && battle->summonFadePredicate != 0) {
        fadeEnabled = battle->summonFadePredicate(action->indexWork.skillId);
    }
    if (fadeEnabled != 0) {
        fadeTask = btlCreateUnitDefeatFadeInTask(unit, 0x12);
        fadeTask->startCondition.kind = 4;
        fadeTask->startDelay = 2;
        fadeTask->endDelay = 16;
        fadeTask->startCondition.value.handle = load->handle;
        fadeTask->ownerId = action->unit->identity;
        btlStartTask(fadeTask);
    } else {
        fadeTask = btlCreateUnitDefeatFadeInTask(unit, -1);
        fadeTask->startCondition.kind = 4;
        fadeTask->startDelay = 2;
        fadeTask->endDelay = 16;
        fadeTask->startCondition.value.handle = load->handle;
        fadeTask->ownerId = action->unit->identity;
        btlStartTask(fadeTask);
    }
    if (action->unit->status.flags & 0x200) {
        task = (BtlRuntimeTask *)fldCreateSceneGroupAction((u8 *)action, 100, 1);
        task->ownerId = action->unit->identity;
        btlStartTask(task);
    } else {
        effectId = 0xAC;
        effectDelay = 0;
        if (battle->summonEffectId != 0) {
            s32 override = battle->summonEffectId(modelId);
            if (override != -1) {
                effectId = override;
            }
        }
        if (battle->summonEffectDelay != 0) {
            effectDelay = battle->summonEffectDelay(modelId);
        }
        task = (BtlRuntimeTask *)btlCreateEffObjB(unit, effectId);
        task->startCondition.kind = 5;
        task->startDelay = effectDelay;
        task->startCondition.value.handle = fadeTask->handle;
        task->ownerId = action->unit->identity;
        btlStartTask(task);
        completionTask = btlCreateImmediateCompletionTask();
        completionTask->startCondition.kind = 10;
        completionTask->startCondition.value.taskKind = 0x2B;
        completionTask->ownerId = action->unit->identity;
        btlStartTask(completionTask);
        completionHandle = completionTask->handle;
        task = sndCreateReleaseTask(12);
        task->startCondition.kind = 4;
        task->startCondition.value.handle = completionHandle;
        btlStartTask(task);
        task = btlCreateSceneLightingRestoreTask(12);
        task->startCondition.kind = 4;
        task->startCondition.value.handle = completionHandle;
        btlStartTask(task);
        task = btlCreateWaitUnitListIdleTask(12);
        task->startCondition.kind = 4;
        task->startCondition.value.handle = completionHandle;
        btlStartTask(task);
        task = btlCreateApplyToActiveActorsTask(12);
        task->startCondition.kind = 4;
        task->startCondition.value.handle = completionHandle;
        btlStartTask(task);
        task = (BtlRuntimeTask *)btlCreateFadeStateResetTask();
        task->startCondition.kind = 4;
        task->startCondition.value.handle = completionHandle;
        btlStartTask(task);
    }
    if (action->unit->partyRecord.status & 0x480) {
        btlDispatchStateHandler(action, 0x18);
    } else {
        btlDispatchStateHandler(action, 0x1A);
    }
}


u64 btlStartOwnerEffectTasks(s32 *arguments) {
    BtlUnit *owner = (BtlUnit *)arguments[0x34 / 4];
    BtlRuntimeTask *value = btlCreateEffObjA(owner, arguments[0x20 / 4]);
    btlStartTask(value);
    value = btlCreateUnitExitFadeTask(owner, BTL_UNIT_EXIT_FADE_MODE_FRAME_ALPHA);
    value->startDelay = 7;
    return btlStartTask(value);
}

extern void btlSynchronizePartyActorRecords(BtlUnit *, u8);


extern void btlReleaseUnitResources(BtlUnit *);

extern BtlTask *btlFindUnitByActor(BtlUnit *target);

void btlReloadLinkedUnitAndQueueEffects(BtlTask *task) {
    BtlUnit *unit = task->indexWork.linkedUnit;
    datUnitWork_t *record;
    BtlRuntimeTask *load;
    BtlRuntimeTask *spawned;
    BtlRuntimeTask *delayed;
    u32 species;
    u32 flags;
    u32 unitId;
    f32 rate;

    if (!(unit->status.flags & 0x40)) {
        return;
    }
    if (btlPollActorResourceCleanup(unit) == BTL_ACTOR_RESOURCE_CLEANUP_PENDING) {
        return;
    }
    unit->status.flags &= ~0xC0;
    record = &unit->partyRecord;
    btlClearPartyRecordStatusBits(record, 8);
    if (unit->status.flags & 0x200) {
        btlSyncPlayerWork(unit);
    }
    btlReleaseUnitResources(unit);
    btlResetUnitLinks(unit);
    btlSynchronizePartyActorRecords(unit, (u8)task->indexWork.summonId);
    flags = unit->status.flags;
    unit->status.flags = flags | 0x301;
    if (record->flags & 0x1000) {
        unit->status.flags = flags | 0x1301;
    }
    if (record->flags & 0x4000) {
        unit->status.stateFlags |= 0x2000;
    }
    if (unit->partyRecord.status & 0x4000) {
        unit->status.flags |= 0x20;
    }
    unitId = unit->partyRecord.unitId;
    unit->modelVariant = unitId;
    unit->displaySpecies = unitId + 0x10;
    unit->modelId = 0;
    unit->modelResourceKind = 0;
    species = (unit->status.flags & 0x1000) ? unit->displaySpecies : unitId;
    btlFindUnitByActor(unit)->flags |= 0x100;
    unit->baseColor = 0x80808080;
    unit->overlayColor = 0x80808080;
    if (!(unit->partyRecord.status & 0x1000)) {
        load = btlCreateModelLoadPollTask(unit, 0, species, 0);
    } else {
        load = btlCreateModelLoadPollTask(unit, 0, 0x1F, 0);
    }
    rate = 1.0f;
    load->ownerId = task->unit->identity;
    btlStartTask(load);
    spawned = btlCreateGunLoadPollTask(unit);
    spawned->ownerId = task->unit->identity;
    btlStartTask(spawned);
    spawned = btlCreateUnitPositionLerpTowardTargetTask(unit, unit->currentPosition, rate);
    spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
    spawned->startCondition.value.handle = load->handle;
    btlStartTask(spawned);
    if (!(unit->status.flags & 0xE0)) {
        spawned = btlCreateUnitRotationInterpolationTask(unit, unit->orientation, rate);
    } else {
        spawned = btlCreateUnitRotationInterpolationTask(unit, unit->rotation, rate);
    }
    spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
    spawned->startCondition.value.handle = load->handle;
    btlStartTask(spawned);
    delayed = btlCreateUnitDefeatFadeInTask(unit, 0x12);
    delayed->ownerId = task->unit->identity;
    delayed->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
    delayed->startDelay = 2;
    delayed->startCondition.value.handle = load->handle;
    btlStartTask(delayed);
    spawned = btlCreateUnitBaseLightTask(unit);
    spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
    spawned->startCondition.value.handle = delayed->handle;
    btlStartTask(spawned);
    spawned = (BtlRuntimeTask *)fldCreateSceneGroupAction((u8 *)task, 0x64, 1);
    spawned->ownerId = task->unit->identity;
    btlStartTask(spawned);
    if ((task->unit->partyRecord.status & 0x480) && task->indexWork.linkedUnit != task->unit) {
        btlDispatchStateHandler(task, 0x18);
    } else {
        btlDispatchStateHandler(task, 0x1A);
    }
}

extern s32 btlRollEscapeChance(BtlUnit *);

/* Count an escape attempt and record its chance-test result in battle flags. */
void btlRecordEscapeAttemptOutcome(BtlTask *task) {
    BtlState *context = btlGetRuntime();
    BtlUnit *target = task->unit;
    context->escapeAttemptCount += 1;
    if (btlRollEscapeChance(target)) {
        context->battleFlags |= 0x2000;
    } else {
        context->battleFlags |= 0x1000;
    }
}

extern BtlRuntimeTask *btlCreateUnitDefeatFadeOutTask(BtlUnit *unit, s32 duration);

/* Queue escape-result sounds and stagger eligible party effects; successful
 * escapes also fade each eligible actor, with its identity as the task owner. */
void btlScheduleEscapeResultTasks(BtlTask *task) {
    BtlState *state = btlGetRuntime();
    BtlUnit *unit;
    BtlRuntimeTask *effect;
    s32 delay;
    u32 flags;

    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    btlStartTask(btlCreateCommandSoundTask((BtlTask *)task, 0xF));
    if (state->battleFlags & 0x2000) {
        if (!btlCheckSpecialAbility(&task->unit->partyRecord, 0x230)) {
            btlStartTask(btlCreateEffObjA(task->unit, task->indexWork.phase));
        } else {
            btlStartTask(btlCreateEffObjD(task->unit, 0x230));
        }
        delay = 0xF;
        for (unit = state->units; unit; unit = unit->next) {
            flags = unit->status.flags;
            /* Retail tests these three admission bits independently. */
            if (!(flags & 1)) continue;
            if (!(flags & 0x200)) continue;
            if (flags & 0xE0) continue;
            if (btlTestActorStatusPredicate(unit)) continue;
            effect = btlAllocateIndexedUnitEffectTask(unit, 0xD, 2, 1.0f);
            effect->startDelay = delay;
            btlStartTask(effect);
            effect = btlCreateUnitDefeatFadeOutTask(unit, 6);
            {
                /* Capture the full-width owner before deriving the fade delay:
                 * see docs/idioms.md, "Trace-justified staging: escape-result task metadata". */
                u64 ownerId = unit->identity;
                s32 fadeDelay = delay + 0xE;
                effect->ownerId = ownerId;
                effect->startDelay = fadeDelay;
            }
            btlStartTask(effect);
            delay++;
        }
    } else {
        delay = 7;
        btlStartTask(btlCreateEffObjA(task->unit, task->indexWork.phase));
        for (unit = state->units; unit; unit = unit->next) {
            flags = unit->status.flags;
            if (!(flags & 1)) continue;
            if (!(flags & 0x200)) continue;
            if (flags & 0xE0) continue;
            if (btlTestActorStatusPredicate(unit)) continue;
            effect = btlAllocateIndexedUnitEffectTask(unit, 0xD, 2, 1.0f);
            effect->startDelay = delay++;
            btlStartTask(effect);
        }
        /* Retail passes the exhausted (null) traversal cursor to this effect. */
        effect = btlCreateEffObjB(unit, 0xA8);
        effect->startDelay = delay + 0xA;
        btlStartTask(effect);
    }
    if ((task->unit->partyRecord.status & 0x480) && !(state->battleFlags & 0x2000)) {
        btlDispatchStateHandler(task, 0x18);
    } else {
        btlDispatchStateHandler(task, 0x1A);
    }
}

/* Empty entry callback for the retail BAD DAMAGE action state. */
void btlEnterStatusDamageState(void) {
}


extern s32 btlSumOtherTargetHitAmounts(BtlTask *);

extern s32 btlComputeStatusPenaltyFifth(BtlUnit *);


/* After owned tasks finish, queue ailment damage, penalties and their effects. */
void btlProcessPostActionStatusEffects(BtlTask *action) {
    BtlOperandEntry spec;
    s32 amount;
    BtlUnit *unit;
    BtlRuntimeTask *deltaTask;
    BtlRuntimeTask *effectTask;

    if (btlCountTasksByKind(0x45) != 0) return;
    if (btlCountTasksByKind(0x46) != 0) return;
    if (btlCountTasksByKind(0x47) != 0) return;
    unit = action->unit;
    if (btlCountTasksForOwner(unit->identity) != 0) return;
    if (action->indexWork.phase == 15) {
        effectTask = (BtlRuntimeTask *)fldCreateSceneGroupAction((u8 *)action, 100, 1);
        effectTask->ownerId = unit->identity;
        btlStartTask(effectTask);
    }
    switch (unit->partyRecord.status & 0x7FFF) {
    case 0x400:
        amount = btlSumOtherTargetHitAmounts(action);
        if (amount < 0) {
            btlStartTask(sndCreateStationedSeTask(0x1000A));
            memset(&spec, 0, sizeof(spec));
            spec.hpDelta = amount;
            deltaTask = btlCreateActorParameterDeltaTask(unit, &spec);
            btlStartTask(deltaTask);
            if (spec.hpDelta != 0) {
                effectTask = btlCreateLinkedEffectTask(unit, spec.hpDelta, 0);
                effectTask->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                effectTask->startCondition.value.handle = deltaTask->handle;
                effectTask->ownerId = btlAdvanceRuntimeSequenceCounter();
                btlStartTask(effectTask);
            }
            if ((unit->status.flags & 0x200) != 0 && btlIsUnitDefeatTriggeredByValueDelta(unit, spec.hpDelta) != 0) {
                btlStartTask(btlCreateActorModelBlendTask(unit, 0, 11, 2, 1.0f));
                btlStartTask(btlCreateMoveOtherUnitsTask(unit, 11));
            } else {
                btlStartTask(btlCreateStiffenDamageShakeTask(unit, 8.0f));
            }
        }
        btlDispatchStateHandler(action, 0x1A);
        return;
    case 0x80:
    case 0x2000:
        btlStartTask(sndCreateStationedSeTask(0x1000A));
        memset(&spec, 0, sizeof(spec));
        spec.hpDelta = btlComputeStatusPenaltyFifth(unit);
        deltaTask = btlCreateActorParameterDeltaTask(unit, &spec);
        btlStartTask(deltaTask);
        if (spec.hpDelta != 0) {
            effectTask = btlCreateLinkedEffectTask(unit, spec.hpDelta, 0);
            effectTask->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
            effectTask->startCondition.value.handle = deltaTask->handle;
            effectTask->ownerId = btlAdvanceRuntimeSequenceCounter();
            btlStartTask(effectTask);
        }
        if ((unit->status.flags & 0x200) != 0 && btlIsUnitDefeatTriggeredByValueDelta(unit, spec.hpDelta) != 0) {
            btlStartTask(btlCreateActorModelBlendTask(unit, 0, 11, 2, 1.0f));
            btlStartTask(btlCreateMoveOtherUnitsTask(unit, 11));
        } else {
            btlStartTask(btlCreateStiffenDamageShakeTask(unit, 8.0f));
        }
        btlDispatchStateHandler(action, 0x1A);
        return;
    }
    btlDispatchStateHandler(action, 0x1A);
}

void btlSpawnSceneActionAndSwitchState(void) {
}

/* Retail PRESS state: start scene action 0x1194, then enter the owner-task wait. */
void btlSchedulePressStateSceneAction(u8 *unit) {
    u8 *task = fldCreateSceneGroupAction(unit, 0x1194, 1);

    btlStartTask(task);
    btlDispatchStateHandler(unit, 0x1a);
}

/* Empty entry callback for the retail PACKET action state. */
void btlEnterPacketActionState(void) {
}


void btlAdvanceStateWhenLinkedTasksFinish(BtlTask *task) {
    BtlUnit *owner = task->unit;
    if (btlCountTasksForOwner(owner->identity) == 0) {
        btlDispatchStateHandler(task, 0x1C);
    }
}

void btlAdvanceLinkedUnitWhenOwnerIdle(void) {
}

/* Clear the actor's in-progress bit and return to STANDBY when its tasks finish. */
void btlResumeStandbyWhenActorTasksFinish(BtlTask *task) {
    BtlUnit *owner = task->unit;
    if (btlCountTasksForOwner(owner->identity) == 0) {
        owner->status.flags = (u32)owner->status.flags & (~0x4000);
        btlDispatchStateHandler(task, 2);
    }
}

void btlFinalizeLinkedActionAndAdvanceHistory(void) {
}

extern void btlAdvanceHistoryCounter(BtlTask *task);

void btlUnitTurnEndCommit(BtlTask *task) {
    void (*hook)(BtlTask *) = *(void (**)(BtlTask *))((u8 *)btlGetRuntime() + 0x600);
    BtlUnit *owner = task->unit;

    if (hook != 0) {
        hook(task);
    }
    btlAdvanceHistoryCounter(task);
    btlResetIndexWork(&task->indexWork);
    owner->pendingSkillNameIndex = BTL_SKILL_NAME_INDEX_NONE;
    task->options &= ~1;
    task->completedTurns += 1;
    owner->status.flags &= ~0x4000;
    fldUpdateSceneGroupTask((s32)task);
    if (task->unit->status.flags & 0x20) {
        btlUnitTurnEndStateSelect(task);
    } else {
        btlDispatchStateHandler(task, 2);
    }
}

extern void btlAccumulateEnemyDefeatRewards(BtlUnit *);


extern BtlRuntimeTask *btlCreateSelectedEffectUpdateTask(BtlUnit *);


void btlStartActorDefeatTransition(BtlTask *commandTask) {
    BtlActorWork *work = (BtlActorWork *)btlGetRuntime();
    BtlUnit *actor = commandTask->unit;
    datUnitWork_t *profile = &actor->partyRecord;
    BtlRuntimeTask *soundTask;
    BtlRuntimeTask *object;
    s64 sequence;
    s32 entryFlags;
    s32 result;

    actor->selectedEntryIndex = BTL_COMMAND_ID_INVALID;
    btlRaisePartyRecordStatusValue(&actor->partyRecord, 0x4000);
    btlGetActorResourceBankEntry(actor->resourceKind, actor->species);
    if (!(commandTask->flags & 0x100)) {
        soundTask = btlCreateMoveOtherUnitsTask(actor, 11);
        btlStartTask(soundTask);
        sequence = soundTask->handle;
    } else {
        sequence = btlAdvanceRuntimeSequenceCounter();
    }
    if (actor->status.flags & 0x200) {
        if (!(commandTask->flags & 0x100)) {
            if (!(actor->status.flags & 0x8000000) && actor->motionId != 11) {
                object = btlCreateActorModelBlendTask(actor, 0, 11, 2, 1.0f);
                object->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                object->startCondition.value.handle = sequence;
                btlStartTask(object);
            }
            btlRefreshUnitMotionSelection(actor);
        }
    } else if (actor->status.flags & 0x400) {
        btlAccumulateEnemyDefeatRewards(actor);
        if (actor->status.flags & 0x8000000) {
            object = btlCreateSelectedEffectUpdateTask(actor);
            object->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
            object->startCondition.value.handle = sequence;
            btlStartTask(object);
            object = sndCreateStationedSeTask(0x1000E);
            object->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
            object->startCondition.value.handle = sequence;
            btlStartTask(object);
            actor->status.flags &= ~1;
        } else {
            entryFlags = btlGetEntryFlagsUnlessDisabled(profile);
            result = 0;
            if (work->hook5E4 != 0) {
                result = work->hook5E4(actor);
            }
            if ((entryFlags & 0x200) && result == 0) {
                result = 1;
                if (work->hook5E8 != 0) {
                    result = work->hook5E8(actor);
                }
                if (result != 0) {
                    if (actor->motionId != 11) {
                        object = btlCreateActorModelBlendTask(actor, 0, 11, 2, 1.0f);
                        object->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                        object->startCondition.value.handle = sequence;
                        btlStartTask(object);
                    }
                    btlRefreshUnitMotionSelection(actor);
                }
            } else {
                object = btlCreateUnitExitFadeTask(actor, BTL_UNIT_EXIT_FADE_MODE_EVENT_COLOR);
                object->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                object->startCondition.value.handle = sequence;
                btlStartTask(object);
                actor->status.flags &= ~1;
            }
        }
        actor->partyRecord.flags &= ~2;
    }
}

void btlRemoveEligibleActorSceneTask(BtlTask *task) {
    BtlUnit *actor = task->unit;
    s32 entryFlags;

    if (actor->status.flags & 0x200) {
        if (btlPollActorResourceCleanup(actor) == BTL_ACTOR_RESOURCE_CLEANUP_PENDING) {
            return;
        }
        btlResetIndexWork(&task->indexWork);
        actor->pendingSkillNameIndex = BTL_SKILL_NAME_INDEX_NONE;
        btlClearAllActorEntrySlots(actor);
        fldUpdateSceneGroupTask(task);
        btlRemoveTaskFromSceneGroup(task);
        btlDispatchStateHandler(task, 1);
    } else {
        if ((actor->status.flags & 0x400) == 0) {
            return;
        }
        entryFlags = btlGetEntryFlagsUnlessDisabled(&actor->partyRecord);
        if ((actor->status.flags & 0x40) == 0 && !(entryFlags & 0x200)) {
            return;
        }
        fldUpdateSceneGroupTask(task);
        btlRemoveTaskFromSceneGroup(task);
        if ((entryFlags & 0x200) && (actor->status.flags & 0x40) == 0) {
            btlDispatchStateHandler(task, 1);
        } else {
            btlDispatchStateHandler(task, 0x1E);
        }
    }
}

/* Empty entry callback for the retail DEAD action state. */
void btlEnterDeadActionState(void) {
}

void btlCommandTaskReleaseActor(BtlTask *task) {
    if ((task->flags & 8) != 0) {
        if (btlPollActorResourceCleanup(task->unit) == BTL_ACTOR_RESOURCE_CLEANUP_PENDING) {
            return;
        }
        if (task->unit != 0) {
            btlReleaseUnitResources(task->unit);
            {
                BtlUnit *fx = task->unit;
                fx->baseColor = 0x80808080;
                fx->overlayColor = 0x80808080;
                fx->status.flags = (u32)fx->status.flags & 0x700;
                btlInitUnitFxDefaults(fx);
            }
            {
                BtlUnit *model = task->unit;
                SDF_VU0_COPYVECTOR(model->currentPosition, model->position);
                SDF_VU0_COPYVECTOR(model->orientation, model->rotation);
                model->selectedEntryIndex = -1;
            }
            task->unit = 0;
            task->flags &= ~8;
        }
    }
    btlClearSceneTaskActiveFlag(task);
    task->flags |= 2;
}

void btlFlagLinkedActorActionInProgress(BtlTask *unit) {
    unit->unit->status.flags |= 0x4000;
}
void btlRunHookAndAdvanceUnitState(BtlTask *task) {
    void (*callback)(BtlTask *) = *(void (**)(BtlTask *))((u8 *)btlGetRuntime() + 0x660);
    if (callback != 0) {
        callback(task);
    }
    btlDispatchStateHandler(task, 0x1A);
}

/* Empty entry callback for the retail DEBUG action state. */
void btlEnterDebugActionState(void) {
}

/* The retail DEBUG state's update callback performs no work. */
void btlUpdateDebugActionState(void) {
}

/* Initialize the battle debug-menu tables on entry to the retail TEST state. */
void btlEnterTestActionState(void) {
    btlInitializeBattleDebugMenu();
}

void btlAdvanceUnitWhenActionGateClears(BtlTask *action) {
    s32 temp_v0;

    temp_v0 = btlUpdateBattleModelDebugEditor();
    if (temp_v0 == 0) {
        btlDispatchStateHandler(action, 6);
        return;
    }
}

extern BtlActionStateHandler D_00359B28[];

void btlDispatchStateHandler(void *object, BtlActionStateIndex index) {
    BtlActionStateOwner *owner = object;
    owner->state = index;
    owner->stateTime = 0;
    D_00359B28[index].start(owner);
}

extern char D_003A3788[]; /* "btl:action seq create[%p]\n" */

BtlTask *btlCreateActionSeq(void) {
    BtlTask *sequence = sdfAllocAndClearQuadwords(sizeof(*sequence));
    BtlState *context;
    BtlTask *head;

    sequence->actionNumber = 1;
    btlInitBattleIndexWork(&sequence->indexWork);
    context = (BtlState *)btlGetRuntime();
    sequence->prev = 0;
    head = context->tasks;
    if (head != 0) {
        head->prev = sequence;
        sequence->next = context->tasks;
    } else {
        sequence->next = 0;
    }
    context->tasks = sequence;
    btlDispatchStateHandler(sequence, 0);
    btlBossDebugPrintf(D_003A3788, sequence);
    return sequence;
}

extern char D_003A37A8[]; /* "btl:action seq delete[%p]\n" */

void btlDestroyActionSeq(BtlTask *object) {
    btlBossDebugPrintf(D_003A37A8, object);
    btlReleaseObjectBuffers(&object->indexWork);
    if (object->next != 0) {
        object->next->prev = object->prev;
    }
    if (object->prev != 0) {
        object->prev->next = object->next;
    } else {
        BtlState *context = (BtlState *)btlGetRuntime();
        context->tasks = object->next;
    }
    sdfChipFreeDirect(object);
}

void btlUpdateActionSeqs(void) {
    BtlTask *action = ((BtlState *)btlGetRuntime())->tasks;
    while (action != 0) {
        s32 flags = action->flags;
        BtlTask *next = action->next;
        if (flags & 1) {
            D_00359B28[action->state].update(action);
            action->stateTime += 1;
        } else if (flags & 2) {
            btlDestroyActionSeq(action);
        }
        action = next;
    }
}

void btlDestroyAllActionSeqs(void) {
    BtlTask *node = ((BtlState *)btlGetRuntime())->tasks;
    while (node != 0) {
        BtlTask *next = node->next;
        btlDestroyActionSeq(node);
        node = next;
    }
}

BtlTask *btlFindUnitByActor(BtlUnit *target) {
    BtlTask *node;

    node = ((BtlState *)btlGetRuntime())->tasks;
    while (node != 0) {
        if (node->unit == target) {
            return node;
        }
        node = node->next;
    }
    return 0;
}

extern void btlBossDebugPrintfN(s32, s32, s32, s32, ...);

extern char D_003BB5E0[];

extern char D_003BB5E8[];

extern char D_003A37C8[];

/* Display the eight slot entries, retaining each group's last valid task. */
void btlDebugPrintActionOrder(s32 x, s32 y) {
    BtlState *controller = (BtlState *)btlGetRuntime();
    BtlTask **primary;
    BtlTask **secondary;
    BtlTask **tertiary;
    BtlTask *task;
    s32 color;
    u32 i;

    if ((controller->battleFlags & 4) == 0) {
        return;
    }
    btlBossDebugPrintfN(x, y, 0, (s32)D_003A37C8);
    primary = controller->groupPrimary;
    secondary = controller->groupSecondary;
    tertiary = controller->groupTertiary;
    for (i = 0; i < 8; i++) {
        switch (controller->slots[i].group) {
        case 1:
            color = 0;
            task = *primary;
            if (primary[1] != NULL) {
                primary++;
            }
            break;
        case 2:
            task = *secondary;
            color = 5;
            if (secondary[1] != NULL) {
                secondary++;
            }
            break;
        case 3:
            task = *tertiary;
            color = 6;
            if (tertiary[1] != NULL) {
                tertiary++;
            }
            break;
        default:
            task = NULL;
            color = 0;
            break;
        }
        if (task == NULL || task->unit == NULL) {
            continue;
        }
        if (controller->slots[i].remaining == 100) {
            btlBossDebugPrintfN(x, y + (i + 1) * 12, color, (s32)D_003BB5E0,
                               D_00359B28[task->state].name);
        } else {
            btlBossDebugPrintfN(x, y + (i + 1) * 12, color, (s32)D_003BB5E8,
                               D_00359B28[task->state].name);
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3738);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3748);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3758);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3768);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3778);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3788);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A37A8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A37C8);

INCLUDE_SDATA(const s32, "game/code_001C8890", D_003BB5E0);

INCLUDE_SDATA(const s32, "game/code_001C8890", D_003BB5E8);

INCLUDE_SDATA(const s32, "game/code_001C8890", btlDeferredTaskTail);
