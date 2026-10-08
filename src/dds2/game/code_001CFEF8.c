#include "common.h"
#include "pcp_vu0.h"
#include "btl_state.h"
#include "btl_task_args.h"
#include "btl_command.h"
#include "btl_action.h"
#include "dat_state.h"

#define FLD_SCENE_INITIAL_ID 1
#define FLD_SCENE_GROUP_PRIMARY_ID 1
#define FLD_SCENE_GROUP_SECONDARY_ID 2
#define FLD_SCENE_GROUP_TERTIARY_ID 3
#define FLD_SCENE_ACTOR_PRIMARY_BIT 0x200
#define FLD_SCENE_ACTOR_SECONDARY_BIT 0x400
#define FLD_SCENE_ACTOR_TERTIARY_BIT 0x800
#define FLD_SCENE_ACTOR_GROUP_MASK 0xE00
#define FLD_SCENE_TASK_HANDLE_GROUP_BIT 0x40
#define FLD_SCENE_PRIMARY_TASK_COUNT 0x14
#define FLD_SCENE_SECONDARY_TASK_COUNT 0x2D
#define FLD_SCENE_TERTIARY_TASK_COUNT 0xF
#define FLD_SCENE_HANDLE_TASK_COUNT 8
#define FLD_SCENE_SLOT_COUNT 8
#define FLD_SCENE_SLOT_SHIFT_COUNT 7
#define FLD_SCENE_SLOT_HALF_COUNTER 50
#define FLD_SCENE_COUNTERS_ENABLED_BIT 0x100
#define FLD_SCENE_SLOTS_FINISHED_BIT 0x1000
#define FLD_SCENE_COUNTER_FRONT_MODE 1
#define FLD_SCENE_ADVANCE_BIT 4
#define FLD_SCENE_ADVANCE_ENABLE_BITS 0xc
/* Keep the native unsigned mask instead of changing the expression's type. */
#define FLD_SCENE_ADVANCE_CLEAR_MASK 0xfffffffb
#define FLD_SCENE_TASK_ACTIVE_BIT 1
#define FLD_SCENE_TASK_BOUND_BIT 8
#define FLD_SCENE_COUNTER_REQUEST_BYTES 0xC
#define FLD_SCENE_INSERT_REQUEST_BYTES 8
#define FLD_SCENE_SWAP_REQUEST_BYTES 4
#define FLD_SCENE_COUNTER_ACTION_ID 0x61
#define FLD_SCENE_INSERT_ACTION_ID 0x62
#define FLD_SCENE_SWAP_ACTION_ID 0x63

extern s32 btlGetRuntime(void);
extern s32 btlDoesEnabledStatusMatchCurrentId(DatPartyRecord *, u32);

extern void btlDispatchStateHandler(void *, s32);
extern u64 btlStartTask(void *);
extern BtlRuntimeTask *btlAllocTask(s32);

extern s32 btlHasRegisteredGuidePanelTask(void);

extern void btlBossDebugPrintf(const char *, ...);

extern s32 datEnemyRecords;

extern void func_001C7DB8(s32, s32);

extern void func_00230960(s32);


extern s32 func_00230978(void);

extern void kwlnFadeInStart(s32, s32, s32, s32);

extern s32 func_001B4040();

extern s32 func_001B4210();

extern void func_0022AF90();

extern void func_00229728();

extern void btlSelectSceneAudioTrack();

typedef struct {
    void (*initialize)(s32);
    s32 (*update)(s32);
    s32 flags;
} SceneInitializer;

typedef struct BattleEffectParams {
    f32 position[4];
    f32 rotation[4];
    f32 scale[4];
} BattleEffectParams;




extern f32 *D_0037F770[];

extern s32 btlCreateEffectTaskWithSourceParams(BattleEffectParams *, s32);

extern s32 btlCountTasksForOwner(s64);

extern SceneInitializer D_003B6938[];

extern void btlRemoveTaskFromSceneGroup(ActionStateLink *);

extern void fldInitializeSceneGroups(void);




extern s32 func_00206090();

extern void btlRepositionPartyAroundBattleCenter();

extern s32 func_001AC648();

extern void func_001AEEA8();

extern void kwlnFadeBackgroundStartOut();

extern void kwlnDrawSetOverlayTransition();

extern void kwlnDrawEnableD88();

extern void kwlnDrawEnableDc8();

extern void kwlnDrawEnableE08();

extern void kwlnDrawSetupC70B();

extern void kwlnDrawEnableCd0();

extern void kwlnDrawEnableD30();

extern void btlMarkRuntimeUpdatePending();
extern s32 btlAreWorkBuffersReady(void);
extern s32 func_0022E490(void);
extern s32 btlResetSceneSlotFades(void);
extern s32 func_001B3DD8(void);
extern void func_001AD5B0(u16);

extern void kwlnFadeStartIn();

extern s32 fldGetEncounterRuntimeResult();

extern void fldSetEncounterPendingValue();

extern void evtSetSolarOverlayFullyVisible();

extern void btlSyncModelFlagFromEventThresholds();

extern void btlResetTitleStreamOnBattleFlag();

extern void func_001E9410();

extern void btlSpawnBattleWorldAction();

extern void btlCreateRainEffect();

extern s32 btlReleaseScriptResourceA();

extern s32 btlReleaseScriptResource();

extern BtlRuntimeTask *btlCreateSoundUpdateTask(u32);

extern s32 btlCreateSoundReleaseTask();

extern s32 btlCreateWaitUnitListIdleTask();

extern s32 btlCreateApplyToActiveActorsTask();

extern s32 btlCreateFadeStateResetTask();

extern void btlCreateGuidePanelTask();

extern void btlRequestGuidePanelClose();

extern void fldEnableSceneGroupAdvancement(void);

extern s8 D_0037F531[];

extern char *D_004368B0;

void fldCreateSceneCleanupTask(void);

void func_001CFEF8(void) {
}

u32 func_001CFF00(void) {
    return 0;
}

void fldBattleSceneEnterInit(u8 *scene) {
    u32 id = ((BtlState *)scene)->battleMode;

    if (id < 0x400 && (datBattleSceneRecords[id].flags & 0x8000) != 0) {
        ((BtlState *)scene)->commandRestrictFlags |= 8;
        kwlnFadeInStart(0xFF, 0xFF, 0xFF, 0);
    }
    if (func_001B4040() == 0) {
        if (func_001B4210() == 0) {
            ((BtlState *)scene)->encounterKind = 0;
        } else {
            ((BtlState *)scene)->encounterKind = 2;
        }
    } else {
        ((BtlState *)scene)->encounterKind = 3;
    }
    func_0022AF90();
    func_00229728(((BtlState *)scene)->battleMode);
    btlSelectSceneAudioTrack(((BtlState *)scene)->effectLayer, ((BtlState *)scene)->battleMode);
    VU0_STORE_VF($vf0, scene);
}

s32 btlLoadBankWhenTasksIdle(void) {
    if (sndIsStreamStatusTwoOrThree() != 0 &&
        func_0022B108() != 0 &&
        func_0022E460() != 0) {
        sndLoadBattleBank();
        func_0022B288();
        return 3;
    }
    return 0;
}

void fldMarkGridTiles(BtlState *scene) {
    BtlRuntimeTask *tile = (BtlRuntimeTask *)fldCreateSceneTileTask(scene->background.ids.major, scene->background.ids.minor);
    tile->ownerId = 0x8000000000000001ULL;
    btlStartTask(tile);
    tile = (BtlRuntimeTask *)btlCreateFloorLoadTask(scene->background.ids.major, scene->background.ids.minor);
    btlStartTask(tile);
}

s32 fldSceneStateStartTileEffect(BtlState *scene) {
    BattleEffectParams params;
    f32 *origin;
    if (btlCountTasksForOwner(0x8000000000000001LL) == 0) {
        btlResetTitleStreamOnBattleFlag();
        func_001E9410();
        btlSpawnBattleWorldAction();
        btlCreateRainEffect(scene->background.ids.major, scene->background.ids.minor);
        if (scene->commandRestrictFlags & 0x40000) {
            params.rotation[0] = 0.19607843f;
            params.rotation[1] = 0.19607843f;
            params.rotation[2] = 0.19607843f;
            params.scale[0] = 0.19607843f;
            params.scale[1] = 0.19607843f;
            params.scale[2] = 0.19607843f;
            origin = D_0037F770[0];
            params.position[0] = origin[4];
            params.position[1] = origin[5];
            params.position[2] = origin[6];
            btlStartTask(btlCreateEffectTaskWithSourceParams(&params, 0));
        }
        return 4;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001CFEF8", func_001D0140);

s32 fldSceneStateRestoreDisplay(BtlState *scene) {
    if (btlCountTasksForOwner(0x8000000000000002LL) == 0) {
        func_00206090();
        if (!(scene->battleFlags & 0x4000)) {
            btlRepositionPartyAroundBattleCenter();
        }
        func_001AC648();
        if (scene->commandRestrictFlags & 0x80) {
            func_001AEEA8();
            scene->commandRestrictFlags &= ~0x80;
        }
        if (!(scene->battleFlags & 0x4000)) {
            kwlnFadeBackgroundStartOut(0);
            kwlnDrawSetOverlayTransition(0, 0, 1);
            kwlnDrawEnableD88(0);
            kwlnDrawEnableDc8(0);
            kwlnDrawEnableE08(0);
            kwlnDrawSetupC70B(0);
            kwlnDrawEnableCd0(0);
            kwlnDrawEnableD30(0);
            if (!(scene->commandRestrictFlags & 0x100000)) {
                btlMarkRuntimeUpdatePending();
            }
            if (!(scene->commandRestrictFlags & 8)) {
                kwlnFadeStartIn(0);
            }
            if (fldGetEncounterRuntimeResult() != 0) {
                fldSetEncounterPendingValue(1);
            }
            evtSetSolarOverlayFullyVisible();
        }
        if (scene->sceneStatus == 0) {
            fldCreateSceneCleanupTask();
            if (!(scene->commandRestrictFlags & 0x2000)) {
                scene->battleFlags |= 0x100000;
            } else {
                scene->commandRestrictFlags &= ~0x2000;
                scene->battleFlags &= 0xFFEFFFFF;
            }
        }
        btlSyncModelFlagFromEventThresholds();
        return 5;
    }
    return 0;
}

extern u64 btlAdvanceRuntimeSequenceCounter(void);
extern BtlRuntimeTask *btlCreateCommandSoundUpdateTask(void);
extern BtlRuntimeTask *btlCreateSecondaryCommandSoundTask(void);
extern BtlRuntimeTask *btlCreateCommandSoundTask(s32, s32);
extern BtlRuntimeTask *btlCreateModelLoadPollTask(BtlUnit *, u32, u32, s8);
extern BtlRuntimeTask *btlCreateActorTransparencyTask(BtlUnit *);
extern BtlRuntimeTask *btlCreateUnitBaseLightTask(BtlUnit *);
extern BtlRuntimeTask *btlCreateUnitFadeInTask(BtlUnit *, u32, u32);
extern BtlRuntimeTask *func_001E5FF8(BtlUnit *, s32);
extern BtlRuntimeTask *sndCreateActorEffectTask(struct SoundResourceNode *, BtlUnit *, u32);
extern BtlRuntimeTask *btlCreateImmediateCompletionTask(void);
extern BtlRuntimeTask *btlCreateGunLoadPollTask(BtlUnit *);
extern BtlRuntimeTask *sndCreateEarringTask(void);
extern BtlRuntimeTask *btlCreateEffObjB(BtlUnit *, s32);
extern s32 btlIsActorModeActionCodeAllowed(BtlUnit *);
extern s32 btlHasSpecialAbilityOrModelFlag(DatPartyRecord *);

void func_001D08A8(BtlState *scene) {
    BtlUnit *unit;
    BtlUnit *tail;
    BtlRuntimeTask *task;
    BtlRuntimeTask *load;
    BtlRuntimeTask *light;
    BtlRuntimeTask *lastFade = NULL;
    u64 chain;
    u64 firstPrimary;
    s32 kind;
    s32 delay;
    s32 havePrimary;

    chain = btlAdvanceRuntimeSequenceCounter();
    firstPrimary = btlAdvanceRuntimeSequenceCounter();
    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    btlStartTask(btlCreateCommandSoundTask(0, 2));
    if (scene->beginBattleEntryTasks != NULL) {
        chain = scene->beginBattleEntryTasks(0);
    }
    tail = NULL;
    for (unit = scene->units; unit != NULL; unit = unit->nextActor) {
        if ((btlUnitStatusPair(unit) & 0x403) == 0x401) {
            kind = scene->selectEntryModelVariant != NULL ? scene->selectEntryModelVariant(unit) : unit->combatantKind;
            load = btlCreateModelLoadPollTask(unit, unit->unkDC, kind, 0);
            load->startCondition.kind = 4;
            load->startDelay = 1;
            load->startCondition.value.handle = chain;
            btlStartTask(load);
            if (!(scene->commandRestrictFlags & 0xC) && btlIsActorModeActionCodeAllowed(unit)) {
                task = btlCreateActorTransparencyTask(unit);
                task->startCondition.kind = 4;
                task->startDelay = 1;
                task->startCondition.value.handle = load->handle;
                btlStartTask(task);
            }
            light = btlCreateUnitBaseLightTask(unit);
            light->startCondition.kind = 4;
            light->startCondition.value.handle = load->handle;
            light->ownerId = 0x8000000000000003ULL;
            btlStartTask(light);
            chain = light->handle;
        }
        tail = unit;
    }
    delay = 1;
    for (unit = tail; unit != NULL; unit = unit->previousActor) {
        if ((btlUnitStatusPair(unit) & 0x403) == 0x401) {
            if (!(unit->stateFlags & 0x800) && !(scene->commandRestrictFlags & 0x100008)) {
                if (!(scene->commandRestrictFlags & 4) && btlIsActorModeActionCodeAllowed(unit)) {
                    lastFade = func_001E5FF8(unit, 8);
                } else {
                    lastFade = btlCreateUnitFadeInTask(unit, 4, 6);
                }
                lastFade->startCondition.kind = 4;
                lastFade->startCondition.value.handle = chain;
                lastFade->startDelay = delay;
                lastFade->ownerId = 0x8000000000000003ULL;
                btlStartTask(lastFade);
                if (scene->commandRestrictFlags & 4) {
                    task = sndCreateActorEffectTask(scene->resources[46], unit, 0x10);
                    task->startCondition.kind = 4;
                    task->startCondition.value.handle = chain;
                    task->startDelay = delay;
                    task->ownerId = 0x8000000000000003ULL;
                    btlStartTask(task);
                }
                if (!(scene->commandRestrictFlags & 4)) {
                    delay += 8;
                } else {
                    delay += 10;
                }
            } else {
                lastFade = func_001E5FF8(unit, 0);
                lastFade->startCondition.kind = 4;
                lastFade->startCondition.value.handle = chain;
                lastFade->startDelay = delay;
                lastFade->ownerId = 0x8000000000000003ULL;
                btlStartTask(lastFade);
            }
        }
    }
    if (scene->finishEnemyEntryTasks != NULL) {
        scene->finishEnemyEntryTasks(chain);
    }
    if (scene->commandRestrictFlags & 0x100000) {
        task = btlCreateImmediateCompletionTask();
        task->startCondition.kind = 4;
        task->startCondition.value.handle = chain;
        task->ownerId = 0x8000000000000004ULL;
        btlStartTask(task);
    }
    havePrimary = 0;
    for (unit = tail; unit != NULL; unit = unit->previousActor) {
        if ((btlUnitStatusPair(unit) & 0x203) == 0x201) {
            if ((btlUnitStatusPair(unit) & 0x0010000000001000ULL) == 0x1000) {
                load = btlCreateModelLoadPollTask(unit, unit->unkDC, unit->combatantKind, 0);
            } else {
                load = btlCreateModelLoadPollTask(unit, unit->modelId, unit->modelVariant, 0);
            }
            load->startCondition.kind = 4;
            load->startDelay = 1;
            load->startCondition.value.handle = chain;
            btlStartTask(load);
            light = btlCreateUnitBaseLightTask(unit);
            light->startCondition.kind = 4;
            light->startCondition.value.handle = load->handle;
            light->ownerId = 0x8000000000000003ULL;
            btlStartTask(light);
            chain = light->handle;
            if (!havePrimary) {
                firstPrimary = chain;
                havePrimary = 1;
            }
            if (!(unit->stateFlags & 0x800) && !(scene->commandRestrictFlags & 8)) {
                lastFade = btlCreateUnitFadeInTask(unit, 4, 6);
                lastFade->startCondition.kind = 4;
                lastFade->startDelay = 1;
                lastFade->startCondition.value.handle = light->handle;
                lastFade->ownerId = 0x8000000000000003ULL;
                btlStartTask(lastFade);
                if ((scene->commandRestrictFlags & 2) && !btlHasSpecialAbilityOrModelFlag(&unit->partyRecord)) {
                    task = sndCreateActorEffectTask(scene->resources[46], unit, 0x10);
                    task->startCondition.kind = 4;
                    task->startDelay = 1;
                    task->startCondition.value.handle = light->handle;
                    btlStartTask(task);
                }
            } else {
                lastFade = func_001E5FF8(unit, 0);
                lastFade->startCondition.kind = 4;
                lastFade->startDelay = 1;
                lastFade->startCondition.value.handle = light->handle;
                lastFade->ownerId = 0x8000000000000003ULL;
                btlStartTask(lastFade);
            }
        }
    }
    for (unit = tail; unit != NULL; unit = unit->previousActor) {
        if ((btlUnitStatusPair(unit) & 0x203) == 0x201) {
            task = btlCreateGunLoadPollTask(unit);
            task->startCondition.kind = 4;
            task->startCondition.value.handle = chain;
            btlStartTask(task);
        }
    }
    if (havePrimary) {
        task = sndCreateEarringTask();
        task->startCondition.kind = 4;
        task->startCondition.value.handle = firstPrimary;
        task->ownerId = 0x8000000000000003ULL;
        btlStartTask(task);
    }
    switch (scene->encounterKind) {
        case 1:
            task = btlCreateEffObjB(NULL, 0xD);
            task->startCondition.kind = 4;
            task->startCondition.value.handle = chain;
            task->ownerId = 0x8000000000000003ULL;
            btlStartTask(task);
            break;
        case 2:
            task = btlCreateEffObjB(NULL, 0xAB);
            task->startCondition.kind = 4;
            task->startCondition.value.handle = chain;
            task->ownerId = 0x8000000000000003ULL;
            btlStartTask(task);
            break;
        case 3:
            task = btlCreateEffObjB(NULL, 0xDD);
            task->startCondition.kind = 4;
            task->startCondition.value.handle = chain;
            task->ownerId = 0x8000000000000003ULL;
            btlStartTask(task);
            break;
    }
    if (scene->battleFlags & 0x4000) {
        task = btlCreateEffObjB(NULL, 0x88);
        task->startCondition.kind = 4;
        task->startCondition.value.handle = chain;
        task->ownerId = 0x8000000000000003ULL;
        btlStartTask(task);
    }
    if (lastFade != NULL) {
        task = btlCreateImmediateCompletionTask();
        task->startCondition.kind = 4;
        task->startDelay = 6;
        task->startCondition.value.handle = lastFade->handle;
        task->ownerId = 0x8000000000000003ULL;
        btlStartTask(task);
    }
    if (!(scene->commandRestrictFlags & 0x40000)) {
        task = btlCreateSoundUpdateTask(12);
        task->startCondition.kind = 7;
        task->startCondition.value.owner = 0x8000000000000003ULL;
        btlStartTask(task);
    }
}


s32 btlInitializeSceneAfterTasksAndBuffersReady(BtlState *scene) {
    u32 i;

    if (scene->commandRestrictFlags & 0x100000) {
        if (btlCountTasksForOwner(0x8000000000000004LL) == 0) {
            btlMarkRuntimeUpdatePending();
            scene->commandRestrictFlags &= ~0x100000;
        }
    }
    if (btlCountTasksForOwner(0x8000000000000003LL) == 0 &&
        btlAreWorkBuffersReady() != 0 &&
        func_0022E490() != 0) {
        fldInitializeSceneGroups();
        btlResetSceneSlotFades();
        scene->eventReady = 0;
        func_001B3DD8();
        if (datBattleSceneRecords[scene->battleMode].unk01 != 0) {
            for (i = 0; i < datBattleSceneRecords[scene->battleMode].unk02; i++) {
                func_001AD5B0(datBattleSceneRecords[scene->battleMode].unk01);
            }
        }
        return 8;
    }
    return 0;
}

void btlConsumeSceneAdvanceFlags(void) {
}

s32 fldConsumeSceneInputFlags(BtlState *scene) {
    u32 flags = scene->battleFlags;
    s32 result;
    if ((flags & 0x800) != 0) {
        fldClearSceneAdvanceFlag();
        result = 8;
    } else if ((flags & 0x400) != 0) {
        fldClearSceneAdvanceFlag();
        result = 7;
    } else {
        return 0;
    }
    scene->battleFlags |= 0x20;
    return result;
}

void fldAdvanceSceneGroupInitialization(BtlState *scene) {
    s32 notFirst = scene->mode != 1;
    scene->mode = 2 - notFirst;
    if (scene->sceneCallback != 0) {
        s32 variant = scene->sceneCallback();
        if (variant != -1) {
            scene->mode = variant;
        }
    }
    scene->groupHandleCount = 0;
    scene->turnCount = scene->turnCount + 1;
    fldInitializeSceneGroups();
    btlResetSceneSlotFades();
}

extern void btlTickActorEntryCountdowns(u8 *);
extern void btlResetBattleHistoryCounters(void);
extern void btlClearActorSelectedEntryIndex(BtlUnit *);
extern void btlRefreshUnitMotionSelection(BtlUnit *);
extern BtlRuntimeTask *btlCreateEffObjC(BtlUnit *, s32);

/* Advance the scene after its bound actor tasks finish their preparation. */
extern s32 btlAdvanceSceneWhenActorTasksReady(BtlState *);
INCLUDE_ASM(const s32, "game/code_001CFEF8", btlAdvanceSceneWhenActorTasksReady);

extern void fldClearSceneAdvanceFlag(void);
extern s32 btlCanStartPrimaryScriptTask(void);
extern s32 btlHasScriptResource(void);
extern void btlStartPrimaryScriptTask(void);
extern void btlStartSecondaryScriptTask(void);
extern void btlStartSkillEventTask(s32);
extern void evtBeginSolarOverlayFadeOut(s32);
extern s32 fldGetActiveSceneGroupValue(void);
extern u32 kwlnDrawControlFlags;

void btlChooseAndStartSceneScript(BtlState *scene) {
    scene->scriptState = -1;
    fldClearSceneAdvanceFlag();
    scene->battleFlags |= 0x20;
    if (scene->selectScriptState != NULL) {
        scene->scriptState = scene->selectScriptState();
    }
    if (scene->scriptState == -1) {
        if (btlCanStartPrimaryScriptTask() != 0) {
            scene->scriptState = 0xF000002;
        } else if (btlHasScriptResource() != 0) {
            scene->scriptState = 0xF000003;
        } else if (scene->selectScriptArg != NULL) {
            s32 arg = scene->selectScriptArg();

            if (arg >= 0) {
                scene->scriptArg = arg;
                scene->scriptState = 0xF000000;
            }
        }
    }
    switch (scene->scriptState) {
    case 0xF000002:
        if (!(scene->commandRestrictFlags & 0x4000)) {
            btlStartTask(btlCreateCommandSoundTask(0, 3));
        }
        btlStartPrimaryScriptTask();
        return;
    case 0xF000003:
        evtBeginSolarOverlayFadeOut(8);
        btlStartSecondaryScriptTask();
        kwlnDrawControlFlags &= 0xDFFFFFFF;
        return;
    case 0xF000000: {
        ActionStateLink *task = (ActionStateLink *)fldGetActiveSceneGroupValue();

        if (task != NULL) {
            btlStartTask(btlCreateCommandSoundUpdateTask());
            btlStartTask(btlCreateSecondaryCommandSoundTask());
            if (task->unit->flags & FLD_SCENE_ACTOR_PRIMARY_BIT) {
                btlStartTask(btlCreateCommandSoundTask((s32)task, 9));
            } else {
                btlStartTask(btlCreateCommandSoundTask((s32)task, 3));
            }
        }
        func_001C7DB8(0, 20);
        return;
    }
    default:
        if (scene->scriptState != -1) {
            if (!(scene->commandRestrictFlags & 0x4000)) {
                btlStartTask(btlCreateCommandSoundUpdateTask());
                btlStartTask(btlCreateCommandSoundTask(0, 3));
            }
            btlStartSkillEventTask(scene->scriptState);
        }
        break;
    }
}

s32 fldSceneStateWaitScriptRelease(BtlState *scene) {
    s32 finished = 1;
    s32 state = scene->scriptState;

    switch (state) {
    case 0xF000002:
        if (btlReleaseScriptResourceA() == 0) {
            finished = 0;
        } else if (!(scene->commandRestrictFlags & 0x40000)) {
            btlStartTask(btlCreateSoundUpdateTask(0xC));
            btlStartTask(btlCreateSoundReleaseTask(0xC));
            btlStartTask(btlCreateWaitUnitListIdleTask(0xC));
            btlStartTask(btlCreateApplyToActiveActorsTask(0xC));
            btlStartTask(btlCreateFadeStateResetTask());
            btlStartTask(btlCreateSecondaryCommandSoundTask());
        }
        break;
    case 0xF000000:
        finished = 0;
        if (scene->frame == 0x14) {
            btlCreateGuidePanelTask(scene->scriptTarget, scene->scriptArg);
        } else if (scene->frame >= 0x2D) {
            if (btlHasRegisteredGuidePanelTask() != 0) {
                if (D_0037F531[0] < 0) {
                    btlRequestGuidePanelClose();
                    func_001C7DB8(1, 0x14);
                }
            } else {
                finished = 1;
            }
        }
        break;
    case 0xF000003:
        break;
    default:
        if (state != -1) {
            finished = btlReleaseScriptResource() != 0;
        }
        break;
    }

    if (finished != 0) {
        if ((scene->battleFlags & 0x800) == 0) {
            fldEnableSceneGroupAdvancement();
            scene->battleFlags &= ~0x400;
            scene->battleFlags &= ~0x1000;
            scene->battleFlags &= ~0x20;
            return 6;
        }
        return 9;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001CFEF8", func_001D18D8);

INCLUDE_ASM(const s32, "game/code_001CFEF8", func_001D22D8);

extern void itfMesClearFlags();

extern void brsTaskAllowUpdate();

extern void func_001AA868();

extern s32 func_0029D000();

extern void datAdjustCurrentHp();

/* Nudge every active party entry's cursor by weight * 0.05 once scene flag
 * 0x40 is clear. */
void btlApplyPartyEntryWeightedDelta(BtlState *scene) {
    u32 i;
    f32 scale;
    i = 0;
    itfMesClearFlags(1);
    scale = 0.05f;
    brsTaskAllowUpdate();
    evtBeginSolarOverlayFadeOut(8);
    do {
        func_001AA868(&datGameState->party[i], -0x45D1);
        if (!(scene->commandRestrictFlags & 0x40)) {
            if (datGameState->party[i].flags & 2) {
                if (scene->eventReady == 1) {
                    if (datGameState->party[i].huntExp != 0 || scene->unk2F8 != 0) {
                        if (!(datGameState->party[i].status & 0x40)) {
                            if (func_0029D000(&datGameState->party[i]) == 0) {
                                datAdjustCurrentHp(&datGameState->party[i], (s32)((f32)datGameState->party[i].maxHp * scale));
                            }
                        }
                    }
                }
            }
        }
        i++;
    } while (i < 5);
    scene->loadStep = 0;
}

extern s32 func_002998D8();

extern s32 fldLoadAreaResource();

extern void btlReleaseEventAssets();

extern void btlReleaseBossData();

extern void fldPollAreaResourceLoad();

extern s32 fldGetResourceReadyFlag();

extern s32 brsTaskPollDone();

/* Field-load step: while loading, sends every linked scene node to state
 * 0x1F, otherwise advances loadStep (0 start, 1 polling, 2 done). Returns
 * 0xB while a load is still pending. */
u32 fldStepAreaLoad(BtlState *work) {
    ActionStateLink *node;
    s32 step;
    if (func_002998D8() == 1) {
        work->commandRestrictFlags |= 0x20;
        if (work->tasks != 0) {
            for (node = work->tasks; node != 0; node = node->next) {
                if (node->state != 0x1F) {
                    btlDispatchStateHandler(node, 0x1F);
                }
            }
        } else {
            switch (work->loadStep) {
            case 0:
                work->battleFlags &= ~0x10;
                if (fldLoadAreaResource() != 0) {
                    work->loadStep = 1;
                } else {
                    work->loadStep = 2;
                }
                btlReleaseEventAssets();
                btlReleaseBossData();
                break;
            case 1:
                fldPollAreaResourceLoad();
                if (fldGetResourceReadyFlag() == 0) {
                    work->loadStep = 2;
                    btlBossDebugPrintf("btl:field loding end\n");
                }
                break;
            case 2:
                break;
            }
        }
    }
    step = work->loadStep;
    if (step < 3) {
        if (step > 0) {
            if (brsTaskPollDone() == 0) {
                return 0xB;
            }
        }
    }
    return 0;
}

void fldMarkLinkedSceneActors(u8 *scene) {
    u8 *node;
    btlAdvanceTitleStateWithAudioCleanup(scene);
    node = (u8 *)((BtlState *)scene)->tasks;
    while (node != 0) {
        if (((ActionStateLink *)node)->state != 0x1F) {
            btlDispatchStateHandler(node, 0x1F);
        }
        node = (u8 *)((ActionStateLink *)node)->next;
    }
    btlFlagTasksForUpdate();
}

extern void ptyApplySkillRecovery(DatPartyRecord *entry, u32 skillId);
extern void dds3WorkClear(void);
extern s32 btlCountRegisteredTasks(void);

/* Restore the party after all scene tasks finish, then clear battle activity. */
s32 func_001D2A78(BtlState *scene) {
    u32 i;
    if (scene->tasks != NULL) {
        return 0;
    }
    for (i = 0; i < 5; i++) {
        if (datGameState->party[i].flags & 1) {
            if (!(scene->commandRestrictFlags & 0x40)) {
                if ((datGameState->party[i].flags & 2) &&
                    (datGameState->party[i].status & 0x7FFF) != 0x4000 &&
                    datGameState->party[i].hp != 0) {
                    ptyApplySkillRecovery(&datGameState->party[i], 0x24A);
                    ptyApplySkillRecovery(&datGameState->party[i], 0x24B);
                }
                if ((datGameState->party[i].status & 0x4000) ||
                    datGameState->party[i].hp == 0) {
                    datGameState->party[i].status &= ~0x4000;
                    datGameState->party[i].hp = 1;
                }
            }
        }
    }
    dds3WorkClear();
    if (!(scene->commandRestrictFlags & 0x40)) {
        for (i = 0; i < 5; i++) {
            func_001AA868(&datGameState->party[i], -0x5D1);
        }
    }
    if (btlCountRegisteredTasks() == 0) scene->battleFlags &= ~1;
    return 0;
}


void fldMarkSceneRefresh(BtlState *scene) {
    func_00230960((s32)scene);
    scene->unk220 |= 4;
}

u32 fldBeginFadeWhenSceneReady(void) {
    if (func_00230978() != 0) {
        kwlnFadeInStart(0, 0, 0, 0);
        return 2;
    }
    return 0;
}

/* Switch immediately, reset the frame/state, and call the unchecked initializer. */
void btlSetScene(s32 sceneId) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    scene->currentScene = sceneId;
    scene->frame = 0;
    scene->sceneState = 0;
    D_003B6938[sceneId].initialize((s32)scene);
}

/* Queue a transition for the next scene update. */
void btlQueueScene(u32 sceneId) {
    BtlState *scene;

    scene = (BtlState *)btlGetRuntime();
    scene->queuedScene = sceneId;
}

/* Consume a queued transition, run the current updater, and advance its frame. */
void btlUpdateScene(void) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    SceneInitializer *sceneInitializer;
    s32 updateResult;
    if (scene->queuedScene != 0) {
        btlSetScene(scene->queuedScene);
        scene->queuedScene = 0;
    }
    sceneInitializer = &D_003B6938[scene->currentScene];
    updateResult = sceneInitializer->update((s32)scene);
    if (updateResult != 0) {
        btlQueueScene(updateResult);
    }
    scene->frame++;
}

/* Enter the initial scene and discard any queued transition. */
void btlResetToInitialScene(void) {
    BtlState *scene;

    scene = (BtlState *)btlGetRuntime();
    btlSetScene(FLD_SCENE_INITIAL_ID);
    scene->queuedScene = 0;
}

void btlGetCurrentSceneRecordValue(void) {
}

/* Return the current initializer row's flags word. */
s32 fldGetSceneDescriptorProperty(void) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    s32 sceneId = scene->currentScene;
    return D_003B6938[sceneId].flags;
}

/* Select the task's group array; the handle-group bit takes precedence. */
ActionStateLink **fldGetActorSceneGroupResource(ActionStateLink *task) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    ActionStateLink **groupEntries = scene->groupSecondary;
    u32 actorGroupBits;
    if ((task->pendingFlags & FLD_SCENE_TASK_HANDLE_GROUP_BIT) != 0) {
        return scene->groupHandles;
    }
    actorGroupBits = task->unit->flags & FLD_SCENE_ACTOR_GROUP_MASK;
    switch (actorGroupBits) {
    case FLD_SCENE_ACTOR_PRIMARY_BIT:
        groupEntries = scene->groupPrimary;
        break;
    case FLD_SCENE_ACTOR_SECONDARY_BIT:
        break;
    case FLD_SCENE_ACTOR_TERTIARY_BIT:
        groupEntries = scene->groupTertiary;
        break;
    default:
        groupEntries = 0;
        break;
    }
    return groupEntries;
}

/* Resolve one of the three group IDs; all other IDs return a null address. */
ActionStateLink **fldGetSceneGroupResource(u8 groupId) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    ActionStateLink **groupEntries;
    switch (groupId) {
    case FLD_SCENE_GROUP_PRIMARY_ID:
        groupEntries = scene->groupPrimary;
        break;
    case FLD_SCENE_GROUP_SECONDARY_ID:
        groupEntries = scene->groupSecondary;
        break;
    case FLD_SCENE_GROUP_TERTIARY_ID:
        groupEntries = scene->groupTertiary;
        break;
    default:
        groupEntries = 0;
        break;
    }
    return groupEntries;
}

/* Return the selected group's capacity, not its ID; keep the native runtime call. */
s32 fldClassifyActorSceneGroup(ActionStateLink *task) {
    u32 actorGroupBits;
    btlGetRuntime();
    if (task->pendingFlags & FLD_SCENE_TASK_HANDLE_GROUP_BIT) {
        return FLD_SCENE_HANDLE_TASK_COUNT;
    }
    actorGroupBits = task->unit->flags & FLD_SCENE_ACTOR_GROUP_MASK;
    switch (actorGroupBits) {
    case FLD_SCENE_ACTOR_PRIMARY_BIT: return FLD_SCENE_PRIMARY_TASK_COUNT;
    case FLD_SCENE_ACTOR_SECONDARY_BIT: return FLD_SCENE_SECONDARY_TASK_COUNT;
    case FLD_SCENE_ACTOR_TERTIARY_BIT: return FLD_SCENE_TERTIARY_TASK_COUNT;
    default: return 0;
    }
}

extern u32 func_001AB8D8();

/* Sort adjacent occupied tasks by descending accessor value; null slots stay put. */
void btlSortSceneGroupByPriorityDesc(ActionStateLink **group, s32 entryCount) {
    s32 swapped;
    do {
        ActionStateLink **pairCursor = group;
        u32 pairIndex = 0;
        swapped = 0;
        for (; pairIndex < entryCount - 1; pairIndex++, pairCursor++) {
            ActionStateLink *first = pairCursor[0];
            ActionStateLink *second = pairCursor[1];
            if (first != 0 && second != 0 &&
                func_001AB8D8((s32)&first->unit->partyRecord.flags, 3) < func_001AB8D8((s32)&second->unit->partyRecord.flags, 3)) {
                pairCursor[0] = second;
                swapped = 1;
                pairCursor[1] = first;
            }
        }
    } while (swapped != 0);
}

/* Sort adjacent occupied tasks by ascending actor byte priority; null slots stay put. */
void fldSortGroupByPriority(ActionStateLink **group, s32 entryCount) {
    s32 swapped;
    do {
        ActionStateLink **pairCursor = group;
        u32 pairIndex = 0;
        swapped = 0;
        for (; pairIndex < entryCount - 1; pairIndex++, pairCursor++) {
            ActionStateLink *first = pairCursor[0];
            ActionStateLink *second = pairCursor[1];
            if (first != 0 && second != 0 &&
                first->unit->lookupId > second->unit->lookupId) {
                pairCursor[0] = second;
                swapped = 1;
                pairCursor[1] = first;
            }
        }
    } while (swapped != 0);
}

/* Map an exact actor-group bit pattern to its group ID; combined bits return zero. */
s32 fldGetSceneGroupIndexByActorFlags(u8 *task) {
    u32 actorGroupBits = ((ActionStateLink *)task)->unit->flags & FLD_SCENE_ACTOR_GROUP_MASK;
    s32 groupId;
    switch (actorGroupBits) {
    case FLD_SCENE_ACTOR_PRIMARY_BIT:
        groupId = FLD_SCENE_GROUP_PRIMARY_ID;
        break;
    case FLD_SCENE_ACTOR_SECONDARY_BIT:
        groupId = FLD_SCENE_GROUP_SECONDARY_ID;
        break;
    case FLD_SCENE_ACTOR_TERTIARY_BIT:
        groupId = FLD_SCENE_GROUP_TERTIARY_ID;
        break;
    default:
        groupId = 0;
        break;
    }
    return groupId;
}

INCLUDE_ASM(const s32, "game/code_001CFEF8", func_001D30E0);

/* Pop the front slot only when its remaining counter is zero. */
void fldCompactSceneSlots(void) {
    BtlSceneSlot *slotCursor = ((BtlState *)btlGetRuntime())->slots;
    u32 slotIndex;
    if (slotCursor->remaining == 0) {
        for (slotIndex = 0; slotIndex < FLD_SCENE_SLOT_SHIFT_COUNT; slotIndex++) {
            slotCursor[0].group = slotCursor[1].group;
            slotCursor[0].remaining = slotCursor[1].remaining;
            slotCursor[0].id = slotCursor[1].id;
            slotCursor++;
        }
        slotCursor->group = 0;
        slotCursor->remaining = 0;
        slotCursor->id = 0;
    }
}

/* Mode 1 drains front counters; modes 2/3 spend full counters in steps of 50. */
void fldConsumeSceneSlotCounters(s32 amount, u8 mode) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    u8 headGroup;
    u32 i;
    if (scene->battleFlags & 0x100) {
        headGroup = scene->slots[0].group;
        switch (mode) {
        case 0:
            break;
        case 1:
            if (amount < 100) {
                if (amount < scene->slots[0].remaining) {
                    scene->slots[0].remaining -= amount;
                } else {
                    scene->slots[0].remaining = 0;
                    fldCompactSceneSlots();
                }
            } else {
                while (amount > 0) {
                    scene->slots[0].remaining = 0;
                    fldCompactSceneSlots();
                    if (scene->slots[0].group != headGroup) {
                        return;
                    }
                    if (scene->slots[0].remaining == 0) {
                        break;
                    }
                    amount -= 100;
                }
            }
            break;
        case 2:
        case 3:
            while (amount > 0) {
                for (i = 0; i < 8; i++) {
                    if (scene->slots[i].group == headGroup && scene->slots[i].remaining == 100) {
                        break;
                    }
                }
                if (i < 8) {
                    scene->slots[i].remaining -= 50;
                } else {
                    scene->slots[0].remaining = 0;
                    fldCompactSceneSlots();
                }
                amount -= 50;
            }
            break;
        }
    }
}

/* Insert half-counter slots for the current variant, clamped to eight entries. */
void fldInsertSceneSlots(s32 slotCount) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    s32 groupId;
    s32 usedSlotCount;
    s32 slotIndex;

    groupId = scene->mode == FLD_SCENE_GROUP_PRIMARY_ID ? FLD_SCENE_GROUP_PRIMARY_ID : FLD_SCENE_GROUP_SECONDARY_ID;
    usedSlotCount = fldCountSceneSlots();
    if (usedSlotCount + slotCount >= FLD_SCENE_SLOT_COUNT) {
        slotCount = FLD_SCENE_SLOT_COUNT - usedSlotCount;
    }
    if (slotCount <= 0) {
        return;
    }
    for (slotIndex = usedSlotCount + slotCount - 1; slotIndex != slotCount - 1; slotIndex--) {
        scene->slots[slotIndex].group = scene->slots[slotIndex - slotCount].group;
        scene->slots[slotIndex].remaining = scene->slots[slotIndex - slotCount].remaining;
        scene->slots[slotIndex].id = slotIndex + 1;
    }
    for (; slotIndex != -1; slotIndex--) {
        scene->slots[slotIndex].group = groupId;
        scene->slots[slotIndex].remaining = FLD_SCENE_SLOT_HALF_COUNTER;
        scene->slots[slotIndex].id = slotIndex + 1;
    }
    fldInitSceneFadeRecords();
}

/* Spend amount first; swap front groups only if the original counter survives. */
void fldSwapSceneSlots(s32 counterAmount) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    if (scene->battleFlags & FLD_SCENE_COUNTERS_ENABLED_BIT) {
        u8 firstGroupId = scene->slots[0].group;
        u8 remainingBeforeSpend = scene->slots[0].remaining;
        fldConsumeSceneSlotCounters(counterAmount, FLD_SCENE_COUNTER_FRONT_MODE);
        if (counterAmount < remainingBeforeSpend) {
            if (scene->slots[1].group != 0 && scene->slots[1].group != firstGroupId) {
                u8 savedGroupId = scene->slots[0].group;
                u8 savedRemaining = scene->slots[0].remaining;
                u8 savedSlotId = scene->slots[0].id;
                scene->slots[0].group = scene->slots[1].group;
                scene->slots[0].remaining = scene->slots[1].remaining;
                scene->slots[0].id = scene->slots[1].id;
                scene->slots[1].group = savedGroupId;
                scene->slots[1].remaining = savedRemaining;
                scene->slots[1].id = savedSlotId;
            }
        }
    }
}

/* Count slots with both group and counter; zero-counter groups can still block finish. */
s32 fldCountSceneSlots(void) {
    BtlSceneSlot *slotCursor = ((BtlState *)btlGetRuntime())->slots;
    s32 occupiedSlotCount = 0;
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < FLD_SCENE_SLOT_COUNT; slotIndex++, slotCursor++) {
        if (slotCursor->group != 0 && slotCursor->remaining != 0) {
            occupiedSlotCount++;
        }
    }
    return occupiedSlotCount;
}

/* Finished when no group remains, or the explicit finish flag is set. */
s32 fldAreSceneSlotsFinished(void) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    BtlSceneSlot *slotCursor;
    u32 slotIndex;
    if (scene->battleFlags & FLD_SCENE_SLOTS_FINISHED_BIT) {
        return 1;
    }
    slotCursor = scene->slots;
    for (slotIndex = 0; slotIndex < FLD_SCENE_SLOT_COUNT; slotIndex++) {
        if (slotCursor->group != 0) {
            return 0;
        }
        slotCursor++;
    }
    return 1;
}

/* Sort the three bounded task groups before the native slot initialization call. */
void fldInitializeSceneGroups(void) {
    BtlState *scene;

    scene = (BtlState *)btlGetRuntime();
    fldSortGroupByPriority(scene->groupPrimary, FLD_SCENE_PRIMARY_TASK_COUNT);
    btlSortSceneGroupByPriorityDesc(scene->groupSecondary, FLD_SCENE_SECONDARY_TASK_COUNT);
    btlSortSceneGroupByPriorityDesc(scene->groupTertiary, FLD_SCENE_TERTIARY_TASK_COUNT);
    func_001D30E0();
}

/* Move a member behind its occupied successors; native membership/bounds are unchecked. */
void btlMoveTaskToGroupTail(ActionStateLink *task) {
    ActionStateLink **groupCursor = fldGetActorSceneGroupResource(task);
    u32 groupCapacity = fldClassifyActorSceneGroup(task);
    u32 lastIndex;
    u32 entryIndex;
    for (entryIndex = 0; entryIndex < groupCapacity; entryIndex++, groupCursor++) {
        if (*groupCursor == task) {
            break;
        }
    }
    lastIndex = groupCapacity - 1;
    for (; entryIndex < lastIndex && groupCursor[1] != 0; entryIndex++, groupCursor++) {
        *groupCursor = groupCursor[1];
    }
    *groupCursor = task;
}

/* Sort, then rotate at most one primary-group capacity to put a bound task first. */
void btlRotateGroupUntilTaskFirst(ActionStateLink *task) {
    BtlState *scene;
    u32 rotationCount;
    if (task != 0 && (task->pendingFlags & FLD_SCENE_TASK_BOUND_BIT) != 0 && task->unit != 0 && !(task->pendingFlags & FLD_SCENE_TASK_HANDLE_GROUP_BIT) &&
        (task->unit->flags & FLD_SCENE_ACTOR_PRIMARY_BIT) != 0) {
        rotationCount = 0;
        scene = (BtlState *)btlGetRuntime();
        fldSortGroupByPriority(scene->groupPrimary, FLD_SCENE_PRIMARY_TASK_COUNT);
        for (; rotationCount < FLD_SCENE_PRIMARY_TASK_COUNT && scene->groupPrimary[0] != task; rotationCount++) {
            btlMoveTaskToGroupTail(scene->groupPrimary[0]);
        }
    }
}

/* Append at the first empty entry, then restore the former head's ordering.
 * Retain the native capacity query and unchecked append scan. */
void fldAppendTaskToGroup(ActionStateLink *task) {
    ActionStateLink **groupCursor = fldGetActorSceneGroupResource(task);
    ActionStateLink *previousHead;
    fldClassifyActorSceneGroup(task);
    previousHead = *groupCursor;
    while (*groupCursor != 0) {
        groupCursor++;
    }
    *groupCursor = task;
    btlRotateGroupUntilTaskFirst(previousHead);
}

/* Append a handle to the first zero entry; the caller must leave room. */
void fldAppendSceneGroupHandle(ActionStateLink *handle) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    ActionStateLink **handleCursor = scene->groupHandles;
    while (*handleCursor != 0) {
        handleCursor++;
    }
    *handleCursor = handle;
}

void fldUpdateSceneGroupTask(ActionStateLink *task) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    BtlUnit *actor;
    s32 kind;

    if ((task->pendingFlags & 0x40) == 0) {
        actor = task->unit;
        kind = (actor->flags & 0x200) != 0 ? 1 : 2;
        scene->groupHandleCount++;
        if (kind == 1) {
            scene->activeGroupCount++;
        }
        if (scene->currentTask == task && scene->mode == kind) {
            scene->battleFlags |= 8;
        }
        if ((task->combinedFlags & 0x400000100LL) == 0) {
            if (btlDoesEnabledStatusMatchCurrentId(&actor->partyRecord, 0xDE) != 0) {
                task->flags |= 4;
            }
        } else {
            task->flags &= ~4;
        }
        if ((scene->battleFlags & 0x40) != 0 && (task->flags & 4) == 0) {
            btlMoveTaskToGroupTail(task);
        }
    } else {
        btlRemoveTaskFromSceneGroup(task);
        scene->battleFlags |= 8;
        task->pendingFlags &= ~0x40;
    }
}

/* Clear the matching member and move that hole to the last group entry. */
void btlRemoveTaskFromSceneGroup(ActionStateLink *task) {
    ActionStateLink **groupEntries = fldGetActorSceneGroupResource(task);
    u32 groupCapacity = fldClassifyActorSceneGroup(task);
    u32 entryIndex = 0;
    u32 lastIndex;
    for (; entryIndex < groupCapacity; entryIndex++) {
        if (groupEntries[entryIndex] == task) {
            groupEntries[entryIndex] = 0;
            break;
        }
    }
    lastIndex = groupCapacity - 1;
    for (; entryIndex < lastIndex; entryIndex++) {
        ActionStateLink *current = groupEntries[entryIndex];
        ActionStateLink *next = groupEntries[entryIndex + 1];
        groupEntries[entryIndex + 1] = current;
        groupEntries[entryIndex] = next;
    }
}

/* Set the native advance/update bit pair without changing other scene flags. */
void fldEnableSceneGroupAdvancement(void) {
    BtlState *scene;

    scene = (BtlState *)btlGetRuntime();
    scene->battleFlags = scene->battleFlags | FLD_SCENE_ADVANCE_ENABLE_BITS;
}

/* Clear only the advancement bit, retaining the unsigned native mask. */
void fldClearSceneAdvanceFlag(void) {
    BtlState *scene;

    scene = (BtlState *)btlGetRuntime();
    scene->battleFlags = scene->battleFlags & FLD_SCENE_ADVANCE_CLEAR_MASK;
}

/* Prefer the special handle queue; otherwise return the front group's first task. */
s32 fldGetActiveSceneGroupValue(void) {
    u8 *scene = (u8 *)btlGetRuntime();
    s32 headValue = (s32)((BtlState *)scene)->groupHandles[0];
    if (headValue != 0) {
        return headValue;
    }
    return *(s32 *)fldGetSceneGroupResource(((BtlState *)scene)->slots[0].group);
}

/* Read one task word from the front group; an empty front slot returns zero. */
s32 fldGetSceneGroupEntry(s32 entryIndex) {
    u8 *scene = (u8 *)btlGetRuntime();
    if (((BtlState *)scene)->slots[0].group == 0) {
        return 0;
    }
    return ((s32 *)fldGetSceneGroupResource(((BtlState *)scene)->slots[0].group))[entryIndex];
}

extern s32 func_001B2630();

/* Per-frame scene advance: picks the next group task (or finishes the
 * slot sequence) and dispatches it once flags 4 and 8 are set. */
void func_001D3ED8(void) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    ActionStateLink *task;
    void (*hook)(void);
    u32 pendingFlags;
    if (scene->battleFlags & 4) {
        if (scene->battleFlags & 8) {
            if (scene->battleFlags & 0x800) {
                return;
            }
            if (scene->battleFlags & 0x400) {
                return;
            }
            if (func_001B2630() != 0) {
                scene->battleFlags |= 0x800;
                return;
            }
            hook = scene->completionHook;
            if (hook != 0) {
                hook();
            }
            task = scene->groupHandles[0];
            if (task != 0) {
                if (task->state != 2) {
                    return;
                }
                pendingFlags = task->pendingFlags;
                scene->currentTask = task;
                task->pendingFlags = pendingFlags | 0x40;
                btlDispatchStateHandler(task, 3);
                scene->battleFlags &= ~8;
            } else {
                if (fldAreSceneSlotsFinished() != 0) {
                    scene->battleFlags |= 0x400;
                    return;
                }
                task = *fldGetSceneGroupResource(scene->slots[0].group);
                if (task == 0) {
                    return;
                }
                if (task->unit->flags & 0xE0) {
                    return;
                }
                if (task->state != 2) {
                    return;
                }
                if (task->unit->flags & 0x30400000) {
                    return;
                }
                scene->currentTask = task;
                btlDispatchStateHandler(task, 3);
                scene->battleFlags &= ~8;
            }
        }
    }
}

/* Clear scheduling slots and group queues, preserving their fade snapshots. */
void fldClearSceneSlotsAndGroups(void) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    u32 i;

    for (i = 0; i < 8; i++) {
        scene->slots[i].group = 0;
        scene->slots[i].remaining = 0;
        scene->slots[i].id = 0;
    }
    for (i = 0; i < 20; i++) {
        scene->groupPrimary[i] = 0;
    }
    for (i = 0; i < 45; i++) {
        scene->groupSecondary[i] = 0;
    }
    for (i = 0; i < 15; i++) {
        scene->groupTertiary[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        scene->groupHandles[i] = 0;
    }
    fldClearSceneAdvanceFlag();
}

/* Apply a counter request unless its actor belongs to the handle group. */
s32 fldDispatchSceneGroupRequestWhenAllowed(BtlSceneCounterArgs *request) {
    if (request->actor != 0 &&
        (request->actor->pendingFlags & FLD_SCENE_TASK_HANDLE_GROUP_BIT) != 0) {
        return 1;
    }
    fldConsumeSceneSlotCounters(request->amount, request->mode);
    return 1;
}

/* Allocate a counter request, link the source task's actor owner, and store amount/mode. */
BtlRuntimeTask *fldCreateSceneGroupAction(ActionStateLink *actorTask, u32 counterAmount, s32 counterMode) {
    u8 counterModeByte = counterMode;
    BtlRuntimeTask *actionTask = btlAllocTask(FLD_SCENE_COUNTER_REQUEST_BYTES);
    BtlSceneCounterArgs *requestData;
    actionTask->startCondition.kind = 1;
    actionTask->taskId = FLD_SCENE_COUNTER_ACTION_ID;
    actionTask->endCondition.kind = 0;
    if (actorTask != 0) {
        actionTask->ownerId = actorTask->unit->owner;
    }
    actionTask->callback = fldDispatchSceneGroupRequestWhenAllowed;
    actionTask->onStart = 0;
    requestData = btlGetTaskArguments(actionTask);
    requestData->actor = actorTask;
    requestData->amount = counterAmount;
    requestData->mode = counterModeByte;
    return actionTask;
}

/* Stop slot-insert advancement without changing other scene flags. */
void fldStopSceneActorActionUpdate(void) {
    BtlState *scene;

    scene = (BtlState *)btlGetRuntime();
    scene->battleFlags = scene->battleFlags & FLD_SCENE_ADVANCE_CLEAR_MASK;
}

/* Enable advancement before checking the source task, then insert its requested slots.
 * A handle-group task skips insertion but still leaves advancement enabled. */
s32 fldActivateRequestedSceneActor(BtlSceneInsertArgs *request) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    ActionStateLink *actorTask = request->actor;
    scene->battleFlags |= FLD_SCENE_ADVANCE_BIT;
    if (actorTask != 0 && (actorTask->pendingFlags & FLD_SCENE_TASK_HANDLE_GROUP_BIT) != 0) {
        return 1;
    }
    fldInsertSceneSlots(request->count);
    return 1;
}

/* Allocate a slot-insert request; its update callback clears advancement. */
BtlRuntimeTask *fldCreateSceneActorAction(ActionStateLink *actorTask, u32 slotCount) {
    BtlRuntimeTask *actionTask = btlAllocTask(FLD_SCENE_INSERT_REQUEST_BYTES);
    BtlSceneInsertArgs *requestData;
    actionTask->startCondition.kind = 1;
    actionTask->taskId = FLD_SCENE_INSERT_ACTION_ID;
    actionTask->endCondition.kind = 0;
    if (actorTask != 0) {
        actionTask->ownerId = actorTask->unit->owner;
    }
    actionTask->onStart = fldStopSceneActorActionUpdate;
    actionTask->callback = fldActivateRequestedSceneActor;
    requestData = btlGetTaskArguments(actionTask);
    requestData->actor = actorTask;
    requestData->count = slotCount;
    return actionTask;
}

/* Spend the request's counter amount and conditionally swap the front slots. */
s32 fldApplySceneSlotSwapRequest(u32 *request) {
    fldSwapSceneSlots(*request);
    return 1;
}

/* Allocate a one-word counter-spend/swap request, without an actor-owner link. */
u8 *fldCreateActorAction(s32 counterAmount) {
    BtlRuntimeTask *actionTask = btlAllocTask(FLD_SCENE_SWAP_REQUEST_BYTES);
    actionTask->startCondition.kind = 1;
    actionTask->taskId = FLD_SCENE_SWAP_ACTION_ID;
    actionTask->callback = fldApplySceneSlotSwapRequest;
    actionTask->endCondition.kind = 0;
    actionTask->onStart = 0;
    *(u32 *)btlGetTaskArguments(actionTask) = counterAmount;
    return (u8 *)actionTask;
}

/* Set the task's active bit without changing its other flags. */
void btlSetSceneTaskActiveFlag(s32 taskAddress) {
    ((ActionStateLink *)taskAddress)->pendingFlags |= FLD_SCENE_TASK_ACTIVE_BIT;
}

/* Clear only the task's active bit; retain the signed native complement. */
void btlClearSceneTaskActiveFlag(s32 taskAddress) {
    ((ActionStateLink *)taskAddress)->pendingFlags &= ~FLD_SCENE_TASK_ACTIVE_BIT;
}

/* Bind the actor, select a valid secondary-group action, and mark the task bound. */
void btlBindActorTaskAndSelectActionNumber(s32 taskAddress, s32 actorAddress) {
    u32 flags;

    flags = ((BtlUnit *)actorAddress)->flags;
    ((ActionStateLink *)taskAddress)->unit = (BtlUnit *)actorAddress;
    if ((flags & FLD_SCENE_ACTOR_SECONDARY_BIT) != 0 &&
        ((BtlUnit *)actorAddress)->partyRecord.unitId <= 0x17F) {
        ((ActionStateLink *)taskAddress)->actionNumber =
                  (u16)*(u8 *)(((u32)((BtlUnit *)actorAddress)->partyRecord.unitId * 0x14 -
                                                      (u32)((BtlUnit *)actorAddress)->partyRecord.unitId) * 4 + datEnemyRecords + 0x15);
    }
    flags = ((ActionStateLink *)taskAddress)->pendingFlags;
    ((ActionStateLink *)taskAddress)->pendingFlags = flags | FLD_SCENE_TASK_BOUND_BIT;
}

INCLUDE_RODATA(const s32, "game/code_001CFEF8", D_00417278);

INCLUDE_RODATA(const s32, "game/code_001CFEF8", D_00417288);

INCLUDE_SDATA(const s32, "game/code_001CFEF8", D_004368C8);

INCLUDE_SDATA(const s32, "game/code_001CFEF8", D_004368D0);

INCLUDE_SDATA(const s32, "game/code_001CFEF8", D_004368D8);

INCLUDE_SDATA(const s32, "game/code_001CFEF8", D_004368E0);

INCLUDE_SDATA(const s32, "game/code_001CFEF8", D_004368E8);

INCLUDE_SDATA(const s32, "game/code_001CFEF8", D_004368F0);

INCLUDE_SDATA(const s32, "game/code_001CFEF8", D_004368F8);

INCLUDE_SDATA(const s32, "game/code_001CFEF8", D_00436900);

INCLUDE_SDATA(const s32, "game/code_001CFEF8", D_00436908);

INCLUDE_SDATA(const s32, "game/code_001CFEF8", D_00436910);

INCLUDE_SDATA(const s32, "game/code_001CFEF8", D_00436918);

