#include "common.h"

u32 func_00304030(void *arg0, const char *arg1, s32 arg2);

extern u32 D_00437210[];

void func_002437F0(u32 *arg0) {
    *arg0 = func_00304030(D_00437210, "solarnoise.spr", 0);
}

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243830);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243850);

INCLUDE_ASM(const s32, "game/code_002437F0", func_002438B0);

INCLUDE_ASM(const s32, "game/code_002437F0", func_002438F0);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243958);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243AD8);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243C68);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243DB8);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243EE8);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243FD8);

INCLUDE_ASM(const s32, "game/code_002437F0", func_002440C8);

s32 func_00244178(s32 arg0) {
    s32 v;

    v = *(u16 *)(arg0 + 4) + 1;
    *(u16 *)(arg0 + 4) = v;
    if ((f32)(s16)v > 60.0f) {
        *(u16 *)(arg0 + 4) = 0;
        *(u8 *)(arg0 + 6) = 0;
    }
    return *(s8 *)(arg0 + 6);
}

s32 func_002441B8(s32 arg0) {
    s32 v;

    v = *(u16 *)(arg0 + 4) + 1;
    *(u16 *)(arg0 + 4) = v;
    if ((f32)(s16)v > 80.0f) {
        *(u16 *)(arg0 + 4) = 0;
        *(u8 *)(arg0 + 6) = 0;
    }
    return *(s8 *)(arg0 + 6);
}

INCLUDE_ASM(const s32, "game/code_002437F0", func_002441F8);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00244408);

INCLUDE_ASM(const s32, "game/code_002437F0", func_002446C8);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00244828);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00244988);

INCLUDE_ASM(const s32, "game/code_002437F0", func_002449E0);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00244A38);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00244AE0);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00244B90);

INCLUDE_RODATA(const s32, "game/code_002437F0", D_00422168);

INCLUDE_RODATA(const s32, "game/code_002437F0", D_004221D8);

INCLUDE_RODATA(const s32, "game/code_002437F0", D_004221E8);

INCLUDE_RODATA(const s32, "game/code_002437F0", D_00422238);

INCLUDE_RODATA(const s32, "game/code_002437F0", D_00422308);

INCLUDE_SDATA(const s32, "game/code_002437F0", D_00437210);

INCLUDE_SDATA(const s32, "game/code_002437F0", D_00437218);

