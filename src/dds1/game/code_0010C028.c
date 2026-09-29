#include "common.h"

typedef struct {
    u8 pad0[0x18];
    s32 pc;
    s32 sp;
    u8 stackTypes[28];
    u32 stackValues[28];
    u8 padAC[0x10];
    u32 *instructions;
    u8 padC0[0x30];
    void *actor; /* 0xF0: battle actor associated with the active script */
} ScriptContext;

extern s32 func_00101A70(void);

void scrSetCurrentActor(u32 unused, void *actor) {
    ScriptContext *context;

    context = (ScriptContext *)func_00101A70();
    context->actor = actor;
}

u32 scrGetCurrentActor(void) {
    ScriptContext *context;

    context = (ScriptContext *)func_00101A70();
    return (u32)context->actor;
}

void func_0010C070(u32 task) {
    s64 previousContext;

    previousContext = func_00101A70();
    if (previousContext != 0) {
        func_0010BD20(previousContext);
    }
    func_00101A68(task, 0);
}

void func_0010C0B0(void) {
    bfContextStep();
}

INCLUDE_ASM(const s32, "game/code_0010C028", bfTaskUpdate);

void scrPushInteger(ScriptContext *script, u32 value) {
    script->stackTypes[script->sp] = 0;
    script->stackValues[script->sp] = value;
    script->sp = script->sp + 1;
}

INCLUDE_ASM(const s32, "game/code_0010C028", bfStackPushFloat);

void scrPushString(ScriptContext *script, u32 value) {
    script->stackTypes[script->sp] = 5;
    script->stackValues[script->sp] = value;
    script->sp = script->sp + 1;
}

void scrPushTypeFourValue(ScriptContext *script, u32 value) {
    script->stackTypes[script->sp] = 4;
    script->stackValues[script->sp] = value;
    script->sp = script->sp + 1;
}

INCLUDE_ASM(const s32, "game/code_0010C028", bfStackPopInt);

INCLUDE_ASM(const s32, "game/code_0010C028", bfStackPopFloat);

u32 scrPushNextInstructionValue(ScriptContext *script) {
    s32 nextPc;
    ScriptContext *context;

    context = script;
    nextPc = context->pc + 1;
    context->pc = nextPc;
    scrPushInteger(script, context->instructions[nextPc]);
    context->pc = context->pc + 1;
    return 1;
}
