#include "sdk_ps2.h"
#include "evt_start_event_api.h"
#include "sdf_dev_console_node.h"
#include "sdf_gs_blend.h"
#include "common.h"
#include "file_menu_start_mode.h"

#include "dds3_background_fade_api.h"
#include "mnu_camp_state_ack_api.h"
#include "fld_lmap_task_presence.h"
#include "fld_terminal_scene_state.h"
#include "dds3_admin_control_api.h"
#include "sdf_heap_overlay_block.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "sdf_chip.h"
#include "dds3_task_priority_query.h"
#include "dds3_task_create.h"
#include "sdf_heap_api.h"
#include "sdf_general_heap_api.h"
#include "scr.h"
#include "dds3_task_lifecycle.h"
#include "brs_task.h"
#include "fld_resource_wait_api.h"
#include "fld_scene_sequence_api.h"
#include "itf.h"
extern void btlCreateStageTestTask(void);
extern s32 btlExitWhenAudioAndTasksIdle(void);
extern void evtBattleStageTestStopTask(void);
extern void evtFreeStage(void);
extern void evtInitEventTest(void);
extern void evtViewerCreateTaskWithSky(void);
extern void fileMenuReleaseNoOp(void);
extern void fileMenuWorkCreate(u32 startBranchFlag);
extern void fileReleaseMenuResources(void);
extern void fldProcNextJump(void);
extern void fldProc_CallSisetuReturn(void);
extern void fldRunPendingSceneAction(void);
extern void fldShutdownSceneTasksAndWorld(void);
extern void fldStopLmapTask(void);
extern void fldStopSceneTasks(void);
extern void itfInitializeAdminInterfaceNoop(void);
extern void itfShutdownAdminInterfaceNoop(void);
extern void frTestFontInit(void);
extern void mdlCreateViewerPackageTask(void);
extern u32 mdlStopViewerPackageTask(void);
extern void mnuCampDestroyPanelTasks(void);
extern void dds3ConfigProcessEnd(void);
extern void mnuCreateCampShopPopupTasks(const s32 *stockGroup);
extern void mnuCreateCampTasks(void);
extern void dds3ConfigProcessStart(s32 mode);
extern void InitMovieViewer(void);
extern void mnuCreateTerminalSceneTasks(s32 reduced, s32 slot);
extern void mnuDestroyCampTasks(void);
extern void FinishMovieViewer(void);
extern s32 mnuDestroyTitleMenuTask(void);
extern u32 mnuOpenTitleMenuForMode(s32 mode);
extern dds3ProcessID_t *mnuStaffCreateTasks(void);
extern u32 mnuStaffDestroyTasks(void);
extern u32 mnuStartStaffMovieRequest(void);
extern s32 mnuStopStaffTasks(void);
enum {
    EVT_PACKET_LIST_BYTES = 0x20,
    EVT_GS_COMMAND_BYTES = 0x30,
    EVT_GS_TEST_PRIMARY = 0x47,
    EVT_GS_TEST_SECONDARY = 0x48,
    EVT_GS_ALPHA_PRIMARY = 0x42,
    EVT_FIXED_ALPHA_INTERPOLATION = 0x64,
    EVT_QUAD_DEFAULT_DEPTH = 0xFFFFFF,
    EVT_FRAME_REFERENCE_PACKET_BYTES = 0x40,
    EVT_FRAME_DRAW_RECORD_BYTES = 0x1F40,
    EVT_BACKGROUND_BLEND_FLAG = 0x100,
    EVT_DRAW_COLOR_BLEND_FLAG = 0x200,
    EVT_DRAW_VECTOR_BLEND_FLAG = 0x400,
    EVT_SAVED_VECTOR_BLEND_FLAG = 0x4000,
    EVT_QUAD_DESCRIPTOR_BYTES = 0x2C,
    EVT_QUAD_VERTEX_COUNT = 4,
    EVT_SELECTION_ALLOCATION_BYTES = 0x28,
    EVT_DEV_CONSOLE_BUFFER_BYTES = 0x200,
    BF_FLW0_MAGIC = 0x30574C46
};
extern u8 mdlViewerPackageTaskExists(void);
extern u32 D_00435D28;
extern s32 fileMenuOperationSucceeded(void);
extern s32 fmProcessChk(void);
extern void fileStartMemoryCardMenuForMode(FileMenuStartMode mode);
extern u32 D_00435BB0;
extern s32 checkConfigState(void);
extern s32 mnuCampConsumePanelTaskCompletion(void);
extern FldTerminalScenePollResult fldPollSceneState(void);
extern u8 fldLmapTaskExists(void);
extern sdfDevConsCB_tag *evtConsoleFontResourceChain;
extern char D_00411370[];
extern void fldMainLoadStart(void);
extern char D_00435D20[];
extern void evtEventViewerDestroyTask(void);
extern char D_00411380[];
extern s32 D_00389780[];
extern void fldStartLmapTask(s32 mode);
extern u32 scrHandleCount;
extern sdfGrOt_t sdfDebugGrOt;
extern s8 D_0037F53B[];
extern void evtDrawHeapUsageOverlay(sdfGrOt_t *);
#include "sdf_packet_alloc.h"
extern s32 dds3StartMenuProcess(dds3ProcessID_t *task);
extern dds3ProcessID_t *evtInitSky(void);
extern void evtChkEndProcess(void);
extern void evtExitSky(void);
extern void btlStart(s32 mode, s32 pack, s32 encounter);
extern s32 evtDrawGeneralHeapUsageGraph(struct dds3ProcessID_t *);
extern void *D_00438E64;
extern void *sdfDevPrintf(s32, s32, s32, s32, const char *, ...);
#include "fpu.h"

dds3ProcessID_t *CallDebug(void) {
    return dds3StartProcess(D_00411370, 0x2AF9, 0, 0, dds3StartMenuProcess, 0, 0);
}

u32 ExitDebug(void) {
    dds3KillProcessName(D_00411370, 1);
    return 1;
}

/* Admin mode 0 cleanup: zero leaves the mode pending. */
u32 CheckDebug(void) {
    return 0;
}

void CallBootMc(void) {
    fileStartMemoryCardMenuForMode(FILE_MENU_START_MODE_BOOT);
}

u32 ExitBootMc(void) {
    s32 operationResult;
    s32 status;

    fileMenuReleaseNoOp();
    fileReleaseMenuResources();
    operationResult = fileMenuOperationSucceeded();
    status = -1;
    if (operationResult != 0) {
        status = 0;
    }
    return status;
}

u8 CheckBootMc(void) {
    s32 operationResult;

    operationResult = fmProcessChk();
    return operationResult == 0;
}

u32 CallTitle(u32 unused, u32 *modeOverride) {
    if (modeOverride == 0) {
        return mnuOpenTitleMenuForMode(0);
    }
    return mnuOpenTitleMenuForMode(*modeOverride);
}

u32 ExitTitle(void) {
    mnuDestroyTitleMenuTask();
    return 0;
}

/* Admin mode 2 cleanup: zero leaves the mode pending. */
u32 CheckTitle(void) {
    return 0;
}

void CallLoad(void) {
    fileStartMemoryCardMenuForMode(FILE_MENU_START_MODE_SLOT_SELECTION);
}

s32 ExitLoad(void) {
    fileMenuReleaseNoOp();
    fileReleaseMenuResources();
    if (fileMenuOperationSucceeded() != 0) {
        fldMainLoadStart();
        return 0;
    }
    return -1;
}

u8 CheckLoad_dds3SequenceList(void) {
    s32 operationResult;

    operationResult = fmProcessChk();
    return operationResult == 0;
}

u32 CallStaff(void) {
    return mnuStartStaffMovieRequest();
}

u32 ExitStaff(void) {
    mnuStopStaffTasks();
    return 0;
}

/* Admin mode 12 cleanup: zero leaves the mode pending. */
u32 CheckStaff(void) {
    return 0;
}

void CallEndSave(void) {
    fileStartMemoryCardMenuForMode(FILE_MENU_START_MODE_NEW_CYCLE);
}

u32 ExitEndSave(void) {
    s32 operationResult;
    s32 status;

    fileMenuReleaseNoOp();
    fileReleaseMenuResources();
    operationResult = fileMenuOperationSucceeded();
    status = -1;
    if (operationResult != 0) {
        status = 0;
    }
    return status;
}

u8 CheckEndSave(void) {
    s32 operationResult;

    operationResult = fmProcessChk();
    return operationResult == 0;
}

void CallField(u32 selection, const struct fldSceneParam_t *record) {
    dds3FadeBackgroundStartOut(0);
    fldProcTrigSequence(record, selection);
}

u32 ExitField(void) {
    FldResourceWaitResult operationResult;
    s32 status;

    fldShutdownSceneTasksAndWorld();
    operationResult = fldIsFieldResourceWaitFinished();
    status = -1;
    if (operationResult != FLD_RESOURCE_WAIT_PENDING) {
        status = 0;
    }
    return status;
}

/* Admin mode 5 cleanup: zero leaves the mode pending. */
u32 CheckField(void) {
    return 0;
}

void evtStartSelectionFadeOut(void) {
    dds3FadeBackgroundStartOut(0);
}

/* Admin mode 11 destroy callback: no work, zero transition result. */
u32 evtDestroySelectionFadeOutNoOp(void) {
    return 0;
}

/* Admin mode 11 cleanup: zero leaves the mode pending. */
u32 evtKeepSelectionFadeOutPending(void) {
    return 0;
}

/* Source zero takes an explicit event ID; source one uses the pending event ID. */
void CallEvent(s32 selectionSource, EvtScriptEventId *eventParams) {
    EvtScriptEventId eventIdToDispatch = 0;

    D_00435BB0 = 0;
    switch (selectionSource) {
    case 0:
        eventIdToDispatch = eventParams[0];
        break;
    case 1:
        eventIdToDispatch = evtPendingEventSelection;
        break;
    }
    if (eventIdToDispatch <= 0) {
        return;
    }
    evtInitSky();
    evtStartEvent(eventIdToDispatch);
}

u32 ExitEvent(void) {
    D_00435BB0 = 1;
    evtExitSky();
    fldProcNextJump();
    return 0;
}

/* Admin mode 6 cleanup: zero leaves the mode pending. */
u32 CheckEvent(void) {
    return 0;
}

void CallCamp(void) {
    mnuCreateCampTasks();
}

u32 ExitCamp(void) {
    mnuDestroyCampTasks();
    return 0;
}

u8 CheckCamp(void) {
    MnuCampStateAcknowledgementResult state;

    state = campCheckExitState();
    return state == MNU_CAMP_STATE_ACK_NOT_ACTIVE;
}

/* Admin mode 8 entry intentionally performs no work. */
void CallRecover(void) {
}

/* Admin mode 8 destroy callback: no work, zero transition result. */
u32 ExitRecover(void) {
    return 0;
}

/* Admin mode 8 cleanup: one immediately reports completion. */
u32 CheckRecover(void) {
    return 1;
}

/* Admin mode 9 entry intentionally performs no work. */
void CallTerminal(void) {
}

/* Admin mode 9 destroy: no work, zero transition result. */
u32 ExitTerminal(void) {
    return 0;
}

/* Admin mode 9 cleanup: one immediately reports completion. */
u32 CheckTerminal(void) {
    return 1;
}

s32 CallResult2(void) {
    return (s32)mnuStaffCreateTasks();
}

u32 ExitResult2(void) {
    fldProcNextJump();
    mnuStaffDestroyTasks();
    return 0;
}

u8 CheckResult2(void) {
    BrsTaskPollResult done;

    done = brsTaskConsumeDone();
    return done == BRS_TASK_POLL_RESULT_INACTIVE;
}

void CallConfig(void) {
    dds3ConfigProcessStart(1);
}

u32 ExitConfig(void) {
    dds3ConfigProcessEnd();
    return 0;
}

u8 CheckConfig(void) {
    s32 result;

    result = checkConfigState();
    return result == 0;
}

/* Source zero forwards an optional parameter pair; other sources resume deferred field work. */
void evtDispatchSelectionCommand(s32 commandSource, s32 *commandParams) {
    if (commandSource == 0) {
        if (commandParams == NULL) {
            btlStart(1, 0, 0);
        }
        else {
            btlStart(0, commandParams[0], commandParams[1]);
        }
    }
    else {
        dds3AdminiExitSequence();
        fldProcNextJump();
    }
}

/* Check battle shutdown; the callee exits only after audio and task work become idle. */
s32 evtCheckBattleShutdown(void) {
    return btlExitWhenAudioAndTasksIdle();
}

u32 CheckNewBattleTest(void) {
    if (dds3SearchProcessName(D_00435D20) != NULL) {
        return 0;
    }
    if (dds3SearchProcessPriority(0x3FA) == NULL) {
        fldProcNextJump();
        return 1;
    }
    return 0;
}

void evtCreateBattleStageTestTask(void) {
    btlCreateStageTestTask();
}

/* Stop the battle-stage test task and return the scene callback's native zero status. */
u32 evtEndBattleStageTest(void) {
    evtBattleStageTestStopTask();
    return 0;
}

/* Admin mode 15 cleanup: zero leaves the mode pending. */
u32 evtKeepBattleStageTestModePending(void) {
    return 0;
}

dds3ProcessID_t *evtCreateTestAndSkyTasks(void) {
    D_00435BB0 = 0;
    D_00435D28 = 0;
    evtInitEventTest();
    return evtInitSky();
}

u32 evtInitializeSelectionScene(void) {
    D_00435BB0 = 1;
    dds3FadeBackgroundStartOut(0);
    evtChkEndProcess();
    evtExitSky();
    return 1;
}

/* Admin mode 16 cleanup: zero leaves the mode pending. */
u32 evtKeepTestSkyModePending(void) {
    return 0;
}

void evtClearSecondaryWorldAndViewer(void) {
    D_00435BB0 = 0;
    evtFreeStage();
    evtViewerCreateTaskWithSky();
}

u32 evtInitializeSelectionScreen(void) {
    evtEventViewerDestroyTask();
    D_00435BB0 = 1;
    dds3FadeBackgroundStartOut(0);
    return 0;
}

/* Admin mode 17 cleanup: zero leaves the mode pending. */
u32 evtKeepEventViewerModePending(void) {
    return 0;
}

/* Admin mode 18 entry intentionally performs no work. */
void evtAdminModeEighteenEntryNoOp(void) {
}

/* Admin mode 18 destroy: no work, zero transition result. */
u32 evtAdminModeEighteenDestroyNoOp(void) {
    return 0;
}

/* Admin mode 18 cleanup: zero leaves the mode pending. */
u32 evtKeepAdminModeEighteenPending(void) {
    return 0;
}

void evtCreateCampTasksAlternate(void) {
    mnuCreateCampTasks();
}

u32 evtExitCampTaskGroup(void) {
    mnuDestroyCampTasks();
    return 0;
}

u8 mnuCheckCampStateAndAcknowledge(void) {
    MnuCampStateAcknowledgementResult state;

    state = campCheckExitState();
    return state == MNU_CAMP_STATE_ACK_NOT_ACTIVE;
}

void evtCreateSkyAndCampTasks(u32 unused, u32 campMode) {
    evtInitSky();
    mnuCreateCampShopPopupTasks(campMode);
}

u32 evtDestroySkyAndCampTasks(void) {
    evtExitSky();
    mnuCampDestroyPanelTasks();
    fldProc_CallSisetuReturn();
    return 0;
}

u8 CheckRecoverTest(void) {
    s32 result;

    result = mnuCampConsumePanelTaskCompletion();
    return result == 0;
}

void CallTerminalTest(u32 unused, u32 *fieldArgs) {
    evtInitSky();
    if (fieldArgs != 0) {
        mnuCreateTerminalSceneTasks(fieldArgs[0], fieldArgs[1]);
        return;
    }
    mnuCreateTerminalSceneTasks(0, 1);
}

u32 ExitTerminalTest(void) {
    evtExitSky();
    fldStopSceneTasks();
    return 0;
}

u8 CheckTerminalTest(void) {
    FldTerminalScenePollResult pollResult;

    pollResult = fldPollSceneState();
    return pollResult == FLD_TERMINAL_SCENE_POLL_NOT_PENDING;
}

s32 evtCreateStaffTasksAlternate(void) {
    return (s32)mnuStaffCreateTasks();
}

u32 evtDestroyStaffTasksAlternate(void) {
    mnuStaffDestroyTasks();
    return 0;
}

u8 evtWaitStaffTaskDoneAlternate(void) {
    BrsTaskPollResult done;

    done = brsTaskConsumeDone();
    return done == BRS_TASK_POLL_RESULT_INACTIVE;
}

/* Admin mode 23 entry intentionally performs no work. */
void CallSoundTest(void) {
}

/* Admin mode 23 destroy: no work, zero transition result. */
u32 ExitSoundTest(void) {
    return 0;
}

/* Admin mode 23 cleanup: zero leaves the mode pending. */
u32 CheckSoundTest(void) {
    return 0;
}

/* Admin mode 24 entry intentionally performs no work. */
void CallBillboardTest(void) {
}

/* Admin mode 24 destroy: no work, zero transition result. */
u32 ExitBillboardTest(void) {
    return 0;
}

/* Admin mode 24 cleanup: zero leaves the mode pending. */
u32 CheckBillboardTest(void) {
    return 0;
}

void CallFontTest(void) {
    frTestFontInit();
}

/* Admin mode 25 destroy callback: no work, zero transition result. */
u32 ExitFontTest(void) {
    return 0;
}

u32 CheckFontTest(void) {
    return dds3SearchProcessName(D_00411380) == NULL;
}

void evtCreateMovieViewerTask(void) {
    InitMovieViewer();
}

u32 evtDestroyMovieViewerTask(void) {
    FinishMovieViewer();
    return 0;
}

/* Admin mode 26 cleanup: zero leaves the mode pending. */
u32 evtKeepMovieViewerModePending(void) {
    return 0;
}

void evtEnterInterfaceMode(void) {
    itfInitializeAdminInterfaceNoop();
}

u32 evtDestroyInterfaceMode(void) {
    itfShutdownAdminInterfaceNoop();
    return 0;
}

u32 evtKeepInterfaceModePending(void) {
    return 0;
}

void evtSelectFontResource(s32 unused, s32 *lmapArgs) {
    if (lmapArgs == NULL) {
        fldStartLmapTask(0);
    }
    else {
        fldStartLmapTask(lmapArgs[0]);
    }
    D_00389780[0] = 0x3E7;
}

u32 evtDestroyLmapTask(void) {
    fldStopLmapTask();
    fldRunPendingSceneAction();
    return 0;
}

u8 evtWaitLmapTaskGone(void) {
    u8 taskPresence;

    taskPresence = fldLmapTaskExists();
    return taskPresence == FLD_LOCAL_MAP_TASK_ABSENT;
}

/* Start the shooting-game task: the callee allocates MnuShootingWork and starts "stgProcess". */
void evtStartShootingGameTask(void) {
    mdlCreateViewerPackageTask();
}

/* Destroy that shooting-game task hierarchy and invoke its work callback; return native zero status. */
u32 evtStopShootingGameTask(void) {
    mdlStopViewerPackageTask();
    return 0;
}

/* Complete the wait only after the named "stgProcess" task is absent. */
u8 evtWaitShootingGameTaskGone(void) {
    s32 taskExists;

    taskExists = mdlViewerPackageTaskExists();
    return taskExists == 0;
}

void evtOpenFileMenuFromParams(u32 unused, u32 *menuArgs) {
    u32 menuArg;

    menuArg = 0;
    if (menuArgs != 0) {
        menuArg = *menuArgs;
    }
    fileMenuWorkCreate(menuArg);
    fileStartMemoryCardMenuForMode(FILE_MENU_START_MODE_ORIGINAL_TITLE);
}

extern void fileReleaseMenuFlowResource();

s32 evtPollFileMenuAndRelease(void) {
    fileMenuReleaseNoOp();
    fileReleaseMenuResources();
    if (fileMenuOperationSucceeded() == 0) {
        return -1;
    }
    fileReleaseMenuFlowResource();
    return 0;
}

u8 evtWaitFileMenuTaskFour(void) {
    s32 operationResult;

    operationResult = fmProcessChk();
    return operationResult == 0;
}

u32 evtEnsureFontResourceChain(void) {
    if (evtConsoleFontResourceChain == 0) {
        evtConsoleFontResourceChain = sdfDevConsOpen(0x7100, 0x7A60, 0x28, 0x14);
        sdfDevConsSetMode(evtConsoleFontResourceChain, 2);
        sdfDevConsAttribute(evtConsoleFontResourceChain, 7);
    }
    return 0;
}

void evtDestroyFontResourceChain(void) {
    if (evtConsoleFontResourceChain != 0) {
        sdfDevConsClose(evtConsoleFontResourceChain);
        evtConsoleFontResourceChain = 0;
    }
}

/* printf into the dev console node when one exists. */

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", D_00435D20);

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", D_00435D28);

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", evtConsoleFontResourceChain);

void datDebugPrintf(const char *format, ...) {
    char messageBuffer[EVT_DEV_CONSOLE_BUFFER_BYTES];
    __builtin_va_list formatArgs;

    __builtin_stdarg_start(formatArgs, format);
    if (evtConsoleFontResourceChain != 0) {
        vsprintf(messageBuffer, format, formatArgs);
        sdfDevConsPrintf(evtConsoleFontResourceChain, "%s", messageBuffer);
    }
}

extern u16 D_00435D40;
extern u16 D_00435D42;
extern char evtGeneralHeapPercentFormat[];

extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);

s32 evtDrawGeneralHeapUsageGraph(struct dds3ProcessID_t *task) {
    SdfGeneralHeapStats heapStats;
    sdfDrawTag_t *list;
    void *packet;
    s32 freePercent;
    s32 originX = 0x7100;
    s32 currentColumn;
    s32 peakColumn;

    sdfMemGetInfo(&heapStats);
    freePercent = heapStats.freeBytes * 100 / heapStats.totalBytes;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(packet);
    sdfDrawTagLink(list, (u32)packet);
    sdfDrawTagLink(list, (u32)dds3PrimFrameRect(0x70D0, 0x85A8, 0xFFFF7F, 0x1260, 0xC0, 0x20000000, 0x40806040));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7100, 0x85C0, 0xFFFF80, 0, evtGeneralHeapPercentFormat, freePercent));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x8080, 0x85C0, 0xFFFF80, 6, evtGeneralHeapPercentFormat, D_00435D40));
    currentColumn = (s32)((f32)freePercent / 100.0f * 200.0f) + 0x2A;
    sdfPrimLinkRect(list, 0x8000C000, 0, 0x73A0, 0x85F0, currentColumn * 16 + originX, 0x8600, 0xFF0000, 0);
    peakColumn = (s32)((f32)D_00435D40 / 100.0f * 200.0f) + 0x2B;
    sdfPrimLinkRect(list, 0x8000FFFF, 0, peakColumn * 16 + originX, 0x85D0, (peakColumn + 2) * 16 + originX, 0x8610, 0xFF0000, 0);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    if (D_00435D42 >= 0x47 || D_00435D40 < freePercent) {
        D_00435D40 = freePercent;
        D_00435D42 = 0;
    } else {
        D_00435D42 = D_00435D42 + 1;
    }
    return 0;
}

void evtGeneralHeapGraphTaskDestroy(dds3ProcessID_t *task) {
}

INCLUDE_RODATA(const s32, "kernel/dds3SequenceList", D_00411370);

INCLUDE_RODATA(const s32, "kernel/dds3SequenceList", D_00411380);

void evtToggleDebugTimeGraphTask(s8 mode) {
    if (mode == 1) {
        D_00438E64 = dds3StartProcess("DebugTimeGrph", 0x2710, 1, 1, evtDrawGeneralHeapUsageGraph, evtGeneralHeapGraphTaskDestroy, 0);
    } else if (mode == 0) {
        dds3KillProcess(D_00438E64, 0);
    }
}

extern u32 D_00435D4C;
/* Render an allocation span as partial rows and complete heap-map rows. */
void sdfDrawHeapSpanOverlay(sdfDrawTag_t *packetList, s32 x, s32 y,
                   u32 heapBase, u32 blockAddress, s32 blockSize, u8 blockState) {
    s32 color;
    u32 byteOffset;
    s32 rowIndex;
    s32 columnIndex;
    s32 leftX;
    s32 topY;
    u32 bytesPerRow;
    u32 bytesPerCell;

    switch (blockState) {
    case SDF_MEM_BLOCK_STATE_FREE:
        color = 0x80000000;
        break;
    case SDF_MEM_BLOCK_STATE_USED:
        color = 0x80008080;
        break;
    case SDF_MEM_BLOCK_STATE_END_SENTINEL:
        color = 0x80000080;
        break;
    default:
        color = 0x80800000;
        break;
    }
    byteOffset = blockAddress - heapBase;
    bytesPerRow = D_00435D4C >> 8;
    bytesPerCell = D_00435D4C >> 14;
    rowIndex = byteOffset / bytesPerRow;
    columnIndex = (byteOffset % bytesPerRow) / bytesPerCell;
    if (D_00435D4C < blockSize || blockSize < 0) {
        return;
    }

    topY = y + rowIndex * 8;
    leftX = x + columnIndex * 16;
    if (columnIndex > 0) {
        s32 remainingCells = 64 - columnIndex;

        if (blockSize >= remainingCells / bytesPerCell) {
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + remainingCells * 16, topY + 8,
                                         0x0FFFFF80, NULL);
            blockSize -= remainingCells / (D_00435D4C >> 14);
        } else {
            s32 cellCount = blockSize / bytesPerCell;

            if (cellCount == 0 && blockSize > 0) {
                cellCount = 1;
            }
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + cellCount * 16, topY + 8,
                                         0x0FFFFF80, NULL);
            blockSize = 0;
        }
        topY += 8;
    }
    while (blockSize >= (D_00435D4C >> 8)) {
        sdfPrimLinkLine(packetList, color, 0, x, topY,
                                     x + 0x400, topY + 8, 0x0FFFFF80, NULL);
        blockSize -= D_00435D4C >> 8;
        topY += 8;
    }
    if (blockSize > 0) {
        sdfPrimLinkLine(packetList, color, 0, x, topY,
                                     x + (blockSize / (D_00435D4C >> 14)) * 16,
                                     topY + 8, 0x0FFFFF80, NULL);
    }
}

void sdfDrawHeapMapOverlay(sdfDrawTag_t *packetList, s32 x, s32 y) {
    SdfHeapOverlayBlock *snapshot;
    SdfHeapOverlayBlock *entry;
    const sdfMemHandle_t *block;
    u32 count;
    u32 i;
    u32 heapBase;
    u32 offset;
    s32 restoreInterrupts;
    s32 markerX;
    s32 markerY;

    snapshot = (SdfHeapOverlayBlock *)sdfGrOpenPacketBuff();
    restoreInterrupts = DIntr();
    block = sdfMemGetTop();
    entry = snapshot;
    heapBase = sdfMemGetPtr(block);
    /* Count only the interrupt-protected snapshot traversal. */
    count = 0;
    do {
        count++;
        entry->address = sdfMemGetPtr(block);
        entry->blockState = (u8)sdfMemGetType(block);
        entry->size = sdfMemGetSize(block);
        entry++;
        block = sdfMemGetNext(block);
    } while (block != NULL);
    if (restoreInterrupts) {
        EIntr();
    }
    sdfGrClosePacketBuff((s32)entry);

    entry = snapshot;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(x - 0x20, y - 0x10,
        0x0FFFFF7F, 0x440, (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x20,
        0x80000000, 0x80806020));
    for (i = 0; i < count; i++) {
        sdfDrawHeapSpanOverlay(packetList, x, y, heapBase, entry->address,
            entry->size, entry->blockState);
        entry++;
    }
    markerY = y;
    markerX = x + 0x420;
    sdfPrimLinkLine(packetList, 0x80806020, 0, markerX, markerY,
        x + 0x4C0, markerY, 0x0FFFFF80, NULL);
    for (offset = 0x100000; offset < D_00435D4C; offset += 0x100000) {
        markerY = y + (offset / (D_00435D4C >> 8)) * 8;
        sdfPrimLinkLine(packetList, 0x80806020, 0, markerX, markerY,
            x + 0x470, markerY, 0x0FFFFF80, NULL);
    }
    markerY = y + (D_00435D4C / (D_00435D4C >> 8)) * 8;
    sdfPrimLinkLine(packetList, 0x80806020, 0, markerX, markerY,
        markerX + 0xA0, markerY, 0x0FFFFF80, NULL);
}

extern char D_00435D50[];
extern char D_00435D58[];
extern void sdfDrawHeapMapOverlay(sdfDrawTag_t *packetList, s32 x, s32 y);

/* Format general/chip free-memory statistics into the supplied draw surface.
   Keep the native heap-ratio coordinate calculation and title-specific initial packet. */
void evtDrawHeapUsageOverlay(sdfGrOt_t *surface) {
    SdfGeneralHeapStats generalHeapStats;
    SdfChipHeapStats chipHeapStats;
    char statusText[100];
    sdfDrawTag_t *packetList;

    sdfMemGetInfo(&generalHeapStats);
    D_00435D4C = generalHeapStats.totalBytes;
    sdfChipGetInfo(&chipHeapStats);
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(packetList);
    sdfDrawHeapMapOverlay(packetList, 0x8AC0, 0x79C0);
    sprintf(statusText, D_00435D50, generalHeapStats.freeBytes);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x86C0,
        (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x7A00, 0x0FFFFF80, 0, statusText));
    sprintf(statusText, D_00435D58, chipHeapStats.freeBytes);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x86C0,
        (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x7A60, 0x0FFFFF80, 0, statusText));
    surface->regFunc(surface, packetList);
}

/* Draw the overlay only when the control byte is zero; return zero in either case. */
s32 evtDrawConditionalHeapUsageOverlay(void) {
    if (D_0037F53B[0] != 0) {
        return 0;
    }
    evtDrawHeapUsageOverlay(&sdfDebugGrOt);
    return 0;
}

extern s32 frGetFontHandleNum(void);
extern s32 frGetTextHandleNum(void);
extern s32 fmGetRestBufNum(void);
extern const char D_004113A0[];

s32 evtDrawResourceCountDebugTask(dds3ProcessID_t *task) {
    SdfGeneralHeapStats heapStats;
    s32 heapRatio;
    sdfDrawTag_t *list;
    void *packet;
    s32 fontCount;
    s32 textCount;
    s32 gsCount;

    sdfMemGetInfo(&heapStats);
    /* The unused heap ratio retains the native zero-divisor check. */
    heapRatio = heapStats.freeBytes / heapStats.totalBytes;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(packet);
    sdfDrawTagLink(list, (u32)packet);
    sdfDrawTagLink(list, (u32)dds3PrimFrameRect(0x70d0, 0x7968, 0xffff7f,
                                          0xf70, 0x98, 0x20000000, 0x40806040));
    fontCount = frGetFontHandleNum();
    textCount = frGetTextHandleNum();
    gsCount = fmGetRestBufNum();
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7100, 0x7980,
                    0xffff80, 0, D_004113A0, fontCount, textCount, gsCount));
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    return 0;
}

void evtResourceCountDebugTaskDestroy(dds3ProcessID_t *task) {
}

extern s32 evtDrawResourceCountDebugTask(dds3ProcessID_t *task);
extern void evtResourceCountDebugTaskDestroy(dds3ProcessID_t *task);
extern dds3ProcessID_t *D_00438E68;
extern char D_004113B8[]; /* "DebugTimeGrph" */

void evtToggleAlternateDebugTimeGraphTask(s8 mode) {
    if (mode == 1) {
        D_00438E68 = dds3StartProcess(D_004113B8, 0x2710, 1, 1, evtDrawResourceCountDebugTask, evtResourceCountDebugTaskDestroy, 0);
    } else if (mode == 0) {
        dds3KillProcess(D_00438E68, 0);
    }
}

/* Append to the event-work doubly linked list, maintaining both endpoints. */
void scrLinkHandle(scrProcessWork_s *node) {
    scrProcessWork_s *tail = scrHandleLast;

    if (tail == NULL) {
        scrHandleFirst = node;
        scrHandleLast = node;
        node->prev = NULL;
        node->next = NULL;
    }
    else {
        node->prev = tail;
        tail->next = node;
        node->next = NULL;
        scrHandleLast = node;
    }
    scrHandleCount++;
}

/* Detach from either end or the middle, and clear the old links. */
void scrUnlinkHandle(scrProcessWork_s *node) {
    if (scrHandleFirst == node) {
        scrHandleFirst = node->next;
    }
    else {
        node->prev->next = node->next;
    }
    if (scrHandleLast == node) {
        scrHandleLast = node->prev;
    }
    else {
        node->next->prev = node->prev;
    }
    node->prev = NULL;
    node->next = NULL;
    scrHandleCount--;
}

extern ItfMesWindowIndex itfMesMngInitialize(ItfMesSub *data);

INCLUDE_RODATA(const s32, "kernel/dds3SequenceList", D_004113A0);

INCLUDE_RODATA(const s32, "kernel/dds3SequenceList", D_004113B8);

scrProcessWork_s *scrPrintProcedureStart(const scrHeader_t *header, const scrTypeTable_t *procedureSection,
                        const scrProcTable_t *procedures, const scrLabelTable_t *labels,
                        const scrCode_u *instructions, struct ItfMesSub *auxiliaryData,
                        const char *strings, ScrProcedureIndex procedureIndex) {
    scrProcessWork_s *process;
    s32 i;
    u8 *types;
    scrStackData_u *values;

    if (header == NULL || procedureSection == NULL || procedures == NULL ||
        instructions == NULL || procedureIndex < 0 ||
        procedureIndex >= procedureSection->ItemCount) {
        return NULL;
    }
    process = sdfChipAllocDirect(0xF4);
    if (process == NULL) {
        return NULL;
    }
    i = 0;
    while ((((char *)process->Name)[i] = procedures[procedureIndex].Name[i]) != '\0') {
        i++;
    }
    process->CP = procedures[procedureIndex].CodeAddress;
    process->SP = 0;
    values = process->StackData;
    types = process->StackType;
    for (i = SCR_STACK_RETURN_VALUE_SLOT; i >= 0; i--) {
        *types++ = 0;
        values++->i = 0;
    }
    process->TypeTable = procedureSection;
    process->LabelTable = labels;
    process->Code = instructions;
    process->StringData = (u8 *)strings;
    process->StartProc = procedureIndex;
    process->MesHandle = ITF_MES_WINDOW_INDEX_INVALID;
    process->Header = header;
    process->ProcTable = procedures;
    process->MessageData = (u8 *)auxiliaryData;
    process->ScriptTimer = 0;
    process->CommandTimer = 0;
    process->MemHandle = NULL;
    process->LocalInt = NULL;
    process->LocalFloat = NULL;
    process->ProcessID = NULL;
    process->prev = NULL;
    process->next = NULL;
    if (header->LocalIntCount > 0) {
        process->LocalInt = sdfChipAllocDirect(header->LocalIntCount * sizeof(ScrVmInteger));
        for (i = 0; i < header->LocalIntCount; i++) {
            process->LocalInt[i] = 0;
        }
    } else {
        process->LocalInt = NULL;
    }
    if (header->LocalFloatCount > 0) {
        process->LocalFloat = sdfChipAllocDirect(header->LocalFloatCount * sizeof(f32));
        for (i = 0; i < header->LocalFloatCount; i++) {
            process->LocalFloat[i] = 0;
        }
    } else {
        process->LocalFloat = NULL;
    }
    if (auxiliaryData != NULL) {
        process->MesHandle = itfMesMngInitialize(auxiliaryData);
    }
    scrLinkHandle(process);
    datDebugPrintf("start <%s>\n", procedures[procedureIndex].Name);
    return process;
}

/* Resolve supported FLW0 sections relative to the header; reject bad magic/unknown kinds.
   Zero-count auxiliary sections are ignored, and absent sections remain null. */
scrProcessWork_s *fldFlowParse(const scrHeader_t *header, ScrProcedureIndex procedureIndex) {
    const scrTypeTable_t *sections;
    const scrProcTable_t *procedures = NULL;
    const scrLabelTable_t *labels = NULL;
    const u8 *instructionSection = NULL;
    struct ItfMesSub *auxiliaryData = NULL;
    const char *strings = NULL;
    s32 i;

    sections = (const scrTypeTable_t *)(header + 1);
    if (*(s32 *)header->MagicCode != BF_FLW0_MAGIC) {
        return 0;
    }
    for (i = 0; i < header->TypeTableCount; i++) {
        switch (sections[i].Type) {
            case SCR_DATATYPE_PROCTABLE:
                procedures = (const scrProcTable_t *)((const u8 *)header + sections[i].ItemAddress);
                break;
            case SCR_DATATYPE_LABELTABLE:
                labels = (const scrLabelTable_t *)((const u8 *)header + sections[i].ItemAddress);
                break;
            case SCR_DATATYPE_SCRDATA:
                instructionSection = (const u8 *)header + sections[i].ItemAddress;
                break;
            case SCR_DATATYPE_MESSAGE:
                if (sections[i].ItemCount != 0) {
                    auxiliaryData = (struct ItfMesSub *)((u8 *)header + sections[i].ItemAddress);
                }
                break;
            case SCR_DATATYPE_STRINGS:
                strings = (const char *)header + sections[i].ItemAddress;
                break;
            default:
                return 0;
        }
    }
    return scrPrintProcedureStart(header, sections, procedures, labels,
                           (const scrCode_u *)instructionSection,
                           auxiliaryData, strings, procedureIndex);
}

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", D_00435D40);

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", D_00435D42);

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", evtGeneralHeapPercentFormat);

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", D_00435D4C);

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", D_00435D50);

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", D_00435D58);

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", scrHandleCount);

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", scrHandleFirst);

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", scrHandleLast);

