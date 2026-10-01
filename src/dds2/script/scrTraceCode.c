#include "common.h"
#include "scr.h"

extern u32 bfStackPopInt();
void scrPushInteger(ScrData *scr, s32 val);
void bfStackPushFloat(ScrData *scr, f32 val);
f32 bfStackPopFloat();

#define SCR_STACK_TYPE_STRING 5

extern ScrData *D_00438E8C;

#define SCR_STACK_RET 27

extern ScrVM *D_00435DD0;
extern u32 (*D_00384948[])(ScrData *scr);

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
    scrPushInteger(scr, D_00435DD0->ints[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 scrPushGlobalFloat(ScrData *scr) {
    bfStackPushFloat(scr, D_00435DD0->floats[scr->instructions[scr->pc].parts.sOperand]);
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
    D_00435DD0->ints[scr->instructions[scr->pc].parts.sOperand] = bfStackPopInt();
    scr->pc++;
    return 1;
}

u32 scrStoreGlobalFloat(ScrData *scr) {
    f32 value;

    value = bfStackPopFloat();
    D_00435DD0->floats[scr->instructions[scr->pc].parts.sOperand] = value;
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

INCLUDE_ASM(const s32, "script/scrTraceCode", bfOpWaitDispatch);

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

u32 func_0010CB88(ScrData *scr) {
    scr->pc++;
    return 1;
}

u32 scrJumpLabel(ScrData *scr) {
    scr->pc = scr->labels[scr->instructions[scr->pc].parts.sOperand].addr;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", bfOpBinaryEval);

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
        D_00435DD0->ints[scr->stackValues[scr->sp - 1].i] =
            -D_00435DD0->ints[scr->stackValues[scr->sp - 1].i];
        break;
    case 3:
        D_00435DD0->floats[scr->stackValues[scr->sp - 1].i] =
            -D_00435DD0->floats[scr->stackValues[scr->sp - 1].i];
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
    ScrData *scr = D_00438E8C;
    s32 stackIndex = scr->sp - idx - 1;

    switch (scr->stackTypes[stackIndex]) {
    case 0:
    case 4:
        return scr->stackValues[stackIndex].i;
    case 1:
        return scr->stackValues[stackIndex].f;
    case 2:
        return D_00435DD0->ints[scr->stackValues[stackIndex].i];
    case 3:
        return D_00435DD0->floats[scr->stackValues[stackIndex].i];
    }
    return 0;
}

f32 bfWaitReadArgFloat(s32 idx)
{
    ScrData *scr = D_00438E8C;
    s32 stackIndex = scr->sp - idx - 1;

    switch (scr->stackTypes[stackIndex]) {
    case 0:
    case 4:
        return (f32)scr->stackValues[stackIndex].i;
    case 1:
        return scr->stackValues[stackIndex].f;
    case 2:
        return (f32)D_00435DD0->ints[scr->stackValues[stackIndex].i];
    case 3:
        return D_00435DD0->floats[scr->stackValues[stackIndex].i];
    }
    return 0.0f;
}

/* Return a string parameter only when its VM stack tag is STRING. */
char *scrReadStringParameter(s32 paramIdx)
{
    ScrData *scr = D_00438E8C;
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
    D_00438E8C->stackTypes[SCR_STACK_RET] = 0;
    D_00438E8C->stackValues[SCR_STACK_RET].i = retVal;
}

void scrSetFloatReturnValue(f32 retVal)
{
    D_00438E8C->stackTypes[SCR_STACK_RET] = 1;
    D_00438E8C->stackValues[SCR_STACK_RET].f = retVal;
}
