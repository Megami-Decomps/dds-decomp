#include "common.h"
#include "dat_state.h"
#include "scr.h"


extern u32 kwlnTaskGetUserValue(KwlnTask *task);
extern void kwlnTaskSetUserValue(KwlnTask *task, u32 value);

void scrSetCurrentActor(KwlnTask *task, void *actor) {
    ScrData *context;

    context = (ScrData *)kwlnTaskGetUserValue(task);
    context->actor = actor;
}

void *scrGetCurrentActor(KwlnTask *task) {
    ScrData *context;

    context = (ScrData *)kwlnTaskGetUserValue(task);
    return context->actor;
}

void scrReplaceCurrentTask(KwlnTask *task) {
    ScrData *previousContext;

    previousContext = (ScrData *)kwlnTaskGetUserValue(task);
    if (previousContext != NULL) {
        scrProcDestroyTask(previousContext);
    }
    kwlnTaskSetUserValue(task, 0);
}

void bfStepContext(ScrData *context) {
    bfContextStep(context);
}

s32 bfTaskUpdate(KwlnTask *task) {
    switch (bfContextStep((ScrData *)kwlnTaskGetUserValue(task))) {
    case 0:
        return -1;
    case 2:
        return -1;
    case 1:
    default:
        return 0;
    }
}

void scrPushInteger(ScrData *script, s32 value) {
    script->stackTypes[script->sp] = 0;
    script->stackValues[script->sp].i = value;
    script->sp = script->sp + 1;
}

void bfStackPushFloat(ScrData *script, f32 value) {
    script->stackTypes[script->sp] = 1;
    script->stackValues[script->sp].f = value;
    script->sp = script->sp + 1;
}

void scrPushString(ScrData *script, char *value) {
    script->stackTypes[script->sp] = 5;
    script->stackValues[script->sp].s = value;
    script->sp = script->sp + 1;
}

void scrPushTypeFourValue(ScrData *script, s32 value) {
    script->stackTypes[script->sp] = 4;
    script->stackValues[script->sp].i = value;
    script->sp = script->sp + 1;
}


s32 bfStackPopInt(ScrData *script) {
    s32 stackIndex = script->sp;

    script->sp = stackIndex - 1;
    switch (script->stackTypes[stackIndex - 1]) {
    case 0:
    case 4:
        return script->stackValues[script->sp].i;
    case 1:
        return script->stackValues[script->sp].f;
    case 2:
        return datGameState->script.ints[script->stackValues[script->sp].i];
    case 3:
        return datGameState->script.floats[script->stackValues[script->sp].i];
    }
    return 0;
}

f32 bfStackPopFloat(ScrData *script) {
    s32 stackIndex = script->sp;

    script->sp = stackIndex - 1;
    switch (script->stackTypes[stackIndex - 1]) {
    case 0:
        return (f32)script->stackValues[script->sp].i;
    case 4:
        return 0.0f;
    case 1:
        return script->stackValues[script->sp].f;
    case 2:
        return (f32)datGameState->script.ints[script->stackValues[script->sp].i];
    case 3:
        return datGameState->script.floats[script->stackValues[script->sp].i];
    }
    return 0.0f;
}

/* Push the word following the opcode, then advance past its operand. */
u32 scrPushNextInstructionValue(ScrData *script) {
    s32 operandPc;

    operandPc = script->pc + 1;
    script->pc = operandPc;
    scrPushInteger(script, script->instructions[operandPc].iOperand);
    script->pc = script->pc + 1;
    return 1;
}
