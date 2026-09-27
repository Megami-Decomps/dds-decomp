#include "common.h"

extern u32 func_002DA450(void);

extern u32 D_003BD34C;

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9748);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D99B0);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9A70);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9B08);

void func_002D9B50(s32 arg0) {
    func_002D9B08(*(u32 *)(arg0 + 0x90));
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9B68);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9C28);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9CC8);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9D00);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9D80);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9DD8);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9E58);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9E98);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9ED8);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9F08);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9F38);

void func_002D9F50(s32 arg0) {
    *(u8 *)(arg0 + 0x19) = *(u8 *)(arg0 + 0x19) & 0xfd;
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9F60);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9F80);

void func_002D9FA0(u32 arg0) {
    D_003BD34C = arg0;
}

void func_002D9FA8(u32 arg0) {
    func_002E75F0(arg0, 4, 4);
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9FC8);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA058);

void func_002DA0C0(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;

    if ((arg1 < *(s16 *)(arg0 + 4)) && (arg2 != 0)) {
        temp_v0 = (s32)arg1;
        do {
            temp_v0 = temp_v0 + 1;
        } while ((s64)temp_v0 != (s64)*(s16 *)(arg0 + 4));
        *(s16 *)(arg0 + 4) = (s16)arg1;
    }
    func_002E7730();
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA118);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA1B0);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA240);

void func_002DA270(u32 arg0) {
    func_002E75F0(arg0, 4, 8);
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA290);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA2F8);

void func_002DA340(void) {
    func_002E76B0();
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA358);

void func_002DA3C0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 3;
}

void func_002DA3D8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 3;
}

void func_002DA3F0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 3;
}

void func_002DA408(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x28) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 3;
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA420);

void func_002DA438(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x2c) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 3;
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA450);

void func_002DA490(s32 arg0) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x38) == 0) {
        temp_v0 = func_002DA450();
        *(u32 *)(arg0 + 0x38) = temp_v0;
    }
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA4C8);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA548);

void func_002DA5B0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 0x30;
}

void func_002DA5C8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x34) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 0x30;
}

void func_002DA5E0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x30) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 0x30;
}

void func_002DA5F8(s32 arg0) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x3c) == 0) {
        temp_v0 = func_002DA450();
        *(u32 *)(arg0 + 0x3c) = temp_v0;
    }
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA630);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA6B0);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA718);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA730);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA830);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAA00);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAA68);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAAA0);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAAE0);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAB80);

void func_002DAC68(s32 arg0, s32 arg1) {
    func_002DAB80(arg1 + 0x68, *(u32 *)(arg0 + 0x38));
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAC88);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAD18);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAD30);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAE00);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAEE8);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAF88);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAFE8);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DB048);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DB158);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DB1C8);
