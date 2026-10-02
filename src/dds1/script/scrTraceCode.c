#include "common.h"
#include "scr.h"

extern ScrVM *datGameState;
extern ScrData *scrCurrentContext;
extern const ScrCommand D_0039E288[];
extern u32 (*D_00329930[])(ScrData *scr);

void scrPushInteger(ScrData *scr, s32 val);
void bfStackPushFloat(ScrData *scr, f32 val);
void scrPushString(ScrData *scr, char *str);
void scrPushTypeFourValue(ScrData *scr, s32 val);
s32 bfStackPopInt(ScrData *scr);
f32 bfStackPopFloat(ScrData *scr);
void bfOpBinaryEval(ScrData *scr, s32 op);

/* Shared operand, result, and type slots for the binary bytecode operators. */
extern s32 D_003BD76C;
extern s32 D_003BD770;
extern s32 D_003BD774;
extern f32 D_003BD778;
extern f32 D_003BD77C;
extern f32 D_003BD780;
extern s32 D_003BD784;
extern s32 D_003BD788;

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
    f32 value;

    value = bfStackPopFloat(scr);
    datGameState->floats[scr->instructions[scr->pc].parts.sOperand] = value;
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
    s32 pc = scr->pc;
    s32 command = scr->instructions[pc].parts.sOperand;

    scrCurrentContext = scr;
    if (D_0039E288[command].func() == 0) {
        return 2;
    }
    scr->sp -= D_0039E288[command].paramCount;
    if (pc == scr->pc) {
        scr->pc = pc + 1;
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

void bfOpBinaryEval(ScrData *scr, s32 op)
{
    s32 secondType = scr->stackTypes[scr->sp - 1];
    s32 firstType = scr->stackTypes[scr->sp - 2];

    D_003BD788 = firstType;
    D_003BD784 = secondType;

    if ((secondType == 0 || secondType == 2) &&
        (firstType == 0 || firstType == 2)) {
        D_003BD76C = bfStackPopInt(scr);
        D_003BD770 = bfStackPopInt(scr);

        switch (op) {
        case 0:
            D_003BD774 = D_003BD76C + D_003BD770;
            break;
        case 1:
            D_003BD774 = D_003BD76C - D_003BD770;
            break;
        case 2:
            D_003BD774 = D_003BD76C * D_003BD770;
            break;
        case 3:
            D_003BD774 = D_003BD76C / D_003BD770;
            break;
        case 4:
            if (D_003BD76C != 0 || D_003BD770 != 0) {
                D_003BD774 = 1;
            } else {
                D_003BD774 = 0;
            }
            break;
        case 5:
            if (D_003BD76C != 0 && D_003BD770 != 0) {
                D_003BD774 = 1;
            } else {
                D_003BD774 = 0;
            }
            break;
        case 6:
            if (D_003BD76C == D_003BD770) {
                D_003BD774 = 1;
            } else {
                D_003BD774 = 0;
            }
            break;
        case 7:
            if (D_003BD76C != D_003BD770) {
                D_003BD774 = 1;
            } else {
                D_003BD774 = 0;
            }
            break;
        case 8:
            if (D_003BD76C < D_003BD770) {
                D_003BD774 = 1;
            } else {
                D_003BD774 = 0;
            }
            break;
        case 9:
            if (D_003BD76C > D_003BD770) {
                D_003BD774 = 1;
            } else {
                D_003BD774 = 0;
            }
            break;
        case 10:
            if (D_003BD76C <= D_003BD770) {
                D_003BD774 = 1;
            } else {
                D_003BD774 = 0;
            }
            break;
        case 11:
            if (D_003BD76C >= D_003BD770) {
                D_003BD774 = 1;
            } else {
                D_003BD774 = 0;
            }
            break;
        }
        scrPushInteger(scr, D_003BD774);
    } else {
        D_003BD778 = bfStackPopFloat(scr);
        D_003BD77C = bfStackPopFloat(scr);

        switch (op) {
        case 0:
            D_003BD780 = D_003BD778 + D_003BD77C;
            break;
        case 1:
            D_003BD780 = D_003BD778 - D_003BD77C;
            break;
        case 2:
            D_003BD780 = D_003BD778 * D_003BD77C;
            break;
        case 3:
            D_003BD780 = D_003BD778 / D_003BD77C;
            break;
        case 4:
            if (D_003BD778 != 0.0 || D_003BD77C != 0.0) {
                D_003BD774 = 1;
            } else {
                D_003BD774 = 0;
            }
            scrPushInteger(scr, D_003BD774);
            return;
        case 5:
            if (D_003BD778 != 0.0 && D_003BD77C != 0.0) {
                D_003BD774 = 1;
            } else {
                D_003BD774 = 0;
            }
            scrPushInteger(scr, D_003BD774);
            return;
        case 6:
            if (D_003BD778 == D_003BD77C) {
                D_003BD774 = 1;
            } else {
                D_003BD774 = 0;
            }
            scrPushInteger(scr, D_003BD774);
            return;
        case 7:
            if (D_003BD778 != D_003BD77C) {
                D_003BD774 = 1;
            } else {
                D_003BD774 = 0;
            }
            scrPushInteger(scr, D_003BD774);
            return;
        case 8:
            if (D_003BD778 < D_003BD77C) {
                D_003BD774 = 1;
            } else {
                D_003BD774 = 0;
            }
            scrPushInteger(scr, D_003BD774);
            return;
        case 9:
            if (D_003BD778 > D_003BD77C) {
                D_003BD774 = 1;
            } else {
                D_003BD774 = 0;
            }
            scrPushInteger(scr, D_003BD774);
            return;
        case 10:
            if (D_003BD778 <= D_003BD77C) {
                D_003BD774 = 1;
            } else {
                D_003BD774 = 0;
            }
            scrPushInteger(scr, D_003BD774);
            return;
        case 11:
            if (D_003BD778 >= D_003BD77C) {
                D_003BD774 = 1;
            } else {
                D_003BD774 = 0;
            }
            scrPushInteger(scr, D_003BD774);
            return;
        }
        bfStackPushFloat(scr, D_003BD780);
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
    s32 result;

    switch (scr->stackTypes[scr->sp - 1]) {
    case 0:
    case 2:
        result = bfStackPopInt(scr);
        break;
    case 1:
    case 3:
        result = (bfStackPopFloat(scr) != 0.0f);
        break;
    case 4:
        result = 0;
        break;
    default:
        result = 0;
        break;
    }
    if (result != 0)
        scr->pc++;
    else
        scr->pc = scr->labels[scr->instructions[scr->pc].parts.sOperand].addr;
    return 1;
}

/* Run instructions until one stops the context: returns 2 when a handler returns 0 (finished),
 * 1 when a handler returns 2 (yield for one tick). */
u32 bfContextStep(ScrData *scr)
{
    u32 result;

    while (1) {
        result = D_00329930[scr->instructions[scr->pc].parts.opCode](scr);
        if (result == 0) {
            scr->cmdTimer = 0;
            return 2;
        }
        if (result == 2) {
            break;
        }
        if (result == 1) {
            scr->cmdTimer = 0;
        }
    }
    scr->cmdTimer++;
    scr->timer++;
    return 1;
}

/* Script command parameter `idx` (0 = first) as an int, converting floats and
 * dereferencing global variable references. */
s32 scrReadIntParameter(s32 idx) {
    ScrData *scr = scrCurrentContext;
    s32 stackIndex = scr->sp - idx - 1;

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

f32 bfWaitReadArgFloat(s32 idx)
{
    ScrData *scr = scrCurrentContext;
    s32 stackIndex = scr->sp - idx - 1;

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
char *scrReadStringParameter(s32 paramIdx)
{
    ScrData *scr = scrCurrentContext;
    s32 stackIndex = scr->sp - paramIdx - 1;
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
