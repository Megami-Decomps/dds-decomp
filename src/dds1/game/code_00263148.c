#include "common.h"

extern u32 func_00265E68(u32, s32);

extern s32 func_0021F600(u32);

extern s32 func_00101A70();

INCLUDE_ASM(const s32, "game/code_00263148", func_00263148);

u32 func_00263220(u32 arg0) {
    u32 temp_v0;
    s32 temp_v1;

    temp_v1 = (s32)arg0;
    temp_v0 = func_00265E68(**(u32 **)(temp_v1 + 0x98), temp_v1 + 0x4c4);
    *(u32 *)(temp_v1 + 0x244) = temp_v0;
    func_00263148(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00263148", func_00263260);

INCLUDE_ASM(const s32, "game/code_00263148", func_00263378);

u32 func_002633D8(void) {
    s64 temp_v0;

    temp_v0 = func_0021F600(0x911);
    if (temp_v0 == 0) {
        func_0021F580(0x911);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00263148", func_00263408);

INCLUDE_ASM(const s32, "game/code_00263148", func_00263570);

INCLUDE_ASM(const s32, "game/code_00263148", func_002635C0);

s32 func_00263608(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();
    temp_v0[144] = 0;
    temp_v0[241] = 0;
    return 1;
}

u32 func_00263638(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00263148", func_00263640);

INCLUDE_ASM(const s32, "game/code_00263148", func_00263728);

INCLUDE_ASM(const s32, "game/code_00263148", func_00263838);

void func_002639E0(s32 arg0) {
    func_0027B268(arg0 + 0xd1c, 0x20);
}

INCLUDE_ASM(const s32, "game/code_00263148", func_00263A00);

INCLUDE_ASM(const s32, "game/code_00263148", func_00263B78);

INCLUDE_ASM(const s32, "game/code_00263148", func_00263C98);

INCLUDE_ASM(const s32, "game/code_00263148", func_00263D10);

u32 func_00263D70(void) {
    s8 temp_v0;
    s32 temp_v1;
    u32 *puVar3;
    s8 *pcVar4;
    s32 temp_v2;
    s32 temp_v3;
    s32 temp_v4;

    temp_v1 = func_00101A70();
    temp_v4 = 0;
    temp_v2 = 4;
    temp_v3 = (*(s32 **)(temp_v1 + 0x98))[1] * 3;
    pcVar4 = (s8 *)(**(s32 **)(temp_v1 + 0x98) + 0x16);
    do {
        temp_v0 = *pcVar4;
        pcVar4 = pcVar4 + 1;
        temp_v2 = temp_v2 - 1;
        temp_v4 = temp_v4 + temp_v0;
    } while (-1 < temp_v2);
    *(u32 *)(temp_v1 + 0x3cc) = 0;
    temp_v2 = 4;
    puVar3 = (u32 *)(temp_v1 + 0x3e0);
    if (0x1ef - temp_v4 < temp_v3) {
        temp_v3 = 0x1ef - temp_v4;
    }
    *(s32 *)(temp_v1 + 0x3c8) = temp_v3;
    do {
        temp_v2 = temp_v2 - 1;
        *puVar3 = 0;
        puVar3 = puVar3 + -1;
    } while (-1 < temp_v2);
    if (*(s32 *)(temp_v1 + 0x1578) != 0) {
        func_002830F0(*(u32 *)(temp_v1 + 0xd10), 0);
    }
    return 1;
}

u32 func_00263E30(void) {
    return 1;
}

void func_00263E38(s32 arg0) {
    s32 temp_v0;
    u32 *puVar2;

    *(u32 *)(arg0 + 0x3cc) = 0;
    puVar2 = (u32 *)(arg0 + 0x3e0);
    temp_v0 = 4;
    do {
        temp_v0 = temp_v0 - 1;
        *puVar2 = 0;
        puVar2 = puVar2 + -1;
    } while (-1 < temp_v0);
}

void func_00263E70(u32 arg0, u32 arg1) {
    func_002CCE60(arg0, (s32)arg1 + 0x3d0);
    func_00262AC0(arg0, arg1);
}



INCLUDE_SDATA(const s32, "game/code_00263148", D_003BC550);

