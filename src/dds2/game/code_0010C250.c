#include "common.h"
#include "scr.h"

extern u32 kwlnTaskGetUserValue(void);

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
    void *actor; /* 0xF0: battle actor bound to the active script */
} ScriptContext;

void scrSetCurrentActor(u32 unused, u32 actor) {
    ScriptContext *context;

    context = (ScriptContext *)kwlnTaskGetUserValue();
    context->actor = (void *)actor;
}

u32 scrGetCurrentActor(void) {
    ScriptContext *context;

    context = (ScriptContext *)kwlnTaskGetUserValue();
    return (u32)context->actor;
}

void scrReplaceCurrentTask(u32 task) {
    ScrProcTask *context = (ScrProcTask *)kwlnTaskGetUserValue();
    if (context != NULL) {
        scrProcDestroyTask(context);
    }
    kwlnTaskSetUserValue(task, 0);
}

void bfStepContext(void) {
    bfContextStep();
}

s32 bfTaskUpdate(void) {
    switch (bfContextStep(kwlnTaskGetUserValue())) {
    case 0:
        return -1;
    case 2:
        return -1;
    case 1:
    default:
        return 0;
    }
}

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

extern ScriptGlobals *datGameState;

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
        return datGameState->ints[script->stack.stackValues[script->stackDepth]];
    case 3:
        return datGameState->floats[script->stack.stackValues[script->stackDepth]];
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
        return (f32)datGameState->ints[script->stack.stackValues[script->stackDepth]];
    case 3:
        return datGameState->floats[script->stack.stackValues[script->stackDepth]];
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
