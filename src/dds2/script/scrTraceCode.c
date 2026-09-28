#include "common.h"
#include "scr.h"

extern u32 func_0010C408(void);

#define SCR_STACK_TYPE_STRING 5

extern ScrData *D_00438E8C;

#define SCR_STACK_RET 27

extern ScrVM *D_00435DD0;

INCLUDE_ASM(const s32, "script/scrTraceCode", scrPushImmediateInteger);

INCLUDE_ASM(const s32, "script/scrTraceCode", scrPushImmediateFloat);

INCLUDE_ASM(const s32, "script/scrTraceCode", scrPushGlobalInteger);

INCLUDE_ASM(const s32, "script/scrTraceCode", scrPushGlobalFloat);

u32 scrPushLocalInteger(ScrData *scr) {
    scrPushInteger(scr, scr->localInt[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", scrPushLocalFloat);

u32 scrPushStringLiteral(ScrData *scr) {
    scrPushString(scr, scr->strings + scr->instructions[scr->pc].parts.sOperand);
    scr->pc++;
    return 1;
}

u32 scrPushReturnValue(ScrData *scr) {
    scr->stackTypes[scr->sp] = scr->stackTypes[SCR_STACK_RET];
    scr->stackValues[scr->sp].i = scr->stackValues[SCR_STACK_RET].i;
    scr->sp++;
    scr->pc++;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", scrStoreGlobalInteger);

INCLUDE_ASM(const s32, "script/scrTraceCode", scrStoreGlobalFloat);

u32 scrStoreLocalInteger(ScrData *scr) {
    u32 value;

    value = func_0010C408();
    scr->localInt[scr->instructions[scr->pc].parts.sOperand] = value;
    scr->pc++;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", scrStoreLocalFloat);

u32 func_0010CA08(ScrData *scr) {
    scr->pc++;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010CA20);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010CAC0);

u32 scrJumpProcedure(ScrData *scr) {
    scr->pc = scr->procedures[scr->instructions[scr->pc].parts.sOperand].addr;
    return 1;
}

u32 scrCallProcedure(u32 arg0) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    scrPushTypeFourValue(arg0, *(u32 *)(temp_v0 + 0x18));
    *(u32 *)(temp_v0 + 0x18) =
              *(u32 *)
                (*(s16 *)(*(s32 *)(temp_v0 + 0x18) * 4 + *(s32 *)(temp_v0 + 0xbc) + 2) * 0x20 +
                  *(s32 *)(temp_v0 + 0xb4) + 0x18);
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

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010CBD0);

u32 func_0010D0B8(u32 arg0) {
    func_0010CBD0(arg0, 0);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D0F0(u32 arg0) {
    func_0010CBD0(arg0, 1);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D128(u32 arg0) {
    func_0010CBD0(arg0, 2);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D160(u32 arg0) {
    func_0010CBD0(arg0, 3);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D198);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D280);

u32 func_0010D328(u32 arg0) {
    func_0010CBD0(arg0, 4);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D360(u32 arg0) {
    func_0010CBD0(arg0, 5);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D398(u32 arg0) {
    func_0010CBD0(arg0, 6);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D3D0(u32 arg0) {
    func_0010CBD0(arg0, 7);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D408(u32 arg0) {
    func_0010CBD0(arg0, 8);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D440(u32 arg0) {
    func_0010CBD0(arg0, 9);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D478(u32 arg0) {
    func_0010CBD0(arg0, 10);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D4B0(u32 arg0) {
    func_0010CBD0(arg0, 0xb);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D4E8);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D5A8);

/* Script command parameter `idx` (0 = first) as an int, converting floats and
 * dereferencing global variable references. */
s32 func_0010D650(s32 idx) {
    ScrData *scr = D_00438E8C;
    s32 i = scr->sp - idx - 1;

    switch (scr->stackTypes[i]) {
    case 0:
    case 4:
        return scr->stackValues[i].i;
    case 1:
        return scr->stackValues[i].f;
    case 2:
        return D_00435DD0->ints[scr->stackValues[i].i];
    case 3:
        return D_00435DD0->floats[scr->stackValues[i].i];
    }
    return 0;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D718);

char *func_0010D7D0(s32 paramIdx)
{
    ScrData *scr = D_00438E8C;
    s32 paramSP = scr->sp - paramIdx - 1;
    s32 type = (s8)scr->stackTypes[paramSP];

    if (type >= 0) {
        if (type >= SCR_STACK_TYPE_STRING) {
            if (type == SCR_STACK_TYPE_STRING) {
                return scr->stackValues[paramSP].s;
            }
        }
    }
    return NULL;
}

void func_0010D818(s32 retVal)
{
    D_00438E8C->stackTypes[SCR_STACK_RET] = 0;
    D_00438E8C->stackValues[SCR_STACK_RET].i = retVal;
}

void func_0010D830(f32 retVal)
{
    D_00438E8C->stackTypes[SCR_STACK_RET] = 1;
    D_00438E8C->stackValues[SCR_STACK_RET].f = retVal;
}
