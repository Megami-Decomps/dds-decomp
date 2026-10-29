#include "sdk_ps2.h"
#include "common.h"
#include "kwln_draw_buffer.h"
#include "sdf_timer.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "kwln_task_runner.h"
#include "kwln_process_main_result.h"
#include "fld_file_io_priority_state.h"

extern void kwlnUpdatePadInputState(void);

extern u8 D_003BA709;
extern u32 D_003BA704;
extern u32 D_003BA71C;
extern u32 D_003BA720;
extern u32 D_003BA718;

extern u32 D_003BA70C;
extern s32 D_003BA710;
extern u32 D_003BA714;

extern u32 D_003BD680;

extern u32 D_003BA700;

extern s8 D_003BA708;

extern dds3ProcessID_t *startinglistf;
extern dds3ProcessID_t *D_003BA804;
extern s32 startinglistcnt;
extern dds3ProcessID_t *endinglistf;
extern dds3ProcessID_t *D_003BA810;
extern s32 endinglistcnt;
extern dds3ProcessID_t *runninglistf;
extern dds3ProcessID_t *D_003BA81C;
extern s32 runninglistcnt;

extern char D_003BA738[]; /* "1\n" */
extern char D_003BA740[]; /* "2\n" */
extern char D_003BA748[]; /* "3\n" */
extern char D_003BA750[]; /* "1st" */
extern char D_003BA758[]; /* "4\n" */
extern char D_003BA760[]; /* "draw1st" */
extern char D_003BA768[]; /* "5\n" */
extern char D_003BA770[]; /* "6\n" */
extern char D_003BA778[]; /* "7\n" */
extern char D_003BA780[]; /* "8\n" */
extern char D_003BA788[]; /* "9\n" */
extern char D_003BA790[]; /* "10\n" */
extern char D_003BA798[]; /* "11\n" */
extern char D_003BA7A0[]; /* "12\n" */
extern char D_003BA7A8[]; /* "12-1\n" */
extern char D_003BA7B0[]; /* "13\n" */
extern char D_003BA7B8[]; /* "14\n" */
extern char D_003BA7C0[]; /* "15\n" */
extern char D_003BA7C8[]; /* "16\n" */
extern char D_003BA7D0[]; /* "17\n" */
extern char D_003BA7D8[]; /* "18\n" */
extern char D_003BA7E0[]; /* "19\n" */
extern char D_003BA7E8[]; /* "20\n" */
extern char D_003BA7F0[]; /* "21\n" */
extern char D_003BA7F8[]; /* "22\n" */
#include "sdf_debug_api.h"
extern s32 dds3FirstProcess(dds3ProcessID_t *task);
extern s32 dds3DrawFirstProcess(dds3ProcessID_t *task);
extern s32 dds3DrawProcess(dds3ProcessID_t *task);
extern KwlnTaskUpdateResult dds3BasicMainProcess(dds3ProcessID_t *task);
extern KwlnTaskUpdateResult dds3BasicDrawProcess(dds3ProcessID_t *task);
extern KwlnTaskUpdateResult frFlipPacketBuf(dds3ProcessID_t *task);
extern void frDestroyFontTable(dds3ProcessID_t *task);
extern s32 dds3DebugDevDumpProcess(dds3ProcessID_t *task);

struct dds3Basic_t;
extern struct dds3Basic_t * dds3AddWorld(s32 capacity);
extern void mdlInitManager(void);
extern void btlLoadInputIconsAndSystemSounds(void);
extern void dds3InitDrawEnv(void);
extern void evtStartSolarOverlay(void);
extern void fileSysInit(void);
extern void fldCreateEncounterTask(void);
extern void fldCreateFieldEffectTask(void);
extern void fldInitializeDisplayAndTables(void);
extern void frInitFontTable(s32 rowBudget, s32 poolCount);
extern void itfMesMngInitializeManager(void);
extern void dds3InitAdmini(void);
extern void dds3FadeClear(void);
extern void dds3InitRandCB(void);
extern void dds3PadInitialize(void);
extern void dds3InitDebugSystem(void);
extern void dds3StartMovieManager(void);
extern void mnuInitializeTitleAudioAndEffects(void);
extern void mnuLoadStaffImageHandles(void);
extern void dds3GlobalWorkInit(void);
extern void sdfDevConsInit(void);
extern void sdfResetChannels(void);

extern void dds3AdminiCallSequence(s32 requestedMode, const void *requestData, u32 dataBytes, s32 markHistory);

/* Kernel startup: create the frame/draw/font tasks and initialize every subsystem, logging each step. */
void kwlnInitializeRuntime(void) {
    printf(D_003BA738);
    dds3InitDrawEnv();
    printf(D_003BA740);
    dds3PadInitialize();
    printf(D_003BA748);
    dds3StartProcess(D_003BA750, 0, 0, 0, dds3FirstProcess, 0, 0);
    printf(D_003BA758);
    dds3StartProcess(D_003BA760, 0x2710, 0, 0, dds3DrawFirstProcess, 0, 0);
    printf(D_003BA768);
    dds3StartProcess("RequestDraw", 0x4E20, 0, 0, dds3DrawProcess, 0, 0);
    printf(D_003BA770);
    dds3StartProcess("basic_main", 0x3FC, 1, 1, dds3BasicMainProcess, 0, 0);
    printf(D_003BA778);
    dds3StartProcess("basic_draw", 0x2B09, 1, 1, dds3BasicDrawProcess, 0, 0);
    printf(D_003BA780);
    frInitFontTable(0x100, 0x200);
    dds3StartProcess("font_sys", 0x4E1F, 0, 0, frFlipPacketBuf, frDestroyFontTable, 0);
    printf(D_003BA788);
    fileSysInit();
    printf(D_003BA790);
    itfMesMngInitializeManager();
    mnuInitializeTitleAudioAndEffects();
    printf(D_003BA798);
    sdfDevConsInit();
    mdlInitManager();
    printf(D_003BA7A0);
    fldCreateFieldEffectTask();
    printf(D_003BA7A8);
    fldInitializeDisplayAndTables();
    printf(D_003BA7B0);
    sdfResetChannels();
    btlLoadInputIconsAndSystemSounds();
    printf(D_003BA7B8);
    mnuLoadStaffImageHandles();
    printf(D_003BA7C0);
    dds3StartMovieManager();
    printf(D_003BA7C8);
    dds3GlobalWorkInit();
    printf(D_003BA7D0);
    D_003BA708 = 0;
    D_003BA700 = 0;
    dds3AddWorld(0x2710);
    printf(D_003BA7D8);
    dds3InitRandCB();
    dds3InitAdmini();
    printf(D_003BA7E0);
    dds3AdminiCallSequence(0, 0, 0, 0);
    dds3InitDebugSystem();
    printf(D_003BA7E8);
    dds3StartProcess("DbgDevDump", 0x5215, 0, 0, dds3DebugDevDumpProcess, 0, 0);
    printf(D_003BA7F0);
    dds3FadeClear();
    evtStartSolarOverlay();
    fldCreateEncounterTask();
    printf(D_003BA7F8);
    dds3AdminiCallSequence(1, 0, 0, 0);
}

void dds3SoftReset(void) {
    D_003BA708 = 1;
}

u32 dds3GetKernelCount(void) {
    return D_003BA700;
}

u32 dds3GetFrameID(void) {
    return D_003BD680;
}

void dds3PauseOn(void) {
    D_003BA710 = 0;
    D_003BA70C = 1;
    D_003BA714 = 0;
}

void dds3PauseOff(void) {
    D_003BA70C = 0;
    D_003BA710 = 0;
    D_003BA714 = 0;
}

void dds3Stop(u32 pauseCount) {
    D_003BA710 = pauseCount;
    D_003BA70C = 1;
    D_003BA714 = 0;
}

void dds3SlowOn(u32 first, u32 second) {
    s32 value = D_003BA704;
    D_003BA714 = 1;
    D_003BA718 = first;
    D_003BA71C = value;
    D_003BA720 = second;
    D_003BA70C = 0;
    D_003BA710 = 0;
}

extern s32 D_003BA710;
extern u32 D_003BA714;

void dds3SlowOff(void) {
    D_003BA70C = 0;
    D_003BA718 = 4;
    D_003BA710 = 0;
    D_003BA714 = 0;
}

void func_001005A0(void) {
}

void func_001005A8(void) {
}

void dds3SoftResetOn(void) {
    D_003BA709 = 0;
}

void dds3SoftResetOff(void) {
    D_003BA709 = 1;
}

extern s16 D_003BA728;
extern u16 D_003BA72A;
extern FldFileIoPriorityState D_003BA734;
extern u16 D_003BD32E;
extern u16 D_003BD334;
extern u16 dds3VblanksPerFrame;
extern void sdfWaitVblank(s32 count);
extern void sdfGrReqDrawDelay(s32);
extern void sdfPadBuildButtonStates(void);
extern void sdfFileCmdThreadSetLoPrio(void);
extern void sdfFileCmdThreadSetHiPrio(void);

extern KwlnProcessMainResult dds3ProcessMain(void);

void kwlnRunMainLoop(void) {
    u32 startTick;

    kwlnInitializeRuntime();
    D_003BD680 = 0;
    while (D_003BA708 == 0) {
        sdfWaitVblank(dds3VblanksPerFrame);
        sdfGrReqDrawDelay(dds3VblanksPerFrame - 1);
        sdfPadBuildButtonStates();
        if (D_003BA734 != FLD_FILE_IO_PRIORITY_HIGH) {
            sdfFileCmdThreadSetLoPrio();
        }
        if (D_003BA70C == 0) {
            startTick = sdfGetRunTimeCountBase();
            kwlnUpdatePadInputState();
            if (dds3ProcessMain() == KWLN_PROCESS_MAIN_STOP) {
                break;
            }
            D_003BA700++;
            D_003BA728 = sdfGetRunTimeCount(startTick) - D_003BD334;
            D_003BD680 ^= 1;
            D_003BA72A = D_003BD32E;
        } else if (D_003BA710 > 0) {
            D_003BA710--;
            if (D_003BA710 == 0) {
                D_003BA70C = 0;
            }
        }
        D_003BA704++;
        if (D_003BA734 == FLD_FILE_IO_PRIORITY_HIGH) {
            sdfFileCmdThreadSetHiPrio();
        }
    }
    LoadExecPS2("cdrom0:\\SLPS_999.99;1", 0, NULL);
}

/* Remove a task from the doubly linked queue of its state (1 delayed start, 2 active, 3 delayed destroy). */
void DeleteProcessList(dds3ProcessID_t *task) {
    switch (task->flag & 0xF) {
    case 0:
        return;
    case 1:
    case 2:
    case 3:
        break;
    default:
        return;
    }
    if (task->prev != NULL) {
        task->prev->next = task->next;
    } else {
        switch (task->flag & 0xF) {
        case 1:
            startinglistf = task->next;
            break;
        case 2:
            runninglistf = task->next;
            break;
        case 3:
            endinglistf = task->next;
            break;
        }
    }
    if (task->next != NULL) {
        task->next->prev = task->prev;
    } else {
        switch (task->flag & 0xF) {
        case 1:
            D_003BA804 = task->prev;
            break;
        case 2:
            D_003BA81C = task->prev;
            break;
        case 3:
            D_003BA810 = task->prev;
            break;
        }
    }
    task->next = NULL;
    task->prev = NULL;
    switch (task->flag & 0xF) {
    case 1:
        startinglistcnt--;
        break;
    case 2:
        runninglistcnt--;
        break;
    case 3:
        endinglistcnt--;
        break;
    }
}

void AddProcessList(dds3ProcessID_t *task) {
    dds3ProcessID_t *cur;
    dds3ProcessID_t *prev;

    switch (task->flag & 0xF) {
    case 1:
        cur = startinglistf;
        break;
    case 0:
        return;
    case 2:
        cur = runninglistf;
        break;
    case 3:
        cur = endinglistf;
        break;
    default:
        return;
    }
    if (cur == NULL) {
        switch (task->flag & 0xF) {
        case 1:
            startinglistf = task;
            D_003BA804 = task;
            break;
        case 2:
            runninglistf = task;
            D_003BA81C = task;
            break;
        case 3:
            endinglistf = task;
            D_003BA810 = task;
            break;
        }
        task->prev = NULL;
        task->next = NULL;
    } else {
        while (cur != NULL) {
            if (task->priority < cur->priority) {
                if (cur->prev != NULL) {
                    cur->prev->next = task;
                    task->prev = cur->prev;
                    task->next = cur;
                    cur->prev = task;
                } else {
                    switch (task->flag & 0xF) {
                    case 1:
                        startinglistf = task;
                        break;
                    case 2:
                        runninglistf = task;
                        break;
                    case 3:
                        endinglistf = task;
                        break;
                    }
                    task->prev = NULL;
                    task->next = cur;
                    cur->prev = task;
                }
                break;
            }
            cur = cur->next;
        }
        if (cur == NULL) {
            switch (task->flag & 0xF) {
            case 1:
                prev = D_003BA804;
                prev->next = task;
                task->prev = prev;
                D_003BA804 = task;
                break;
            case 2:
                prev = D_003BA81C;
                prev->next = task;
                task->prev = prev;
                D_003BA81C = task;
                break;
            case 3:
                prev = D_003BA810;
                prev->next = task;
                task->prev = prev;
                D_003BA810 = task;
                break;
            }
            task->next = NULL;
        }
    }
    switch (task->flag & 0xF) {
    case 1:
        startinglistcnt++;
        break;
    case 2:
        runninglistcnt++;
        break;
    case 3:
        endinglistcnt++;
        break;
    }
}

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA700);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA704);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA708);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA709);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA70C);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA710);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA714);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA718);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA71C);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA720);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA724);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA728);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA72A);

INCLUDE_SDATA(const s32, "game/code_00100000", dds3VblanksPerFrame);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA730);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA734);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA738);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA740);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA748);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA750);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA758);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA760);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA768);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA770);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA778);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA780);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA788);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA790);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA798);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7A0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7A8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7B0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7B8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7C0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7C8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7D0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7D8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7E0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7E8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7F0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7F8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7FC);

INCLUDE_SDATA(const s32, "game/code_00100000", startinglistf);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA804);

INCLUDE_SDATA(const s32, "game/code_00100000", startinglistcnt);

INCLUDE_SDATA(const s32, "game/code_00100000", endinglistf);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA810);

INCLUDE_SDATA(const s32, "game/code_00100000", endinglistcnt);

INCLUDE_SDATA(const s32, "game/code_00100000", runninglistf);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA81C);

INCLUDE_SDATA(const s32, "game/code_00100000", runninglistcnt);

