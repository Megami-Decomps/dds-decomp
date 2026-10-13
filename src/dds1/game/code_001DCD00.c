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

extern u32 btlSelectCommandTargetPolicy(void *, BtlIndexList *, s32);

extern u32 effMiscRandMod(void *state, u32 modulus);

extern s32 btlHasSingleLinkedResource(BtlLinkedCommand *);

extern void btlResetCameraMotion(BtlLinkedCommand *);

extern void btlUnitGetMuzzlePosVU(void *);

/* Kind 0x27: camera-mode task's complete 20-byte argument packet. */
typedef struct BtlCameraModeArgs {
    BtlTask *actor;
    BtlUnit *unitA;
    BtlUnit *unitB;
    u32 mode;
    s32 category;
} BtlCameraModeArgs;

typedef char BtlCameraModeArgsSize[(sizeof(BtlCameraModeArgs) == 20) ? 1 : -1];

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
        u32 (*commandSound)(BtlCameraModeArgs *);
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

extern SoundTask *btlDeferredTaskHead;

extern DatEnemyRecord *datEnemyRecords;

extern void btlSetDefaultActionCameraPose(BtlLinkedCommand *, BtlCamState *);

extern void btlPrepareUnitPoseWithTiltRotation(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern s32 btlGetRuntime(void);

extern s32 datActionAnimationRecords;

extern s8 effSharedRandomState[];

extern s8 effSharedRandomState[];

s32 btlGetSideIndexedActorStatusTable(s32 arg0, s32 arg1);

extern s32 btlGetRuntime(void);

extern void btlUnitGetBodyPosVU(u8 *);

extern char D_003BB5F8[];

extern u32 D_003BB660;

extern s32 D_003BB668;

extern s32 D_003BB664;

s32 btlAdjustCameraDirectionForDefaultPlane(BtlCamState *state);

void btlAimLinkedUnitAtMuzzle(u8 *action);

/* Allocate a native header followed by capacity pointer entries. */ BtlIndexList *btlAllocateIndexList(s32 capacity);

void btlCopyMotionTransform(BtlCamState *dst, BtlCamState *src);

void btlFaceActionParticipantsTowardLinkedTarget(BtlLinkedCommand *action);

void btlFreeIndexList(BtlIndexList *list);

/* Return the number of live entries, not the allocated capacity. */ u32 btlGetIndexListCount(BtlIndexList *list);

/* The caller supplies an in-range index. */ void *btlGetIndexListEntry(BtlIndexList *list, s32 index);

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

u32 btlCanUseActorCategoryFlag4(BtlLinkedCommand *actor) {
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

/* Skill 0x5F is Fire of Sinai; linked-camera selection treats it specially. */
u8 btlIsLinkedFireOfSinaiCommand(BtlLinkedCommand *action) {
    return action->actionCode == 0x5f;
}

/* Skill 0x1A0 is Item Drop in this game's skill-name table. */
u8 btlIsLinkedItemDropCommand(BtlLinkedCommand *action) {
    return action->actionCode == 0x1a0;
}

void func_001DD4A8(void) {
}

void func_001DD4B0(void) {
}

extern void btlPrepareActionCameraPoseWithActorClearance(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void func_001E4708(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

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
                btlFrameLeftmostUnitCameraPair(&action->camera, &action->frontCamera, &action->backCamera);
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

/* Prepare the embedded primary camera through the existing 27.5f provider. */
void btlPreparePrimaryActionCamera(BtlLinkedCommand *command) {
    btlSetDefaultActionCameraPose(command, &command->camera);
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

/* Let the battle hook intercept the cursor step, then dispatch approach/target/advance kinds. */
void btlDispatchActionCursorStepByKind(BtlLinkedCommand *actor) {
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

extern u32 effMiscRand(void *);
extern void func_001E0B68(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void func_001E0718(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void func_001E0398(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void func_001E1288(BtlLinkedCommand *, BtlCamState *, u8);
extern void func_001E5700(BtlLinkedCommand *, BtlCamState *);
extern void func_001E5460(BtlLinkedCommand *, BtlCamState *);
extern void btlSetupCameraPoseAimUnit(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlConfigureBattleCameraAction(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlBuildSideSelectedActorCameraBlend(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlChooseActionPoseBlendFromActorCount(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlBuildDistanceClampedActionCameraBlend(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void func_001E2578(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlPrepareRandomizedActionCameraPose(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

void btlChooseLinkedActionCameraPose(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlTask *task;
    s32 hasFlag200;
    s32 hasFlag400;
    u32 i;
    u32 count;

    action->stepKind = 0;
    task = action->task;
    if (btlHasActorCategoryFlag40(action) != 0) {
        btlSetDefaultActionCameraPose(action, &action->camera);
        action->flags |= 0x800;
    } else {
        hasFlag200 = 0;
        hasFlag400 = 0;
        count = btlGetIndexListCount(task->indexWork.indices);
        for (i = 0; i < count; i++) {
            BtlUnit *entry = (BtlUnit *)btlGetIndexListEntry(task->indexWork.indices, i);
            if (entry->status.flags & 0x200) {
                hasFlag200 = 1;
            }
            if (entry->status.flags & 0x400) {
                hasFlag400 = 1;
            }
        }
        if (work->cameraPoseBlendHook != 0 && work->cameraPoseBlendHook(action, hasFlag200, hasFlag400) != 0) {
            return;
        }
        if (task->unit->status.flags & 0x200) {
            if ((hasFlag200 == 1 || hasFlag400 != 1) && btlCanUseActorCategoryFlag4(action) == 0) {
                if (btlHasSingleLinkedResource(action) == 0) {
                    if (effMiscRand(effSharedRandomState) & 1) {
                        func_001E0B68(action, &action->frontCamera, &action->backCamera);
                    } else {
                        func_001E0718(action, &action->frontCamera, &action->backCamera);
                    }
                    action->flags |= 0x800;
                } else if (btlMatchFirstLinkedActorFlags(action) != 0) {
                    func_001E0398(action, &action->frontCamera, &action->backCamera);
                    action->motionParameter = 30.0f;
                    action->flags |= 0x841;
                } else {
                    func_001E1288(action, &action->frontCamera, 3);
                    btlCopyMotionTransform(&action->backCamera, &action->frontCamera);
                    action->backCamera.distance += -100.0f;
                    action->motionParameter = 50.0f;
                    action->flags |= 0x841;
                    btlAdjustCameraDirectionForDefaultPlane(&action->backCamera);
                }
            } else if (btlHasLinkedEffectNodeTrigger(action) != 0) {
                switch (((BtlActionAnimationRecord *)datActionAnimationRecords)[action->actionCode].cameraKind) {
                case 6:
                    action->stepKind = 7;
                    func_001E5700(action, &action->camera);
                    break;
                case 7:
                    action->stepKind = 8;
                    func_001E5460(action, &action->camera);
                    break;
                default:
                    action->stepKind = 9;
                    btlFlagUserAndTargetDefeat(action, action);
                    break;
                }
                action->flags |= 0x800;
            } else {
                if (btlCanUseActorCategoryFlag2(action) != 0 || btlCanUseActorCategoryFlag4(action) != 0 ||
                    btlIsLinkedFireOfSinaiCommand(action) != 0) {
                    action->stepKind = 1;
                } else if (btlIsEntryHeightWithinLimit() == 0 && btlHasSingleLinkedResource(action) == 0) {
                    action->stepKind = 1;
                } else {
                    action->stepKind = (effMiscRand(effSharedRandomState) & 1) ? 1 : 2;
                }
                switch (action->stepKind) {
                case 1:
                    switch (((BtlActionAnimationRecord *)datActionAnimationRecords)[action->actionCode].cameraKind) {
                    case 6:
                    case 7:
                        btlSetupCameraPoseAimUnit(action, &action->frontCamera, &action->backCamera);
                        action->stepKind = 3;
                        break;
                    default:
                        btlConfigureBattleCameraAction(action, &action->frontCamera, &action->backCamera);
                        break;
                    }
                    break;
                case 2:
                    btlBuildSideSelectedActorCameraBlend(action, &action->frontCamera, &action->backCamera);
                    break;
                }
            }
        } else {
            if (btlIsLinkedItemDropCommand(action) != 0) {
                btlChooseActionPoseBlendFromActorCount(action, &action->frontCamera, &action->backCamera);
                action->flags |= 0x800;
            } else if (btlHasSingleLinkedResource(action) != 0 && btlHasLinkedEffectNodeTrigger(action) != 0) {
                action->stepKind = 5;
                btlBuildDistanceClampedActionCameraBlend(action, &action->frontCamera, &action->backCamera);
            } else if (btlCanUseActorCategoryFlag4(action) != 0 && (hasFlag200 != 1 || hasFlag400 == hasFlag200)) {
                func_001E2578(action, &action->frontCamera, &action->backCamera);
                action->flags |= 0x800;
            } else if ((hasFlag200 != 1 || hasFlag400 == hasFlag200) &&
                       (btlCanUseActorCategoryFlag2(action) != 0 ||
                        ((effMiscRand(effSharedRandomState) & 1) && btlIsActorCategoryTypeTwo(action) == 0))) {
                func_001E0B68(action, &action->frontCamera, &action->backCamera);
                action->flags |= 0x800;
            } else {
                if (btlHasActorCategoryFlag100(action) == 0 && btlCanUseActorCategoryFlag2(action) == 0 &&
                    btlCanUseActorCategoryFlag4(action) == 0 && (hasFlag200 == 1 || hasFlag400 != 1)) {
                    action->stepKind = (effMiscRand(effSharedRandomState) & 1) ? 4 : 5;
                } else {
                    action->stepKind = 4;
                }
                if (btlHasActorCategoryFlag100(action) != 0) {
                    btlFaceActionParticipantsTowardLinkedTarget(action);
                }
                switch (action->stepKind) {
                case 4:
                    btlPrepareRandomizedActionCameraPose(action, &action->frontCamera, &action->backCamera);
                    break;
                case 5:
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

extern void func_001E5718(BtlLinkedCommand *, BtlCamState *);

extern void btlBuildHeightClampedApproachCamera(BtlLinkedCommand *, BtlCamState *);

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
        btlBeginActionEndCameraPullback(actor, actor);
        break;
    case 7:
        func_001E5718((BtlLinkedCommand *)actor, &((BtlLinkedCommand *)actor)->camera);
        break;
    case 8:
        btlBuildHeightClampedApproachCamera((BtlLinkedCommand *)actor, &((BtlLinkedCommand *)actor)->camera);
        break;
    }
}

extern void btlAdvancePlayerCursorAnimation(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvanceTargetCursorAnimation(BtlLinkedCommand *, BtlCamState *);

extern void btlBuildBattleUnitFramingCameraPose(BtlLinkedCommand *, BtlCamState *, f32);

extern void func_001E0718(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void func_001E0398(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void func_001E1288(BtlLinkedCommand *, BtlCamState *, u8);
extern void func_001E16C0(BtlLinkedCommand *, BtlCamState *);
extern void btlInitCommandCursorForFirstActor(BtlLinkedCommand *, BtlCamState *);
extern void btlInitTargetCursorAndFacing(BtlLinkedCommand *, BtlCamState *);
extern void func_001E2578(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern u32 effMiscRand(void *state);

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
        choice = effMiscRandMod(NULL, 2);
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
    action->stepKind = 0;
    if (btlHasActorCategoryFlag100(action)) {
        return;
    }
    hasFlag200 = 0;
    hasFlag400 = 0;
    count = btlGetIndexListCount(task->indexWork.indices);
    for (i = 0; i < count; i++) {
        unit = btlGetIndexListEntry(task->indexWork.indices, i);
        if (unit->status.flags & 0x200) {
            hasFlag200 = 1;
        }
        if (unit->status.flags & 0x400) {
            hasFlag400 = 1;
        }
    }
    if (state->handleActorCategoryCamera != NULL &&
        state->handleActorCategoryCamera(action, hasFlag200, hasFlag400)) {
        return;
    }

    if (task->unit->status.flags & 0x200) {
        if (btlHasSingleLinkedResource(action)) {
            if (hasFlag200 != 1 && hasFlag400 == 1) {
                if (btlIsActorCategoryMarked(action)) {
                    btlInitCommandCursorForFirstActor(action, &action->camera);
                    action->stepKind = 12;
                } else if (btlCanUseActorCategoryFlag4(action)) {
                    func_001E2578(action, &action->frontCamera, &action->backCamera);
                } else {
                    choice = btlChooseLinkedCameraVariant(action, &reduceDistance);
                    if (choice == 0) {
                        func_001E0398(action, &action->frontCamera, &action->backCamera);
                        action->motionParameter = 30.0f;
                        action->flags |= 0x41;
                    } else {
                        f32 distance;

                        func_001E1288(action, &action->frontCamera, 1);
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
                    func_001E2578(action, &action->frontCamera, &action->backCamera);
                } else if (btlMatchFirstLinkedActorFlags(action)) {
                    func_001E0398(action, &action->frontCamera, &action->backCamera);
                    action->motionParameter = 30.0f;
                    action->flags |= 0x41;
                } else {
                    f32 distance;

                    func_001E1288(action, &action->frontCamera, 3);
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
                func_001E2578(action, &action->frontCamera, &action->backCamera);
            } else {
                choice = btlChooseLinkedCameraVariant(action, NULL);
                if (choice != 0) {
                    f32 distance;

                    func_001E16C0(action, &action->frontCamera);
                    btlCopyMotionTransform(&action->backCamera, &action->frontCamera);
                    distance = action->backCamera.distance;
                    action->flags |= 0x41;
                    action->motionParameter = 30.0f;
                    action->backCamera.distance = distance + 100.0f;
                    btlAdjustCameraDirectionForDefaultPlane(&action->backCamera);
                } else {
                    func_001E0718(action, &action->frontCamera, &action->backCamera);
                }
            }
        } else if (hasFlag200 == 1 && hasFlag400 != hasFlag200) {
            if (btlCanUseActorCategoryFlag4(action)) {
                func_001E2578(action, &action->frontCamera, &action->backCamera);
            } else {
                func_001E0718(action, &action->frontCamera, &action->backCamera);
            }
        } else {
            btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
            action->motionParameter = 200.0f;
            action->flags |= 0x41;
        }
    } else if (hasFlag200 == 1 && hasFlag400 != 1) {
        if (btlHasSingleLinkedResource(action)) {
            if (btlCanUseActorCategoryFlag4(action)) {
                func_001E2578(action, &action->frontCamera, &action->backCamera);
            } else if (btlCanUseLinkedActor(action) &&
                       ((effMiscRand(effSharedRandomState) & 1) ||
                        btlCanUseActorCategoryFlag2(action))) {
                func_001E0398(action, &action->frontCamera, &action->backCamera);
                action->motionParameter = 30.0f;
                action->flags |= 0x41;
            } else {
                f32 distance;

                func_001E1288(action, &action->frontCamera, 2);
                btlCopyMotionTransform(&action->backCamera, &action->frontCamera);
                distance = action->backCamera.distance;
                action->flags |= 0x41;
                action->motionParameter = 35.0f;
                action->backCamera.distance = distance + 150.0f;
                btlAdjustCameraDirectionForDefaultPlane(&action->backCamera);
            }
        } else if (btlCanUseActorCategoryFlag4(action)) {
            func_001E2578(action, &action->frontCamera, &action->backCamera);
        } else if (btlCanUseLinkedActor(action) &&
                   ((effMiscRand(effSharedRandomState) & 1) ||
                    btlCanUseActorCategoryFlag2(action))) {
            func_001E0718(action, &action->frontCamera, &action->backCamera);
        } else {
            btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
            btlAimLinkedUnitAtMuzzle((u8 *)action);
            action->motionParameter = 200.0f;
            action->flags |= 0x41;
        }
    } else if (hasFlag200 != 1 && hasFlag400 == 1) {
        if (btlCanUseActorCategoryFlag4(action)) {
            func_001E2578(action, &action->frontCamera, &action->backCamera);
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
    s32 (*callback)(BtlLinkedCommand *) = *(s32 (**)(BtlLinkedCommand *))(btlGetRuntime() + 0x634);
    if (callback != 0 && callback(actor) != 0) {
        return;
    }
    if (actor->stepKind == 12) {
        btlAdvanceTargetCursorAnimation(actor, &actor->camera);
    }
}

extern void btlApplyMarkedUnitActionCamera(BtlLinkedCommand *, BtlCamState *, BtlUnit *, s32);

void btlUpdateActionPoseForLinkedTarget(BtlLinkedCommand *action) {
    BtlUnit *target;

    if (((BtlState *)btlGetRuntime())->unk_1FC & 1) {
        target = action->task->unit;
        if (target->status.flags & 0x400) {
            btlSetDefaultActionCameraPose(action, &action->camera);
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

void func_001DE5F0(void) {
}

INCLUDE_ASM(const s32, "game/code_001DCD00", func_001DE5F8);

void func_001DE958(void) {
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
    if (action->task->unit->status.flags & 0x200) {
        btlApplyMarkedUnitActionCamera(action, &action->camera, target, 0);
        return;
    }
    if (btlHasActorCategoryFlag100(action) != 0) {
        return;
    }
    btlUnitGetBodyPosVU(target);
    VU0_STORE_VF(vf10, targetPosition);
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

void func_001DEA68(void) {
}

extern void btlInitSkillCommandCursor(BtlLinkedCommand *, BtlCamState *);

void btlStartLinkedActionPoseBlendIfEligible(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (action->task->unit->status.flags & 0x400) {
        if (work->cameraPoseBlendHook != 0) {
            s32 hasFlag200 = 0;
            s32 hasFlag400 = 0;
            u32 i;
            u32 count = btlGetIndexListCount(action->task->indexWork.indices);
            for (i = 0; i < count; i++) {
                BtlUnit *entry = (BtlUnit *)btlGetIndexListEntry(action->task->indexWork.indices, i);
                if (entry->status.flags & 0x200) {
                    hasFlag200 = 1;
                }
                if (entry->status.flags & 0x400) {
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

extern s32 btlGetRuntime(void);
extern s32 datActionAnimationRecords;
extern s32 btlCanUseLinkedActor(BtlLinkedCommand *);
extern u32 effMiscRandMod(void *,u32);
extern void btlPrepareUnitPoseWithTiltRotation(BtlLinkedCommand *,BtlCamState *,BtlCamState *);
extern void btlInitializeActionCamera(BtlLinkedCommand *);
extern void func_001E0B68(BtlLinkedCommand *,BtlCamState *,BtlCamState *);
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
            if (flags & 0x200) {
                hasFlag200 = 1;
            }
            if (flags & 0x400) {
                hasFlag400 = 1;
            }
        } while (index < count);
    }
    policy = ((BtlActionAnimationRecord *)datActionAnimationRecords)[action->actionCode].flags & 0xC00;
    if (policy != 0) {
        if (policy == 0xC00) {
            index = effMiscRandMod(NULL, 2);
        } else {
            index = ((policy >> 10) ^ 1) & 1;
        }
        if ((btlCanUseLinkedActor(action) == 0 || index != 0) && battle->handleActorCategoryCamera == 0) {
            btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
            action->motionParameter = 200.0f;
            action->flags |= 0x10041;
            return;
        }
        if ((action->task->unit->status.flags & 0x400) && hasFlag200 == 0 && hasFlag400 == 1) {
            func_001E0B68(action, &action->frontCamera, &action->backCamera);
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
