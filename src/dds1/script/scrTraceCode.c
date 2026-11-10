#include "common.h"
#include "kwln.h"
#include "dat_state.h"
#include "scr.h"
#include "scr_process_task_owner_api.h"
#include "scr_trace_result.h"
#include "kwln_task_runner.h"
#include "scr_command_result.h"
#include "scr_binary_operator.h"
#include "scr_string_parameter_api.h"
#include "scr_arg_read.h"
#include "scr_integer_parameter_api.h"
#include "scr_command_timer_api.h"
#include "scr_program_counter_api.h"
#include "scr_current_process_user_data_api.h"
#include "mdl_flag_api.h"

void PushInt(scrProcessWork_s *script, ScrVmInteger integerValue) {
    script->stackTypes[script->sp] = SCR_STACK_TYPE_INT;
    script->stackValues[script->sp].i = integerValue;
    script->sp = script->sp + 1;
}

void PushFloat(scrProcessWork_s *script, f32 floatValue) {
    script->stackTypes[script->sp] = SCR_STACK_TYPE_FLOAT;
    script->stackValues[script->sp].f = floatValue;
    script->sp = script->sp + 1;
}

void PushStr(scrProcessWork_s *script, const char *string) {
    script->stackTypes[script->sp] = SCR_STACK_TYPE_STRING;
    script->stackValues[script->sp].i = EE_ADDR_S(string);
    script->sp = script->sp + 1;
}

void PushRet(scrProcessWork_s *script, ScrProgramCounter returnAddress) {
    script->stackTypes[script->sp] = SCR_STACK_TYPE_RET;
    script->stackValues[script->sp].i = returnAddress;
    script->sp = script->sp + 1;
}


ScrVmInteger PopInt(scrProcessWork_s *script) {
    ScrStackDepth stackDepth = script->sp;

    script->sp = stackDepth - 1;
    switch (script->stackTypes[stackDepth - 1]) {
    case SCR_STACK_TYPE_INT:
        return script->stackValues[script->sp].i;
    case SCR_STACK_TYPE_RET:
        return script->stackValues[script->sp].i;
    case SCR_STACK_TYPE_FLOAT:
        return script->stackValues[script->sp].f;
    case SCR_STACK_TYPE_INTVAR:
        return DDS3_GBWK->script.ints[script->stackValues[script->sp].i];
    case SCR_STACK_TYPE_FLOATVAR:
        return DDS3_GBWK->script.floats[script->stackValues[script->sp].i];
    }
    return 0;
}

f32 PopFloat(scrProcessWork_s *script) {
    ScrStackDepth stackDepth = script->sp;

    script->sp = stackDepth - 1;
    switch (script->stackTypes[stackDepth - 1]) {
    case SCR_STACK_TYPE_INT:
        return (f32)script->stackValues[script->sp].i;
    case SCR_STACK_TYPE_RET:
        return 0.0f;
    case SCR_STACK_TYPE_FLOAT:
        return script->stackValues[script->sp].f;
    case SCR_STACK_TYPE_INTVAR:
        return (f32)DDS3_GBWK->script.ints[script->stackValues[script->sp].i];
    case SCR_STACK_TYPE_FLOATVAR:
        return DDS3_GBWK->script.floats[script->stackValues[script->sp].i];
    }
    return 0.0f;
}

/* Push the word following the opcode, then advance past its operand. */
u32 CodeFunc_PushI(scrProcessWork_s *script) {
    s32 operandPc;

    operandPc = script->pc + 1;
    script->pc = operandPc;
    PushInt(script, script->instructions[operandPc].i);
    script->pc = script->pc + 1;
    return 1;
}

extern const ScrCommand D_0039E288[];
extern ScrOpcodeHandlerResult (*codefunc[])(scrProcessWork_s *scr);

void CodeFunc_2Operation(scrProcessWork_s *scr, ScrBinaryOperator operatorCode);

ScrOpcodeHandlerResult CodeFunc_PushIS(scrProcessWork_s *scr)
{
    PushInt(scr, scr->instructions[scr->pc].s.s1);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushF(scrProcessWork_s *scr)
{
    scr->pc++;
    PushFloat(scr, scr->instructions[scr->pc].f);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushIX(scrProcessWork_s *scr)
{
    PushInt(scr, DDS3_GBWK->script.ints[(ScrGlobalIntegerIndex)scr->instructions[scr->pc].s.s1]);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushFX(scrProcessWork_s *scr)
{
    PushFloat(scr, DDS3_GBWK->script.floats[(ScrGlobalFloatIndex)scr->instructions[scr->pc].s.s1]);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushLIX(scrProcessWork_s *scr)
{
    PushInt(scr, scr->localInt[scr->instructions[scr->pc].s.s1]);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushLFX(scrProcessWork_s *scr)
{
    PushFloat(scr, scr->localFloat[scr->instructions[scr->pc].s.s1]);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushSTR(scrProcessWork_s *scr)
{
    PushStr(scr, scr->strings + scr->instructions[scr->pc].s.s1);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

/* Copy the VM's reserved return slot back onto its active operand stack. */
ScrOpcodeHandlerResult CodeFunc_PushReg(scrProcessWork_s *scr)
{
    scr->stackTypes[scr->sp] = scr->stackTypes[SCR_STACK_RETURN_VALUE_SLOT];
    scr->stackValues[scr->sp].i = scr->stackValues[SCR_STACK_RETURN_VALUE_SLOT].i;
    scr->sp++;
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PopIX(scrProcessWork_s *scr)
{
    DDS3_GBWK->script.ints[(ScrGlobalIntegerIndex)scr->instructions[scr->pc].s.s1] = PopInt(scr);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PopFX(scrProcessWork_s *scr)
{
    f32 floatValue;

    floatValue = PopFloat(scr);
    DDS3_GBWK->script.floats[(ScrGlobalFloatIndex)scr->instructions[scr->pc].s.s1] = floatValue;
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PopLIX(scrProcessWork_s *scr)
{
    scr->localInt[scr->instructions[scr->pc].s.s1] = PopInt(scr);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PopLFX(scrProcessWork_s *scr)
{
    scr->localFloat[scr->instructions[scr->pc].s.s1] = PopFloat(scr);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Proc(scrProcessWork_s *scr)
{
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

/* Yield without consuming operands until the command completes. */
ScrOpcodeHandlerResult CodeFunc_Comm(scrProcessWork_s *scr)
{
    s32 commandPc = scr->pc;
    ScrCommandIndex commandIndex = scr->instructions[commandPc].s.s1;
    ScrCommandResult commandResult;

    nowpwork = scr;
    commandResult = D_0039E288[commandIndex].func();
    if (commandResult == SCR_COMMAND_RESULT_PENDING) {
        return SCR_OPCODE_HANDLER_RESULT_YIELDED;
    }
    scr->sp -= D_0039E288[commandIndex].paramCount;
    if (commandPc == scr->pc) {
        scr->pc = commandPc + 1;
    }
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_End(scrProcessWork_s *scr)
{
    ScrProgramCounter returnAddress;

    if (scr->sp == 0) {
        return SCR_OPCODE_HANDLER_RESULT_FINISHED;
    }
    returnAddress = PopInt(scr);
    scr->pc = returnAddress + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Jump(scrProcessWork_s *scr)
{
    scr->pc = scr->procedures[scr->instructions[scr->pc].s.s1].addr;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

/* Save the return PC before transferring control to a procedure. */
ScrOpcodeHandlerResult CodeFunc_Call(scrProcessWork_s *scr)
{
    PushRet(scr, scr->pc);
    scr->pc = scr->procedures[scr->instructions[scr->pc].s.s1].addr;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Run(scrProcessWork_s *scr)
{
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Goto(scrProcessWork_s *scr)
{
    scr->pc = scr->labels[scr->instructions[scr->pc].s.s1].addr;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

extern s32 CodeFunc_2Operation_i1;

extern s32 CodeFunc_2Operation_i2;

extern s32 CodeFunc_2Operation_ri;

extern f32 CodeFunc_2Operation_f1;

extern f32 CodeFunc_2Operation_f2;

extern f32 CodeFunc_2Operation_rf;

extern s32 CodeFunc_2Operation_t1;

extern s32 CodeFunc_2Operation_t2;

void CodeFunc_2Operation(scrProcessWork_s *scr, ScrBinaryOperator operatorCode)
{
    CodeFunc_2Operation_t1 = scr->stackTypes[scr->sp - 1];
    CodeFunc_2Operation_t2 = scr->stackTypes[scr->sp - 2];
    if ((CodeFunc_2Operation_t1 == SCR_STACK_TYPE_INT ||
         CodeFunc_2Operation_t1 == SCR_STACK_TYPE_INTVAR) &&
        (CodeFunc_2Operation_t2 == SCR_STACK_TYPE_INT ||
         CodeFunc_2Operation_t2 == SCR_STACK_TYPE_INTVAR)) {
        CodeFunc_2Operation_i1 = PopInt(scr);
        CodeFunc_2Operation_i2 = PopInt(scr);
        switch (operatorCode) {
        case SCR_BINARY_OPERATOR_ADD:
            CodeFunc_2Operation_ri = CodeFunc_2Operation_i1 + CodeFunc_2Operation_i2;
            break;
        case SCR_BINARY_OPERATOR_SUBTRACT:
            CodeFunc_2Operation_ri = CodeFunc_2Operation_i1 - CodeFunc_2Operation_i2;
            break;
        case SCR_BINARY_OPERATOR_MULTIPLY:
            CodeFunc_2Operation_ri = CodeFunc_2Operation_i1 * CodeFunc_2Operation_i2;
            break;
        case SCR_BINARY_OPERATOR_DIVIDE:
            CodeFunc_2Operation_ri = CodeFunc_2Operation_i1 / CodeFunc_2Operation_i2;
            break;
        case SCR_BINARY_OPERATOR_LOGICAL_OR:
            if (CodeFunc_2Operation_i1 != 0 || CodeFunc_2Operation_i2 != 0) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case SCR_BINARY_OPERATOR_LOGICAL_AND:
            if (CodeFunc_2Operation_i1 != 0 && CodeFunc_2Operation_i2 != 0) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case SCR_BINARY_OPERATOR_EQUAL:
            if (CodeFunc_2Operation_i1 == CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case SCR_BINARY_OPERATOR_NOT_EQUAL:
            if (CodeFunc_2Operation_i1 != CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case SCR_BINARY_OPERATOR_LESS_THAN:
            if (CodeFunc_2Operation_i1 < CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case SCR_BINARY_OPERATOR_GREATER_THAN:
            if (CodeFunc_2Operation_i1 > CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case SCR_BINARY_OPERATOR_LESS_OR_EQUAL:
            if (CodeFunc_2Operation_i1 <= CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case SCR_BINARY_OPERATOR_GREATER_OR_EQUAL:
            if (CodeFunc_2Operation_i1 >= CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        }
        PushInt(scr, CodeFunc_2Operation_ri);
    } else {
        CodeFunc_2Operation_f1 = PopFloat(scr);
        CodeFunc_2Operation_f2 = PopFloat(scr);
        switch (operatorCode) {
        case SCR_BINARY_OPERATOR_ADD:
            CodeFunc_2Operation_rf = CodeFunc_2Operation_f1 + CodeFunc_2Operation_f2;
            break;
        case SCR_BINARY_OPERATOR_SUBTRACT:
            CodeFunc_2Operation_rf = CodeFunc_2Operation_f1 - CodeFunc_2Operation_f2;
            break;
        case SCR_BINARY_OPERATOR_MULTIPLY:
            CodeFunc_2Operation_rf = CodeFunc_2Operation_f1 * CodeFunc_2Operation_f2;
            break;
        case SCR_BINARY_OPERATOR_DIVIDE:
            CodeFunc_2Operation_rf = CodeFunc_2Operation_f1 / CodeFunc_2Operation_f2;
            break;
        case SCR_BINARY_OPERATOR_LOGICAL_OR:
            if (CodeFunc_2Operation_f1 != 0.0 || CodeFunc_2Operation_f2 != 0.0) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(scr, CodeFunc_2Operation_ri);
            return;
        case SCR_BINARY_OPERATOR_LOGICAL_AND:
            if (CodeFunc_2Operation_f1 != 0.0 && CodeFunc_2Operation_f2 != 0.0) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(scr, CodeFunc_2Operation_ri);
            return;
        case SCR_BINARY_OPERATOR_EQUAL:
            if (CodeFunc_2Operation_f1 == CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(scr, CodeFunc_2Operation_ri);
            return;
        case SCR_BINARY_OPERATOR_NOT_EQUAL:
            if (CodeFunc_2Operation_f1 != CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(scr, CodeFunc_2Operation_ri);
            return;
        case SCR_BINARY_OPERATOR_LESS_THAN:
            if (CodeFunc_2Operation_f1 < CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(scr, CodeFunc_2Operation_ri);
            return;
        case SCR_BINARY_OPERATOR_GREATER_THAN:
            if (CodeFunc_2Operation_f1 > CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(scr, CodeFunc_2Operation_ri);
            return;
        case SCR_BINARY_OPERATOR_LESS_OR_EQUAL:
            if (CodeFunc_2Operation_f1 <= CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(scr, CodeFunc_2Operation_ri);
            return;
        case SCR_BINARY_OPERATOR_GREATER_OR_EQUAL:
            if (CodeFunc_2Operation_f1 >= CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(scr, CodeFunc_2Operation_ri);
            return;
        }
        PushFloat(scr, CodeFunc_2Operation_rf);
    }
}

ScrOpcodeHandlerResult CodeFunc_Add(scrProcessWork_s *scr)
{
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_ADD);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Sub(scrProcessWork_s *scr)
{
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_SUBTRACT);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Mul(scrProcessWork_s *scr)
{
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_MULTIPLY);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Div(scrProcessWork_s *scr)
{
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_DIVIDE);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Minus(scrProcessWork_s *scr) {
    switch (scr->stackTypes[scr->sp - 1]) {
    case SCR_STACK_TYPE_INT:
        scr->stackValues[scr->sp - 1].i = -scr->stackValues[scr->sp - 1].i;
        break;
    case SCR_STACK_TYPE_FLOAT:
        scr->stackValues[scr->sp - 1].f = -scr->stackValues[scr->sp - 1].f;
        break;
    case SCR_STACK_TYPE_INTVAR:
        DDS3_GBWK->script.ints[scr->stackValues[scr->sp - 1].i] =
            -DDS3_GBWK->script.ints[scr->stackValues[scr->sp - 1].i];
        break;
    case SCR_STACK_TYPE_FLOATVAR:
        DDS3_GBWK->script.floats[scr->stackValues[scr->sp - 1].i] =
            -DDS3_GBWK->script.floats[scr->stackValues[scr->sp - 1].i];
        break;
    case SCR_STACK_TYPE_RET:
        break;
    }
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Not(scrProcessWork_s *scr) {
    switch (scr->stackTypes[scr->sp - 1]) {
    case SCR_STACK_TYPE_INT:
    case SCR_STACK_TYPE_INTVAR:
        PushInt(scr, PopInt(scr) == 0);
        break;
    case SCR_STACK_TYPE_FLOAT:
    case SCR_STACK_TYPE_FLOATVAR:
        PushInt(scr, PopFloat(scr) == 0.0f);
        break;
    case SCR_STACK_TYPE_RET:
        break;
    }
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Or(scrProcessWork_s *scr)
{
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_LOGICAL_OR);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_And(scrProcessWork_s *scr)
{
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_LOGICAL_AND);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_EQ(scrProcessWork_s *scr)
{
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_EQUAL);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_NEQ(scrProcessWork_s *scr)
{
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_NOT_EQUAL);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_S(scrProcessWork_s *scr)
{
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_LESS_THAN);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_L(scrProcessWork_s *scr)
{
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_GREATER_THAN);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_SE(scrProcessWork_s *scr)
{
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_LESS_OR_EQUAL);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_LE(scrProcessWork_s *scr)
{
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_GREATER_OR_EQUAL);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_IF(scrProcessWork_s *scr)
{
    s32 conditionIsTruthy;

    switch (scr->stackTypes[scr->sp - 1]) {
    case SCR_STACK_TYPE_INT:
    case SCR_STACK_TYPE_INTVAR:
        conditionIsTruthy = PopInt(scr);
        break;
    case SCR_STACK_TYPE_FLOAT:
    case SCR_STACK_TYPE_FLOATVAR:
        conditionIsTruthy = (PopFloat(scr) != 0.0f);
        break;
    case SCR_STACK_TYPE_RET:
        conditionIsTruthy = 0;
        break;
    default:
        conditionIsTruthy = 0;
        break;
    }
    if (conditionIsTruthy != 0)
        scr->pc++;
    else
        scr->pc = scr->labels[scr->instructions[scr->pc].s.s1].addr;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

/* Run instructions until one stops the context: returns 2 when a handler returns 0 (finished),
 * 1 when a handler returns 2 (yield for one tick). */
ScrTraceResult scrTraceCode(scrProcessWork_s *scr)
{
    ScrOpcodeHandlerResult handlerResult;

    while (1) {
        handlerResult = codefunc[scr->instructions[scr->pc].s.s0](scr);
        if (handlerResult == SCR_OPCODE_HANDLER_RESULT_FINISHED) {
            scr->cmdTimer = 0;
            return SCR_TRACE_RESULT_FINISHED;
        }
        if (handlerResult == SCR_OPCODE_HANDLER_RESULT_YIELDED) {
            break;
        }
        if (handlerResult == SCR_OPCODE_HANDLER_RESULT_CONTINUE) {
            scr->cmdTimer = 0;
        }
    }
    scr->cmdTimer++;
    scr->timer++;
    return SCR_TRACE_RESULT_YIELDED;
}

/* Script command parameter `parameterIndex` (0 = first) as an int, converting floats and
 * dereferencing global variable references. */
ScrVmInteger scrGetIntPara(ScrCommandParameterIndex parameterIndex) {
    scrProcessWork_s *scr = nowpwork;
    ScrStackValueIndex stackIndex = scr->sp - parameterIndex - 1;

    switch (scr->stackTypes[stackIndex]) {
    case SCR_STACK_TYPE_INT:
        return scr->stackValues[stackIndex].i;
    case SCR_STACK_TYPE_RET:
        return scr->stackValues[stackIndex].i;
    case SCR_STACK_TYPE_FLOAT:
        return scr->stackValues[stackIndex].f;
    case SCR_STACK_TYPE_INTVAR:
        return DDS3_GBWK->script.ints[scr->stackValues[stackIndex].i];
    case SCR_STACK_TYPE_FLOATVAR:
        return DDS3_GBWK->script.floats[scr->stackValues[stackIndex].i];
    }
    return 0;
}

f32 scrGetFloatPara(ScrCommandParameterIndex parameterIndex)
{
    scrProcessWork_s *scr = nowpwork;
    ScrStackValueIndex stackIndex = scr->sp - parameterIndex - 1;

    switch (scr->stackTypes[stackIndex]) {
    case SCR_STACK_TYPE_INT:
        return (f32)scr->stackValues[stackIndex].i;
    case SCR_STACK_TYPE_RET:
        return (f32)scr->stackValues[stackIndex].i;
    case SCR_STACK_TYPE_FLOAT:
        return scr->stackValues[stackIndex].f;
    case SCR_STACK_TYPE_INTVAR:
        return (f32)DDS3_GBWK->script.ints[scr->stackValues[stackIndex].i];
    case SCR_STACK_TYPE_FLOATVAR:
        return DDS3_GBWK->script.floats[scr->stackValues[stackIndex].i];
    }
    return 0.0f;
}

/* Return a string parameter only when its VM stack tag is STRING. */
const char *scrGetStrPara(ScrCommandParameterIndex parameterIndex)
{
    scrProcessWork_s *scr = nowpwork;
    ScrStackValueIndex stackIndex = scr->sp - parameterIndex - 1;
    s32 stackType = (s8)scr->stackTypes[stackIndex];

    if (stackType >= 0) {
        if (stackType >= SCR_STACK_TYPE_STRING) {
            if (stackType == SCR_STACK_TYPE_STRING) {
                return EE_ADDR_AS(const char *, scr->stackValues[stackIndex].i);
            }
        }
    }
    return NULL;
}

/* A command's return value: its type tag, then its value, in the running
 * context's return slot. The tag goes through the context as read on entry,
 * the value through nowpwork read again, since the byte store may
 * have changed it as far as C knows; retail reads it twice. */

#define SCR_SET_RETURN_VALUE(scr, type, member, value)                   \
    do {                                                                 \
        (scr)->stackTypes[SCR_STACK_RETURN_VALUE_SLOT] = (type);          \
        nowpwork->stackValues[SCR_STACK_RETURN_VALUE_SLOT].member = (value); \
    } while (0)

/* The setters return the context they set the value in, which retail
 * leaves in v0: a command that ends by setting its value returns it as its
 * result, and a nonzero result tells CodeFunc_Comm the command is done. */
scrProcessWork_s *scrSetIntReturnValue(ScrVmInteger retVal)
{
    scrProcessWork_s *scr = nowpwork;
    SCR_SET_RETURN_VALUE(scr, SCR_STACK_TYPE_INT, i, retVal);
    return scr;
}

scrProcessWork_s *scrSetFloatReturnValue(f32 retVal)
{
    scrProcessWork_s *scr = nowpwork;
    SCR_SET_RETURN_VALUE(scr, SCR_STACK_TYPE_FLOAT, f, retVal);
    return scr;
}

ScrProgramCounter scrGetProcedureCodeAddress(ScrProcedureIndex procedureIndex) {
    return nowpwork->procedures[procedureIndex].addr;
}

ScrProgramCounter scrGetLabelCodeAddress(ScrLabelIndex labelIndex) {
    return nowpwork->labels[labelIndex].addr;
}

ScrProgramCounter scrGetCodePointer(void) {
    return nowpwork->pc;
}

void scrSetCodePointer(ScrProgramCounter programCounter) {
    nowpwork->pc = programCounter;
}

u32 scrGetScriptTimer(void) {
    return nowpwork->timer;
}

ScrCommandYieldCount scrGetCommandTimer(void) {
    return nowpwork->cmdTimer;
}

ItfMesWindowIndex scrGetMessageHandle(void) {
    return nowpwork->messageWindowIndex;
}

scrProcessWork_s *scrGetScriptHandle(void) {
    return nowpwork;
}

void *scrGetCurrentProcessUserData(void) {
    return nowpwork->userData;
}
