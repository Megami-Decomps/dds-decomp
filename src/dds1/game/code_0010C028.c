#include "common.h"

typedef struct {
    u8 pad0[0x18];
    s32 pc;
    s32 sp;
    u8 stackTypes[28];
    u32 stackValues[28];
    u8 padAC[0x10];
    u32 *instructions;
} ScriptContext;

extern s32 func_00101A70(void);

void func_0010C028(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(u32 *)(temp_v0 + 0xf0) = arg1;
}

u32 func_0010C050(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    return *(u32 *)(temp_v0 + 0xf0);
}

void func_0010C070(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_00101A70();
    if (temp_v0 != 0) {
        func_0010BD20(temp_v0);
    }
    func_00101A68(arg0, 0);
}

void func_0010C0B0(void) {
    func_0010D380();
}

INCLUDE_ASM(const s32, "game/code_0010C028", func_0010C0C8);

void func_0010C120(ScriptContext *script, u32 value) {
    script->stackTypes[script->sp] = 0;
    script->stackValues[script->sp] = value;
    script->sp = script->sp + 1;
}

INCLUDE_ASM(const s32, "game/code_0010C028", func_0010C150);

void func_0010C180(ScriptContext *script, u32 value) {
    script->stackTypes[script->sp] = 5;
    script->stackValues[script->sp] = value;
    script->sp = script->sp + 1;
}

void func_0010C1B0(ScriptContext *script, u32 value) {
    script->stackTypes[script->sp] = 4;
    script->stackValues[script->sp] = value;
    script->sp = script->sp + 1;
}

INCLUDE_ASM(const s32, "game/code_0010C028", func_0010C1E0);

INCLUDE_ASM(const s32, "game/code_0010C028", func_0010C2B8);

u32 func_0010C3A0(ScriptContext *script) {
    s32 nextPc;
    ScriptContext *context;

    context = script;
    nextPc = context->pc + 1;
    context->pc = nextPc;
    func_0010C120(script, context->instructions[nextPc]);
    context->pc = context->pc + 1;
    return 1;
}
