#include "common.h"
#include "kwln_task_create.h"
#include "kwln_task_lifecycle.h"

extern void scrDestroyAllNamedProcesses(void);

extern char D_004221D8[]; /* "EventTest" */

extern char D_004221E8[]; /* "PolygonMovie" */

void evtFreeStage(void);

extern u32 D_00435CBC;

extern s32 dpy;

void func_00245810(void) {
}

/* Retail test-task update is idle from creation; return 0 each tick. */
s32 evtDebugCameraStep(dds3ProcessID_t *task) {
    return 0;
}

/* Test-task teardown destroys the named script processes. */
void EventDebugShutdown(dds3ProcessID_t *task) {
    scrDestroyAllNamedProcesses();
}

void evtInitEventTest(void) {
    D_00435CBC = 0x80000000;
    dds3StartProcess(D_004221D8, 0x2AF9, 1, 1, evtDebugCameraStep, EventDebugShutdown, 0);
}

void evtChkEndProcess(void) {
    dds3KillProcessName(D_004221D8, 1);
    dds3KillProcessName(D_004221E8, 0);
    evtFreeStage();
}

INCLUDE_RODATA(const s32, "game/code_00245810", D_004221C8);

INCLUDE_RODATA(const s32, "game/code_00245810", D_004221D8);

INCLUDE_RODATA(const s32, "game/code_00245810", D_004221E8);

INCLUDE_ASM(const s32, "game/code_00245810", evtSceneDispProcess);

void func_00245F80(void) {
    dpy = 0;
}

INCLUDE_SDATA(const s32, "game/code_00245810", D_00437220);

INCLUDE_SDATA(const s32, "game/code_00245810", dpy);

INCLUDE_SDATA(const s32, "game/code_00245810", curbase);

INCLUDE_SDATA(const s32, "game/code_00245810", curpoint);

INCLUDE_SDATA(const s32, "game/code_00245810", D_00437238);

INCLUDE_SDATA(const s32, "game/code_00245810", D_00437240);

INCLUDE_SDATA(const s32, "game/code_00245810", D_00437248);

INCLUDE_SDATA(const s32, "game/code_00245810", D_00437250);

INCLUDE_SDATA(const s32, "game/code_00245810", D_00437258);

INCLUDE_SDATA(const s32, "game/code_00245810", D_00437260);

INCLUDE_SDATA(const s32, "game/code_00245810", D_00437268);

INCLUDE_SDATA(const s32, "game/code_00245810", D_00437270);

INCLUDE_SDATA(const s32, "game/code_00245810", D_00437278);

INCLUDE_SDATA(const s32, "game/code_00245810", D_00437280);

INCLUDE_SDATA(const s32, "game/code_00245810", D_00437288);

INCLUDE_SDATA(const s32, "game/code_00245810", D_00437290);

INCLUDE_SDATA(const s32, "game/code_00245810", D_00437298);

INCLUDE_SDATA(const s32, "game/code_00245810", D_004372A0);

INCLUDE_SDATA(const s32, "game/code_00245810", D_004372A8);

