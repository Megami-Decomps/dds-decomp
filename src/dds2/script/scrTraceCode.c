#include "common.h"
#include "scr.h"

extern u32 bfStackPopInt();
void scrPushInteger(ScrData *scr, s32 val);
void bfStackPushFloat(ScrData *scr, f32 val);
f32 bfStackPopFloat();

extern ScrData *scrCurrentContext;

extern ScrVM *datGameState;
extern u32 (*D_00384948[])(ScrData *scr);
extern const ScrCommand D_00411408[];

/* Shared operand, result, and type slots for the binary bytecode operators. */
extern s32 D_00438E6C;
extern s32 D_00438E70;
extern s32 D_00438E74;
extern f32 D_00438E78;
extern f32 D_00438E7C;
extern f32 D_00438E80;
extern s32 D_00438E84;
extern s32 D_00438E88;

u32 scrPushImmediateInteger(ScrData *scr) {
    scrPushInteger(scr, scr->instructions[scr->pc].parts.sOperand);
    scr->pc++;
    return 1;
}

u32 scrPushImmediateFloat(ScrData *scr) {
    scr->pc++;
    bfStackPushFloat(scr, scr->instructions[scr->pc].fOperand);
    scr->pc++;
    return 1;
}

u32 scrPushGlobalInteger(ScrData *scr) {
    scrPushInteger(scr, datGameState->ints[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 scrPushGlobalFloat(ScrData *scr) {
    bfStackPushFloat(scr, datGameState->floats[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 scrPushLocalInteger(ScrData *scr) {
    scrPushInteger(scr, scr->localInt[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 scrPushLocalFloat(ScrData *scr) {
    bfStackPushFloat(scr, scr->localFloat[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 scrPushStringLiteral(ScrData *scr) {
    scrPushString(scr, scr->strings + scr->instructions[scr->pc].parts.sOperand);
    scr->pc++;
    return 1;
}

/* Copy the VM's reserved return slot back onto its active operand stack. */
u32 scrPushReturnValue(ScrData *scr) {
    scr->stackTypes[scr->sp] = scr->stackTypes[SCR_STACK_RET];
    scr->stackValues[scr->sp].i = scr->stackValues[SCR_STACK_RET].i;
    scr->sp++;
    scr->pc++;
    return 1;
}

u32 scrStoreGlobalInteger(ScrData *scr) {
    datGameState->ints[scr->instructions[scr->pc].parts.sOperand] = bfStackPopInt();
    scr->pc++;
    return 1;
}

u32 scrStoreGlobalFloat(ScrData *scr) {
    f32 value;

    value = bfStackPopFloat();
    datGameState->floats[scr->instructions[scr->pc].parts.sOperand] = value;
    scr->pc++;
    return 1;
}

u32 scrStoreLocalInteger(ScrData *scr) {
    u32 value;

    value = bfStackPopInt();
    scr->localInt[scr->instructions[scr->pc].parts.sOperand] = value;
    scr->pc++;
    return 1;
}

u32 scrStoreLocalFloat(ScrData *scr) {
    scr->localFloat[scr->instructions[scr->pc].parts.sOperand] = bfStackPopFloat();
    scr->pc++;
    return 1;
}

u32 scrAdvanceTraceProgramCounter(ScrData *scr) {
    scr->pc++;
    return 1;
}

/* Yield without consuming operands until the command completes. */
u32 bfOpWaitDispatch(ScrData *scr) {
    s32 pc = scr->pc;
    s32 command = scr->instructions[pc].parts.sOperand;

    scrCurrentContext = scr;
    if (D_00411408[command].func() == 0) {
        return 2;
    }
    scr->sp -= D_00411408[command].paramCount;
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

u32 scrJumpProcedure(ScrData *scr) {
    scr->pc = scr->procedures[scr->instructions[scr->pc].parts.sOperand].addr;
    return 1;
}

/* Save the return PC before transferring control to a procedure. */
u32 scrCallProcedure(u32 scriptAddress) {
    s32 address;
    ScrData *scr;

    address = (s32)scriptAddress;
    scr = (ScrData *)address;
    scrPushTypeFourValue(scriptAddress, scr->pc);
    scr->pc = scr->procedures[scr->instructions[scr->pc].parts.sOperand].addr;
    return 1;
}

u32 scrTraceAdvanceProgramCounter(ScrData *scr) {
    scr->pc++;
    return 1;
}

u32 scrJumpLabel(ScrData *scr) {
    scr->pc = scr->labels[scr->instructions[scr->pc].parts.sOperand].addr;
    return 1;
}

void bfOpBinaryEval(ScrData *scr, s32 op) {
    s32 secondType = scr->stackTypes[scr->sp - 1];
    s32 firstType = scr->stackTypes[scr->sp - 2];

    D_00438E88 = firstType;
    D_00438E84 = secondType;

    if ((secondType == 0 || secondType == 2) &&
        (firstType == 0 || firstType == 2)) {
        D_00438E6C = bfStackPopInt(scr);
        D_00438E70 = bfStackPopInt(scr);

        switch (op) {
        case 0:
            D_00438E74 = D_00438E6C + D_00438E70;
            break;
        case 1:
            D_00438E74 = D_00438E6C - D_00438E70;
            break;
        case 2:
            D_00438E74 = D_00438E6C * D_00438E70;
            break;
        case 3:
            D_00438E74 = D_00438E6C / D_00438E70;
            break;
        case 4:
            if (D_00438E6C != 0 || D_00438E70 != 0) {
                D_00438E74 = 1;
            } else {
                D_00438E74 = 0;
            }
            break;
        case 5:
            if (D_00438E6C != 0 && D_00438E70 != 0) {
                D_00438E74 = 1;
            } else {
                D_00438E74 = 0;
            }
            break;
        case 6:
            if (D_00438E6C == D_00438E70) {
                D_00438E74 = 1;
            } else {
                D_00438E74 = 0;
            }
            break;
        case 7:
            if (D_00438E6C != D_00438E70) {
                D_00438E74 = 1;
            } else {
                D_00438E74 = 0;
            }
            break;
        case 8:
            if (D_00438E6C < D_00438E70) {
                D_00438E74 = 1;
            } else {
                D_00438E74 = 0;
            }
            break;
        case 9:
            if (D_00438E6C > D_00438E70) {
                D_00438E74 = 1;
            } else {
                D_00438E74 = 0;
            }
            break;
        case 10:
            if (D_00438E6C <= D_00438E70) {
                D_00438E74 = 1;
            } else {
                D_00438E74 = 0;
            }
            break;
        case 11:
            if (D_00438E6C >= D_00438E70) {
                D_00438E74 = 1;
            } else {
                D_00438E74 = 0;
            }
            break;
        }
        scrPushInteger(scr, D_00438E74);
    } else {
        D_00438E78 = bfStackPopFloat(scr);
        D_00438E7C = bfStackPopFloat(scr);

        switch (op) {
        case 0:
            D_00438E80 = D_00438E78 + D_00438E7C;
            break;
        case 1:
            D_00438E80 = D_00438E78 - D_00438E7C;
            break;
        case 2:
            D_00438E80 = D_00438E78 * D_00438E7C;
            break;
        case 3:
            D_00438E80 = D_00438E78 / D_00438E7C;
            break;
        case 4:
            if (D_00438E78 != 0.0 || D_00438E7C != 0.0) {
                D_00438E74 = 1;
            } else {
                D_00438E74 = 0;
            }
            scrPushInteger(scr, D_00438E74);
            return;
        case 5:
            if (D_00438E78 != 0.0 && D_00438E7C != 0.0) {
                D_00438E74 = 1;
            } else {
                D_00438E74 = 0;
            }
            scrPushInteger(scr, D_00438E74);
            return;
        case 6:
            if (D_00438E78 == D_00438E7C) {
                D_00438E74 = 1;
            } else {
                D_00438E74 = 0;
            }
            scrPushInteger(scr, D_00438E74);
            return;
        case 7:
            if (D_00438E78 != D_00438E7C) {
                D_00438E74 = 1;
            } else {
                D_00438E74 = 0;
            }
            scrPushInteger(scr, D_00438E74);
            return;
        case 8:
            if (D_00438E78 < D_00438E7C) {
                D_00438E74 = 1;
            } else {
                D_00438E74 = 0;
            }
            scrPushInteger(scr, D_00438E74);
            return;
        case 9:
            if (D_00438E78 > D_00438E7C) {
                D_00438E74 = 1;
            } else {
                D_00438E74 = 0;
            }
            scrPushInteger(scr, D_00438E74);
            return;
        case 10:
            if (D_00438E78 <= D_00438E7C) {
                D_00438E74 = 1;
            } else {
                D_00438E74 = 0;
            }
            scrPushInteger(scr, D_00438E74);
            return;
        case 11:
            if (D_00438E78 >= D_00438E7C) {
                D_00438E74 = 1;
            } else {
                D_00438E74 = 0;
            }
            scrPushInteger(scr, D_00438E74);
            return;
        }
        bfStackPushFloat(scr, D_00438E80);
    }
}

u32 scrOpAdd(ScrData *scr) {
    bfOpBinaryEval(scr, 0);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 scrOpSubtract(ScrData *scr) {
    bfOpBinaryEval(scr, 1);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 scrOpMultiply(ScrData *scr) {
    bfOpBinaryEval(scr, 2);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 scrOpDivide(ScrData *scr) {
    bfOpBinaryEval(scr, 3);
    scr->pc = scr->pc + 1;
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

u32 scrOpLogicalOr(ScrData *scr) {
    bfOpBinaryEval(scr, 4);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 scrOpLogicalAnd(ScrData *scr) {
    bfOpBinaryEval(scr, 5);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 scrOpCompareEqual(ScrData *scr) {
    bfOpBinaryEval(scr, 6);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 scrOpCompareNotEqual(ScrData *scr) {
    bfOpBinaryEval(scr, 7);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 scrOpCompareLess(ScrData *scr) {
    bfOpBinaryEval(scr, 8);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 scrOpCompareGreater(ScrData *scr) {
    bfOpBinaryEval(scr, 9);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 scrOpCompareLessEqual(ScrData *scr) {
    bfOpBinaryEval(scr, 10);
    scr->pc = scr->pc + 1;
    return 1;
}

u32 scrOpCompareGreaterEqual(ScrData *scr) {
    bfOpBinaryEval(scr, 0xb);
    scr->pc = scr->pc + 1;
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
u32 bfContextStep(ScrData *scr) {
    u32 result;

    while (1) {
        result = D_00384948[scr->instructions[scr->pc].parts.opCode](scr);
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
