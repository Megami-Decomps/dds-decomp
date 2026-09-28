#include "common.h"

extern s32 D_00360348[];
extern s32 D_0035FFE0[];
extern s32 D_003BB6B8;
extern s32 D_003BD854;
extern s32 func_002EB028(s32, u32 *, s32);
extern void btlCmdSimpleB(s32, u16);
extern void btlCmdSimpleA(s32, u16);
extern void btlCmdSimpleE(s32, u16);
extern void btlCmdSimpleD(s32, u16);
extern void btlCmdSimpleJ(s32, u16);
extern void func_001FF030(s32, u16);

extern s8 D_003BB870;

extern s8 D_003A5440[];

extern s8 D_003A5460[];

extern s8 D_003A5488[];

extern u8 D_003BB818[];

extern u8 D_003BD476;

extern void *func_002CFEB8(s32 size);

extern s32 sceDopen(char *);

extern s32 D_003BD868;

extern s8 D_003A54A8[];

extern void func_001FB0A8(s32, ...);

extern s8 D_003A54C8[];

extern s8 D_003A54F0[];

extern s8 D_003A5518[];

extern s8 D_003A5538[];

extern s8 D_003A5558[];

extern u32 D_003BB6C8;

extern void func_003014F0();

extern u8 D_003BB820[];

extern u64 func_001D9718(void);

extern u64 func_001D9780(void);

extern u64 func_001DB930(u64, u64);

extern s32 D_003BB3D8;

extern u32 func_0020A3F0(void);

extern u32 func_00207BB0(void);

extern u32 func_00207B28(void);

extern u32 func_002099A0(void);

extern u32 func_00208C68(void);

extern u64 func_001ACAE0(void);

extern u16 func_0010D428(u32);

extern s32 func_0010D6A8(void);

extern u32 D_003BD858;

extern u32 func_0029BF88(u32, u32);

extern s32 func_001A17F0(void);
extern s32 func_001FEC68(s32 context, s32 actor, u32 mask);
extern void func_0010D5F0();
extern void btlCmdSimpleC(s32, u16);
extern u8 *func_001D4748(s32);
extern void func_001F60E8(void);

u8 *battleCreateControlObject(void) {
    u8 *object;
    object = func_001D4748(0);
    object[0] = 1;
    *(void (**)(void))(object + 0x4c) = func_001F60E8;
    *(u16 *)(object + 0x20) = 0x60;
    *(s32 *)(object + 0x48) = 0;
    object[0x10] = 0;
    return object;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6158);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6300);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6498);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6510);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6588);

f32 func_001F6618(s32 arg0) {
    f32 temp_f2;
    f32 temp_f1;

    temp_f2 = *(f32 *)(arg0 + 0xb4);
    temp_f1 = *(f32 *)(arg0 + 0xb0);
    if (temp_f1 < temp_f2) {
        return temp_f2 * *(f32 *)(arg0 + 0x80);
    }
    return temp_f1 * *(f32 *)(arg0 + 0x80);
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6640);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6688);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F66D8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6970);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6C08);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6CB0);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6D40);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6E28);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F70C8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F71E8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F72C8);

void func_001F73E0(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    for (temp_v0 = *(s32 *)(temp_v0 + 0x228); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0x344)) {
        func_001D5440(temp_v0);
    }
}

void func_001F7428(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    for (temp_v0 = *(s32 *)(temp_v0 + 0x228); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0x344)) {
        func_001D54C0(temp_v0);
    }
}

void func_001F7470(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(func_001A17F0() + 0x228);
    if (temp_v0 != 0) {
        do {
            if (*(s32 *)(temp_v0 + 0x110) & arg0) {
                func_001D5440(temp_v0);
            }
            temp_v0 = *(s32 *)(temp_v0 + 0x344);
        } while (temp_v0 != 0);
    }
}

void func_001F74D0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(func_001A17F0() + 0x228);
    if (temp_v0 != 0) {
        do {
            if (*(s32 *)(temp_v0 + 0x110) & arg0) {
                func_001D54C0(temp_v0);
            }
            temp_v0 = *(s32 *)(temp_v0 + 0x344);
        } while (temp_v0 != 0);
    }
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7530);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7598);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7600);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F76F0);

s32 func_001F7770(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1 = 0;
    s32 temp_v2;

    temp_v0 = *(s32 *)(func_001A17F0() + 0x228);
    if (temp_v0 != 0) {
        do {
            temp_v2 = *(s32 *)(temp_v0 + 0x110);
            if (((temp_v2 & arg0) != 0) && ((temp_v2 & 0x20) == 0)) {
                temp_v1 += temp_v2 & 1;
            }
            temp_v0 = *(s32 *)(temp_v0 + 0x344);
        } while (temp_v0 != 0);
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F77D8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7868);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7940);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F79A0);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7A00);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7A70);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7AE0);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7BB0);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7C68);

void func_001F7CC8(s32 arg0, f32 arg1) {
    *(f32 *)(arg0 + 0) = arg1;
    *(s32 *)(arg0 + 4) = 0;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7CD8);

void func_001F7D30(s32 arg0, f32 arg1) {
    f32 temp_f0;

    *(f32 *)(arg0 + 0xc) = 0.0f;
    *(f32 *)(arg0 + 0) = arg1;
    temp_f0 = *(f32 *)(arg0 + 0xc);
    *(f32 *)(arg0 + 4) = arg1;
    *(f32 *)(arg0 + 0x10) = temp_f0;
    if (arg1 == temp_f0) {
        return;
    }
    *(f32 *)(arg0 + 0x8) = 1.0f / (arg1 * arg1 * 0.25f);
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7D80);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7DF8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7F70);

void func_001F8280(void) {
    func_001FB0A8((s32)"btl:[%s]\n", D_003BB6B8);
    D_003BD854 = func_002EB028(D_003BB6B8, &D_003BD858, 0);
}

void func_001F82B8(void) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v0 = func_001A17F0();
    temp_v1 = func_0029BF88(D_003BD858, 0x10000);
    *(u32 *)(temp_v0 + 0x4b4) = temp_v1;
}

void func_001F82F0(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    func_0029BFB0(*(u32 *)(temp_v0 + 0x4b4));
    *(u32 *)(temp_v0 + 0x4b4) = 0;
}

u32 func_001F8328(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    *(u32 *)(temp_v0 + 0x20) = 1;
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

u32 func_001F8358(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    *(u32 *)(temp_v0 + 0x20) = 6;
    *(u32 *)(temp_v0 + 0x24) = 0xc2;
    return 1;
}

u32 func_001F8388(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    *(u32 *)(temp_v0 + 0x24) = 1;
    *(u32 *)(temp_v0 + 0x20) = 0xd;
    return 1;
}

u32 func_001F83B8(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    *(u32 *)(temp_v0 + 0x24) = 1;
    *(u32 *)(temp_v0 + 0x20) = 10;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F83E8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F84A8);

u32 func_001F8508(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    *(u32 *)(temp_v0 + 0xc) = *(u32 *)(temp_v0 + 0xc) | 1;
    return 1;
}

u32 func_001F8538(void) {
    u16 temp_v0;
    s32 temp_v1;

    temp_v1 = func_0010D6A8();
    temp_v0 = func_0010D428(0);
    *(u16 *)(*(s32 *)(temp_v1 + 0x18) + 0x122) = temp_v0;
    return 1;
}

u32 func_001F8578(void) {
    s32 temp_v0;
    s16 temp_v1;

    temp_v0 = func_001A17F0();
    func_0010D6A8();
    temp_v1 = func_0010D428(0);
    *(u8 *)(temp_v0 + 0x25e) = 4;
    *(s32 *)(temp_v0 + 0x280) = temp_v1;
    return 1;
}

u32 func_001F85C8(void) {
    btlCmdWithArgA(func_0010D6A8());
    return 1;
}

u32 func_001F85F0(void) {
    s32 context = func_0010D6A8();
    u16 value = func_0010D428(0);
    btlCmdSimpleB(context, value);
    return 1;
}

u32 func_001F8630(void) {
    btlCmdWithArgB(func_0010D6A8());
    return 1;
}

u32 func_001F8658(void) {
    btlCmdWithArgC(func_0010D6A8());
    return 1;
}

u32 func_001F8680(void) {
    s32 context = func_0010D6A8();
    u16 value = func_0010D428(0);
    btlCmdSimpleA(context, value);
    return 1;
}

u32 func_001F86C0(void) {
    btlCmdWithArgE(func_0010D6A8());
    return 1;
}

u32 func_001F86E8(void) {
    btlCmdWithArgD(func_0010D6A8());
    return 1;
}

u32 func_001F8710(void) {
    s32 context = func_0010D6A8();
    u16 value = func_0010D428(0);
    btlCmdSimpleC(context, value);
    return 1;
}

u32 func_001F8750(void) {
    s32 context = func_0010D6A8();
    u16 value = func_0010D428(0);
    btlCmdSimpleD(context, value);
    return 1;
}

u32 func_001F8790(void) {
    s32 context = func_0010D6A8();
    u16 value = func_0010D428(0);
    btlCmdSimpleE(context, value);
    return 1;
}

u32 func_001F87D0(void) {
    btlCmdWithArgA(func_0010D6A8());
    return 1;
}

u32 func_001F87F8(void) {
    btlCmdSimpleF(func_0010D6A8(), 0);
    return 1;
}

u32 func_001F8828(void) {
    btlCmdSimpleG(func_0010D6A8(), 0);
    return 1;
}

u32 func_001F8858(void) {
    btlCmdSimpleH(func_0010D6A8(), 0);
    return 1;
}

u32 func_001F8888(void) {
    btlCmdWithArgF(func_0010D6A8());
    return 1;
}

u32 func_001F88B0(void) {
    s32 context = func_0010D6A8();
    u16 value = func_0010D428(0);
    btlCmdSimpleJ(context, value);
    return 1;
}

u32 func_001F88F0(void) {
    btlCmdSimpleI(func_0010D6A8(), 0);
    return 1;
}

u32 nbScriptCheckActorFlag(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 1;
        *(s32 *)(context + 0x98) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~1;
    }
    return 1;
}

u32 func_001F89B0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x6000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 2;
        *(s32 *)(context + 0x9C) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~2;
    }
    return 1;
}

u32 func_001F8A40(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 4;
        *(s32 *)(context + 0xA0) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~4;
    }
    return 1;
}
u32 func_001F8AD0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x1400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 8;
        *(s32 *)(context + 0xA4) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~8;
    }
    return 1;
}
u32 func_001F8B60(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x1800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x10;
        *(s32 *)(context + 0xA8) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x10;
    }
    return 1;
}
u32 func_001F8BF0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x1C00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x20;
        *(s32 *)(context + 0xAC) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x20;
    }
    return 1;
}
u32 func_001F8C80(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x2000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x40;
        *(s32 *)(context + 0xB0) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x40;
    }
    return 1;
}
u32 func_001F8D10(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x2400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x80;
        *(s32 *)(context + 0xB4) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x80;
    }
    return 1;
}
u32 func_001F8DA0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x2800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x100;
        *(s32 *)(context + 0xB8) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x100;
    }
    return 1;
}
u32 func_001F8E30(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x2C00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x200;
        *(s32 *)(context + 0xBC) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x200;
    }
    return 1;
}
u32 func_001F8EC0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x3000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x400;
        *(s32 *)(context + 0xC0) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x400;
    }
    return 1;
}
u32 func_001F8F50(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x3400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x800;
        *(s32 *)(context + 0xC4) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x800;
    }
    return 1;
}
u32 func_001F8FE0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x3800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x1000;
        *(s32 *)(context + 0xC8) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x1000;
    }
    return 1;
}

u32 func_001F9070(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), 0x4800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x2000;
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x2000;
    }
    return 1;
}

u32 func_001F90E8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), 0x5C00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x2000000;
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x2000000;
    }
    return 1;
}

u32 func_001F9168(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x4C00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x4000;
        *(s32 *)(context + 0xD0) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x4000;
    }
    return 1;
}
u32 func_001F91F8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x5000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x4000;
        *(s32 *)(context + 0xD0) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x4000;
    }
    return 1;
}

u32 func_001F9288(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    if (func_001FEC68(temp_v0, *(s32 *)(temp_v0 + 0x18), 0x6c00000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F92D8(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    if (func_001FEC68(temp_v0, *(s32 *)(temp_v0 + 0x18), 0x9400000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9328(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    if (func_001FEC68(temp_v0, *(s32 *)(temp_v0 + 0x18), 0x9c00000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9378(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x8C00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x40000;
        *(s32 *)(context + 0xE0) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x40000;
    }
    return 1;
}
u32 func_001F9410(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x9000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x80000;
        *(s32 *)(context + 0xE4) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x80000;
    }
    return 1;
}

u32 func_001F94A8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x10400000)) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9518(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x10800000)) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9588(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xA000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x100000;
        *(s32 *)(context + 0xE8) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x100000;
    }
    return 1;
}
u32 func_001F9620(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xA400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x200000;
        *(s32 *)(context + 0xEC) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x200000;
    }
    return 1;
}
u32 func_001F96B8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xA800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x400000;
        *(s32 *)(context + 0xF0) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x400000;
    }
    return 1;
}
u32 func_001F9750(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xAC00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x800000;
        *(s32 *)(context + 0xF4) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x800000;
    }
    return 1;
}

u32 func_001F97E8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xB800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 1;
        *(s32 *)(context + 0x104) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~1;
    }
    return 1;
}
u32 func_001F9878(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xBC00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 2;
        *(s32 *)(context + 0x108) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~2;
    }
    return 1;
}
u32 func_001F9908(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xC000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 4;
        *(s32 *)(context + 0x10C) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~4;
    }
    return 1;
}
u32 func_001F9998(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xC400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 8;
        *(s32 *)(context + 0x110) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~8;
    }
    return 1;
}
u32 func_001F9A28(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xC800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x10;
        *(s32 *)(context + 0x114) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x10;
    }
    return 1;
}
u32 func_001F9AB8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xCC00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x20;
        *(s32 *)(context + 0x118) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x20;
    }
    return 1;
}
u32 func_001F9B48(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xD400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x400;
        *(s32 *)(context + 0x12C) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x400;
    }
    return 1;
}
u32 func_001F9BD8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xD800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x800;
        *(s32 *)(context + 0x130) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x800;
    }
    return 1;
}

u32 func_001F9C68(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x10C00000)) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9CD8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x11000000)) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9D48(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x9800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x40;
        *(s32 *)(context + 0x11C) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x40;
    }
    return 1;
}
u32 func_001F9DD8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xDC00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x1000;
        *(s32 *)(context + 0x134) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x1000;
    }
    return 1;
}

u32 func_001F9E68(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), 0xD000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x80;
        *(s32 *)(context + 0x120) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x80;
    }
    return 1;
}

u32 func_001F9EF0(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    if (func_001FEC68(temp_v0, *(s32 *)(temp_v0 + 0x18), 0xe800000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9F40(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    if (func_001FEC68(temp_v0, *(s32 *)(temp_v0 + 0x18), 0xec00000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9F90(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), 0xB000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x1000000;
        *(s32 *)(context + 0xF8) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x1000000;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FA018);

u32 func_001FA0A0(void) {
    if (func_002099E0() != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001FA0E0(void) {
    if (func_00207B68() != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001FA120(void) {
    if (func_00207C18() != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FA160);

u32 func_001FA270(void) {
    if (func_0020A418() != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FA2B0);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FA340);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FA3D0);

u32 func_001FA460(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    if (func_001FEC68(temp_v0, *(s32 *)(temp_v0 + 0x18), 0x10000000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001FA4B0(void) {
    u64 temp_v0;

    temp_v0 = func_001ACAE0();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_001FA4D8(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    func_0010D5F0(*(u32 *)(temp_v0 + 0x250));
    return 1;
}

u32 func_001FA500(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    func_0010D5F0(*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 0x134));
    return 1;
}

u32 func_001FA530(void) {
    u32 temp_v0;

    temp_v0 = func_00208C68();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_001FA558(void) {
    u32 temp_v0;

    temp_v0 = func_002099A0();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_001FA580(void) {
    u32 temp_v0;

    temp_v0 = func_00207B28();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_001FA5A8(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    func_0010D5F0(*(u16 *)(temp_v0 + 0x25c));
    return 1;
}

u32 func_001FA5D0(void) {
    u32 temp_v0;

    temp_v0 = func_00207BB0();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_001FA5F8(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    func_0010D5F0(*(s8 *)(temp_v0 + 0x146));
    return 1;
}

u32 func_001FA620(void) {
    func_0010D5F0(D_003BB870);
    return 1;
}

u32 func_001FA648(void) {
    u32 temp_v0;

    temp_v0 = func_0020A3F0();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_001FA670(void) {
    func_001FED20(func_0010D6A8());
    return 1;
}

u32 func_001FA698(void) {
    s32 context = func_0010D6A8();
    u16 value = func_0010D428(0);
    func_001FF030(context, value);
    return 1;
}

u32 func_001FA6D8(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = func_0010D6A8();
    temp_v0 = D_003BB3D8;
    *(u32 *)(temp_v1 + 0x20) = 0x10;
    *(u32 *)(temp_v1 + 0x24) = 0;
    *(u8 *)(temp_v0 + 0x54) = 1;
    return 1;
}

u32 func_001FA718(void) {
    func_001C44D0();
    return 1;
}

u32 func_001FA738(void) {
    func_001C4490();
    return 1;
}

u32 func_001FA758(void) {
    func_001F73E0();
    return 1;
}

u32 func_001FA778(void) {
    func_001F7428();
    func_001F7470(0x200);
    return 1;
}

u32 func_001FA7A0(void) {
    func_001F7428();
    func_001F7470(0x400);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FA7C8);

u32 func_001FA898(void) {
    u64 temp_v0;

    temp_v0 = func_001D9718();
    func_001D4860(temp_v0);
    temp_v0 = func_001D9780();
    func_001D4860(temp_v0);
    temp_v0 = func_001DB930(func_0010D6A8(), 0x11);
    func_001D4860(temp_v0);
    return 1;
}

u32 func_001FA8F0(void) {
    u64 temp_v0;

    temp_v0 = func_001D9718();
    func_001D4860(temp_v0);
    temp_v0 = func_001D9780();
    func_001D4860(temp_v0);
    temp_v0 = func_001DB930(0, 3);
    func_001D4860(temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FA940);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FA9C8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FAA50);

u32 func_001FAB38(void) {
    func_001FB0A8(D_003A5440);
    func_0010D5F0(0);
    return 1;
}

u32 func_001FAB68(void) {
    func_001FB0A8(D_003A5460);
    func_0010D5F0(0x14);
    return 1;
}

u32 func_001FAB98(void) {
    func_0010D6A8();
    return 1;
}

u32 func_001FABB8(void) {
    func_001FB0A8(D_003A5488);
    func_0010D5F0(0);
    return 1;
}

u32 func_001FABE8(void) {
    func_001FB0A8(D_003A54A8);
    return 1;
}

u32 func_001FAC10(void) {
    func_001FB0A8(D_003A54C8);
    return 1;
}

u32 func_001FAC38(void) {
    func_001FB0A8(D_003A54F0);
    return 1;
}

u32 func_001FAC60(void) {
    func_001FB0A8(D_003A5518);
    return 1;
}

u32 func_001FAC88(void) {
    func_001FB0A8(D_003A5538);
    return 1;
}

u32 func_001FACB0(void) {
    func_001FB0A8(D_003A5558);
    return 1;
}

u32 func_001FACD8(void) {
    func_00204FE0();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FACF8);

void func_001FADA8(void) {
}

void func_001FADB0(void) {
}

void func_001FADB8(void) {
}

u32 func_001FADC0(void) {
    D_003BB6C8 = D_003BB6C8 | 0x2000000;
    return 0;
}

u32 func_001FADD8(u32 arg0) {
    return arg0;
}

void func_001FADE0(void) {
}

void func_001FADE8(void) {
}

void func_001FADF0(void) {
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FADF8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FAE78);

void func_001FB050(void) {
    D_00360348[4] &= ~0x80;
    D_0035FFE0[4] &= ~0x80;
}

void func_001FB080(void) {
}

void func_001FB088(void) {
}

void func_001FB090(void) {
}

void func_001FB098(void) {
}

void func_001FB0A0(void) {
}

void func_001FB0A8(s32 arg0, ...) {
}

void func_001FB0F0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, ...) {
}

void func_001FB130(void) {
}

void func_001FB138(void) {
}

void func_001FB140(void) {
}

void func_001FB148(void) {
}

void func_001FB150(void) {
}

void func_001FB158(void) {
}

void func_001FB160(void) {
}

void func_001FB168(void) {
}

void func_001FB170(void) {
}

void func_001FB178(void) {
}

void func_001FB180(void) {
}

void func_001FB188(void) {
}

void func_001FB190(void) {
}

void func_001FB198(void) {
}

void func_001FB1A0(void) {
}

void func_001FB1A8(void) {
}

void func_001FB1B0(void) {
}

void func_001FB1B8(void) {
}

void func_001FB1C0(void) {
}

void func_001FB1C8(void) {
}

void func_001FB1D0(void) {
}

void func_001FB1D8(void) {
}

void func_001FB1E0(void) {
}

void func_001FB1E8(void) {
}

void func_001FB1F0(void) {
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FB1F8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FB240);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5440);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5460);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5488);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A54A8);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A54C8);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A54F0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5518);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5538);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5558);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5580);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5590);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55A0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55B0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55C0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55D0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55E0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55F0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5600);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5610);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5620);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5630);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5640);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5650);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5660);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5670);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5680);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5690);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56A0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56B0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56C0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56D0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56E0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56F0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5700);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5710);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5720);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5730);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5740);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5750);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5760);

s32 func_001FB2A0(s32 arg0) {
    char buf[0x70];

    if (D_003BD476 != 0) {
        func_003014F0(buf, "pfs0:/%s", arg0);
        return sceDopen(buf);
    }
    D_003BD868 = 0;
    return 0;
}

void func_001FB2F0(void) {
    if (D_003BD476 == 0) {
        return;
    }
    func_003101B8();
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FB320);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FB3C8);

void func_001FB870(s32 arg0) {
    s32 temp_v0;

    if (*(s32 *)(arg0 + 8) != 0) {
        temp_v0 = *(s32 *)(arg0 + 8);
        do {
            s32 temp_v1 = *(s32 *)(temp_v0 + 0x40);
            func_002CFF98(temp_v0);
            temp_v0 = temp_v1;
        } while (temp_v0 != 0);
    }
    func_002CFF98(*(s32 *)(arg0 + 4));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FB8C8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FB9A8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FBA38);

void func_001FBEE8(s32 resource) {
    s32 handle = *(s32 *)(resource + 0x3c);
    if (handle != 0 && *(s32 *)(resource + 0x40) == 1) {
        func_002D2D00(handle);
    }
    func_002CFF98(resource);
}

void func_001FBF30(s32 arg0, s32 arg1, s32 arg2) {
    *(s32 *)(arg0 + 0) = arg1;
    *(s32 *)(arg0 + 4) = arg2;
}

u32 func_001FBF40(s32 arg0) {
    return *(u32 *)(arg0 + 0x14);
}

u32 func_001FBF48(s32 arg0) {
    return *(u32 *)(arg0 + 8);
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FBF50);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FBFC8);

u32 func_001FC078(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x34) + 4);
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FC088);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FC100);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FC160);

s32 func_001FC280(s32 arg0) {
    s32 temp_v0;

    temp_v0 = (s32)func_002CFEB8(0x38);
    *(s32 *)(temp_v0 + 0x14) = 9;
    *(s32 *)(temp_v0 + 0) = 8;
    *(s32 *)(temp_v0 + 4) = 8;
    *(s32 *)(temp_v0 + 8) = 0;
    *(s32 *)(temp_v0 + 0x10) = 0;
    *(s32 *)(temp_v0 + 0xc) = 0;
    *(s32 *)(temp_v0 + 0x18) = 0;
    strcpy(temp_v0 + 0x1c, arg0);
    return temp_v0;
}

void func_001FC2E8(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FC300);

void func_001FC720(s32 arg0, s32 arg1, s32 arg2) {
    *(s32 *)(arg0 + 0) = arg1;
    *(s32 *)(arg0 + 4) = arg2;
}

u32 func_001FC730(s32 arg0) {
    return *(u32 *)(arg0 + 8);
}

void func_001FC738(char *arg0, char *arg1) {
    strcpy(arg0 + 0x21, arg1);
    *(s32 *)(arg0 + 0x10) = strlen(arg1);
}

void func_001FC778(s32 arg0, void *arg1) {
    func_003014F0(arg1, D_003BB818, arg0 + 0x21, arg0 + 0x1c);
}

void func_001FC7A8(s32 arg0, void *arg1) {
    func_003014F0(arg1, D_003BB820, arg0 + 0x21);
}

void func_001FC7D0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6B8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6C8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6CC);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6D0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6D4);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6D8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6E0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6E4);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6E8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6F0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6F8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB700);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB708);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB710);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB718);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB720);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB724);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB728);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB730);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB738);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB740);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB748);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB750);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB758);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB760);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB768);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB76C);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB770);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB778);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB780);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB788);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB790);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB798);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7A0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7A8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7B0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7B8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7C0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7C8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7D0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7D8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7E0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7F8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB800);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB808);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB810);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB818);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB820);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB828);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB830);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB838);

