#include "common.h"

extern u32 D_003BB874;

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FC7D8);

void func_001FC990(void) {
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FC998);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FCAC0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FCB50);

INCLUDE_RODATA(const s32, "game/code_001FC7D8", D_003A57E0);

INCLUDE_RODATA(const s32, "game/code_001FC7D8", D_003A57F0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FCBA0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FCFB8);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FD170);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FD3C0);

s32 func_001FD5B0(s32 arg0, s32 arg1) {
    return arg1 + (((*(s32 *)(arg0 + 0x110) >> 9) ^ 1U) & 1);
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FD5C8);

void func_001FD990(s32 arg0) {
    u8 temp_v0;

    arg0 = *(s32 *)(arg0 + 0x20);
    temp_v0 = *(u8 *)(arg0 + 0x318);
    if (temp_v0 != 0) {
        *(u8 *)(arg0 + 0x318) = temp_v0 + 0xff;
    }
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FD9B0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FDA78);

void func_001FDED8(s32 arg0) {
    u8 temp_v0;

    arg0 = *(s32 *)(arg0 + 0x20);
    temp_v0 = *(u8 *)(arg0 + 0x319);
    if (temp_v0 != 0) {
        *(u8 *)(arg0 + 0x319) = temp_v0 + 0xff;
    }
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FDEF8);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FDF98);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE088);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE118);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE198);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE228);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE320);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE3B8);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE468);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE500);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE5C0);

u32 func_001FE658(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE660);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE6F0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE790);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE820);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE8B8);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE950);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE9F8);

u32 func_001FEA78(u32 arg0) {
    D_003BB874 = D_003BB874 * 0x41c64e6d + 0x3039;
    return (D_003BB874 >> 0x10) * (arg0 & 0xffff) >> 0x10;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEAA8);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEB10);

u32 func_001FEB78(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0xb;
    *(u32 *)(arg0 + 0x24) = 0xc2;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEB90);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEC10);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEC68);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FECD8);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FED20);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEDE0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEE30);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEE50);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEE70);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEE90);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEEB0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEED0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEEF0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEF10);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEF30);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEF50);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEF70);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEF90);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEFB0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEFD0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEFF0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FF010);
