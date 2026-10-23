#include "sdk_ps2.h"
#include "common.h"
#include "mdl_flag_api.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_packet_list.h"
#include "sdf.h"
#include "sdf_sif_command.h"
#include "kwln_task_lifecycle.h"
#include "sdf_packet_alloc.h"

extern void scrDestroyAllNamedProcesses(void);

extern s32 dpy;

s32 EventDebugProcess1(dds3ProcessID_t *task);

extern u32 D_003BA8EC;

extern char D_003ACD18[]; /* "EventTest" */

void evtFreeStage(void);

void func_0022AB58(void) {
}

/* After launching a test script, keep the test task idle and return 0. */
s32 evtDebugCameraStep(dds3ProcessID_t *task) {
    func_0022AB58();
    return 0;
}

/* Return the same update callback installed by evtInitEventTest. */
void *evtGetDebugProcess(void) {
    return (void *)EventDebugProcess1;
}

extern s32 sdfFileCheckExist(const char *);

extern dds3ProcessID_t *scrStartScriptByName(s32, const char *, s32);

extern SdfPoolNode D_00325708;

extern u8 dds3PadButtonState[2][2][16];

extern s32 D_003BBDEC;

INCLUDE_RODATA(const s32, "game/code_0022AB58", D_003ACC58);

s32 EventDebugProcess1(dds3ProcessID_t *task) {
    char path[0x40];
    sdfDrawTag_t *list;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    dds3ConsPanelTagLink(list, 0x84, 0x54, 0x15, 10);
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7840, 0x7BA0, 0xFEFFFF, 0, "Event Debug"));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7A80, 0x7C60, 0xFEFFFF, 6, "E%03d.BF", D_003BBDEC));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7900, 0x7D20, 0xFEFFFF, 0, "L,R  = EVENT SELECT"));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7900, 0x7D80, 0xFEFFFF, 0, "RR   = ENTER"));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7900, 0x7DE0, 0xFEFFFF, 0, "RU   = SET E500"));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7900, 0x7E40, 0xFEFFFF, 0, "RL   = SET E600"));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7900, 0x7EA0, 0xFEFFFF, 0, "RD   = RESET FLAG"));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7900, 0x7F00, 0xFEFFFF, 0, "L1R1 = +-100"));
    D_00325708.append(&D_00325708, list);

    if ((s8)dds3PadButtonState[1][0][1] < 0) {
        sprintf(path, "/event/e%03d/e%03d/scr/e%03d.bf",
                      D_003BBDEC - D_003BBDEC % 10, D_003BBDEC, D_003BBDEC);
        if (sdfFileCheckExist(path) != 0) {
            D_003BA8EC = 0x80000000;
            scrStartScriptByName(0x3EB, path, 0);
            return (s32)evtDebugCameraStep;
        }
    }
    if ((s8)dds3PadButtonState[1][0][3] < 0) {
        evtBitAllOff();
        return (s32)evtGetDebugProcess;
    }
    if (dds3PadButtonState[1][0][5] & 2) {
        D_003BBDEC++;
    }
    if ((dds3PadButtonState[1][0][4] & 2) && D_003BBDEC > 500) {
        D_003BBDEC--;
    }
    if ((s8)dds3PadButtonState[1][0][2] < 0) {
        D_003BBDEC = 500;
    }
    if ((s8)dds3PadButtonState[1][0][0] < 0) {
        D_003BBDEC = 600;
    }
    if ((dds3PadButtonState[1][0][8] & 2) && D_003BBDEC > 500) {
        D_003BBDEC -= 100;
        if (D_003BBDEC < 500) {
            D_003BBDEC = 500;
        }
    }
    if (dds3PadButtonState[1][0][10] & 2) {
        D_003BBDEC += 100;
    }
    return 0;
}

/* Test-task teardown destroys the named script processes. */
void EventDebugShutdown(dds3ProcessID_t *task) {
    scrDestroyAllNamedProcesses();
}

void evtInitEventTest(void) {
    D_003BA8EC = 0x80000000;
    dds3StartProcess(D_003ACD18, 0x2AF9, 1, 1, EventDebugProcess1, EventDebugShutdown, 0);
}

INCLUDE_RODATA(const s32, "game/code_0022AB58", D_003ACD18);

void evtChkEndProcess(void) {
    dds3KillProcessName(D_003ACD18, 1);
    dds3KillProcessName("PolygonMovie", 0);
    evtFreeStage();
}

INCLUDE_RODATA(const s32, "game/code_0022AB58", D_003ACD38);

INCLUDE_RODATA(const s32, "game/code_0022AB58", D_003ACD48);

INCLUDE_RODATA(const s32, "game/code_0022AB58", D_003ACD58);

INCLUDE_RODATA(const s32, "game/code_0022AB58", D_003ACD68);

INCLUDE_RODATA(const s32, "game/code_0022AB58", D_003ACD78);

INCLUDE_ASM(const s32, "game/code_0022AB58", evtSceneDispProcess);

void func_0022B618(void) {
    dpy = 0;
}

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBDE0);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBDEC);

INCLUDE_SDATA(const s32, "game/code_0022AB58", dpy);

INCLUDE_SDATA(const s32, "game/code_0022AB58", curbase);

INCLUDE_SDATA(const s32, "game/code_0022AB58", curpoint);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE00);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE08);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE10);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE18);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE20);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE28);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE30);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE38);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE40);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE48);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE50);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE58);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE60);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE68);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE70);

