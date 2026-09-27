#include "common.h"

extern u32 func_002CFEB8(u32);

extern u32 func_002D42B8(u32);

extern s32 D_003BD314;
extern u32 D_003BD318;
extern s32 D_003BD320;
extern s32 D_003BD324;

extern u64 func_002D3E88(void);
extern s64 func_00312C08(void);

extern s32 D_003BD30C;
extern s32 func_002CF440(u32, u32, u32);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D33C8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3558);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3598);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D35B8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D37A8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3880);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D38B8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D39B0);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3B28);

void func_002D3BE0(u32 *arg0, u32 arg1) {
    if (D_003BD30C < 0) {
        D_003BD30C = func_002CF440(1, 0x7f, 0);
    }
    *arg0 = arg1;
    arg0[1] = 0;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3C30);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3D00);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3D40);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3E10);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3E88);

u64 func_002D3EE8(void) {
    s64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_00312C08();
    temp_v1 = func_002D3E88();
    if (temp_v0 != 0) {
        EIntr();
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3F30);

void func_002D3FA0(s32 arg0) {
    D_003BD320 = (&D_003BD318)[arg0];
    D_003BD324 = (&D_003BD318)[arg0] + D_003BD314;
}

s32 func_002D3FC0(void) {
    return D_003BD324 - D_003BD320;
}

s32 func_002D3FD0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_003BD320;
    D_003BD320 = D_003BD320 + ((arg0 + 0xfU) & 0xfffffff0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3FF0);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3FF8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4010);

void func_002D4038(s32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 8);
    if (temp_v0 == 0) {
        *(u32 *)(arg0 + 4) = arg1;
    }
    else {
        *(u8 *)(temp_v0 + 3) = 0x20;
        *(u32 *)(temp_v0 + 4) = arg1 & 0xfffffff;
    }
    *(u32 *)(arg0 + 8) = arg1;
}

void func_002D4070(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 8);
    if (temp_v0 == 0) {
        *(u32 *)(arg0 + 4) = arg1;
    }
    else {
        *(u8 *)(temp_v0 + 3) = 0x20;
        *(u32 *)(temp_v0 + 4) = arg1 & 0xfffffff;
    }
    *(u32 *)(arg0 + 8) = arg2;
}

void func_002D40A8(s32 arg0, u32 arg1) {
    s32 temp_v0;

    *(u8 *)(arg1 + 3) = 0x30;
    temp_v0 = *(s32 *)(arg0 + 8);
    if (temp_v0 == 0) {
        *(u32 *)(arg0 + 4) = arg1;
    }
    else {
        *(u8 *)(temp_v0 + 3) = 0x20;
        *(u32 *)(temp_v0 + 4) = arg1 & 0xfffffff;
    }
    *(u32 *)(arg0 + 8) = arg1 + 0x10;
}

void func_002D40E8(s32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 8);
    if (temp_v0 == 0) {
        *(u32 *)(arg0 + 4) = arg1;
    }
    else {
        *(u8 *)(temp_v0 + 3) = 0x20;
        *(u32 *)(temp_v0 + 4) = arg1 & 0xfffffff;
    }
    *(u32 *)(arg0 + 8) = arg1 + 0x30;
}

void func_002D4120(s32 arg0, u32 arg1) {
    s32 temp_v0;

    *(u8 *)(arg1 + 3) = 0x50;
    temp_v0 = *(s32 *)(arg0 + 8);
    if (temp_v0 == 0) {
        *(u32 *)(arg0 + 4) = arg1;
    }
    else {
        *(u8 *)(temp_v0 + 3) = 0x20;
        *(u32 *)(temp_v0 + 4) = arg1 & 0xfffffff;
    }
    *(u32 *)(arg0 + 8) = arg1 + 0x10;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4160);

void func_002D41C0(s32 arg0, s32 arg1) {
    s32 *piVar1;

    if (*(s32 *)(arg1 + 4) != 0) {
        piVar1 = *(s32 **)(arg0 + 8);
        if (piVar1 == (s32 *)0x0) {
            *(s32 *)(arg0 + 4) = arg1;
        }
        else {
            *piVar1 = arg1;
            func_002D4368(piVar1);
        }
        *(s32 *)(arg0 + 8) = arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4218);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4240);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D42B8);

s32 func_002D4320(s32 arg0, u32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002D42B8(arg1);
    *(u8 *)(arg0 + 3) = 0x20;
    *(u32 *)(arg0 + 4) = temp_v0 & 0xfffffff;
    return temp_v0 + 0x10;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4368);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D43F8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4490);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4540);

void func_002D4558(s32 arg0, u32 *arg1) {
    if (*(u32 **)(arg0 + 8) == (u32 *)0x0) {
        *(u32 **)(arg0 + 4) = arg1;
    }
    else {
        **(u32 **)(arg0 + 8) = arg1;
    }
    *(u32 **)(arg0 + 8) = arg1;
    *arg1 = 0;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4578);

void func_002D4588(s32 *arg0, s32 arg1) {
    if (arg0[1] == 0) {
        *arg0 = arg1;
    }
    else {
        **(u32 **)(arg0[1] + 8) = *(u32 *)(arg1 + 8);
    }
    arg0[1] = arg1;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D45B0);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D45F0);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4678);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4730);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D47B8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4800);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D48A8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D49E8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4BA0);

void func_002D4C40(u32 arg0, u32 arg1, u32 arg2) {
    func_002D4558(arg1, arg2);
    func_002D4038(arg0, (s32)arg2 + 0x10);
}

void func_002D4C80(s32 arg0, u32 arg1, s32 arg2) {
    if (arg2 == 0) {
        func_002D45F0(arg1, arg0 + 0x180, 1);
        return;
    }
    func_002D45F0(arg1, arg0 + 400, 1);
}

void func_002D4CC8(s32 arg0, u32 arg1, s32 arg2) {
    if (arg2 == 0) {
        func_002D45F0(arg1, arg0 + 0x70, 1);
        return;
    }
    func_002D45F0(arg1, arg0 + 0xb0, 1);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4D10);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4D70);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4DD0);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4E70);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4EE8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4FE8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5000);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5018);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5498);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5510);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5558);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D55B8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D55E0);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5608);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5668);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D56C8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D56F0);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5718);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5778);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D57D8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5800);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5828);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5888);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D58E8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5910);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5938);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5998);

void func_002D59F8(u64 *arg0) {
    *arg0 = 0;
    arg0[1] = 0x3f;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5A08);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5A68);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5B18);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5B70);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5C90);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5CD0);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5DF8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5EB0);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5FD8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6080);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6188);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6258);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6380);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6450);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6578);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6690);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D67E8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D68D8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6A40);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6B98);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6D40);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6E80);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7008);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D71B8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7390);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7410);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7500);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7580);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7670);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7720);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7810);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7830);

void func_002D78B8(s32 arg0) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x30) == 0) {
        temp_v0 = func_002CFEB8(0x100);
        *(u32 *)(arg0 + 0x30) = temp_v0;
    }
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D78F0);

void func_002D7988(u32 arg0) {
    func_002D78F0();
    func_002CFF98(*(u32 *)((s32)arg0 + 0x30));
    *(u32 *)((s32)arg0 + 0x30) = 0;
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D79C0);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7A50);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7AC8);

void func_002D7B50(u32 *arg0) {
    func_002E7730(*arg0);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7B68);

void func_002D7BD8(s32 *arg0, u32 arg1, u32 arg2) {
    s16 temp_v0;
    s32 temp_v1;
    s32 temp_v2;
    s32 temp_v3;

    temp_v2 = *arg0;
    temp_v0 = *(s16 *)(temp_v2 + 4);
    temp_v3 = temp_v0 + 1;
    if ((s64)*(s16 *)(temp_v2 + 6) < (s64)temp_v3) {
        func_002E76B0(temp_v2);
        temp_v2 = *arg0;
    }
    temp_v1 = *(s32 *)(temp_v2 + 0xc);
    *(s32 **)((s32)arg2 + 0x10) = arg0;
    *(s16 *)(temp_v2 + 4) = (s16)temp_v3;
    *(s32 *)(temp_v0 * 4 + temp_v1) = (s32)arg2;
    func_002D7CD0(arg2, arg1);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7C68);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7CD0);
