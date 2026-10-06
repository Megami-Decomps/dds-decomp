#include "common.h"
#include "scr.h"


extern u64 scrReadIntParameter(u64);
extern u64 mdlFlagTest(u64);


extern s8 D_00398629[];

u32 scrGetProcedureAddress(s32 index) {
    return scrCurrentContext->procedures[index].addr;
}

u32 scrGetLabelAddress(s32 index) {
    return scrCurrentContext->labels[index].addr;
}

u32 scrGetProgramCounter(void) {
    return scrCurrentContext->pc;
}

void scrSetProgramCounter(u32 address) {
    scrCurrentContext->pc = address;
}

u32 scrGetTimer(void) {
    return scrCurrentContext->timer;
}

u32 scrGetCommandTimer(void) {
    return scrCurrentContext->cmdTimer;
}

u32 scrGetWindow(void) {
    return scrCurrentContext->resourceIndex;
}

ScrData *scrGetCurrentContext(void) {
    return scrCurrentContext;
}

void *scrGetCurrentCommandWork(void) {
    return scrCurrentContext->actor;
}

s32 scrHasNegativeMarkerDuringCommandTimer(void) {
    if (scrGetCommandTimer() == 0) {
        return 0;
    }
    if (D_00398629[0] < 0) {
        return 1;
    }
    return 0;
}

/* Evaluate a script-supplied model flag and push the test result. */
u32 scrOpcodeTestModelFlag(void) {
    u64 flagId;

    flagId = scrReadIntParameter(0);
    flagId = mdlFlagTest(flagId);
    scrSetIntegerReturnValue(flagId);
    return 1;
}

u32 scrOpcodeSetModelFlag(void) {
    u64 flagId;

    flagId = scrReadIntParameter(0);
    mdlFlagSet(flagId);
    return 1;
}

u32 scrOpcodeClearModelFlag(void) {
    u64 flagId;

    flagId = scrReadIntParameter(0);
    mdlFlagClear(flagId);
    return 1;
}
