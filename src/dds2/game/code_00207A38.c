#include "common.h"

extern s32 func_00211DE0(void);

extern s32 func_001AA6F8(void);

extern u32 D_00438F70;

extern u32 func_002DDCA0(u32, u32);

extern s32 func_0010D8D0(void);

extern u16 func_0010D650(u32);

extern u64 func_001B76F0(void);

extern u64 func_00220958(void);

extern s32 D_004367F4;

extern u64 func_001E66D8(void);

extern u64 func_001E6740(void);

extern u64 func_001E8B40(u64, u64);

extern u32 D_00436B00;

extern s32 D_00436AF0;

extern s32 D_00438F6C;

extern s32 func_00343ED0(s32, u32 *, s32);

extern void func_0020D128(s32, ...);

extern void btlCmdSimpleB(s32, u16);

extern void btlCmdSimpleA(s32, u16);

extern void btlCmdSimpleD(s32, u16);

extern void btlCmdSimpleE(s32, u16);

extern void btlCmdSimpleJ(s32, u16);

extern s8 D_00436CAC;

extern void func_002112C8(s32, u16);

extern s8 D_00419570[];

extern s8 D_00419590[];

extern s8 D_004195B8[];

extern s8 D_004195D8[];

extern s8 D_004195F8[];

extern s8 D_00419620[];

extern s8 D_00419648[];

extern s8 D_00419668[];

extern s8 D_00419688[];

extern s32 D_003BEA48[];

extern s32 D_003BE6E0[];

extern u8 D_00438B66;

extern s32 sceDopen(char *);

extern s32 D_00438F80;

extern void func_0035C860();

extern void *func_00328D68(s32 size);

extern u32 func_002290D0(void);

extern u32 func_00229158(void);

extern u32 func_002192D8(void);

extern void btlCmdSimpleC(s32, u16);

extern s32 func_00210EA0(s32 context, s32 actor, u32 mask);

extern void func_0010D818();

extern u8 *func_001E1468(s32);

extern void func_00207A10(void);

extern void func_001E21A0(s32 actor);

extern void func_001E2220(s32 actor);

extern u32 func_001E8058(s32 actor);

extern s32 func_001E8060(s32 actor, u32 index);
extern void func_0021F3E8(s32);
extern void func_00211108(s32);
extern void func_002110E8(s32, u16);
extern void func_00226558(u8);
extern s32 func_00221090(void);

INCLUDE_ASM(const s32, "game/code_00207A38", battleCreateControlObject);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00207A80);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00207C28);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00207DC0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00207E38);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00207EB0);

f32 func_00207F40(s32 arg0) {
    f32 temp_f2;
    f32 temp_f1;

    temp_f2 = *(f32 *)(arg0 + 0xb4);
    temp_f1 = *(f32 *)(arg0 + 0xb0);
    if (temp_f1 < temp_f2) {
        return temp_f2 * *(f32 *)(arg0 + 0x80);
    }
    return temp_f1 * *(f32 *)(arg0 + 0x80);
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00207F68);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00207FB0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208000);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208298);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208530);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002085D8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208668);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208750);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002089F0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208B10);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208BF0);

void func_00208D58(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    for (temp_v0 = *(s32 *)(temp_v0 + 0x24c); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0x364)) {
        func_001E21A0(temp_v0);
    }
}

void func_00208DA0(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    for (temp_v0 = *(s32 *)(temp_v0 + 0x24c); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0x364)) {
        func_001E2220(temp_v0);
    }
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208DE8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208E48);

void func_00208EA8(s32 actor) {
    u32 i = 0;
    u32 count = func_001E8058(actor);
    if (count != 0) {
        do {
            func_001E21A0(func_001E8060(actor, i));
            i++;
        } while (i < count);
    }
}

void func_00208F10(s32 actor) {
    u32 i = 0;
    u32 count = func_001E8058(actor);
    if (count != 0) {
        do {
            func_001E2220(func_001E8060(actor, i));
            i++;
        } while (i < count);
    }
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208F78);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00209078);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00209160);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002091C8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00209258);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00209330);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00209390);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002093F0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00209460);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002094D0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002095A0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00209658);

void func_002096B8(s32 arg0, f32 arg1) {
    *(f32 *)(arg0 + 0) = arg1;
    *(s32 *)(arg0 + 4) = 0;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_002096C8);

void func_00209720(s32 arg0, f32 arg1) {
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

INCLUDE_ASM(const s32, "game/code_00207A38", func_00209770);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002097E8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00209978);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002099C0);

void func_00209CD0(void) {
    func_0020D128((s32)"btl:[%s]\n", D_00436AF0);
    D_00438F6C = func_00343ED0(D_00436AF0, &D_00438F70, 0);
}

void func_00209D08(void) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v0 = func_001AA6F8();
    temp_v1 = func_002DDCA0(D_00438F70, 0x10000);
    *(u32 *)(temp_v0 + 0x4e8) = temp_v1;
}

void func_00209D40(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    func_002DDCC8(*(u32 *)(temp_v0 + 0x4e8));
    *(u32 *)(temp_v0 + 0x4e8) = 0;
}

u32 func_00209D78(void) {
    s32 temp_v0;

    temp_v0 = func_0010D8D0();
    *(u32 *)(temp_v0 + 0x20) = 1;
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

u32 func_00209DA8(void) {
    s32 temp_v0;

    temp_v0 = func_0010D8D0();
    *(u32 *)(temp_v0 + 0x20) = 6;
    *(u32 *)(temp_v0 + 0x24) = 0xc2;
    return 1;
}

u32 func_00209DD8(void) {
    s32 temp_v0;

    temp_v0 = func_0010D8D0();
    *(u32 *)(temp_v0 + 0x24) = 1;
    *(u32 *)(temp_v0 + 0x20) = 0xd;
    return 1;
}

u32 func_00209E08(void) {
    s32 temp_v0;

    temp_v0 = func_0010D8D0();
    *(u32 *)(temp_v0 + 0x24) = 1;
    *(u32 *)(temp_v0 + 0x20) = 0x12;
    return 1;
}

u32 func_00209E38(void) {
    s32 temp_v0;

    temp_v0 = func_0010D8D0();
    *(u32 *)(temp_v0 + 0x24) = 1;
    *(u32 *)(temp_v0 + 0x20) = 10;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00209E68);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00209F28);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00209FA0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00209FF8);

s32 func_0020A048(void) {
    func_0021F3E8(func_0010D8D0());
    return 1;
}

u32 func_0020A070(void) {
    s32 temp_v0;

    temp_v0 = func_0010D8D0();
    *(u32 *)(temp_v0 + 0xc) = *(u32 *)(temp_v0 + 0xc) | 1;
    return 1;
}

u32 func_0020A0A0(void) {
    u16 temp_v0;
    s32 temp_v1;

    temp_v1 = func_0010D8D0();
    temp_v0 = func_0010D650(0);
    *(u16 *)(*(s32 *)(temp_v1 + 0x18) + 0x122) = temp_v0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020A0E0);

u32 func_0020A130(void) {
    btlCmdWithArgA(func_0010D8D0());
    return 1;
}

u32 func_0020A158(void) {
    s32 context = func_0010D8D0();
    u16 value = func_0010D650(0);
    btlCmdSimpleB(context, value);
    return 1;
}

u32 func_0020A198(void) {
    btlCmdWithArgB(func_0010D8D0());
    return 1;
}

u32 func_0020A1C0(void) {
    btlCmdWithArgC(func_0010D8D0());
    return 1;
}

u32 func_0020A1E8(void) {
    s32 context = func_0010D8D0();
    u16 value = func_0010D650(0);
    btlCmdSimpleA(context, value);
    return 1;
}

u32 func_0020A228(void) {
    btlCmdWithArgE(func_0010D8D0());
    return 1;
}

u32 func_0020A250(void) {
    btlCmdWithArgD(func_0010D8D0());
    return 1;
}

u32 func_0020A278(void) {
    s32 context = func_0010D8D0();
    u16 value = func_0010D650(0);
    btlCmdSimpleC(context, value);
    return 1;
}

u32 func_0020A2B8(void) {
    s32 context = func_0010D8D0();
    u16 value = func_0010D650(0);
    btlCmdSimpleD(context, value);
    return 1;
}

u32 func_0020A2F8(void) {
    s32 context = func_0010D8D0();
    u16 value = func_0010D650(0);
    btlCmdSimpleE(context, value);
    return 1;
}

u32 func_0020A338(void) {
    btlCmdWithArgA(func_0010D8D0());
    return 1;
}

u32 func_0020A360(void) {
    btlCmdSimpleG(func_0010D8D0(), 0);
    return 1;
}

u32 func_0020A390(void) {
    func_00211228(func_0010D8D0(), 0);
    return 1;
}

u32 func_0020A3C0(void) {
    btlCmdSimpleH(func_0010D8D0(), 0);
    return 1;
}

s32 func_0020A3F0(void) {
    s32 context = func_0010D8D0();
    func_002110E8(context, func_0010D650(0));
    return 1;
}

u32 func_0020A430(void) {
    btlCmdWithArgF(func_0010D8D0());
    return 1;
}

s32 func_0020A458(void) {
    func_00211108(func_0010D8D0());
    return 1;
}

u32 func_0020A480(void) {
    s32 context = func_0010D8D0();
    u16 value = func_0010D650(0);
    btlCmdSimpleJ(context, value);
    return 1;
}

u32 func_0020A4C0(void) {
    btlCmdSimpleI(func_0010D8D0(), 0);
    return 1;
}

s32 func_0020A4F0(void) {
    s32 context = func_0010D8D0();
    u32 choice = func_0010D650(0);
    if (func_00210EA0(context, *(s32 *)(context + 0x18), choice | 0x400000)) {
        func_0010D818(1);
        *(u32 *)(context + 0xa0) = choice;
        *(u32 *)(context + 0x98) |= 1;
    } else {
        func_0010D818(0);
        *(u32 *)(context + 0x98) &= ~1U;
    }
    return 1;
}

s32 func_0020A580(void) {
    s32 context = func_0010D8D0();
    u32 choice = func_0010D650(0);
    if (func_00210EA0(context, *(s32 *)(context + 0x18), choice | 0x7000000)) {
        func_0010D818(1);
        *(u32 *)(context + 0xa4) = choice;
        *(u32 *)(context + 0x98) |= 2;
    } else {
        func_0010D818(0);
        *(u32 *)(context + 0x98) &= ~2U;
    }
    return 1;
}

s32 func_0020A610(void) {
    s32 context = func_0010D8D0();
    u32 choice = func_0010D650(0);
    if (func_00210EA0(context, *(s32 *)(context + 0x18), choice | 0x7000000)) {
        func_0010D818(1);
        *(u32 *)(context + 0xa4) = choice;
        *(u32 *)(context + 0x98) |= 2;
    } else {
        func_0010D818(0);
        *(u32 *)(context + 0x98) &= ~2U;
    }
    return 1;
}

s32 func_0020A6A0(void) {
    s32 context = func_0010D8D0();
    u32 choice = func_0010D650(0);
    if (func_00210EA0(context, *(s32 *)(context + 0x18), choice | 0x800000)) {
        func_0010D818(1);
        *(u32 *)(context + 0xa8) = choice;
        *(u32 *)(context + 0x98) |= 4;
    } else {
        func_0010D818(0);
        *(u32 *)(context + 0x98) &= ~4U;
    }
    return 1;
}

s32 func_0020A730(void) {
    s32 context = func_0010D8D0();
    u32 choice = func_0010D650(0);
    if (func_00210EA0(context, *(s32 *)(context + 0x18), choice | 0xc00000)) {
        func_0010D818(1);
        *(u32 *)(context + 0xa8) = choice;
        *(u32 *)(context + 0x98) |= 4;
    } else {
        func_0010D818(0);
        *(u32 *)(context + 0x98) &= ~4U;
    }
    return 1;
}

s32 func_0020A7C0(void) {
    s32 context = func_0010D8D0();
    u32 choice = func_0010D650(0);
    if (func_00210EA0(context, *(s32 *)(context + 0x18), choice | 0x1000000)) {
        func_0010D818(1);
        *(u32 *)(context + 0xa8) = choice;
        *(u32 *)(context + 0x98) |= 4;
    } else {
        func_0010D818(0);
        *(u32 *)(context + 0x98) &= ~4U;
    }
    return 1;
}

s32 func_0020A850(void) {
    s32 context = func_0010D8D0();
    u32 choice = func_0010D650(0);
    if (func_00210EA0(context, *(s32 *)(context + 0x18), choice | 0x1c00000)) {
        func_0010D818(1);
        *(u32 *)(context + 0xac) = choice;
        *(u32 *)(context + 0x98) |= 8;
    } else {
        func_0010D818(0);
        *(u32 *)(context + 0x98) &= ~8U;
    }
    return 1;
}

s32 func_0020A8E0(void) {
    s32 context = func_0010D8D0();
    u32 choice = func_0010D650(0);
    if (func_00210EA0(context, *(s32 *)(context + 0x18), choice | 0x2000000)) {
        func_0010D818(1);
        *(u32 *)(context + 0xb0) = choice;
        *(u32 *)(context + 0x98) |= 0x10;
    } else {
        func_0010D818(0);
        *(u32 *)(context + 0x98) &= ~0x10U;
    }
    return 1;
}

s32 func_0020A970(void) {
    s32 context = func_0010D8D0();
    u32 choice = func_0010D650(0);
    if (func_00210EA0(context, *(s32 *)(context + 0x18), choice | 0x2400000)) {
        func_0010D818(1);
        *(u32 *)(context + 0xb4) = choice;
        *(u32 *)(context + 0x98) |= 0x20;
    } else {
        func_0010D818(0);
        *(u32 *)(context + 0x98) &= ~0x20U;
    }
    return 1;
}

s32 func_0020AA00(void) {
    s32 context = func_0010D8D0();
    u32 choice = func_0010D650(0);
    if (func_00210EA0(context, *(s32 *)(context + 0x18), choice | 0x2800000)) {
        func_0010D818(1);
        *(u32 *)(context + 0xb8) = choice;
        *(u32 *)(context + 0x98) |= 0x40;
    } else {
        func_0010D818(0);
        *(u32 *)(context + 0x98) &= ~0x40U;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020AA90);

s32 func_0020AB38(void) {
    s32 context = func_0010D8D0();
    u32 choice = func_0010D650(0);
    if (func_00210EA0(context, *(s32 *)(context + 0x18), choice | 0x3000000)) {
        func_0010D818(1);
        *(u32 *)(context + 0xbc) = choice;
        *(u32 *)(context + 0x98) |= 0x80;
    } else {
        func_0010D818(0);
        *(u32 *)(context + 0x98) &= ~0x80U;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020ABC8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020AC58);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020ACE8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020AD78);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020AE08);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020AE98);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020AF10);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020AF90);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B020);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B0B0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B100);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B150);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B1A0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B240);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B2E0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B350);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B3C0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B460);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B500);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B5A0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B640);

s32 func_0020B6E0(void) {
    s32 context = func_0010D8D0();
    u32 choice = func_0010D650(0);
    if (func_00210EA0(context, *(s32 *)(context + 0x18), choice | 0xd800000)) {
        func_0010D818(1);
        *(u32 *)(context + 0x10c) = choice;
        *(u32 *)(context + 0x9c) |= 1;
    } else {
        func_0010D818(0);
        *(u32 *)(context + 0x9c) &= ~1U;
    }
    return 1;
}

s32 func_0020B770(void) {
    s32 context = func_0010D8D0();
    u32 choice = func_0010D650(0);
    if (func_00210EA0(context, *(s32 *)(context + 0x18), choice | 0xdc00000)) {
        func_0010D818(1);
        *(u32 *)(context + 0x110) = choice;
        *(u32 *)(context + 0x9c) |= 2;
    } else {
        func_0010D818(0);
        *(u32 *)(context + 0x9c) &= ~2U;
    }
    return 1;
}

s32 func_0020B800(void) {
    s32 context = func_0010D8D0();
    u32 choice = func_0010D650(0);
    if (func_00210EA0(context, *(s32 *)(context + 0x18), choice | 0xe000000)) {
        func_0010D818(1);
        *(u32 *)(context + 0x114) = choice;
        *(u32 *)(context + 0x9c) |= 4;
    } else {
        func_0010D818(0);
        *(u32 *)(context + 0x9c) &= ~4U;
    }
    return 1;
}

s32 func_0020B890(void) {
    s32 context = func_0010D8D0();
    u32 choice = func_0010D650(0);
    if (func_00210EA0(context, *(s32 *)(context + 0x18), choice | 0xe400000)) {
        func_0010D818(1);
        *(u32 *)(context + 0x118) = choice;
        *(u32 *)(context + 0x9c) |= 8;
    } else {
        func_0010D818(0);
        *(u32 *)(context + 0x9c) &= ~8U;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B920);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B9B0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020BA40);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020BAD0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020BB60);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020BBD0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020BC40);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020BCD0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020BD60);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020BDE8);

u32 func_0020BE38(void) {
    s32 context = func_0010D8D0();
    if (func_00210EA0(context, *(s32 *)(context + 0x18), func_0010D650(0) | 0x10C00000)) {
        func_0010D818(1);
    } else {
        func_0010D818(0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020BEA8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020BEF8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020BF48);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020BFE0);

u32 func_0020C078(void) {
    return 1;
}

u32 func_0020C080(void) {
    if (func_00229110() != 0) {
        func_0010D818(1);
    } else {
        func_0010D818(0);
    }
    return 1;
}

u32 func_0020C0C0(void) {
    if (func_002291C0() != 0) {
        func_0010D818(1);
    } else {
        func_0010D818(0);
    }
    return 1;
}

u32 func_0020C100(void) {
    if (func_00219290() != 0) {
        func_0010D818(1);
    } else {
        func_0010D818(0);
    }
    return 1;
}

u32 func_0020C140(void) {
    if (func_0021F808() != 0) {
        func_0010D818(1);
    } else {
        func_0010D818(0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020C180);

u32 func_0020C290(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020C298);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020C328);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020C3C0);

s32 func_0020C450(void) {
    s32 context = func_0010D8D0();
    if (func_00210EA0(context, *(s32 *)(context + 0x18), 0x12800000)) {
        func_0010D818(1);
    } else {
        func_0010D818(0);
    }
    return 1;
}

s32 func_0020C4A0(void) {
    if (func_00221090()) {
        func_0010D818(1);
    } else {
        func_0010D818(0);
    }
    return 1;
}

u32 func_0020C4E0(void) {
    u64 temp_v0;

    temp_v0 = func_001B76F0();
    func_0010D818(temp_v0);
    return 1;
}

u32 func_0020C508(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    func_0010D818(*(u32 *)(temp_v0 + 0x274));
    return 1;
}

u32 func_0020C530(void) {
    s32 temp_v0;

    temp_v0 = func_0010D8D0();
    func_0010D818(*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 0x134));
    return 1;
}

u32 func_0020C560(void) {
    return 1;
}

u32 func_0020C568(void) {
    return 1;
}

u32 func_0020C570(void) {
    u32 temp_v0;

    temp_v0 = func_002290D0();
    func_0010D818(temp_v0);
    return 1;
}

u32 func_0020C598(void) {
    u64 temp_v0;

    temp_v0 = func_00220958();
    func_0010D818(temp_v0);
    return 1;
}

u32 func_0020C5C0(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    func_0010D818(*(u16 *)(temp_v0 + 0x280));
    return 1;
}

u32 func_0020C5E8(void) {
    u32 temp_v0;

    temp_v0 = func_00229158();
    func_0010D818(temp_v0);
    return 1;
}

u32 func_0020C610(void) {
    u32 temp_v0;

    temp_v0 = func_002192D8();
    func_0010D818(temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020C638);

u32 func_0020C660(void) {
    func_0010D818(D_00436CAC);
    return 1;
}

u32 func_0020C688(void) {
    return 1;
}

u32 func_0020C690(void) {
    func_00210F58(func_0010D8D0());
    return 1;
}

u32 func_0020C6B8(void) {
    s32 context = func_0010D8D0();
    u16 value = func_0010D650(0);
    func_002112C8(context, value);
    return 1;
}

s32 func_0020C6F8(void) {
    func_00226558(func_0010D650(0));
    return 1;
}

u32 func_0020C720(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = func_0010D8D0();
    temp_v0 = D_004367F4;
    *(u32 *)(temp_v1 + 0x20) = 0x10;
    *(u32 *)(temp_v1 + 0x24) = 0;
    *(u8 *)(temp_v0 + 0x54) = 1;
    return 1;
}

u32 func_0020C760(void) {
    func_001CFB20();
    return 1;
}

u32 func_0020C780(void) {
    func_001CFAE0();
    return 1;
}

u32 func_0020C7A0(void) {
    func_00208D58();
    return 1;
}

u32 func_0020C7C0(void) {
    func_00208DA0();
    func_00208DE8(0x200);
    return 1;
}

u32 func_0020C7E8(void) {
    func_00208DA0();
    func_00208DE8(0x400);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020C810);

u32 func_0020C8E0(void) {
    u64 temp_v0;

    temp_v0 = func_001E66D8();
    func_001E1580(temp_v0);
    temp_v0 = func_001E6740();
    func_001E1580(temp_v0);
    temp_v0 = func_001E8B40(func_0010D8D0(), 0x11);
    func_001E1580(temp_v0);
    return 1;
}

u32 func_0020C938(void) {
    u64 temp_v0;

    temp_v0 = func_001E66D8();
    func_001E1580(temp_v0);
    temp_v0 = func_001E6740();
    func_001E1580(temp_v0);
    temp_v0 = func_001E8B40(0, 3);
    func_001E1580(temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020C988);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020CA10);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020CA98);

u32 func_0020CB80(void) {
    func_0020D128(D_00419570);
    func_0010D818(0);
    return 1;
}

u32 func_0020CBB0(void) {
    func_0020D128(D_00419590);
    func_0010D818(0x14);
    return 1;
}

u32 func_0020CBE0(void) {
    s32 temp_v0;

    temp_v0 = func_0010D8D0();
    func_0010D818(*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 0x122));
    return 1;
}

u32 func_0020CC10(void) {
    func_0020D128(D_004195B8);
    func_0010D818(0);
    return 1;
}

u32 func_0020CC40(void) {
    func_0020D128(D_004195D8);
    return 1;
}

u32 func_0020CC68(void) {
    func_0020D128(D_004195F8);
    return 1;
}

u32 func_0020CC90(void) {
    func_0020D128(D_00419620);
    return 1;
}

u32 func_0020CCB8(void) {
    func_0020D128(D_00419648);
    return 1;
}

u32 func_0020CCE0(void) {
    func_0020D128(D_00419668);
    return 1;
}

u32 func_0020CD08(void) {
    func_0020D128(D_00419688);
    return 1;
}

u32 func_0020CD30(void) {
    func_002269E0();
    return 1;
}

u32 func_0020CD50(void) {
    s32 temp_v0;

    temp_v0 = func_00211DE0();
    func_0010D818(temp_v0 + 1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020CD78);

void func_0020CE28(void) {
}

void func_0020CE30(void) {
}

void func_0020CE38(void) {
}

u32 func_0020CE40(void) {
    D_00436B00 = D_00436B00 | 0x2000000;
    return 0;
}

u32 func_0020CE58(u32 arg0) {
    return arg0;
}

void func_0020CE60(void) {
}

void func_0020CE68(void) {
}

void func_0020CE70(void) {
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020CE78);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020CEF8);

void func_0020D0D0(void) {
    D_003BEA48[4] &= ~0x80;
    D_003BE6E0[4] &= ~0x80;
}

void func_0020D100(void) {
}

void func_0020D108(void) {
}

void func_0020D110(void) {
}

void func_0020D118(void) {
}

void func_0020D120(void) {
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020D128);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020D170);

void func_0020D1B0(void) {
}

void func_0020D1B8(void) {
}

void func_0020D1C0(void) {
}

void func_0020D1C8(void) {
}

void func_0020D1D0(void) {
}

void func_0020D1D8(void) {
}

void func_0020D1E0(void) {
}

void func_0020D1E8(void) {
}

void func_0020D1F0(void) {
}

void func_0020D1F8(void) {
}

void func_0020D200(void) {
}

void func_0020D208(void) {
}

void func_0020D210(void) {
}

void func_0020D218(void) {
}

void func_0020D220(void) {
}

void func_0020D228(void) {
}

void func_0020D230(void) {
}

void func_0020D238(void) {
}

void func_0020D240(void) {
}

void func_0020D248(void) {
}

void func_0020D250(void) {
}

void func_0020D258(void) {
}

u32 func_0020D260(u32 arg0) {
    return arg0;
}

void func_0020D268(void) {
}

void func_0020D270(void) {
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020D278);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020D2C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419570);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419590);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004195B8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004195D8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004195F8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419620);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419648);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419668);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419688);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004196B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004196C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004196D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004196E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004196F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419700);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419710);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419720);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419730);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419740);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419750);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419760);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419770);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419780);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419790);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004197A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004197B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004197C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004197D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004197E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004197F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419800);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419810);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419820);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419830);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419840);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419850);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419860);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419870);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419880);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419890);

s32 func_0020D320(s32 arg0) {
    char buf[0x70];

    if (D_00438B66 != 0) {
        func_0035C860(buf, "pfs0:/%s", arg0);
        return sceDopen(buf);
    }
    D_00438F80 = 0;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020D370);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020D3A0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020D448);

void func_0020D8F0(s32 arg0) {
    s32 temp_v0;

    if (*(s32 *)(arg0 + 8) != 0) {
        temp_v0 = *(s32 *)(arg0 + 8);
        do {
            s32 temp_v1 = *(s32 *)(temp_v0 + 0x40);
            func_00328E48(temp_v0);
            temp_v0 = temp_v1;
        } while (temp_v0 != 0);
    }
    func_00328E48(*(s32 *)(arg0 + 4));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020D948);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020DA28);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020DAB8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020DF68);

void func_0020DFB0(s32 arg0, s32 arg1, s32 arg2) {
    *(s32 *)(arg0 + 0) = arg1;
    *(s32 *)(arg0 + 4) = arg2;
}

u32 func_0020DFC0(s32 arg0) {
    return *(u32 *)(arg0 + 0x14);
}

u32 func_0020DFC8(s32 arg0) {
    return *(u32 *)(arg0 + 8);
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020DFD0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020E048);

u32 func_0020E0F8(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x34) + 4);
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020E108);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020E180);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020E1E0);

s32 func_0020E300(s32 arg0) {
    s32 temp_v0;

    temp_v0 = (s32)func_00328D68(0x38);
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

void func_0020E368(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020E380);

void func_0020E7A0(s32 arg0, s32 arg1, s32 arg2) {
    *(s32 *)(arg0 + 0) = arg1;
    *(s32 *)(arg0 + 4) = arg2;
}

u32 func_0020E7B0(s32 arg0) {
    return *(u32 *)(arg0 + 8);
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020E7B8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020E7F8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020E828);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436AF0);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B00);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B04);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B08);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B0C);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B10);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B18);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B1C);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B20);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B28);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B30);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B38);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B40);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B48);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B50);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B58);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B5C);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B60);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B68);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B70);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B78);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B80);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B88);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B90);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B98);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BA0);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BA4);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BA8);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BB0);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BB8);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BC0);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BC8);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BD0);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BD8);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BE0);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BE8);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BF0);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BF8);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C00);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C08);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C10);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C18);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C30);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C38);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C40);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C48);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C50);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C58);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C60);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C68);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C70);

