#include "common.h"

extern u32 func_00151FC8(u32);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00186DC8);

void func_00186E28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x2c) = arg1;
}

void func_00186E30(s32 arg0) {
    u32 temp_v0;

    temp_v0 = func_00151FC8(2);
    *(u32 *)(arg0 + 0x2c) = temp_v0;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00186E60);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00186F90);

void func_00187080(s32 arg0) {
    func_002D0918(*(u32 *)(arg0 + 0x30));
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187098);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001872A0);

void func_00187308(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x2c) = arg1;
}

void func_00187310(s32 arg0) {
    u32 temp_v0;

    temp_v0 = func_00151FC8(2);
    *(u32 *)(arg0 + 0x2c) = temp_v0;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187340);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001873A8);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001873E0);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187460);

void func_00187580(s32 arg0) {
    func_002D0918(*(u32 *)(arg0 + 0x30));
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187598);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187788);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187988);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187C08);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_003A0F00);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_003A0F08);

