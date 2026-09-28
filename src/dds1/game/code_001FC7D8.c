#include "common.h"

extern s32 func_001A2FD8(s32, s32);
extern void func_0010F770(s32);
extern s32 func_002D9E98(s32, s32);
extern void func_0011E280(s32, f32, f32, f32, f32);

extern s32 func_001A17F0();
extern s8 D_003BB870;
extern s32 D_003BB87C;
extern s32 func_002011C8(s32, s32);
extern s32 func_002CFF68(s32);
extern void func_002CFF98(s32);

extern u32 D_003BB874;

extern void func_00202668(s32, s32);
extern void func_00202F90(s32, s32);
extern void func_00203248(s32, s32);
extern void func_00203098();
extern void func_00203A80(s32, s32);
extern void func_00203BA8(s32, s32);
extern void func_002041A0();
extern void func_002033C0();
extern void func_002034D0();
extern void func_002035E0();
extern void func_00204008();
extern void func_00204028();
extern void func_00203F98();
extern void func_00203CA8();
extern void func_00204048(s32, s32);
extern void func_00204080();

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

s32 func_001FEB10(s32 arg0) {
    s32 temp_v0 = func_002CFF68(0x10);
    s32 temp_v1 = *(s32 *)(arg0 + 0x18);

    D_003BB87C = temp_v0;
    *(s32 *)temp_v0 = arg0;
    if (func_002011C8(temp_v1, 0) != 0) {
        func_002CFF98(D_003BB87C);
        return 1;
    }
    func_002CFF98(D_003BB87C);
    return 0;
}

u32 func_001FEB78(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0xb;
    *(u32 *)(arg0 + 0x24) = 0xc2;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEB90);

void func_001FEC10(void) {
    s32 node = *(s32 *)(func_001A17F0() + 0x224);

    if (node != NULL) {
        do {
            if (*(s32 *)(node + 0x18) != 0) {
                *(u8 *)(node + 0x146) = 0;
            }
            node = *(s32 *)(node + 0x16C);
        } while (node != NULL);
    }
    D_003BB870 = 0;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEC68);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FECD8);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FED20);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEDE0);

void func_001FEE30(s32 arg0) {
    func_00202668(arg0, 0);
}

void func_001FEE50(s32 arg0) {
    func_00202F90(arg0, 0);
}

void func_001FEE70(s32 arg0) {
    func_00203248(arg0, 0);
}

void func_001FEE90(void) {
    func_00203098();
}

void func_001FEEB0(s32 arg0) {
    func_00203A80(arg0, 0);
}

void func_001FEED0(s32 arg0) {
    func_00203BA8(arg0, 0);
}

void func_001FEEF0(void) {
    func_002041A0();
}

void func_001FEF10(void) {
    func_002033C0();
}

void func_001FEF30(void) {
    func_002034D0();
}

void func_001FEF50(void) {
    func_002035E0();
}

void func_001FEF70(void) {
    func_00204008();
}

void func_001FEF90(void) {
    func_00204028();
}

void func_001FEFB0(void) {
    func_00203F98();
}

void func_001FEFD0(void) {
    func_00203CA8();
}

void func_001FEFF0(s32 arg0) {
    func_00204048(arg0, 0);
}

void func_001FF010(void) {
    func_00204080();
}
