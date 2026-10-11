#include "btl_motion_transform.h"
#include "common.h"
#include "dds3obj.h"
#include "btl_effect_position.h"
#include "btl_task_condition.h"
#include "btl_state.h"
#include "btl_command.h"
#include "btl_action.h"
#include "btl_unit_tasks.h"
#include "pcp_vu0.h"
#include "evt_unit.h"
#include "mdl.h"
#include "eff_transform.h"
#include "dat_state.h"
#include "sdf_resource.h"

extern void btlInterpolateVectorStep();

struct EffRandState;
extern struct EffRandState effSharedRandomState;
extern u32 effMiscRand(struct EffRandState *);
extern void btlBuildLinkedCommandCameraPair(BtlLinkedCommand *, BtlCamState *, BtlCamState *, f32, f32, f32, s8, s8);
extern void btlClearAllUnitDefeatCandidatesTask(void);
extern void btlFlagLinkedGroupDefeatCandidatesTask(BtlLinkedCommand *);
extern void btlUpdateActionPoseForLinkedTarget(BtlLinkedCommand *);



extern s32 mdlFlagTest(s32);


extern s32 btlGetRuntime(void);

extern void btlBossDebugPrintf(const char *format, ...);

extern u32 btlGetEffectActive(void);

extern u32 btlGetEffectValue(void);

extern BtlUnit *btlGetEffectActor(void);

/* The model flag word is the first member of the extension's +0x8C info object.
 * Retail accesses unit+0x340 -> extension+0x8C -> flags+0; no second unit view. */
typedef struct BtlUnitInfo {
    union {
        u8 b0;
        u32 flags;
    };
    u8 pad1[0x14];
    s32 unk18;
    struct BtlUnitData *data;
} BtlUnitInfo;


extern BtlRuntimeTask *btlCreateUnitFadeOutTask(BtlUnit *, u32, u32);

extern s32 btlHasEffectActor(void);

extern s32 btlHasEffectActor(void);

extern s32 btlHasEffectActor(void);

extern u64 btlStartTask(void *);

extern BtlRuntimeTask *btlCreateCommandSoundUpdateTask(void);

extern s32 btlCreateSecondaryCommandSoundTask();

extern BtlRuntimeTask *btlCreateCommandSoundTask(s32, s32);

extern BtlRuntimeTask *btlCreateEffObjB(BtlUnit *, s32);

extern BtlRuntimeTask *fldCreateSceneGroupAction(ActionStateLink *, u32, s32);

extern char D_00436CF8[];

extern char D_0041B628[];

extern char D_0041B640[];
extern s32 btlHasMarkedEntry14(s32 actor);
extern void btlClearRuntimeFlag2000(void);
extern u32 effMiscRandMod(void *state, u32 modulus);
extern void btlSetEffectCameraKeys(s32 command, f32, f32, f32, f32, f32, f32, f32, f32,
    f32, f32, f32, f32, f32, f32, f32, f32);


extern u64 btlAdvanceRuntimeSequenceCounter(void);
extern u32 btlCreateScriptResourceTask(BtlUnit *unit, u32 group);
extern BtlRuntimeTask *sndCreateStationedSeTask(u32 value);
extern BtlRuntimeTask *sndCreateCustomTask(s32 value, s32 option);
extern BtlRuntimeTask *btlScheduleRefreshTask(BtlUnit *unit);
extern BtlRuntimeTask *btlCreateSoundUpdateTask(u32 value);
extern BtlRuntimeTask *btlCreateFadeInTask(u32 value);
extern ActionStateLink *fldGetSceneGroupEntry(s32 entryIndex);

void func_00227288(void) {
    btlUpdateLinkedEffectUnitTransforms();
}

extern s32 btlIsCurrentValueBelowQuarterThreshold(BtlUnit *unit);
extern BtlRuntimeTask *btlCreateStiffenDamageShakeTask(BtlUnit *unit, f32 amount);

s32 func_002272A0(BtlUnit *unit, s32 code, s32 unused) {
    BtlState *battle;
    BattleLinkedEffectState *effect;
    BtlUnit *actor;
    BtlUnit *first;
    BtlUnit *second;
    BtlRuntimeTask *task;
    s32 secondLow;
    s32 firstLow;

    if ((unit->status.flags & 0x400) == 0) {
        return code;
    }
    if ((unit->status.flags & 1) == 0) {
        return -1;
    }

    battle = (BtlState *)btlGetRuntime();
    effect = &battle->effect->linked;
    first = NULL;
    second = NULL;
    for (actor = battle->units; actor != NULL; actor = actor->nextActor) {
        u32 flags = actor->status.flags;
        if (flags & 1) {
            if (flags & 0x400) {
                switch (actor->partyRecord.unitId) {
                case 0x12E:
                    first = actor;
                    break;
                case 0x12F:
                    second = actor;
                    break;
                }
            }
        }
    }

    if (effect->active != 0) {
        if (effect->active == 1) {
            if (second == NULL || first == NULL) {
                return code;
            }
            if (code == 1) {
                if (unit == second && first->unkEC == 0x11) {
                    return code;
                }
                if (unit == first && second->unkEC == 0x10) {
                    return code;
                }
                if (unit->partyRecord.unitId == 0x12E) {
                    return 0x11;
                }
                if (unit->partyRecord.unitId == 0x12F) {
                    return 0x10;
                }
            }
            switch (code) {
            case 0:
            case 2:
            case 10:
                secondLow = btlIsCurrentValueBelowQuarterThreshold(second);
                firstLow = btlIsCurrentValueBelowQuarterThreshold(first);
                if (secondLow != 0) {
                    return firstLow != 0 ? 10 : 0x12;
                }
                return firstLow != 0 ? 0x13 : 0;
            }
            if (code == 11) {
                return unit == second ? 0x10 : 0x11;
            }
        }
    } else {
        if (second == unit) {
            if (code == 1 && btlHasEffectActor() != 0) {
                task = btlCreateStiffenDamageShakeTask(unit, 8.0f);
                task->startDelay = code;
                btlStartTask(task);
                return -1;
            }
            if (code == 0x10) {
                if (first == NULL) {
                    return 0x11;
                }
                if ((first->status.flags & 0xE0) != 0) {
                    return 0x11;
                }
            }
        }
    }
    return code;
}

s32 btlIsEffectPhaseInRange(s32 unused, s32 value) {
    BattleEffectPayload *effect = ((BtlState *)btlGetRuntime())->effect;
    if (effect->linked.active != 1) {
        return 0;
    }
    switch (value) {
    case 0x12:
    case 0x13:
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_00227528);

s32 btlRemapEffectActiveCombatantAction(BtlUnit *unit, s32 code) {
    u32 id;
    if (!(unit->status.flags & 1)) {
        return code;
    }
    if (!(unit->status.flags & 0x400)) {
        return code;
    }
    if (((BtlState *)btlGetRuntime())->effect->linked.active != 1) {
        return code;
    }
    id = unit->partyRecord.unitId;
    if (id == 0x119) {
        return code;
    }
    switch (code) {
    case 1:
        if (id == 0x12E) {
            return 0xC8;
        }
        if (id == 0x12F) {
            return 1;
        }
        break;
    case 2:
        if (id == 0x12E) {
            return 0xC8;
        }
        if (id == 0x12F) {
            return 0x66;
        }
        break;
    default:
        if (id == 0x12E) {
            return code + 0xC8;
        }
        if (id == 0x12F) {
            return code + 0x64;
        }
        break;
    }
    return code;
}

BtlUnit *btlFindFlaggedSpecialSpeciesUnit(s32 category, s32 species) {
    BtlUnit *unit;
    if (category != 1) {
        return 0;
    }
    if (species != 0x119) {
        return 0;
    }
    unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        u32 flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (flags & 2) {
                    if (unit->resourceIndex == 0x119) {
                        return unit;
                    }
                }
            }
        }
        unit = unit->nextActor;
    }
    return 0;
}

/* Collapse three special unit modes to one display code. */
s32 btlGetCanonicalCombatantKind(BtlUnit *unit) {
    switch (unit->partyRecord.unitId) {
    case 0x119:
    case 0x12E:
    case 0x12F:
        return 0x119;
    }
    return unit->combatantKind;
}


/* Lower the linked actor, or return eligible actors to ground level. */
void btlUpdateLinkedActorGroundHeight(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BattleEffectPayload *effect;
    f32 vector[4] __attribute__((aligned(16)));
    f32 height;
    f32 speed;
    f32 limit;

    if ((battle->battleFlags & 0x80000) == 0) {
        return;
    }
    effect = battle->effect;
    if (effect->linked.actor != 0) {
        BtlUnit *selected = effect->linked.actor;
        BtlUnit *actor;
        if ((selected->status.flags & 2) == 0) {
            return;
        }
        effObjFetchInnerPosition(selected->effectObject);
        VU0_STORE_VF(vf10, (u128 *)vector);
        if (!(vector[1] > -125.0f)) {
            return;
        }
        actor = effect->linked.actor;
        limit = actor->resourceIndex == 0x16 ? -62.5f : -125.0f;
        height = effect->linked.height - effect->linked.speed;
        speed = effect->linked.speed / 1.11f;
        effect->linked.height = height;
        effect->linked.speed = speed;
        if (height < limit) {
            effect->linked.height = limit;
        }
        vector[1] = effect->linked.height;
        effObjSetInnerPosition(actor->effectObject, (u128 *)vector);
        return;
    }

    {
        BtlUnit *unit = battle->units;
        if (unit == NULL) {
            return;
        }
        {
            f32 ceiling = 10000.0f;
            f32 lowerLimit = -1.0f;
            f32 decay = 1.05f;
            f32 zero = 0.0f;

            while (unit != NULL) {
                u32 flags = unit->status.flags;

                if (flags & 1) {
                    if (flags & 0x200) {
                        if (flags & 2) {
                            effObjFetchInnerPosition(unit->effectObject);
                            VU0_STORE_VF(vf10, (u128 *)vector);
                            if (!(vector[1] > ceiling)) {
                                if (vector[1] < lowerLimit) {
                                    height = effect->linked.height + effect->linked.speed;
                                    speed = effect->linked.speed * decay;
                                    effect->linked.height = height;
                                    effect->linked.speed = speed;
                                    if (height > zero) {
                                        effect->linked.height = zero;
                                    }
                                    vector[1] = effect->linked.height;
                                    effObjSetInnerPosition(unit->effectObject, (u128 *)vector);
                                } else {
                                    vector[1] = zero;
                                    effObjSetInnerPosition(unit->effectObject, (u128 *)vector);
                                }
                            }
                        }
                    }
                }
                unit = unit->nextActor;
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00227288", func_002279F0);

s32 btlGetEffectTaskActorMatchCode(ActionStateLink *task) {
    BattleEffectPayload *effect;
    if ((task->pendingFlags & 8) == 0) {
        return -1;
    }
    effect = ((BtlState *)btlGetRuntime())->effect;
    return effect->linked.actor == task->unit ? 12 : -1;
}

s32 btlEffectTaskStartFinale(ActionStateLink *task) {
    BattleEffectPayload *effect;
    BtlRuntimeTask *group;

    if ((task->pendingFlags & 8) == 0) {
        return -1;
    }
    effect = ((BtlState *)btlGetRuntime())->effect;
    if (effect->linked.actor != task->unit) {
        return -1;
    }
    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    btlStartTask(btlCreateCommandSoundTask(task, 9));
    btlStartTask(btlCreateEffObjB(task->unit, 0xB4));
    group = fldCreateSceneGroupAction(task, 0x64, 1);
    group->startDelay = 0x16;
    btlStartTask(group);
    effect->linked.phase = 1;
    return (task->unit->partyRecord.status & 0x480) ? 0x19 : 0x1B;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_00227DA8);

s32 btlIsEffectActor(BtlUnit *actor) {
    BattleEffectPayload *state = ((BtlState *)btlGetRuntime())->effect;
    BtlUnit *active = state->linked.actor;
    if (active != 0) {
        return active == actor;
    }
    return 0;
}

s32 btlGetSoleTargetKind(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *target;
    BtlUnit *unit;
    BtlUnit *last;
    s32 count;
    if (btlHasEffectActor() == 0) {
        return -1;
    }
    target = btlGetEffectActor();
    last = NULL;
    count = 0;
    for (unit = state->units; unit != NULL; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x200) {
                if (!(unit->status.flags & 0xE0)) {
                    if (!(unit->partyRecord.status & 0x800)) {
                        count++;
                        last = unit;
                    }
                }
            }
        }
    }
    if (count == 1 && (last == NULL || last == target)) {
        return 7;
    }
    return -1;
}

s32 btlHasDifferentActiveTarget(BtlUnit *target) {
    if (btlHasEffectActor() == 0) {
        return 1;
    }
    return btlGetEffectActor() != target;
}

s32 btlSetLinkFlagOff(BtlUnit *requestedUnit) {
    BtlUnit *unit;
    BtlUnit *other;
    if (btlHasEffectActor() == 0) {
        return 1;
    }
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != NULL; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x400) {
                if (unit->partyRecord.unitId == 0x12F) {
                    break;
                }
            }
        }
    }
    if (unit != NULL) {
        other = btlGetEffectActor();
        if (!(other->status.flags & 2)) {
            return 1;
        }
        if (unit == requestedUnit) {
            other->ext->owner->flags &= ~MDL_SKIP_TRANSFORMS;
            return 1;
        }
        if (other != requestedUnit) {
            return 1;
        }
        if (unit->status.flags & 4) {
            other->ext->owner->flags &= ~MDL_SKIP_TRANSFORMS;
        } else {
            other->ext->owner->flags |= MDL_SKIP_TRANSFORMS;
        }
        return 0;
    }
    return 1;
}

s32 btlSetLinkFlagOn(BtlUnit *requestedUnit) {
    BtlUnit *unit;
    BtlUnit *other;
    if (btlHasEffectActor() == 0) {
        return 1;
    }
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != NULL; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x400) {
                if (unit->partyRecord.unitId == 0x12F) {
                    break;
                }
            }
        }
    }
    if (unit != NULL) {
        other = btlGetEffectActor();
        if (!(other->status.flags & 2)) {
            return 1;
        }
        if (unit == requestedUnit) {
            other->ext->owner->flags |= MDL_SKIP_TRANSFORMS;
            return 1;
        }
        if (other != requestedUnit) {
            return 1;
        }
        if (unit->status.flags & 4) {
            other->ext->owner->flags &= ~MDL_SKIP_TRANSFORMS;
        } else {
            other->ext->owner->flags |= MDL_SKIP_TRANSFORMS;
        }
        return 0;
    }
    return 1;
}

void btlQueueLinkedActorModelStateTasks(ActionStateLink *action) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BattleEffectPayload *effect = battle->effect;
    BtlUnit *unit;
    BtlRuntimeTask *task;
    u64 soundSequence;
    u64 refreshSequence;
    u64 modelSequence;
    u64 scriptSequence;

    effect->linked.active = 0;
    soundSequence = btlAdvanceRuntimeSequenceCounter();
    refreshSequence = btlAdvanceRuntimeSequenceCounter();
    modelSequence = btlAdvanceRuntimeSequenceCounter();
    scriptSequence = btlAdvanceRuntimeSequenceCounter();

    for (unit = battle->units; unit != 0; unit = unit->nextActor) {
        s32 flags = unit->status.flags;
        s32 unitId;

        if ((flags & 1) == 0) {
            continue;
        }
        if ((flags & 0x400) == 0) {
            continue;
        }
        unitId = unit->partyRecord.unitId;
        if (unitId >= 0x130) {
            continue;
        }
        if (unitId < 0x12E) {
            continue;
        }
        if (unit->resourceIndex != 0x119) {
            continue;
        }

        if ((flags & 0xE0) == 0) {
            if (unitId != 0x12F) {
                u32 scriptTask = btlCreateScriptResourceTask(unit, 0x50);
                ((BtlRuntimeTask *)scriptTask)->ownerId = scriptSequence;
                btlStartTask((void *)scriptTask);
                task = sndCreateStationedSeTask(battle->sequenceHandle);
                task->ownerId = soundSequence;
                btlStartTask(task);
            } else {
                u32 scriptTask = btlCreateScriptResourceTask(unit, 0x51);
                ((BtlRuntimeTask *)scriptTask)->ownerId = scriptSequence;
                btlStartTask((void *)scriptTask);
                task = sndCreateStationedSeTask(battle->sequenceHandle + 1);
                task->ownerId = soundSequence;
                btlStartTask(task);
            }

            task = sndCreateCustomTask((s32)0x80FFFFFF, 0xC);
            task->startCondition.kind = BTL_TASK_CONDITION_OWNER_RUNNING_OR_ABSENT;
            task->startCondition.value.handle = scriptSequence;
            if (unit->partyRecord.unitId == 0x12F) {
                task->startDelay = 0x6C;
            } else {
                task->startDelay = 0x4E;
            }
            task->endDelay = 0xC;
            task->ownerId = soundSequence;
            btlStartTask(task);
        } else {
            if (unitId == 0x12E) {
                effect->linked.timer |= 4;
            } else {
                effect->linked.timer |= 2;
            }
        }

        {
            BtlRuntimeTask *refreshTask = btlScheduleRefreshTask(unit);
            refreshTask->startCondition.kind = BTL_TASK_CONDITION_OWNER_ABSENT;
            refreshTask->startCondition.value.handle = scriptSequence;
            refreshTask->ownerId = refreshSequence;
            btlStartTask(refreshTask);
        }

        if ((unit->status.flags & 0xE0) == 0) {
            BtlRuntimeTask *modelTask = btlCreateModelLoadPollTask(unit, unit->resourceKind,
                unit->partyRecord.unitId, 1);
            modelTask->startCondition.kind = BTL_TASK_CONDITION_OWNER_ABSENT;
            modelTask->startCondition.value.handle = refreshSequence;
            modelTask->ownerId = modelSequence;
            btlStartTask(modelTask);
        }
    }

    {
        ActionStateLink *sceneAction;

        task = btlCreateSoundUpdateTask(0);
        task->startCondition.kind = BTL_TASK_CONDITION_OWNER_ABSENT;
        task->startCondition.value.handle = modelSequence;
        task->startDelay = 2;
        task->ownerId = action->unit->owner;
        btlStartTask(task);

        task = btlCreateCommandSoundUpdateTask();
        task->startCondition.kind = BTL_TASK_CONDITION_OWNER_ABSENT;
        task->startCondition.value.handle = modelSequence;
        task->ownerId = action->unit->owner;
        btlStartTask(task);

        sceneAction = fldGetSceneGroupEntry(0);
        if (sceneAction != 0 && (sceneAction->pendingFlags & 8) != 0 &&
            (sceneAction->unit->status.flags & 0x200) != 0) {
            task = btlCreateCommandSoundTask((s32)sceneAction, 9);
            task->startCondition.kind = BTL_TASK_CONDITION_OWNER_ABSENT;
            task->startCondition.value.handle = modelSequence;
            task->ownerId = action->unit->owner;
            btlStartTask(task);
        } else {
            task = btlCreateCommandSoundTask(0, 3);
            task->startCondition.kind = BTL_TASK_CONDITION_OWNER_ABSENT;
            task->startCondition.value.handle = modelSequence;
            task->ownerId = action->unit->owner;
            btlStartTask(task);
        }

        task = btlCreateFadeInTask(0x10);
        task->startCondition.kind = BTL_TASK_CONDITION_OWNER_ABSENT;
        task->startCondition.value.handle = modelSequence;
        task->endDelay = 0x1F;
        task->ownerId = action->unit->owner;
        btlStartTask(task);
    }
}



s32 btlTryScheduleMarkedUnitTask(BtlUnit *unit) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BattleEffectPayload *effect;
    BtlUnit *other;
    if ((unit->status.flags & 0x400) == 0) {
        return 1;
    }
    effect = battle->effect;
    if (effect->linked.active != 0) {
        return 1;
    }
    if (unit->resourceIndex == 0x119) {
        return 1;
    }
    other = battle->units;
    while (other != 0) {
        u32 flags = other->status.flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (other != unit) {
                    if (flags & 0xe0) {
                        return 1;
                    }
                }
            }
        }
        other = other->nextActor;
    }
    btlStartTask(btlCreateUnitFadeOutTask(unit, 8, 10));
    unit->status.flags &= ~0x100;
    return 1;
}

s32 func_00228B08(BtlLinkedCommand *command) {
    ActionStateLink *scene;

    switch (command->actionCode) {
    case 0x188:
        command->motionProgress = 0;
        goto unhandled;
    case 0x189:
        btlBuildLinkedCommandCameraPair(command, &command->frontCamera, &command->backCamera, 1.25f, 2.0f, 0.0f, 1, 0);
        command->motionParameter = 25.0f;
        command->motionProgress = 0;
        command->flags |= 0x841;
        command->frontCamera.distance -= 300.0f;
        command->backCamera.distance += 150.0f;
        btlAdjustCameraDirectionForDefaultPlane(&command->backCamera);
        goto handled;
    case 0x171:
        scene = fldGetSceneGroupEntry(0);
        if (scene != NULL && (scene->pendingFlags & 8) && (scene->unit->status.flags & 0x200)) {
            command->link = scene;
            command->status = 9;
            btlUpdateActionPoseForLinkedTarget(command);
        } else {
            btlClearAllUnitDefeatCandidatesTask();
            btlFlagLinkedGroupDefeatCandidatesTask(command);
            if (effMiscRand(&effSharedRandomState) & 1) {
                btlSetEffectCameraKeys((s32)command, 237.1f, -287.8f, -460.5f, 0.021f, -0.127f, -0.022f, -0.982f, 841.4f,
                                       -406.5f, -442.7f, 0.108f, 0.293f, 0.013f, 0.94f, 40.0f, 30.0f);
            } else {
                btlSetEffectCameraKeys((s32)command, 661.7f, -200.6f, -349.7f, -0.011f, 0.269f, -0.02f, 0.953f, 254.2f,
                                       -445.6f, -431.6f, 0.152f, -0.129f, -0.036f, 0.97f, 40.0f, 30.0f);
            }
        }
        command->flags |= 0x800;
        goto handled;
    default:
        goto unhandled;
    }
handled:
    return 1;
unhandled:
    return 0;
}

/* Initialize the linked command's target aim once in state 0x1E. Action 0x171
 * succeeds without setup; unsupported actions return 0, deferred setup returns 1. */
s32 btlUnitStartAimAtTarget(BtlLinkedCommand *command) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *target;
    u32 flags;
    f32 distance;

    if (command->actionCode == 0x171) {
        return 1;
    }
    if (command->actionCode != 0x189) {
        return 0;
    }
    for (target = battle->units; target != 0; target = target->nextActor) {
        flags = target->status.flags;
        if ((flags & 1) != 0) {
            if ((flags & 0x400) != 0) {
                if ((flags & 2) != 0) {
                    if (target->partyRecord.unitId == 0x12F) {
                        break;
                    }
                }
            }
        }
    }
    if (target == 0) {
        return 1;
    }
    if (command->state != 0x1E || command->motionProgress != 0) {
        return 1;
    }
    btlCopyMotionTransform(&command->frontCamera, &command->camera);
    command->motionParameter = 10.0f;
    command->motionProgress = 1;
    command->state = 0;
    btlSetActorEffectParameterOrMuzzlePosition((BtlUnit *)target, 0);
    VU0_STORE_VF_UNCLOBBERED(vf10, command->backCamera.position);
    command->backCamera.position[1] += 150.0f;
    btlInterpolateVectorStep(&command->frontCamera);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, command->backCamera.position);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    command->backCamera.distance = distance;
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, command->backCamera.direction);
    command->backCamera.distance += 45.0f;
    btlAdjustCameraDirectionForDefaultPlane(&command->backCamera);
    return 1;
}

/* Recognize the two action codes accepted by the linked-command aim handler. */
s32 func_00228F20(BtlLinkedCommand *command) {
    switch (command->actionCode) {
    case 0x171:
        return 1;
    case 0x189:
        return 1;
    }
    return 0;
}

s32 func_00228F48(BtlLinkedCommand *command) {
    u32 choice;

    if (command->actionCode != 0x188) {
        return 0;
    }
    if (btlHasMarkedEntry14((s32)command) != 0) {
        if (command->motionProgress >= 0x19) {
            btlClearRuntimeFlag2000();
            if (command->motionProgress == 0x19) {
                choice = effMiscRandMod(0, 2);
                switch (choice) {
                case 0:
                    btlSetEffectCameraKeys((s32)command,
                        -438.9f, -787.7f, 536.2f, 0.091f,
                        -0.889f, -0.328f, 0.275f, -533.3f,
                        -901.0f, 631.6f, 0.098f, -0.883f,
                        -0.326f, 0.295f, 40.0f, 15.0f);
                    break;
                case 1:
                    btlSetEffectCameraKeys((s32)command,
                        359.4f, -728.1f, 534.7f, -0.086f,
                        -0.911f, -0.283f, -0.254f, 399.0f,
                        -801.4f, 632.3f, -0.084f, -0.913f,
                        -0.284f, -0.248f, 40.0f, 15.0f);
                    break;
                }
                command->state = 0;
            }
        }
        command->motionProgress++;
    }
    return 1;
}

u32 btlGetEffectActive(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    u32 battleId = battle->battleMode;
    BattleEffectPayload *state;
    if (battleId != 0x312) {
        return 0;
    }
    state = battle->effect;
    if (state != NULL) {
        return state->linked.active;
    }
    return 0;
}

s32 btlHasEffectActor(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    u32 battleId = battle->battleMode;
    BattleEffectPayload *state;
    if (battleId != 0x312) {
        return 0;
    }
    state = battle->effect;
    if (state == NULL) {
        return 0;
    }
    return state->linked.actor != 0;
}

u32 btlGetEffectValue(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    u32 battleId = battle->battleMode;
    BattleEffectPayload *state;
    if (battleId != 0x312) {
        return 0;
    }
    state = battle->effect;
    if (state != NULL) {
        return state->linked.value;
    }
    return 0;
}

BtlUnit *btlGetEffectActor(void) {
    BattleEffectPayload *state = ((BtlState *)btlGetRuntime())->effect;
    return state->linked.actor;
}

s32 btlIsSpecialEnemyEffectLinkSatisfied(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit = battle->units;
    BattleEffectPayload *effect = battle->effect;
    while (unit != 0) {
        if ((unit->status.flags & 0x400) &&
            unit->partyRecord.unitId == 0x12F) {
            break;
        }
        unit = unit->nextActor;
    }
    if (unit == 0) {
        return 1;
    }
    if (!(unit->status.flags & 0xe0)) {
        return 0;
    }
    return effect->linked.linkedUnit == unit;
}

void btlArmEventResourceTrigger(void) {
    BattleEventResourceTriggerState *trigger;
    BtlState *battle;

    battle = (BtlState *)btlGetRuntime();
    trigger = &battle->effect->eventTrigger;
    trigger->armed = 1;
    trigger->consumed = 0;
}

u32 func_00229278(void) {
    return 0xffffffff;
}

s32 btlConsumeReadyEventScriptResource(void) {
    s32 result = -1;
    s32 state;

    state = *(s32 *)(btlGetRuntime() + 0x718);
    if (*(s8 *)(state + 0) != 0) {
        if (*(s8 *)(state + 1) != 0) {
            *(u8 *)(state + 1) = 0;
            return btlFindScriptResource(D_00436CF8);
        }
        *(u8 *)(state + 0) = 0;
        return -1;
    }
    return result;
}

void func_002292D8(void) {
    u16 flags;
    DatPartyRecord *entry;
    u32 markedCount;
    u32 index;
    u32 activeCount;
    u8 hasKindTwo;

    hasKindTwo = 0;
    markedCount = 0;
    activeCount = 0;
    entry = datGameState->party;
    index = 0;
    do {
        flags = entry->flags;
        if ((flags & 1) != 0) {
            if (entry->unitId == 2) {
                if ((flags & 2) != 0) {
                    return;
                }
                hasKindTwo = 1;
            }
            activeCount = activeCount + 1;
            if ((flags & 2) != 0) {
                markedCount = markedCount + 1;
            }
        }
        index = index + 1;
        entry++;
    } while (index < 5);
    if (((activeCount < 4) && (markedCount < 3)) && (hasKindTwo)) {
        ptyRebalanceFrontline(2);
        return;
    }
}

s32 btlClearEffectNodeRuntimeFlagForActiveUnits(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if (unit->status.flags & 1) {
            unit->status.stateFlags &= ~0x100000;
        }
        unit = unit->nextActor;
    }
    return -1;
}

void btlMarkBattleUnitEntryForActiveKind(BtlUnit *unit) {
    u32 flags = unit->status.flags;
    if ((flags & 0x200) && unit->partyRecord.unitId == 2) {
        unit->status.flags = flags | 0x1000;
        unit->status.stateFlags |= 0x100000;
        unit->partyRecord.flags |= 0x1000;
    }
}

void btlSetAlternateKindForEnabledSpecialUnit(BtlUnit *unit) {
    if ((unit->status.flags & 0x400) &&
        unit->partyRecord.unitId == 0x144 &&
        mdlFlagTest(0x841)) {
        unit->partyRecord.hp = 1;
    }
}

u32 func_00229470(void) {
    return 6;
}

void func_00229478(void) {
    ptyRebalanceFrontline(7);
}

void func_00229490(void) {
    ptyRebalanceFrontline(1);
    ptyRebalanceFrontline(4);
    ptyRebalanceFrontline(5);
}

void func_002294B8(void) {
    ptyRebalanceFrontline(8);
}

extern s32 btlIsCurrentValueBelowQuarterThreshold(BtlUnit *unit);

/* Only scene-listed special units use these alternate action codes. */
s32 btlRemapListedUnitAction(BtlUnit *unit, s32 action) {
    BtlState *battle;
    u16 *listedMode;
    u32 i;

    if ((unit->status.flags & 0x400) == 0) {
        return action;
    }
    battle = (BtlState *)btlGetRuntime();
    i = 0;
    listedMode = ((DatBattleSceneRecord *)(battle->battleMode * (s32)sizeof(DatBattleSceneRecord) +
                                      (u32)datBattleSceneRecords))->unitModes;
    while (i < 0xB && listedMode[i] != unit->partyRecord.unitId) {
        i++;
    }
    if (i == 0xB) {
        return action;
    }
    switch (action) {
    case 2:
    case 9:
        return btlIsCurrentValueBelowQuarterThreshold(unit) ? 10 : 0;
    case 11:
        return 1;
    case 13:
        return -1;
    default:
        return action;
    }
}

s32 func_002295D8(BtlUnit *unit, s32 action, s32 unused) {
    BtlState *battle;
    u16 *listedMode;
    u32 i;

    if ((unit->status.flags & 0x400) == 0) {
        return action;
    }
    battle = (BtlState *)btlGetRuntime();
    i = 0;
    listedMode = ((DatBattleSceneRecord *)(battle->battleMode *
                    (s32)sizeof(DatBattleSceneRecord) +
                    (u32)datBattleSceneRecords))->unitModes;
    while (i < 11 && listedMode[i] != unit->partyRecord.unitId) {
        i++;
    }
    if (i == 11) {
        return action;
    }
    return action == 13 ? -1 : action;
}

s32 btlIsSceneUnitModeListed(BtlUnit *unit) {
    BtlState *battle;
    u16 *listedMode;
    u32 i;

    if ((unit->status.flags & 0x400) == 0) {
        return 0;
    }
    battle = (BtlState *)btlGetRuntime();
    i = 0;
    listedMode = ((DatBattleSceneRecord *)(battle->battleMode * (s32)sizeof(DatBattleSceneRecord) + (u32)datBattleSceneRecords))->unitModes;
    for (; i < 0xB; i++) {
        if (listedMode[i] == unit->partyRecord.unitId) {
            return 1;
        }
    }
    return 0;
}

#include "sdf_projection.h"

typedef struct ActionUnit ActionUnit;
extern const char D_0041B4F8[];
extern void *memset(void *, s32, u32);
extern u32 btlAcceptLinkedActorCommand(BtlUnit *unit, u32 command);
extern s32 btlAccumulateBossRatioScale(ActionStateLink *task);
extern s32 btlActionResourceTypeToMotionId(BtlUnit *unit, s32 action);
extern u32 btlActivateMarkedActionFromCommand(u32 unused1, u32 unused2, u32 action);
extern s32 btlAdvanceBrahmaRatioOnAction(ActionStateLink *unit);
extern s32 btlAdvanceTimedActionState(BtlLinkedCommand *command);
extern s32 btlAimAtLinkedTargetOrGroupCamera(BtlLinkedCommand *object);
extern s32 btlAimLinkedTargetOrSetCameraTransform(BtlLinkedCommand *object);
extern u32 btlApplySingleTargetCameraOffset(BtlLinkedCommand *unit);
extern void btlApplySpecialActionRenderGroup(BtlUnit *unit, u32 group, f32 opacity);
extern void btlCancelCurrentSubtask(void);
extern void btlCenterMarkedFormationAroundLead(void);
extern s32 btlCheckActionRecordUnit(ActionStateLink *record);
extern s32 btlCheckActionUnitResourceEligibility(BtlUnit *unit, s32 type);
extern s32 btlCheckActiveEffectForSpecialTarget(BtlUnit *actor, BtlUnit *target, s32 command, s32 bits);
extern void btlClaimCommandSlot(BtlUnit *unit, BtlOperandEntry *entry);
extern void btlClaimCommandSlotAndTarget(BtlUnit *unit, BtlOperandEntry *entry);
extern void btlClearActionPhaseOnNegativeState(BtlUnit *unit, u32 state);
extern void btlClearSubtaskHandle(void);
extern s32 btlClearUnitRestrictionFlag(void);
extern u64 btlCreateLinkedActorTransformTasks(u64 prerequisiteHandle);
extern void btlDestroyActionActor(void);
extern s32 btlDispatchActionByResourceFlags(BtlLinkedCommand *unit);
extern s64 btlEnsureHeroUnitTask(u64 prerequisiteHandle);
extern s32 btlFilterActionByUnitFlags(BtlUnit *unit, s32 action);
extern s32 btlFilterRestrictedCommand(BtlUnit *battler, s32 command);
extern BtlUnit *btlFindUnitByMode(void);
extern s32 btlFlagAbadonHpMpTrigger(s32 unit, s32 unused, s32 action);
extern u32 btlFlagBattleForSpecialAction(u32 unit, u32 actor, u32 action);
extern void btlFormatSpecialMotionDisplayCode(s32 unit, u32 action, char *buffer);
extern f32 btlGetActionScaleFactor(BtlUnit *unit, BtlUnit *other);
extern s32 btlGetBossEntryKind(BtlUnit *unit, s32 index);
extern f32 btlGetBossPresenceActionScale(BtlUnit *unit, BtlUnit *target);
extern f32 btlGetBossRatioScale(BtlUnit *unit, s32 unused, s32 kind, s32 flag);
extern f32 btlGetBrahmaActionScale(BtlUnit *unit, u32 actor, u32 action, u32 mode);
extern s32 btlGetHealthyAllyActionStatus(BtlUnit *unit);
extern u32 btlGetMarkedActionMotionCode(u32 unit, u32 action);
extern u32 btlGetMarkedUnitActionResponse(BtlUnit *unit, BtlUnit *actor, u32 action);
extern s32 btlGetMarkedUnitActionStatus(BtlUnit *unit);
extern BtlUnit *btlGetReadyUnitForSpecies(s32 mode, u32 species);
extern u32 btlGetSelectedActorAction(void);
extern u32 btlGetSelectionEmptyValue(void);
extern s32 btlGetSpecialEnemyActionStatus(BtlUnit *unit);
extern s32 btlGetSpecialEnemySubtaskGuardResponse(BtlUnit *unit);
extern s32 btlGetSubtaskActorMotionClass(void);
extern s32 btlHandleTargetDirectionOrAction(BtlLinkedCommand *unit);
extern s32 btlInitializeEffectVectors(BtlLinkedCommand *command);
extern u32 btlInitializeMarkedActionTimer(BtlLinkedCommand *unit);
extern u8 btlIsCommandCodeF(u32 unusedUnit, s32 command);
extern s32 btlIsSpecialEnemyActionCode(s32 battler, s32 action);
extern s32 btlIsSpecialMotion(BtlUnit *actor);
extern s32 btlIsSupportedActorAction(BtlUnit *actor, s32 action);
extern s32 btlIsUnitListReady(void);
extern s32 btlLiftLinkedTargetAndUpdateMotion(BtlLinkedCommand *object);
extern s32 btlLiftTowardLinkedTarget(BtlLinkedCommand *object);
extern s32 btlLiftUnitForLinkedTarget(BtlLinkedCommand *object);
extern s32 btlMapActorMotionId(u32 id);
extern s32 btlMapBossEntryKindToIndex(BtlUnit *unit, s32 index);
extern void btlMarkActiveBossUnitExtensionFlags(BtlUnit *unit);
extern void btlMarkSpecialActionUnit(BtlUnit *actor);
extern void btlMarkUnitActionAndStatusForMode(BtlUnit *unit);
extern u64 btlMaskValueWhenSubtaskInactive(u64 value);
extern s32 btlMotionOffsetForActor(s32 actor, s32 base);
extern u32 btlOffsetSpecialActionValue(BtlUnit *unit, u32 base);
extern s32 btlOffsetSpecialEnemyForCommand(BtlLinkedCommand *command);
extern s32 btlOverrideActionResultForEnemyMode(s32 battler, s32 action, s32 defaultValue);
extern s32 btlOverrideSpecialModeCheckResult(BtlUnit *unit, s32 kind, s32 fallback);
extern s32 btlPlayStationedSoundForActiveBossAction(BtlUnit *unit);
extern void btlPrepareLinkedSpecialActionMotion(BtlUnit *unit, s32 selector, s32 firstParameter, s32 secondParameter, s32 mode, f32 frameStep);
extern void btlPrepareSpecialActionSelection(BtlUnit *unit, BtlOperandEntry *entry);
extern void btlPrepareUnitMotionWithSavedOverride(BtlUnit *unit, s32 selector, s32 firstParameter, s32 secondParameter, s32 mode, f32 frameStep);
extern s32 btlQueryLinkedGroupResponse(ActionStateLink *link);
extern void btlQueueLoneFreeTeamHandle(void);
extern s32 btlQueueMarkedSpecialActorSceneGroup(void);
extern void btlQueueSelectedActorResourceAndSound(ActionUnit *unit);
extern u32 btlRaiseSingleTargetCameraPoints(BtlLinkedCommand *unit);
extern s32 btlRaiseUnitForCommandSlot(BtlLinkedCommand *command);
extern void btlRecenterActorsAroundLead(void);
extern void btlRefreshSpecialActionUnits(void);
extern s32 btlRemapBossResponseForActionPhase(BtlUnit *unit, s32 value);
extern s32 btlRemapCommandKind(BtlUnit *unit, s32 kind);
extern s32 btlRemapMarkedUnitCommand(BtlUnit *unit, s32 action);
extern void btlResetActionEffectOnUnit(void);
extern void btlResetActionScale(void);
extern void btlResetBossRatioScale(void);
extern u32 btlResetDelayedActionTimer(BtlLinkedCommand *unit);
extern void btlResetEffectState(void);
extern void btlResetSpecialActionActorPresentation(void);
extern void btlResetUnitPlacement(void);
extern void btlResetUnitSelectionStateAndSetMode(void);
extern s32 btlResolveBoundActionCode(s32 battler, s32 action);
extern s64 btlRestoreEnemyUnitWhenModelFlagSet(BtlUnit *battler, BtlOperandEntry *resource);
extern void btlRestoreLinkedActorSceneColor(void);
extern void btlRestoreMarkedUnitMotionOnStateChange(void);
extern void btlReturnUnitToGroup(ActionStateLink *task);
extern s32 btlSelectActionCameraByTableFlags(BtlLinkedCommand *unit);
extern s32 btlSelectActionTransitionCamera(BtlLinkedCommand *command, s8 modeA, s8 modeB);
extern s32 btlSelectDisabledCommand(BtlUnit *battler);
extern s32 btlSelectLinkedActionCameraPose(BtlLinkedCommand *command, s8 modeA, s8 modeB);
extern s32 btlSelectLowestStatTarget(ActionStateLink *actor);
extern s32 btlSelectMarkedActorAndClearEntryFlags(BtlUnit *unit, BtlOperandEntry *entry);
extern s32 btlSelectRaisedCameraFromActionFlags(BtlLinkedCommand *unit);
extern s32 btlSelectSoleEligibleActor(void);
extern u32 btlSetBattleActionFlag(u32 unused1, u32 unused2, u32 action);
extern s32 btlSetLinkedDefeatCameraPresetA(BtlLinkedCommand *, BtlCamState *, s32);
extern s32 btlSetLinkedDefeatCameraPresetB(BtlLinkedCommand *command, BtlCamState *camera, s32 rotate);
extern s32 btlSetSpecialDefeatCameraPreset(BtlLinkedCommand *command, BtlCamState *camera, s32 rotate);
extern s32 btlSetSpecialLinkedActionCamera(BtlLinkedCommand *command);
extern s32 btlSetSubtaskControlEnabled(s32 unused, s32 ignored, s32 action);
extern void btlSetUnitResourceFloatByMode(BtlUnit *unit, s32 group, f32 value);
extern void btlSetUnitResourceHalvesByMode(BtlUnit *unit, s32 first, s32 second);
extern s32 btlSetupHariActionCameraPair(BtlLinkedCommand *command, s32 unusedGroup200, s32 unusedGroup400);
extern s32 btlShiftUnitUpForScriptAction(BtlLinkedCommand *command);
extern void btlSpawnBrahmaActionEffectTasks(ActionStateLink *unit, u32 action, u32 unused, u64 prerequisiteHandle);
extern s32 btlSpawnLinkedActionEffect(u8 *task);
extern u32 btlStartAction19A(BtlLinkedCommand *unit);
extern void btlStartActionRecordSoundTask(ActionStateLink *record, u64 prerequisiteHandle, s32 delayBase);
extern s32 btlStartActionRecordTasks(ActionStateLink *record);
extern s32 btlStartLinkedActionMotionPrimary(BtlLinkedCommand *command);
extern u32 btlStartLinkedDefeatCandidateAction(BtlLinkedCommand *unit);
extern u32 btlStartMarkedActionRuntimeUpdate(BtlLinkedCommand *unit);
extern s32 btlStartOtherMarkedUnitTasks(void);
extern void btlStartPrevUnitScriptAction(ActionStateLink *handle);
extern void btlStartReadyUnitAction(void);
extern void btlStartReadyUnitActionCopy(void);
extern u64 btlStartSubtaskWithInput(u64 prerequisiteHandle);
extern void btlStartUnitActionIfPairedSelected(void);
extern u32 btlTickAction19A(BtlLinkedCommand *unit);
extern u32 btlTickAction6B(BtlLinkedCommand *unit);
extern u32 btlTickDelayedMarkedAction(BtlLinkedCommand *unit);
extern u32 btlTickLinkedDefeatCandidateAction(BtlLinkedCommand *unit);
extern void btlToggleActionByteForFlaggedActor(u32 unused, u32 actor);
extern void btlToggleMarkedTaskActionState(BtlUnit *unused, s32 *delta);
extern s32 btlTriggerLinkedActionMotion(BtlLinkedCommand *command);
extern s32 btlTriggerLinkedActionMotionAlternate(BtlLinkedCommand *command);
extern u32 btlTryTransitionSingleTargetAction(BtlLinkedCommand *unit);
extern void btlUpdateLinkedEffectUnitTransforms(void);
extern void btlUpdateSpecialActorChunkFade(void);
extern void effBTLFieldColorSetFlags(u32 bits);
extern void func_00217EB8(ActionStateLink *action);
extern s32 func_00218150(void);
extern void func_00218250(void);
extern s32 func_00218690(void);
extern s32 func_002186C0(BtlUnit *unit, s32 action);
extern s32 func_00218798(BtlLinkedCommand *command);
extern void func_00218968(void);
extern void func_00218BA8(BtlUnit *unit, u8 *arg1);
extern void func_00219278(void);
extern void func_002195E0(ActionStateLink *record);
extern void func_00219760(void);
extern s32 func_002198D8(u8 *unit);
extern s32 func_00219950(u8 *unit);
extern s32 func_00219F28(s32 battler, s32 action);
extern s32 func_0021A098(void);
extern void func_0021B4A8(void);
extern void func_0021C0C8(ActionStateLink *handle, s32 unused, u64 completionOwner, u64 prerequisite);
extern s32 func_0021C818(BtlLinkedCommand *command, s8 side, s8 targetSide);
extern s32 func_0021E778(BtlLinkedCommand *command);
extern u8 func_0021F3A0(s32 arg0);
extern void func_00220368(void);
extern void func_00220998(void);
extern s32 func_00220DD8(BtlUnit *unit, s32 command);
extern s32 func_00220F68(BtlUnit *unit);
extern u32 func_00221828(u32 unit);
extern u32 func_00221858(u32 unit);
extern u32 func_00221F40(u32 unit, u32 actor, u32 action);
extern void func_00222100(ActionStateLink *link);
extern s32 func_00222450(BtlLinkedCommand *command, BtlCamState *camera, s32 rotate);
extern s32 func_00223BD8(BtlLinkedCommand *unit);
extern s32 func_00223DD8(BtlLinkedCommand *unit);
extern u32 func_00223FB0(BtlLinkedCommand *unit);
extern u32 func_00223FE0(BtlLinkedCommand *unit);
extern u32 func_00224010(u32 unused, s32 motion);
extern s32 func_00224500(s32 object);
extern s32 func_00224DF0(BtlLinkedCommand *unit);
extern s32 func_002259A0(BtlLinkedCommand *command, s8 modeA, s8 modeB);
extern s32 func_00225B48(BtlLinkedCommand *unit);
extern s32 func_002261A8(BtlLinkedCommand *unit);
extern f32 func_00226308(BtlUnit *unit, s32 actor, s32 command, s32 mode);
extern s32 func_00226540(u32 unused1, u32 unused2, s32 action);
extern s32 func_00226598(BtlLinkedCommand *command);
extern u32 func_00226670(void);
extern s64 func_00226820(ActionStateLink *unit);
extern u32 func_00226850(BtlLinkedCommand *unit);
extern u32 func_00226868(BtlLinkedCommand *unit);
extern void func_00226C98(BtlUnit *unit, BtlOperandEntry *entry);
extern void mdlFlagClear(s32 flag);

/* Audit artifact only; no repository edits. Consumer-complete DDS2 ABI. */
extern void btlTrackSpecialEnemyCommandRestrictionByTurn(BtlUnit *, BtlOperandEntry *);
extern s32 btlUnitWrapA(BtlLinkedCommand *, BtlCamState *, s32);
extern s32 btlUnitWrapB(BtlLinkedCommand *, BtlCamState *, s32);
extern s32 func_002181E8(void);
extern void func_0021A978(BtlUnit *, s32, s32, s32, s32, f32);
extern void func_0021B828(ActionStateLink *);
extern s32 func_0021C7F8(BtlLinkedCommand *, BtlCamState *, s32);
extern s32 func_0021CF18(BtlLinkedCommand *, s32, s32);
extern s32 func_0021E8C0(BtlLinkedCommand *);
extern void func_0021F848(ActionStateLink *, BtlUnit *, u64, u64);
extern void btlSetSpecialEnemyGeometry(void);
extern s32 func_00223350(BtlLinkedCommand *, s32, s32);
extern s32 func_002247D0(BtlLinkedCommand *, s32, s32);
extern s32 func_00225778(BtlLinkedCommand *, BtlCamState *, s32);
extern s32 func_00225BF8(BtlLinkedCommand *, s32, s32);
extern s32 func_00227528(BtlUnit *);
extern void func_002279F0(void);
extern void func_00227DA8(ActionStateLink *, s32, u64, u64, u64);

/* Copy the verified four-byte EE callback representation into its actual slot.
 * Some callback slots remain byte storage or carry older shared prototypes. */
#define BTL_INSTALL_CALLBACK(storage, byteOffset, provider) { \
    __typeof__(&(provider)) callback = &(provider); \
    typedef char CallbackWidthIsFour[(sizeof(callback) == 4) ? 1 : -1]; \
    typedef char CallbackFitsSlot[((byteOffset) + sizeof(callback) <= sizeof(storage)) ? 1 : -1]; \
    memcpy((u8 *)&(storage) + (byteOffset), &callback, sizeof(callback)); \
}

void func_00229728(s32 mode) {
    BtlState *battle;
    void *allocation;
    void (*setup)(void);

    battle = (BtlState *)btlGetRuntime();
    battle->effect = NULL;
    switch (mode) {
    case 0x300: {
        BTL_INSTALL_CALLBACK(battle->completionHook, 0x0, btlStartUnitActionIfPairedSelected);
        BTL_INSTALL_CALLBACK(battle->linkedActionHook, 0x0, func_00217EB8);
        BTL_INSTALL_CALLBACK(battle->scriptReturnHook, 0x0, btlGetSelectionEmptyValue);
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, btlShiftUnitUpForScriptAction);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, func_002295D8);
        allocation = sdfAllocateBlockBySizeThreshold(0x1);
        battle->effect = allocation;
        memset(allocation, 0, 0x1);
        break;
    }
    case 0x302:
    case 0x31E:
    case 0x32A:
    case 0x32B: {
        if (mode == 0x302) {
            mdlFlagClear(0x804);
            battle->commandRestrictFlags |= 0x84000;
            BTL_INSTALL_CALLBACK(battle->selectScriptState, 0x0, func_002181E8);
            BTL_INSTALL_CALLBACK(battle->pad5E8, 0x0, func_00218150);
        }
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, btlRestoreEnemyUnitWhenModelFlagSet);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, func_002295D8);
        break;
    }
    case 0x303:
    case 0x304: {
        u32 commandFlags;
        u32 preparedFlags;
        commandFlags = battle->commandRestrictFlags;
        preparedFlags = commandFlags | 0x800;
        battle->commandRestrictFlags = preparedFlags;
        if (mode == 0x303) {
            battle->commandRestrictFlags = preparedFlags | 0x100000;
        } else {
            battle->commandRestrictFlags = commandFlags | 0x804;
        }
        BTL_INSTALL_CALLBACK(battle->unk6F0, 0x0, btlPrepareUnitMotionWithSavedOverride);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, func_002186C0);
        BTL_INSTALL_CALLBACK(battle->unk6FC, 0x0, btlOverrideSpecialModeCheckResult);
        BTL_INSTALL_CALLBACK(battle->scriptReturnHook, 0x0, func_00218690);
        BTL_INSTALL_CALLBACK(battle->pad634, 0x0, func_00218250);
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, btlClaimCommandSlot);
        BTL_INSTALL_CALLBACK(battle->completionHook, 0x0, btlStartReadyUnitAction);
        BTL_INSTALL_CALLBACK(battle->linkedActionHook, 0x0, btlStartPrevUnitScriptAction);
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, func_00218798);
        allocation = sdfAllocateBlockBySizeThreshold(0x8);
        battle->effect = allocation;
        memset(allocation, 0, 0x8);
        break;
    }
    case 0x305: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        battle->commandRestrictFlags = commandFlags | 0x100800;
        BTL_INSTALL_CALLBACK(battle->pad5C4, 0x0, func_00218968);
        BTL_INSTALL_CALLBACK(battle->unk6FC, 0x0, btlOverrideActionResultForEnemyMode);
        BTL_INSTALL_CALLBACK(battle->scriptReturnHook, 0x0, btlIsUnitListReady);
        break;
    }
    case 0x306: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        battle->commandRestrictFlags = commandFlags | 0x100004;
        BTL_INSTALL_CALLBACK(battle->pad5C4, 0x0, btlResetUnitSelectionStateAndSetMode);
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, func_00218BA8);
        BTL_INSTALL_CALLBACK(battle->actionHitOverride, 0x0, btlFlagAbadonHpMpTrigger);
        BTL_INSTALL_CALLBACK(battle->actionStateSelectionHook, 0x0, btlCheckActionRecordUnit);
        BTL_INSTALL_CALLBACK(battle->commandHook, 0x0, btlStartActionRecordTasks);
        BTL_INSTALL_CALLBACK(battle->unk5D8, 0x0, btlGetSpecialEnemySubtaskGuardResponse);
        BTL_INSTALL_CALLBACK(battle->unk5DC, 0x0, btlIsSpecialEnemyActionCode);
        BTL_INSTALL_CALLBACK(battle->pad5E8, 0x0, btlSelectSoleEligibleActor);
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, btlRaiseUnitForCommandSlot);
        BTL_INSTALL_CALLBACK(battle->pad674, 0x0, btlSpawnLinkedActionEffect);
        BTL_INSTALL_CALLBACK(battle->modelChangeSoundHook, 0x0, btlStartActionRecordSoundTask);
        BTL_INSTALL_CALLBACK(battle->postPlacementCallback, 0x0, btlResetActionEffectOnUnit);
        BTL_INSTALL_CALLBACK(battle->afterUnitUpdate, 0x0, func_00219278);
        allocation = sdfAllocateBlockBySizeThreshold(0xC);
        battle->effect = allocation;
        memset(allocation, 0, 0xC);
        break;
    }
    case 0x307:
    case 0x308:
    case 0x309: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        battle->commandRestrictFlags = commandFlags | 0x100000;
        battle->cameraCommand.cameraDistanceOffset = 150.0f;
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, btlClaimCommandSlotAndTarget);
        BTL_INSTALL_CALLBACK(battle->actionHitOverride, 0x0, btlSetSubtaskControlEnabled);
        BTL_INSTALL_CALLBACK(battle->pad634, 0x0, btlClearSubtaskHandle);
        BTL_INSTALL_CALLBACK(battle->completionHook, 0x0, btlStartReadyUnitActionCopy);
        BTL_INSTALL_CALLBACK(battle->linkedActionHook, 0x0, func_002195E0);
        BTL_INSTALL_CALLBACK(battle->afterUnitUpdate, 0x0, func_00219760);
        BTL_INSTALL_CALLBACK(battle->scriptReturnHook, 0x0, btlGetSubtaskActorMotionClass);
        BTL_INSTALL_CALLBACK(battle->unk69C, 0x0, func_002198D8);
        BTL_INSTALL_CALLBACK(battle->unk6A0, 0x0, func_00219950);
        BTL_INSTALL_CALLBACK(battle->unk648, 0x0, btlTriggerLinkedActionMotionAlternate);
        BTL_INSTALL_CALLBACK(battle->unk64C, 0x0, btlTriggerLinkedActionMotion);
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, btlStartLinkedActionMotionPrimary);
        BTL_INSTALL_CALLBACK(battle->handleActorCategoryCamera, 0x0, btlSetupHariActionCameraPair);
        BTL_INSTALL_CALLBACK(battle->unk66C, 0x0, btlAdvanceTimedActionState);
        BTL_INSTALL_CALLBACK(battle->unk5D8, 0x0, btlGetSpecialEnemyActionStatus);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlRemapCommandKind);
        BTL_INSTALL_CALLBACK(battle->unk5DC, 0x0, func_00219F28);
        allocation = sdfAllocateBlockBySizeThreshold(0x10);
        battle->effect = allocation;
        memset(allocation, 0, 0x10);
        break;
    }
    case 0x30A:
    case 0x320: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        battle->commandRestrictFlags = commandFlags | 0x200000;
        BTL_INSTALL_CALLBACK(battle->postPlacementCallback, 0x0, btlRecenterActorsAroundLead);
        BTL_INSTALL_CALLBACK(battle->pad5E8, 0x0, func_0021A098);
        break;
    }
    case 0x30B: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        commandFlags = commandFlags | 0x400200;
        battle->commandRestrictFlags = commandFlags | 0x100000;
        effBTLFieldColorSetFlags(1);
        BTL_INSTALL_CALLBACK(battle->effectParameterCallback, 0x0, btlMotionOffsetForActor);
        BTL_INSTALL_CALLBACK(battle->unitReturnHook, 0x0, btlReturnUnitToGroup);
        BTL_INSTALL_CALLBACK(battle->pad610, 0x0, btlUpdateSpecialActorChunkFade);
        BTL_INSTALL_CALLBACK(battle->findModelActor, 0x0, btlGetReadyUnitForSpecies);
        BTL_INSTALL_CALLBACK(battle->beginBattleEntryTasks, 0x0, btlEnsureHeroUnitTask);
        BTL_INSTALL_CALLBACK(battle->pad604, 0x0, btlCancelCurrentSubtask);
        BTL_INSTALL_CALLBACK(battle->finishEnemyEntryTasks, 0x0, btlStartSubtaskWithInput);
        BTL_INSTALL_CALLBACK(battle->afterActorModelReady, 0x0, btlMarkActiveBossUnitExtensionFlags);
        BTL_INSTALL_CALLBACK(battle->hitChanceScale, 0x0, btlGetBossPresenceActionScale);
        BTL_INSTALL_CALLBACK(battle->postPlacementCallback, 0x0, btlResetSpecialActionActorPresentation);
        BTL_INSTALL_CALLBACK(battle->afterUnitUpdate, 0x0, func_0021B4A8);
        BTL_INSTALL_CALLBACK(battle->unk6F0, 0x0, func_0021A978);
        BTL_INSTALL_CALLBACK(battle->unk6F4, 0x0, btlSetUnitResourceFloatByMode);
        BTL_INSTALL_CALLBACK(battle->unk6F8, 0x0, btlSetUnitResourceHalvesByMode);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlResolveBoundActionCode);
        BTL_INSTALL_CALLBACK(battle->unk670, 0x0, btlInitializeEffectVectors);
        BTL_INSTALL_CALLBACK(battle->pad634, 0x0, func_0021B828);
        BTL_INSTALL_CALLBACK(battle->preActionHook, 0x0, func_0021C0C8);
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, func_0021C818);
        BTL_INSTALL_CALLBACK(battle->handleActorCategoryCamera, 0x0, func_0021CF18);
        BTL_INSTALL_CALLBACK(battle->cameraArrangementHook, 0x0, btlSetLinkedDefeatCameraPresetA);
        BTL_INSTALL_CALLBACK(battle->defeatCameraHook, 0x0, func_0021C7F8);
        BTL_INSTALL_CALLBACK(battle->unk684, 0x0, btlMapBossEntryKindToIndex);
        BTL_INSTALL_CALLBACK(battle->unk688, 0x0, btlGetBossEntryKind);
        BTL_INSTALL_CALLBACK(battle->unk710, 0x0, btlRemapBossResponseForActionPhase);
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, btlPlayStationedSoundForActiveBossAction);
        BTL_INSTALL_CALLBACK(battle->actionCameraSetupHook, 0x0, func_0021E778);
        BTL_INSTALL_CALLBACK(battle->unk658, 0x0, func_0021E8C0);
        allocation = sdfAllocateBlockBySizeThreshold(0xC);
        battle->effect = allocation;
        memset(allocation, 0, 0xC);
        break;
    }
    case 0x301: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        battle->commandRestrictFlags = commandFlags | 4;
        BTL_INSTALL_CALLBACK(battle->pad5C4, 0x0, btlResetBossRatioScale);
        BTL_INSTALL_CALLBACK(battle->pad634, 0x0, btlAccumulateBossRatioScale);
        BTL_INSTALL_CALLBACK(battle->commandAmountScaleHook, 0x0, btlGetBossRatioScale);
        allocation = sdfAllocateBlockBySizeThreshold(0x4);
        battle->effect = allocation;
        memset(allocation, 0, 0x4);
        break;
    }
    case 0x30E: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        battle->commandRestrictFlags = commandFlags | 4;
        BTL_INSTALL_CALLBACK(battle->selectSoundEffectTarget, 0x0, btlFindUnitByMode);
        BTL_INSTALL_CALLBACK(battle->unk6E4, 0x0, btlMaskValueWhenSubtaskInactive);
        BTL_INSTALL_CALLBACK(battle->afterUnitUpdate, 0x0, btlRestoreMarkedUnitMotionOnStateChange);
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, btlToggleActionByteForFlaggedActor);
        BTL_INSTALL_CALLBACK(battle->actionHitOverride, 0x0, btlGetMarkedUnitActionResponse);
        BTL_INSTALL_CALLBACK(battle->unk5D8, 0x0, btlGetMarkedUnitActionStatus);
        BTL_INSTALL_CALLBACK(battle->actionResourceNameHook, 0x0, btlFormatSpecialMotionDisplayCode);
        BTL_INSTALL_CALLBACK(battle->postPlacementCallback, 0x0, btlCenterMarkedFormationAroundLead);
        BTL_INSTALL_CALLBACK(battle->pad5E8, 0x0, btlStartOtherMarkedUnitTasks);
        BTL_INSTALL_CALLBACK(battle->pad674, 0x4, func_0021F3A0);
        BTL_INSTALL_CALLBACK(battle->pad6C0, 0x4, btlMapActorMotionId);
        BTL_INSTALL_CALLBACK(battle->pad694, 0x0, func_0021F848);
        BTL_INSTALL_CALLBACK(battle->unk618, 0x0, btlIsSpecialMotion);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, func_002295D8);
        allocation = sdfAllocateBlockBySizeThreshold(0x8);
        battle->effect = allocation;
        memset(allocation, 0, 0x8);
        break;
    }
    case 0x314: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        BTL_INSTALL_CALLBACK(battle->afterUnitUpdate, 0x0, func_00220368);
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, btlToggleMarkedTaskActionState);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlRemapMarkedUnitCommand);
        commandFlags = commandFlags | 0x100000;
        battle->commandRestrictFlags = commandFlags | 0x200000;
        BTL_INSTALL_CALLBACK(battle->hitResultOverride, 0x0, btlSetBattleActionFlag);
        BTL_INSTALL_CALLBACK(battle->unk5D8, 0x0, btlGetHealthyAllyActionStatus);
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, btlOffsetSpecialEnemyForCommand);
        BTL_INSTALL_CALLBACK(battle->unk5DC, 0x0, btlIsSupportedActorAction);
        allocation = sdfAllocateBlockBySizeThreshold(0x3);
        battle->effect = allocation;
        memset(allocation, 0, 0x3);
        break;
    }
    case 0x30C: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        BTL_INSTALL_CALLBACK(battle->pad5C4, 0x0, func_00220998);
        BTL_INSTALL_CALLBACK(battle->initializeUnitEntry, 0x0, btlMarkUnitActionAndStatusForMode);
        BTL_INSTALL_CALLBACK(battle->actionCameraStepHook, 0x0, btlTickAction19A);
        battle->commandRestrictFlags = (commandFlags & ~2) | 0x100000;
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, btlStartAction19A);
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, btlSelectMarkedActorAndClearEntryFlags);
        BTL_INSTALL_CALLBACK(battle->completionHook, 0x0, btlQueueLoneFreeTeamHandle);
        BTL_INSTALL_CALLBACK(battle->linkedActionHook, 0x0, btlQueueSelectedActorResourceAndSound);
        BTL_INSTALL_CALLBACK(battle->scriptReturnHook, 0x0, btlGetSelectedActorAction);
        allocation = sdfAllocateBlockBySizeThreshold(0x4);
        battle->effect = allocation;
        memset(allocation, 0, 0x4);
        break;
    }
    case 0x30F: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        battle->commandRestrictFlags = commandFlags | 4;
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, btlStartLinkedDefeatCandidateAction);
        BTL_INSTALL_CALLBACK(battle->actionCameraStepHook, 0x0, btlTickLinkedDefeatCandidateAction);
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, btlClearActionPhaseOnNegativeState);
        BTL_INSTALL_CALLBACK(battle->actionHitOverride, 0x0, btlActivateMarkedActionFromCommand);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, func_00220DD8);
        BTL_INSTALL_CALLBACK(battle->unk5D8, 0x0, func_00220F68);
        BTL_INSTALL_CALLBACK(battle->unk5DC, 0x0, btlAcceptLinkedActorCommand);
        BTL_INSTALL_CALLBACK(battle->hitChanceScale, 0x0, btlGetActionScaleFactor);
        BTL_INSTALL_CALLBACK(battle->actionEffectOverride, 0x0, btlQueryLinkedGroupResponse);
        allocation = sdfAllocateBlockBySizeThreshold(0x4);
        battle->effect = allocation;
        memset(allocation, 0, 0x4);
        break;
    }
    case 0x316:
    case 0x327: {
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, btlInitializeMarkedActionTimer);
        BTL_INSTALL_CALLBACK(battle->actionCameraStepHook, 0x0, btlStartMarkedActionRuntimeUpdate);
        BTL_INSTALL_CALLBACK(battle->unk66C, 0x0, btlTickAction6B);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlRemapListedUnitAction);
        BTL_INSTALL_CALLBACK(battle->unk618, 0x0, btlIsSceneUnitModeListed);
        break;
    }
    case 0x317:
    case 0x326: {
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, btlResetDelayedActionTimer);
        BTL_INSTALL_CALLBACK(battle->unk66C, 0x0, btlTickDelayedMarkedAction);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlRemapListedUnitAction);
        BTL_INSTALL_CALLBACK(battle->unk618, 0x0, btlIsSceneUnitModeListed);
        break;
    }
    case 0x315: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        commandFlags = commandFlags | 0x18200;
        battle->commandRestrictFlags = commandFlags | 0x800000;
        sdfSceneProjectionParameters.camera.farZ = 200000.0f;
        effBTLFieldColorSetFlags(2);
        BTL_INSTALL_CALLBACK(battle->pad5C4, 0x0, btlResetActionScale);
        BTL_INSTALL_CALLBACK(battle->pad610, 0x0, btlRestoreLinkedActorSceneColor);
        BTL_INSTALL_CALLBACK(battle->initializeUnitEntry, 0x0, btlMarkSpecialActionUnit);
        BTL_INSTALL_CALLBACK(battle->postPlacementCallback, 0x0, btlSetSpecialEnemyGeometry);
        BTL_INSTALL_CALLBACK(battle->pad634, 0x0, btlAdvanceBrahmaRatioOnAction);
        BTL_INSTALL_CALLBACK(battle->commandAmountScaleHook, 0x0, btlGetBrahmaActionScale);
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, btlPrepareSpecialActionSelection);
        BTL_INSTALL_CALLBACK(battle->actionHitOverride, 0x0, func_00221F40);
        BTL_INSTALL_CALLBACK(battle->sceneCallback, 0x0, btlQueueMarkedSpecialActorSceneGroup);
        BTL_INSTALL_CALLBACK(battle->linkedActionHook, 0x0, func_00222100);
        BTL_INSTALL_CALLBACK(battle->unk6F0, 0x0, btlPrepareLinkedSpecialActionMotion);
        BTL_INSTALL_CALLBACK(battle->unk6F4, 0x0, btlApplySpecialActionRenderGroup);
        BTL_INSTALL_CALLBACK(battle->unk684, 0x0, btlActionResourceTypeToMotionId);
        BTL_INSTALL_CALLBACK(battle->afterUnitUpdate, 0x0, btlRefreshSpecialActionUnits);
        BTL_INSTALL_CALLBACK(battle->effectParameterCallback, 0x0, btlOffsetSpecialActionValue);
        BTL_INSTALL_CALLBACK(battle->beginBattleEntryTasks, 0x0, btlCreateLinkedActorTransformTasks);
        BTL_INSTALL_CALLBACK(battle->pad604, 0x0, btlDestroyActionActor);
        BTL_INSTALL_CALLBACK(battle->unk69C, 0x0, func_00221828);
        BTL_INSTALL_CALLBACK(battle->unk6A0, 0x0, func_00221858);
        BTL_INSTALL_CALLBACK(battle->cameraArrangementHook, 0x0, func_00222450);
        BTL_INSTALL_CALLBACK(battle->defeatCameraHook, 0x0, btlUnitWrapA);
        BTL_INSTALL_CALLBACK(battle->unk650, 0x0, btlApplySingleTargetCameraOffset);
        BTL_INSTALL_CALLBACK(battle->unk648, 0x0, btlAimAtLinkedTargetOrGroupCamera);
        BTL_INSTALL_CALLBACK(battle->unk64C, 0x0, btlLiftTowardLinkedTarget);
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, btlSelectActionTransitionCamera);
        BTL_INSTALL_CALLBACK(battle->actionCameraStepHook, 0x0, btlSetSpecialLinkedActionCamera);
        BTL_INSTALL_CALLBACK(battle->handleActorCategoryCamera, 0x0, func_00223350);
        BTL_INSTALL_CALLBACK(battle->unk66C, 0x0, func_00223BD8);
        BTL_INSTALL_CALLBACK(battle->actionCameraSetupHook, 0x0, btlSelectActionCameraByTableFlags);
        BTL_INSTALL_CALLBACK(battle->unk658, 0x0, func_00223DD8);
        BTL_INSTALL_CALLBACK(battle->actionEffectOverride, 0x0, btlGetMarkedActionMotionCode);
        BTL_INSTALL_CALLBACK(battle->preActionHook, 0x0, btlSpawnBrahmaActionEffectTasks);
        allocation = sdfAllocateBlockBySizeThreshold(0x10);
        battle->effect = allocation;
        memset(allocation, 0, 0x10);
        break;
    }
    case 0x31D: {
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, func_00223FB0);
        BTL_INSTALL_CALLBACK(battle->actionCameraStepHook, 0x0, func_00223FE0);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlRemapListedUnitAction);
        BTL_INSTALL_CALLBACK(battle->unk618, 0x0, btlIsSceneUnitModeListed);
        break;
    }
    case 0x31A: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        battle->commandRestrictFlags = commandFlags | 0x100000;
        BTL_INSTALL_CALLBACK(battle->cameraArrangementHook, 0x0, btlSetLinkedDefeatCameraPresetB);
        BTL_INSTALL_CALLBACK(battle->defeatCameraHook, 0x0, btlUnitWrapB);
        BTL_INSTALL_CALLBACK(battle->unk650, 0x0, btlRaiseSingleTargetCameraPoints);
        BTL_INSTALL_CALLBACK(battle->unk648, 0x0, btlAimLinkedTargetOrSetCameraTransform);
        BTL_INSTALL_CALLBACK(battle->unk64C, 0x0, btlLiftLinkedTargetAndUpdateMotion);
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, btlSelectLinkedActionCameraPose);
        BTL_INSTALL_CALLBACK(battle->actionCameraStepHook, 0x0, func_00224500);
        BTL_INSTALL_CALLBACK(battle->handleActorCategoryCamera, 0x0, func_002247D0);
        BTL_INSTALL_CALLBACK(battle->actionCameraSetupHook, 0x0, btlSelectRaisedCameraFromActionFlags);
        BTL_INSTALL_CALLBACK(battle->unk658, 0x0, func_00224DF0);
        BTL_INSTALL_CALLBACK(battle->actionEffectOverride, 0x0, func_00224010);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlRemapListedUnitAction);
        BTL_INSTALL_CALLBACK(battle->unk618, 0x0, btlIsSceneUnitModeListed);
        break;
    }
    case 0x31B: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        commandFlags = commandFlags | 0x100000;
        commandFlags = commandFlags | 0x200000;
        BTL_INSTALL_CALLBACK(battle->cameraArrangementHook, 0x0, btlSetSpecialDefeatCameraPreset);
        battle->commandRestrictFlags = commandFlags;
        BTL_INSTALL_CALLBACK(battle->defeatCameraHook, 0x0, func_00225778);
        BTL_INSTALL_CALLBACK(battle->unk650, 0x0, btlTryTransitionSingleTargetAction);
        BTL_INSTALL_CALLBACK(battle->unk648, 0x0, btlHandleTargetDirectionOrAction);
        BTL_INSTALL_CALLBACK(battle->unk64C, 0x0, btlLiftUnitForLinkedTarget);
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, func_002259A0);
        BTL_INSTALL_CALLBACK(battle->actionCameraStepHook, 0x0, func_00225B48);
        BTL_INSTALL_CALLBACK(battle->handleActorCategoryCamera, 0x0, func_00225BF8);
        BTL_INSTALL_CALLBACK(battle->actionCameraSetupHook, 0x0, btlDispatchActionByResourceFlags);
        BTL_INSTALL_CALLBACK(battle->unk658, 0x0, func_002261A8);
        BTL_INSTALL_CALLBACK(battle->actionHitOverride, 0x0, btlFlagBattleForSpecialAction);
        BTL_INSTALL_CALLBACK(battle->selectSingleTargetOverride, 0x0, btlSelectLowestStatTarget);
        BTL_INSTALL_CALLBACK(battle->unk684, 0x0, btlCheckActionUnitResourceEligibility);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlFilterActionByUnitFlags);
        BTL_INSTALL_CALLBACK(battle->commandAmountScaleHook, 0x0, func_00226308);
        BTL_INSTALL_CALLBACK(battle->actionPointsOverride, 0x0, func_00226540);
        allocation = sdfAllocateBlockBySizeThreshold(0x1);
        battle->effect = allocation;
        memset(allocation, 0, 0x1);
        break;
    }
    case 0x310: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        commandFlags = commandFlags | 0x100001;
        battle->commandRestrictFlags = commandFlags | 0x200000;
        BTL_INSTALL_CALLBACK(battle->selectScriptArg, 0x0, func_00226670);
        break;
    }
    case 0x311: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        commandFlags = commandFlags | 0x100001;
        battle->commandRestrictFlags = commandFlags | 0x200000;
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlFilterRestrictedCommand);
        BTL_INSTALL_CALLBACK(battle->unk5DC, 0x0, btlIsCommandCodeF);
        BTL_INSTALL_CALLBACK(battle->unk5D8, 0x0, btlSelectDisabledCommand);
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, btlTrackSpecialEnemyCommandRestrictionByTurn);
        BTL_INSTALL_CALLBACK(battle->sceneCallback, 0x0, btlClearUnitRestrictionFlag);
        BTL_INSTALL_CALLBACK(battle->commandTurnEndHook, 0x0, func_00226820);
        BTL_INSTALL_CALLBACK(battle->afterUnitUpdate, 0x0, btlResetUnitPlacement);
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, func_00226850);
        BTL_INSTALL_CALLBACK(battle->unk66C, 0x0, func_00226868);
        allocation = sdfAllocateBlockBySizeThreshold(0x4);
        battle->effect = allocation;
        memset(allocation, 0, 0x4);
        break;
    }
    case 0x312: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        commandFlags = commandFlags | 0x100001;
        battle->commandRestrictFlags = commandFlags | 0x200000;
        BTL_INSTALL_CALLBACK(battle->pad5C4, 0x0, btlResetEffectState);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, func_002272A0);
        BTL_INSTALL_CALLBACK(battle->unk5DC, 0x0, btlIsEffectPhaseInRange);
        BTL_INSTALL_CALLBACK(battle->unk5D8, 0x0, func_00227528);
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, func_00226C98);
        BTL_INSTALL_CALLBACK(battle->actionHitOverride, 0x0, btlCheckActiveEffectForSpecialTarget);
        BTL_INSTALL_CALLBACK(battle->postPlacementCallback, 0x0, btlUpdateLinkedEffectUnitTransforms);
        BTL_INSTALL_CALLBACK(battle->afterUnitUpdate, 0x0, func_00227288);
        BTL_INSTALL_CALLBACK(battle->effectParameterCallback, 0x0, btlRemapEffectActiveCombatantAction);
        BTL_INSTALL_CALLBACK(battle->findModelActor, 0x0, btlFindFlaggedSpecialSpeciesUnit);
        BTL_INSTALL_CALLBACK(battle->selectEntryModelVariant, 0x0, btlGetCanonicalCombatantKind);
        BTL_INSTALL_CALLBACK(battle->pad610, 0x0, btlUpdateLinkedActorGroundHeight);
        BTL_INSTALL_CALLBACK(battle->completionHook, 0x0, func_002279F0);
        BTL_INSTALL_CALLBACK(battle->actionStateSelectionHook, 0x0, btlGetEffectTaskActorMatchCode);
        BTL_INSTALL_CALLBACK(battle->commandHook, 0x0, btlEffectTaskStartFinale);
        BTL_INSTALL_CALLBACK(battle->preActionHook, 0x0, func_00227DA8);
        BTL_INSTALL_CALLBACK(battle->pad694, 0x4, btlIsEffectActor);
        BTL_INSTALL_CALLBACK(battle->pad5E8, 0x0, btlGetSoleTargetKind);
        BTL_INSTALL_CALLBACK(battle->actorEligibilityOverride, 0x0, btlHasDifferentActiveTarget);
        BTL_INSTALL_CALLBACK(battle->unk69C, 0x0, btlSetLinkFlagOff);
        BTL_INSTALL_CALLBACK(battle->unk6A0, 0x0, btlSetLinkFlagOn);
        BTL_INSTALL_CALLBACK(battle->linkedActionHook, 0x0, btlQueueLinkedActorModelStateTasks);
        BTL_INSTALL_CALLBACK(battle->unk61C, 0x0, btlTryScheduleMarkedUnitTask);
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, func_00228B08);
        BTL_INSTALL_CALLBACK(battle->actionCameraStepHook, 0x0, btlUnitStartAimAtTarget);
        BTL_INSTALL_CALLBACK(battle->handleActorCategoryCamera, 0x0, func_00228F20);
        BTL_INSTALL_CALLBACK(battle->unk66C, 0x0, func_00228F48);
        allocation = sdfAllocateBlockBySizeThreshold(0x18);
        battle->effect = allocation;
        memset(allocation, 0, 0x18);
        break;
    }
    case 0x313: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        commandFlags = commandFlags | 0x100001;
        battle->commandRestrictFlags = commandFlags | 0x200000;
        BTL_INSTALL_CALLBACK(battle->pad5C4, 0x0, btlArmEventResourceTrigger);
        BTL_INSTALL_CALLBACK(battle->sceneCallback, 0x0, func_00229278);
        BTL_INSTALL_CALLBACK(battle->selectScriptState, 0x0, btlConsumeReadyEventScriptResource);
        allocation = sdfAllocateBlockBySizeThreshold(0x2);
        battle->effect = allocation;
        memset(allocation, 0, 0x2);
        break;
    }
    case 0x30D: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        battle->commandRestrictFlags = commandFlags | 4;
        battle->cameraCommand.cameraDistanceOffset = 100.0f;
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, func_00226598);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlRemapListedUnitAction);
        BTL_INSTALL_CALLBACK(battle->unk618, 0x0, btlIsSceneUnitModeListed);
        break;
    }
    case 0x345: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        battle->commandRestrictFlags = commandFlags | 0x4000;
        BTL_INSTALL_CALLBACK(battle->pad5C4, 0x0, func_002292D8);
        BTL_INSTALL_CALLBACK(battle->actionStateSelectionHook, 0x0, btlClearEffectNodeRuntimeFlagForActiveUnits);
        BTL_INSTALL_CALLBACK(battle->initializeUnitEntry, 0x0, btlMarkBattleUnitEntryForActiveKind);
        break;
    }
    case 0x336: {
        if (mdlFlagTest(0x841) == 0) {
            battle->commandRestrictFlags |= 0x40000;
        }
        battle->commandRestrictFlags |= 0x82800;
        BTL_INSTALL_CALLBACK(battle->initializeUnitEntry, 0x0, btlSetAlternateKindForEnabledSpecialUnit);
        BTL_INSTALL_CALLBACK(battle->pad5E8, 0x0, func_00229470);
        break;
    }
    case 0x32D: {
        BTL_INSTALL_CALLBACK(battle->pad5C4, 0x0, func_00229478);
        break;
    }
    case 0x321: {
        BTL_INSTALL_CALLBACK(battle->pad5C4, 0x0, func_00229490);
        break;
    }
    case 0x344: {
        BTL_INSTALL_CALLBACK(battle->pad5C4, 0x0, func_002294B8);
        break;
    }
    case 0x318:
    case 0x319:
    case 0x31C:
    case 0x31F:
    case 0x328: {
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlRemapListedUnitAction);
        BTL_INSTALL_CALLBACK(battle->unk618, 0x0, btlIsSceneUnitModeListed);
        break;
    }
    default:
        return;
    }

    memcpy(&setup, battle->pad5C4, sizeof(setup));
    if (setup != NULL) {
        setup();
    }
    battle->battleFlags |= 0x80000;
    btlBossDebugPrintf(D_0041B4F8);
}

#undef BTL_INSTALL_CALLBACK

void btlRunCleanupAndLog(void) {
    u8 *battle = (u8 *)btlGetRuntime();
    void (*cleanup)(void) = *(void (**)(void))(battle + 0x604);
    if (cleanup != 0) {
        cleanup();
    }
    btlBossDebugPrintf(D_0041B628);
}

void btlReleaseBossData(void) {
    u8 *battle = (u8 *)btlGetRuntime();
    void (*cleanup)(void);
    if ((((BtlState *)battle)->battleFlags & 0x80000) == 0) {
        return;
    }
    cleanup = ((BtlState *)battle)->bossCleanup;
    if (cleanup != 0) {
        cleanup();
    }
    btlRunCleanupAndLog();
    if (((BtlState *)battle)->effect != 0) {
        sdfReleaseChipOrRetainedResource(((BtlState *)battle)->effect);
        ((BtlState *)battle)->effect = 0;
    }
    ((BtlState *)battle)->battleFlags &= ~0x80000;
    btlBossDebugPrintf(D_0041B640);
}

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B628);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B640);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436CF8);

INCLUDE_SDATA(const s32, "game/code_00227288", btlPrimaryScriptResourceName);

INCLUDE_SDATA(const s32, "game/code_00227288", btlSecondaryScriptResourceName);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D10);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D18);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D20);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D28);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D30);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D38);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D40);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D48);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D50);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D58);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D60);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D68);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D70);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D78);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D80);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D88);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D8E);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D90);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D98);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DA0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DA8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DB0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DB8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DC0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DC8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DD0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DD8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DE0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DE8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DF0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DF8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E00);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E08);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E10);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E18);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E20);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E28);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E30);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E38);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E40);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E48);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E50);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E58);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E60);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E68);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E70);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E78);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E80);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E88);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E90);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E98);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EA0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EA8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EB0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EB8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EC0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EC8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436ED0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436ED8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EE0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EE8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EF0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EF8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F00);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F08);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F10);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F18);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F20);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F28);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F30);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F38);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F40);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F48);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F50);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F58);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F5D);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F5E);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F5F);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F60);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F61);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F62);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F64);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F68);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F6C);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F6E);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F70);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F74);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F78);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F7C);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F80);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F84);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F88);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F90);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F96);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F98);

