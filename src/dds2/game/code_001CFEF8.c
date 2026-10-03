#include "common.h"
#include "pcp_vu0.h"

extern s32 btlGetRuntime(void);

extern void btlDispatchStateHandler();

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
    u8 pad01[0x1F];
    u16 flags;
    u8 pad22[6];
} SceneDescriptor;

typedef struct BattleEffectParams {
    f32 position[4];
    f32 rotation[4];
    f32 scale[4];
} BattleEffectParams;

typedef struct SceneActor {
    u8 pad_00[0xC8];
    s32 species;              /* 0xC8 */
    u8 pad_CC[0x3C];
    s64 ownerId;
    union {
        u64 flags;            /* 0x110: combined status mask */
        struct {
            u32 activeFlags;  /* 0x110 */
            u32 stateFlags;   /* 0x114 */
        } words;
    } status;
    u32 extraFlags;           /* 0x118 */
    u8 priority;
    u8 pad_11D[3];
    u8 entryData[4];
    u16 kind;
    u8 pad_126[2];
    u16 statA;                /* 0x128 */
    u8 pad_12A[2];
    u16 statB;                /* 0x12C */
    u16 selectionFlags;
    u8 pad_130[0x12];
    u16 cards[8];
    u8 pad_152[0x1C2];
    s32 actionResource;       /* 0x314 */
    s32 resourceNode;
    u8 pad_31C[8];
    s32 listNode;
    u8 pad_328[0xC];
    s32 pendingResource;
    u8 pad_338[0x2C];
    struct SceneActor *next;
} SceneActor;

typedef struct SceneTask {
    s32 state;
    u16 actionNumber;         /* 0x04 */
    u8 pad06[2];
    u32 flags;
    u32 options;              /* 0x0C */
    u8 pad10[8];
    SceneActor *actor;
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
    s32 state;
    u8 pad04[0x174];
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
    SceneActor *actors;
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

extern void func_001B8078();

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

INCLUDE_ASM(const s32, "game/code_001CFEF8", func_001D0FE0);

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

INCLUDE_ASM(const s32, "game/code_001CFEF8", func_001D1200);

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
            func_001B8078(scene->scriptTarget, scene->scriptArg);
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

void btlSetScene(s32 scene) {
    BattleSceneWork *work = (BattleSceneWork *)btlGetRuntime();
    work->currentScene = scene;
    work->frame = 0;
    work->sceneState = 0;
    D_003B6938[scene].initialize((s32)work);
}

void btlQueueScene(u32 scene) {
    BattleSceneWork *work;

    work = (BattleSceneWork *)btlGetRuntime();
    work->queuedScene = scene;
}

void btlUpdateScene(void) {
    BattleSceneWork *work = (BattleSceneWork *)btlGetRuntime();
    SceneInitializer *entry;
    s32 event;
    if (work->queuedScene != 0) {
        btlSetScene(work->queuedScene);
        work->queuedScene = 0;
    }
    entry = &D_003B6938[work->currentScene];
    event = entry->update((s32)work);
    if (event != 0) {
        btlQueueScene(event);
    }
    work->frame++;
}

void btlResetToInitialScene(void) {
    BattleSceneWork *work;

    work = (BattleSceneWork *)btlGetRuntime();
    btlSetScene(1);
    work->queuedScene = 0;
}

void btlGetCurrentSceneRecordValue(void) {
}

s32 fldGetSceneDescriptorProperty(void) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    s32 index = scene->currentScene;
    return D_003B6938[index].flags;
}

SceneTask **fldGetActorSceneGroupResource(SceneTask *task) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    SceneTask **entry = scene->groupSecondary;
    u32 flags;
    if ((task->flags & 0x40) != 0) {
        return scene->groupHandles;
    }
    flags = (u32)task->actor->status.flags & 0xE00;
    switch (flags) {
    case 0x200:
        entry = scene->groupPrimary;
        break;
    case 0x400:
        break;
    case 0x800:
        entry = scene->groupTertiary;
        break;
    default:
        entry = 0;
        break;
    }
    return entry;
}

SceneTask **fldGetSceneGroupResource(u8 group) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    SceneTask **entry;
    switch (group) {
    case 1:
        entry = scene->groupPrimary;
        break;
    case 2:
        entry = scene->groupSecondary;
        break;
    case 3:
        entry = scene->groupTertiary;
        break;
    default:
        entry = 0;
        break;
    }
    return entry;
}

s32 fldClassifyActorSceneGroup(SceneTask *task) {
    u32 flags;
    btlGetRuntime();
    if (task->flags & 0x40) {
        return 8;
    }
    flags = (u32)task->actor->status.flags & 0xE00;
    switch (flags) {
    case 0x200: return 0x14;
    case 0x400: return 0x2D;
    case 0x800: return 0xF;
    default: return 0;
    }
}

extern u32 func_001AB8D8();

void btlSortSceneGroupByPriorityDesc(SceneTask **list, s32 count) {
    s32 swapped;
    do {
        SceneTask **entry = list;
        u32 i = 0;
        swapped = 0;
        for (; i < count - 1; i++, entry++) {
            SceneTask *first = entry[0];
            SceneTask *second = entry[1];
            if (first != 0 && second != 0 &&
                func_001AB8D8((s32)first->actor + 0x120, 3) < func_001AB8D8((s32)second->actor + 0x120, 3)) {
                entry[0] = second;
                swapped = 1;
                entry[1] = first;
            }
        }
    } while (swapped != 0);
}

void fldSortGroupByPriority(SceneTask **group, s32 count) {
    s32 swapped;
    do {
        SceneTask **entry = group;
        u32 i = 0;
        swapped = 0;
        for (; i < count - 1; i++, entry++) {
            SceneTask *first = entry[0];
            SceneTask *second = entry[1];
            if (first != 0 && second != 0 &&
                first->actor->priority > second->actor->priority) {
                entry[0] = second;
                swapped = 1;
                entry[1] = first;
            }
        }
    } while (swapped != 0);
}

s32 fldGetSceneGroupIndexByActorFlags(u8 *object) {
    u32 flags = ((SceneTask *)object)->actor->status.words.activeFlags & 0xE00;
    s32 result;
    switch (flags) {
    case 0x200:
        result = 1;
        break;
    case 0x400:
        result = 2;
        break;
    case 0x800:
        result = 3;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_001CFEF8", func_001D30E0);

/* Pop the front slot only when its remaining counter is zero. */
void fldCompactSceneSlots(void) {
    SceneSlot *slot = ((BattleSceneWork *)btlGetRuntime())->slots;
    u32 i;
    if (slot->remaining == 0) {
        for (i = 0; i < 7; i++) {
            slot[0].group = slot[1].group;
            slot[0].remaining = slot[1].remaining;
            slot[0].id = slot[1].id;
            slot++;
        }
        slot->group = 0;
        slot->remaining = 0;
        slot->id = 0;
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
void fldInsertSceneSlots(s32 count) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    s32 group;
    s32 used;
    s32 i;

    group = scene->variant == 1 ? 1 : 2;
    used = fldCountSceneSlots();
    if (used + count >= 8) {
        count = 8 - used;
    }
    if (count <= 0) {
        return;
    }
    for (i = used + count - 1; i != count - 1; i--) {
        scene->slots[i].group = scene->slots[i - count].group;
        scene->slots[i].remaining = scene->slots[i - count].remaining;
        scene->slots[i].id = i + 1;
    }
    for (; i != -1; i--) {
        scene->slots[i].group = group;
        scene->slots[i].remaining = 0x32;
        scene->slots[i].id = i + 1;
    }
    fldInitSceneFadeRecords();
}

/* Spend amount first; swap front groups only if the original counter survives. */
void fldSwapSceneSlots(s32 amount) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    if (scene->flags & 0x100) {
        u8 firstGroup = scene->slots[0].group;
        u8 remainingBefore = scene->slots[0].remaining;
        fldConsumeSceneSlotCounters(amount, 1);
        if (amount < remainingBefore) {
            if (scene->slots[1].group != 0 && scene->slots[1].group != firstGroup) {
                u8 group = scene->slots[0].group;
                u8 remaining = scene->slots[0].remaining;
                u8 id = scene->slots[0].id;
                scene->slots[0].group = scene->slots[1].group;
                scene->slots[0].remaining = scene->slots[1].remaining;
                scene->slots[0].id = scene->slots[1].id;
                scene->slots[1].group = group;
                scene->slots[1].remaining = remaining;
                scene->slots[1].id = id;
            }
        }
    }
}

/* Count slots with both group and counter; zero-counter groups can still block finish. */
s32 fldCountSceneSlots(void) {
    SceneSlot *slot = ((BattleSceneWork *)btlGetRuntime())->slots;
    s32 count = 0;
    u32 i;
    for (i = 0; i < 8; i++, slot++) {
        if (slot->group != 0 && slot->remaining != 0) {
            count++;
        }
    }
    return count;
}

/* Finished when no group remains, or the explicit finish flag is set. */
s32 fldAreSceneSlotsFinished(void) {
    BattleSceneWork *work = (BattleSceneWork *)btlGetRuntime();
    SceneSlot *slot;
    u32 i;
    if (work->flags & 0x1000) {
        return 1;
    }
    slot = work->slots;
    for (i = 0; i < 8; i++) {
        if (slot->group != 0) {
            return 0;
        }
        slot++;
    }
    return 1;
}

void fldInitializeSceneGroups(void) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)btlGetRuntime();
    fldSortGroupByPriority(scene->groupPrimary, 0x14);
    btlSortSceneGroupByPriorityDesc(scene->groupSecondary, 0x2d);
    btlSortSceneGroupByPriorityDesc(scene->groupTertiary, 0xf);
    func_001D30E0();
}

void btlMoveTaskToGroupTail(SceneTask *task) {
    SceneTask **group = fldGetActorSceneGroupResource(task);
    u32 count = fldClassifyActorSceneGroup(task);
    u32 last;
    u32 i;
    for (i = 0; i < count; i++, group++) {
        if (*group == task) {
            break;
        }
    }
    last = count - 1;
    for (; i < last && group[1] != 0; i++, group++) {
        *group = group[1];
    }
    *group = task;
}

void btlRotateGroupUntilTaskFirst(SceneTask *task) {
    BattleSceneWork *scene;
    u32 i;
    if (task != 0 && (task->flags & 8) != 0 && task->actor != 0 && !(task->flags & 0x40) &&
        ((u32)task->actor->status.flags & 0x200) != 0) {
        i = 0;
        scene = (BattleSceneWork *)btlGetRuntime();
        fldSortGroupByPriority(scene->groupPrimary, 0x14);
        for (; i < 0x14 && scene->groupPrimary[0] != task; i++) {
            btlMoveTaskToGroupTail(scene->groupPrimary[0]);
        }
    }
}

void fldAppendTaskToGroup(SceneTask *task) {
    SceneTask **slot = fldGetActorSceneGroupResource(task);
    SceneTask *head;
    fldClassifyActorSceneGroup(task);
    head = *slot;
    while (*slot != 0) {
        slot++;
    }
    *slot = task;
    btlRotateGroupUntilTaskFirst(head);
}

void fldAppendSceneGroupHandle(s32 handle) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    u32 *slot = scene->groupHandles;
    while (*slot != 0) {
        slot++;
    }
    *slot = handle;
}

void fldUpdateSceneGroupTask(SceneTask *task) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    SceneActor *actor;
    s32 kind;

    if ((task->flags & 0x40) == 0) {
        actor = task->actor;
        kind = (actor->status.words.activeFlags & 0x200) != 0 ? 1 : 2;
        scene->groupHandleCount++;
        if (kind == 1) {
            scene->activeGroupCount++;
        }
        if (scene->currentTask == task && scene->variant == kind) {
            scene->flags |= 8;
        }
        if ((*(u64 *)&task->flags & 0x400000100LL) == 0) {
            if (btlDoesEnabledStatusMatchCurrentId((u8 *)actor + 0x120, 0xDE) != 0) {
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

void btlRemoveTaskFromSceneGroup(SceneTask *task) {
    SceneTask **group = fldGetActorSceneGroupResource(task);
    u32 count = fldClassifyActorSceneGroup(task);
    u32 i = 0;
    u32 last;
    for (; i < count; i++) {
        if (group[i] == task) {
            group[i] = 0;
            break;
        }
    }
    last = count - 1;
    for (; i < last; i++) {
        SceneTask *current = group[i];
        SceneTask *next = group[i + 1];
        group[i + 1] = current;
        group[i] = next;
    }
}

void fldEnableSceneGroupAdvancement(void) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)btlGetRuntime();
    scene->flags = scene->flags | 0xc;
}

void fldClearSceneAdvanceFlag(void) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)btlGetRuntime();
    scene->flags = scene->flags & 0xfffffffb;
}

/* Prefer the special handle queue; otherwise return the front group's first task. */
s32 fldGetActiveSceneGroupValue(void) {
    u8 *scene = (u8 *)btlGetRuntime();
    s32 value = (s32)((BattleSceneWork *)scene)->groupHandles[0];
    if (value != 0) {
        return value;
    }
    return *(s32 *)fldGetSceneGroupResource(((BattleSceneWork *)scene)->slots[0].group);
}

/* Read one task word from the front group; an empty front slot returns zero. */
s32 fldGetSceneGroupEntry(s32 index) {
    u8 *scene = (u8 *)btlGetRuntime();
    if (((BattleSceneWork *)scene)->slots[0].group == 0) {
        return 0;
    }
    return ((s32 *)fldGetSceneGroupResource(((BattleSceneWork *)scene)->slots[0].group))[index];
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
                if (task->actor->status.words.activeFlags & 0xE0) {
                    return;
                }
                if (task->state != 2) {
                    return;
                }
                if (task->actor->status.words.activeFlags & 0x30400000) {
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

u32 fldDispatchSceneGroupRequestWhenAllowed(s32 *request) {
    u8 groupIndex;

    if (*request == 0) {
        groupIndex = (u8)request[2];
    }
    else {
        if ((*(u32 *)(*request + 8) & 0x40) != 0) {
            return 1;
        }
        groupIndex = (u8)request[2];
    }
    fldConsumeSceneSlotCounters(request[1], groupIndex);
    return 1;
}

u8 *fldCreateSceneGroupAction(u8 *actor, u32 owner, s32 groupIndex) {
    u8 group = groupIndex;
    u8 *object = (u8 *)btlAllocTask(0xC);
    u8 *fields;
    object[0] = 1;
    *(s16 *)(object + 0x20) = 0x61;
    object[0x10] = 0;
    if (actor != 0) {
        ((SceneTask *)object)->linkedOwnerId = ((SceneTask *)actor)->actor->ownerId;
    }
    ((SceneTask *)object)->onComplete = (void (*)(void))fldDispatchSceneGroupRequestWhenAllowed;
    ((SceneTask *)object)->onUpdate = 0;
    fields = (u8 *)btlGetTaskArguments(object);
    *(u32 *)(fields + 0) = (u32)actor;
    *(u32 *)(fields + 4) = owner;
    fields[8] = group;
    return object;
}

void fldStopSceneActorActionUpdate(void) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)btlGetRuntime();
    scene->flags = scene->flags & 0xfffffffb;
}

s32 fldActivateRequestedSceneActor(u32 *request) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    u32 *actor = (u32 *)request[0];
    scene->flags |= 4;
    if (actor != 0 && (actor[2] & 0x40) != 0) {
        return 1;
    }
    fldInsertSceneSlots(request[1]);
    return 1;
}

u8 *fldCreateSceneActorAction(u8 *actor, u32 owner) {
    u8 *object = (u8 *)btlAllocTask(8);
    u32 *fields;
    object[0] = 1;
    *(s16 *)(object + 0x20) = 0x62;
    object[0x10] = 0;
    if (actor != 0) {
        ((SceneTask *)object)->linkedOwnerId = ((SceneTask *)actor)->actor->ownerId;
    }
    ((SceneTask *)object)->onUpdate = (void (*)(void))fldStopSceneActorActionUpdate;
    ((SceneTask *)object)->onComplete = (void (*)(void))fldActivateRequestedSceneActor;
    fields = (u32 *)btlGetTaskArguments(object);
    fields[0] = (u32)actor;
    fields[1] = owner;
    return object;
}

u32 fldApplySceneSlotSwapRequest(u32 *slotIndex) {
    fldSwapSceneSlots(*slotIndex);
    return 1;
}

u8 *fldCreateActorAction(s32 owner) {
    u8 *object = (u8 *)btlAllocTask(4);
    object[0] = 1;
    *(s16 *)(object + 0x20) = 0x63;
    ((SceneTask *)object)->onComplete = (void (*)(void))fldApplySceneSlotSwapRequest;
    object[0x10] = 0;
    ((SceneTask *)object)->onUpdate = 0;
    *(u32 *)btlGetTaskArguments(object) = owner;
    return object;
}

void btlSetSceneTaskActiveFlag(s32 task) {
    ((SceneTask *)task)->flags |= 1;
}

void btlClearSceneTaskActiveFlag(s32 task) {
    ((SceneTask *)task)->flags &= ~1;
}

void btlBindActorTaskAndSelectActionNumber(s32 task, s32 actor) {
    u32 flags;

    flags = ((SceneActor *)actor)->status.words.activeFlags;
    ((SceneTask *)task)->actor = (SceneActor *)actor;
    if ((flags & 0x400) != 0) {
        if (0x17f < ((SceneActor *)actor)->kind) {
            flags = ((SceneTask *)task)->flags;
            goto store;
        }
        ((SceneTask *)task)->actionNumber =
                  (u16)*(u8 *)(((u32)((SceneActor *)actor)->kind * 0x14 -
                                                      (u32)((SceneActor *)actor)->kind) * 4 + datEnemyRecords + 0x15);
    }
    flags = ((SceneTask *)task)->flags;
store:
    ((SceneTask *)task)->flags = flags | 8;
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

