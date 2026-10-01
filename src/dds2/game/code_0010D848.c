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

extern ScriptState *D_00438E8C;

extern u64 scrReadIntParameter(u64);

extern s8 D_0040B7D9[];

extern u64 mdlFlagTest(u64);

u32 scrGetProcedureAddress(s32 index) {
    return D_00438E8C->procedures[index].address;
}

u32 scrGetLabelAddress(s32 index) {
    return D_00438E8C->labels[index].address;
}

u32 scrGetProgramCounter(void) {
    return D_00438E8C->programCounter;
}

void scrSetProgramCounter(u32 address) {
    D_00438E8C->programCounter = address;
}

u32 scrGetTimer(void) {
    return D_00438E8C->timer;
}

u32 scrGetCommandTimer(void) {
    return D_00438E8C->commandTimer;
}

u32 scrGetWindow(void) {
    return D_00438E8C->window;
}

s32 func_0010D8C8(void) {
    return (s32)D_00438E8C;
}

u32 func_0010D8D0(void) {
    return D_00438E8C->unkF0;
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
