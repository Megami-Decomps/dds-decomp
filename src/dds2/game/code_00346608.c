#include "common.h"

INCLUDE_ASM(const s32, "game/code_00346608", func_00346608);

INCLUDE_ASM(const s32, "game/code_00346608", func_00346778);

INCLUDE_ASM(const s32, "game/code_00346608", func_00346988);

INCLUDE_ASM(const s32, "game/code_00346608", func_00346A60);

INCLUDE_ASM(const s32, "game/code_00346608", func_00346A80);

void func_00346AD8(s32 arg0) {
    *(u8 *)(arg0 + 1) = *(u8 *)(arg0 + 1) | 1;
}

void func_00346AE8(s32 arg0) {
    *(u8 *)(arg0 + 1) = *(u8 *)(arg0 + 1) | 2;
}

INCLUDE_ASM(const s32, "game/code_00346608", func_00346AF8);

void func_00346B30(u8 *arg0) {
    *arg0 = 0xff;
}

void func_00346B40(s32 arg0, s32 arg1) {
    *(s32 *)(arg0 + 0x10) = *(s32 *)(arg0 + 0x10) + arg1;
    *(s32 *)(arg0 + 0x14) = *(s32 *)(arg0 + 0x14) - arg1;
    *(s32 *)(arg0 + 0x18) = *(s32 *)(arg0 + 0x18) + arg1;
}

INCLUDE_ASM(const s32, "game/code_00346608", func_00346B68);

INCLUDE_ASM(const s32, "game/code_00346608", func_00346C40);
