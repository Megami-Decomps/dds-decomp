#include "common.h"
#include "pcp_vu0.h"

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
    s32 state;
    u8 pad04[0x168];
    struct SceneLinkedNode *next; /* 0x16C */
} SceneLinkedNode;


/* Actor-task prefix; the separate unit-data list uses UiObject. */
typedef struct SceneTask {
    s32 state;
    u8 pad04[4];
    u32 flags;
    u8 pad0C[0xC];
    UiObject *actor;
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
    u8 pad_490[0x120];
    s32 (*sceneCallback)();
    u8 pad_5B4[0x44];
    void (*completionHook)(void);
} BattleSceneWork;

extern s32 btlCountTasksForOwner(s64);

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
    __asm__ volatile(".set noreorder\n\tsqc2 vf0, 0(%0)\n\t.set reorder" : : "r"(arg0));
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
    s32 *node = (s32 *)fldCreateSceneTileTask(*(s16 *)(context + 0x288), *(s16 *)(context + 0x28A));
    *(u64 *)((u8 *)node + 0x40) = 0x8000000000000001ULL;
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

INCLUDE_ASM(const s32, "game/code_001C48A8", func_001C5910);

INCLUDE_ASM(const s32, "game/code_001C48A8", func_001C5B90);

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
            func_001AD468(*(s32 *)(arg0 + 0x4A0), *(s32 *)(arg0 + 0x220));
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

extern void btlDispatchStateHandler();

extern s32 fldLoadAreaResource();

extern void btlReleaseEventAssets();

extern void btlReleaseBossData();

extern void fldPollAreaResourceLoad();

extern s32 fldGetResourceReadyFlag();

extern s32 btlBossDebugPrintf();

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
void btlSetScene(s32 scene) {
    s32 work = btlGetRuntime();

    ((BattleSceneWork *)work)->currentScene = scene;
    ((BattleSceneWork *)work)->frame = 0;
    ((BattleSceneWork *)work)->sceneState = 0;
    D_00359A88[scene].initialize(work);
}

/* Queue a transition for the next scene update. */
void btlQueueScene(u32 scene) {
    s32 work;

    work = btlGetRuntime();
    ((BattleSceneWork *)work)->queuedScene = scene;
}

extern void btlSetScene(s32);


/* Consume a queued transition, run the current updater, and advance its frame. */
void btlUpdateScene(void) {
    s32 context = btlGetRuntime();
    s32 next = ((BattleSceneWork *)context)->queuedScene;
    s32 result;
    if (next != 0) {
        btlSetScene(next);
        ((BattleSceneWork *)context)->queuedScene = 0;
    }
    result = D_00359A88[((BattleSceneWork *)context)->currentScene].update(context);
    if (result != 0) {
        btlQueueScene(result);
    }
    ((BattleSceneWork *)context)->frame += 1;
}

void btlResetToInitialScene(void) {
    s32 work;

    work = btlGetRuntime();
    btlSetScene(1);
    ((BattleSceneWork *)work)->queuedScene = 0;
}

void btlGetCurrentSceneRecordValue(void) {
}

/* Return the current initializer row's flags word. */
s32 fldGetSceneDescriptorProperty(void) {
    s32 scene;

    scene = ((BattleSceneWork *)btlGetRuntime())->currentScene;
    return D_00359A88[scene].flags;
}

s32 *fldGetActorSceneGroupResource(s32 *object) {
    s32 context = btlGetRuntime();
    s32 *result = (s32 *)((BattleSceneWork *)context)->groupSecondary;
    u32 flags;
    if (((SceneTask *)object)->flags & 0x40) {
        return (s32 *)((BattleSceneWork *)context)->groupHandles;
    }
    flags = ((SceneTask *)object)->actor->flags & 0xE00;
    switch (flags) {
    case 0x200:
        result = (s32 *)((BattleSceneWork *)context)->groupPrimary;
        break;
    case 0x400:
        break;
    case 0x800:
        result = (s32 *)((BattleSceneWork *)context)->groupTertiary;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

s32 fldGetSceneGroupResource(u8 group) {
    s32 context = btlGetRuntime();
    s32 resource;

    switch (group) {
    case 1:
        resource = (s32)((BattleSceneWork *)context)->groupPrimary;
        break;
    case 2:
        resource = (s32)((BattleSceneWork *)context)->groupSecondary;
        break;
    case 3:
        resource = (s32)((BattleSceneWork *)context)->groupTertiary;
        break;
    default:
        resource = 0;
        break;
    }
    return resource;
}

s32 fldClassifyActorSceneGroup(s32 *object) {
    u32 flags;
    btlGetRuntime();
    if (((SceneTask *)object)->flags & 0x40) {
        return 8;
    }
    flags = ((SceneTask *)object)->actor->flags & 0xE00;
    switch (flags) {
    case 0x200: return 0x14;
    case 0x400: return 0x2D;
    case 0x800: return 0xF;
    default: return 0;
    }
}

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
                func_001A29D0((s32)first->actor + 0x120, 3) < func_001A29D0((s32)second->actor + 0x120, 3)) {
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
                *(u8 *)((s32)first->actor + 0x11C) > *(u8 *)((s32)second->actor + 0x11C)) {
                entry[0] = second;
                swapped = 1;
                entry[1] = first;
            }
        }
    } while (swapped != 0);
}

s32 fldGetSceneGroupIndexByActorFlags(u8 *object) {
    u32 flags = *(u32 *)(*(u8 **)(object + 0x18) + 0x110) & 0xE00;
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

INCLUDE_ASM(const s32, "game/code_001C48A8", func_001C7690);

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
    s32 context = btlGetRuntime();
    SceneSlot *slot;
    u32 i;
    if (((BattleSceneWork *)context)->flags & 0x1000) {
        return 1;
    }
    slot = ((BattleSceneWork *)context)->slots;
    for (i = 0; i < 8; i++, slot++) {
        if (slot->group != 0) {
            return 0;
        }
    }
    return 1;
}

void fldInitializeSceneGroups(void) {
    s32 work;

    work = btlGetRuntime();
    fldSortGroupByPriority(((BattleSceneWork *)work)->groupPrimary, 0x14);
    btlSortSceneGroupByPriorityDesc(((BattleSceneWork *)work)->groupSecondary, 0x2d);
    btlSortSceneGroupByPriorityDesc(((BattleSceneWork *)work)->groupTertiary, 0xf);
    func_001C7690();
}

void btlMoveTaskToGroupTail(SceneTask *task) {
    SceneTask **group = (SceneTask **)fldGetActorSceneGroupResource((s32 *)task);
    u32 count = fldClassifyActorSceneGroup((s32 *)task);
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

    if (task != 0 && (task->flags & 8) != 0 && task->actor != 0 &&
        (task->flags & 0x40) == 0 && (task->actor->flags & 0x200) != 0) {
        i = 0;
        scene = (BattleSceneWork *)btlGetRuntime();
        fldSortGroupByPriority(scene->groupPrimary, 0x14);
        for (; i < 0x14 && scene->groupPrimary[0] != task; i++) {
            btlMoveTaskToGroupTail(scene->groupPrimary[0]);
        }
    }
}

void fldAppendTaskToGroup(SceneTask *task) {
    SceneTask **slot = (SceneTask **)fldGetActorSceneGroupResource((s32 *)task);
    SceneTask *head;
    fldClassifyActorSceneGroup((s32 *)task);
    head = *slot;
    while (*slot != 0) {
        slot++;
    }
    *slot = task;
    btlRotateGroupUntilTaskFirst(head);
}

void fldAppendSceneGroupHandle(s32 value) {
    s32 *slot = (s32 *)((BattleSceneWork *)btlGetRuntime())->groupHandles;
    while (*slot != 0) {
        slot++;
    }
    *slot = value;
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

void btlRemoveTaskFromSceneGroup(SceneTask *task) {
    SceneTask **group = (SceneTask **)fldGetActorSceneGroupResource((s32 *)task);
    u32 count = fldClassifyActorSceneGroup((s32 *)task);
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
    s32 work;

    work = btlGetRuntime();
    ((BattleSceneWork *)work)->flags |= 0xc;
}

void fldClearSceneAdvanceFlag(void) {
    s32 work;

    work = btlGetRuntime();
    ((BattleSceneWork *)work)->flags &= 0xfffffffb;
}

/* Prefer the special handle queue; otherwise return the front group's first task. */
s32 fldGetActiveSceneGroupValue(void) {
    s32 work;
    s32 value;
    u32 groupAddress;

    work = btlGetRuntime();
    value = (s32)((BattleSceneWork *)work)->groupHandles[0];
    if (value != 0) {
        return value;
    }
    groupAddress = fldGetSceneGroupResource(((BattleSceneWork *)work)->slots[0].group);
    return *(s32 *)groupAddress;
}

/* Read one task word from the front group; an empty front slot returns zero. */
s32 fldGetSceneGroupEntry(s32 index) {
    s32 context = btlGetRuntime();

    if (((BattleSceneWork *)context)->slots[0].group == 0) {
        return 0;
    }
    return *(s32 *)(fldGetSceneGroupResource(((BattleSceneWork *)context)->slots[0].group) + index * 4);
}

extern s32 func_001A8188(void);
extern void btlDispatchStateHandler();

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

u32 fldDispatchSceneGroupRequestWhenAllowed(s32 *arg0) {
    u8 temp_v0;

    if (*arg0 == 0) {
        temp_v0 = (u8)arg0[2];
    }
    else {
        if ((*(u32 *)(*arg0 + 8) & 0x40) != 0) {
            return 1;
        }
        temp_v0 = (u8)arg0[2];
    }
    fldConsumeSceneSlotCounters(arg0[1], temp_v0);
    return 1;
}

u8 *fldCreateSceneGroupAction(u8 *actor, u32 owner, s32 groupIndex) {
    u8 group = groupIndex;
    u8 *object = (u8 *)btlAllocTask(0xC);
    u8 *fields;
    object[0] = 1;
    *(s16 *)(object + 0x20) = 0x5C;
    object[0x10] = 0;
    if (actor != 0) {
        *(u64 *)(object + 0x40) = *(u64 *)(*(u8 **)(actor + 0x18) + 0x108);
    }
    *(u32 *)(object + 0x4C) = (u32)fldDispatchSceneGroupRequestWhenAllowed;
    *(u32 *)(object + 0x48) = 0;
    fields = (u8 *)btlGetTaskArguments(object);
    *(u32 *)(fields + 0) = (u32)actor;
    *(u32 *)(fields + 4) = owner;
    fields[8] = group;
    return object;
}

void fldStopSceneActorActionUpdate(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) & 0xfffffffb;
}

u32 fldActivateRequestedSceneActor(s32 *request) {
    s32 context = btlGetRuntime();
    s32 actor = request[0];

    *(u32 *)(context + 0x1F4) |= 4;
    if (actor != 0 && (*(u32 *)(actor + 8) & 0x40) != 0) {
        return 1;
    }
    fldInsertSceneSlots(request[1]);
    return 1;
}

extern u32 fldActivateRequestedSceneActor(s32 *);

u8 *fldCreateSceneActorAction(u8 *owner, s32 parameter) {
    u8 *task = (u8 *)btlAllocTask(8);
    u8 *data;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x5D;
    task[0x10] = 0;
    if (owner != 0) {
        *(u64 *)(task + 0x40) = *(u64 *)(*(u8 **)(owner + 0x18) + 0x108);
    }
    *(u32 *)(task + 0x48) = (u32)fldStopSceneActorActionUpdate;
    *(u32 *)(task + 0x4C) = (u32)fldActivateRequestedSceneActor;
    data = (u8 *)btlGetTaskArguments(task);
    *(u32 *)data = (u32)owner;
    *(s32 *)(data + 4) = parameter;
    return task;
}

u32 fldApplySceneSlotSwapRequest(u32 *arg0) {
    fldSwapSceneSlots(*arg0);
    return 1;
}

s32 *fldCreateActorAction(s32 owner) {
    u8 *task = (u8 *)btlAllocTask(4);
    *task = 1;
    *(u16 *)(task + 0x20) = 0x5E;
    *(u32 *)(task + 0x4C) = (u32)fldApplySceneSlotSwapRequest;
    task[0x10] = 0;
    *(u32 *)(task + 0x48) = 0;
    *(s32 *)btlGetTaskArguments(task) = owner;
    return (s32 *)task;
}

void btlSetSceneTaskActiveFlag(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) | 1;
}

void btlClearSceneTaskActiveFlag(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xfffffffe;
}

void btlBindActorTaskAndSelectActionNumber(s32 arg0, s32 arg1) {
    u32 flags;

    flags = *(u32 *)(arg1 + 0x110);
    *(s32 *)(arg0 + 0x18) = arg1;
    if ((flags & 0x400) != 0) {
        if (0x17f < *(u16 *)(arg1 + 0x124)) {
            flags = *(u32 *)(arg0 + 8);
            goto store;
        }
        *(u16 *)(arg0 + 4) =
                  (u16)*(u8 *)(((u32)*(u16 *)(arg1 + 0x124) * 0x14 -
                                                      (u32)*(u16 *)(arg1 + 0x124)) * 4 + datEnemyRecords + 0x15);
    }
    flags = *(u32 *)(arg0 + 8);
store:
    *(u32 *)(arg0 + 8) = flags | 8;
}

INCLUDE_RODATA(const s32, "game/code_001C48A8", D_003A35A8);

INCLUDE_RODATA(const s32, "game/code_001C48A8", D_003A35B8);

