#include "common.h"
#include "scr.h"


extern s32 scrReadIntParameter(s32);

extern s8 D_0040B7D9[];

extern s32 mdlFlagTest(s32);
extern void mdlFlagSet(s32);
extern void mdlFlagClear(s32);
extern void scrSetIntegerReturnValue(s32);

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
    if (D_0040B7D9[0] < 0) {
        return 1;
    }
    return 0;
}

/* Evaluate a script-supplied model flag and push the test result. */
u32 scrOpcodeTestModelFlag(void) {
    s32 value;

    value = scrReadIntParameter(0);
    value = mdlFlagTest(value);
    scrSetIntegerReturnValue(value);
    return 1;
}

u32 scrOpcodeSetModelFlag(void) {
    s32 flagId;

    flagId = scrReadIntParameter(0);
    mdlFlagSet(flagId);
    return 1;
}

u32 scrOpcodeClearModelFlag(void) {
    s32 flagId;

    flagId = scrReadIntParameter(0);
    mdlFlagClear(flagId);
    return 1;
}
