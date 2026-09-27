#include "common.h"

extern u32 D_003BD9E4;

extern u32 D_003BD9E0;

extern u8 D_003BD2F0;
extern u32 D_003BD2F4;
extern u32 D_003BD2F8;

void func_002D10B0(u32 arg0, u32 arg1) {
    D_003BD2F4 = arg0;
    D_003BD2F8 = arg1;
    D_003BD2F0 = 1;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D10C8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1318);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1350);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1380);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D14C8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1590);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1698);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D16F0);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1740);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1798);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D17D8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D18F8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1A18);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1AB8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1B28);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1B90);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1C08);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1C28);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1C70);

u32 func_002D1D10(void) {
    return D_003BD9E0;
}

u32 func_002D1D18(void) {
    return D_003BD9E4;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1D20);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1D80);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1FF0);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2070);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2128);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2140);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2168);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D22C0);

u32 func_002D2300(s32 arg0) {
    return *(u32 *)(arg0 + 0x28);
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2308);

s32 func_002D2330(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x2c);
    if (temp_v0 == 0) {
        func_002D2F80();
        temp_v0 = *(s32 *)(arg0 + 0x2c);
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2368);

u8 func_002D2390(s32 arg0) {
    return *(u8 *)(arg0 + 0x18);
}

u32 func_002D2398(s32 arg0) {
    u32 temp_v0;

    temp_v0 = 0;
    if (*(s32 *)(arg0 + 0x14) != 0) {
        temp_v0 = *(u32 *)(*(s32 *)(arg0 + 0x14) + 0xc);
    }
    return temp_v0;
}

u32 func_002D23B0(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x10) + 0xc);
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D23C0);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2410);

u64 func_002D2468(s32 arg0) {
    return *(u64 *)(*(s32 *)(arg0 + 0x28) + 0x20);
}

u64 func_002D2478(s32 arg0) {
    return *(u64 *)(*(s32 *)(arg0 + 0x28) + 0x10);
}

u64 func_002D2488(s32 arg0) {
    return *(u64 *)(*(s32 *)(arg0 + 0x28) + 0x30);
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2498);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D24C0);

void func_002D2530(s32 arg0, u8 arg1) {
    *(u8 *)(arg0 + 0x1f) = arg1;
    func_002D2FB0();
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2548);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2650);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D26A8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2700);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2728);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2800);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2950);
