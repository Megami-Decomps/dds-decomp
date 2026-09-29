#include "common.h"
#include "scr.h"

#define SCR_STACK_RET 27
#define SCR_STACK_TYPE_STRING 5

extern ScrVM *D_003BAA00;
extern ScrData *D_003BD78C;
extern ScrCommand D_0039E288[];
extern u32 (*D_00329930[])(ScrData *scr);

void scrPushInteger(ScrData *scr, s32 val);
void bfStackPushFloat(ScrData *scr, f32 val);
void scrPushString(ScrData *scr, char *str);
void scrPushTypeFourValue(ScrData *scr, s32 val);
s32 bfStackPopInt(ScrData *scr);
f32 bfStackPopFloat(ScrData *scr);
u32 bfOpBinaryEval(ScrData *scr, s32 op);

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
    scrPushInteger(scr, D_003BAA00->ints[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 scrPushGlobalFloat(ScrData *scr)
{
    bfStackPushFloat(scr, D_003BAA00->floats[scr->instructions[scr->pc].parts.sOperand]);
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
    D_003BAA00->ints[scr->instructions[scr->pc].parts.sOperand] = bfStackPopInt(scr);
    scr->pc++;
    return 1;
}

u32 scrStoreGlobalFloat(ScrData *scr)
{
    f32 value;

    value = bfStackPopFloat(scr);
    D_003BAA00->floats[scr->instructions[scr->pc].parts.sOperand] = value;
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

u32 func_0010C7E0(ScrData *scr)
{
    scr->pc++;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", bfOpWaitDispatch);

u32 func_0010C898(ScrData *scr)
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

u32 func_0010C960(ScrData *scr)
{
    scr->pc++;
    return 1;
}

u32 scrJumpLabel(ScrData *scr)
{
    scr->pc = scr->labels[scr->instructions[scr->pc].parts.sOperand].addr;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", bfOpBinaryEval);

u32 func_0010CE90(ScrData *scr)
{
    bfOpBinaryEval(scr, 0);
    scr->pc++;
    return 1;
}

u32 func_0010CEC8(ScrData *scr)
{
    bfOpBinaryEval(scr, 1);
    scr->pc++;
    return 1;
}

u32 func_0010CF00(ScrData *scr)
{
    bfOpBinaryEval(scr, 2);
    scr->pc++;
    return 1;
}

u32 func_0010CF38(ScrData *scr)
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
        D_003BAA00->ints[scr->stackValues[scr->sp - 1].i] =
            -D_003BAA00->ints[scr->stackValues[scr->sp - 1].i];
        break;
    case 3:
        D_003BAA00->floats[scr->stackValues[scr->sp - 1].i] =
            -D_003BAA00->floats[scr->stackValues[scr->sp - 1].i];
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

u32 func_0010D100(ScrData *scr)
{
    bfOpBinaryEval(scr, 4);
    scr->pc++;
    return 1;
}

u32 func_0010D138(ScrData *scr)
{
    bfOpBinaryEval(scr, 5);
    scr->pc++;
    return 1;
}

u32 func_0010D170(ScrData *scr)
{
    bfOpBinaryEval(scr, 6);
    scr->pc++;
    return 1;
}

u32 func_0010D1A8(ScrData *scr)
{
    bfOpBinaryEval(scr, 7);
    scr->pc++;
    return 1;
}

u32 func_0010D1E0(ScrData *scr)
{
    bfOpBinaryEval(scr, 8);
    scr->pc++;
    return 1;
}

u32 func_0010D218(ScrData *scr)
{
    bfOpBinaryEval(scr, 9);
    scr->pc++;
    return 1;
}

u32 func_0010D250(ScrData *scr)
{
    bfOpBinaryEval(scr, 10);
    scr->pc++;
    return 1;
}

u32 func_0010D288(ScrData *scr)
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

INCLUDE_ASM(const s32, "script/scrTraceCode", bfContextStep);

/* Script command parameter `idx` (0 = first) as an int, converting floats and
 * dereferencing global variable references. */
s32 scrReadIntParameter(s32 idx) {
    ScrData *scr = D_003BD78C;
    s32 stackIndex = scr->sp - idx - 1;

    switch (scr->stackTypes[stackIndex]) {
    case 0:
    case 4:
        return scr->stackValues[stackIndex].i;
    case 1:
        return scr->stackValues[stackIndex].f;
    case 2:
        return D_003BAA00->ints[scr->stackValues[stackIndex].i];
    case 3:
        return D_003BAA00->floats[scr->stackValues[stackIndex].i];
    }
    return 0;
}

f32 bfWaitReadArgFloat(s32 idx)
{
    ScrData *scr = D_003BD78C;
    s32 stackIndex = scr->sp - idx - 1;

    switch (scr->stackTypes[stackIndex]) {
    case 0:
    case 4:
        return (f32)scr->stackValues[stackIndex].i;
    case 1:
        return scr->stackValues[stackIndex].f;
    case 2:
        return (f32)D_003BAA00->ints[scr->stackValues[stackIndex].i];
    case 3:
        return D_003BAA00->floats[scr->stackValues[stackIndex].i];
    }
    return 0.0f;
}

/* Return a string parameter only when its VM stack tag is STRING. */
char *scrReadStringParameter(s32 paramIdx)
{
    ScrData *scr = D_003BD78C;
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

void func_0010D5F0(s32 retVal)
{
    D_003BD78C->stackTypes[SCR_STACK_RET] = 0;
    D_003BD78C->stackValues[SCR_STACK_RET].i = retVal;
}

void func_0010D608(f32 retVal)
{
    D_003BD78C->stackTypes[SCR_STACK_RET] = 1;
    D_003BD78C->stackValues[SCR_STACK_RET].f = retVal;
}
