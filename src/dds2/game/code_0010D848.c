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

extern u64 func_0010D650(u64);

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

INCLUDE_ASM(const s32, "game/code_0010D848", func_0010D8C8);

u32 func_0010D8D0(void) {
    return D_00438E8C->unkF0;
}

INCLUDE_ASM(const s32, "game/code_0010D848", func_0010D8E0);

u32 scrOpcodeTestModelFlag(void) {
    u64 flag;

    flag = func_0010D650(0);
    flag = mdlFlagTest(flag);
    func_0010D818(flag);
    return 1;
}

u32 scrOpcodeSetModelFlag(void) {
    u64 flag;

    flag = func_0010D650(0);
    mdlFlagSet(flag);
    return 1;
}

u32 scrOpcodeClearModelFlag(void) {
    u64 flag;

    flag = func_0010D650(0);
    mdlFlagClear(flag);
    return 1;
}
