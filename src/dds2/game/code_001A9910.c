#include "sdk_ps2.h"
#include "evt_run_context_api.h"
#include "fld_next_scene_request_api.h"
#include "common.h"
#include "snd_battle_sound_file_request_status.h"
#include "btl_runtime_activity_status_api.h"
#include "btl_boss_data_api.h"
#include "btl_event_asset_api.h"
#include "dds3_task_add_child.h"
#include "btl_enc_bg_entry.h"
#include "eff_random_mod.h"
#include "btl_item_drop.h"
#include "dds3_task_priority_query.h"
#include "dds3_task_create.h"
#include "sdf_heap_api.h"
#include "itf.h"
#include "btl_state.h"
#include "sdf_projection.h"
#include "mnu_result.h"
#include "mdl_resource_table.h"
#include "dds3_task_lifecycle.h"
#include "mdl_flag_api.h"

extern u64 D_004366E8;
extern BtlState *btlRuntime;
extern EncBgRow *D_00435E14;
typedef struct BattleController {
    u8 pad_000[0x214];
    s32 frame;
    u32 flags;
    u32 flags21C;
    u8 pad_220[0x28];
    ActionStateLink *taskHead; /* 0x248 */
    BtlUnit *actors;
    u8 pad_250[0x1E];
    u8 mode; /* 0x26E: mode 3 scales defeat experience in DDS2. */
    u8 pad26F;
    u16 variant; /* 0x270 */
    u8 pad_272[0x22];
    s32 adjustmentRecordIndex; /* 0x294 */
    s32 adjustmentGroupIndex;  /* 0x298 */
    s32 adjustmentEntryIndex;  /* 0x29C */
    u8 pad_2A0[0x24];
    dds3ProcessID_t *drawTask;
    u8 pad2C8[0x14];
    BattleItemDrop itemDrops[3];
    s32 moneyEarned; /* 0x2E8 */
    u32 rewardMacca; /* 0x2EC */
    s32 experienceEarned; /* 0x2F0 */
    s32 epEarned; /* 0x2F4 */
    s32 totalAp; /* 0x2F8: defeat EP accumulated for the result screen. */
    u16 specialEnemyDefeats; /* 0x2FC: defeated enemy kinds 100 through 103. */
    u8 pad2FE[0x3DE];
    s32 (*commandRangeOverride)(BtlUnit *, s32);
} BattleController;
extern char D_004150B0[]; /* "btl:hunt ep=%d[id=%X]\n" */
extern void btlBossDebugPrintf(const char *, ...);
extern EffRandState effSharedRandomState[];
#define BTL_ENTRY_STATUS_MASK 0x7FFF
extern u32 datComputeSkillBoostedMaxHp(datUnitWork_t *);
extern u32 datComputeSkillBoostedMaxMp(datUnitWork_t *);

void btlResetRuntimeSequenceCounter(void) {
    D_004366E8 = 1;
}

u64 btlAdvanceRuntimeSequenceCounter(void) {
    s64 value = D_004366E8 + 1;

    if (value < 0) {
        value = 1;
        D_004366E8 = value;
    } else {
        D_004366E8 = value;
    }
    return value;
}

/* Clear the battle model flags; the exclusive upper limit differs by title. */
void btlClearModelFlagRange(void) {
    s32 flagIndex;

    for (flagIndex = 0xBE0; flagIndex < 0xC00; flagIndex++) {
        evtBitOff(flagIndex);
    }
}

extern s32 btlUpdateFadeColor(void);

extern s32 btlUpdateAutoMusic(void);

extern s32 btlUpdateTintAndWorldLight(void);

extern void btlUpdateMotionSoundLoading(void);

extern s32 btlUpdateScene(void);

extern s32 fldAdvanceSceneGroupScheduler(void);

extern s32 btlUpdateActionSeqs(void);

extern s32 btlUpdateActiveActorModelState(void);

extern void btlUpdateActiveFrameNoOp(void);

extern s32 btlSweepFinishedTasks(void);

extern void btlUpdateBattleCameraFrame(void);

extern s32 btlExitWhenAudioAndTasksIdle(void);

s32 btlUpdateActiveBattleFrame(dds3ProcessID_t *task) {
    if (btlRuntime == NULL) {
        return 0;
    }
    if (((BattleController *)btlRuntime)->flags & 1) {
        btlUpdateFadeColor();
        btlUpdateAutoMusic();
        btlUpdateTintAndWorldLight();
        btlUpdateMotionSoundLoading();
        btlUpdateScene();
        fldAdvanceSceneGroupScheduler();
        btlUpdateActionSeqs();
        btlUpdateActiveActorModelState();
        btlUpdateActiveFrameNoOp();
        btlSweepFinishedTasks();
        btlUpdateBattleCameraFrame();
        ((BattleController *)btlRuntime)->frame = ((BattleController *)btlRuntime)->frame + 1;
    } else {
        btlExitWhenAudioAndTasksIdle();
    }
    return 0;
}

extern s32 btlTickFieldSwayAndTint(void);

extern s32 btlReleaseRainSoundTransition(void);

extern s32 btlSweepFloorModelLists(void);

extern s32 btlUpdateActorModelColorAndLinks(void);

extern void btlUpdateEventPlayback(void);

extern void btlUpdateFieldPresentationNoOp(void);

extern s32 btlUpdateSceneFlow(void);

extern void btlRunDeferredTasks(void);

s32 btlUpdateBattleFieldPresentation(dds3ProcessID_t *task) {
    BattleController *battle = (BattleController *)btlRuntime;
    if (battle == 0) {
        return 0;
    }
    if (battle->flags & 1) {
        btlTickFieldSwayAndTint();
        btlReleaseRainSoundTransition();
        btlSweepFloorModelLists();
        btlUpdateActorModelColorAndLinks();
        btlUpdateEventPlayback();
        btlUpdateFieldPresentationNoOp();
        btlUpdateSceneFlow();
        btlRunDeferredTasks();
    }
    return 0;
}

/* Empty destructor callback for the active battle-frame task. */
void btlReleaseActiveBattleFrameTask(dds3ProcessID_t *task) {
}

extern char D_004366F0[];

extern void btlReleaseActiveBattleFrameTask(dds3ProcessID_t *task);

extern s32 btlUpdateBattleFieldPresentation(dds3ProcessID_t *task);

extern s32 btlUpdateActiveBattleFrame(dds3ProcessID_t *task);

void btlCreateDrawTasks(void) {
    BattleController *battle = (BattleController *)btlRuntime;
    dds3ProcessID_t *draw;
    draw = dds3StartProcess(D_004366F0, 0x3F9, 0, 0, btlUpdateActiveBattleFrame, btlReleaseActiveBattleFrameTask, 0);
    battle->drawTask = draw;
    dds3LinkProcess(draw, dds3StartProcess("battle_draw", 0x2B0E, 0, 0, btlUpdateBattleFieldPresentation, 0, 0));
}

void btlDestroyDrawTaskAtPriorityWhenPresent(void) {
    dds3ProcessID_t *task;

    task = dds3SearchProcessPriority(0x3f9);
    if (task != 0) {
        BattleController *battle = (BattleController *)btlRuntime;
        dds3KillProcess(battle->drawTask, 1);
        return;
    }
}

typedef struct BattleInitState {
    u8 pad000[0x214];
    u32 tick;
    u8 pad218[4];
    u32 commandRestrictFlags;
    u8 pad220[4];
    u32 unk224;
    u8 pad228[0x20];
    u32 listHeads[6];
    u8 pad260[0x1C];
    u8 endCode;
    u8 pad27D[0x2F];
    u16 backgroundA;
    u16 backgroundB;
    u32 unk2B0;
    u8 unk2B4;
    u8 pad2B5[0x20F];
    u8 timingRate;
    u8 pad4C5[3];
    f32 modelFrameScale;
    s32 messageWindows[3];
    u8 pad4D8[0xE0];
    u8 unk5B8;
    u8 pad5B9[3];
    u32 unk5BC;
} BattleInitState;

extern sdfMemHandle_t *D_004366E0;

extern ItfMesSub *datStatusHelpMessages;

extern ItfMesSub D_003858D8;

extern ItfMesSub D_00385228;

extern u16 dds3VblanksPerFrame;

extern u32 dds3GetKernelCount(void);

extern u32 btlIsCurrentDarkZone(void);

extern void sdfRandomize(void *, u32);

extern ItfMesWindowIndex itfMesMngInitialize(ItfMesSub *);

extern void btlResetActorEntryState(void);

extern char D_004366F8[];

extern char D_003B4D90[];

extern char D_003B4DA8[];

extern void btlAllocatePanelTaskWork(void);
extern void btlInitializeRuntimeNoOp(void);
extern void btlRefreshSoundEntries(void);
extern void btlResetDeferredTaskQueue(void);
extern void btlResetFieldColorAndSweepFlags(void);
extern void btlResetMotionSoundFrameCounters(void);
extern void btlResetToInitialScene(void);
extern void btlRetainButtonTexture(void);
extern void fldClearSceneSlotsAndGroups(void);
extern void sndResetTransition(void);

/* Allocate/reset battle work, initialize windows and queue shared resources. */
void btlInitializeBattleRuntime(void) {
    sdfMemHandle_t *allocation;
    u32 seed;
    u8 *runtime;

    btlResetRuntimeSequenceCounter();
    allocation = sdfMemAllocDirect(0xFD4);
    D_004366E0 = allocation;
    runtime = (u8 *)sdfMemLock(allocation);
    btlRuntime = (BtlState *)runtime;
    memset(runtime, 0, 0xFD4);

    ((BattleInitState *)btlRuntime)->tick = 0;
    ((BattleInitState *)btlRuntime)->unk224 = 0;
    ((BattleInitState *)btlRuntime)->listHeads[0] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[1] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[2] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[3] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[4] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[5] = 0;
    dds3VblanksPerFrame = 2;
    ((BattleInitState *)btlRuntime)->timingRate = 0x1E;
    {
        BattleInitState *state = (BattleInitState *)btlRuntime;
        state->endCode = 0;
        state->modelFrameScale = 1.0f;
    }
    ((BattleInitState *)btlRuntime)->unk5B8 = 0;
    ((BattleInitState *)btlRuntime)->unk5BC = 0x80808080;
    ((BattleInitState *)btlRuntime)->backgroundA = 0xC9;
    ((BattleInitState *)btlRuntime)->backgroundB = 1;
    ((BattleInitState *)btlRuntime)->unk2B0 = 0;
    ((BattleInitState *)btlRuntime)->unk2B4 = btlIsCurrentDarkZone();
    seed = dds3GetKernelCount();
    sdfRandomize(effSharedRandomState, seed);
    btlResetActorEntryState();

    ((BattleInitState *)btlRuntime)->messageWindows[0] =
        itfMesMngInitialize(datStatusHelpMessages);
    ((BattleInitState *)btlRuntime)->messageWindows[1] =
        itfMesMngInitialize(&D_003858D8);
    ((BattleInitState *)btlRuntime)->messageWindows[2] =
        itfMesMngInitialize(&D_00385228);
    sndResetTransition();
    btlResetMotionSoundFrameCounters();
    btlResetDeferredTaskQueue();
    btlResetToInitialScene();
    fldClearSceneSlotsAndGroups();
    btlResetFieldColorAndSweepFlags();
    btlRefreshSoundEntries();
    btlRetainButtonTexture();
    btlAllocatePanelTaskWork();
    btlInitializeRuntimeNoOp();
    btlClearModelFlagRange();
    evtBitOff(0x82B);

    if (evtBitCheck(0x290) == 0) {
        mdlSetViewerSlotResourceHandles(0, 1, 0, D_003B4D90, 0);
        btlBossDebugPrintf(D_004366F8, D_003B4D90);
    } else {
        mdlSetViewerSlotResourceHandles(0, 1, 0, D_003B4DA8, 0);
        btlBossDebugPrintf(D_004366F8, D_003B4DA8);
    }
}

extern void itfMesMngRelease(ItfMesWindowIndex window);

extern char D_00414F48[];

extern char D_00414F68[];

extern char D_00414F80[];

extern char D_00414F98[];

extern char D_00414FB0[];

extern s32 btlCountRegisteredTasks(void);

extern u32 brsTaskTryDestroy(void);
extern void btlAdvanceTitleStateWithAudioCleanup(void);
extern void btlClearPendingSoundList(void);
extern void btlClearSoundAndModelResources(void);
extern void btlClearTaskLists(void);
extern void btlClearTintAndEnableCamera(void);
extern void btlDestroyAllActionSeqs(void);
extern void btlDestroyAllUnits(void);
extern void btlFlagTasksForUpdate(void);
extern void btlFreeFieldBlocks(void);
extern void btlGetCurrentSceneRecordValue(void);
extern void btlReleaseButtonTexture(void);
extern void btlStopRainSoundTransition(void);
extern void effResetSlots(void);
extern void evtDestroySelectionState(void);
extern void evtDisableMoonDraw(void);
extern void fldDestroySceneTasksAndBuffers(void);
extern void itfMesClearFlags(u32 mask);
extern void dds3StopQuake(void);
extern void sndClearList(void);
extern void sndFreeBattleSoundEntries(void);

s32 btlExitWhenAudioAndTasksIdle(void) {
    BattleInitState *state;

    if (btlRuntime == NULL) {
        btlBossDebugPrintf(D_00414F48);
        return 0;
    }
    if (sndHasOccupiedNodeSlots() != SND_BATTLE_SOUND_FILE_REQUESTS_CLEAR) {
        btlBossDebugPrintf(D_00414F68);
        return -1;
    }
    if (btlCountRegisteredTasks() != 0) {
        btlBossDebugPrintf(D_00414F80);
        btlFlagTasksForUpdate();
        return -1;
    }
    btlReleaseBossData();
    btlClearTaskLists();
    btlDestroyAllActionSeqs();
    btlDestroyAllUnits();
    btlClearPendingSoundList();
    btlReleaseEventAssets();
    btlGetCurrentSceneRecordValue();
    btlFreeFieldBlocks();
    btlStopRainSoundTransition();
    btlClearTintAndEnableCamera();
    fldDestroySceneTasksAndBuffers();
    btlReleaseButtonTexture();
    sndFreeBattleSoundEntries();
    sndClearList();
    brsTaskTryDestroy();
    btlClearSoundAndModelResources();
    btlDestroyDrawTaskAtPriorityWhenPresent();
    itfMesMngRelease(((BattleInitState *)btlRuntime)->messageWindows[2]);
    itfMesMngRelease(((BattleInitState *)btlRuntime)->messageWindows[1]);
    itfMesMngRelease(((BattleInitState *)btlRuntime)->messageWindows[0]);
    btlAdvanceTitleStateWithAudioCleanup();
    dds3StopQuake();
    evtDestroySelectionState();
    effResetSlots();
    evtDisableMoonDraw();
    itfMesClearFlags(1);

    state = (BattleInitState *)btlRuntime;
    if (state->commandRestrictFlags & 0x8000) {
        state->commandRestrictFlags &= ~0x8000;
        dds3ScreenParam.camera.farZ = 65536.0f;
        btlBossDebugPrintf(D_00414F98);
    }

    sdfMemFreeDirect(D_004366E0);
    D_004366E0 = 0;
    btlRuntime = NULL;
    btlBossDebugPrintf(D_00414FB0);
    return 0;
}

extern void datDebugPrintf(const char *, ...);

extern void btlSetScene(s32);

extern void itfMesSetFlags(u32);

extern s32 btlResolveQueuedSceneRequestParameters(s32 *, s32 *);

/* Start the requested battle mode with its encounter and background parameters. */

INCLUDE_RODATA(const s32, "game/code_001A9910", D_00414F48);

INCLUDE_RODATA(const s32, "game/code_001A9910", D_00414F68);

INCLUDE_RODATA(const s32, "game/code_001A9910", D_00414F80);

INCLUDE_RODATA(const s32, "game/code_001A9910", D_00414F98);

INCLUDE_RODATA(const s32, "game/code_001A9910", D_00414FB0);

void btlStart(s32 mode, s32 pack, s32 encounter) {
    struct { s32 code; s32 parameter; } req;
    u16 paramA;
    u16 paramB;

    btlInitializeBattleRuntime();
    btlCreateDrawTasks();
    switch (mode) {
    case 0:
        ((BtlState *)btlRuntime)->encounterPack = pack;
        ((BtlState *)btlRuntime)->adjustmentGroupIndex = (u8)(encounter >> 24);
        ((BtlState *)btlRuntime)->adjustmentEntryIndex = (u8)(encounter >> 16);
        ((BtlState *)btlRuntime)->battleMode = (u16)encounter;
        btlBossDebugPrintf("enc[pack:%X,no:%X,index:%X,encNo:%X]\n", pack, (u8)(encounter >> 24), (u8)(encounter >> 16), (u16)encounter);
        if (pack > 0) {
            ((BtlState *)btlRuntime)->background.ids.major = D_00435E0C[pack].encounterParamA;
            ((BtlState *)btlRuntime)->background.ids.minor = D_00435E0C[pack].encounterParamB;
            if (((BtlState *)btlRuntime)->battleMode == 0x3A0 && evtBitCheck(0x80D) != 0) {
                ((BtlState *)btlRuntime)->background.ids.major = 0xDF;
                ((BtlState *)btlRuntime)->background.ids.minor = 5;
            } else if (((BtlState *)btlRuntime)->battleMode == 0x3A1 && evtBitCheck(0x80D) != 0) {
                ((BtlState *)btlRuntime)->background.ids.major = 0xE0;
                ((BtlState *)btlRuntime)->background.ids.minor = 6;
            } else if (((BtlState *)btlRuntime)->battleMode == 0x33B && evtBitCheck(0x818) != 0) {
                ((BtlState *)btlRuntime)->background.ids.major = 0xE2;
                ((BtlState *)btlRuntime)->background.ids.minor = 1;
            } else if (((BtlState *)btlRuntime)->battleMode == 0x340 && evtBitCheck(0x818) != 0) {
                ((BtlState *)btlRuntime)->background.ids.major = 0xE4;
                ((BtlState *)btlRuntime)->background.ids.minor = 2;
            }
            datDebugPrintf("Pack:0x%X Maj:%d Min:%d\n", pack, ((BtlState *)btlRuntime)->background.ids.major, ((BtlState *)btlRuntime)->background.ids.minor);
            if (((BtlState *)btlRuntime)->background.ids.major == 0 && ((BtlState *)btlRuntime)->background.ids.minor == 0) {
                if (btlResolveQueuedSceneRequestParameters(&req.code, &req.parameter) != 0) {
                    datDebugPrintf("GetFldBG Maj:%d Min:%d\n", req.code, req.parameter);
                    paramA = req.code;
                    paramB = req.parameter;
                    ((BtlState *)btlRuntime)->background.ids.major = paramA;
                    ((BtlState *)btlRuntime)->background.ids.minor = paramB;
                }
            }
        } else {
            ((BtlState *)btlRuntime)->background.ids.major = datBattleSceneRecords[((BtlState *)btlRuntime)->battleMode].backgroundMajorId;
            ((BtlState *)btlRuntime)->background.ids.minor = datBattleSceneRecords[((BtlState *)btlRuntime)->battleMode].backgroundMinorId;
            if (((BtlState *)btlRuntime)->battleMode == 0x3A0 && evtBitCheck(0x80D) != 0) {
                ((BtlState *)btlRuntime)->background.ids.major = 0xDF;
                ((BtlState *)btlRuntime)->background.ids.minor = 5;
            } else if (((BtlState *)btlRuntime)->battleMode == 0x3A1 && evtBitCheck(0x80D) != 0) {
                ((BtlState *)btlRuntime)->background.ids.major = 0xE0;
                ((BtlState *)btlRuntime)->background.ids.minor = 6;
            } else if (((BtlState *)btlRuntime)->battleMode == 0x33B && evtBitCheck(0x818) != 0) {
                ((BtlState *)btlRuntime)->background.ids.major = 0xE2;
                ((BtlState *)btlRuntime)->background.ids.minor = 1;
            } else if (((BtlState *)btlRuntime)->battleMode == 0x340 && evtBitCheck(0x818) != 0) {
                ((BtlState *)btlRuntime)->background.ids.major = 0xE4;
                ((BtlState *)btlRuntime)->background.ids.minor = 2;
            }
            if (((BtlState *)btlRuntime)->background.ids.major == 0 && ((BtlState *)btlRuntime)->background.ids.minor == 0) {
                if (btlResolveQueuedSceneRequestParameters(&req.code, &req.parameter) != 0) {
                    ((BtlState *)btlRuntime)->background.ids.major = req.code;
                    ((BtlState *)btlRuntime)->background.ids.minor = req.parameter;
                } else {
                    ((BtlState *)btlRuntime)->background.ids.major = 0xC9;
                    ((BtlState *)btlRuntime)->background.ids.minor = 1;
                }
            }
        }
        btlSetScene(2);
        break;
    case 1:
        btlSetScene(0xC);
        break;
    }
    if (((BtlState *)btlRuntime)->background.ids.major == 0 && ((BtlState *)btlRuntime)->background.ids.minor == 0) {
        ((BtlState *)btlRuntime)->background.ids.major = 0xC9;
        ((BtlState *)btlRuntime)->background.ids.minor = 1;
    }
    itfMesSetFlags(1);
    evtBitOn(0xC0F);
    ((BtlState *)btlRuntime)->battleFlags |= 0x701C1;
    ((BtlState *)btlRuntime)->commandRestrictFlags |= 2;
    btlBossDebugPrintf("** btlStart **************\n");
}

extern void btlLoadStartupPanelResources(void);
extern void btlOpenButtonIconResource(void);
extern void sndLoadSysEffLb(void);

void btlLoadInputIconsAndSystemSounds(void) {
    btlOpenButtonIconResource();
    btlLoadStartupPanelResources();
    sndLoadSysEffLb();
}

u8 btlIsRuntimeAllocated(void) {
    return btlRuntime != NULL;
}

u8 btlIsCurrentActorFullyMarked(void) {
    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    return (((BattleController *)btlRuntime)->flags & 0x06000000) == 0x06000000;
}

BtlRuntimeActivityStatus btlHasPendingRuntimeActivity(void) {
    BtlState *state;
    if (btlIsRuntimeAllocated() == 0) {
        return BTL_RUNTIME_ACTIVITY_NONE;
    }
    state = (BtlState *)btlRuntime;
    if (state->tasks != 0) {
        return BTL_RUNTIME_ACTIVITY_PENDING;
    }
    if ((state->eventFlags & 2) != 0) {
        return BTL_RUNTIME_ACTIVITY_PENDING;
    }
    return state->effect != 0 ? BTL_RUNTIME_ACTIVITY_PENDING
                              : BTL_RUNTIME_ACTIVITY_NONE;
}

void btlResetActorEntryState(void) {
    s32 context = (s32)btlRuntime;
    datUnitWork_t *party = DDS3_GBWK->party;
    u32 i;
    ((BattleController *)context)->moneyEarned = 0;
    ((BattleController *)context)->rewardMacca = 0;
    ((BattleController *)context)->experienceEarned = 0;
    ((BattleController *)context)->epEarned = 0;
    ((BattleController *)context)->totalAp = 0;
    ((BattleController *)context)->specialEnemyDefeats = 0;
    for (i = 0; i < 5; i++) {
        party[i].huntExp = 0;
    }
    memset(((BattleController *)btlRuntime)->itemDrops, 0,
           sizeof(((BattleController *)btlRuntime)->itemDrops));
}

s32 btlCaptureRewardSummary(BrsRewardSummary *rewards) {
    u32 i;

    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    rewards->macca = evtBitCheck(0x290) ? ((BattleController *)btlRuntime)->rewardMacca : 0;
    rewards->mitama = ((BattleController *)btlRuntime)->specialEnemyDefeats;
    rewards->totalExp = ((BattleController *)btlRuntime)->experienceEarned;
    rewards->totalAp = ((BattleController *)btlRuntime)->totalAp;
    btlBossDebugPrintf("btl:----------------------\n");
    btlBossDebugPrintf("btl:money =%d\n", rewards->macca);
    btlBossDebugPrintf("btl:exp   =%d\n", rewards->totalExp);
    btlBossDebugPrintf("btl:ep    =%d\n", rewards->totalAp);
    btlBossDebugPrintf("btl:mitama=%d\n", rewards->mitama);
    for (i = 0; i < 5; i++) {
        rewards->unitApBonus[i] = DDS3_GBWK->party[i].huntExp;
        btlBossDebugPrintf(D_004150B0, rewards->unitApBonus[i], DDS3_GBWK->party[i].unitId);
    }
    btlBossDebugPrintf("btl:----------------------\n");
    for (i = 0; i < 3; i++) {
        rewards->icons[i].id = ((BattleController *)btlRuntime)->itemDrops[i].id;
        rewards->icons[i].param = ((BattleController *)btlRuntime)->itemDrops[i].count;
    }
    return 1;
}

s32 btlResolveQueuedSceneRequestParameters(s32 *outCode, s32 *outParameter) {
    FldNextSceneRequestMapCode mapCode;
    FldNextSceneRequestEntryIndex entryIndex;
    s16 i;

    if (fldConsumeNextSceneRequest(&mapCode, &entryIndex) == FLD_NEXT_SCENE_REQUEST_NO_REQUEST) {
        return 0;
    }
    for (i = 0; i < 0x10; i++) {
        if (D_00435E14[i].mapIdBase + 0xC8 != mapCode) {
            continue;
        }
        *outCode = D_00435E14[i].mapIdBase + 0xC8;
        if (D_00435E14[i].entries[entryIndex].flagB != -1 &&
            evtBitCheck(D_00435E14[i].entries[entryIndex].flagB) != 0) {
            *outParameter = D_00435E14[i].entries[entryIndex].parameterB;
            return 1;
        }
        if (D_00435E14[i].entries[entryIndex].flagA != -1 &&
            evtBitCheck(D_00435E14[i].entries[entryIndex].flagA) != 0) {
            *outParameter = D_00435E14[i].entries[entryIndex].parameterA;
            return 1;
        }
        *outParameter = D_00435E14[i].entries[entryIndex].fallbackParameter;
        return 1;
    }
    return 0;
}

void *btlGetRuntime(void) {
    return btlRuntime;
}

/* Read current HP from a unit-entry address. */
s32 btlReadCurrentUnitHp(datUnitWork_t *entry) {
    return entry->hp;
}

/* Word-return getter; native callers choose whether to truncate MP to 16 bits. */
s32 btlReadCurrentUnitMp(datUnitWork_t *entry) {
    return entry->mp;
}

extern s32 ptyComputeMaxHp(datUnitWork_t *);

extern s32 ptyComputeMaxMp(datUnitWork_t *);

void btlComputeProfileMaxHp(datUnitWork_t *unit) {
    ptyComputeMaxHp(unit);
}

void btlComputeProfileMaxMp(datUnitWork_t *unit) {
    ptyComputeMaxMp(unit);
}

u32 btlComputeSkillAdjustedMaxHp(datUnitWork_t *stats) {
    return datComputeSkillBoostedMaxHp(stats);
}

u32 btlComputeSkillAdjustedMaxMp(datUnitWork_t *stats) {
    return datComputeSkillBoostedMaxMp(stats);
}

extern s32 datAddHp(datUnitWork_t *, s32);

extern s32 datAddMp(datUnitWork_t *, s32);

s32 btlAdjustUnitHp(datUnitWork_t *object, s32 delta) {
    return datAddHp(object, delta);
}

s32 btlAdjustUnitMp(datUnitWork_t *object, s32 delta) {
    return datAddMp(object, delta);
}

/* Cache the skill-adjusted maximum and return current HP clamped to it.
 * The comparison uses the full-width result, not the u16 cache. */
u16 btlRefreshUnitMaximumHpAndClampCurrentHp(datUnitWork_t *entryAddress) {
    datUnitWork_t *entry = entryAddress;
    u32 currentHp = btlReadCurrentUnitHp(entry);
    u32 maxHp = btlComputeSkillAdjustedMaxHp(entryAddress);
    entry->maxhp = maxHp;
    if (maxHp < currentHp) {
        entry->hp = maxHp;
    }
    return entry->hp;
}

/* Cache the skill-adjusted maximum and return current MP clamped to it.
 * The comparison uses the full-width result, not the u16 cache. */
u16 btlRefreshUnitMaximumMpAndClampCurrentMp(datUnitWork_t *entryAddress) {
    u32 currentMp = btlReadCurrentUnitMp(entryAddress);
    u32 maxMp = btlComputeSkillAdjustedMaxMp(entryAddress);
    entryAddress->maxMp = maxMp;
    if (maxMp < currentMp) {
        entryAddress->mp = maxMp;
    }
    return entryAddress->mp;
}

/* Return the low 15 status bits; do not expose the stored high bit. */
u16 btlReadUnitStatusMask(datUnitWork_t *entry) {
    return entry->status & BTL_ENTRY_STATUS_MASK;
}

extern void sdfRaisePackedChannelValue(datUnitWork_t *item, u32 minimumValue);

/* Forward the record and value to the packed low-15-bit status update. */
void btlRaisePartyRecordStatusValue(datUnitWork_t *item, u32 minimumValue) {
    sdfRaisePackedChannelValue(item, minimumValue);
}

INCLUDE_RODATA(const s32, "game/code_001A9910", D_004150B0);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_004366E0);

INCLUDE_SDATA(const s32, "game/code_001A9910", btlRuntime);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_004366E8);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_004366F0);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_004366F8);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436700);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436708);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436710);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436718);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436720);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436728);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436730);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436738);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436740);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436748);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436750);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436758);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436760);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436768);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436770);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436778);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436780);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436788);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436790);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_00436798);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_004367A0);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_004367A8);

INCLUDE_SDATA(const s32, "game/code_001A9910", D_004367B0);

