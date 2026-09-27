#include "common.h"

extern u64 func_0010D650(u64);

extern u64 func_0011C0B0(u64, u64);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EC90);

u32 func_0011ECC8(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    temp_v0 = func_0011C0B0(temp_v0, temp_v1);
    func_0010D818(temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011ED10);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011ED60);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011ED88);

void func_0011EE28(s32 *arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;

    temp_v0 = arg0[1];
    if (temp_v0 == 0) {
        *arg0 = arg1;
    }
    else {
        *(s32 *)(temp_v0 + arg2 + 4) = arg1;
    }
    *(s32 *)(arg1 + arg2) = temp_v0;
    ((s32 *)(arg1 + arg2))[1] = 0;
    arg0[1] = arg1;
}

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EE58);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EE98);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EED8);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EF18);

void func_0011EF48(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 8) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EF50);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EFA0);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EFE0);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011F010);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011F028);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011F0C0);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011F0E0);
