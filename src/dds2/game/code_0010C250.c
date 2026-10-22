#include "common.h"
#include "kwln.h"
#include "scr.h"
#include "dat_state.h"


void scrSetUserData(KwlnTask *task, void *actor) {
    ScrData *context;

    context = (ScrData *)dds3GetProcessData(task);
    context->actor = actor;
}

void *scrGetUserData(KwlnTask *task) {
    ScrData *context;

    context = (ScrData *)dds3GetProcessData(task);
    return context->actor;
}

void scrScriptShutdown(KwlnTask *task) {
    ScrData *context = (ScrData *)dds3GetProcessData(task);
    if (context != NULL) {
        scrProcDestroyTask(context);
    }
    dds3SetProcessData(task, 0);
}

void scrRunScript(ScrData *context) {
    scrTraceCode(context);
}

s32 scrScriptProcess(KwlnTask *task) {
    switch (scrTraceCode((ScrData *)dds3GetProcessData(task))) {
    case 0:
        return -1;
    case 2:
        return -1;
    case 1:
    default:
        return 0;
    }
}

void PushInt(ScrData *script, s32 integerValue) {
    script->stackTypes[script->sp] = 0;
    script->stackValues[script->sp].i = integerValue;
    script->sp = script->sp + 1;
}

void PushFloat(ScrData *script, f32 floatValue) {
    script->stackTypes[script->sp] = 1;
    script->stackValues[script->sp].f = floatValue;
    script->sp = script->sp + 1;
}


void PushStr(ScrData *script, char *string) {
    script->stackTypes[script->sp] = 5;
    script->stackValues[script->sp].s = string;
    script->sp = script->sp + 1;
}

void PushRet(ScrData *script, s32 programCounter) {
    script->stackTypes[script->sp] = 4;
    script->stackValues[script->sp].i = programCounter;
    script->sp = script->sp + 1;
}


s32 PopInt(ScrData *script) {
    s32 stackIndex = script->sp;

    script->sp = stackIndex - 1;
    switch (script->stackTypes[stackIndex - 1]) {
    case 0:
    case 4:
        return script->stackValues[script->sp].i;
    case 1:
        return script->stackValues[script->sp].f;
    case 2:
        return DDS3_GBWK->script.ints[script->stackValues[script->sp].i];
    case 3:
        return DDS3_GBWK->script.floats[script->stackValues[script->sp].i];
    }
    return 0;
}

f32 PopFloat(ScrData *script) {
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
        return (f32)DDS3_GBWK->script.ints[script->stackValues[script->sp].i];
    case 3:
        return DDS3_GBWK->script.floats[script->stackValues[script->sp].i];
    }
    return 0.0f;
}

/* Push the word following the opcode, then advance past its operand. */
u32 CodeFunc_PushI(ScrData *script) {
    s32 operandPc;

    operandPc = script->pc + 1;
    script->pc = operandPc;
    PushInt(script, script->instructions[operandPc].iOperand);
    script->pc = script->pc + 1;
    return 1;
}
