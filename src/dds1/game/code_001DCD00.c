#include "btl_motion_transform.h"
#include "btl_camera_pose_api.h"
#include "common.h"
#include "btl_unit_geometry_api.h"
#include "btl_command_target_policy.h"
#include "btl_actor_category_flag_api.h"
#include "sdf_vu0.h"
#include "btl_actor_status_table_api.h"
#include "eff_random_mod.h"
#include "eff_random.h"
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
#include "btl_effect_node.h"
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
    BTL_UNIT_STATUS_FLAG_ACTIVE = 1,
    BTL_UNIT_STATUS_FLAG_ALLY = 0x200,
    BTL_UNIT_STATUS_FLAG_ENEMY = 0x400,
    BTL_UNIT_STATUS_SIDE_MASK = 0x600
};

extern s32 btlHasSingleLinkedResource(BtlLinkedCommand *);

extern void btlResetCameraMotion(BtlLinkedCommand *);


extern DatEnemyRecord *datEnemyRecords;

extern void fldCamUpdateDefault(BtlLinkedCommand *, BtlCamState *);

extern void btlPrepareUnitPoseWithTiltRotation(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern BtlActionAnimationRecord *datActionAnimationRecords;

extern EffRandState effSharedRandomState[];

extern EffRandState effSharedRandomState[];


extern char D_003BB5F8[];

extern u32 D_003BB660;

extern s32 D_003BB668;

extern s32 D_003BB664;

void btlAimLinkedUnitAtMuzzle(BtlLinkedCommand *action);

/* Allocate a native header followed by capacity pointer entries. */ BtlIndexList *btlAllocateIndexList(s32 capacity);

void btlFaceActionParticipantsTowardLinkedTarget(BtlLinkedCommand *action);

void btlFreeIndexList(BtlIndexList *list);

/* Return the number of live entries, not the allocated capacity. */ u32 btlGetIndexListCount(BtlIndexList *list);

/* The caller supplies an in-range index. */ BtlUnit *btlGetIndexListEntry(BtlIndexList *list, s32 index);

s32 btlMatchFirstLinkedActorFlags(BtlLinkedCommand *action) {
    BtlTask *linked;

    switch (action->status) {
    case 5:
    case 6:
        break;
    default:
        return 0;
    }
    linked = action->task;
    if (btlGetIndexListCount(linked->indexWork.indices) >= 2) {
        return 0;
    }
    return btlGetIndexListEntry(linked->indexWork.indices, 0) == linked->unit;
}

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
    if (category != 0 && (datActionAnimationRecords[category].flags & 1)) {
        return 0;
    }
    return 1;
}

/* Return whether a live linked group has its byte at 0x10 marked. */
u32 btlHasMarkedEntry10(BtlLinkedCommand *command) {
    BtlTask *resource = command->task;
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
extern f32 btlUnitGetTopY(BtlUnit *);

s32 btlCheckActorDistanceLimit(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *actor = state->units;

    while (actor != 0) {
        u32 flags = (u32)actor->status.flags;
        if (flags & BTL_UNIT_STATUS_FLAG_ACTIVE) {
            if (flags & BTL_UNIT_STATUS_FLAG_ENEMY) {
                if (btlUnitGetTopY(actor) > 400.0f) {
                    return 0;
                }
            }
        }
        actor = actor->next;
    }
    return 1;
}

/* With a qualifying unit, the span query leaves its last muzzle point in VF10. */
extern f32 btlMeasureMaskedUnitBounds(s32, f32 *, f32 *);

s32 btlIsEntryHeightWithinLimit(void) {
    if (btlMeasureMaskedUnitBounds(BTL_UNIT_STATUS_FLAG_ENEMY, 0, 0) > 600.0f) {
        return 0;
    }
    return 1;
}

/* Find an unmarked linked kind-two slot whose unit is not disabled. */
s32 btlHasIdleLinkedSlotKindTwo(BtlLinkedCommand *actor) {
    BtlTask *linked = actor->task;
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
s32 btlHasEligibleLinkedEntryTypeTwo(BtlLinkedCommand *actor) {
    BtlTask *linked = actor->task;
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
    if ((datActionAnimationRecords[index].flags & 0x100) == 0) {
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

BtlActorCategoryFlagUseResult btlCanUseActorCategoryFlag2(BtlLinkedCommand *actor) {
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
    if ((datActionAnimationRecords[index].flags & 2) != 0) {
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

BtlActorCategoryFlagUseResult btlCanUseActorCategoryFlag4(BtlLinkedCommand *actor) {
    s32 index;
    if (btlCanUseLinkedActor(actor) == 0) {
        return 0;
    }
    index = actor->actionCode;
    if (index == 0) {
        return 0;
    }
    if ((datActionAnimationRecords[index].flags & 4) != 0) {
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
    if ((datActionAnimationRecords[index].flags & 0x40) == 0) {
        return 0;
    }
    return 1;
}

s32 btlMatchLinkedActorFlags(BtlLinkedCommand *action) {
    BtlTask *linked;
    BtlUnit *entry;

    switch (action->status) {
    case 4:
    case 5:
    case 6:
        break;
    default:
        return 0;
    }
    linked = action->task;
    if (linked == 0) {
        return 0;
    }
    if (btlGetIndexListCount(linked->indexWork.indices) >= 2) {
        return 0;
    }
    entry = btlGetIndexListEntry(linked->indexWork.indices, 0);
    return (((u32)linked->unit->status.flags ^ (u32)entry->status.flags) & BTL_UNIT_STATUS_SIDE_MASK) == 0;
}

s32 btlHasFirstLinkedCategoryFlag1000(BtlLinkedCommand *node) {
    BtlTask *resource = node->task;
    BtlUnit *actor;
    u32 id;
    if (resource == 0) return 0;
    if (btlGetIndexListCount(resource->indexWork.indices) >= 2) return 0;
    actor = btlGetIndexListEntry(resource->indexWork.indices, 0);
    if ((actor->status.flags & BTL_UNIT_STATUS_FLAG_ENEMY) == 0) return 0;
    id = actor->species;
    if (id >= 0x180) return 0;
    if (datEnemyRecords[id].flags & 0x1000) return 1;
    return 0;
}

/* Skill 0x5F is Fire of Sinai; linked-camera selection treats it specially. */
u8 btlIsLinkedFireOfSinaiCommand(BtlLinkedCommand *action) {
    return action->actionCode == 0x5f;
}

/* Skill 0x1A0 is Item Drop in this game's skill-name table. */
u8 btlIsLinkedItemDropCommand(BtlLinkedCommand *action) {
    return action->actionCode == 0x1a0;
}

void btlInitializeFreeCameraNoOp(void) {
}

void btlUpdateFreeCameraNoOp(void) {
}

extern void btlPrepareActionCameraPoseWithActorClearance(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void btlUpdateTiltedActionCameraPose(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void btlFrameLeftmostUnitCameraPair(BtlCamState *, BtlCamState *, BtlCamState *);

/* Choose the action's camera pose from active ally and enemy height maxima. */
void btlChooseCameraPoseByActorHeights(BtlLinkedCommand *action) {
    BtlUnit *unit;
    s32 enemyCount = 0;
    f32 enemyHeight = 0.0f;
    f32 allyHeight = 0.0f;
    f32 height;

    unit = ((BtlState *)btlGetRuntime())->units;
    for (; unit != NULL; unit = unit->next) {
        if (unit->status.flags & BTL_UNIT_STATUS_FLAG_ACTIVE) {
            height = btlUnitGetTopY(unit);
            if (unit->status.flags & BTL_UNIT_STATUS_FLAG_ALLY) {
                if (allyHeight < height) {
                    allyHeight = height;
                }
            } else if (unit->status.flags & BTL_UNIT_STATUS_FLAG_ENEMY) {
                if (enemyHeight < height) {
                    enemyHeight = height;
                }
                enemyCount++;
            }
        }
    }
    if (enemyCount == 1 && allyHeight + 100.0f < enemyHeight) {
        switch (dds3GetRandIntA(0, 4)) {
        case 0:
        case 1:
            if (enemyHeight <= 500.0f) {
                btlFrameLeftmostUnitCameraPair(&action->camera, &action->frontCamera, &action->backCamera);
            } else {
                btlPrepareActionCameraPoseWithActorClearance(action, &action->frontCamera, &action->backCamera);
            }
            break;
        case 2:
            btlPrepareActionCameraPoseWithActorClearance(action, &action->frontCamera, &action->backCamera);
            break;
        case 3:
            btlUpdateTiltedActionCameraPose(action, &action->frontCamera, &action->backCamera);
            break;
        }
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            btlPrepareActionCameraPoseWithActorClearance(action, &action->frontCamera, &action->backCamera);
            break;
        case 1:
            btlUpdateTiltedActionCameraPose(action, &action->frontCamera, &action->backCamera);
            break;
        }
    }
    action->motionParameter = 100.0f;
    action->flags |= 0x41;
}

void btlUpdateEncounterCameraNoOp(void) {
}

/* Prepare the embedded primary camera through the existing 27.5f provider. */
void btlPreparePrimaryActionCamera(BtlLinkedCommand *command) {
    fldCamUpdateDefault(command, &command->camera);
}

void btlUpdatePrimaryActionCameraNoOp(void) {
}

extern void btlFlagUserAndTargetDefeat(BtlLinkedCommand *, BtlLinkedCommand *);

extern void btlInitTargetCursorAndFacing(BtlLinkedCommand *, BtlCamState *);

extern void btlInitializeCameraCursorForActorMode(BtlLinkedCommand *, BtlCamState *);

extern void btlFrameCommandTargetGroups(BtlLinkedCommand *);

extern void btlSetupActionCameraPair(BtlLinkedCommand *);

void btlInitializeLinkedActionCamera(BtlLinkedCommand *action) {
    s32 (*hook)(BtlLinkedCommand *) = ((BtlState *)btlGetRuntime())->cameraStateChangePredicate;
    BtlTask *link;

    action->stepKind = BTL_LINKED_CAMERA_STEP_NONE;
    link = action->task;
    if (hook != NULL && hook(action) != 0) {
        return;
    }
    if (link->unit->status.flags & BTL_UNIT_STATUS_FLAG_ALLY) {
        if (link->unit->status.flags & 0x1000) {
            if (btlHasSingleLinkedResource(action)) {
                action->stepKind = BTL_LINKED_CAMERA_STEP_APPROACH_POSE;
                btlFlagUserAndTargetDefeat(action, action);
            } else {
                btlInitTargetCursorAndFacing(action, &action->camera);
            }
        } else {
            action->stepKind = BTL_LINKED_CAMERA_STEP_ADVANCE_UNMARKED_CURSOR;
            btlInitializeCameraCursorForActorMode(action, &action->camera);
        }
    } else {
        if (btlMatchLinkedActorFlags(action)) {
            btlFrameCommandTargetGroups(action);
        } else if (btlHasSingleLinkedResource(action)) {
            action->stepKind = BTL_LINKED_CAMERA_STEP_TARGET_CAMERA_UPDATE;
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

extern void btlBuildApproachCamera(BtlLinkedCommand *, BtlCamState *);

extern void btlUpdateActionTargetCameraPose(BtlLinkedCommand *);

extern void btlAdvanceCursorForUnmarkedUnit(BtlLinkedCommand *, BtlCamState *);

/* Let the battle hook intercept the cursor step, then dispatch approach/target/advance kinds. */
void btlDispatchActionCursorStepByKind(BtlLinkedCommand *actor) {
    s32 (*callback)(BtlLinkedCommand *) = *(s32 (**)(BtlLinkedCommand *))((u8 *)btlGetRuntime() + 0x614);

    if (callback != 0 && callback(actor) != 0) {
        return;
    }

    switch (actor->stepKind) {
    case BTL_LINKED_CAMERA_STEP_APPROACH_POSE:
        btlBuildApproachCamera(actor, &actor->camera);
        break;
    case BTL_LINKED_CAMERA_STEP_TARGET_CAMERA_UPDATE:
        btlUpdateActionTargetCameraPose(actor);
        break;
    case BTL_LINKED_CAMERA_STEP_ADVANCE_UNMARKED_CURSOR:
        btlAdvanceCursorForUnmarkedUnit(actor, &actor->camera);
        break;
    }
}

extern void btlBuildGroupSizeActionCameraPair(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlBuildFilteredGroupCameraPair(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlBuildRandomTargetCameraStates(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlBuildActorPairFramingCameraPose(BtlLinkedCommand *, BtlCamState *, u8);
extern void btlPrepareCameraMode6Participants(BtlLinkedCommand *, BtlCamState *);
extern void btlPrepareCameraMode7Participants(BtlLinkedCommand *, BtlCamState *);
extern void btlSetupCameraPoseAimUnit(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlConfigureBattleCameraAction(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlBuildSideSelectedActorCameraBlend(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlChooseActionPoseBlendFromActorCount(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlBuildDistanceClampedActionCameraBlend(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlBuildRandomizedActionCameraPosePair(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlPrepareRandomizedActionCameraPose(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

void btlChooseLinkedActionCameraPose(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlTask *task;
    s8 hasFlag200;
    s8 hasFlag400;
    u32 i;
    u32 count;

    action->stepKind = BTL_LINKED_CAMERA_STEP_NONE;
    task = action->task;
    if (btlHasActorCategoryFlag40(action) != 0) {
        fldCamUpdateDefault(action, &action->camera);
        action->flags |= 0x800;
    } else {
        hasFlag200 = 0;
        hasFlag400 = 0;
        count = btlGetIndexListCount(task->indexWork.indices);
        for (i = 0; i < count; i++) {
            BtlUnit *entry = (BtlUnit *)btlGetIndexListEntry(task->indexWork.indices, i);
            if (entry->status.flags & BTL_UNIT_STATUS_FLAG_ALLY) {
                hasFlag200 = 1;
            }
            if (entry->status.flags & BTL_UNIT_STATUS_FLAG_ENEMY) {
                hasFlag400 = 1;
            }
        }
        if (work->cameraPoseBlendHook != 0 && work->cameraPoseBlendHook(action, hasFlag200, hasFlag400) != 0) {
            return;
        }
        if (task->unit->status.flags & BTL_UNIT_STATUS_FLAG_ALLY) {
            if ((hasFlag200 == 1 || hasFlag400 != 1) && btlCanUseActorCategoryFlag4(action) == 0) {
                if (btlHasSingleLinkedResource(action) == 0) {
                    if (dds3GetRandA((EffRandState *)effSharedRandomState) & 1) {
                        btlBuildGroupSizeActionCameraPair(action, &action->frontCamera, &action->backCamera);
                    } else {
                        btlBuildFilteredGroupCameraPair(action, &action->frontCamera, &action->backCamera);
                    }
                    action->flags |= 0x800;
                } else if (btlMatchFirstLinkedActorFlags(action) != 0) {
                    btlBuildRandomTargetCameraStates(action, &action->frontCamera, &action->backCamera);
                    action->motionParameter = 30.0f;
                    action->flags |= 0x841;
                } else {
                    btlBuildActorPairFramingCameraPose(action, &action->frontCamera, 3);
                    btlCopyMotionTransform(&action->backCamera, &action->frontCamera);
                    action->backCamera.distance += -100.0f;
                    action->motionParameter = 50.0f;
                    action->flags |= 0x841;
                    btlAdjustCameraDirectionForDefaultPlane(&action->backCamera);
                }
            } else if (btlHasLinkedEffectNodeTrigger(action) != 0) {
                switch (datActionAnimationRecords[action->actionCode].cameraKind) {
                case 6:
                    action->stepKind = BTL_LINKED_CAMERA_STEP_MODE_6_APPROACH_POSE;
                    btlPrepareCameraMode6Participants(action, &action->camera);
                    break;
                case 7:
                    action->stepKind = BTL_LINKED_CAMERA_STEP_MODE_7_APPROACH_POSE;
                    btlPrepareCameraMode7Participants(action, &action->camera);
                    break;
                default:
                    action->stepKind = BTL_LINKED_CAMERA_STEP_APPROACH_POSE;
                    btlFlagUserAndTargetDefeat(action, action);
                    break;
                }
                action->flags |= 0x800;
            } else {
                if (btlCanUseActorCategoryFlag2(action) != 0 || btlCanUseActorCategoryFlag4(action) != 0 ||
                    btlIsLinkedFireOfSinaiCommand(action) != 0) {
                    action->stepKind = BTL_LINKED_CAMERA_STEP_INITIAL_POSE_SELECTION;
                } else if (btlIsEntryHeightWithinLimit() == 0 && btlHasSingleLinkedResource(action) == 0) {
                    action->stepKind = BTL_LINKED_CAMERA_STEP_INITIAL_POSE_SELECTION;
                } else {
                    action->stepKind = (dds3GetRandA((EffRandState *)effSharedRandomState) & 1)
                        ? BTL_LINKED_CAMERA_STEP_INITIAL_POSE_SELECTION
                        : BTL_LINKED_CAMERA_STEP_SIDE_SELECTED_ACTOR_BLEND;
                }
                switch (action->stepKind) {
                case BTL_LINKED_CAMERA_STEP_INITIAL_POSE_SELECTION:
                    switch (datActionAnimationRecords[action->actionCode].cameraKind) {
                    case 6:
                    case 7:
                        btlSetupCameraPoseAimUnit(action, &action->frontCamera, &action->backCamera);
                        action->stepKind = BTL_LINKED_CAMERA_STEP_AIMED_ACTION_POSE;
                        break;
                    default:
                        btlConfigureBattleCameraAction(action, &action->frontCamera, &action->backCamera);
                        break;
                    }
                    break;
                case BTL_LINKED_CAMERA_STEP_SIDE_SELECTED_ACTOR_BLEND:
                    btlBuildSideSelectedActorCameraBlend(action, &action->frontCamera, &action->backCamera);
                    break;
                }
            }
        } else {
            if (btlIsLinkedItemDropCommand(action) != 0) {
                btlChooseActionPoseBlendFromActorCount(action, &action->frontCamera, &action->backCamera);
                action->flags |= 0x800;
            } else if (btlHasSingleLinkedResource(action) != 0 && btlHasLinkedEffectNodeTrigger(action) != 0) {
                action->stepKind = BTL_LINKED_CAMERA_STEP_ACTION_END_PULLBACK;
                btlBuildDistanceClampedActionCameraBlend(action, &action->frontCamera, &action->backCamera);
            } else if (btlCanUseActorCategoryFlag4(action) != 0 && (hasFlag200 != 1 || hasFlag400 == hasFlag200)) {
                btlBuildRandomizedActionCameraPosePair(action, &action->frontCamera, &action->backCamera);
                action->flags |= 0x800;
            } else if ((hasFlag200 != 1 || hasFlag400 == hasFlag200) &&
                       (btlCanUseActorCategoryFlag2(action) != 0 ||
                        ((dds3GetRandA((EffRandState *)effSharedRandomState) & 1) && btlIsActorCategoryTypeTwo(action) == 0))) {
                btlBuildGroupSizeActionCameraPair(action, &action->frontCamera, &action->backCamera);
                action->flags |= 0x800;
            } else {
                if (btlHasActorCategoryFlag100(action) == 0 && btlCanUseActorCategoryFlag2(action) == 0 &&
                    btlCanUseActorCategoryFlag4(action) == 0 && (hasFlag200 == 1 || hasFlag400 != 1)) {
                    action->stepKind = (dds3GetRandA((EffRandState *)effSharedRandomState) & 1)
                        ? BTL_LINKED_CAMERA_STEP_AIM_EFFECT_POSE
                        : BTL_LINKED_CAMERA_STEP_ACTION_END_PULLBACK;
                } else {
                    action->stepKind = BTL_LINKED_CAMERA_STEP_AIM_EFFECT_POSE;
                }
                if (btlHasActorCategoryFlag100(action) != 0) {
                    btlFaceActionParticipantsTowardLinkedTarget(action);
                }
                switch (action->stepKind) {
                case BTL_LINKED_CAMERA_STEP_AIM_EFFECT_POSE:
                    btlPrepareRandomizedActionCameraPose(action, &action->frontCamera, &action->backCamera);
                    break;
                case BTL_LINKED_CAMERA_STEP_ACTION_END_PULLBACK:
                    btlBuildDistanceClampedActionCameraBlend(action, &action->frontCamera, &action->backCamera);
                    action->motionProgress = 1;
                    break;
                }
            }
            btlResetCameraMotion(action);
        }
    }
}

extern void btlRefreshActionPoseBlendSnapshot();

extern void btlAimEffectPoseAtUnit();

extern void btlBeginActionEndCameraPullback();

extern void btlUpdateApproachCameraPose(BtlLinkedCommand *, BtlCamState *);

extern void btlBuildHeightClampedApproachCamera(BtlLinkedCommand *, BtlCamState *);

void btlDispatchActionCameraStep(BtlLinkedCommand *action) {
    BtlState *runtime = (BtlState *)btlGetRuntime();
    s32 (*callback)(BtlLinkedCommand *) = runtime->actionCameraStepHook;

    if (callback != 0 && callback(action) != 0) {
        return;
    }
    switch (action->stepKind) {
    case BTL_LINKED_CAMERA_STEP_SIDE_SELECTED_ACTOR_BLEND:
        btlRefreshActionPoseBlendSnapshot(action, action);
        break;
    case BTL_LINKED_CAMERA_STEP_APPROACH_POSE:
        btlBuildApproachCamera(action, &action->camera);
        break;
    case BTL_LINKED_CAMERA_STEP_AIM_EFFECT_POSE:
        btlAimEffectPoseAtUnit(action, action);
        break;
    case BTL_LINKED_CAMERA_STEP_ACTION_END_PULLBACK:
        btlBeginActionEndCameraPullback(action, action);
        break;
    case BTL_LINKED_CAMERA_STEP_MODE_6_APPROACH_POSE:
        btlUpdateApproachCameraPose(action, &action->camera);
        break;
    case BTL_LINKED_CAMERA_STEP_MODE_7_APPROACH_POSE:
        btlBuildHeightClampedApproachCamera(action, &action->camera);
        break;
    }
}

extern void btlAdvancePlayerCursorAnimation(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvanceTargetCursorAnimation(BtlLinkedCommand *, BtlCamState *);


extern void btlBuildFilteredGroupCameraPair(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlBuildRandomTargetCameraStates(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlBuildActorPairFramingCameraPose(BtlLinkedCommand *, BtlCamState *, u8);
extern void btlBuildTargetGroupFramingCameraPose(BtlLinkedCommand *, BtlCamState *);
extern void btlInitCommandCursorForFirstActor(BtlLinkedCommand *, BtlCamState *);
extern void btlInitTargetCursorAndFacing(BtlLinkedCommand *, BtlCamState *);
extern void btlBuildRandomizedActionCameraPosePair(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

/* Same chooser: DDS1 001DE048..001DE08C and 001DE218..001DE248;
 * DDS2 twins 001EB728..001EB76C and 001EB8F8..001EB928, plus independent
 * DDS2 sites 0021D3E8..0021D418 and 0022487C..002248B0.
 */
static inline s32 btlChooseLinkedCameraVariant(BtlLinkedCommand *action, s32 *shrinkDistance) {
    s32 choice;

    if (btlCanUseLinkedActor(action)) {
        if (btlCanUseActorCategoryFlag2(action)) {
            if (shrinkDistance != NULL) {
                *shrinkDistance = 0;
            }
            return 0;
        }
        choice = dds3GetRandIntA(NULL, 2);
        if (shrinkDistance != NULL) {
            *shrinkDistance = 1;
        }
        return choice;
    }
    if (shrinkDistance != NULL) {
        *shrinkDistance = 0;
    }
    return 1;
}

void btlInitializeActionCamera(BtlLinkedCommand *action) {
    BtlState *state;
    BtlTask *task;
    BtlUnit *unit;
    u32 count;
    s32 i;
    s32 hasFlag200;
    s32 hasFlag400;
    s32 choice;
    s32 reduceDistance;

    state = (BtlState *)btlGetRuntime();
    task = action->task;
    action->stepKind = BTL_LINKED_CAMERA_STEP_NONE;
    if (btlHasActorCategoryFlag100(action)) {
        return;
    }
    hasFlag200 = 0;
    hasFlag400 = 0;
    count = btlGetIndexListCount(task->indexWork.indices);
    for (i = 0; i < count; i++) {
        unit = btlGetIndexListEntry(task->indexWork.indices, i);
        if (unit->status.flags & BTL_UNIT_STATUS_FLAG_ALLY) {
            hasFlag200 = 1;
        }
        if (unit->status.flags & BTL_UNIT_STATUS_FLAG_ENEMY) {
            hasFlag400 = 1;
        }
    }
    if (state->handleActorCategoryCamera != NULL &&
        state->handleActorCategoryCamera(action, hasFlag200, hasFlag400)) {
        return;
    }

    if (task->unit->status.flags & BTL_UNIT_STATUS_FLAG_ALLY) {
        if (btlHasSingleLinkedResource(action)) {
            if (hasFlag200 != 1 && hasFlag400 == 1) {
                if (btlIsActorCategoryMarked(action)) {
                    btlInitCommandCursorForFirstActor(action, &action->camera);
                    action->stepKind = BTL_LINKED_CAMERA_STEP_ADVANCE_TARGET_CURSOR_DDS1;
                } else if (btlCanUseActorCategoryFlag4(action)) {
                    btlBuildRandomizedActionCameraPosePair(action, &action->frontCamera, &action->backCamera);
                } else {
                    choice = btlChooseLinkedCameraVariant(action, &reduceDistance);
                    if (choice == 0) {
                        btlBuildRandomTargetCameraStates(action, &action->frontCamera, &action->backCamera);
                        action->motionParameter = 30.0f;
                        action->flags |= 0x41;
                    } else {
                        f32 distance;

                        btlBuildActorPairFramingCameraPose(action, &action->frontCamera, 1);
                        btlCopyMotionTransform(&action->backCamera, &action->frontCamera);
                        if (reduceDistance) {
                            action->frontCamera.distance *= 0.75f;
                        }
                        distance = action->backCamera.distance;
                        action->flags |= 0x41;
                        action->motionParameter = 35.0f;
                        action->backCamera.distance = distance + 150.0f;
                        btlAdjustCameraDirectionForDefaultPlane(&action->backCamera);
                    }
                }
            } else if (hasFlag200 == 1 && hasFlag400 != hasFlag200) {
                if (btlCanUseActorCategoryFlag4(action)) {
                    btlBuildRandomizedActionCameraPosePair(action, &action->frontCamera, &action->backCamera);
                } else if (btlMatchFirstLinkedActorFlags(action)) {
                    btlBuildRandomTargetCameraStates(action, &action->frontCamera, &action->backCamera);
                    action->motionParameter = 30.0f;
                    action->flags |= 0x41;
                } else {
                    f32 distance;

                    btlBuildActorPairFramingCameraPose(action, &action->frontCamera, 3);
                    btlCopyMotionTransform(&action->backCamera, &action->frontCamera);
                    distance = action->backCamera.distance;
                    action->flags |= 0x41;
                    action->motionParameter = 50.0f;
                    action->backCamera.distance = distance + (-100.0f);
                    btlAdjustCameraDirectionForDefaultPlane(&action->backCamera);
                }
            } else {
                btlBuildBattleUnitFramingCameraPose(action, &action->camera, 20.0f);
            }
        } else if (hasFlag200 != 1 && hasFlag400 == 1) {
            if (btlIsLinkedFireOfSinaiCommand(action)) {
                btlInitTargetCursorAndFacing(action, &action->camera);
            } else if (btlCanUseActorCategoryFlag4(action)) {
                btlBuildRandomizedActionCameraPosePair(action, &action->frontCamera, &action->backCamera);
            } else {
                choice = btlChooseLinkedCameraVariant(action, NULL);
                if (choice != 0) {
                    f32 distance;

                    btlBuildTargetGroupFramingCameraPose(action, &action->frontCamera);
                    btlCopyMotionTransform(&action->backCamera, &action->frontCamera);
                    distance = action->backCamera.distance;
                    action->flags |= 0x41;
                    action->motionParameter = 30.0f;
                    action->backCamera.distance = distance + 100.0f;
                    btlAdjustCameraDirectionForDefaultPlane(&action->backCamera);
                } else {
                    btlBuildFilteredGroupCameraPair(action, &action->frontCamera, &action->backCamera);
                }
            }
        } else if (hasFlag200 == 1 && hasFlag400 != hasFlag200) {
            if (btlCanUseActorCategoryFlag4(action)) {
                btlBuildRandomizedActionCameraPosePair(action, &action->frontCamera, &action->backCamera);
            } else {
                btlBuildFilteredGroupCameraPair(action, &action->frontCamera, &action->backCamera);
            }
        } else {
            btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
            action->motionParameter = 200.0f;
            action->flags |= 0x41;
        }
    } else if (hasFlag200 == 1 && hasFlag400 != 1) {
        if (btlHasSingleLinkedResource(action)) {
            if (btlCanUseActorCategoryFlag4(action)) {
                btlBuildRandomizedActionCameraPosePair(action, &action->frontCamera, &action->backCamera);
            } else if (btlCanUseLinkedActor(action) &&
                       ((dds3GetRandA((EffRandState *)effSharedRandomState) & 1) ||
                        btlCanUseActorCategoryFlag2(action))) {
                btlBuildRandomTargetCameraStates(action, &action->frontCamera, &action->backCamera);
                action->motionParameter = 30.0f;
                action->flags |= 0x41;
            } else {
                f32 distance;

                btlBuildActorPairFramingCameraPose(action, &action->frontCamera, 2);
                btlCopyMotionTransform(&action->backCamera, &action->frontCamera);
                distance = action->backCamera.distance;
                action->flags |= 0x41;
                action->motionParameter = 35.0f;
                action->backCamera.distance = distance + 150.0f;
                btlAdjustCameraDirectionForDefaultPlane(&action->backCamera);
            }
        } else if (btlCanUseActorCategoryFlag4(action)) {
            btlBuildRandomizedActionCameraPosePair(action, &action->frontCamera, &action->backCamera);
        } else if (btlCanUseLinkedActor(action) &&
                   ((dds3GetRandA((EffRandState *)effSharedRandomState) & 1) ||
                    btlCanUseActorCategoryFlag2(action))) {
            btlBuildFilteredGroupCameraPair(action, &action->frontCamera, &action->backCamera);
        } else {
            btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
            btlAimLinkedUnitAtMuzzle(action);
            action->motionParameter = 200.0f;
            action->flags |= 0x41;
        }
    } else if (hasFlag200 != 1 && hasFlag400 == 1) {
        if (btlCanUseActorCategoryFlag4(action)) {
            btlBuildRandomizedActionCameraPosePair(action, &action->frontCamera, &action->backCamera);
        } else {
            btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
            action->motionParameter = 200.0f;
            action->flags |= 0x41;
        }
    } else {
        btlBuildBattleUnitFramingCameraPose(action, &action->camera, 20.0f);
    }
}

void btlAdvanceTargetCursorUnlessHookHandles(BtlLinkedCommand *actor) {
    s32 (*callback)(BtlLinkedCommand *) = *(s32 (**)(BtlLinkedCommand *))((u8 *)btlGetRuntime() + 0x634);
    if (callback != 0 && callback(actor) != 0) {
        return;
    }
    if (actor->stepKind == BTL_LINKED_CAMERA_STEP_ADVANCE_TARGET_CURSOR_DDS1) {
        btlAdvanceTargetCursorAnimation(actor, &actor->camera);
    }
}

extern void btlApplyMarkedUnitActionCamera(BtlLinkedCommand *, BtlCamState *, BtlUnit *, s32);

void btlUpdateActionPoseForLinkedTarget(BtlLinkedCommand *action) {
    BtlUnit *target;

    if (((BtlState *)btlGetRuntime())->runtimeFlags & 1) {
        target = action->task->unit;
        if (target->status.flags & BTL_UNIT_STATUS_FLAG_ENEMY) {
            fldCamUpdateDefault(action, &action->camera);
            return;
        }
    }
    if (action->actionKind == action->status || action->actionKind == 0xA || (action->flags & 0x40000)) {
        btlCopyMotionTransform(&action->frontCamera, &action->camera);
        btlApplyMarkedUnitActionCamera(action, &action->backCamera, action->task->unit, 0);
        action->motionParameter = 7.0f;
        action->flags = (action->flags | 0x1041) & 0xFFFBFFFF;
    } else {
        btlApplyMarkedUnitActionCamera(action, &action->camera, action->task->unit, 0);
    }
}

void btlUpdateCommandCameraNoOp(void) {
}

extern void btlBuildActorSetCameraPose(BtlLinkedCommand *action, BtlCamState *pose, BtlUnit *startActor, BtlIndexList *indices, s8 mode);

/* Update the action camera from target-policy results once the link reaches state seven. */
void btlUpdateActionCameraFromTargetPolicy(BtlLinkedCommand *action) {
    BtlUnit *unit;
    BtlIndexList *list;
    u32 listCount;
    s8 hasUnready;
    s32 hasFlag200;
    s32 hasFlag400;
    s32 singleIsTarget;
    s32 matchingActorCount;
    BtlState *runtime;
    BtlTask *link;
    u32 i;
    u32 listResult;

    runtime = (BtlState *)btlGetRuntime();
    link = action->task;
    if (link->indexWork.phase < 0 || link->state != 7) {
        return;
    }
    list = btlAllocateIndexList(0xD);
    listResult = btlSelectCommandTargetPolicy(link, list, 0);
    listCount = btlGetIndexListCount(list);
    if (listCount != 0) {
        hasUnready = 0;
        hasFlag200 = 0;
        hasFlag400 = 0;
        for (i = 0; i < listCount; i++) {
            u32 flags = ((BtlUnit *)btlGetIndexListEntry(list, i))->status.flags;
            if ((flags & BTL_UNIT_STATUS_FLAG_ACTIVE) == 0) {
                hasUnready = 1;
            }
            if (flags & BTL_UNIT_STATUS_FLAG_ALLY) {
                hasFlag200 = 1;
            }
            if (flags & BTL_UNIT_STATUS_FLAG_ENEMY) {
                hasFlag400 = 1;
            }
        }
        singleIsTarget = 0;
        if (listCount == 1) {
            singleIsTarget = link->unit == btlGetIndexListEntry(list, 0);
        }
        matchingActorCount = 0;
        for (unit = runtime->units; unit != 0; unit = unit->next) {
            u32 flags = unit->status.flags;
            if ((flags & BTL_UNIT_STATUS_FLAG_ACTIVE) != 0 || hasUnready != 0) {
                if (flags & BTL_UNIT_STATUS_FLAG_ALLY) {
                    matchingActorCount++;
                }
            }
        }
        if (matchingActorCount == 3) {
            /* Native SDK samples; see docs/idioms.md,
             * "Camera target dispatcher: two retained position samples". */
            f32 positions[2][4];

            btlUnitGetMuzzlePosVU(link->unit);
            /* Retail 001DE74C keeps this SDK snapshot at SP+0x10. */
            SDF_VU0_STOREVECTOR(positions[1]);
            for (unit = runtime->units; unit != 0; unit = unit->next) {
                if (unit != link->unit) {
                    u32 flags = unit->status.flags;
                    if ((flags & BTL_UNIT_STATUS_FLAG_ACTIVE) != 0) {
                        if ((flags & BTL_UNIT_STATUS_FLAG_ALLY) != 0) {
                            btlUnitGetMuzzlePosVU(unit);
                            /* Retail 001DE794 keeps the ally snapshot at SP+0. */
                            SDF_VU0_STOREVECTOR(positions[0]);
                        }
                    }
                }
            }
        }
        if (action->actionKind != 9 && (action->flags & 0x40000) == 0) {
            btlApplyMarkedUnitActionCamera(action, &action->camera, link->unit, 0);
        }
        if (hasFlag200 != 1) {
            if (hasFlag400 == 1 && (action->flags & 0x40000) != 0) {
                btlCopyMotionTransform(&action->frontCamera, &action->camera);
                btlApplyMarkedUnitActionCamera(action, &action->backCamera, link->unit, 0);
                action->motionParameter = 10.0f;
                action->state = 0;
                action->flags = (action->flags | 0x41) & 0xFFFBFFFF;
            }
        } else if (hasFlag400 != hasFlag200) {
            if (singleIsTarget == 0) {
                if (action->actionKind != 9 && (action->flags & 0x40000) == 0) {
                    btlBuildActorSetCameraPose(action, &action->camera, link->unit, list, hasUnready);
                } else {
                    btlCopyMotionTransform(&action->frontCamera, &action->camera);
                    btlBuildActorSetCameraPose(action, &action->backCamera, link->unit, list, hasUnready);
                    action->motionParameter = 10.0f;
                    action->state = 0;
                    action->flags = (action->flags | 0x41) & 0xFFFBFFFF;
                }
            }
        } else if (listResult != 2) {
            if (action->actionKind == 9 || (action->flags & 0x40000) != 0) {
                btlCopyMotionTransform(&action->frontCamera, &action->camera);
                fldCamUpdateDefault(action, &action->backCamera);
                action->motionParameter = 10.0f;
                action->state = 0;
                action->flags = (action->flags | 0x41) & 0xFFFBFFFF;
            } else {
                fldCamUpdateDefault(action, &action->camera);
            }
        }
        btlFreeIndexList(list);
    }
}

void btlUpdateTargetCameraNoOp(void) {
}

extern void btlSetupCameraAimDualUnit(BtlLinkedCommand *, BtlCamState *, s32, f32, f32);

/* Prepare paired camera poses and blend flags for one target; party actors delegate. */
void btlPrepareSingleTargetCameraBlend(BtlLinkedCommand *action) {
    BtlUnit *target;
    s32 poseKind;
    f32 targetPosition[4];
    if (btlGetIndexListCount(action->targetList) != 1) {
        return;
    }
    target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
    if (action->task->unit->status.flags & BTL_UNIT_STATUS_FLAG_ALLY) {
        btlApplyMarkedUnitActionCamera(action, &action->camera, target, 0);
        return;
    }
    if (btlHasActorCategoryFlag100(action) != 0) {
        return;
    }
    btlUnitGetBodyPosVU(target);
    SDF_VU0_STOREVECTOR(targetPosition);
    if (targetPosition[0] > 0.0f) {
        poseKind = 2;
    } else {
        poseKind = 3;
    }
    btlSetupCameraAimDualUnit(action, &action->frontCamera, poseKind, 45.0f, 0.25f);
    btlSetupCameraAimDualUnit(action, &action->backCamera, poseKind, 1.0f, 0.5f);
    action->motionParameter = 30.0f;
    action->flags |= 0x41;
}

void btlUpdateSummonCameraNoOp(void) {
}

extern void btlInitSkillCommandCursor(BtlLinkedCommand *, BtlCamState *);

void btlStartLinkedActionPoseBlendIfEligible(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (action->task->unit->status.flags & BTL_UNIT_STATUS_FLAG_ENEMY) {
        if (work->cameraPoseBlendHook != 0) {
            s8 hasFlag200 = 0;
            s8 hasFlag400 = 0;
            u32 i;
            u32 count = btlGetIndexListCount(action->task->indexWork.indices);
            for (i = 0; i < count; i++) {
                BtlUnit *entry = (BtlUnit *)btlGetIndexListEntry(action->task->indexWork.indices, i);
                if (entry->status.flags & BTL_UNIT_STATUS_FLAG_ALLY) {
                    hasFlag200 = 1;
                }
                if (entry->status.flags & BTL_UNIT_STATUS_FLAG_ENEMY) {
                    hasFlag400 = 1;
                }
            }
            if (work->cameraPoseBlendHook(action, hasFlag200, hasFlag400) != 0) {
                action->flags |= 0x10000;
                return;
            }
        }
        btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
        action->motionParameter = 200.0f;
        action->flags |= 0x10041;
    } else {
        btlInitSkillCommandCursor(action, &action->camera);
    }
}

void btlAdvanceUnblockedPlayerCursorAnimation(BtlLinkedCommand *action) {
    if ((action->flags & 0x10000) != 0) {
        return;
    }
    btlAdvancePlayerCursorAnimation(action, &action->camera);
}
extern BtlActionAnimationRecord *datActionAnimationRecords;
extern s32 btlCanUseLinkedActor(BtlLinkedCommand *);
extern void btlPrepareUnitPoseWithTiltRotation(BtlLinkedCommand *,BtlCamState *,BtlCamState *);
extern void btlInitializeActionCamera(BtlLinkedCommand *);
extern void btlBuildGroupSizeActionCameraPair(BtlLinkedCommand *,BtlCamState *,BtlCamState *);
extern void btlSelectLinkageSkillCameraPreset();

void btlSelectActionCameraPose(BtlLinkedCommand *action) {
    s32 index = 0;
    s32 hasFlag200 = 0;
    s32 hasFlag400 = 0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    u32 count = btlGetIndexListCount(action->task->indexWork.indices);
    BtlUnit *unit;
    s32 flags;
    s32 policy;

    if (count != 0) {
        do {
            unit = btlGetIndexListEntry(action->task->indexWork.indices, index++);
            flags = unit->status.flags;
            if (flags & BTL_UNIT_STATUS_FLAG_ALLY) {
                hasFlag200 = 1;
            }
            if (flags & BTL_UNIT_STATUS_FLAG_ENEMY) {
                hasFlag400 = 1;
            }
        } while (index < count);
    }
    policy = datActionAnimationRecords[action->actionCode].flags & 0xC00;
    if (policy != 0) {
        if (policy == 0xC00) {
            index = dds3GetRandIntA(NULL, 2);
        } else {
            index = ((policy >> 10) ^ 1) & 1;
        }
        if ((btlCanUseLinkedActor(action) == 0 || index != 0) && battle->handleActorCategoryCamera == 0) {
            btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
            action->motionParameter = 200.0f;
            action->flags |= 0x10041;
            return;
        }
        if ((action->task->unit->status.flags & BTL_UNIT_STATUS_FLAG_ENEMY) && hasFlag200 == 0 && hasFlag400 == 1) {
            btlBuildGroupSizeActionCameraPair(action, &action->frontCamera, &action->backCamera);
            action->flags |= 0x10000;
            return;
        }
        btlInitializeActionCamera(action);
        action->status = 6;
        return;
    } else {
        if (battle->actionCameraSetupHook != 0 && battle->actionCameraSetupHook(action) != 0) {
            action->flags |= 0x10000;
        } else {
            btlSelectLinkageSkillCameraPreset(action, action);
        }
    }
}
