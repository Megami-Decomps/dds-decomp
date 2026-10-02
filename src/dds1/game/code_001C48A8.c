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

typedef struct SceneSlot {
    u8 a;
    u8 b;
    u8 id;
} SceneSlot;

/* Actor-task prefix; the separate unit-data list uses UiObject. */
typedef struct SceneTask {
    s32 state;
    u8 pad04[4];
    u32 flags;
    u8 pad0C[0xC];
    UiObject *actor;
} SceneTask;

typedef struct BattleController {
    u8 pad_000[0x1F4];
    u32 flags;
    u8 pad_1F8[0x30];
    UiObject *actors;
    u8 pad_22C[0x20];
    u16 variant;
    u8 pad_24E[2];
    s32 step;
    u8 pad_254[0x28];
    s32 mode;
    u8 pad_280[0x1C];
    s32 taskParent;
    u8 pad_2A0[0xC];
    s32 spriteObject;
    u8 pad_2B0[0x24];
    SceneSlot slots[8];
    SceneTask *groupPrimary[20];
    SceneTask *groupSecondary[45];
    SceneTask *groupTertiary[15];
    SceneTask *groupHandles[8];
    u8 pad_44C[0x40];
    SceneTask *currentTask;
    u8 pad_490[0x120];
    s32 (*sceneCallback)();
    u8 pad_5B4[0x44];
    void (*completionHook)(void);
} BattleController;

extern s32 btlCountTasksForOwner(s64);

extern void func_001BCB88(s32, s32);

extern void btlUpdateScene(void);

extern s8 D_00324530[];

extern s32 D_00359A90[];

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

extern void kwlnDrawSetOffsetTransition(s32, s32, s32);

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

INCLUDE_ASM(const s32, "game/code_001C48A8", fldSceneStateRestoreDisplay);

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

void fldAdvanceSceneVariant(BattleController *scene) {
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

typedef struct SceneLoadNode {
    s32 state;
    u8 pad_004[0x168];
    struct SceneLoadNode *next;
} SceneLoadNode;

typedef struct BattleSceneDeltaState {
    u8 pad_000[0x1F4];
    u32 flags;
    u32 subFlags;
    u8 pad_1FC[0x28];
    SceneLoadNode *linkedNodes;
    u8 pad_228[0x30];
    u8 phaseFlag;
    u8 pad_259[0x33];
    s32 loadStep;
    u8 pad_290[0x40];
    s32 alternateLink;
} BattleSceneDeltaState;

extern PartyDeltaState *datGameState;

extern void itfMesClearFlags(s32);

extern void brsTaskAllowUpdate(void);

extern void evtBeginSolarOverlayFadeOut(s32);

extern void func_001A1960();

extern s32 mnuIsTitleEntryAvailable();

extern void datMoveCursorX();

void btlApplyPartyEntryWeightedDelta(BattleSceneDeltaState *scene) {
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
                if (scene->phaseFlag == 1) {
                    if (datGameState->entry[i].link != 0 || scene->alternateLink != 0) {
                        if (!(datGameState->entry[i].mask & 0x40)) {
                            if (mnuIsTitleEntryAvailable(&datGameState->entry[i]) == 0) {
                                datMoveCursorX(&datGameState->entry[i],
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

u32 fldStepAreaLoad(BattleSceneDeltaState *work) {
    SceneLoadNode *node;
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
    s32 *entry;
    btlAdvanceTitleStateWithAudioCleanup(context);
    entry = *(s32 **)(context + 0x224);
    while (entry != 0) {
        if (entry[0] != 30) {
            btlDispatchStateHandler(entry, 30);
        }
        entry = *(s32 **)((u8 *)entry + 0x16C);
    }
    btlFlagTasksForUpdate();
}

INCLUDE_ASM(const s32, "game/code_001C48A8", func_001C7028);

void fldMarkSceneRefresh(s32 arg0) {
    func_00215FE0(arg0);
    *(u32 *)(arg0 + 0x1fc) = *(u32 *)(arg0 + 0x1fc) | 4;
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

void btlSetScene(s32 scene) {
    s32 work = btlGetRuntime();

    *(s32 *)(work + 0x208) = scene;
    *(s32 *)(work + 0x210) = 0;
    *(s32 *)(work + 0x214) = 0;
    D_00359A88[scene].initialize(work);
}

void btlQueueScene(u32 arg0) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 0x20c) = arg0;
}

extern void btlSetScene(s32);

extern s32 D_00359A8C[];

void btlUpdateScene(void) {
    s32 context = btlGetRuntime();
    s32 next = *(s32 *)(context + 0x20C);
    s32 result;
    if (next != 0) {
        btlSetScene(next);
        *(s32 *)(context + 0x20C) = 0;
    }
    result = ((s32 (*)(s32))D_00359A8C[*(s32 *)(context + 0x208) * 3])(context);
    if (result != 0) {
        btlQueueScene(result);
    }
    *(s32 *)(context + 0x210) += 1;
}

void btlResetToInitialScene(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    btlSetScene(1);
    *(u32 *)(temp_v0 + 0x20c) = 0;
}

void btlGetCurrentSceneRecordValue(void) {
}

s32 fldGetSceneDescriptorProperty(void) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(btlGetRuntime() + 0x208);
    return D_00359A90[temp_v0 * 3];
}

s32 *fldGetActorSceneGroupResource(s32 *object) {
    s32 context = btlGetRuntime();
    s32 *result = (s32 *)(context + 0x33C);
    u32 flags;
    if (object[2] & 0x40) {
        return (s32 *)(context + 0x42C);
    }
    flags = *(u32 *)(object[6] + 0x110) & 0xE00;
    switch (flags) {
    case 0x200:
        result = (s32 *)(context + 0x2EC);
        break;
    case 0x400:
        break;
    case 0x800:
        result = (s32 *)(context + 0x3F0);
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
        resource = context + 0x2EC;
        break;
    case 2:
        resource = context + 0x33C;
        break;
    case 3:
        resource = context + 0x3F0;
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
    if (object[2] & 0x40) {
        return 8;
    }
    flags = *(u32 *)(object[6] + 0x110) & 0xE00;
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

void fldCompactSceneSlots(void) {
    SceneSlot *slot = ((BattleController *)btlGetRuntime())->slots;
    u32 i;
    if (slot->b == 0) {
        for (i = 0; i < 7; i++) {
            slot[0].a = slot[1].a;
            slot[0].b = slot[1].b;
            slot[0].id = slot[1].id;
            slot++;
        }
        slot->a = 0;
        slot->b = 0;
        slot->id = 0;
    }
}

void fldConsumeSceneSlotCounters(s32 amount, u8 mode) {
    BattleController *scene = (BattleController *)btlGetRuntime();
    u8 head;
    u32 i;

    if (scene->flags & 0x100) {
        head = scene->slots[0].a;
        switch (mode) {
        case 0:
            break;
        case 1:
            if (amount < 100) {
                if (amount < scene->slots[0].b) {
                    scene->slots[0].b -= amount;
                } else {
                    scene->slots[0].b = 0;
                    fldCompactSceneSlots();
                }
            } else {
                while (amount > 0) {
                    scene->slots[0].b = 0;
                    fldCompactSceneSlots();
                    if (scene->slots[0].a != head) {
                        return;
                    }
                    if (scene->slots[0].b == 0) {
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
                    if (scene->slots[i].a == head && scene->slots[i].b == 100) {
                        break;
                    }
                }
                if (i < 8) {
                    scene->slots[i].b -= 50;
                } else {
                    scene->slots[0].b = 0;
                    fldCompactSceneSlots();
                }
                amount -= 50;
            }
            break;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001C48A8", fldInsertSceneSlots);

void fldSwapSceneSlots(s32 index) {
    BattleController *scene = (BattleController *)btlGetRuntime();
    if (scene->flags & 0x100) {
        u8 firstId = scene->slots[0].a;
        u8 count = scene->slots[0].b;
        fldConsumeSceneSlotCounters(index, 1);
        if (index < count) {
            if (scene->slots[1].a != 0 && scene->slots[1].a != firstId) {
                u8 a = scene->slots[0].a;
                u8 b = scene->slots[0].b;
                u8 id = scene->slots[0].id;
                scene->slots[0].a = scene->slots[1].a;
                scene->slots[0].b = scene->slots[1].b;
                scene->slots[0].id = scene->slots[1].id;
                scene->slots[1].a = a;
                scene->slots[1].b = b;
                scene->slots[1].id = id;
            }
        }
    }
}

s32 fldCountSceneSlots(void) {
    SceneSlot *slot = ((BattleController *)btlGetRuntime())->slots;
    s32 count = 0;
    u32 i;
    for (i = 0; i < 8; i++, slot++) {
        if (slot->a != 0 && slot->b != 0) {
            count++;
        }
    }
    return count;
}

s32 fldAreSceneSlotsFinished(void) {
    s32 context = btlGetRuntime();
    u8 *slot;
    u32 i;
    if (*(u32 *)(context + 0x1F4) & 0x1000) {
        return 1;
    }
    slot = (u8 *)(context + 0x2D4);
    for (i = 0; i < 8; i++, slot += 3) {
        if (*slot != 0) {
            return 0;
        }
    }
    return 1;
}

void fldInitializeSceneGroups(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    fldSortGroupByPriority(temp_v0 + 0x2ec, 0x14);
    btlSortSceneGroupByPriorityDesc(temp_v0 + 0x33c, 0x2d);
    btlSortSceneGroupByPriorityDesc(temp_v0 + 0x3f0, 0xf);
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
    BattleController *scene;
    u32 i;

    if (task != 0 && (task->flags & 8) != 0 && task->actor != 0 &&
        (task->flags & 0x40) == 0 && (task->actor->flags & 0x200) != 0) {
        i = 0;
        scene = (BattleController *)btlGetRuntime();
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
    s32 *slot = (s32 *)(btlGetRuntime() + 0x42C);
    while (*slot != 0) {
        slot++;
    }
    *slot = value;
}

extern void btlRemoveTaskFromSceneGroup(SceneTask *task);

void fldUpdateSceneGroupTask(s32 taskValue) {
    SceneTask *task = (SceneTask *)taskValue;
    BattleController *scene = (BattleController *)btlGetRuntime();
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
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) | 0xc;
}

void fldClearSceneAdvanceFlag(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) & 0xfffffffb;
}

s32 fldGetActiveSceneGroupValue(void) {
    s32 temp_v0;
    s32 temp_v1;
    u32 temp_v2;

    temp_v0 = btlGetRuntime();
    temp_v1 = *(s32 *)(temp_v0 + 0x42c);
    if (temp_v1 != 0) {
        return temp_v1;
    }
    temp_v2 = fldGetSceneGroupResource(*(u8 *)(temp_v0 + 0x2d4));
    return *(s32 *)temp_v2;
}

s32 fldGetSceneGroupEntry(s32 index) {
    s32 context = btlGetRuntime();

    if (*(u8 *)(context + 0x2D4) == 0) {
        return 0;
    }
    return *(s32 *)(fldGetSceneGroupResource(*(u8 *)(context + 0x2D4)) + index * 4);
}

extern s32 func_001A8188(void);
extern void btlDispatchStateHandler();

/* Advance one scene group task after both scene gates have opened. */
void func_001C8330(void) {
    BattleController *scene = (BattleController *)btlGetRuntime();
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
                task = *(SceneTask **)fldGetSceneGroupResource(*(u8 *)((u8 *)scene + 0x2D4));
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

void fldClearSceneSlotsAndGroups(void) {
    BattleController *scene = (BattleController *)btlGetRuntime();
    u32 i;

    for (i = 0; i < 8; i++) {
        scene->slots[i].a = 0;
        scene->slots[i].b = 0;
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
