#include "common.h"

void *func_00343ED0(s32 arg0, u32 *arg1, s32 arg2);

s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

s32 func_0010C2F0();

s32 func_0010C298();

typedef struct { u8 pad0[0x40]; s32 unk40[256]; s32 unk440[256]; } ScrProcGlobals;

extern ScrProcGlobals *D_00435DD0;

s32 func_0010BD50(s32 arg0, s32 arg1)
{
    u32 buf[4];
    void *r;
    s32 p;
    s32 r2;
    r = func_00343ED0(arg0, buf, 0);
    p = buf[0];
    if (p == 0)
    {
        return 0;
    }
    r2 = func_0010BC40(p, arg1);
    if (r2 != 0)
    {
        *(s32 *)(r2 + 0xD8) = (s32)r;
    }
    return r2;
}

s32 scrProcCreateTask(s32 arg0, s32 arg1)
{
    s32 r;
    r = kwlnTaskCreate(*(s32 *)(arg1 + 0xB4) + (*(s32 *)(arg1 + 0xC8) << 5), arg0, 1, 1, func_0010C2F0, func_0010C298, arg1);
    *(s32 *)(arg1 + 0xE4) = r;
    return r;
}

void func_0010BE08(void)
{
    s32 i;
    /* Countdown with a forward index; gcc keeps a single pointer (see asm). */
    for (i = 255; i >= 0; i--)
    {
        D_00435DD0->unk40[255 - i] = 0;
        D_00435DD0->unk440[255 - i] = 0;
    }
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BE40);

void func_0010BE78(u32 arg0) {
    func_0010BD50(arg0, 0);
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BE90);

void func_0010BEC8(u32 arg0) {
    func_0010BC40(arg0, 0);
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BEE0);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BF30);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BF48);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BFE0);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010C058);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010C100);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010C158);







INCLUDE_RODATA(const s32, "script/scrScriptProcess", D_00411408);

