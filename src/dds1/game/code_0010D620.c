#include "common.h"

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

extern u64 func_0010D428(u64);
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

u32 func_0010D6E8(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    temp_v0 = mdlFlagTest(temp_v0);
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_0010D718(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    mdlFlagSet(temp_v0);
    return 1;
}

u32 func_0010D740(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    mdlFlagClear(temp_v0);
    return 1;
}
