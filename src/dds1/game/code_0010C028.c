#include "common.h"

typedef struct ScriptContext {
    u8 pad0[0x18];
    s32 pc; /* 0x18 */
    s32 stackDepth; /* 0x1C: number of stack values */
    u8 stackTypes[28];
    u32 stackValues[28];
    u8 padAC[0x10];
    u32 *instructions; /* 0xBC */
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
        scrProcDestroyTask(previousContext);
    }
    func_00101A68(task, 0);
}

void func_0010C0B0(void) {
    bfContextStep();
}

INCLUDE_ASM(const s32, "game/code_0010C028", bfTaskUpdate);

void scrPushInteger(ScriptContext *script, u32 value) {
    script->stackTypes[script->stackDepth] = 0;
    script->stackValues[script->stackDepth] = value;
    script->stackDepth = script->stackDepth + 1;
}

INCLUDE_ASM(const s32, "game/code_0010C028", bfStackPushFloat);

void scrPushString(ScriptContext *script, u32 value) {
    script->stackTypes[script->stackDepth] = 5;
    script->stackValues[script->stackDepth] = value;
    script->stackDepth = script->stackDepth + 1;
}

void scrPushTypeFourValue(ScriptContext *script, u32 value) {
    script->stackTypes[script->stackDepth] = 4;
    script->stackValues[script->stackDepth] = value;
    script->stackDepth = script->stackDepth + 1;
}

INCLUDE_ASM(const s32, "game/code_0010C028", bfStackPopInt);

INCLUDE_ASM(const s32, "game/code_0010C028", bfStackPopFloat);

/* Push the word following the opcode, then advance past its operand. */
u32 scrPushNextInstructionValue(ScriptContext *script) {
    s32 operandPc;
    ScriptContext *context;

    context = script;
    operandPc = context->pc + 1;
    context->pc = operandPc;
    scrPushInteger(script, context->instructions[operandPc]);
    context->pc = context->pc + 1;
    return 1;
}
