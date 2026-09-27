#include "common.h"

INCLUDE_ASM(const s32, "effect/effBattle", func_00160B00);

u16 func_00160B90(s32 arg0) {
    return *(u16 *)(arg0 + 0x1c);
}

u32 func_00160B98(s32 arg0) {
    return *(u32 *)(arg0 + 0x10);
}

u32 func_00160BA0(u32 *arg0) {
    return *arg0;
}

void func_00160BA8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x118) = arg1;
}

void func_00160BB0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x11c) = arg1;
}

INCLUDE_ASM(const s32, "effect/effBattle", func_00160BB8);

INCLUDE_ASM(const s32, "effect/effBattle", func_00160BC8);

u32 func_00160BE0(s32 arg0) {
    return *(u32 *)(arg0 + 0x14);
}

INCLUDE_ASM(const s32, "effect/effBattle", func_00160BE8);

u32 func_00160C18(s32 arg0) {
    return *(u32 *)(arg0 + 0x18);
}

void func_00160C20(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x120) = arg1;
}

u32 func_00160C28(s32 arg0) {
    return *(u32 *)(arg0 + 0x120);
}

INCLUDE_ASM(const s32, "effect/effBattle", func_00160C30);

INCLUDE_RODATA(const s32, "effect/effBattle", D_003A0DE0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_003A0DF0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_003A0E00);

INCLUDE_ASM(const s32, "effect/effBattle", func_00160D88);

INCLUDE_ASM(const s32, "effect/effBattle", func_00161588);

INCLUDE_ASM(const s32, "effect/effBattle", func_00161600);

INCLUDE_ASM(const s32, "effect/effBattle", func_00161650);

INCLUDE_ASM(const s32, "effect/effBattle", func_00161790);
