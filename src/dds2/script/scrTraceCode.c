#include "dat_state.h"
#include "common.h"
#include "scr.h"
#include "scr_string_parameter_api.h"
#include "scr_arg_read.h"


extern u32 (*scrOpcodeHandlers[])(ScrData *scr);
extern const ScrCommand D_00411408[];

u32 CodeFunc_PushIS(ScrData *scr) {
    PushInt(scr, scr->instructions[scr->pc].parts.sOperand);
    scr->pc++;
    return 1;
}

u32 CodeFunc_PushF(ScrData *scr) {
    scr->pc++;
    PushFloat(scr, scr->instructions[scr->pc].fOperand);
    scr->pc++;
    return 1;
}

u32 CodeFunc_PushIX(ScrData *scr) {
    PushInt(scr, datGameState->script.ints[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 CodeFunc_PushFX(ScrData *scr) {
    PushFloat(scr, datGameState->script.floats[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 CodeFunc_PushLIX(ScrData *scr) {
    PushInt(scr, scr->localInt[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 CodeFunc_PushLFX(ScrData *scr) {
    PushFloat(scr, scr->localFloat[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 CodeFunc_PushSTR(ScrData *scr) {
    PushStr(scr, scr->strings + scr->instructions[scr->pc].parts.sOperand);
    scr->pc++;
    return 1;
}

/* Copy the VM's reserved return slot back onto its active operand stack. */
u32 CodeFunc_PushReg(ScrData *scr) {
    scr->stackTypes[scr->sp] = scr->stackTypes[SCR_STACK_RET];
    scr->stackValues[scr->sp].i = scr->stackValues[SCR_STACK_RET].i;
    scr->sp++;
    scr->pc++;
    return 1;
}

u32 CodeFunc_PopIX(ScrData *scr) {
    datGameState->script.ints[scr->instructions[scr->pc].parts.sOperand] = PopInt(scr);
    scr->pc++;
    return 1;
}

u32 CodeFunc_PopFX(ScrData *scr) {
    f32 floatValue;

    floatValue = PopFloat(scr);
    datGameState->script.floats[scr->instructions[scr->pc].parts.sOperand] = floatValue;
    scr->pc++;
    return 1;
}

u32 CodeFunc_PopLIX(ScrData *scr) {
    u32 integerValue;

    integerValue = PopInt(scr);
    scr->localInt[scr->instructions[scr->pc].parts.sOperand] = integerValue;
    scr->pc++;
    return 1;
}

u32 CodeFunc_PopLFX(ScrData *scr) {
    scr->localFloat[scr->instructions[scr->pc].parts.sOperand] = PopFloat(scr);
    scr->pc++;
    return 1;
}

u32 CodeFunc_Proc(ScrData *scr) {
    scr->pc++;
    return 1;
}

/* Yield without consuming operands until the command completes. */
u32 CodeFunc_Comm(ScrData *scr) {
    s32 programCounter = scr->pc;
    s32 commandIndex = scr->instructions[programCounter].parts.sOperand;

    scrCurrentContext = scr;
    if (D_00411408[commandIndex].func() == 0) {
        return 2;
    }
    scr->sp -= D_00411408[commandIndex].paramCount;
    if (programCounter == scr->pc) {
        scr->pc = programCounter + 1;
    }
    return 1;
}

u32 CodeFunc_End(ScrData *scr)
{
    if (scr->sp == 0) {
        return 0;
    }
    scr->pc = PopInt(scr) + 1;
    return 1;
}

u32 CodeFunc_Jump(ScrData *scr) {
    scr->pc = scr->procedures[scr->instructions[scr->pc].parts.sOperand].addr;
    return 1;
}

/* Save the return PC before transferring control to a procedure. */
u32 CodeFunc_Call(ScrData *scr) {
    PushRet(scr, scr->pc);
    scr->pc = scr->procedures[scr->instructions[scr->pc].parts.sOperand].addr;
    return 1;
}

u32 CodeFunc_Run(ScrData *scr) {
    scr->pc++;
    return 1;
}

u32 CodeFunc_Goto(ScrData *scr) {
    scr->pc = scr->labels[scr->instructions[scr->pc].parts.sOperand].addr;
    return 1;
}

extern s32 scrBinaryIntegerLeftOperand;
extern s32 scrBinaryIntegerRightOperand;
extern s32 scrBinaryIntegerResult;
extern f32 scrBinaryFloatLeftOperand;
extern f32 scrBinaryFloatRightOperand;
extern f32 scrBinaryFloatResult;
extern s32 scrBinaryLeftOperandType;
extern s32 scrBinaryRightOperandType;

void CodeFunc_2Operation(ScrData *scr, s32 operatorCode)
{
    scrBinaryLeftOperandType = scr->stackTypes[scr->sp - 1];
    scrBinaryRightOperandType = scr->stackTypes[scr->sp - 2];
    if ((scrBinaryLeftOperandType == 0 || scrBinaryLeftOperandType == 2) && (scrBinaryRightOperandType == 0 || scrBinaryRightOperandType == 2)) {
        scrBinaryIntegerLeftOperand = PopInt(scr);
        scrBinaryIntegerRightOperand = PopInt(scr);
        switch (operatorCode) {
        case 0:
            scrBinaryIntegerResult = scrBinaryIntegerLeftOperand + scrBinaryIntegerRightOperand;
            break;
        case 1:
            scrBinaryIntegerResult = scrBinaryIntegerLeftOperand - scrBinaryIntegerRightOperand;
            break;
        case 2:
            scrBinaryIntegerResult = scrBinaryIntegerLeftOperand * scrBinaryIntegerRightOperand;
            break;
        case 3:
            scrBinaryIntegerResult = scrBinaryIntegerLeftOperand / scrBinaryIntegerRightOperand;
            break;
        case 4:
            if (scrBinaryIntegerLeftOperand != 0 || scrBinaryIntegerRightOperand != 0) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            break;
        case 5:
            if (scrBinaryIntegerLeftOperand != 0 && scrBinaryIntegerRightOperand != 0) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            break;
        case 6:
            if (scrBinaryIntegerLeftOperand == scrBinaryIntegerRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            break;
        case 7:
            if (scrBinaryIntegerLeftOperand != scrBinaryIntegerRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            break;
        case 8:
            if (scrBinaryIntegerLeftOperand < scrBinaryIntegerRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            break;
        case 9:
            if (scrBinaryIntegerLeftOperand > scrBinaryIntegerRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            break;
        case 10:
            if (scrBinaryIntegerLeftOperand <= scrBinaryIntegerRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            break;
        case 11:
            if (scrBinaryIntegerLeftOperand >= scrBinaryIntegerRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            break;
        }
        PushInt(scr, scrBinaryIntegerResult);
    } else {
        scrBinaryFloatLeftOperand = PopFloat(scr);
        scrBinaryFloatRightOperand = PopFloat(scr);
        switch (operatorCode) {
        case 0:
            scrBinaryFloatResult = scrBinaryFloatLeftOperand + scrBinaryFloatRightOperand;
            break;
        case 1:
            scrBinaryFloatResult = scrBinaryFloatLeftOperand - scrBinaryFloatRightOperand;
            break;
        case 2:
            scrBinaryFloatResult = scrBinaryFloatLeftOperand * scrBinaryFloatRightOperand;
            break;
        case 3:
            scrBinaryFloatResult = scrBinaryFloatLeftOperand / scrBinaryFloatRightOperand;
            break;
        case 4:
            if (scrBinaryFloatLeftOperand != 0.0 || scrBinaryFloatRightOperand != 0.0) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            PushInt(scr, scrBinaryIntegerResult);
            return;
        case 5:
            if (scrBinaryFloatLeftOperand != 0.0 && scrBinaryFloatRightOperand != 0.0) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            PushInt(scr, scrBinaryIntegerResult);
            return;
        case 6:
            if (scrBinaryFloatLeftOperand == scrBinaryFloatRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            PushInt(scr, scrBinaryIntegerResult);
            return;
        case 7:
            if (scrBinaryFloatLeftOperand != scrBinaryFloatRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            PushInt(scr, scrBinaryIntegerResult);
            return;
        case 8:
            if (scrBinaryFloatLeftOperand < scrBinaryFloatRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            PushInt(scr, scrBinaryIntegerResult);
            return;
        case 9:
            if (scrBinaryFloatLeftOperand > scrBinaryFloatRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            PushInt(scr, scrBinaryIntegerResult);
            return;
        case 10:
            if (scrBinaryFloatLeftOperand <= scrBinaryFloatRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            PushInt(scr, scrBinaryIntegerResult);
            return;
        case 11:
            if (scrBinaryFloatLeftOperand >= scrBinaryFloatRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            PushInt(scr, scrBinaryIntegerResult);
            return;
        }
        PushFloat(scr, scrBinaryFloatResult);
    }
}

u32 CodeFunc_Add(ScrData *scr) {
    CodeFunc_2Operation(scr, 0);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 CodeFunc_Sub(ScrData *scr) {
    CodeFunc_2Operation(scr, 1);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 CodeFunc_Mul(ScrData *scr) {
    CodeFunc_2Operation(scr, 2);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 CodeFunc_Div(ScrData *scr) {
    CodeFunc_2Operation(scr, 3);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 CodeFunc_Minus(ScrData *scr) {
    switch (scr->stackTypes[scr->sp - 1]) {
    case 0:
        scr->stackValues[scr->sp - 1].i = -scr->stackValues[scr->sp - 1].i;
        break;
    case 1:
        scr->stackValues[scr->sp - 1].f = -scr->stackValues[scr->sp - 1].f;
        break;
    case 2:
        datGameState->script.ints[scr->stackValues[scr->sp - 1].i] =
            -datGameState->script.ints[scr->stackValues[scr->sp - 1].i];
        break;
    case 3:
        datGameState->script.floats[scr->stackValues[scr->sp - 1].i] =
            -datGameState->script.floats[scr->stackValues[scr->sp - 1].i];
        break;
    case 4:
        break;
    }
    scr->pc++;
    return 1;
}

u32 CodeFunc_Not(ScrData *scr) {
    switch (scr->stackTypes[scr->sp - 1]) {
    case 0:
    case 2:
        PushInt(scr, PopInt(scr) == 0);
        break;
    case 1:
    case 3:
        PushInt(scr, PopFloat(scr) == 0.0f);
        break;
    case 4:
        break;
    }
    scr->pc++;
    return 1;
}

u32 CodeFunc_Or(ScrData *scr) {
    CodeFunc_2Operation(scr, 4);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 CodeFunc_And(ScrData *scr) {
    CodeFunc_2Operation(scr, 5);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 CodeFunc_EQ(ScrData *scr) {
    CodeFunc_2Operation(scr, 6);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 CodeFunc_NEQ(ScrData *scr) {
    CodeFunc_2Operation(scr, 7);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 CodeFunc_S(ScrData *scr) {
    CodeFunc_2Operation(scr, 8);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 CodeFunc_L(ScrData *scr) {
    CodeFunc_2Operation(scr, 9);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 CodeFunc_SE(ScrData *scr) {
    CodeFunc_2Operation(scr, 10);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 CodeFunc_LE(ScrData *scr) {
    CodeFunc_2Operation(scr, 0xb);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 CodeFunc_IF(ScrData *scr)
{
    s32 conditionIsTruthy;

    switch (scr->stackTypes[scr->sp - 1]) {
    case 0:
    case 2:
        conditionIsTruthy = PopInt(scr);
        break;
    case 1:
    case 3:
        conditionIsTruthy = (PopFloat(scr) != 0.0f);
        break;
    case 4:
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
    return 1;
}

/* Run instructions until one stops the context: returns 2 when a handler returns 0 (finished),
 * 1 when a handler returns 2 (yield for one tick). */
s32 scrTraceCode(ScrData *scr) {
    u32 handlerResult;

    while (1) {
        handlerResult = scrOpcodeHandlers[scr->instructions[scr->pc].parts.opCode](scr);
        if (handlerResult == 0) {
            scr->cmdTimer = 0;
            return 2;
        }
        if (handlerResult == 2) {
            break;
        }
        if (handlerResult == 1) {
            scr->cmdTimer = 0;
        }
    }
    scr->cmdTimer++;
    scr->timer++;
    return 1;
}

/* Script command parameter `parameterIndex` (0 = first) as an int, converting floats and
 * dereferencing global variable references. */
s32 scrGetIntPara(s32 parameterIndex) {
    ScrData *scr = scrCurrentContext;
    s32 stackIndex = scr->sp - parameterIndex - 1;

    switch (scr->stackTypes[stackIndex]) {
    case 0:
    case 4:
        return scr->stackValues[stackIndex].i;
    case 1:
        return scr->stackValues[stackIndex].f;
    case 2:
        return datGameState->script.ints[scr->stackValues[stackIndex].i];
    case 3:
        return datGameState->script.floats[scr->stackValues[stackIndex].i];
    }
    return 0;
}

f32 scrGetFloatPara(s32 parameterIndex)
{
    ScrData *scr = scrCurrentContext;
    s32 stackIndex = scr->sp - parameterIndex - 1;

    switch (scr->stackTypes[stackIndex]) {
    case 0:
    case 4:
        return (f32)scr->stackValues[stackIndex].i;
    case 1:
        return scr->stackValues[stackIndex].f;
    case 2:
        return (f32)datGameState->script.ints[scr->stackValues[stackIndex].i];
    case 3:
        return datGameState->script.floats[scr->stackValues[stackIndex].i];
    }
    return 0.0f;
}

/* Return a string parameter only when its VM stack tag is STRING. */
char *scrGetStrPara(s32 parameterIndex)
{
    ScrData *scr = scrCurrentContext;
    s32 stackIndex = scr->sp - parameterIndex - 1;
    s32 stackType = (s8)scr->stackTypes[stackIndex];

    if (stackType >= 0) {
        if (stackType >= SCR_STACK_TYPE_STRING) {
            if (stackType == SCR_STACK_TYPE_STRING) {
                return scr->stackValues[stackIndex].s;
            }
        }
    }
    return NULL;
}

/* A command's return value: its type tag, then its value, in the running
 * context's return slot. The tag goes through the context as read on entry,
 * the value through scrCurrentContext read again, since the byte store may
 * have changed it as far as C knows; retail reads it twice. */
#define SCR_SET_RETURN_VALUE(scr, type, member, value)                   \
    do {                                                                 \
        (scr)->stackTypes[SCR_STACK_RET] = (type);                       \
        scrCurrentContext->stackValues[SCR_STACK_RET].member = (value);  \
    } while (0)

/* The setters return the context they set the value in, which retail
 * leaves in v0: a command that ends by setting its value returns it as its
 * result, and a nonzero result tells CodeFunc_Comm the command is done. */
ScrData *scrSetIntReturnValue(s32 retVal)
{
    ScrData *scr = scrCurrentContext;
    SCR_SET_RETURN_VALUE(scr, 0, i, retVal);
    return scr;
}

ScrData *scrSetFloatReturnValue(f32 retVal)
{
    ScrData *scr = scrCurrentContext;
    SCR_SET_RETURN_VALUE(scr, 1, f, retVal);
    return scr;
}
