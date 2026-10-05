#include "common.h"
#include "pcp_vu0.h"
#include "btl.h"

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
#define FLD_SCENE_TASK_FLAGS_OFFSET 8

extern s32 btlGetRuntime(void);
extern s32 btlDoesEnabledStatusMatchCurrentId(s32, u32);

extern void btlDispatchStateHandler(void *, s32);
extern s64 btlStartTask(void *);

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

typedef struct SceneDescriptor {
    s8 unk00;
    u8 unk01;
    u8 unk02;
    u8 pad03[0x1D];
    u16 flags;
    u8 pad22[6];
} SceneDescriptor;

typedef struct BattleEffectParams {
    f32 position[4];
    f32 rotation[4];
    f32 scale[4];
} BattleEffectParams;


typedef struct SceneTask {
    s32 state;
    u16 actionNumber;         /* 0x04 */
    u8 pad06[2];
    u32 flags;
    u32 options;              /* 0x0C */
    u8 pad10[8];
    BtlUnit *actor;
    u8 pad1C[4];
    s32 command;
    s32 commandValue;         /* 0x24 */
    s32 commandReference;      /* 0x28: resolved for command 4 */
    u8 pad2C[0x14];
    s64 linkedOwnerId;        /* 0x40: owner linked to scene-group/effect tasks */
    void (*onUpdate)(void);   /* 0x48 */
    void (*onComplete)(void); /* 0x4C */
    u16 actionStage;          /* 0x50 */
    u8 pad52[2];
    s32 effect;               /* 0x54 */
    u8 pad58[8];
    s32 actorHandle;
    u8 pad64[4];
    s64 ownerId;
} SceneTask;

/* Three-byte scheduling slot. IDs move with slots; insertion renumbers them. */
typedef struct SceneSlot {
    u8 group;
    u8 remaining;
    u8 id;
} SceneSlot;

/* Native fade snapshot: slot bytes, alpha byte, then signed target word. */
typedef struct SceneFadingRecord {
    SceneSlot slot;
    u8 alpha;
    s32 target;
} SceneFadingRecord;

typedef struct SceneLinkedNode {
    u32 state;
    u8 pad04[4];
    u32 flags;
    u32 options;
    u8 pad10[8];
    BtlUnit *actor;
    u8 pad1C[0x15C];
    struct SceneLinkedNode *next; /* 0x178: scene-linked chain */
} SceneLinkedNode;

/* Primary scene controller; loading and group scheduling share this record. */
typedef struct BattleSceneWork {
    u8 pad00[0x218];
    u32 flags;
    u32 subFlags;
    u32 refreshFlags;
    u8 pad224[8];
    s32 currentScene;
    s32 queuedScene;
    s32 frame;
    s32 sceneState;
    u8 pad23C[4];
    s32 scriptState;          /* 0x240 */
    s32 scriptArg;            /* 0x244 */
    SceneLinkedNode *linkedNodes;
    BtlUnit *actors;
    u8 pad250[0x1E];
    u8 phaseFlag;
    u8 pad26F;
    u16 variant;
    u8 pad272[2];
    s32 step;
    u8 pad278[4];
    u8 unk27C;
    u8 pad27D[0x17];
    s32 effectLayer;
    u8 pad298[8];
    s32 mode;
    u8 pad2A4[8];
    s16 tileX;
    s16 tileY;
    s32 loadStep;             /* 0x2B0 */
    u8 pad2B4[0x10];
    u32 taskParent;
    u8 pad2C8[4];
    s32 pendingTask;          /* 0x2CC */
    u32 sceneObject;
    u32 spriteObject;
    u32 sceneStatus;
    u8 pad2DC[0x1C];
    s32 unk2F8;
    u8 pad2FC[2];
    SceneSlot slots[8];
    u8 pad316[2];
    SceneTask *groupPrimary[20];    /* 0x318 */
    SceneTask *groupSecondary[45];  /* 0x368 */
    SceneTask *groupTertiary[15];   /* 0x41C */
    SceneTask *groupHandles[8];     /* 0x458 */
    u16 groupHandleCount;
    u8 pad47A[2];
    s32 activeGroupCount;
    SceneFadingRecord fading[8];
    SceneTask *currentTask;
    u8 pad4C4[0x10];
    s32 scriptTarget;         /* 0x4D4 */
    u8 pad4D8[0xC];
    u32 values[64];
    s32 (*sceneCallback)();    /* 0x5E4 */
    u8 pad5E8[0x44];
    void (*completionHook)();  /* 0x62C */
} BattleSceneWork;

extern SceneDescriptor *datBattleSceneRecords;

extern f32 *D_0037F770[];

extern s32 btlCreateEffectTaskWithSourceParams(BattleEffectParams *, s32);

extern s32 btlCountTasksForOwner(s64);

extern SceneInitializer D_003B6938[];

extern void btlRemoveTaskFromSceneGroup(SceneTask *);

extern void fldInitializeSceneGroups(void);

typedef struct SceneEntry {
    u16 flags;                /* 0x000 */
    u8 pad02[6];
    u16 weight;               /* 0x008 */
    u8 pad0A[4];
    u16 mask;                 /* 0x00E */
    u8 pad10[0x1A8];
    s32 link;                 /* 0x1B8 */
    u8 pad1BC[8];
} SceneEntry;

typedef struct SceneParty {
    u8 pad00[0xA60];
    SceneEntry entry[5];
} SceneParty;

extern SceneParty *datGameState;

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
extern s32 func_001B73E8(void);
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

extern s32 btlCreateSoundUpdateTask();

extern s32 btlCreateSoundReleaseTask();

extern s32 btlCreateWaitUnitListIdleTask();

extern s32 btlCreateApplyToActiveActorsTask();

extern s32 btlCreateFadeStateResetTask();

extern void btlCreateGuidePanelTask();

extern void func_001B81B0();

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
    u32 id = ((BattleSceneWork *)scene)->mode;

    if (id < 0x400 && (datBattleSceneRecords[id].flags & 0x8000) != 0) {
        ((BattleSceneWork *)scene)->subFlags |= 8;
        kwlnFadeInStart(0xFF, 0xFF, 0xFF, 0);
    }
    if (func_001B4040() == 0) {
        if (func_001B4210() == 0) {
            ((BattleSceneWork *)scene)->phaseFlag = 0;
        } else {
            ((BattleSceneWork *)scene)->phaseFlag = 2;
        }
    } else {
        ((BattleSceneWork *)scene)->phaseFlag = 3;
    }
    func_0022AF90();
    func_00229728(((BattleSceneWork *)scene)->mode);
    btlSelectSceneAudioTrack(((BattleSceneWork *)scene)->effectLayer, ((BattleSceneWork *)scene)->mode);
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

void fldMarkGridTiles(BattleSceneWork *scene) {
    u8 *tile = (u8 *)fldCreateSceneTileTask(scene->tileX, scene->tileY);
    ((SceneTask *)tile)->linkedOwnerId = 0x8000000000000001ULL;
    btlStartTask(tile);
    tile = (u8 *)btlCreateFloorLoadTask(scene->tileX, scene->tileY);
    btlStartTask(tile);
}

s32 fldSceneStateStartTileEffect(BattleSceneWork *scene) {
    BattleEffectParams params;
    f32 *origin;
    if (btlCountTasksForOwner(0x8000000000000001LL) == 0) {
        btlResetTitleStreamOnBattleFlag();
        func_001E9410();
        btlSpawnBattleWorldAction();
        btlCreateRainEffect(scene->tileX, scene->tileY);
        if (scene->subFlags & 0x40000) {
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

s32 fldSceneStateRestoreDisplay(BattleSceneWork *scene) {
    if (btlCountTasksForOwner(0x8000000000000002LL) == 0) {
        func_00206090();
        if (!(scene->flags & 0x4000)) {
            btlRepositionPartyAroundBattleCenter();
        }
        func_001AC648();
        if (scene->subFlags & 0x80) {
            func_001AEEA8();
            scene->subFlags &= ~0x80;
        }
        if (!(scene->flags & 0x4000)) {
            kwlnFadeBackgroundStartOut(0);
            kwlnDrawSetOverlayTransition(0, 0, 1);
            kwlnDrawEnableD88(0);
            kwlnDrawEnableDc8(0);
            kwlnDrawEnableE08(0);
            kwlnDrawSetupC70B(0);
            kwlnDrawEnableCd0(0);
            kwlnDrawEnableD30(0);
            if (!(scene->subFlags & 0x100000)) {
                btlMarkRuntimeUpdatePending();
            }
            if (!(scene->subFlags & 8)) {
                kwlnFadeStartIn(0);
            }
            if (fldGetEncounterRuntimeResult() != 0) {
                fldSetEncounterPendingValue(1);
            }
            evtSetSolarOverlayFullyVisible();
        }
        if (scene->sceneStatus == 0) {
            fldCreateSceneCleanupTask();
            if (!(scene->subFlags & 0x2000)) {
                scene->flags |= 0x100000;
            } else {
                scene->subFlags &= ~0x2000;
                scene->flags &= 0xFFEFFFFF;
            }
        }
        btlSyncModelFlagFromEventThresholds();
        return 5;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001CFEF8", func_001D08A8);

s32 btlInitializeSceneAfterTasksAndBuffersReady(BattleSceneWork *scene) {
    u32 i;

    if (scene->subFlags & 0x100000) {
        if (btlCountTasksForOwner(0x8000000000000004LL) == 0) {
            btlMarkRuntimeUpdatePending();
            scene->subFlags &= ~0x100000;
        }
    }
    if (btlCountTasksForOwner(0x8000000000000003LL) == 0 &&
        btlAreWorkBuffersReady() != 0 &&
        func_0022E490() != 0) {
        fldInitializeSceneGroups();
        func_001B73E8();
        scene->unk27C = 0;
        func_001B3DD8();
        if (datBattleSceneRecords[scene->mode].unk01 != 0) {
            for (i = 0; i < datBattleSceneRecords[scene->mode].unk02; i++) {
                func_001AD5B0(datBattleSceneRecords[scene->mode].unk01);
            }
        }
        return 8;
    }
    return 0;
}

void btlConsumeSceneAdvanceFlags(void) {
}

s32 fldConsumeSceneInputFlags(BattleSceneWork *scene) {
    u32 flags = scene->flags;
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
    scene->flags |= 0x20;
    return result;
}

void fldAdvanceSceneGroupInitialization(BattleSceneWork *scene) {
    s32 notFirst = scene->variant != 1;
    scene->variant = 2 - notFirst;
    if (scene->sceneCallback != 0) {
        s32 variant = scene->sceneCallback();
        if (variant != -1) {
            scene->variant = variant;
        }
    }
    scene->groupHandleCount = 0;
    scene->step = scene->step + 1;
    fldInitializeSceneGroups();
    func_001B73E8();
}

typedef struct SceneEffectRequest {
    u8 startKind;
    u8 pad01[7];
    u16 taskId;
} SceneEffectRequest;

extern void btlTickActorEntryCountdowns(u8 *);
extern void btlResetBattleHistoryCounters(void);
extern void btlClearActorSelectedEntryIndex(BtlUnit *);
extern void btlRefreshUnitMotionSelection(u8 *);
extern SceneEffectRequest *btlCreateEffObjC(BtlUnit *, s32);

/* Advance the scene after its bound actor tasks finish their preparation. */
s32 btlAdvanceSceneWhenActorTasksReady(BattleSceneWork *scene) {
    s32 ready = 1;
    u32 group = scene->variant == 1 ? FLD_SCENE_ACTOR_PRIMARY_BIT : FLD_SCENE_ACTOR_SECONDARY_BIT;
    SceneLinkedNode *head = scene->linkedNodes;
    SceneLinkedNode *node;
    BtlUnit *actor;

    for (node = head; node != NULL; node = node->next) {
        if ((node->flags & FLD_SCENE_TASK_BOUND_BIT) &&
            (node->actor->flags & FLD_SCENE_TASK_ACTIVE_BIT) &&
            node->state >= 3) {
            ready = 0;
            break;
        }
    }
    for (node = head; node != NULL; node = node->next) {
        if (node->flags & FLD_SCENE_TASK_BOUND_BIT) {
            actor = node->actor;
            if ((actor->flags & group) &&
                (actor->flags & FLD_SCENE_TASK_ACTIVE_BIT) &&
                !(actor->flags & 0xE0)) {
                if (node->state != 2) {
                    ready = 0;
                }
            }
        }
    }
    if (ready && scene->frame > 16) {
        for (node = head; node != NULL; node = node->next) {
            if (node->flags & FLD_SCENE_TASK_BOUND_BIT) {
                actor = node->actor;
                if (actor->flags & FLD_SCENE_TASK_ACTIVE_BIT) {
                    if (!(actor->flags & 0xE0)) {
                        actor->flags &= ~0x10U;
                        btlTickActorEntryCountdowns((u8 *)actor);
                        if (actor->conditionFlags & 0x322F) {
                            btlDispatchStateHandler(node, 4);
                        }
                        node->options &= ~4U;
                        if (!(actor->flags & group)) {
                            actor->stateFlags &= ~0x800000U;
                        } else {
                            btlResetBattleHistoryCounters();
                            btlClearActorSelectedEntryIndex(actor);
                            btlRefreshUnitMotionSelection((u8 *)actor);
                        }
                    }
                }
            }
        }
        if (scene->subFlags & FLD_SCENE_COUNTERS_ENABLED_BIT) {
            s32 affected = 0;
            s32 total = 0;
            BtlUnit *selected = NULL;
            s32 message;
            SceneEffectRequest *request;
            u16 status;

            for (actor = scene->actors; actor != NULL; actor = actor->nextActor) {
                if (actor->flags & FLD_SCENE_TASK_ACTIVE_BIT) {
                    if (actor->flags & FLD_SCENE_ACTOR_SECONDARY_BIT) {
                        total++;
                        status = actor->conditionFlags & 1;
                        if (status != 0) {
                            affected++;
                            selected = actor;
                        }
                    }
                }
            }
            if (affected == 1) {
                message = 0;
            } else if (affected == total) {
                message = 1;
            } else {
                message = 2;
            }
            if (affected != 0) {
                request = btlCreateEffObjC(selected, message);
                request->startKind = 0xA;
                request->taskId = 0x2E;
                btlStartTask(request);
            }
            scene->subFlags &= ~FLD_SCENE_COUNTERS_ENABLED_BIT;
        }
        return 8;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001CFEF8", func_001D14B0);

s32 fldSceneStateWaitScriptRelease(BattleSceneWork *scene) {
    s32 finished = 1;
    s32 state = scene->scriptState;

    switch (state) {
    case 0xF000002:
        if (btlReleaseScriptResourceA() == 0) {
            finished = 0;
        } else if (!(scene->subFlags & 0x40000)) {
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
                    func_001B81B0();
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
        if ((scene->flags & 0x800) == 0) {
            fldEnableSceneGroupAdvancement();
            scene->flags &= ~0x400;
            scene->flags &= ~0x1000;
            scene->flags &= ~0x20;
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

extern void evtBeginSolarOverlayFadeOut();

extern void func_001AA868();

extern s32 func_0029D000();

extern void datAdjustCurrentHp();

/* Nudge every active party entry's cursor by weight * 0.05 once scene flag
 * 0x40 is clear. */
void btlApplyPartyEntryWeightedDelta(BattleSceneWork *scene) {
    u32 i;
    f32 scale;
    i = 0;
    itfMesClearFlags(1);
    scale = 0.05f;
    brsTaskAllowUpdate();
    evtBeginSolarOverlayFadeOut(8);
    do {
        func_001AA868(&datGameState->entry[i], -0x45D1);
        if (!(scene->subFlags & 0x40)) {
            if (datGameState->entry[i].flags & 2) {
                if (scene->unk27C == 1) {
                    if (datGameState->entry[i].link != 0 || scene->unk2F8 != 0) {
                        if (!(datGameState->entry[i].mask & 0x40)) {
                            if (func_0029D000(&datGameState->entry[i]) == 0) {
                                datAdjustCurrentHp(&datGameState->entry[i], (s32)((f32)datGameState->entry[i].weight * scale));
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
u32 fldStepAreaLoad(BattleSceneWork *work) {
    SceneLinkedNode *node;
    s32 step;
    if (func_002998D8() == 1) {
        work->subFlags |= 0x20;
        if (work->linkedNodes != 0) {
            for (node = work->linkedNodes; node != 0; node = node->next) {
                if (node->state != 0x1F) {
                    btlDispatchStateHandler(node, 0x1F);
                }
            }
        } else {
            switch (work->loadStep) {
            case 0:
                work->flags &= ~0x10;
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
    node = (u8 *)((BattleSceneWork *)scene)->linkedNodes;
    while (node != 0) {
        if (((SceneLinkedNode *)node)->state != 0x1F) {
            btlDispatchStateHandler(node, 0x1F);
        }
        node = (u8 *)((SceneLinkedNode *)node)->next;
    }
    btlFlagTasksForUpdate();
}

INCLUDE_ASM(const s32, "game/code_001CFEF8", func_001D2A78);

void fldMarkSceneRefresh(BattleSceneWork *scene) {
    func_00230960((s32)scene);
    scene->refreshFlags |= 4;
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
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    scene->currentScene = sceneId;
    scene->frame = 0;
    scene->sceneState = 0;
    D_003B6938[sceneId].initialize((s32)scene);
}

/* Queue a transition for the next scene update. */
void btlQueueScene(u32 sceneId) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)btlGetRuntime();
    scene->queuedScene = sceneId;
}

/* Consume a queued transition, run the current updater, and advance its frame. */
void btlUpdateScene(void) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
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
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)btlGetRuntime();
    btlSetScene(FLD_SCENE_INITIAL_ID);
    scene->queuedScene = 0;
}

void btlGetCurrentSceneRecordValue(void) {
}

/* Return the current initializer row's flags word. */
s32 fldGetSceneDescriptorProperty(void) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    s32 sceneId = scene->currentScene;
    return D_003B6938[sceneId].flags;
}

/* Select the task's group array; the handle-group bit takes precedence. */
SceneTask **fldGetActorSceneGroupResource(SceneTask *task) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    SceneTask **groupEntries = scene->groupSecondary;
    u32 actorGroupBits;
    if ((task->flags & FLD_SCENE_TASK_HANDLE_GROUP_BIT) != 0) {
        return scene->groupHandles;
    }
    actorGroupBits = (u32)task->actor->flags64 & FLD_SCENE_ACTOR_GROUP_MASK;
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
SceneTask **fldGetSceneGroupResource(u8 groupId) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    SceneTask **groupEntries;
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
s32 fldClassifyActorSceneGroup(SceneTask *task) {
    u32 actorGroupBits;
    btlGetRuntime();
    if (task->flags & FLD_SCENE_TASK_HANDLE_GROUP_BIT) {
        return FLD_SCENE_HANDLE_TASK_COUNT;
    }
    actorGroupBits = (u32)task->actor->flags64 & FLD_SCENE_ACTOR_GROUP_MASK;
    switch (actorGroupBits) {
    case FLD_SCENE_ACTOR_PRIMARY_BIT: return FLD_SCENE_PRIMARY_TASK_COUNT;
    case FLD_SCENE_ACTOR_SECONDARY_BIT: return FLD_SCENE_SECONDARY_TASK_COUNT;
    case FLD_SCENE_ACTOR_TERTIARY_BIT: return FLD_SCENE_TERTIARY_TASK_COUNT;
    default: return 0;
    }
}

extern u32 func_001AB8D8();

/* Sort adjacent occupied tasks by descending accessor value; null slots stay put. */
void btlSortSceneGroupByPriorityDesc(SceneTask **group, s32 entryCount) {
    s32 swapped;
    do {
        SceneTask **pairCursor = group;
        u32 pairIndex = 0;
        swapped = 0;
        for (; pairIndex < entryCount - 1; pairIndex++, pairCursor++) {
            SceneTask *first = pairCursor[0];
            SceneTask *second = pairCursor[1];
            if (first != 0 && second != 0 &&
                func_001AB8D8((s32)&first->actor->statBits, 3) < func_001AB8D8((s32)&second->actor->statBits, 3)) {
                pairCursor[0] = second;
                swapped = 1;
                pairCursor[1] = first;
            }
        }
    } while (swapped != 0);
}

/* Sort adjacent occupied tasks by ascending actor byte priority; null slots stay put. */
void fldSortGroupByPriority(SceneTask **group, s32 entryCount) {
    s32 swapped;
    do {
        SceneTask **pairCursor = group;
        u32 pairIndex = 0;
        swapped = 0;
        for (; pairIndex < entryCount - 1; pairIndex++, pairCursor++) {
            SceneTask *first = pairCursor[0];
            SceneTask *second = pairCursor[1];
            if (first != 0 && second != 0 &&
                first->actor->lookupId > second->actor->lookupId) {
                pairCursor[0] = second;
                swapped = 1;
                pairCursor[1] = first;
            }
        }
    } while (swapped != 0);
}

/* Map an exact actor-group bit pattern to its group ID; combined bits return zero. */
s32 fldGetSceneGroupIndexByActorFlags(u8 *task) {
    u32 actorGroupBits = ((SceneTask *)task)->actor->flags & FLD_SCENE_ACTOR_GROUP_MASK;
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
    SceneSlot *slotCursor = ((BattleSceneWork *)btlGetRuntime())->slots;
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
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    u8 headGroup;
    u32 i;
    if (scene->flags & 0x100) {
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
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    s32 groupId;
    s32 usedSlotCount;
    s32 slotIndex;

    groupId = scene->variant == FLD_SCENE_GROUP_PRIMARY_ID ? FLD_SCENE_GROUP_PRIMARY_ID : FLD_SCENE_GROUP_SECONDARY_ID;
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
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    if (scene->flags & FLD_SCENE_COUNTERS_ENABLED_BIT) {
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
    SceneSlot *slotCursor = ((BattleSceneWork *)btlGetRuntime())->slots;
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
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    SceneSlot *slotCursor;
    u32 slotIndex;
    if (scene->flags & FLD_SCENE_SLOTS_FINISHED_BIT) {
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
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)btlGetRuntime();
    fldSortGroupByPriority(scene->groupPrimary, FLD_SCENE_PRIMARY_TASK_COUNT);
    btlSortSceneGroupByPriorityDesc(scene->groupSecondary, FLD_SCENE_SECONDARY_TASK_COUNT);
    btlSortSceneGroupByPriorityDesc(scene->groupTertiary, FLD_SCENE_TERTIARY_TASK_COUNT);
    func_001D30E0();
}

/* Move a member behind its occupied successors; native membership/bounds are unchecked. */
void btlMoveTaskToGroupTail(SceneTask *task) {
    SceneTask **groupCursor = fldGetActorSceneGroupResource(task);
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
void btlRotateGroupUntilTaskFirst(SceneTask *task) {
    BattleSceneWork *scene;
    u32 rotationCount;
    if (task != 0 && (task->flags & FLD_SCENE_TASK_BOUND_BIT) != 0 && task->actor != 0 && !(task->flags & FLD_SCENE_TASK_HANDLE_GROUP_BIT) &&
        ((u32)task->actor->flags64 & FLD_SCENE_ACTOR_PRIMARY_BIT) != 0) {
        rotationCount = 0;
        scene = (BattleSceneWork *)btlGetRuntime();
        fldSortGroupByPriority(scene->groupPrimary, FLD_SCENE_PRIMARY_TASK_COUNT);
        for (; rotationCount < FLD_SCENE_PRIMARY_TASK_COUNT && scene->groupPrimary[0] != task; rotationCount++) {
            btlMoveTaskToGroupTail(scene->groupPrimary[0]);
        }
    }
}

/* Append at the first empty entry, then restore the former head's ordering.
 * Retain the native capacity query and unchecked append scan. */
void fldAppendTaskToGroup(SceneTask *task) {
    SceneTask **groupCursor = fldGetActorSceneGroupResource(task);
    SceneTask *previousHead;
    fldClassifyActorSceneGroup(task);
    previousHead = *groupCursor;
    while (*groupCursor != 0) {
        groupCursor++;
    }
    *groupCursor = task;
    btlRotateGroupUntilTaskFirst(previousHead);
}

/* Append a handle to the first zero entry; the caller must leave room. */
void fldAppendSceneGroupHandle(s32 handle) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    u32 *handleCursor = scene->groupHandles;
    while (*handleCursor != 0) {
        handleCursor++;
    }
    *handleCursor = handle;
}

void fldUpdateSceneGroupTask(SceneTask *task) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    BtlUnit *actor;
    s32 kind;

    if ((task->flags & 0x40) == 0) {
        actor = task->actor;
        kind = (actor->flags & 0x200) != 0 ? 1 : 2;
        scene->groupHandleCount++;
        if (kind == 1) {
            scene->activeGroupCount++;
        }
        if (scene->currentTask == task && scene->variant == kind) {
            scene->flags |= 8;
        }
        if ((*(u64 *)&task->flags & 0x400000100LL) == 0) {
            if (btlDoesEnabledStatusMatchCurrentId((s32)&actor->statBits, 0xDE) != 0) {
                task->options |= 4;
            }
        } else {
            task->options &= ~4;
        }
        if ((scene->flags & 0x40) != 0 && (task->options & 4) == 0) {
            btlMoveTaskToGroupTail(task);
        }
    } else {
        btlRemoveTaskFromSceneGroup(task);
        scene->flags |= 8;
        task->flags &= ~0x40;
    }
}

/* Clear the matching member and move that hole to the last group entry. */
void btlRemoveTaskFromSceneGroup(SceneTask *task) {
    SceneTask **groupEntries = fldGetActorSceneGroupResource(task);
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
        SceneTask *current = groupEntries[entryIndex];
        SceneTask *next = groupEntries[entryIndex + 1];
        groupEntries[entryIndex + 1] = current;
        groupEntries[entryIndex] = next;
    }
}

/* Set the native advance/update bit pair without changing other scene flags. */
void fldEnableSceneGroupAdvancement(void) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)btlGetRuntime();
    scene->flags = scene->flags | FLD_SCENE_ADVANCE_ENABLE_BITS;
}

/* Clear only the advancement bit, retaining the unsigned native mask. */
void fldClearSceneAdvanceFlag(void) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)btlGetRuntime();
    scene->flags = scene->flags & FLD_SCENE_ADVANCE_CLEAR_MASK;
}

/* Prefer the special handle queue; otherwise return the front group's first task. */
s32 fldGetActiveSceneGroupValue(void) {
    u8 *scene = (u8 *)btlGetRuntime();
    s32 headValue = (s32)((BattleSceneWork *)scene)->groupHandles[0];
    if (headValue != 0) {
        return headValue;
    }
    return *(s32 *)fldGetSceneGroupResource(((BattleSceneWork *)scene)->slots[0].group);
}

/* Read one task word from the front group; an empty front slot returns zero. */
s32 fldGetSceneGroupEntry(s32 entryIndex) {
    u8 *scene = (u8 *)btlGetRuntime();
    if (((BattleSceneWork *)scene)->slots[0].group == 0) {
        return 0;
    }
    return ((s32 *)fldGetSceneGroupResource(((BattleSceneWork *)scene)->slots[0].group))[entryIndex];
}

extern s32 func_001B2630();

/* Per-frame scene advance: picks the next group task (or finishes the
 * slot sequence) and dispatches it once flags 4 and 8 are set. */
void func_001D3ED8(void) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    SceneTask *task;
    void (*hook)(void);
    if (scene->flags & 4) {
        if (scene->flags & 8) {
            if (scene->flags & 0x800) {
                return;
            }
            if (scene->flags & 0x400) {
                return;
            }
            if (func_001B2630() != 0) {
                scene->flags |= 0x800;
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
                scene->currentTask = task;
                task->flags |= 0x40;
                btlDispatchStateHandler(task, 3);
                scene->flags &= ~8;
            } else {
                if (fldAreSceneSlotsFinished() != 0) {
                    scene->flags |= 0x400;
                    return;
                }
                task = *fldGetSceneGroupResource(scene->slots[0].group);
                if (task == 0) {
                    return;
                }
                if (task->actor->flags & 0xE0) {
                    return;
                }
                if (task->state != 2) {
                    return;
                }
                if (task->actor->flags & 0x30400000) {
                    return;
                }
                scene->currentTask = task;
                btlDispatchStateHandler(task, 3);
                scene->flags &= ~8;
            }
        }
    }
}

/* Clear scheduling slots and group queues, preserving their fade snapshots. */
void fldClearSceneSlotsAndGroups(void) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
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

/* Apply the requested amount/mode unless its source task uses the handle group.
 * Always return 1, including the blocked path; keep both native mode loads. */
u32 fldDispatchSceneGroupRequestWhenAllowed(s32 *request) {
    u8 counterMode;

    if (*request == 0) {
        counterMode = (u8)request[2];
    }
    else {
        if ((*(u32 *)(*request + FLD_SCENE_TASK_FLAGS_OFFSET) & FLD_SCENE_TASK_HANDLE_GROUP_BIT) != 0) {
            return 1;
        }
        counterMode = (u8)request[2];
    }
    fldConsumeSceneSlotCounters(request[1], counterMode);
    return 1;
}

/* Allocate a counter request, link the source task's actor owner, and store amount/mode. */
u8 *fldCreateSceneGroupAction(u8 *actorTask, u32 counterAmount, s32 counterMode) {
    u8 counterModeByte = counterMode;
    u8 *actionTask = (u8 *)btlAllocTask(FLD_SCENE_COUNTER_REQUEST_BYTES);
    u8 *requestData;
    actionTask[0] = 1;
    *(s16 *)(actionTask + 0x20) = FLD_SCENE_COUNTER_ACTION_ID;
    actionTask[0x10] = 0;
    if (actorTask != 0) {
        ((SceneTask *)actionTask)->linkedOwnerId = ((SceneTask *)actorTask)->actor->owner;
    }
    ((SceneTask *)actionTask)->onComplete = (void (*)(void))fldDispatchSceneGroupRequestWhenAllowed;
    ((SceneTask *)actionTask)->onUpdate = 0;
    requestData = (u8 *)btlGetTaskArguments(actionTask);
    *(u32 *)(requestData + 0) = (u32)actorTask;
    *(u32 *)(requestData + 4) = counterAmount;
    requestData[8] = counterModeByte;
    return actionTask;
}

/* Stop slot-insert advancement without changing other scene flags. */
void fldStopSceneActorActionUpdate(void) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)btlGetRuntime();
    scene->flags = scene->flags & FLD_SCENE_ADVANCE_CLEAR_MASK;
}

/* Enable advancement before checking the source task, then insert its requested slots.
 * A handle-group task skips insertion but still leaves advancement enabled. */
s32 fldActivateRequestedSceneActor(u32 *request) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    u32 *actorTask = (u32 *)request[0];
    scene->flags |= FLD_SCENE_ADVANCE_BIT;
    if (actorTask != 0 && (actorTask[2] & FLD_SCENE_TASK_HANDLE_GROUP_BIT) != 0) {
        return 1;
    }
    fldInsertSceneSlots(request[1]);
    return 1;
}

/* Allocate a slot-insert request; its update callback clears advancement. */
u8 *fldCreateSceneActorAction(u8 *actorTask, u32 slotCount) {
    u8 *actionTask = (u8 *)btlAllocTask(FLD_SCENE_INSERT_REQUEST_BYTES);
    u32 *requestData;
    actionTask[0] = 1;
    *(s16 *)(actionTask + 0x20) = FLD_SCENE_INSERT_ACTION_ID;
    actionTask[0x10] = 0;
    if (actorTask != 0) {
        ((SceneTask *)actionTask)->linkedOwnerId = ((SceneTask *)actorTask)->actor->owner;
    }
    ((SceneTask *)actionTask)->onUpdate = (void (*)(void))fldStopSceneActorActionUpdate;
    ((SceneTask *)actionTask)->onComplete = (void (*)(void))fldActivateRequestedSceneActor;
    requestData = (u32 *)btlGetTaskArguments(actionTask);
    requestData[0] = (u32)actorTask;
    requestData[1] = slotCount;
    return actionTask;
}

/* Spend the request's counter amount and conditionally swap the front slots. */
u32 fldApplySceneSlotSwapRequest(u32 *request) {
    fldSwapSceneSlots(*request);
    return 1;
}

/* Allocate a one-word counter-spend/swap request, without an actor-owner link. */
u8 *fldCreateActorAction(s32 counterAmount) {
    u8 *actionTask = (u8 *)btlAllocTask(FLD_SCENE_SWAP_REQUEST_BYTES);
    actionTask[0] = 1;
    *(s16 *)(actionTask + 0x20) = FLD_SCENE_SWAP_ACTION_ID;
    ((SceneTask *)actionTask)->onComplete = (void (*)(void))fldApplySceneSlotSwapRequest;
    actionTask[0x10] = 0;
    ((SceneTask *)actionTask)->onUpdate = 0;
    *(u32 *)btlGetTaskArguments(actionTask) = counterAmount;
    return actionTask;
}

/* Set the task's active bit without changing its other flags. */
void btlSetSceneTaskActiveFlag(s32 taskAddress) {
    ((SceneTask *)taskAddress)->flags |= FLD_SCENE_TASK_ACTIVE_BIT;
}

/* Clear only the task's active bit; retain the signed native complement. */
void btlClearSceneTaskActiveFlag(s32 taskAddress) {
    ((SceneTask *)taskAddress)->flags &= ~FLD_SCENE_TASK_ACTIVE_BIT;
}

/* Bind the actor, select a valid secondary-group action, and mark the task bound. */
void btlBindActorTaskAndSelectActionNumber(s32 taskAddress, s32 actorAddress) {
    u32 flags;

    flags = ((BtlUnit *)actorAddress)->flags;
    ((SceneTask *)taskAddress)->actor = (BtlUnit *)actorAddress;
    if ((flags & FLD_SCENE_ACTOR_SECONDARY_BIT) != 0 &&
        ((BtlUnit *)actorAddress)->mode <= 0x17F) {
        ((SceneTask *)taskAddress)->actionNumber =
                  (u16)*(u8 *)(((u32)((BtlUnit *)actorAddress)->mode * 0x14 -
                                                      (u32)((BtlUnit *)actorAddress)->mode) * 4 + datEnemyRecords + 0x15);
    }
    flags = ((SceneTask *)taskAddress)->flags;
    ((SceneTask *)taskAddress)->flags = flags | FLD_SCENE_TASK_BOUND_BIT;
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

