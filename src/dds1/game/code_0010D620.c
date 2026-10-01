#include "common.h"

/* Script label records are 0x20 bytes; the address lives at +0x18. */
typedef struct {
    u8 pad[0x18];
    u32 address;
    u8 pad1C[4];
} ScriptLabel;

typedef struct {
    u8 pad0[0x18];
    u32 programCounter;
    u8 pad1C[0x98];
    ScriptLabel *procedures;
    ScriptLabel *labels;
    u8 padBC[0x10];
    u32 window;
    u32 timer;
    u32 commandTimer;
    u8 padD8[0x18];
    u32 unkF0;
} ScriptState;

extern u64 scrReadIntParameter(u64);
extern u64 mdlFlagTest(u64);

extern ScriptState *scrCurrentContext;

extern s8 D_00398629[];

u32 scrGetProcedureAddress(s32 index) {
    return scrCurrentContext->procedures[index].address;
}

u32 scrGetLabelAddress(s32 index) {
    return scrCurrentContext->labels[index].address;
}

u32 scrGetProgramCounter(void) {
    return scrCurrentContext->programCounter;
}

void scrSetProgramCounter(u32 address) {
    scrCurrentContext->programCounter = address;
}

u32 scrGetTimer(void) {
    return scrCurrentContext->timer;
}

u32 scrGetCommandTimer(void) {
    return scrCurrentContext->commandTimer;
}

u32 scrGetWindow(void) {
    return scrCurrentContext->window;
}

s32 scrGetCurrentContext(void) {
    return (s32)scrCurrentContext;
}

u32 func_0010D6A8(void) {
    return scrCurrentContext->unkF0;
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
