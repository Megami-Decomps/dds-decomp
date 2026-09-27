#include "common.h"

extern u64 func_0010FFE8(void);

extern s32 func_00110C70(u64, u64, u64);

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116CF8);

void func_00116D40(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x10) = arg1;
}

u32 func_00116D50(u64 arg0) {
    s32 temp_v0;
    u64 temp_v1;

    temp_v1 = func_0010FFE8();
    temp_v0 = func_00110C70(temp_v1, arg0, 6);
    return *(u32 *)(*(s32 *)(temp_v0 + 0x18) + 0x10);
}

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116D90);

u32 func_00116DE0(s32 arg0) {
    return *(u32 *)(arg0 + 0x18);
}

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116DE8);

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116F50);

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116FA0);
