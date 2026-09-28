#include "common.h"
#include "scr.h"

#define SCR_STACK_RET 27
#define SCR_STACK_TYPE_STRING 5

extern ScrVM *D_003BAA00;
extern ScrData *D_003BD78C;
extern ScrCommand D_0039E288[];
extern u32 (*D_00329930[])(ScrData *scr);

void scrPushInteger(ScrData *scr, s32 val);
void func_0010C150(ScrData *scr, f32 val);
void scrPushString(ScrData *scr, char *str);
void scrPushTypeFourValue(ScrData *scr, s32 val);
s32 func_0010C1E0(ScrData *scr);
f32 func_0010C2B8(ScrData *scr);
u32 func_0010C9A8(ScrData *scr, s32 op);

u32 scrPushImmediateInteger(ScrData *scr)
{
    scrPushInteger(scr, scr->instructions[scr->pc].parts.sOperand);
    scr->pc++;
    return 1;
}

u32 scrPushImmediateFloat(ScrData *scr)
{
    scr->pc++;
    func_0010C150(scr, scr->instructions[scr->pc].fOperand);
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
    func_0010C150(scr, D_003BAA00->floats[scr->instructions[scr->pc].parts.sOperand]);
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
    func_0010C150(scr, scr->localFloat[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 scrPushStringLiteral(ScrData *scr)
{
    scrPushString(scr, scr->strings + scr->instructions[scr->pc].parts.sOperand);
    scr->pc++;
    return 1;
}

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
    D_003BAA00->ints[scr->instructions[scr->pc].parts.sOperand] = func_0010C1E0(scr);
    scr->pc++;
    return 1;
}

u32 scrStoreGlobalFloat(ScrData *scr)
{
    f32 value;

    value = func_0010C2B8(scr);
    D_003BAA00->floats[scr->instructions[scr->pc].parts.sOperand] = value;
    scr->pc++;
    return 1;
}

u32 scrStoreLocalInteger(ScrData *scr)
{
    scr->localInt[scr->instructions[scr->pc].parts.sOperand] = func_0010C1E0(scr);
    scr->pc++;
    return 1;
}

u32 scrStoreLocalFloat(ScrData *scr)
{
    scr->localFloat[scr->instructions[scr->pc].parts.sOperand] = func_0010C2B8(scr);
    scr->pc++;
    return 1;
}

u32 func_0010C7E0(ScrData *scr)
{
    scr->pc++;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C7F8);

u32 func_0010C898(ScrData *scr)
{
    if (scr->sp == 0) {
        return 0;
    }
    scr->pc = func_0010C1E0(scr) + 1;
    return 1;
}

u32 scrJumpProcedure(ScrData *scr)
{
    scr->pc = scr->procedures[scr->instructions[scr->pc].parts.sOperand].addr;
    return 1;
}

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

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C9A8);

u32 func_0010CE90(ScrData *scr)
{
    func_0010C9A8(scr, 0);
    scr->pc++;
    return 1;
}

u32 func_0010CEC8(ScrData *scr)
{
    func_0010C9A8(scr, 1);
    scr->pc++;
    return 1;
}

u32 func_0010CF00(ScrData *scr)
{
    func_0010C9A8(scr, 2);
    scr->pc++;
    return 1;
}

u32 func_0010CF38(ScrData *scr)
{
    func_0010C9A8(scr, 3);
    scr->pc++;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010CF70);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D058);

u32 func_0010D100(ScrData *scr)
{
    func_0010C9A8(scr, 4);
    scr->pc++;
    return 1;
}

u32 func_0010D138(ScrData *scr)
{
    func_0010C9A8(scr, 5);
    scr->pc++;
    return 1;
}

u32 func_0010D170(ScrData *scr)
{
    func_0010C9A8(scr, 6);
    scr->pc++;
    return 1;
}

u32 func_0010D1A8(ScrData *scr)
{
    func_0010C9A8(scr, 7);
    scr->pc++;
    return 1;
}

u32 func_0010D1E0(ScrData *scr)
{
    func_0010C9A8(scr, 8);
    scr->pc++;
    return 1;
}

u32 func_0010D218(ScrData *scr)
{
    func_0010C9A8(scr, 9);
    scr->pc++;
    return 1;
}

u32 func_0010D250(ScrData *scr)
{
    func_0010C9A8(scr, 10);
    scr->pc++;
    return 1;
}

u32 func_0010D288(ScrData *scr)
{
    func_0010C9A8(scr, 11);
    scr->pc++;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D2C0);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D380);

/* Script command parameter `idx` (0 = first) as an int, converting floats and
 * dereferencing global variable references. */
s32 func_0010D428(s32 idx) {
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

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D4F0);

char *func_0010D5A8(s32 paramIdx)
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
