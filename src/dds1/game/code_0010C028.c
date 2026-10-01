#include "common.h"

typedef struct ScriptContext {
    u8 pad0[0x18];
    s32 pc; /* 0x18 */
    s32 stackDepth; /* 0x1C: number of stack values */
    s8 stackTypes[28]; /* 0x20 */
    union {
        s32 stackValues[28];
        f32 stackFloats[28];
    } stack; /* 0x3C */
    u8 padAC[0x10];
    u32 *instructions; /* 0xBC */
    u8 padC0[0x30];
    void *actor; /* 0xF0: battle actor associated with the active script */
} ScriptContext;

extern s32 kwlnTaskGetUserValue(void);

void scrSetCurrentActor(u32 unused, void *actor) {
    ScriptContext *context;

    context = (ScriptContext *)kwlnTaskGetUserValue();
    context->actor = actor;
}

u32 scrGetCurrentActor(void) {
    ScriptContext *context;

    context = (ScriptContext *)kwlnTaskGetUserValue();
    return (u32)context->actor;
}

void scrReplaceCurrentTask(u32 task) {
    s64 previousContext;

    previousContext = kwlnTaskGetUserValue();
    if (previousContext != 0) {
        scrProcDestroyTask(previousContext);
    }
    kwlnTaskSetUserValue(task, 0);
}

void bfStepContext(void) {
    bfContextStep();
}

INCLUDE_ASM(const s32, "game/code_0010C028", bfTaskUpdate);

void scrPushInteger(ScriptContext *script, u32 value) {
    script->stackTypes[script->stackDepth] = 0;
    script->stack.stackValues[script->stackDepth] = value;
    script->stackDepth = script->stackDepth + 1;
}

void bfStackPushFloat(ScriptContext *script, f32 value) {
    script->stackTypes[script->stackDepth] = 1;
    script->stack.stackFloats[script->stackDepth] = value;
    script->stackDepth = script->stackDepth + 1;
}

void scrPushString(ScriptContext *script, u32 value) {
    script->stackTypes[script->stackDepth] = 5;
    script->stack.stackValues[script->stackDepth] = value;
    script->stackDepth = script->stackDepth + 1;
}

void scrPushTypeFourValue(ScriptContext *script, u32 value) {
    script->stackTypes[script->stackDepth] = 4;
    script->stack.stackValues[script->stackDepth] = value;
    script->stackDepth = script->stackDepth + 1;
}

typedef struct ScriptGlobals {
    u8 pad00[0x40];
    s32 ints[256];
    f32 floats[256];
} ScriptGlobals;

extern ScriptGlobals *D_003BAA00;

s32 bfStackPopInt(ScriptContext *script) {
    s32 stackIndex = script->stackDepth;

    script->stackDepth = stackIndex - 1;
    switch (script->stackTypes[stackIndex - 1]) {
    case 0:
    case 4:
        return script->stack.stackValues[script->stackDepth];
    case 1:
        return script->stack.stackFloats[script->stackDepth];
    case 2:
        return D_003BAA00->ints[script->stack.stackValues[script->stackDepth]];
    case 3:
        return D_003BAA00->floats[script->stack.stackValues[script->stackDepth]];
    }
    return 0;
}

f32 bfStackPopFloat(ScriptContext *script) {
    s32 stackIndex = script->stackDepth;

    script->stackDepth = stackIndex - 1;
    switch (script->stackTypes[stackIndex - 1]) {
    case 0:
        return (f32)script->stack.stackValues[script->stackDepth];
    case 4:
        return 0.0f;
    case 1:
        return script->stack.stackFloats[script->stackDepth];
    case 2:
        return (f32)D_003BAA00->ints[script->stack.stackValues[script->stackDepth]];
    case 3:
        return D_003BAA00->floats[script->stack.stackValues[script->stackDepth]];
    }
    return 0.0f;
}

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
