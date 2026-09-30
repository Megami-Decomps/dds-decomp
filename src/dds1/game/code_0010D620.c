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

extern ScriptState *D_003BD78C;

u32 scrGetProcedureAddress(s32 index) {
    return D_003BD78C->procedures[index].address;
}

u32 scrGetLabelAddress(s32 index) {
    return D_003BD78C->labels[index].address;
}

u32 scrGetProgramCounter(void) {
    return D_003BD78C->programCounter;
}

void scrSetProgramCounter(u32 address) {
    D_003BD78C->programCounter = address;
}

u32 scrGetTimer(void) {
    return D_003BD78C->timer;
}

u32 scrGetCommandTimer(void) {
    return D_003BD78C->commandTimer;
}

u32 scrGetWindow(void) {
    return D_003BD78C->window;
}

INCLUDE_ASM(const s32, "game/code_0010D620", func_0010D6A0);

u32 func_0010D6A8(void) {
    return D_003BD78C->unkF0;
}

INCLUDE_ASM(const s32, "game/code_0010D620", func_0010D6B8);

/* Evaluate a script-supplied model flag and push the test result. */
u32 scrOpcodeTestModelFlag(void) {
    u64 flagId;

    flagId = scrReadIntParameter(0);
    flagId = mdlFlagTest(flagId);
    func_0010D5F0(flagId);
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
