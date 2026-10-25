#include "common.h"
#include "kwln.h"
#include "scr.h"
#include "scr_trace_result.h"
#include "dat_state.h"


void scrSetUserData(dds3ProcessID_t *task, void *userData) {
    scrProcessWork_s *context;

    context = (scrProcessWork_s *)dds3GetProcessData(task);
    context->userData = userData;
}

void *scrGetUserData(dds3ProcessID_t *task) {
    scrProcessWork_s *context;

    context = (scrProcessWork_s *)dds3GetProcessData(task);
    return context->userData;
}

void scrScriptShutdown(dds3ProcessID_t *task) {
    scrProcessWork_s *context = (scrProcessWork_s *)dds3GetProcessData(task);
    if (context != NULL) {
        scrProcDestroyTask(context);
    }
    dds3SetProcessData(task, 0);
}

void scrRunScript(scrProcessWork_s *context) {
    scrTraceCode(context);
}

s32 scrScriptProcess(dds3ProcessID_t *task) {
    switch (scrTraceCode((scrProcessWork_s *)dds3GetProcessData(task))) {
    case 0:
        return -1;
    case SCR_TRACE_RESULT_FINISHED:
        return -1;
    case SCR_TRACE_RESULT_YIELDED:
    default:
        return 0;
    }
}

void PushInt(scrProcessWork_s *script, s32 integerValue) {
    script->stackTypes[script->sp] = SCR_STACK_TYPE_INTEGER;
    script->stackValues[script->sp].integer = integerValue;
    script->sp = script->sp + 1;
}

void PushFloat(scrProcessWork_s *script, f32 floatValue) {
    script->stackTypes[script->sp] = SCR_STACK_TYPE_FLOAT;
    script->stackValues[script->sp].floating = floatValue;
    script->sp = script->sp + 1;
}


void PushStr(scrProcessWork_s *script, const char *string) {
    script->stackTypes[script->sp] = SCR_STACK_TYPE_STRING;
    script->stackValues[script->sp].string = string;
    script->sp = script->sp + 1;
}

void PushRet(scrProcessWork_s *script, ScrProgramCounter returnAddress) {
    script->stackTypes[script->sp] = SCR_STACK_TYPE_RETURN_ADDRESS;
    script->stackValues[script->sp].returnAddress = returnAddress;
    script->sp = script->sp + 1;
}


s32 PopInt(scrProcessWork_s *script) {
    s32 stackIndex = script->sp;

    script->sp = stackIndex - 1;
    switch (script->stackTypes[stackIndex - 1]) {
    case SCR_STACK_TYPE_INTEGER:
        return script->stackValues[script->sp].integer;
    case SCR_STACK_TYPE_RETURN_ADDRESS:
        return script->stackValues[script->sp].returnAddress;
    case SCR_STACK_TYPE_FLOAT:
        return script->stackValues[script->sp].floating;
    case SCR_STACK_TYPE_INTEGER_REFERENCE:
        return DDS3_GBWK->script.ints[script->stackValues[script->sp].globalIntegerIndex];
    case SCR_STACK_TYPE_FLOAT_REFERENCE:
        return DDS3_GBWK->script.floats[script->stackValues[script->sp].globalFloatIndex];
    }
    return 0;
}

f32 PopFloat(scrProcessWork_s *script) {
    s32 stackIndex = script->sp;

    script->sp = stackIndex - 1;
    switch (script->stackTypes[stackIndex - 1]) {
    case SCR_STACK_TYPE_INTEGER:
        return (f32)script->stackValues[script->sp].integer;
    case SCR_STACK_TYPE_RETURN_ADDRESS:
        return 0.0f;
    case SCR_STACK_TYPE_FLOAT:
        return script->stackValues[script->sp].floating;
    case SCR_STACK_TYPE_INTEGER_REFERENCE:
        return (f32)DDS3_GBWK->script.ints[script->stackValues[script->sp].globalIntegerIndex];
    case SCR_STACK_TYPE_FLOAT_REFERENCE:
        return DDS3_GBWK->script.floats[script->stackValues[script->sp].globalFloatIndex];
    }
    return 0.0f;
}

/* Push the word following the opcode, then advance past its operand. */
u32 CodeFunc_PushI(scrProcessWork_s *script) {
    s32 operandPc;

    operandPc = script->pc + 1;
    script->pc = operandPc;
    PushInt(script, script->instructions[operandPc].iOperand);
    script->pc = script->pc + 1;
    return 1;
}
