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
#include "btl_effect_node.h"
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

extern void func_001F3C30(BtlLinkedCommand *action);

extern u8 effSharedRandomState[];

extern void func_001ECBF8();

typedef struct BtlUnit BtlUnit;

extern s32 btlGetRuntime(void);

extern struct BtlRuntimeTask *btlDeferredTaskHead;

extern s32 datActionAnimationRecords;

extern f32 func_00208000(s32, f32 *, f32 *);

extern void btlClearRuntimeFlag2000(void);

extern void btlBuildApproachCamera(BtlLinkedCommand *, BtlCamState *);

extern void btlUpdateActionTargetCameraPose(BtlLinkedCommand *);

extern void func_001F3E48(BtlLinkedCommand *);

extern void btlAdvanceCursorForUnmarkedUnit(BtlLinkedCommand *, BtlCamState *);

extern void btlUnitGetMuzzlePosVU(BtlUnit *);

extern s32 btlGetSideIndexedActorStatusTable(s32, s32);

extern s32 btlHasSingleLinkedResource(BtlLinkedCommand *);

extern void btlResetCameraMotion(BtlLinkedCommand *);

extern s32 func_001FF5D8(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvanceCursorForUnmarkedUnit(BtlLinkedCommand *, BtlCamState *);

extern DatEnemyRecord *datEnemyRecords;

extern DatEnemyRecord *datEnemyRecords;

extern u32 btlSelectCommandTargetPolicy(ActionStateLink *, BtlIndexList *, s32);

extern s32 btlGetSideIndexedActorStatusTable(s32, s32);

extern u32 effMiscRandMod(void *state, u32 modulus);

extern void btlUnitGetBodyPosVU(BtlUnit *);

typedef struct {
    u8 unk00[0x110];
    u32 flags;
    u8 unk114[0x10];
    u16 objectId;
} BattleEntryHeader;

extern char D_00436A28[];

extern u32 D_00436A98;

extern s32 D_00436A9C;

extern s32 D_00436AA0;

/* Adjust the pose direction when projected camera height is above Y = -20. */ s32 btlAdjustCameraDirectionForDefaultPlane(BtlCamState *state);

void btlAimLinkedUnitAtMuzzle(BtlLinkedCommand *action);

/* Allocate a native header followed by capacity pointer entries. */ BtlIndexList *btlAllocateIndexList(s32 capacity);

void btlCopyMotionTransform(BtlCamState *dst, BtlCamState *src);

void btlFaceActionParticipantsTowardLinkedTarget(BtlLinkedCommand *action);

void btlFreeIndexList(BtlIndexList *list);

/* Return the number of live entries, not the allocated capacity. */ u32 btlGetIndexListCount(BtlIndexList *list);

/* The caller supplies an in-range index. */ void *btlGetIndexListEntry(BtlIndexList *list, s32 index);

s32 btlPositionActorIndexUnits(BtlLinkedCommand *action);

s32 btlMatchFirstLinkedActorFlags(BtlLinkedCommand *action) {
    ActionStateLink *link;

    switch (action->status) {
    case 5:
    case 6:
        break;
    default:
        return 0;
    }
    link = action->link;
    if (btlGetIndexListCount(link->indexWork.indices) >= 2) {
        return 0;
    }
    return btlGetIndexListEntry(link->indexWork.indices, 0) == link->unit;
}

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

s32 btlCanUseLinkedActor(BtlLinkedCommand *actor) {
    u32 status = actor->status;
    ActionStateLink *linked;
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
    linked = actor->link;
    if (linked == 0) {
        return 1;
    }
    if (btlHasMarkedEntry14((s32)actor)) {
        return 0;
    }
    if (linked->unit->partyRecord.status & 0x480) {
        return 0;
    }
    category = actor->actionCode;
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

extern s32 btlIsActorCategoryMarked(BtlLinkedCommand *);

s32 btlCanUseActorCategoryFlag2(BtlLinkedCommand *actor) {
    s32 category;

    if (btlIsActorCategoryMarked(actor)) {
        return 1;
    }
    if (!btlCanUseLinkedActor(actor)) {
        return 0;
    }
    category = actor->actionCode;
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

s32 btlCanUseActorCategoryFlag4(BtlLinkedCommand *actor) {
    s32 category = actor->actionCode;

    if (category == 0) {
        return 0;
    }
    if ((datCommandRecords[category].options & 1) == 0) {
        if (!btlCanUseLinkedActor(actor)) {
            return 0;
        }
    }
    return btlHasFlag(((BtlActionTableEntry *)datActionAnimationRecords)[actor->actionCode].flags, 4);
}

s32 btlIsActorCategoryMarked(BtlLinkedCommand *actor) {
    s32 category = actor->actionCode;

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

/* Skill 0x5F is Fire of Sinai; linked-camera selection treats it specially. */
u8 btlIsLinkedFireOfSinaiCommand(BtlLinkedCommand *action) {
    return action->actionCode == 0x5f;
}

s32 btlMapActorCategory(BtlLinkedCommand *actor) {
    switch (actor->actionCode) {
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

/* Skill 0x1A0 is Dropped Item in this game's skill-name table. */
u8 btlIsLinkedItemDropCommand(BtlLinkedCommand *action) {
    return action->actionCode == 0x1a0;
}

void func_001EAA18(void) {
}

void func_001EAA20(void) {
}

extern void btlPrepareActionCameraPoseWithActorClearance(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void func_001F20B0(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void btlFrameLeftmostUnitCameraPair(BtlCamState *, BtlCamState *, BtlCamState *);

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
                btlFrameLeftmostUnitCameraPair(&action->camera, &action->frontCamera, &action->backCamera);
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

/* Prepare the embedded primary camera through the existing 27.5f provider. */
void btlPreparePrimaryActionCamera(BtlLinkedCommand *action) {
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

extern u32 effMiscRand(void *state);
extern void btlStartLinkedActionPoseBlendIfEligible(BtlLinkedCommand *action);
extern void func_001EE458(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void func_001EDFB8(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void func_001EDC38(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void func_001EEB78(BtlLinkedCommand *, BtlCamState *, u8);
extern void func_001F34C8();
extern void func_001F3228();
extern void btlSetupCameraPoseAimUnit(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlPrepareHeightScaledActionCameraPose(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlBuildSideSelectedActorCameraBlend(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlChooseActionPoseBlendFromActorCount();
extern void btlBuildDistanceClampedActionCameraBlend(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void func_001EFEE8(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlPrepareRandomizedActionCameraPose(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

void btlChooseLinkedActionCameraPose(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();
    ActionStateLink *link;
    s32 hasFlag200;
    s32 hasFlag400;
    u32 i;
    u32 count;

    action->stepKind = 0;
    link = action->link;
    if (func_001EAA00(action) != 0) {
        btlStartLinkedActionPoseBlendIfEligible(action);
        return;
    }
    if (btlHasActorCategoryFlag40(action) != 0) {
        func_001ECBF8(action, action);
        action->flags |= 0x800;
    } else {
        hasFlag200 = 0;
        hasFlag400 = 0;
        count = btlGetIndexListCount(link->indexWork.indices);
        for (i = 0; i < count; i++) {
            BtlUnit *entry = (BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, i);
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
        if (link->unit->status.flags & 0x200) {
            if ((hasFlag200 == 1 || hasFlag400 != 1) && btlCanUseActorCategoryFlag4(action) == 0) {
                if (btlHasSingleLinkedResource(action) == 0) {
                    if (effMiscRand(effSharedRandomState) & 1) {
                        func_001EE458(action, &action->frontCamera, &action->backCamera);
                    } else {
                        func_001EDFB8(action, &action->frontCamera, &action->backCamera);
                    }
                    action->flags |= 0x800;
                } else if (btlMatchFirstLinkedActorFlags(action) != 0) {
                    func_001EDC38(action, &action->frontCamera, &action->backCamera);
                    action->motionParameter = 30.0f;
                    action->flags |= 0x841;
                } else {
                    func_001EEB78(action, &action->frontCamera, 3);
                    btlCopyMotionTransform(&action->backCamera, &action->frontCamera);
                    action->backCamera.distance += -100.0f;
                    action->motionParameter = 50.0f;
                    action->flags |= 0x841;
                    btlAdjustCameraDirectionForDefaultPlane(&action->backCamera);
                }
            } else if (btlHasLinkedEffectNodeTrigger(action) != 0) {
                switch (((BtlActionTableEntry *)datActionAnimationRecords)[action->actionCode].kind) {
                case 6:
                    action->stepKind = 7;
                    func_001F34C8(action, &action->camera);
                    break;
                case 7:
                    action->stepKind = 8;
                    func_001F3228(action, &action->camera);
                    break;
                default:
                    action->stepKind = 9;
                    btlFlagUserAndTargetDefeat(action, action);
                    break;
                }
                action->flags |= 0x800;
            } else {
                if (btlCanUseActorCategoryFlag2(action) != 0 || btlCanUseActorCategoryFlag4(action) != 0 ||
                    btlIsLinkedFireOfSinaiCommand(action) != 0 || btlMapActorCategory(action) != 0) {
                    action->stepKind = 1;
                } else if (btlIsEntryHeightWithinLimit() == 0 && btlHasSingleLinkedResource(action) == 0) {
                    action->stepKind = 1;
                } else {
                    action->stepKind = (effMiscRand(effSharedRandomState) & 1) ? 1 : 2;
                }
                switch (action->stepKind) {
                case 1:
                    switch (((BtlActionTableEntry *)datActionAnimationRecords)[action->actionCode].kind) {
                    case 6:
                    case 7:
                        btlSetupCameraPoseAimUnit(action, &action->frontCamera, &action->backCamera);
                        action->stepKind = 3;
                        break;
                    default:
                        btlPrepareHeightScaledActionCameraPose(action, &action->frontCamera, &action->backCamera);
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
                btlPositionActorIndexUnits(action);
                action->stepKind = 5;
                btlBuildDistanceClampedActionCameraBlend(action, &action->frontCamera, &action->backCamera);
            } else if (btlCanUseActorCategoryFlag4(action) != 0 && (hasFlag200 != 1 || hasFlag400 == hasFlag200)) {
                func_001EFEE8(action, &action->frontCamera, &action->backCamera);
                action->flags |= 0x800;
            } else if ((hasFlag200 != 1 || hasFlag400 == hasFlag200) &&
                       (btlCanUseActorCategoryFlag2(action) != 0 ||
                        ((effMiscRand(effSharedRandomState) & 1) && btlIsActorCategoryTypeTwo(action) == 0))) {
                func_001EE458(action, &action->frontCamera, &action->backCamera);
                action->flags |= 0x800;
            } else {
                if (btlHasActorCategoryFlag100(action) == 0 && btlCanUseActorCategoryFlag2(action) == 0 &&
                    btlCanUseActorCategoryFlag4(action) == 0 && (hasFlag200 == 1 || hasFlag400 != 1)) {
                    action->stepKind = (effMiscRand(effSharedRandomState) & 1) ? 4 : 5;
                } else {
                    action->stepKind = 4;
                }
                if (btlHasActorCategoryFlag100(action) != 0 &&
                    (link->unit->partyRecord.unitId != 0x128 || link->indexWork.slot != 0xE)) {
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

extern void btlAdvanceUnblockedPlayerCursorAnimation(BtlLinkedCommand *);

extern void btlRefreshActionPoseBlendSnapshot();

extern void btlAimEffectPoseAtUnit();

extern void btlBeginActionEndCameraPullback();

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
        btlBeginActionEndCameraPullback(action, &action->camera);
        break;
    case 7:
        func_001F34E0(action, &action->camera);
        break;
    case 8:
        btlBuildHeightClampedApproachCamera(action, &action->camera);
        break;
    }
}

extern void func_003364B8(f32);

extern void btlAdvanceActionSkillCamera(BtlLinkedCommand *);

extern void func_001EF030(void *, void *);

extern void func_001EC868(BtlLinkedCommand *, BtlCamState *, f32);

extern void btlAdvancePlayerCursorAnimation(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvanceTargetCursorAnimation(BtlLinkedCommand *, BtlCamState *);

extern void func_003364B8(f32);
extern void btlAdvanceActionSkillCamera(BtlLinkedCommand *);
extern void func_001EF030(void *, void *);
extern void func_001EC868(BtlLinkedCommand *, BtlCamState *, f32);

extern void btlSelectActionCameraPose(BtlLinkedCommand *);
extern void btlInitCommandCursorForFirstActor(BtlLinkedCommand *, BtlCamState *);
extern void func_001EEB78(BtlLinkedCommand *, BtlCamState *, u8);
extern void func_001EDC38(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void func_001EDFB8(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void func_001EFEE8(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

static inline s32 btlChooseLinkedCameraVariant(BtlLinkedCommand *action) {
    if (btlCanUseLinkedActor(action)) {
        if (btlCanUseActorCategoryFlag2(action)) {
            return 0;
        }
        return effMiscRandMod(NULL, 2);
    }
    return 1;
}

void btlInitializeActionCamera(BtlLinkedCommand *action) {
    BtlState *state;
    ActionStateLink *task;
    BtlUnit *unit;
    u32 count;
    s32 i;
    s32 hasFlag200;
    s32 hasFlag400;
    s32 choice;
    s32 reduceDistance;

    if (func_001EAA00(action)) {
        btlSelectActionCameraPose(action);
        return;
    }
    state = (BtlState *)btlGetRuntime();
    task = action->link;
    action->stepKind = 0;
    if (btlMapActorCategory(action) != 0) {
        action->stageCount = 0;
    }
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
                    action->stepKind = 13;
                } else if (btlCanUseActorCategoryFlag4(action)) {
                    func_001EFEE8(action, &action->frontCamera, &action->backCamera);
                } else {
                    /* Only the random linked variant reduces camera distance. */
                    choice = btlCanUseLinkedActor(action)
                        ? (btlCanUseActorCategoryFlag2(action)
                            ? (reduceDistance = 0, 0)
                            : (reduceDistance = 1, effMiscRandMod(NULL, 2)))
                        : (reduceDistance = 0, 1);
                    if (choice == 0) {
                        func_001EDC38(action, &action->frontCamera, &action->backCamera);
                        action->motionParameter = 30.0f;
                        action->flags |= 0x41;
                    } else {
                        f32 distance;

                        func_001EEB78(action, &action->frontCamera, 1);
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
                    func_001EFEE8(action, &action->frontCamera, &action->backCamera);
                } else if (btlMatchFirstLinkedActorFlags(action)) {
                    func_001EDC38(action, &action->frontCamera, &action->backCamera);
                    action->motionParameter = 30.0f;
                    action->flags |= 0x41;
                } else {
                    f32 distance;

                    func_001EEB78(action, &action->frontCamera, 3);
                    btlCopyMotionTransform(&action->backCamera, &action->frontCamera);
                    distance = action->backCamera.distance;
                    action->flags |= 0x41;
                    action->motionParameter = 50.0f;
                    action->backCamera.distance = distance + (-100.0f);
                    btlAdjustCameraDirectionForDefaultPlane(&action->backCamera);
                }
            } else {
                func_001EC868(action, &action->camera, 20.0f);
            }
        } else if (hasFlag200 != 1 && hasFlag400 == 1) {
            if (btlIsLinkedFireOfSinaiCommand(action)) {
                btlInitTargetCursorAndFacing(action, &action->camera);
            } else if (btlCanUseActorCategoryFlag4(action)) {
                func_001EFEE8(action, &action->frontCamera, &action->backCamera);
            } else {
                choice = btlChooseLinkedCameraVariant(action);
                if (choice != 0) {
                    f32 distance;

                    func_001EF030(action, &action->frontCamera);
                    btlCopyMotionTransform(&action->backCamera, &action->frontCamera);
                    distance = action->backCamera.distance;
                    action->flags |= 0x41;
                    action->motionParameter = 30.0f;
                    action->backCamera.distance = distance + 100.0f;
                    btlAdjustCameraDirectionForDefaultPlane(&action->backCamera);
                } else {
                    func_001EDFB8(action, &action->frontCamera, &action->backCamera);
                }
            }
        } else if (hasFlag200 == 1 && hasFlag400 != hasFlag200) {
            if (btlCanUseActorCategoryFlag4(action)) {
                func_001EFEE8(action, &action->frontCamera, &action->backCamera);
            } else {
                func_001EDFB8(action, &action->frontCamera, &action->backCamera);
            }
        } else {
            btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
            action->motionParameter = 200.0f;
            action->flags |= 0x41;
        }
    } else if (hasFlag200 == 1 && hasFlag400 != 1) {
        if (btlHasSingleLinkedResource(action)) {
            if (btlCanUseActorCategoryFlag4(action)) {
                func_001EFEE8(action, &action->frontCamera, &action->backCamera);
            } else if (btlCanUseLinkedActor(action) &&
                       ((effMiscRand(effSharedRandomState) & 1) ||
                        btlCanUseActorCategoryFlag2(action))) {
                func_001EDC38(action, &action->frontCamera, &action->backCamera);
                action->motionParameter = 30.0f;
                action->flags |= 0x41;
            } else {
                f32 distance;

                func_001EEB78(action, &action->frontCamera, 2);
                btlCopyMotionTransform(&action->backCamera, &action->frontCamera);
                distance = action->backCamera.distance;
                action->flags |= 0x41;
                action->motionParameter = 35.0f;
                action->backCamera.distance = distance + 150.0f;
                btlAdjustCameraDirectionForDefaultPlane(&action->backCamera);
            }
        } else if (btlCanUseActorCategoryFlag4(action)) {
            func_001EFEE8(action, &action->frontCamera, &action->backCamera);
        } else if (btlCanUseLinkedActor(action) &&
                   ((effMiscRand(effSharedRandomState) & 1) ||
                    btlCanUseActorCategoryFlag2(action))) {
            func_001EDFB8(action, &action->frontCamera, &action->backCamera);
        } else {
            btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
            btlAimLinkedUnitAtMuzzle(action);
            action->motionParameter = 200.0f;
            action->flags |= 0x41;
        }
    } else if (hasFlag200 != 1 && hasFlag400 == 1) {
        if (btlCanUseActorCategoryFlag4(action)) {
            func_001EFEE8(action, &action->frontCamera, &action->backCamera);
        } else {
            btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
            action->motionParameter = 200.0f;
            action->flags |= 0x41;
        }
    } else {
        func_001EC868(action, &action->camera, 20.0f);
    }
}

void btlAdvanceActorStageAndPose(BtlLinkedCommand *action) {
    BtlState *work;
    s32 category;
    u8 *out;
    if (func_001EAA00(action) != 0) {
        btlAdvanceActionSkillCamera(action);
        return;
    }
    work = (BtlState *)btlGetRuntime();
    if (work->unk66C != 0 && work->unk66C((BtlUnit *)action) != 0) {
        return;
    }
    category = btlMapActorCategory(action);
    if (category > 0 && category == action->stageCount++) {
        btlClearRuntimeFlag2000();
        if (work->unk658 != 0 && work->unk658((BtlUnit *)action) != 0) {
            return;
        }
        if (btlIsSpecialActorCategory((s32)action) != 0) {
            func_001EC868(action, &action->camera, 0.0f);
            func_003364B8(-(10.0f * 0.017453293f));
            VU0_STORE_VF_UNCLOBBERED(vf10, action->camera.direction);
            VU0_ROTATE_VEC(vf10, vf10);
            VU0_STORE_VF(vf10, action->camera.direction);
            action->camera.distance += 150.0f;
            out = (u8 *)&action->backCamera;
        } else {
            u8 *pose = (u8 *)&action->frontCamera;
            out = (u8 *)&action->backCamera;
            func_001EF030(action, pose);
            btlCopyMotionTransform((BtlCamState *)out, (BtlCamState *)pose);
            action->backCamera.distance += 100.0f;
            action->flags |= 0x41;
            action->state = 0;
            action->motionParameter = 30.0f;
        }
        btlAdjustCameraDirectionForDefaultPlane((BtlCamState *)out);
        return;
    }
    if (action->stepKind == 0xD) {
        btlAdvanceTargetCursorAnimation(action, &action->camera);
    }
}

extern void btlApplyMarkedUnitActionCamera(BtlLinkedCommand *, BtlCamState *, BtlUnit *, s32);

void btlUpdateActionPoseForLinkedTarget(BtlLinkedCommand *action) {
    BtlUnit *target;
    if (((BtlState *)btlGetRuntime())->unk220 & 1) {
        target = action->link->unit;
        if (target->status.flags & 0x400) {
            func_001ECBF8(action, &action->camera, target);
            return;
        }
    }
    if (action->actionKind == action->status || action->actionKind == 0xA || (action->flags & 0x40000)) {
        btlCopyMotionTransform(&action->frontCamera, &action->camera);
        btlApplyMarkedUnitActionCamera(action, &action->backCamera, action->link->unit, 0);
        action->motionParameter = 7.0f;
        action->flags = (action->flags | 0x1041) & 0xFFFBFFFF;
    } else {
        btlApplyMarkedUnitActionCamera(action, &action->camera, action->link->unit, 0);
    }
}

void func_001EBE28(void) {
}

INCLUDE_ASM(const s32, "game/code_001EA120", func_001EBE30);

void func_001EC190(void) {
}

extern void btlFrameTargetExtremesCameraPose(BtlLinkedCommand *, BtlCamState *, s32, f32, f32);

/* Prepare paired camera poses and blend flags for one target; party actors delegate. */
void btlPrepareSingleTargetCameraBlend(BtlLinkedCommand *action) {
    BtlUnit *target;
    s32 poseKind;
    f32 targetPosition[4];
    if (btlGetIndexListCount(action->targetList) != 1) {
        return;
    }
    target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
    if (action->link->unit->status.flags & 0x200) {
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
    btlFrameTargetExtremesCameraPose(action, &action->frontCamera, poseKind, 45.0f, 0.25f);
    btlFrameTargetExtremesCameraPose(action, &action->backCamera, poseKind, 1.0f, 0.5f);
    action->motionParameter = 30.0f;
    action->flags |= 0x41;
}

void func_001EC2A0(void) {
}

extern void btlInitSkillCommandCursor(BtlLinkedCommand *, BtlCamState *);

void btlStartLinkedActionPoseBlendIfEligible(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (action->link->unit->status.flags & 0x400) {
        if (work->cameraPoseBlendHook != 0) {
            s32 hasFlag200 = 0;
            s32 hasFlag400 = 0;
            u32 i;
            u32 count = btlGetIndexListCount(action->link->indexWork.indices);
            for (i = 0; i < count; i++) {
                BtlUnit *entry = (BtlUnit *)btlGetIndexListEntry(action->link->indexWork.indices, i);
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

void btlAdvanceUnblockedPlayerCursorAnimation(BtlLinkedCommand *unit) {
    if (!(unit->flags & 0x10000)) {
        btlAdvancePlayerCursorAnimation(unit, &unit->camera);
    }
}

extern s32 datActionAnimationRecords;
extern void func_001EE458(BtlLinkedCommand *, BtlCamState *, BtlCamState *);
extern void btlSelectLinkageSkillCameraPreset();

void btlSelectActionCameraPose(BtlLinkedCommand *action) {
    s32 index = 0;
    s32 hasFlag200 = 0;
    s32 hasFlag400 = 0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    u32 count = btlGetIndexListCount(action->link->indexWork.indices);
    BtlUnit *unit;
    s32 flags;
    s32 policy;

    if (count != 0) {
        do {
            unit = btlGetIndexListEntry(action->link->indexWork.indices, index++);
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
        if ((action->link->unit->status.flags & 0x400) && hasFlag200 == 0 && hasFlag400 == 1) {
            func_001EE458(action, &action->frontCamera, &action->backCamera);
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
