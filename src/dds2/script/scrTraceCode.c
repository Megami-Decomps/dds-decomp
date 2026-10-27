#include "dat_state.h"
#include "common.h"
#include "scr.h"
#include "scr_command_result.h"
#include "scr_binary_operator.h"
#include "scr_trace_result.h"
#include "scr_string_parameter_api.h"
#include "scr_arg_read.h"


extern ScrOpcodeHandlerResult (*codefunc[])(scrProcessWork_s *scr);
extern const ScrCommand D_00411408[];

ScrOpcodeHandlerResult CodeFunc_PushIS(scrProcessWork_s *scr) {
    PushInt(scr, scr->instructions[scr->pc].parts.sOperand);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushF(scrProcessWork_s *scr) {
    scr->pc++;
    PushFloat(scr, scr->instructions[scr->pc].fOperand);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushIX(scrProcessWork_s *scr) {
    PushInt(scr, DDS3_GBWK->script.ints[(ScrGlobalIntegerIndex)scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushFX(scrProcessWork_s *scr) {
    PushFloat(scr, DDS3_GBWK->script.floats[(ScrGlobalFloatIndex)scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushLIX(scrProcessWork_s *scr) {
    PushInt(scr, scr->localInt[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushLFX(scrProcessWork_s *scr) {
    PushFloat(scr, scr->localFloat[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushSTR(scrProcessWork_s *scr) {
    PushStr(scr, scr->strings + scr->instructions[scr->pc].parts.sOperand);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

/* Copy the VM's reserved return slot back onto its active operand stack. */
ScrOpcodeHandlerResult CodeFunc_PushReg(scrProcessWork_s *scr) {
    scr->stackTypes[scr->sp] = scr->stackTypes[SCR_STACK_RET];
    scr->stackValues[scr->sp].bits = scr->stackValues[SCR_STACK_RET].bits;
    scr->sp++;
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PopIX(scrProcessWork_s *scr) {
    DDS3_GBWK->script.ints[(ScrGlobalIntegerIndex)scr->instructions[scr->pc].parts.sOperand] = PopInt(scr);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PopFX(scrProcessWork_s *scr) {
    f32 floatValue;

    floatValue = PopFloat(scr);
    DDS3_GBWK->script.floats[(ScrGlobalFloatIndex)scr->instructions[scr->pc].parts.sOperand] = floatValue;
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PopLIX(scrProcessWork_s *scr) {
    s32 integerValue;

    integerValue = PopInt(scr);
    scr->localInt[scr->instructions[scr->pc].parts.sOperand] = integerValue;
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PopLFX(scrProcessWork_s *scr) {
    scr->localFloat[scr->instructions[scr->pc].parts.sOperand] = PopFloat(scr);
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Proc(scrProcessWork_s *scr) {
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

/* Yield without consuming operands until the command completes. */
ScrOpcodeHandlerResult CodeFunc_Comm(scrProcessWork_s *scr) {
    s32 programCounter = scr->pc;
    ScrCommandIndex commandIndex = scr->instructions[programCounter].command.commandIndex;
    ScrCommandResult commandResult;

    nowpwork = scr;
    commandResult = D_00411408[commandIndex].func();
    if (commandResult == SCR_COMMAND_RESULT_PENDING) {
        return SCR_OPCODE_HANDLER_RESULT_YIELDED;
    }
    scr->sp -= D_00411408[commandIndex].paramCount;
    if (programCounter == scr->pc) {
        scr->pc = programCounter + 1;
    }
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_End(scrProcessWork_s *scr)
{
    if (scr->sp == 0) {
        return SCR_OPCODE_HANDLER_RESULT_FINISHED;
    }
    scr->pc = PopInt(scr) + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Jump(scrProcessWork_s *scr) {
    scr->pc = scr->procedures[scr->instructions[scr->pc].parts.sOperand].addr;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

/* Save the return PC before transferring control to a procedure. */
ScrOpcodeHandlerResult CodeFunc_Call(scrProcessWork_s *scr) {
    PushRet(scr, scr->pc);
    scr->pc = scr->procedures[scr->instructions[scr->pc].parts.sOperand].addr;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Run(scrProcessWork_s *scr) {
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Goto(scrProcessWork_s *scr) {
    scr->pc = scr->labels[scr->instructions[scr->pc].parts.sOperand].addr;
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
    if ((CodeFunc_2Operation_t1 == SCR_STACK_TYPE_INTEGER ||
         CodeFunc_2Operation_t1 == SCR_STACK_TYPE_INTEGER_REFERENCE) &&
        (CodeFunc_2Operation_t2 == SCR_STACK_TYPE_INTEGER ||
         CodeFunc_2Operation_t2 == SCR_STACK_TYPE_INTEGER_REFERENCE)) {
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

ScrOpcodeHandlerResult CodeFunc_Add(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_ADD);
    scr->pc = scr->pc + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Sub(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_SUBTRACT);
    scr->pc = scr->pc + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Mul(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_MULTIPLY);
    scr->pc = scr->pc + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Div(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_DIVIDE);
    scr->pc = scr->pc + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Minus(scrProcessWork_s *scr) {
    switch (scr->stackTypes[scr->sp - 1]) {
    case SCR_STACK_TYPE_INTEGER:
        scr->stackValues[scr->sp - 1].integer = -scr->stackValues[scr->sp - 1].integer;
        break;
    case SCR_STACK_TYPE_FLOAT:
        scr->stackValues[scr->sp - 1].floating = -scr->stackValues[scr->sp - 1].floating;
        break;
    case SCR_STACK_TYPE_INTEGER_REFERENCE:
        DDS3_GBWK->script.ints[scr->stackValues[scr->sp - 1].globalIntegerIndex] =
            -DDS3_GBWK->script.ints[scr->stackValues[scr->sp - 1].globalIntegerIndex];
        break;
    case SCR_STACK_TYPE_FLOAT_REFERENCE:
        DDS3_GBWK->script.floats[scr->stackValues[scr->sp - 1].globalFloatIndex] =
            -DDS3_GBWK->script.floats[scr->stackValues[scr->sp - 1].globalFloatIndex];
        break;
    case SCR_STACK_TYPE_RETURN_ADDRESS:
        break;
    }
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Not(scrProcessWork_s *scr) {
    switch (scr->stackTypes[scr->sp - 1]) {
    case SCR_STACK_TYPE_INTEGER:
    case SCR_STACK_TYPE_INTEGER_REFERENCE:
        PushInt(scr, PopInt(scr) == 0);
        break;
    case SCR_STACK_TYPE_FLOAT:
    case SCR_STACK_TYPE_FLOAT_REFERENCE:
        PushInt(scr, PopFloat(scr) == 0.0f);
        break;
    case SCR_STACK_TYPE_RETURN_ADDRESS:
        break;
    }
    scr->pc++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Or(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_LOGICAL_OR);
    scr->pc = scr->pc + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_And(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_LOGICAL_AND);
    scr->pc = scr->pc + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_EQ(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_EQUAL);
    scr->pc = scr->pc + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_NEQ(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_NOT_EQUAL);
    scr->pc = scr->pc + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_S(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_LESS_THAN);
    scr->pc = scr->pc + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_L(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_GREATER_THAN);
    scr->pc = scr->pc + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_SE(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_LESS_OR_EQUAL);
    scr->pc = scr->pc + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_LE(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_GREATER_OR_EQUAL);
    scr->pc = scr->pc + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_IF(scrProcessWork_s *scr)
{
    s32 conditionIsTruthy;

    switch (scr->stackTypes[scr->sp - 1]) {
    case SCR_STACK_TYPE_INTEGER:
    case SCR_STACK_TYPE_INTEGER_REFERENCE:
        conditionIsTruthy = PopInt(scr);
        break;
    case SCR_STACK_TYPE_FLOAT:
    case SCR_STACK_TYPE_FLOAT_REFERENCE:
        conditionIsTruthy = (PopFloat(scr) != 0.0f);
        break;
    case SCR_STACK_TYPE_RETURN_ADDRESS:
        conditionIsTruthy = 0;
        break;
    default:
        conditionIsTruthy = 0;
        break;
    }
    if (conditionIsTruthy != 0)
        scr->pc++;
    else
        scr->pc = scr->labels[scr->instructions[scr->pc].parts.sOperand].addr;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

/* Run instructions until one stops the context: returns 2 when a handler returns 0 (finished),
 * 1 when a handler returns 2 (yield for one tick). */
ScrTraceResult scrTraceCode(scrProcessWork_s *scr) {
    ScrOpcodeHandlerResult handlerResult;

    while (1) {
        handlerResult = codefunc[scr->instructions[scr->pc].parts.opCode](scr);
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
s32 scrGetIntPara(ScrCommandParameterIndex parameterIndex) {
    scrProcessWork_s *scr = nowpwork;
    s32 stackIndex = scr->sp - parameterIndex - 1;

    switch (scr->stackTypes[stackIndex]) {
    case SCR_STACK_TYPE_INTEGER:
        return scr->stackValues[stackIndex].integer;
    case SCR_STACK_TYPE_RETURN_ADDRESS:
        return scr->stackValues[stackIndex].returnAddress;
    case SCR_STACK_TYPE_FLOAT:
        return scr->stackValues[stackIndex].floating;
    case SCR_STACK_TYPE_INTEGER_REFERENCE:
        return DDS3_GBWK->script.ints[scr->stackValues[stackIndex].globalIntegerIndex];
    case SCR_STACK_TYPE_FLOAT_REFERENCE:
        return DDS3_GBWK->script.floats[scr->stackValues[stackIndex].globalFloatIndex];
    }
    return 0;
}

f32 scrGetFloatPara(ScrCommandParameterIndex parameterIndex)
{
    scrProcessWork_s *scr = nowpwork;
    s32 stackIndex = scr->sp - parameterIndex - 1;

    switch (scr->stackTypes[stackIndex]) {
    case SCR_STACK_TYPE_INTEGER:
        return (f32)scr->stackValues[stackIndex].integer;
    case SCR_STACK_TYPE_RETURN_ADDRESS:
        return (f32)scr->stackValues[stackIndex].returnAddress;
    case SCR_STACK_TYPE_FLOAT:
        return scr->stackValues[stackIndex].floating;
    case SCR_STACK_TYPE_INTEGER_REFERENCE:
        return (f32)DDS3_GBWK->script.ints[scr->stackValues[stackIndex].globalIntegerIndex];
    case SCR_STACK_TYPE_FLOAT_REFERENCE:
        return DDS3_GBWK->script.floats[scr->stackValues[stackIndex].globalFloatIndex];
    }
    return 0.0f;
}

/* Return a string parameter only when its VM stack tag is STRING. */
const char *scrGetStrPara(ScrCommandParameterIndex parameterIndex)
{
    scrProcessWork_s *scr = nowpwork;
    s32 stackIndex = scr->sp - parameterIndex - 1;
    s32 stackType = (s8)scr->stackTypes[stackIndex];

    if (stackType >= 0) {
        if (stackType >= SCR_STACK_TYPE_STRING) {
            if (stackType == SCR_STACK_TYPE_STRING) {
                return scr->stackValues[stackIndex].string;
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
        (scr)->stackTypes[SCR_STACK_RET] = (type);                       \
        nowpwork->stackValues[SCR_STACK_RET].member = (value);  \
    } while (0)

/* The setters return the context they set the value in, which retail
 * leaves in v0: a command that ends by setting its value returns it as its
 * result, and a nonzero result tells CodeFunc_Comm the command is done. */
scrProcessWork_s *scrSetIntReturnValue(s32 retVal)
{
    scrProcessWork_s *scr = nowpwork;
    SCR_SET_RETURN_VALUE(scr, SCR_STACK_TYPE_INTEGER, integer, retVal);
    return scr;
}

scrProcessWork_s *scrSetFloatReturnValue(f32 retVal)
{
    scrProcessWork_s *scr = nowpwork;
    SCR_SET_RETURN_VALUE(scr, SCR_STACK_TYPE_FLOAT, floating, retVal);
    return scr;
}
