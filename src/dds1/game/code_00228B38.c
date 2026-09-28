#include "common.h"

void func_00228CA0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

u32 func_002BC8F0(void *arg0, const char *arg1, s32 arg2);

extern u32 D_003BBDD0[];

void func_00228B38(u32 *arg0) {
    *arg0 = func_002BC8F0(D_003BBDD0, "solarnoise.spr", 0);
}

INCLUDE_ASM(const s32, "game/code_00228B38", func_00228B78);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00228B98);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00228BF8);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00228C38);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00228CA0);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00228E20);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00228FB0);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229100);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229230);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229320);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229410);

s32 func_002294C0(s32 arg0) {
    s32 v;

    v = *(u16 *)(arg0 + 4) + 1;
    *(u16 *)(arg0 + 4) = v;
    if ((f32)(s16)v > 60.0f) {
        *(u16 *)(arg0 + 4) = 0;
        *(u8 *)(arg0 + 6) = 0;
    }
    return *(s8 *)(arg0 + 6);
}

s32 func_00229500(s32 arg0) {
    s32 v;

    v = *(u16 *)(arg0 + 4) + 1;
    *(u16 *)(arg0 + 4) = v;
    if ((f32)(s16)v > 80.0f) {
        *(u16 *)(arg0 + 4) = 0;
        *(u8 *)(arg0 + 6) = 0;
    }
    return *(s8 *)(arg0 + 6);
}

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229540);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229750);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229A10);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229B70);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229CD0);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229D28);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229D80);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229E28);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229ED8);







INCLUDE_RODATA(const s32, "game/code_00228B38", D_003ACBF8);

INCLUDE_RODATA(const s32, "game/code_00228B38", D_003ACC68);

INCLUDE_RODATA(const s32, "game/code_00228B38", D_003ACC78);

INCLUDE_RODATA(const s32, "game/code_00228B38", D_003ACC88);

INCLUDE_SDATA(const s32, "game/code_00228B38", D_003BBDD0);


INCLUDE_SDATA(const s32, "game/code_00228B38", D_003BBDD8);

