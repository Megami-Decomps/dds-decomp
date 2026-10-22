#include "sdk_ps2.h"
#include "common.h"
#include "kwln_draw_buffer.h"
#include "sdf_timer.h"
#include "kwln.h"
#include "kwln_task_create.h"

extern void kwlnUpdatePadInputState(void);

extern s8 D_00435B88;
extern u32 D_00435B84;
extern u32 D_00435B9C;
extern u32 D_00435BA0;

extern u32 D_00435B80;

extern u32 D_00438D80;

extern u32 D_00435B8C;

extern s32 D_00435B90;

extern u32 D_00435B94;
extern u32 D_00435B98;

extern u8 D_00435B89;

extern KwlnTask *kwlnDelayedStartTaskHead;
extern KwlnTask *D_00435BD4;
extern s32 kwlnDelayedStartTaskCount;
extern KwlnTask *kwlnDelayedDestroyTaskHead;
extern KwlnTask *D_00435BE0;
extern s32 kwlnDelayedDestroyTaskCount;
extern KwlnTask *kwlnActiveTaskHead;
extern KwlnTask *D_00435BEC;
extern s32 kwlnActiveTaskCount;

extern u8 D_00435BB8[];
extern u8 D_00435BC0[];
extern s32 kwlnPrepareFrameDrawPackets(KwlnTask *task);
extern s32 kwlnBuildFrameScenePackets(KwlnTask *task);
extern s32 kwlnRenderFrame(KwlnTask *task);
extern s32 dds3BasicMainProcess(KwlnTask *task);
extern s32 dds3BasicDrawProcess(KwlnTask *task);
extern s32 frFlipPacketBuf(KwlnTask *task);
extern void frDestroyFontTable(KwlnTask *task);
extern s32 dds3DebugDevDumpProcess(KwlnTask *task);

struct EffWorldNode;
extern struct EffWorldNode * dds3AddWorld(s32 capacity);
extern void mdlInitManager(void);
extern void btlLoadInputIconsAndSystemSounds(void);
extern void evtEnsureSolarOverlayTaskAndResetPhase(void);
extern void dds3InitDrawEnv(void);
extern void fileSysInit(void);
extern void fldCreateEncounterTask(void);
extern void fldCreateFieldEffectTask(void);
extern void fldFirstInit(void);
extern void frInitFontTable(s32 rowBudget, s32 poolCount);
extern void itfMesMngInitializeManager(void);
extern void dds3InitAdmini(void);
extern void dds3FadeClear(void);
extern void dds3InitRandCB(void);
extern void dds3PadInitialize(void);
extern void dds3InitDebugSystem(void);
extern void dds3StartMovieManager(void);
extern void mnuInitializeTitleAudioAndEffects(void);
extern void mnuLoadCampResources(void);
extern void sdfCreateRuntimeTask(void);
extern void sdfDevConsInit(void);
extern void sdfResetChannels(void);

extern void dds3AdminiCallSequence(s32 requestedMode, void *requestData, u32 dataBytes, s32 markHistory);

void kwlnInitializeRuntime(void) {
    dds3InitDrawEnv();
    dds3PadInitialize();
    dds3StartProcess(D_00435BB8, 0, 0, 0, kwlnPrepareFrameDrawPackets, 0, 0);
    dds3StartProcess(D_00435BC0, 0x2710, 0, 0, kwlnBuildFrameScenePackets, 0, 0);
    dds3StartProcess("RequestDraw", 0x4E20, 0, 0, kwlnRenderFrame, 0, 0);
    dds3StartProcess("basic_main", 0x3FC, 1, 1, dds3BasicMainProcess, 0, 0);
    dds3StartProcess("basic_draw", 0x2B09, 1, 1, dds3BasicDrawProcess, 0, 0);
    frInitFontTable(0x100, 0x200);
    dds3StartProcess("font_sys", 0x4E1F, 0, 0, frFlipPacketBuf, frDestroyFontTable, 0);
    fileSysInit();
    itfMesMngInitializeManager();
    mnuInitializeTitleAudioAndEffects();
    sdfDevConsInit();
    mdlInitManager();
    fldCreateFieldEffectTask();
    fldFirstInit();
    sdfResetChannels();
    btlLoadInputIconsAndSystemSounds();
    mnuLoadCampResources();
    dds3StartMovieManager();
    sdfCreateRuntimeTask();
    D_00435B88 = 0;
    D_00435B80 = 0;
    dds3AddWorld(0x2710);
    dds3InitRandCB();
    dds3InitAdmini();
    dds3AdminiCallSequence(0, 0, 0, 0);
    dds3InitDebugSystem();
    dds3StartProcess("DbgDevDump", 0x5215, 0, 0, dds3DebugDevDumpProcess, 0, 0);
    dds3FadeClear();
    evtEnsureSolarOverlayTaskAndResetPhase();
    fldCreateEncounterTask();
    dds3AdminiCallSequence(1, 0, 0, 0);
}

void dds3SoftReset(void) {
    D_00435B88 = 1;
}

u32 dds3GetKernelCount(void) {
    return D_00435B80;
}

u32 dds3GetFrameID(void) {
    return D_00438D80;
}

void dds3PauseOn(void) {
    D_00435B90 = 0;
    D_00435B8C = 1;
    D_00435B94 = 0;
}

void dds3PauseOff(void) {
    D_00435B8C = 0;
    D_00435B90 = 0;
    D_00435B94 = 0;
}

void dds3Stop(u32 pauseCount) {
    D_00435B90 = pauseCount;
    D_00435B8C = 1;
    D_00435B94 = 0;
}

void dds3SlowOn(u32 first, u32 second) {
    s32 value = D_00435B84;
    D_00435B94 = 1;
    D_00435B98 = first;
    D_00435B9C = value;
    D_00435BA0 = second;
    D_00435B8C = 0;
    D_00435B90 = 0;
}

void dds3SlowOff(void) {
    D_00435B8C = 0;
    D_00435B98 = 4;
    D_00435B90 = 0;
    D_00435B94 = 0;
}

void func_00100488(void) {
}

void func_00100490(void) {
}

void dds3SoftResetOn(void) {
    D_00435B89 = 0;
}

void dds3SoftResetOff(void) {
    D_00435B89 = 1;
}

extern s16 D_00435BA8;
extern u16 D_00435BAA;
extern s8 D_00435BB4;
extern u16 D_00438A1E;
extern u16 D_00438A24;
extern u16 mnuMovieTaskState;
extern void sdfWaitVblank(s32 count);
extern void sdfGrReqDrawDelay(s32);
extern void sdfPadBuildButtonStates(void);
extern void sdfFileCmdThreadSetLoPrio(void);
extern void sdfFileCmdThreadSetHiPrio(void);

extern s32 dds3ProcessMain(void);

void kwlnRunMainLoop(void) {
    u32 startTick;

    kwlnInitializeRuntime();
    D_00438D80 = 0;
    while (D_00435B88 == 0) {
        sdfWaitVblank(mnuMovieTaskState);
        sdfGrReqDrawDelay(mnuMovieTaskState - 1);
        sdfPadBuildButtonStates();
        if (D_00435BB4 != 0) {
            sdfFileCmdThreadSetLoPrio();
        }
        if (D_00435B8C == 0) {
            startTick = sdfGetRunTimeCountBase();
            kwlnUpdatePadInputState();
            if (dds3ProcessMain() == 0) {
                break;
            }
            D_00435B80++;
            D_00435BA8 = sdfGetRunTimeCount(startTick) - D_00438A24;
            D_00438D80 ^= 1;
            D_00435BAA = D_00438A1E;
        } else if (D_00435B90 > 0) {
            D_00435B90--;
            if (D_00435B90 == 0) {
                D_00435B8C = 0;
            }
        }
        D_00435B84++;
        if (D_00435BB4 == 0) {
            sdfFileCmdThreadSetHiPrio();
        }
    }
    LoadExecPS2("cdrom0:\\SLPS_999.99;1", 0, NULL);
}

/* Remove a task from the doubly linked queue of its state (1 delayed start, 2 active, 3 delayed destroy). */
void DeleteProcessList(KwlnTask *task) {
    switch (task->flags & 0xF) {
    case 0:
        return;
    case 1:
    case 2:
    case 3:
        break;
    default:
        return;
    }
    if (task->listPrev != NULL) {
        task->listPrev->listNext = task->listNext;
    } else {
        switch (task->flags & 0xF) {
        case 1:
            kwlnDelayedStartTaskHead = task->listNext;
            break;
        case 2:
            kwlnActiveTaskHead = task->listNext;
            break;
        case 3:
            kwlnDelayedDestroyTaskHead = task->listNext;
            break;
        }
    }
    if (task->listNext != NULL) {
        task->listNext->listPrev = task->listPrev;
    } else {
        switch (task->flags & 0xF) {
        case 1:
            D_00435BD4 = task->listPrev;
            break;
        case 2:
            D_00435BEC = task->listPrev;
            break;
        case 3:
            D_00435BE0 = task->listPrev;
            break;
        }
    }
    task->listNext = NULL;
    task->listPrev = NULL;
    switch (task->flags & 0xF) {
    case 1:
        kwlnDelayedStartTaskCount--;
        break;
    case 2:
        kwlnActiveTaskCount--;
        break;
    case 3:
        kwlnDelayedDestroyTaskCount--;
        break;
    }
}

void AddProcessList(KwlnTask *task) {
    KwlnTask *cur;
    KwlnTask *prev;

    switch (task->flags & 0xF) {
    case 1:
        cur = kwlnDelayedStartTaskHead;
        break;
    case 0:
        return;
    case 2:
        cur = kwlnActiveTaskHead;
        break;
    case 3:
        cur = kwlnDelayedDestroyTaskHead;
        break;
    default:
        return;
    }
    if (cur == NULL) {
        switch (task->flags & 0xF) {
        case 1:
            kwlnDelayedStartTaskHead = task;
            D_00435BD4 = task;
            break;
        case 2:
            kwlnActiveTaskHead = task;
            D_00435BEC = task;
            break;
        case 3:
            kwlnDelayedDestroyTaskHead = task;
            D_00435BE0 = task;
            break;
        }
        task->listPrev = NULL;
        task->listNext = NULL;
    } else {
        while (cur != NULL) {
            if (task->priority < cur->priority) {
                if (cur->listPrev != NULL) {
                    cur->listPrev->listNext = task;
                    task->listPrev = cur->listPrev;
                    task->listNext = cur;
                    cur->listPrev = task;
                } else {
                    switch (task->flags & 0xF) {
                    case 1:
                        kwlnDelayedStartTaskHead = task;
                        break;
                    case 2:
                        kwlnActiveTaskHead = task;
                        break;
                    case 3:
                        kwlnDelayedDestroyTaskHead = task;
                        break;
                    }
                    task->listPrev = NULL;
                    task->listNext = cur;
                    cur->listPrev = task;
                }
                break;
            }
            cur = cur->listNext;
        }
        if (cur == NULL) {
            switch (task->flags & 0xF) {
            case 1:
                prev = D_00435BD4;
                prev->listNext = task;
                task->listPrev = prev;
                D_00435BD4 = task;
                break;
            case 2:
                prev = D_00435BEC;
                prev->listNext = task;
                task->listPrev = prev;
                D_00435BEC = task;
                break;
            case 3:
                prev = D_00435BE0;
                prev->listNext = task;
                task->listPrev = prev;
                D_00435BE0 = task;
                break;
            }
            task->listNext = NULL;
        }
    }
    switch (task->flags & 0xF) {
    case 1:
        kwlnDelayedStartTaskCount++;
        break;
    case 2:
        kwlnActiveTaskCount++;
        break;
    case 3:
        kwlnDelayedDestroyTaskCount++;
        break;
    }
}

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B80);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B84);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B88);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B89);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B8C);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B90);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B94);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B98);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B9C);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BA0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BA4);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BA8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BAA);

INCLUDE_SDATA(const s32, "game/code_00100000", mnuMovieTaskState);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BB0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BB4);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BB8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BC0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BC8);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnDelayedStartTaskHead);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BD4);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnDelayedStartTaskCount);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnDelayedDestroyTaskHead);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BE0);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnDelayedDestroyTaskCount);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnActiveTaskHead);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BEC);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnActiveTaskCount);

