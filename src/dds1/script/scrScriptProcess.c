#include "common.h"

typedef struct { u8 pad0[0x40]; s32 unk40[256]; s32 unk440[256]; } ScrProcGlobals;
extern ScrProcGlobals *D_003BAA00;
void *func_002EB028(s32 arg0, u32 *arg1, s32 arg2);
s32 func_00101570(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
s32 func_0010C0C8();
s32 func_0010C070();
s32 func_0010B7C0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
s32 func_0010BB28(s32 arg0, s32 arg1)
{
    u32 buf[4];
    void *r;
    s32 p;
    s32 r2;
    r = func_002EB028(arg0, buf, 0);
    p = buf[0];
    if (p == 0)
    {
        return 0;
    }
    r2 = func_0010BA18(p, arg1);
    if (r2 != 0)
    {
        *(s32 *)(r2 + 0xD8) = (s32)r;
    }
    return r2;
}

s32 func_0010BB88(s32 arg0, s32 arg1)
{
    s32 r;
    r = func_00101570(*(s32 *)(arg1 + 0xB4) + (*(s32 *)(arg1 + 0xC8) << 5), arg0, 1, 1, func_0010C0C8, func_0010C070, arg1);
    *(s32 *)(arg1 + 0xE4) = r;
    return r;
}

void func_0010BBE0(void)
{
    s32 i;
    /* Countdown with a forward index; gcc keeps a single pointer (see asm). */
    for (i = 255; i >= 0; i--)
    {
        D_003BAA00->unk40[255 - i] = 0;
        D_003BAA00->unk440[255 - i] = 0;
    }
}

void func_0010BC18(s32 arg0, s32 arg1, s32 arg2)
{
    func_0010BB88(arg0, func_0010BB28(arg1, arg2));
}

s32 func_0010BC50(s32 arg0)
{
    return func_0010BB28(arg0, 0);
}

void func_0010BC68(s32 arg0, s32 arg1, s32 arg2)
{
    func_0010BB88(arg0, func_0010BA18(arg1, arg2));
}

s32 func_0010BCA0(s32 arg0)
{
    return func_0010BA18(arg0, 0);
}

s32 func_0010BCB8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{
    return func_0010BB88(a0, func_0010B7C0(a1, a2, a3, a4, a5, a6, a7, a8));
}

s32 func_0010BD08(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6)
{
    return func_0010B7C0(arg0, arg1, arg2, arg3, arg4, arg5, arg6, 0);
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BD20);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BDB8);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BE30);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BED8);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BF30);
