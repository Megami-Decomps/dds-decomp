#include "common.h"

INCLUDE_ASM(const s32, "game/code_002ED760", func_002ED760);

INCLUDE_ASM(const s32, "game/code_002ED760", func_002ED8D0);

INCLUDE_ASM(const s32, "game/code_002ED760", func_002EDAE0);

INCLUDE_ASM(const s32, "game/code_002ED760", func_002EDBB8);

INCLUDE_ASM(const s32, "game/code_002ED760", func_002EDBD8);

void func_002EDC30(s32 arg0) {
    *(u8 *)(arg0 + 1) = *(u8 *)(arg0 + 1) | 1;
}

void func_002EDC40(s32 arg0) {
    *(u8 *)(arg0 + 1) = *(u8 *)(arg0 + 1) | 2;
}

INCLUDE_ASM(const s32, "game/code_002ED760", func_002EDC50);

void func_002EDC88(u8 *arg0) {
    *arg0 = 0xff;
}

void func_002EDC98(s32 arg0, s32 arg1) {
    *(s32 *)(arg0 + 0x10) = *(s32 *)(arg0 + 0x10) + arg1;
    *(s32 *)(arg0 + 0x14) = *(s32 *)(arg0 + 0x14) - arg1;
    *(s32 *)(arg0 + 0x18) = *(s32 *)(arg0 + 0x18) + arg1;
}

INCLUDE_ASM(const s32, "game/code_002ED760", func_002EDCC0);

INCLUDE_ASM(const s32, "game/code_002ED760", func_002EDD98);

INCLUDE_SDATA(const s32, "game/code_002ED760", D_003BD638);

