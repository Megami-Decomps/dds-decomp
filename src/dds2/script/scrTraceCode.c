#include "common.h"
#include "kwln.h"
#include "scr.h"
#include "scr_process_task_owner_api.h"
#include "scr_trace_result.h"
#include "kwln_task_runner.h"
#include "dat_state.h"
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
    script->StackType[script->SP] = SCR_STACK_TYPE_INT;
    script->StackData[script->SP].i = integerValue;
    script->SP = script->SP + 1;
}

void PushFloat(scrProcessWork_s *script, f32 floatValue) {
    script->StackType[script->SP] = SCR_STACK_TYPE_FLOAT;
    script->StackData[script->SP].f = floatValue;
    script->SP = script->SP + 1;
}

void PushStr(scrProcessWork_s *script, const char *string) {
    script->StackType[script->SP] = SCR_STACK_TYPE_STRING;
    script->StackData[script->SP].i = EE_ADDR_S(string);
    script->SP = script->SP + 1;
}

void PushRet(scrProcessWork_s *script, ScrProgramCounter returnAddress) {
    script->StackType[script->SP] = SCR_STACK_TYPE_RET;
    script->StackData[script->SP].i = returnAddress;
    script->SP = script->SP + 1;
}


ScrVmInteger PopInt(scrProcessWork_s *script) {
    ScrStackDepth stackDepth = script->SP;

    script->SP = stackDepth - 1;
    switch ((s8)script->StackType[stackDepth - 1]) {
    case SCR_STACK_TYPE_INT:
        return script->StackData[script->SP].i;
    case SCR_STACK_TYPE_RET:
        return script->StackData[script->SP].i;
    case SCR_STACK_TYPE_FLOAT:
        return script->StackData[script->SP].f;
    case SCR_STACK_TYPE_INTVAR:
        return DDS3_GBWK->script.ints[script->StackData[script->SP].i];
    case SCR_STACK_TYPE_FLOATVAR:
        return DDS3_GBWK->script.floats[script->StackData[script->SP].i];
    }
    return 0;
}

f32 PopFloat(scrProcessWork_s *script) {
    ScrStackDepth stackDepth = script->SP;

    script->SP = stackDepth - 1;
    switch ((s8)script->StackType[stackDepth - 1]) {
    case SCR_STACK_TYPE_INT:
        return (f32)script->StackData[script->SP].i;
    case SCR_STACK_TYPE_RET:
        return 0.0f;
    case SCR_STACK_TYPE_FLOAT:
        return script->StackData[script->SP].f;
    case SCR_STACK_TYPE_INTVAR:
        return (f32)DDS3_GBWK->script.ints[script->StackData[script->SP].i];
    case SCR_STACK_TYPE_FLOATVAR:
        return DDS3_GBWK->script.floats[script->StackData[script->SP].i];
    }
    return 0.0f;
}

/* Push the word following the opcode, then advance past its operand. */
u32 CodeFunc_PushI(scrProcessWork_s *script) {
    s32 operandPc;

    operandPc = script->CP + 1;
    script->CP = operandPc;
    PushInt(script, script->Code[operandPc].i);
    script->CP = script->CP + 1;
    return 1;
}

extern ScrOpcodeHandlerResult (*codefunc[])(scrProcessWork_s *scr);

extern const scrCommandTable_t D_00411408[];
ScrOpcodeHandlerResult CodeFunc_PushIS(scrProcessWork_s *scr) {
    PushInt(scr, scr->Code[scr->CP].s.s1);
    scr->CP++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushF(scrProcessWork_s *scr) {
    scr->CP++;
    PushFloat(scr, scr->Code[scr->CP].f);
    scr->CP++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushIX(scrProcessWork_s *scr) {
    PushInt(scr, DDS3_GBWK->script.ints[(ScrGlobalIntegerIndex)scr->Code[scr->CP].s.s1]);
    scr->CP++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushFX(scrProcessWork_s *scr) {
    PushFloat(scr, DDS3_GBWK->script.floats[(ScrGlobalFloatIndex)scr->Code[scr->CP].s.s1]);
    scr->CP++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushLIX(scrProcessWork_s *scr) {
    PushInt(scr, scr->LocalInt[scr->Code[scr->CP].s.s1]);
    scr->CP++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushLFX(scrProcessWork_s *scr) {
    PushFloat(scr, scr->LocalFloat[scr->Code[scr->CP].s.s1]);
    scr->CP++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PushSTR(scrProcessWork_s *scr) {
    PushStr(scr, scr->StringData + scr->Code[scr->CP].s.s1);
    scr->CP++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

/* Copy the VM's reserved return slot back onto its active operand stack. */
ScrOpcodeHandlerResult CodeFunc_PushReg(scrProcessWork_s *scr) {
    scr->StackType[scr->SP] = scr->StackType[SCR_STACK_RETURN_VALUE_SLOT];
    scr->StackData[scr->SP].i = scr->StackData[SCR_STACK_RETURN_VALUE_SLOT].i;
    scr->SP++;
    scr->CP++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PopIX(scrProcessWork_s *scr) {
    DDS3_GBWK->script.ints[(ScrGlobalIntegerIndex)scr->Code[scr->CP].s.s1] = PopInt(scr);
    scr->CP++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PopFX(scrProcessWork_s *scr) {
    f32 floatValue;

    floatValue = PopFloat(scr);
    DDS3_GBWK->script.floats[(ScrGlobalFloatIndex)scr->Code[scr->CP].s.s1] = floatValue;
    scr->CP++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PopLIX(scrProcessWork_s *scr) {
    ScrVmInteger integerValue;

    integerValue = PopInt(scr);
    scr->LocalInt[scr->Code[scr->CP].s.s1] = integerValue;
    scr->CP++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_PopLFX(scrProcessWork_s *scr) {
    scr->LocalFloat[scr->Code[scr->CP].s.s1] = PopFloat(scr);
    scr->CP++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Proc(scrProcessWork_s *scr) {
    scr->CP++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

/* Yield without consuming operands until the command completes. */
ScrOpcodeHandlerResult CodeFunc_Comm(scrProcessWork_s *scr) {
    s32 programCounter = scr->CP;
    ScrCommandIndex commandIndex = scr->Code[programCounter].s.s1;
    ScrCommandResult commandResult;

    nowpwork = scr;
    commandResult = D_00411408[commandIndex].comm();
    if (commandResult == SCR_COMMAND_RESULT_PENDING) {
        return SCR_OPCODE_HANDLER_RESULT_YIELDED;
    }
    scr->SP -= D_00411408[commandIndex].paranum;
    if (programCounter == scr->CP) {
        scr->CP = programCounter + 1;
    }
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_End(scrProcessWork_s *scr)
{
    ScrProgramCounter returnAddress;

    if (scr->SP == 0) {
        return SCR_OPCODE_HANDLER_RESULT_FINISHED;
    }
    returnAddress = PopInt(scr);
    scr->CP = returnAddress + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Jump(scrProcessWork_s *scr) {
    scr->CP = scr->ProcTable[scr->Code[scr->CP].s.s1].CodeAddress;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

/* Save the return PC before transferring control to a procedure. */
ScrOpcodeHandlerResult CodeFunc_Call(scrProcessWork_s *scr) {
    PushRet(scr, scr->CP);
    scr->CP = scr->ProcTable[scr->Code[scr->CP].s.s1].CodeAddress;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Run(scrProcessWork_s *scr) {
    scr->CP++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Goto(scrProcessWork_s *scr) {
    scr->CP = scr->LabelTable[scr->Code[scr->CP].s.s1].CodeAddress;
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
    CodeFunc_2Operation_t1 = (s8)scr->StackType[scr->SP - 1];
    CodeFunc_2Operation_t2 = (s8)scr->StackType[scr->SP - 2];
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

ScrOpcodeHandlerResult CodeFunc_Add(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_ADD);
    scr->CP = scr->CP + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Sub(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_SUBTRACT);
    scr->CP = scr->CP + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Mul(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_MULTIPLY);
    scr->CP = scr->CP + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Div(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_DIVIDE);
    scr->CP = scr->CP + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Minus(scrProcessWork_s *scr) {
    switch ((s8)scr->StackType[scr->SP - 1]) {
    case SCR_STACK_TYPE_INT:
        scr->StackData[scr->SP - 1].i = -scr->StackData[scr->SP - 1].i;
        break;
    case SCR_STACK_TYPE_FLOAT:
        scr->StackData[scr->SP - 1].f = -scr->StackData[scr->SP - 1].f;
        break;
    case SCR_STACK_TYPE_INTVAR:
        DDS3_GBWK->script.ints[scr->StackData[scr->SP - 1].i] =
            -DDS3_GBWK->script.ints[scr->StackData[scr->SP - 1].i];
        break;
    case SCR_STACK_TYPE_FLOATVAR:
        DDS3_GBWK->script.floats[scr->StackData[scr->SP - 1].i] =
            -DDS3_GBWK->script.floats[scr->StackData[scr->SP - 1].i];
        break;
    case SCR_STACK_TYPE_RET:
        break;
    }
    scr->CP++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Not(scrProcessWork_s *scr) {
    switch ((s8)scr->StackType[scr->SP - 1]) {
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
    scr->CP++;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_Or(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_LOGICAL_OR);
    scr->CP = scr->CP + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_And(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_LOGICAL_AND);
    scr->CP = scr->CP + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_EQ(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_EQUAL);
    scr->CP = scr->CP + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_NEQ(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_NOT_EQUAL);
    scr->CP = scr->CP + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_S(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_LESS_THAN);
    scr->CP = scr->CP + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_L(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_GREATER_THAN);
    scr->CP = scr->CP + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_SE(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_LESS_OR_EQUAL);
    scr->CP = scr->CP + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_LE(scrProcessWork_s *scr) {
    CodeFunc_2Operation(scr, SCR_BINARY_OPERATOR_GREATER_OR_EQUAL);
    scr->CP = scr->CP + 1;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

ScrOpcodeHandlerResult CodeFunc_IF(scrProcessWork_s *scr)
{
    s32 conditionIsTruthy;

    switch ((s8)scr->StackType[scr->SP - 1]) {
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
        scr->CP++;
    else
        scr->CP = scr->LabelTable[scr->Code[scr->CP].s.s1].CodeAddress;
    return SCR_OPCODE_HANDLER_RESULT_CONTINUE;
}

/* Run instructions until one stops the context: returns 2 when a handler returns 0 (finished),
 * 1 when a handler returns 2 (yield for one tick). */
ScrTraceResult scrTraceCode(scrProcessWork_s *scr) {
    ScrOpcodeHandlerResult handlerResult;

    while (1) {
        handlerResult = codefunc[scr->Code[scr->CP].s.s0](scr);
        if (handlerResult == SCR_OPCODE_HANDLER_RESULT_FINISHED) {
            scr->CommandTimer = 0;
            return SCR_TRACE_RESULT_FINISHED;
        }
        if (handlerResult == SCR_OPCODE_HANDLER_RESULT_YIELDED) {
            break;
        }
        if (handlerResult == SCR_OPCODE_HANDLER_RESULT_CONTINUE) {
            scr->CommandTimer = 0;
        }
    }
    scr->CommandTimer++;
    scr->ScriptTimer++;
    return SCR_TRACE_RESULT_YIELDED;
}

/* Script command parameter `parameterIndex` (0 = first) as an int, converting floats and
 * dereferencing global variable references. */
ScrVmInteger scrGetIntPara(ScrCommandParameterIndex parameterIndex) {
    scrProcessWork_s *scr = nowpwork;
    ScrStackValueIndex stackIndex = scr->SP - parameterIndex - 1;

    switch ((s8)scr->StackType[stackIndex]) {
    case SCR_STACK_TYPE_INT:
        return scr->StackData[stackIndex].i;
    case SCR_STACK_TYPE_RET:
        return scr->StackData[stackIndex].i;
    case SCR_STACK_TYPE_FLOAT:
        return scr->StackData[stackIndex].f;
    case SCR_STACK_TYPE_INTVAR:
        return DDS3_GBWK->script.ints[scr->StackData[stackIndex].i];
    case SCR_STACK_TYPE_FLOATVAR:
        return DDS3_GBWK->script.floats[scr->StackData[stackIndex].i];
    }
    return 0;
}

f32 scrGetFloatPara(ScrCommandParameterIndex parameterIndex)
{
    scrProcessWork_s *scr = nowpwork;
    ScrStackValueIndex stackIndex = scr->SP - parameterIndex - 1;

    switch ((s8)scr->StackType[stackIndex]) {
    case SCR_STACK_TYPE_INT:
        return (f32)scr->StackData[stackIndex].i;
    case SCR_STACK_TYPE_RET:
        return (f32)scr->StackData[stackIndex].i;
    case SCR_STACK_TYPE_FLOAT:
        return scr->StackData[stackIndex].f;
    case SCR_STACK_TYPE_INTVAR:
        return (f32)DDS3_GBWK->script.ints[scr->StackData[stackIndex].i];
    case SCR_STACK_TYPE_FLOATVAR:
        return DDS3_GBWK->script.floats[scr->StackData[stackIndex].i];
    }
    return 0.0f;
}

/* Return a string parameter only when its VM stack tag is STRING. */
const char *scrGetStrPara(ScrCommandParameterIndex parameterIndex)
{
    scrProcessWork_s *scr = nowpwork;
    ScrStackValueIndex stackIndex = scr->SP - parameterIndex - 1;
    s32 stackType = (s8)scr->StackType[stackIndex];

    if (stackType >= 0) {
        if (stackType >= SCR_STACK_TYPE_STRING) {
            if (stackType == SCR_STACK_TYPE_STRING) {
                return EE_ADDR_AS(const char *, scr->StackData[stackIndex].i);
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
        (scr)->StackType[SCR_STACK_RETURN_VALUE_SLOT] = (type);          \
        nowpwork->StackData[SCR_STACK_RETURN_VALUE_SLOT].member = (value); \
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
    return nowpwork->ProcTable[procedureIndex].CodeAddress;
}

ScrProgramCounter scrGetLabelCodeAddress(ScrLabelIndex labelIndex) {
    return nowpwork->LabelTable[labelIndex].CodeAddress;
}

ScrProgramCounter scrGetCodePointer(void) {
    return nowpwork->CP;
}

void scrSetCodePointer(ScrProgramCounter programCounter) {
    nowpwork->CP = programCounter;
}

u32 scrGetScriptTimer(void) {
    return nowpwork->ScriptTimer;
}

ScrCommandYieldCount scrGetCommandTimer(void) {
    return nowpwork->CommandTimer;
}

ItfMesWindowIndex scrGetMessageHandle(void) {
    return nowpwork->MesHandle;
}

scrProcessWork_s *scrGetScriptHandle(void) {
    return nowpwork;
}

void *scrGetCurrentProcessUserData(void) {
    return nowpwork->data;
}
