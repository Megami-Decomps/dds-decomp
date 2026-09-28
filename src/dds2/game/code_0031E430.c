#include "common.h"

extern u64 func_003283E0(u64);

extern u64 func_0035A828(u64);

extern u32 D_004390D8;

extern u8 *D_004389B0;

extern u8 *D_004389AC;

extern u8 *D_004389A8;

extern u8 *D_004389A4;

extern u8 *D_004389A0;

extern u32 D_004390C8;

extern u32 D_004390CC;

extern u32 D_004390E4;

extern u32 D_004390E8;

extern u32 D_004390DC;

extern u32 D_004390E0;

extern u32 D_004390D0;

extern u32 D_004390D4;

extern u32 D_004390C0;

extern u32 D_004390C4;

extern u8 D_004390B8;

extern u64 func_00325BB0(u64, u32);

extern u64 func_00325AB8(u64, u32);

extern u64 func_00320AE8(u64, u64, u32 *);

extern u64 func_00325790(u64, u32);

extern u64 func_0031F0E8(void);

extern s32 func_0031E550(void);

extern u32 D_0043895C;

extern s32 CancelWakeupThread(u64);

extern u64 GetThreadId(void);

extern s32 CreateSema(void *);

extern u32 D_004389BC;

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031E430);

void func_0031E4D0(u32 arg0) {
    D_0043895C = arg0;
}

void func_0031E4D8(void) {
    D_0043895C = 0;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031E4E0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031E530);

u32 func_0031E548(s32 arg0) {
    return *(u32 *)(arg0 + 0x14);
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031E550);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031E578);

void func_0031E640(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = func_0031E550();
    if (temp_v0 != 0) {
        temp_v1 = (s32)arg0;
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 7, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 8, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 9, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 10, 0x54);
        func_0031E578(arg0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031E6E0);

void func_0031E7C8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

void func_0031E7D0(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = func_0031E550();
    if (temp_v0 != 0) {
        temp_v1 = (s32)arg0;
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 3, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 2, 0x54);
        func_0031E6E0(0xe0, 0x2a8, *(u32 *)(temp_v1 + 0x14), *(u32 *)(temp_v1 + 0x18));
        func_0031E578(arg0);
        return;
    }
}

void func_0031E850(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

void func_0031E858(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = func_0031E550();
    if (temp_v0 != 0) {
        temp_v1 = (s32)arg0;
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 1, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 0, 0x54);
        func_0031E6E0(0xe0, 0x118, *(u32 *)(temp_v1 + 0x14), *(u32 *)(temp_v1 + 0x18));
        func_0031E578(arg0);
        return;
    }
}

void func_0031E8D8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x1c) = arg1;
}

void func_0031E8E0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

void func_0031E8E8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031E8F0);

void func_0031ED68(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031ED70);

void func_0031EE28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031EE30);

void func_0031EEE8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031EEF0);

void func_0031EFB8(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = func_0031E550();
    if (temp_v0 != 0) {
        temp_v1 = (s32)arg0;
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 0x1a, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 0x23, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 0x24, 0x54);
        func_0031E578(arg0);
        return;
    }
}

void func_0031F040(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

void func_0031F048(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = func_0031E550();
    if (temp_v0 == 0) {
        return;
    }
    temp_v1 = (s32)arg0;
    func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 6, 0x54);
    if (*(s32 *)(temp_v1 + 0x18) != 1) {
        if (*(s32 *)(temp_v1 + 0x18) != 2) goto LAB_0031f0c4;
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 5, 0x54);
    }
    func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 4, 0x54);
LAB_0031f0c4:
    func_0031E578(arg0);
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F0E8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F138);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F168);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F1B8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F1E8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F208);

u64 func_0031F228(s32 arg0) {
    u64 temp_v0;

    temp_v0 = func_0031F0E8();
    func_00320CE0(*(u32 *)(arg0 + 4), 0, temp_v0);
    return temp_v0;
}

u32 func_0031F270(s32 arg0) {
    return *(u32 *)(*(s32 *)(*(s32 *)(arg0 + 4) + 8) + 0x10);
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F280);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F300);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F340);

void func_0031F410(u32 arg0, u32 arg1) {
    u32 temp_v0 [4];

    temp_v0[0] = arg1;
    func_0031F340(arg0, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F430);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F4D8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F550);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F5C0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F618);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F6A0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F708);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F778);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F840);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031F878);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031FA60);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00320020);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003201A0);

u32 func_00320380(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00320388);

u64 func_00320510(u64 arg0, u64 arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00320AE8(arg0, arg1, temp_v2);
    temp_v1 = func_00325790(temp_v0, temp_v2[0]);
    func_0035A880(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00320560);

u64 func_003206E8(u64 arg0, u64 arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00320AE8(arg0, arg1, temp_v2);
    temp_v1 = func_00325790(temp_v0, temp_v2[0]);
    func_0035A880(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00320738);

u64 func_003208C0(u64 arg0, u64 arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00320AE8(arg0, arg1, temp_v2);
    temp_v1 = func_00325AB8(temp_v0, temp_v2[0]);
    func_0035A880(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00320910);

u64 func_00320A98(u64 arg0, u64 arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00320AE8(arg0, arg1, temp_v2);
    temp_v1 = func_00325BB0(temp_v0, temp_v2[0]);
    func_0035A880(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00320AE8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00320C28);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00320C88);

void func_00320CD0(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x14) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00320CE0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00320D80);

void func_00320EA8(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x10) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00320EB8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00320F68);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00320FD0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321018);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321090);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321130);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321170);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003211B0);

void func_003211F0(void) {
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_003211F8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321208);

u8 * func_00321238(void) {
    return &D_004390B8;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321248);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321258);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003212A8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321308);

void func_00321318(u32 arg0, u32 arg1) {
    D_004390C0 = arg0;
    D_004390C4 = arg1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321328);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321340);

void func_003214C0(void) {
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_003214C8);

void func_003214D0(u32 arg0, s32 arg1) {
    if (arg1 != 0) {
        func_00321908(arg1);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321500);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321528);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321688);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003216A8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321798);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003218A0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321908);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321928);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003219F0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321A30);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321C60);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321E18);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321E70);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321EC8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321ED8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321EE8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321F18);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321F78);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00321F98);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003223F8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00322418);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00322438);

s32 func_00322480(s32 *arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = arg1 << 3;
    if (0 < arg1) {
        do {
            temp_v0 = *arg0;
            arg0 = arg0 + 2;
            arg1 = arg1 - 1;
            temp_v1 = temp_v1 + temp_v0 * 8;
        } while (arg1 != 0);
    }
    return temp_v1;
}

void func_003224B0(void) {
}

void func_003224B8(void) {
}

void func_003224C0(void) {
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_003224C8);

void func_003224E0(u32 arg0, u32 arg1) {
    D_004390D0 = arg0;
    D_004390D4 = arg1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_003224F0);

void func_00322510(u32 arg0, u32 arg1) {
    D_004390DC = arg0;
    D_004390E0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00322520);

void func_00322540(u32 arg0, u32 arg1) {
    D_004390E4 = arg0;
    D_004390E8 = arg1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00322550);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00322570);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003225C0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00322610);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00322670);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003226D8);

void func_00322D08(u32 arg0, u32 arg1) {
    D_004390C8 = arg0;
    D_004390CC = arg1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00322D18);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00322D50);

u32 func_00322D98(void) {
    return D_004390C8;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00322DA0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00322E18);

void func_00322F00(s32 arg0) {
    *(u32 *)(arg0 + 0x40) = *(u32 *)(arg0 + 0x40) & 0xfffffffe;
    if (*(s32 *)(arg0 + 0x3c) != 0) {
        func_00320C88(*(s32 *)(arg0 + 0x3c));
        *(u32 *)(arg0 + 0x3c) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00322F48);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003230A0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003232A0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003233E8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003236B0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00323748);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003238A0);

void func_00323918(u8 *arg0) {
    D_004389A0 = arg0;
}

void func_00323920(u8 *arg0) {
    D_004389A4 = arg0;
}

void func_00323928(u8 *arg0) {
    D_004389A8 = arg0;
}

void func_00323930(u8 *arg0) {
    D_004389AC = arg0;
}

void func_00323938(u8 *arg0) {
    D_004389B0 = arg0;
}

u32 func_00323940(s32 arg0, s32 arg1) {
    if (*(s16 *)(arg0 + 0x36) - arg1 < 1) {
        *(u16 *)(arg0 + 0x36) = 0;
        *(u32 *)(arg0 + 0x40) = *(u32 *)(arg0 + 0x40) | 4;
        return 1;
    }
    *(s16 *)(arg0 + 0x36) = *(s16 *)(arg0 + 0x36) - (s16)arg1;
    *(u32 *)(arg0 + 0x40) = *(u32 *)(arg0 + 0x40) | 2;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00323988);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00323BB8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00323DF0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00324070);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00324238);

u32 func_00324268(void) {
    return D_004390D8;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00324270);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003242D0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00324840);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00324AC0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00324B28);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00324C98);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00324D28);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00324D50);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00324DB8);

void func_00324DF8(u32 *arg0, u32 arg1) {
    func_00320CE0(*arg0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00324E18);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00324E80);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00324EF0);

void func_00324F20(u32 *arg0) {
    func_003211B0(*arg0);
}

void func_00324F38(u32 *arg0) {
    func_00321170(*arg0);
}

u64 func_00324F50(s32 arg0, u64 arg1) {
    u64 temp_v0;

    temp_v0 = func_0035A828(arg1);
    func_00320CE0(*(u32 *)(arg0 + 4), 0, temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00324F98);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00324FD0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003251C0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00325398);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003255A0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00325688);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00325790);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00325AB8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00325BB0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00325CC8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00325EC8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00326018);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00326158);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003262A8);

void func_003268A8(float *arg0, float *arg1) {
    *arg0 = *arg0 + *arg1;
    arg0[1] = arg0[1] + arg1[1];
    arg0[2] = arg0[2] + arg1[2];
}

void func_003268E0(float *arg0, float *arg1) {
    *arg0 = *arg0 - *arg1;
    arg0[1] = arg0[1] - arg1[1];
    arg0[2] = arg0[2] - arg1[2];
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00326918);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00326940);

void func_00326950(float arg0, float *arg1) {
    *arg1 = *arg1 * arg0;
    arg1[1] = arg1[1] * arg0;
    arg1[2] = arg1[2] * arg0;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00326978);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003269F0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00326A40);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00326AE0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00326B00);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00326BC8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003270C8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003275C8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00327AC8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00327BD8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00327C80);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00328018);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00328160);

void func_003282E8(void) {
}

s32 func_003282F0(u32 initial, u32 option, u32 maximum) {
    struct {
        u32 attr;
        u32 option;
        u32 initial;
        u32 reserved[2];
        u32 maximum;
    } sema;

    sema.initial = initial;
    sema.option = option;
    sema.maximum = maximum;
    return CreateSema(&sema);
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00328318);

void func_00328390(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_003283E0(arg1);
    func_00328318(arg0, temp_v0, arg1, arg2);
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_003283E0);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00328420);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00328470);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003284C8);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00328520);

INCLUDE_ASM(const s32, "game/code_0031E430", sdfAddHandler);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00328668);

void func_00328748(void) {
    u32 current;
    while ((current = D_004389BC) != 0) {
        func_00328668(current);
    }
}

void func_00328778(u32 arg0, u32 arg1, u32 arg2) {
    iWakeupThread(arg2);
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00328790);

INCLUDE_ASM(const s32, "game/code_0031E430", func_003287E0);

u32 func_003287F0(u32 base) {
    u32 now;

    now = *(volatile u32 *)0x10000000;
    return (now - base) & 0xFFFF;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_00328808);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00328858);

INCLUDE_ASM(const s32, "game/code_0031E430", func_00328918);

void func_00328988(s32 arg0) {
    u64 temp_v0;
    s32 temp_v1;

    temp_v0 = GetThreadId();
    temp_v1 = CancelWakeupThread(temp_v0);
    arg0 = arg0 - temp_v1;
    do {
        arg0 = arg0 - 1;
        SleepThread();
    } while (0 < arg0);
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_003289C8);
INCLUDE_SDATA(const s32, "game/code_0031E430", D_0043895C);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_00438960);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_00438968);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_00438970);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_00438978);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_00438980);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_00438988);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_00438990);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_00438998);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_0043899C);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_004389A0);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_004389A4);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_004389A8);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_004389AC);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_004389B0);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_004389B4);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_004389B8);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_004389BC);

INCLUDE_SDATA(const s32, "game/code_0031E430", D_004389C0);


INCLUDE_SDATA(const s32, "game/code_0031E430", D_004389C4);

