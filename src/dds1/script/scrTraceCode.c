#include "common.h"
#include "scr_integer_return_value_api.h"
#include "scr.h"
#include "scr_string_parameter_api.h"
#include "scr_arg_read.h"

extern ScrVM *datGameState;
extern const ScrCommand D_0039E288[];
extern u32 (*scrOpcodeHandlers[])(ScrData *scr);

void bfOpBinaryEval(ScrData *scr, s32 operatorCode);

u32 scrPushImmediateInteger(ScrData *scr)
{
    scrPushInteger(scr, scr->instructions[scr->pc].parts.sOperand);
    scr->pc++;
    return 1;
}

u32 scrPushImmediateFloat(ScrData *scr)
{
    scr->pc++;
    bfStackPushFloat(scr, scr->instructions[scr->pc].fOperand);
    scr->pc++;
    return 1;
}

u32 scrPushGlobalInteger(ScrData *scr)
{
    scrPushInteger(scr, datGameState->ints[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 scrPushGlobalFloat(ScrData *scr)
{
    bfStackPushFloat(scr, datGameState->floats[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 scrPushLocalInteger(ScrData *scr)
{
    scrPushInteger(scr, scr->localInt[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 scrPushLocalFloat(ScrData *scr)
{
    bfStackPushFloat(scr, scr->localFloat[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 scrPushStringLiteral(ScrData *scr)
{
    scrPushString(scr, scr->strings + scr->instructions[scr->pc].parts.sOperand);
    scr->pc++;
    return 1;
}

/* Copy the VM's reserved return slot back onto its active operand stack. */
u32 scrPushReturnValue(ScrData *scr)
{
    scr->stackTypes[scr->sp] = scr->stackTypes[SCR_STACK_RET];
    scr->stackValues[scr->sp].i = scr->stackValues[SCR_STACK_RET].i;
    scr->sp++;
    scr->pc++;
    return 1;
}

u32 scrStoreGlobalInteger(ScrData *scr)
{
    datGameState->ints[scr->instructions[scr->pc].parts.sOperand] = bfStackPopInt(scr);
    scr->pc++;
    return 1;
}

u32 scrStoreGlobalFloat(ScrData *scr)
{
    f32 floatValue;

    floatValue = bfStackPopFloat(scr);
    datGameState->floats[scr->instructions[scr->pc].parts.sOperand] = floatValue;
    scr->pc++;
    return 1;
}

u32 scrStoreLocalInteger(ScrData *scr)
{
    scr->localInt[scr->instructions[scr->pc].parts.sOperand] = bfStackPopInt(scr);
    scr->pc++;
    return 1;
}

u32 scrStoreLocalFloat(ScrData *scr)
{
    scr->localFloat[scr->instructions[scr->pc].parts.sOperand] = bfStackPopFloat(scr);
    scr->pc++;
    return 1;
}

u32 scrAdvanceTraceProgramCounter(ScrData *scr)
{
    scr->pc++;
    return 1;
}

/* Yield without consuming operands until the command completes. */
u32 bfOpWaitDispatch(ScrData *scr)
{
    s32 commandPc = scr->pc;
    s32 commandIndex = scr->instructions[commandPc].parts.sOperand;

    scrCurrentContext = scr;
    if (D_0039E288[commandIndex].func() == 0) {
        return 2;
    }
    scr->sp -= D_0039E288[commandIndex].paramCount;
    if (commandPc == scr->pc) {
        scr->pc = commandPc + 1;
    }
    return 1;
}

u32 scrReturnToStackAddress(ScrData *scr)
{
    if (scr->sp == 0) {
        return 0;
    }
    scr->pc = bfStackPopInt(scr) + 1;
    return 1;
}

u32 scrJumpProcedure(ScrData *scr)
{
    scr->pc = scr->procedures[scr->instructions[scr->pc].parts.sOperand].addr;
    return 1;
}

/* Save the return PC before transferring control to a procedure. */
u32 scrCallProcedure(ScrData *scr)
{
    scrPushTypeFourValue(scr, scr->pc);
    scr->pc = scr->procedures[scr->instructions[scr->pc].parts.sOperand].addr;
    return 1;
}

u32 scrTraceAdvanceProgramCounter(ScrData *scr)
{
    scr->pc++;
    return 1;
}

u32 scrJumpLabel(ScrData *scr)
{
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

void bfOpBinaryEval(ScrData *scr, s32 operatorCode)
{
    scrBinaryLeftOperandType = scr->stackTypes[scr->sp - 1];
    scrBinaryRightOperandType = scr->stackTypes[scr->sp - 2];
    if ((scrBinaryLeftOperandType == 0 || scrBinaryLeftOperandType == 2) && (scrBinaryRightOperandType == 0 || scrBinaryRightOperandType == 2)) {
        scrBinaryIntegerLeftOperand = bfStackPopInt(scr);
        scrBinaryIntegerRightOperand = bfStackPopInt(scr);
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
        scrPushInteger(scr, scrBinaryIntegerResult);
    } else {
        scrBinaryFloatLeftOperand = bfStackPopFloat(scr);
        scrBinaryFloatRightOperand = bfStackPopFloat(scr);
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
            scrPushInteger(scr, scrBinaryIntegerResult);
            return;
        case 5:
            if (scrBinaryFloatLeftOperand != 0.0 && scrBinaryFloatRightOperand != 0.0) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            scrPushInteger(scr, scrBinaryIntegerResult);
            return;
        case 6:
            if (scrBinaryFloatLeftOperand == scrBinaryFloatRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            scrPushInteger(scr, scrBinaryIntegerResult);
            return;
        case 7:
            if (scrBinaryFloatLeftOperand != scrBinaryFloatRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            scrPushInteger(scr, scrBinaryIntegerResult);
            return;
        case 8:
            if (scrBinaryFloatLeftOperand < scrBinaryFloatRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            scrPushInteger(scr, scrBinaryIntegerResult);
            return;
        case 9:
            if (scrBinaryFloatLeftOperand > scrBinaryFloatRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            scrPushInteger(scr, scrBinaryIntegerResult);
            return;
        case 10:
            if (scrBinaryFloatLeftOperand <= scrBinaryFloatRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            scrPushInteger(scr, scrBinaryIntegerResult);
            return;
        case 11:
            if (scrBinaryFloatLeftOperand >= scrBinaryFloatRightOperand) {
                scrBinaryIntegerResult = 1;
            } else {
                scrBinaryIntegerResult = 0;
            }
            scrPushInteger(scr, scrBinaryIntegerResult);
            return;
        }
        bfStackPushFloat(scr, scrBinaryFloatResult);
    }
}

u32 scrOpAdd(ScrData *scr)
{
    bfOpBinaryEval(scr, 0);
    scr->pc++;
    return 1;
}

u32 scrOpSubtract(ScrData *scr)
{
    bfOpBinaryEval(scr, 1);
    scr->pc++;
    return 1;
}

u32 scrOpMultiply(ScrData *scr)
{
    bfOpBinaryEval(scr, 2);
    scr->pc++;
    return 1;
}

u32 scrOpDivide(ScrData *scr)
{
    bfOpBinaryEval(scr, 3);
    scr->pc++;
    return 1;
}

u32 bfOpNegate(ScrData *scr) {
    switch (scr->stackTypes[scr->sp - 1]) {
    case 0:
        scr->stackValues[scr->sp - 1].i = -scr->stackValues[scr->sp - 1].i;
        break;
    case 1:
        scr->stackValues[scr->sp - 1].f = -scr->stackValues[scr->sp - 1].f;
        break;
    case 2:
        datGameState->ints[scr->stackValues[scr->sp - 1].i] =
            -datGameState->ints[scr->stackValues[scr->sp - 1].i];
        break;
    case 3:
        datGameState->floats[scr->stackValues[scr->sp - 1].i] =
            -datGameState->floats[scr->stackValues[scr->sp - 1].i];
        break;
    case 4:
        break;
    }
    scr->pc++;
    return 1;
}

u32 bfOpNot(ScrData *scr) {
    switch (scr->stackTypes[scr->sp - 1]) {
    case 0:
    case 2:
        scrPushInteger(scr, bfStackPopInt(scr) == 0);
        break;
    case 1:
    case 3:
        scrPushInteger(scr, bfStackPopFloat(scr) == 0.0f);
        break;
    case 4:
        break;
    }
    scr->pc++;
    return 1;
}

u32 scrOpLogicalOr(ScrData *scr)
{
    bfOpBinaryEval(scr, 4);
    scr->pc++;
    return 1;
}

u32 scrOpLogicalAnd(ScrData *scr)
{
    bfOpBinaryEval(scr, 5);
    scr->pc++;
    return 1;
}

u32 scrOpCompareEqual(ScrData *scr)
{
    bfOpBinaryEval(scr, 6);
    scr->pc++;
    return 1;
}

u32 scrOpCompareNotEqual(ScrData *scr)
{
    bfOpBinaryEval(scr, 7);
    scr->pc++;
    return 1;
}

u32 scrOpCompareLess(ScrData *scr)
{
    bfOpBinaryEval(scr, 8);
    scr->pc++;
    return 1;
}

u32 scrOpCompareGreater(ScrData *scr)
{
    bfOpBinaryEval(scr, 9);
    scr->pc++;
    return 1;
}

u32 scrOpCompareLessEqual(ScrData *scr)
{
    bfOpBinaryEval(scr, 10);
    scr->pc++;
    return 1;
}

u32 scrOpCompareGreaterEqual(ScrData *scr)
{
    bfOpBinaryEval(scr, 11);
    scr->pc++;
    return 1;
}

u32 bfOpJumpIfFalse(ScrData *scr)
{
    s32 conditionIsTruthy;

    switch (scr->stackTypes[scr->sp - 1]) {
    case 0:
    case 2:
        conditionIsTruthy = bfStackPopInt(scr);
        break;
    case 1:
    case 3:
        conditionIsTruthy = (bfStackPopFloat(scr) != 0.0f);
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
s32 bfContextStep(ScrData *scr)
{
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
s32 scrReadIntParameter(s32 parameterIndex) {
    ScrData *scr = scrCurrentContext;
    s32 stackIndex = scr->sp - parameterIndex - 1;

    switch (scr->stackTypes[stackIndex]) {
    case 0:
    case 4:
        return scr->stackValues[stackIndex].i;
    case 1:
        return scr->stackValues[stackIndex].f;
    case 2:
        return datGameState->ints[scr->stackValues[stackIndex].i];
    case 3:
        return datGameState->floats[scr->stackValues[stackIndex].i];
    }
    return 0;
}

f32 bfWaitReadArgFloat(s32 parameterIndex)
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
        return (f32)datGameState->ints[scr->stackValues[stackIndex].i];
    case 3:
        return datGameState->floats[scr->stackValues[stackIndex].i];
    }
    return 0.0f;
}

/* Return a string parameter only when its VM stack tag is STRING. */
char *scrReadStringParameter(s32 parameterIndex)
{
    ScrData *scr = scrCurrentContext;
    s32 stackIndex = scr->sp - parameterIndex - 1;
    s32 type = (s8)scr->stackTypes[stackIndex];

    if (type >= 0) {
        if (type >= SCR_STACK_TYPE_STRING) {
            if (type == SCR_STACK_TYPE_STRING) {
                return scr->stackValues[stackIndex].s;
            }
        }
    }
    return NULL;
}

void scrSetIntegerReturnValue(s32 retVal)
{
    scrCurrentContext->stackTypes[SCR_STACK_RET] = 0;
    scrCurrentContext->stackValues[SCR_STACK_RET].i = retVal;
}

void scrSetFloatReturnValue(f32 retVal)
{
    scrCurrentContext->stackTypes[SCR_STACK_RET] = 1;
    scrCurrentContext->stackValues[SCR_STACK_RET].f = retVal;
}
