#include "common.h"

extern s32 func_00101A70();

INCLUDE_ASM(const s32, "game/code_00272798", func_00272798);

INCLUDE_ASM(const s32, "game/code_00272798", func_002728F8);

INCLUDE_ASM(const s32, "game/code_00272798", func_002729C8);

s32 func_00272A00(void) {
    u8 *context = (u8 *)func_00101A70();
    func_00271308(5, context);
    func_0027E790(*(u32 *)(context + 0x138), *(u32 *)(context + 0x114), 0, 1);
    func_002912C8(0);
    return 1;
}

u32 func_00272A58(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    func_0027E790(*(u32 *)(temp_v0 + 0x138), *(u32 *)(temp_v0 + 0x6c), 0, 1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00272798", func_00272A90);

INCLUDE_ASM(const s32, "game/code_00272798", func_00272B00);

INCLUDE_ASM(const s32, "game/code_00272798", func_00272B80);

u32 func_00272BB8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00272798", func_00272BC0);


INCLUDE_SDATA(const s32, "game/code_00272798", D_003BC6C8);

INCLUDE_SDATA(const s32, "game/code_00272798", D_003BC6D0);


INCLUDE_SDATA(const s32, "game/code_00272798", D_003BC6D8);

