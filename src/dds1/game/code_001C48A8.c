#include "common.h"
#include "pcp_vu0.h"

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
#define FLD_SCENE_COUNTER_ACTION_ID 0x5C
#define FLD_SCENE_INSERT_ACTION_ID 0x5D
#define FLD_SCENE_SWAP_ACTION_ID 0x5E
#define FLD_SCENE_TASK_FLAGS_OFFSET 8
#define FLD_SCENE_FLAGS_OFFSET 0x1F4
#define FLD_SCENE_GROUP_WORD_BYTES 4

typedef struct ActorEntrySlot {
    s16 code;
    s16 unk02;
    s16 countdown;
} ActorEntrySlot;

typedef struct UiObject {
    u8 unk_00[0x110];
    u32 flags;
    u32 actionFlags;
    u8 unk_118[8];
    u16 entryMask;
    u8 entryDataTail[2];
    u16 index;
    u16 currentValue;
    u16 maximumValue;
    u8 unk_12A[4];
    u16 statusFlags;
    u8 unk_130[6];
    u8 unk_136[0x18E];
    u8 kind;
    u8 pad_2C5;
    ActorEntrySlot entrySlots[7];
    s32 selectedEntryIndex;
    u32 marker;
    u8 pad_2F8[0x4C];
    struct UiObject *next;
} UiObject;

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

/* Dispatch-list node, separate from the unit-data actor list. */
typedef struct SceneLinkedNode {
    u32 state;
    u8 pad04[4];
    u32 flags;
    u8 pad0C[0xC];
    UiObject *actor;
    u8 pad1C[0x150];
    struct SceneLinkedNode *next; /* 0x16C */
} SceneLinkedNode;


/* Actor-task prefix; the separate unit-data list uses UiObject. */
typedef struct SceneTask {
    s32 state;
    u8 pad04[4];
    u32 flags;
    u8 pad0C[0xC];
    UiObject *actor;
    u8 pad1C[0x24];
    u64 owner; /* 0x40: identifier used by task-owner queries */
} SceneTask;

/* Primary scene controller; loading and group scheduling share this record. */
typedef struct BattleSceneWork {
    u8 pad_000[0x1F4];
    u32 flags;
    u32 subFlags;              /* 0x1F8 */
    u32 refreshFlags;          /* 0x1FC */
    u8 pad200[8];
    s32 currentScene;          /* 0x208 */
    s32 queuedScene;           /* 0x20C */
    s32 frame;                 /* 0x210 */
    s32 sceneState;            /* 0x214 */
    u8 pad218[4];
    s32 scriptState;           /* 0x21C */
    s32 scriptArg;             /* 0x220 */
    SceneLinkedNode *linkedNodes; /* 0x224 */
    UiObject *actors;
    u8 pad_22C[0x20];
    u16 variant;
    u8 pad_24E[2];
    s32 step;
    u8 pad254[4];
    u8 unk258;
    u8 pad259[0x23];
    s32 mode;
    u8 pad280[8];
    s16 tileX;                 /* 0x288 */
    s16 tileY;                 /* 0x28A */
    s32 loadStep;              /* 0x28C */
    u8 pad290[0xC];
    s32 taskParent;
    u8 pad_2A0[0xC];
    s32 spriteObject;
    u8 pad2B0[0x20];
    s32 unk2D0;
    SceneSlot slots[8];
    SceneTask *groupPrimary[20];
    SceneTask *groupSecondary[45];
    SceneTask *groupTertiary[15];
    SceneTask *groupHandles[8];
    SceneFadingRecord fading[8]; /* 0x44C */
    SceneTask *currentTask;
    u8 pad_490[0x108];
    s32 (*selectScriptArg)(void); /* 0x598 */
    u8 pad59C[0x14];
    s32 (*sceneCallback)();
    u8 pad_5B4[0x40];
    s32 (*selectScriptState)(void); /* 0x5F4 */
    void (*completionHook)(void);
} BattleSceneWork;

extern s32 btlCountTasksForOwner(s64);
extern s64 btlStartTask(void *);
extern void btlDispatchStateHandler(void *, s32);

extern void func_001BCB88(s32, s32);

extern void btlUpdateScene(void);

extern s8 D_00324530[];


extern void func_00215FE0(s32);

u8 *fldCreateSceneGroupAction(u8 *, u32, s32);

extern s32 datEnemyRecords;

extern s32 btlGetRuntime(void);

extern u8 *datBattleSceneRecords;

extern void btlResetToInitialScene(void);

extern void fldClearSceneSlotsAndGroups(void);

extern u8 *datBattleSceneRecords;

extern u8 *datBattleSceneRecords;

extern u8 *datBattleSceneRecords;

extern u8 *datBattleSceneRecords;

u32 btlHasRegisteredGuidePanelTask(void);

u32 func_001A29D0(s32 arg0, s32 arg1);

void func_001AD5A0(void);

void func_001C48A8(void) {
}

u32 func_001C48B0(void) {
    return 0;
}

void fldBattleSceneEnterInit(u8 *arg0) {
    u32 id = *(u32 *)(arg0 + 0x27C);
    if (id < 0x400 && (*(u16 *)(datBattleSceneRecords + id * 40 + 0x20) & 0x8000) != 0) {
        *(u32 *)(arg0 + 0x1F8) |= 8;
        kwlnFadeInStart(0xFF, 0xFF, 0xFF, 0);
    }
    if (func_001A99B0() != 0) {
        *(u8 *)(arg0 + 0x24A) = 2;
    } else {
        *(u8 *)(arg0 + 0x24A) = 0;
    }
    func_0020FF50();
    func_0020ED90(*(u32 *)(arg0 + 0x27C));
    func_001F3278(*(u32 *)(arg0 + 0x270), *(u32 *)(arg0 + 0x27C));
    VU0_STORE_VF($vf0, arg0);
}

s32 btlLoadBankWhenTasksIdle(void) {
    if (sndIsStreamStatusTwoOrThree() != 0 &&
        btlIsEventSequenceTaskReady() != 0 &&
        func_00213B60() != 0) {
        sndLoadBattleBank();
        func_002101C8();
        return 3;
    }
    return 0;
}

void fldMarkGridTiles(u8 *context) {
    SceneTask *node = (SceneTask *)fldCreateSceneTileTask(*(s16 *)(context + 0x288), *(s16 *)(context + 0x28A));
    node->owner = 0x8000000000000001ULL;
    btlStartTask(node);
    btlStartTask(btlCreateFloorLoadTask(*(s16 *)(context + 0x288), *(s16 *)(context + 0x28A)));
}

extern void btlResetTitleStreamOnBattleFlag(void);

extern void func_001DC0E8(void);

extern void btlAdvanceWorldCounterAndSpawnActionObject(void);

extern void btlCreateRainEffect(s16, s16);

s32 btlStartOwnerTaskIfClear(u8 *object) {
    if (btlCountTasksForOwner(0x8000000000000001ULL) == 0) {
        btlResetTitleStreamOnBattleFlag();
        func_001DC0E8();
        btlAdvanceWorldCounterAndSpawnActionObject();
        btlCreateRainEffect(*(s16 *)(object + 0x288), *(s16 *)(object + 0x28A));
        return 4;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001C48A8", func_001C4A80);

extern void btlRepositionPartyAroundBattleCenter(void);

extern s32 func_001A3638(void);

extern void func_001A57A0(void);

extern void kwlnFadeBackgroundStartOut(s32);

extern void kwlnDrawSetOverlayTransition(s32, s32, s32);

extern void kwlnDrawEnableD88(s32);

extern void kwlnDrawEnableDc8(s32);

extern void kwlnDrawEnableE08(s32);

extern void kwlnDrawSetupC70B(s32);

extern void kwlnDrawEnableCd0(s32);

extern void kwlnDrawEnableD30(s32);

extern void btlMarkRuntimeUpdatePending(void);

extern void kwlnFadeStartIn(s32);

extern s32 fldGetEncounterRuntimeResult(void);

extern void fldSetEncounterPendingValue(s32);

extern void evtSetSolarOverlayFullyVisible(void);

extern s64 func_001F53F0(void);

extern void fldCreateSceneCleanupTask(void);

s32 fldSceneStateRestoreDisplay(u8 *scene) {
    if (btlCountTasksForOwner(0x8000000000000002ULL) == 0) {
        func_001F53F0();
        if (!(*(u32 *)(scene + 0x1F4) & 0x4000)) {
            btlRepositionPartyAroundBattleCenter();
        }
        func_001A3638();
        if (*(u32 *)(scene + 0x1F8) & 0x80) {
            func_001A57A0();
            *(u32 *)(scene + 0x1F8) &= ~0x80;
        }
        if (!(*(u32 *)(scene + 0x1F4) & 0x4000)) {
            kwlnFadeBackgroundStartOut(0);
            kwlnDrawSetOverlayTransition(0, 0, 1);
            kwlnDrawEnableD88(0);
            kwlnDrawEnableDc8(0);
            kwlnDrawEnableE08(0);
            kwlnDrawSetupC70B(0);
            kwlnDrawEnableCd0(0);
            kwlnDrawEnableD30(0);
            btlMarkRuntimeUpdatePending();
            if (!(*(u32 *)(scene + 0x1F8) & 8)) {
                kwlnFadeStartIn(0);
            }
            if (fldGetEncounterRuntimeResult() != 0) {
                fldSetEncounterPendingValue(1);
            }
            evtSetSolarOverlayFullyVisible();
        }
        if (*(s32 *)(scene + 0x2B0) == 0) {
            fldCreateSceneCleanupTask();
            if (!(*(u32 *)(scene + 0x1F8) & 0x2000)) {
                *(u32 *)(scene + 0x1F4) |= 0x100000;
            } else {
                *(u32 *)(scene + 0x1F8) &= ~0x2000;
                *(u32 *)(scene + 0x1F4) &= ~0x100000;
            }
        }
        return 5;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001C48A8", func_001C50C0);

extern s32 func_00213B90(void);

extern s32 btlAreWorkBuffersReady(void);

s32 btlInitializeSceneAfterTasksAndBuffersReady(u8 *arg0) {
    u32 i;

    if (btlCountTasksForOwner(0x8000000000000003ULL) == 0 &&
        btlAreWorkBuffersReady() != 0 &&
        func_00213B90() != 0) {
        fldInitializeSceneGroups();
        func_001AC7D8();
        *(u8 *)(arg0 + 0x258) = 0;
        func_001A9780();
        if (*(u8 *)(datBattleSceneRecords + *(s32 *)(arg0 + 0x27C) * 40 + 1) != 0) {
            for (i = 0; i < *(u8 *)(datBattleSceneRecords + *(s32 *)(arg0 + 0x27C) * 40 + 2); i++) {
                func_001A4240(*(u8 *)(datBattleSceneRecords + *(s32 *)(arg0 + 0x27C) * 40 + 1));
            }
        }
        return 8;
    }
    return 0;
}

void btlConsumeSceneAdvanceFlags(void) {
}

s32 fldConsumeSceneInputFlags(s32 scene) {
    u32 flags = *(u32 *)(scene + 0x1F4);
    s32 result;

    if (flags & 0x800) {
        fldClearSceneAdvanceFlag();
        result = 8;
    } else if (flags & 0x400) {
        fldClearSceneAdvanceFlag();
        result = 7;
    } else {
        return 0;
    }
    *(u32 *)(scene + 0x1F4) |= 0x20;
    return result;
}

extern void func_001AC7D8();

void fldAdvanceSceneVariant(BattleSceneWork *scene) {
    s32 notFirst = scene->variant != 1;
    scene->variant = 2 - notFirst;
    if (scene->sceneCallback != 0) {
        s32 variant = scene->sceneCallback();
        if (variant != -1) {
            scene->variant = variant;
        }
    }
    scene->step = scene->step + 1;
    fldInitializeSceneGroups();
    func_001AC7D8();
}

typedef struct SceneEffectRequest {
    u8 startKind;
    u8 pad01[7];
    u16 taskId;
} SceneEffectRequest;

extern void btlTickActorEntryCountdowns(u8 *);
extern void btlClearNodeFlags(void);
extern void btlClearActorSelectedEntryIndex(UiObject *);
extern void btlRefreshUnitMotionSelection(u8 *);
extern SceneEffectRequest *btlCreateEffObjC(UiObject *, s32);

s32 btlAdvanceSceneWhenActorTasksReady(BattleSceneWork *scene) {
    s32 ready = 1;
    u32 group = scene->variant == 1 ? FLD_SCENE_ACTOR_PRIMARY_BIT : FLD_SCENE_ACTOR_SECONDARY_BIT;
    SceneLinkedNode *head = scene->linkedNodes;
    SceneLinkedNode *node;
    UiObject *actor;

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
            if ((actor->flags & group) && (actor->flags & FLD_SCENE_TASK_ACTIVE_BIT) &&
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
                        if (actor->statusFlags & 0x122F) {
                            btlDispatchStateHandler(node, 4);
                        }
                        if (actor->flags & group) {
                            btlClearNodeFlags();
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
            UiObject *selected = NULL;
            s32 message;
            SceneEffectRequest *request;
            u16 status;

            for (actor = scene->actors; actor != NULL; actor = actor->next) {
                if (actor->flags & FLD_SCENE_TASK_ACTIVE_BIT) {
                    if (actor->flags & FLD_SCENE_ACTOR_SECONDARY_BIT) {
                        total++;
                        status = actor->statusFlags & 1;
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
                request->taskId = 0x2B;
                btlStartTask(request);
            }
            scene->subFlags &= ~FLD_SCENE_COUNTERS_ENABLED_BIT;
        }
        return 8;
    }
    return 0;
}

extern void fldClearSceneAdvanceFlag(void);
extern s32 btlCanStartPrimaryScriptTask(void);
extern s32 btlHasScriptResource(void);
extern void btlStartPrimaryScriptTask(void);
extern void btlStartSecondaryScriptTask(void);
extern void btlStartSkillEventTask(s32);
extern struct SoundTask *btlCreateCommandSoundUpdateTask(void);
extern u8 *btlCreateSecondaryCommandSoundTask(void);
extern void *btlCreateCommandSoundTask(s32, s32);
extern void evtBeginSolarOverlayFadeOut(s32);
extern s32 fldGetActiveSceneGroupValue(void);
extern u32 kwlnDrawControlFlags;

void func_001C5B90(BattleSceneWork *scene) {
    scene->scriptState = -1;
    fldClearSceneAdvanceFlag();
    scene->flags |= 0x20;
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
        btlStartTask(btlCreateCommandSoundTask(0, 3));
        btlStartPrimaryScriptTask();
        return;
    case 0xF000003:
        evtBeginSolarOverlayFadeOut(8);
        btlStartSecondaryScriptTask();
        kwlnDrawControlFlags &= 0xDFFFFFFF;
        return;
    case 0xF000000: {
        SceneTask *task = (SceneTask *)fldGetActiveSceneGroupValue();

        if (task != NULL) {
            btlStartTask(btlCreateCommandSoundUpdateTask());
            btlStartTask(btlCreateSecondaryCommandSoundTask());
            if (task->actor->flags & FLD_SCENE_ACTOR_PRIMARY_BIT) {
                btlStartTask(btlCreateCommandSoundTask((s32)task, 9));
            } else {
                btlStartTask(btlCreateCommandSoundTask((s32)task, 3));
            }
        }
        func_001BCB88(0, 20);
        return;
    }
    default:
        if (scene->scriptState != -1) {
            btlStartTask(btlCreateCommandSoundUpdateTask());
            btlStartTask(btlCreateCommandSoundTask(0, 3));
            btlStartSkillEventTask(scene->scriptState);
        }
        break;
    }
}

s32 fldSceneStateWaitScriptRelease(u8 *arg0) {
    s32 finished = 1;
    s32 state = *(s32 *)(arg0 + 0x21C);

    switch (state) {
    case 0xF000002:
        if (btlReleaseScriptResourceA() == 0) {
            finished = 0;
        } else {
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
        if (*(s32 *)(arg0 + 0x210) == 0x14) {
            btlCreateGuidePanelTask(*(s32 *)(arg0 + 0x4A0), *(s32 *)(arg0 + 0x220));
        } else if (*(s32 *)(arg0 + 0x210) >= 0x2D) {
            if (btlHasRegisteredGuidePanelTask() != 0) {
                if (D_00324530[1] < 0) {
                    func_001AD5A0();
                    func_001BCB88(1, 0x14);
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
        if ((*(u32 *)(arg0 + 0x1F4) & 0x800) == 0) {
            fldEnableSceneGroupAdvancement();
            *(u32 *)(arg0 + 0x1F4) &= ~0x400;
            *(u32 *)(arg0 + 0x1F4) &= ~0x1000;
            *(u32 *)(arg0 + 0x1F4) &= ~0x20;
            return 6;
        }
        return 9;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001C48A8", func_001C5F80);

INCLUDE_ASM(const s32, "game/code_001C48A8", func_001C6888);

typedef struct PartyDeltaEntry {
    u16 flags;
    u8 pad_002[6];
    u16 weight;
    u8 pad_00A[4];
    u16 mask;
    u8 pad_010[0x188];
    s32 link;
    u8 pad_19C[8];
} PartyDeltaEntry;

typedef struct PartyDeltaState {
    u8 pad_000[0xA60];
    PartyDeltaEntry entry[5];
} PartyDeltaState;


extern PartyDeltaState *datGameState;

extern void itfMesClearFlags(s32);

extern void brsTaskAllowUpdate(void);

extern void evtBeginSolarOverlayFadeOut(s32);

extern void func_001A1960();

extern s32 mnuIsTitleEntryAvailable();

extern void datAdjustCurrentHp();

/* Apply the gated five-percent party delta, then restart the field-load step. */
void btlApplyPartyEntryWeightedDelta(BattleSceneWork *scene) {
    u32 i;
    f32 scale;

    i = 0;
    itfMesClearFlags(1);
    scale = 0.05f;
    brsTaskAllowUpdate();
    evtBeginSolarOverlayFadeOut(8);
    do {
        func_001A1960(&datGameState->entry[i], -0x45D1);
        if (!(scene->subFlags & 0x40)) {
            if (datGameState->entry[i].flags & 2) {
                if (scene->unk258 == 1) {
                    if (datGameState->entry[i].link != 0 || scene->unk2D0 != 0) {
                        if (!(datGameState->entry[i].mask & 0x40)) {
                            if (mnuIsTitleEntryAvailable(&datGameState->entry[i]) == 0) {
                                datAdjustCurrentHp(&datGameState->entry[i],
                                               (s32)((f32)datGameState->entry[i].weight * scale));
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

extern s32 func_002629A8();


extern s32 fldLoadAreaResource();

extern void btlReleaseEventAssets();

extern void btlReleaseBossData();

extern void fldPollAreaResourceLoad();

extern s32 fldGetResourceReadyFlag();

extern void btlBossDebugPrintf(const char *format, ...);

extern s32 brsTaskPollDone();

/* Drive field loading or dispatch linked nodes to state 30; return 0xB while pending. */
u32 fldStepAreaLoad(BattleSceneWork *work) {
    SceneLinkedNode *node;
    s32 step;

    if (func_002629A8() == 1) {
        work->subFlags |= 0x20;
        if (work->linkedNodes != 0) {
            for (node = work->linkedNodes; node != 0; node = node->next) {
                if (node->state != 0x1E) {
                    btlDispatchStateHandler(node, 0x1E);
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

void fldMarkLinkedSceneActors(s32 context) {
    SceneLinkedNode *entry;
    btlAdvanceTitleStateWithAudioCleanup(context);
    entry = ((BattleSceneWork *)context)->linkedNodes;
    while (entry != 0) {
        if (entry->state != 30) {
            btlDispatchStateHandler(entry, 30);
        }
        entry = entry->next;
    }
    btlFlagTasksForUpdate();
}

INCLUDE_ASM(const s32, "game/code_001C48A8", func_001C7028);

void fldMarkSceneRefresh(s32 scene) {
    func_00215FE0(scene);
    ((BattleSceneWork *)scene)->refreshFlags |= 4;
}

extern s32 func_00215FF8(void);

extern void kwlnFadeInStart(s32, s32, s32, s32);

u32 fldBeginFadeWhenSceneReady(void) {
    if (func_00215FF8() != 0) {
        kwlnFadeInStart(0, 0, 0, 0);
        return 2;
    }
    return 0;
}

typedef struct {
    void (*initialize)(s32);
    s32 (*update)(s32);
    s32 flags;
} SceneInitializer;

extern SceneInitializer D_00359A88[];

/* Select a scene and reset its frame/state before invoking its initializer. */
void btlSetScene(s32 sceneId) {
    s32 scene = btlGetRuntime();

    ((BattleSceneWork *)scene)->currentScene = sceneId;
    ((BattleSceneWork *)scene)->frame = 0;
    ((BattleSceneWork *)scene)->sceneState = 0;
    D_00359A88[sceneId].initialize(scene);
}

/* Queue a transition for the next scene update. */
void btlQueueScene(u32 sceneId) {
    s32 scene;

    scene = btlGetRuntime();
    ((BattleSceneWork *)scene)->queuedScene = sceneId;
}

extern void btlSetScene(s32);


/* Consume a queued transition, run the current updater, and advance its frame. */
void btlUpdateScene(void) {
    s32 scene = btlGetRuntime();
    s32 queuedSceneId = ((BattleSceneWork *)scene)->queuedScene;
    s32 updateResult;
    if (queuedSceneId != 0) {
        btlSetScene(queuedSceneId);
        ((BattleSceneWork *)scene)->queuedScene = 0;
    }
    updateResult = D_00359A88[((BattleSceneWork *)scene)->currentScene].update(scene);
    if (updateResult != 0) {
        btlQueueScene(updateResult);
    }
    ((BattleSceneWork *)scene)->frame += 1;
}

/* Enter the initial scene and discard any queued transition. */
void btlResetToInitialScene(void) {
    s32 scene;

    scene = btlGetRuntime();
    btlSetScene(FLD_SCENE_INITIAL_ID);
    ((BattleSceneWork *)scene)->queuedScene = 0;
}

void btlGetCurrentSceneRecordValue(void) {
}

/* Return the current initializer row's flags word. */
s32 fldGetSceneDescriptorProperty(void) {
    s32 sceneId;

    sceneId = ((BattleSceneWork *)btlGetRuntime())->currentScene;
    return D_00359A88[sceneId].flags;
}

/* Select the task's group array; the handle-group bit takes precedence. */
s32 *fldGetActorSceneGroupResource(s32 *task) {
    s32 scene = btlGetRuntime();
    s32 *groupEntries = (s32 *)((BattleSceneWork *)scene)->groupSecondary;
    u32 actorGroupBits;
    if (((SceneTask *)task)->flags & FLD_SCENE_TASK_HANDLE_GROUP_BIT) {
        return (s32 *)((BattleSceneWork *)scene)->groupHandles;
    }
    actorGroupBits = ((SceneTask *)task)->actor->flags & FLD_SCENE_ACTOR_GROUP_MASK;
    switch (actorGroupBits) {
    case FLD_SCENE_ACTOR_PRIMARY_BIT:
        groupEntries = (s32 *)((BattleSceneWork *)scene)->groupPrimary;
        break;
    case FLD_SCENE_ACTOR_SECONDARY_BIT:
        break;
    case FLD_SCENE_ACTOR_TERTIARY_BIT:
        groupEntries = (s32 *)((BattleSceneWork *)scene)->groupTertiary;
        break;
    default:
        groupEntries = 0;
        break;
    }
    return groupEntries;
}

/* Resolve one of the three group IDs; all other IDs return a null address. */
s32 fldGetSceneGroupResource(u8 groupId) {
    s32 scene = btlGetRuntime();
    s32 groupEntries;

    switch (groupId) {
    case FLD_SCENE_GROUP_PRIMARY_ID:
        groupEntries = (s32)((BattleSceneWork *)scene)->groupPrimary;
        break;
    case FLD_SCENE_GROUP_SECONDARY_ID:
        groupEntries = (s32)((BattleSceneWork *)scene)->groupSecondary;
        break;
    case FLD_SCENE_GROUP_TERTIARY_ID:
        groupEntries = (s32)((BattleSceneWork *)scene)->groupTertiary;
        break;
    default:
        groupEntries = 0;
        break;
    }
    return groupEntries;
}

/* Return the selected group's capacity, not its ID; keep the native runtime call. */
s32 fldClassifyActorSceneGroup(s32 *task) {
    u32 actorGroupBits;
    btlGetRuntime();
    if (((SceneTask *)task)->flags & FLD_SCENE_TASK_HANDLE_GROUP_BIT) {
        return FLD_SCENE_HANDLE_TASK_COUNT;
    }
    actorGroupBits = ((SceneTask *)task)->actor->flags & FLD_SCENE_ACTOR_GROUP_MASK;
    switch (actorGroupBits) {
    case FLD_SCENE_ACTOR_PRIMARY_BIT: return FLD_SCENE_PRIMARY_TASK_COUNT;
    case FLD_SCENE_ACTOR_SECONDARY_BIT: return FLD_SCENE_SECONDARY_TASK_COUNT;
    case FLD_SCENE_ACTOR_TERTIARY_BIT: return FLD_SCENE_TERTIARY_TASK_COUNT;
    default: return 0;
    }
}

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
                func_001A29D0((s32)first->actor + 0x120, 3) < func_001A29D0((s32)second->actor + 0x120, 3)) {
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
                *(u8 *)((s32)first->actor + 0x11C) > *(u8 *)((s32)second->actor + 0x11C)) {
                pairCursor[0] = second;
                swapped = 1;
                pairCursor[1] = first;
            }
        }
    } while (swapped != 0);
}

/* Map an exact actor-group bit pattern to its group ID; combined bits return zero. */
s32 fldGetSceneGroupIndexByActorFlags(u8 *task) {
    u32 actorGroupBits = *(u32 *)(*(u8 **)(task + 0x18) + 0x110) & FLD_SCENE_ACTOR_GROUP_MASK;
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

INCLUDE_ASM(const s32, "game/code_001C48A8", func_001C7690);

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
    s32 scene = btlGetRuntime();
    SceneSlot *slotCursor;
    u32 slotIndex;
    if (((BattleSceneWork *)scene)->flags & FLD_SCENE_SLOTS_FINISHED_BIT) {
        return 1;
    }
    slotCursor = ((BattleSceneWork *)scene)->slots;
    for (slotIndex = 0; slotIndex < FLD_SCENE_SLOT_COUNT; slotIndex++, slotCursor++) {
        if (slotCursor->group != 0) {
            return 0;
        }
    }
    return 1;
}

/* Sort the three bounded task groups before the native slot initialization call. */
void fldInitializeSceneGroups(void) {
    s32 scene;

    scene = btlGetRuntime();
    fldSortGroupByPriority(((BattleSceneWork *)scene)->groupPrimary, FLD_SCENE_PRIMARY_TASK_COUNT);
    btlSortSceneGroupByPriorityDesc(((BattleSceneWork *)scene)->groupSecondary, FLD_SCENE_SECONDARY_TASK_COUNT);
    btlSortSceneGroupByPriorityDesc(((BattleSceneWork *)scene)->groupTertiary, FLD_SCENE_TERTIARY_TASK_COUNT);
    func_001C7690();
}

/* Move a member behind its occupied successors; native membership/bounds are unchecked. */
void btlMoveTaskToGroupTail(SceneTask *task) {
    SceneTask **groupCursor = (SceneTask **)fldGetActorSceneGroupResource((s32 *)task);
    u32 groupCapacity = fldClassifyActorSceneGroup((s32 *)task);
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

    if (task != 0 && (task->flags & FLD_SCENE_TASK_BOUND_BIT) != 0 && task->actor != 0 &&
        (task->flags & FLD_SCENE_TASK_HANDLE_GROUP_BIT) == 0 && (task->actor->flags & FLD_SCENE_ACTOR_PRIMARY_BIT) != 0) {
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
    SceneTask **groupCursor = (SceneTask **)fldGetActorSceneGroupResource((s32 *)task);
    SceneTask *previousHead;
    fldClassifyActorSceneGroup((s32 *)task);
    previousHead = *groupCursor;
    while (*groupCursor != 0) {
        groupCursor++;
    }
    *groupCursor = task;
    btlRotateGroupUntilTaskFirst(previousHead);
}

/* Append a handle to the first zero entry; the caller must leave room. */
void fldAppendSceneGroupHandle(s32 handle) {
    s32 *handleCursor = (s32 *)((BattleSceneWork *)btlGetRuntime())->groupHandles;
    while (*handleCursor != 0) {
        handleCursor++;
    }
    *handleCursor = handle;
}

extern void btlRemoveTaskFromSceneGroup(SceneTask *task);

void fldUpdateSceneGroupTask(s32 taskValue) {
    SceneTask *task = (SceneTask *)taskValue;
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    UiObject *actor;
    s32 kind;

    if ((task->flags & 0x40) == 0) {
        actor = task->actor;
        kind = (actor->flags & 0x200) != 0 ? 1 : 2;
        if (task == *(SceneTask **)fldGetActorSceneGroupResource((s32 *)task) &&
            scene->variant == kind) {
            scene->flags |= 8;
        }
        if (scene->flags & 0x40) {
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
    SceneTask **groupEntries = (SceneTask **)fldGetActorSceneGroupResource((s32 *)task);
    u32 groupCapacity = fldClassifyActorSceneGroup((s32 *)task);
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
    s32 scene;

    scene = btlGetRuntime();
    ((BattleSceneWork *)scene)->flags |= FLD_SCENE_ADVANCE_ENABLE_BITS;
}

/* Clear only the advancement bit, retaining the unsigned native mask. */
void fldClearSceneAdvanceFlag(void) {
    s32 scene;

    scene = btlGetRuntime();
    ((BattleSceneWork *)scene)->flags &= FLD_SCENE_ADVANCE_CLEAR_MASK;
}

/* Prefer the special handle queue; otherwise return the front group's first task. */
s32 fldGetActiveSceneGroupValue(void) {
    s32 scene;
    s32 headValue;
    u32 groupAddress;

    scene = btlGetRuntime();
    headValue = (s32)((BattleSceneWork *)scene)->groupHandles[0];
    if (headValue != 0) {
        return headValue;
    }
    groupAddress = fldGetSceneGroupResource(((BattleSceneWork *)scene)->slots[0].group);
    return *(s32 *)groupAddress;
}

/* Read one task word from the front group; an empty front slot returns zero. */
s32 fldGetSceneGroupEntry(s32 entryIndex) {
    s32 scene = btlGetRuntime();

    if (((BattleSceneWork *)scene)->slots[0].group == 0) {
        return 0;
    }
    return *(s32 *)(fldGetSceneGroupResource(((BattleSceneWork *)scene)->slots[0].group) + entryIndex * FLD_SCENE_GROUP_WORD_BYTES);
}

extern s32 func_001A8188(void);

/* Advance one scene group task after both scene gates have opened. */
void func_001C8330(void) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    SceneTask *task;

    if ((scene->flags & 4) != 0) {
        if ((scene->flags & 8) != 0) {
            if ((scene->flags & 0x800) != 0) {
                return;
            }
            if ((scene->flags & 0x400) != 0) {
                return;
            }
            if (func_001A8188() != 0) {
                scene->flags |= 0x800;
                return;
            }
            if (scene->completionHook != 0) {
                scene->completionHook();
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
            } else if (fldAreSceneSlotsFinished() != 0) {
                scene->flags |= 0x400;
            } else {
                task = *(SceneTask **)fldGetSceneGroupResource(scene->slots[0].group);
                if (task == 0 || (task->actor->flags & 0xE0) != 0 || task->state != 2 ||
                    (task->actor->flags & 0x30400000) != 0) {
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
        *(u64 *)(actionTask + 0x40) = *(u64 *)(*(u8 **)(actorTask + 0x18) + 0x108);
    }
    *(u32 *)(actionTask + 0x4C) = (u32)fldDispatchSceneGroupRequestWhenAllowed;
    *(u32 *)(actionTask + 0x48) = 0;
    requestData = (u8 *)btlGetTaskArguments(actionTask);
    *(u32 *)(requestData + 0) = (u32)actorTask;
    *(u32 *)(requestData + 4) = counterAmount;
    requestData[8] = counterModeByte;
    return actionTask;
}

/* Stop slot-insert advancement without changing other scene flags. */
void fldStopSceneActorActionUpdate(void) {
    s32 scene;

    scene = btlGetRuntime();
    *(u32 *)(scene + FLD_SCENE_FLAGS_OFFSET) = *(u32 *)(scene + FLD_SCENE_FLAGS_OFFSET) & FLD_SCENE_ADVANCE_CLEAR_MASK;
}

/* Enable advancement before checking the source task, then insert its requested slots.
 * A handle-group task skips insertion but still leaves advancement enabled. */
u32 fldActivateRequestedSceneActor(s32 *request) {
    s32 scene = btlGetRuntime();
    s32 actorTask = request[0];

    *(u32 *)(scene + FLD_SCENE_FLAGS_OFFSET) |= FLD_SCENE_ADVANCE_BIT;
    if (actorTask != 0 && (*(u32 *)(actorTask + FLD_SCENE_TASK_FLAGS_OFFSET) & FLD_SCENE_TASK_HANDLE_GROUP_BIT) != 0) {
        return 1;
    }
    fldInsertSceneSlots(request[1]);
    return 1;
}

extern u32 fldActivateRequestedSceneActor(s32 *);

/* Allocate a slot-insert request; its update callback clears advancement. */
u8 *fldCreateSceneActorAction(u8 *actorTask, s32 slotCount) {
    u8 *actionTask = (u8 *)btlAllocTask(FLD_SCENE_INSERT_REQUEST_BYTES);
    u8 *requestData;
    actionTask[0] = 1;
    *(u16 *)(actionTask + 0x20) = FLD_SCENE_INSERT_ACTION_ID;
    actionTask[0x10] = 0;
    if (actorTask != 0) {
        *(u64 *)(actionTask + 0x40) = *(u64 *)(*(u8 **)(actorTask + 0x18) + 0x108);
    }
    *(u32 *)(actionTask + 0x48) = (u32)fldStopSceneActorActionUpdate;
    *(u32 *)(actionTask + 0x4C) = (u32)fldActivateRequestedSceneActor;
    requestData = (u8 *)btlGetTaskArguments(actionTask);
    *(u32 *)requestData = (u32)actorTask;
    *(s32 *)(requestData + 4) = slotCount;
    return actionTask;
}

/* Spend the request's counter amount and conditionally swap the front slots. */
u32 fldApplySceneSlotSwapRequest(u32 *request) {
    fldSwapSceneSlots(*request);
    return 1;
}

/* Allocate a one-word counter-spend/swap request, without an actor-owner link. */
s32 *fldCreateActorAction(s32 counterAmount) {
    u8 *actionTask = (u8 *)btlAllocTask(FLD_SCENE_SWAP_REQUEST_BYTES);
    *actionTask = 1;
    *(u16 *)(actionTask + 0x20) = FLD_SCENE_SWAP_ACTION_ID;
    *(u32 *)(actionTask + 0x4C) = (u32)fldApplySceneSlotSwapRequest;
    actionTask[0x10] = 0;
    *(u32 *)(actionTask + 0x48) = 0;
    *(s32 *)btlGetTaskArguments(actionTask) = counterAmount;
    return (s32 *)actionTask;
}

/* Set the task's active bit without changing its other flags. */
void btlSetSceneTaskActiveFlag(s32 taskAddress) {
    *(u32 *)(taskAddress + FLD_SCENE_TASK_FLAGS_OFFSET) = *(u32 *)(taskAddress + FLD_SCENE_TASK_FLAGS_OFFSET) | FLD_SCENE_TASK_ACTIVE_BIT;
}

/* Clear only the task's active bit; retain the native unsigned clear mask. */
void btlClearSceneTaskActiveFlag(s32 taskAddress) {
    *(u32 *)(taskAddress + FLD_SCENE_TASK_FLAGS_OFFSET) = *(u32 *)(taskAddress + FLD_SCENE_TASK_FLAGS_OFFSET) & 0xfffffffe;
}

/* Bind the actor, select a valid secondary-group action, and mark the task bound. */
void btlBindActorTaskAndSelectActionNumber(s32 taskAddress, s32 actorAddress) {
    u32 flags;

    flags = *(u32 *)(actorAddress + 0x110);
    *(s32 *)(taskAddress + 0x18) = actorAddress;
    if ((flags & FLD_SCENE_ACTOR_SECONDARY_BIT) != 0 &&
        *(u16 *)(actorAddress + 0x124) <= 0x17F) {
        *(u16 *)(taskAddress + 4) =
                  (u16)*(u8 *)(((u32)*(u16 *)(actorAddress + 0x124) * 0x14 -
                                                      (u32)*(u16 *)(actorAddress + 0x124)) * 4 + datEnemyRecords + 0x15);
    }
    flags = *(u32 *)(taskAddress + FLD_SCENE_TASK_FLAGS_OFFSET);
    *(u32 *)(taskAddress + FLD_SCENE_TASK_FLAGS_OFFSET) = flags | FLD_SCENE_TASK_BOUND_BIT;
}

INCLUDE_RODATA(const s32, "game/code_001C48A8", D_003A35A8);

INCLUDE_RODATA(const s32, "game/code_001C48A8", D_003A35B8);

