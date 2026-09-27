#include "common.h"

extern u32 func_0012A6F0(u32);

extern u32 func_0012AC90(u32, u32, u32, u32, u32, u32);

INCLUDE_ASM(const s32, "game/code_001104F0", func_001104F0);

u16 func_00110628(s32 arg0) {
    u16 temp_v0;

    temp_v0 = 0;
    if (arg0 != 0) {
        temp_v0 = *(u16 *)((s32)arg0 + 6);
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110640);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110680);

u32 func_001106B8(s16 *arg0) {
    arg0[2] = *arg0;
    return (u32)~(s32)*arg0 >> 0x1f;
}

INCLUDE_ASM(const s32, "game/code_001104F0", func_001106D8);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110720);

INCLUDE_ASM(const s32, "game/code_001104F0", func_001107A0);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110860);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110938);

INCLUDE_ASM(const s32, "game/code_001104F0", func_001109F0);

void func_00110A88(s32 arg0, s8 arg1) {
    if (*(s32 *)(arg0 + 0x18) != 0) {
        *(s32 *)(*(s32 *)(arg0 + 0x18) + 0x20) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110AA8);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110B50);

void func_00110BE0(s32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    func_00110C18();
    *(u32 *)(temp_v0 + 0xc) = arg1;
}

u32 func_00110C18(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 0xc);
}

void func_00110C28(s32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    func_00110C60();
    *(u32 *)(temp_v0 + 0x10) = arg1;
}

u32 func_00110C60(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x10);
}

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110C70);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110CD8);

void func_00110D70(s32 arg0, u32 arg1) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    temp_v1 = func_0012A6F0(arg1);
    *(u32 *)(temp_v0 + 0x14) = temp_v1;
}

void func_00110DA0(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    temp_v1 = func_0012AC90(arg1, arg2, arg3, arg4, arg5, arg6);
    *(u32 *)(temp_v0 + 0x14) = temp_v1;
}
