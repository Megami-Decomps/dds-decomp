#include "common.h"

extern u32 func_001730B8(u32);

extern u32 func_00173018(u32);

INCLUDE_ASM(const s32, "game/code_00175B00", func_00175B00);

INCLUDE_ASM(const s32, "game/code_00175B00", func_00175B18);

void func_00175B20(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x130) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00175B00", func_00175B28);

INCLUDE_ASM(const s32, "game/code_00175B00", func_00175B50);

void func_00175D88(u32 arg0) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if (*(s32 *)(temp_v0 + 0x7c) != 0) {
        func_00173068(*(s32 *)(temp_v0 + 0x7c));
    }
    func_002DAA68(*(u32 *)(temp_v0 + 0x74));
    func_002D0918(*(u32 *)(temp_v0 + 0x78));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00175B00", func_00175DD0);

void func_00176020(s32 arg0, u32 arg1) {
    u32 temp_v0;

    temp_v0 = func_00173018(arg1);
    *(u32 *)(arg0 + 0x7c) = temp_v0;
}

void func_00176050(s32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_001730B8(*(u32 *)(arg1 + 0x7c));
    *(u32 *)(arg0 + 0x7c) = temp_v0;
}

s32 func_00176080(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 100) + arg1 * *(s32 *)(arg0 + 0x5c) * 0x10;
}

s32 func_00176098(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x68) + arg1 * *(s32 *)(arg0 + 0x5c) * 8;
}

u32 func_001760B0(s32 arg0, s32 arg1) {
    return *(u32 *)(arg1 * 4 + *(s32 *)(arg0 + 0x70));
}

INCLUDE_ASM(const s32, "game/code_00175B00", func_001760C8);

INCLUDE_ASM(const s32, "game/code_00175B00", func_001760F8);
