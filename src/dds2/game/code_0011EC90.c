#include "common.h"

extern u64 func_002AB598(void);

extern u64 func_0011C0B0(u64, u64);

extern s32 func_0010D818(s32 arg0);

extern u64 func_0010D650(u64);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EC90);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011ECC8);

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

void func_0011EFE0(u32 arg0) {
    func_0019C5B0(*(u32 *)((s32)arg0 + 0x10));
    func_00328E48(arg0);
}

void func_0011F010(s32 arg0) {
    func_0019D518(*(u32 *)(arg0 + 0x10));
}

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011F028);

void func_0011F0C0(s32 arg0, u8 arg1) {
    func_0019D120(*(u32 *)(arg0 + 0x10), arg1);
}

void func_0011F0E0(void) {
    func_00328E48();
}

INCLUDE_SDATA(const s32, "game/code_0011EC90", D_00435EA8);

