#include "common.h"

extern u64 func_00231220(void);

extern u8 D_00436F5D;

extern s32 func_0022C9C8(void);

extern s32 func_0023A170(s32);

extern s32 D_00435DD0;

extern u64 func_00219318(void);

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

extern u32 D_00436CB0;

extern s32 func_001B2CC0(void);

extern s32 func_00212CB8(u32, u32, u32);

extern s32 func_001B33C8(void);

extern s32 func_001ABB10(void);

extern s32 func_001B2D38(void);

extern u32 func_001AC360(u64, u64, u64);

extern u64 func_001E7FD8(u64);

extern u32 func_001E8058(u64);

extern s32 func_001E12C8(u32);

extern s32 D_00436AF0;

extern s32 D_00438F6C;

extern s32 func_00343ED0(s32, u32 *, s32);

extern void func_0020D128(s32, ...);

extern void func_00211188(s32, u16);

extern void func_00211128(s32, u16);

extern void func_002111C8(s32, u16);

extern void func_002111E8(s32, u16);

extern void func_002112A8(s32, u16);

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

extern void func_002152D8(s32, s32);

extern void func_00215C70(s32, s32);

extern void func_00215F28(s32, s32);

extern void func_00216760(s32, s32);

extern void func_00216888(s32, s32);

extern void func_002160A0();

extern void func_00216CA0();

extern void func_00216988();

extern s32 *D_00436CB8;

extern s32 func_00212B38();

extern u32 func_002290D0(void);

extern u32 func_00229158(void);

extern u32 func_00229198(void);

extern u32 func_002192D8(void);

extern s8 D_00453068[];

extern s8 D_00453060[];

extern s32 func_003282F0(s32, s32, s32);

extern u32 D_00438F90;

extern s32 D_003C86F0[];

extern s32 D_003C8710[];

INCLUDE_ASM(const s32, "game/code_00207A38", func_00207A38);

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

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208EA8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208F10);

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

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020A048);

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
    func_00211088(func_0010D8D0());
    return 1;
}

u32 func_0020A158(void) {
    s32 context = func_0010D8D0();
    u16 value = func_0010D650(0);
    func_00211188(context, value);
    return 1;
}

u32 func_0020A198(void) {
    func_002110A8(func_0010D8D0());
    return 1;
}

u32 func_0020A1C0(void) {
    func_002110C8(func_0010D8D0());
    return 1;
}

u32 func_0020A1E8(void) {
    s32 context = func_0010D8D0();
    u16 value = func_0010D650(0);
    func_00211128(context, value);
    return 1;
}

u32 func_0020A228(void) {
    func_00211168(func_0010D8D0());
    return 1;
}

u32 func_0020A250(void) {
    func_00211148(func_0010D8D0());
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020A278);

u32 func_0020A2B8(void) {
    s32 context = func_0010D8D0();
    u16 value = func_0010D650(0);
    func_002111C8(context, value);
    return 1;
}

u32 func_0020A2F8(void) {
    s32 context = func_0010D8D0();
    u16 value = func_0010D650(0);
    func_002111E8(context, value);
    return 1;
}

u32 func_0020A338(void) {
    func_00211088(func_0010D8D0());
    return 1;
}

u32 func_0020A360(void) {
    func_00211208(func_0010D8D0(), 0);
    return 1;
}

u32 func_0020A390(void) {
    func_00211228(func_0010D8D0(), 0);
    return 1;
}

u32 func_0020A3C0(void) {
    func_00211248(func_0010D8D0(), 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020A3F0);

u32 func_0020A430(void) {
    func_00211288(func_0010D8D0());
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020A458);

u32 func_0020A480(void) {
    s32 context = func_0010D8D0();
    u16 value = func_0010D650(0);
    func_002112A8(context, value);
    return 1;
}

u32 func_0020A4C0(void) {
    func_00211268(func_0010D8D0(), 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020A4F0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020A580);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020A610);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020A6A0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020A730);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020A7C0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020A850);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020A8E0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020A970);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020AA00);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020AA90);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020AB38);

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

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B6E0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B770);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B800);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020B890);

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

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020BE38);

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

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020C450);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020C4A0);

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

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020C570);

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

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020C5E8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020C610);

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

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020C6F8);

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

void func_0020E850(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020E858);

void func_0020EA10(void) {
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020EA18);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020EB40);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020EBD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419910);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419920);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020EC20);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020F048);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020F200);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020F3B0);

s32 func_0020F5C8(s32 arg0, s32 arg1) {
    return arg1 + (((*(s32 *)(arg0 + 0x110) >> 9) ^ 1U) & 1);
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020F5E0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020F9B0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020F9D0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020FA98);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020FEF8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020FF18);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020FFB8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002100B8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210148);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002101C8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210258);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210360);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002103F8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210498);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210530);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002105F0);

u32 func_00210688(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210690);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210720);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002107C0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210850);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002108E8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210980);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210A28);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210AA8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210B78);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210C10);

u32 func_00210CB0(u32 arg0) {
    D_00436CB0 = D_00436CB0 * 0x41c64e6d + 0x3039;
    return (D_00436CB0 >> 0x10) * (arg0 & 0xffff) >> 0x10;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210CE0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210D48);

u32 func_00210DB0(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0xb;
    *(u32 *)(arg0 + 0x24) = 0xc2;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210DC8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210E48);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210EA0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210F10);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00210F58);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211018);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211088);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002110A8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002110C8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002110E8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211108);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211128);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211148);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211168);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211188);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002111A8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002111C8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002111E8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211208);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211228);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211248);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211268);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211288);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002112A8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002112C8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211360);

u32 func_002115B0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_002115B8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211658);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002119E0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211D20);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211DE0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211EA8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419A88);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00211F38);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002122D8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00212340);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002123C0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00212470);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00212520);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00212550);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002125B0);

u8 func_00212620(void) {
    s64 temp_v0;

    temp_v0 = func_001B2CC0();
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00212640);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002126C8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00212728);

s32 func_00212788(s32 arg0, s32 arg1) {
    return (func_001AA840(arg0 + 0x120, arg1) & arg1) != 0;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_002127B8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00212838);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00212948);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002129C8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00212A40);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00212AB0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00212B38);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00212CB8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00212E60);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00212F20);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00212FA0);

u8 func_00213020(u32 arg0, u32 arg1) {
    s64 temp_v0;

    temp_v0 = func_00212CB8(arg0, arg1, 0);
    return temp_v0 != 0;
}

u8 func_00213040(u32 arg0, u32 arg1) {
    s64 temp_v0;

    temp_v0 = func_00212CB8(arg0, arg1, 1);
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213060);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002130E8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213170);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002131F8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213280);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213308);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213390);

u8 func_002133F8(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_00212B38(arg0, 10);
    return temp_v0 != 0;
}

s32 func_00213418(void) {
    return func_00212B38() != 0;
}

s32 func_00213438(s32 battler) {
    u32 flags;

    if (*(u32 *)(battler + 0x110) & 0x200) {
        return 0;
    }
    flags = *(u16 *)(battler + 0x120) & 0x2000;
    return flags != 0;
}

s32 func_00213460(void) {
    return ((*(s32 *)(*(s32 *)D_00436CB8 + 0xc) & 2) > 0);
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213478);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213498);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002134C8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213518);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213548);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213578);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002135B8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213620);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213688);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002136D8);

u8 func_00213728(void) {
    s64 temp_v0;

    temp_v0 = func_001B33C8();
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213748);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213818);

s32 func_00213900(s32 arg0) {
    return ((*(s32 *)(arg0 + 0x110) & 0x1000) > 0);
}

u8 func_00213910(void) {
    s64 temp_v0;

    temp_v0 = func_001ABB10();
    return temp_v0 == 0;
}

u8 func_00213930(void) {
    s64 temp_v0;

    temp_v0 = func_001B2D38();
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213950);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213A58);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213B38);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213BB8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213C38);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213CA8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213D38);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213DA0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213E18);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213E80);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213EF8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00213F58);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214098);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214168);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214188);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002141A8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214230);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002142C0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214330);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002143A0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214470);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214510);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214578);

s32 func_002145E0(void) {
    return D_00436CAC < 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_002145F0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214658);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002146C0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002146E0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214700);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002147B0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214928);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214948);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214AA0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214B00);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214B60);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214BE0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214C18);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214C78);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00214DF8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00215118);

u64 func_00215268(u64 arg0, u32 *arg1, u32 *arg2) {
    u32 temp_v0;
    u64 temp_v1;

    temp_v1 = func_001E7FD8(0xd);
    temp_v0 = func_001AC360(arg0, temp_v1, 0);
    *arg1 = temp_v0;
    temp_v0 = func_001E8058(temp_v1);
    *arg2 = temp_v0;
    return temp_v1;
}

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419B38);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419B68);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419B88);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419BB8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419BE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419C30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419C58);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419C80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419CB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419CD8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419D00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419D20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419DA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419E20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419EA0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002152D8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00215C70);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00215D78);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00215F28);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002160A0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002161B0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002162C0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002163C8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00216760);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00216888);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00216988);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00216B40);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00216CA0);

u32 func_00216D10(u32 arg0, u32 arg1) {
    func_00217170(arg0, arg1, 1);
    return 1;
}

u32 func_00216D30(u32 arg0, u32 arg1) {
    func_00217170(arg0, arg1, 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00216D50);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00216E98);

u32 func_00216ED0(s32 arg0) {
    u64 temp_v0;

    temp_v0 = func_00219318();
    func_001E8030(*(u32 *)(arg0 + 0x60), temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00216F08);

u32 func_00217020(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00217028);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00217170);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002172B8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00217378);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00217470);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00217650);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00217898);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00217B20);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00217DC8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00217EB8);

u32 func_00217FE8(void) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v0 = func_001AA6F8();
    temp_v1 = 0;
    if (**(s8 **)(temp_v0 + 0x718) == '\0') {
        temp_v1 = 100;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218018);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002180F8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218150);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002181E8);

void func_00218250(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    **(u32 **)(temp_v0 + 0x718) = 0;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218278);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218320);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218418);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218520);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218630);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218690);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002186C0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218798);

void func_00218968(void) {
    func_0011AEE0(1);
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218980);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002189B8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218A78);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218AF0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218B78);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218BA8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218D00);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218D88);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218DE0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218E98);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218EE8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218F18);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00218FD0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002190A0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00219170);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00219210);

void func_00219278(void) {
    func_00219210();
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00219290);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002192D8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00219318);

void func_002193B8(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    **(u32 **)(temp_v0 + 0x718) = 0;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_002193E0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00219488);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002194E8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002195E0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00219760);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00219848);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002198D8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00219950);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002199C8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00219A70);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00219B48);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00219BD0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00219CC8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041A378);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00219D40);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00219E38);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00219F28);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00219F58);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021A098);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021A1D8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021A308);

void func_0021A3A0(s32 arg0) {
    s32 temp_v0;

    *(u32 *)(arg0 + 0x1c) = 0x80808080;
    temp_v0 = *(s32 *)(arg0 + 0xc);
    *(u16 *)(arg0 + 0x14) = *(u16 *)(arg0 + 0x14) & 0xfffd;
    if (temp_v0 != 0) {
        do {
            func_0021A3A0(temp_v0);
            temp_v0 = *(s32 *)(temp_v0 + 4);
        } while (temp_v0 != *(s32 *)(arg0 + 0xc));
    }
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021A408);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021A490);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021A778);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021A8F8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021A978);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021B070);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021B168);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021B250);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021B310);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021B368);

void func_0021B4A8(void) {
    func_0021B368();
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021B4C0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021B520);

void func_0021B5C0(void) {
    s32 *piVar1;
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    piVar1 = *(s32 **)(temp_v0 + 0x718);
    temp_v0 = *piVar1;
    if (temp_v0 != 0) {
        func_001E7D30(temp_v0);
        *piVar1 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021B600);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021B670);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021B6C0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021B788);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021B828);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021C0C8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021C390);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021C428);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021C548);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021C5E0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021C7F8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021C818);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041A5E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041A5F8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021CF18);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021E778);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021E8C0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021EA38);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021EAF8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021EB28);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021EB78);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021EBF0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021EC30);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021EC60);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021ED08);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021ED58);

u64 func_0021EDD8(u64 arg0) {
    s32 temp_v0;
    u64 temp_v1;

    temp_v0 = func_001AA6F8();
    temp_v1 = 0;
    if (*(s8 *)(*(s32 *)(temp_v0 + 0x718) + 2) != '\0') {
        temp_v1 = arg0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021EE10);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021EEF8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021EF48);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021EFD8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021F040);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021F0E8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021F238);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021F378);

u8 func_0021F3A0(s32 arg0) {
    return arg0 != 0x196;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021F3B0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021F3E8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021F698);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021F798);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021F808);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0021F848);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220368);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220450);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220568);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002205C0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002206A0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220700);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220810);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220918);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220958);

void func_00220998(void) {
    func_0011AEE0(9);
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_002209B0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002209F0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220A38);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220A78);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220B20);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220C38);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220D18);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220D48);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220D98);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220DD8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220EE8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00220F68);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221060);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221090);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002210D0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221128);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221158);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221390);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002213E0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221410);

u32 func_00221498(s32 arg0) {
    if (*(s32 *)(arg0 + 0x134) == 0x6c) {
        *(u32 *)(arg0 + 0x13c) = 0;
        return 0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_002214C0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221538);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221568);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221760);

void func_002217E8(void) {
    s32 *piVar1;
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    piVar1 = *(s32 **)(temp_v0 + 0x718);
    temp_v0 = *piVar1;
    if (temp_v0 != 0) {
        func_001E7D30(temp_v0);
        *piVar1 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221828);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221858);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221888);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002218C8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221988);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221A30);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221A80);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221B28);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221B88);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221BC8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221E28);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221EA0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221F40);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00221FE8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00222028);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00222100);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00222298);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00222330);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002223D8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00222450);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002226D0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002226F0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00222768);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ACA8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ACC0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002228C0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00222A08);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00222D18);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00222DA8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00222E58);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00222F18);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00223280);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00223350);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00223BD8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00223D10);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00223DD8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00223ED0);

u32 func_00223FB0(s32 arg0) {
    if (*(s32 *)(arg0 + 0x134) == 0x10b) {
        *(u32 *)(arg0 + 0x110) = *(u32 *)(arg0 + 0x110) | 0x800;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00223FE0);

u32 func_00224010(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = 0xe0;
    if (arg1 != 0x12d) {
        temp_v0 = 0;
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00224020);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002240C0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002240F8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00224188);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00224238);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002242F8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00224500);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00224598);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002247B0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002247D0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00224D28);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00224DF0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00224EE8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00224F88);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00224FC0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002251A0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00225368);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002254C8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00225778);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00225798);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00225828);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002258D8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002259A0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00225B48);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00225BF8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002260E0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002261A8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002262A0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00226308);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00226398);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002263D8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002264E8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00226540);

void func_00226558(u8 arg0) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    if (*(s32 *)(temp_v0 + 0x2a0) == 0x31b) {
        **(u8 **)(temp_v0 + 0x718) = arg0;
    }
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00226598);

u32 func_00226670(void) {
    return 0xffffffff;
}

s32 func_00226678(s32 battler, s32 command) {
    if (command == 1 || command == 0x12) {
        if ((*(u16 *)(battler + 0x120) & 0x2000) != 0) {
            return -1;
        }
    }
    return command;
}

u8 func_002266A8(u32 arg0, s32 arg1) {
    return arg1 == 0xf;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_002266B8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002266D8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002267A0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00226820);

u32 func_00226850(s32 arg0) {
    if (*(s32 *)(arg0 + 0x134) == 0x187) {
        *(u32 *)(arg0 + 0x13c) = 0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00226868);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00226900);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002269E0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00226A60);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00226AB0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00226BB8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00226C48);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041B4D0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00226C98);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00226E98);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00226F58);

void func_00227288(void) {
    func_00226F58();
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_002272A0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002274D0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00227528);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00227660);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00227748);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002277D8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00227820);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002279F0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00227C70);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00227CC8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00227DA8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00228320);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00228360);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00228418);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00228458);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00228598);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002286D8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00228A30);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00228B08);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00228D68);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00228F20);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00228F48);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002290D0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00229110);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00229158);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00229198);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002291C0);

void func_00229248(void) {
    u8 *puVar1;
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    puVar1 = *(u8 **)(temp_v0 + 0x718);
    puVar1[1] = 1;
    *puVar1 = 0;
}

u32 func_00229278(void) {
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00229280);

void func_002292D8(void) {
    u16 temp_v0;
    u16 *puVar2;
    u32 temp_v1;
    u32 temp_v2;
    u32 temp_v3;
    u8 temp_v4;

    temp_v4 = 0;
    temp_v1 = 0;
    temp_v3 = 0;
    puVar2 = (u16 *)(D_00435DD0 + 0xa60);
    temp_v2 = 0;
    do {
        temp_v0 = *puVar2;
        if ((temp_v0 & 1) != 0) {
            if (puVar2[2] == 2) {
                if ((temp_v0 & 2) != 0) {
                    return;
                }
                temp_v4 = 1;
            }
            temp_v3 = temp_v3 + 1;
            if ((temp_v0 & 2) != 0) {
                temp_v1 = temp_v1 + 1;
            }
        }
        temp_v2 = temp_v2 + 1;
        puVar2 = puVar2 + 0xe2;
    } while (temp_v2 < 5);
    if (((temp_v3 < 4) && (temp_v1 < 3)) && (temp_v4)) {
        func_0011AEE0(2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00229378);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002293D8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00229420);

u32 func_00229470(void) {
    return 6;
}

void func_00229478(void) {
    func_0011AEE0(7);
}

void func_00229490(void) {
    func_0011AEE0(1);
    func_0011AEE0(4);
    func_0011AEE0(5);
}

void func_002294B8(void) {
    func_0011AEE0(8);
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_002294D0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002295D8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00229690);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00229728);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022A7D0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022A808);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022A8A8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022A908);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022A980);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022A9D0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022AA38);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022AAC8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022AAE8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022AB60);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022ABF0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022AC10);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022AF90);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022B108);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022B1E8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022B288);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022B348);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022B398);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022B460);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022B510);

u8 func_0022B5E0(void) {
    s64 temp_v0;

    temp_v0 = func_001E12C8(0x1a);
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022B600);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022B6E8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022B760);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022B7A0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022B8E0);

u32 func_0022B928(void) {
    s64 temp_v0;
    s32 temp_v1;
    u32 temp_v2;
    s32 temp_v3;

    temp_v2 = 0;
    temp_v3 = 0;
    do {
        temp_v1 = 0x8ff - temp_v2;
        temp_v2 = temp_v2 + 1;
        temp_v0 = func_0023A170(temp_v1);
        if (temp_v0 != 0) {
            temp_v3 = temp_v3 + 1;
        }
    } while (temp_v2 < 100);
    func_0010D818(temp_v3);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022B988);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022B9D0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022BA08);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022BAA8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022BB28);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022BBA8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022BC28);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022BC90);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022BCD8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022BD28);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022BD78);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022BDC0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022BEB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041B800);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022C040);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022C1B0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022C308);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022C518);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022C600);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022C788);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022C7F0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022C8B0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022C8F0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022C948);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022C9C8);

void func_0022CA40(void) {
}

void func_0022CA48(void) {
    func_0022C8B0();
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022CA60);

void func_0022CB68(void) {
    s64 temp_v0;

    temp_v0 = func_0022C9C8();
    if (temp_v0 != 0) {
        func_0022C7F0(temp_v0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022CBA0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022CD08);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022CD30);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022CD60);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022CE30);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022CF58);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022CFB0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022D040);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022D2B8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022D2F8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022DD18);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022DD70);

void func_0022DDC8(void) {
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022DDD0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022DE38);

void func_0022DEB8(void) {
    func_00105538();
    func_001054E0();
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022DED8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022DF98);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022E028);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022E0E0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022E338);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022E390);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022E3D0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022E428);

s32 func_0022E450(void) {
    return D_00453068[0];
}

s32 func_0022E460(void) {
    u16 state;

    if (D_00453060[8] == 0) {
        return 1;
    }
    state = *(u16 *)(D_00453060 + 4);
    if (state == 0) {
        return 1;
    }
    return state == 2;
}

s32 func_0022E490(void) {
    u16 state;

    if (D_00453060[8] == 0) {
        return 1;
    }
    state = *(u16 *)(D_00453060 + 4);
    if (state == 0) {
        return 1;
    }
    return state == 4;
}

void func_0022E4C0(void) {
    if (D_00453060[8] != 0) {
        D_00453060[7] = 1;
    }
}

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041B9D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041B9E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041B9F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BA00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BA10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BA20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BA30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BA40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BA50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BA60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BA70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BA80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BA90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BAA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BAB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BAC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BAD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BAE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BAF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BB00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BB10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BB20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BB38);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BB50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BB68);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BB80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BB98);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BBB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BBC8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BBE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BBF8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BC10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BC20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BC30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BC40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BC50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BC60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BC70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BC80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BC90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BCA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BCB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BCC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BCD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BCE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BCF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BD00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BD10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BD20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BD30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BD40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BD50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BD60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BD70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BDC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BDD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BDE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BDF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BE00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BE10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BE20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BE30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BE40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BE50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BE60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BE70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BE80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BE90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041BEC8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022E4E0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022EBC0);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022ED38);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022ED90);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022EE88);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022EF18);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022F068);

INCLUDE_ASM(const s32, "game/code_00207A38", func_0022F180);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002303D0);

void func_00230960(void) {
    D_00436F5D = 0;
    func_0011EBC8();
}

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C118);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C128);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C138);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C148);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C158);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C168);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C178);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C188);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C198);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C1A8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C1B8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C1C8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C1D8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C1E8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C1F8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C208);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C218);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C228);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C238);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C248);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C258);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C268);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C278);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C288);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C298);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C2A8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C2B8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C2C8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C2D8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00230978);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00230D90);

void func_002311C0(void) {
    s32 i;

    D_00438F90 = func_003282F0(1, 0x7f, 0);
    for (i = 0; i != 8; i++) {
        D_003C86F0[i] = 0;
        D_003C8710[i] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00231220);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00231250);

INCLUDE_ASM(const s32, "game/code_00207A38", func_002312A0);

void func_002312F8(s32 arg0, s32 arg1) {
    s32 *temp_v0;
    s32 *temp_v1;

    temp_v0 = &D_003C8710[arg0];
    temp_v1 = (s32 *)*temp_v0;
    if (temp_v1 == 0) {
        return;
    }
    do {
        if (*(temp_v1 + 1) == arg1) {
            *temp_v0 = *temp_v1;
            func_00328E48(temp_v1);
            break;
        } else {
            temp_v0 = temp_v1;
            temp_v1 = (s32 *)*temp_v1;
        }
    } while (temp_v1 != 0);
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00231358);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00231470);

void func_00231588(void) {
    u64 temp_v0;

    temp_v0 = func_00231220();
    func_00231470(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_002315A8);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00231618);




INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C300);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C310);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C320);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C330);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C340);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C350);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C360);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C370);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C380);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C390);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C3A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C3B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C3C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C3D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C3E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C3F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C400);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C410);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C420);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C430);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C440);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C450);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C460);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C470);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C480);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C490);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C4A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C4B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C4C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C4D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C4E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C4F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C500);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C510);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C520);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C530);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C540);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C550);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C560);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C570);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C580);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C590);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C5A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C5B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C5C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C5D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C5E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C5F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C600);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C610);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C620);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C630);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C640);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C650);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C660);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C670);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C680);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C690);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C6A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C6B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C6C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C6D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C6E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C6F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C700);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C710);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C720);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C730);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C740);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C750);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C760);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C770);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C780);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C790);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C7A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C7B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C7C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C7D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C7E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C7F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C800);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C810);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C820);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C830);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C840);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C850);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C860);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C870);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C880);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C890);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C8A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C8B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C8C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C8D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C8E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C8F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C900);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C910);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C920);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C930);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C940);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C950);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C960);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C970);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C980);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C990);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C9A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C9B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C9C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C9D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C9E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041C9F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CA00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CA10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CA20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CA30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CA40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CA50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CA60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CA70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CA80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CA90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CAA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CAB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CAC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CAD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CAE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CAF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CB00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CB10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CB20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CB30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CB40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CB50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CB60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CB70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CB80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CB90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CBA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CBB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CBC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CBD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CBE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CBF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CC00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CC10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CC20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CC30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CC40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CC50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CC60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CC70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CC80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CC90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CCA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CCB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CCC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CCD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CCE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CCF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CD00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CD10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CD20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CD30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CD40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CD50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CD60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CD70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CD80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CD90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CDA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CDB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CDC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CDD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CDE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CDF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CE00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CE10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CE20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CE30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CE40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CE50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CE60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CE70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CE80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CE90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CEA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CEB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CEC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CED0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CEE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CEF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CF00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CF10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CF20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CF30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CF40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CF50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CF60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CF70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CF80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CF90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CFA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CFB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CFC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CFD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CFE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041CFF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D000);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D010);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D020);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D030);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D040);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D050);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D060);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D070);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D080);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D090);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D0A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D0B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D0C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D0D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D0E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D0F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D100);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D110);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D120);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D130);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D140);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D150);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D160);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D170);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D180);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D190);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D1A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D1B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D1C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D1D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D1E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D1F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D200);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D210);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D220);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D230);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D240);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D250);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D260);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D270);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D280);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D290);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D2A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D2B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D2C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D2D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D2E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D2F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D300);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D310);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D320);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D330);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D340);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D350);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D360);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D370);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D380);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D390);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D3A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D3B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D3C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D3D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D3E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D3F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D400);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D410);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D420);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D430);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D440);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D450);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D460);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D470);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D480);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D490);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D4A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D4B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D4C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D4D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D4E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D4F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D500);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D510);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D520);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D530);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D540);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D550);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D560);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D570);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D580);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D590);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D5A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D5B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D5C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D5D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D5E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D5F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D600);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D610);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D620);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D630);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D640);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D650);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D660);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D670);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D680);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D690);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D6A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D6B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D6C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D6D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D6E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D6F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D700);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D710);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D720);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D730);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D740);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D750);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D760);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D770);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D780);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D798);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D7B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D7C8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D7E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D7F8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D810);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D828);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D840);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D858);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D870);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D888);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D8A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D8B8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D8D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D8E8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D900);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D918);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D930);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D948);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D960);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D978);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D990);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D9A8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D9C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D9D8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041D9F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DA08);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DA20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DA38);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DA50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DA68);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DA80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DA90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DAA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DAB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DAC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DAD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DAE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DAF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DB00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DB10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DB20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DB30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DB40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DB50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DB60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DB70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DB80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DB90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DBA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DBB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DBC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DBD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DBE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DBF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DC00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DC10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DC20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DC30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DC40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DC50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DC60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DC70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DC80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DC90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DCA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DCB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DCC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DCD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DCE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DCF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DD00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DD10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DD20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DD30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DD40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DD50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DD60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DD70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DD80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DD90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DDA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DDB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DDC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DDD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DDE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DDF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DE00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DE10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DE20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DE30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DE40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DE50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DE60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DE70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DE80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DE90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DEA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DEB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DEC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DED0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DEE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DEF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DF00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DF10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DF20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DF30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DF40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DF50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DF60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DF70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DF80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DF90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DFA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DFB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DFC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DFD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DFE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041DFF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E000);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E010);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E020);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E030);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E040);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E050);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E060);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E070);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E080);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E090);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E0A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E0B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E0C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E0D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E0E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E0F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E100);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E110);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E120);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E130);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E140);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E150);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E160);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E170);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E180);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E190);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E1A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E1B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E1C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E1D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E1E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E1F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E200);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E210);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E220);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E230);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E240);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E250);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E260);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E270);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E280);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E290);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E2A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E2B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E2C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E2D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E2E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E2F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E300);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E310);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E320);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E330);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E340);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E350);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E360);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E370);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E380);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E390);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E3A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E3B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E3C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E3D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E3E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E3F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E400);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E410);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E420);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E430);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E440);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E450);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E460);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E470);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E480);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E490);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E4A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E4B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E4C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E4D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E4E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E4F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E500);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E510);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E520);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E530);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E540);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E550);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E560);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E570);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E580);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E590);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E5A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E5B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E5C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E5D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E5E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E5F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E600);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E610);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E620);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E630);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E640);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E650);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E660);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E670);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E680);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E690);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E6A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E6B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E6C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E6D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E6E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E6F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E700);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E710);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E720);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E730);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E740);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E750);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E760);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E770);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E780);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E790);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E7A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E7B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E7C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E7D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E7E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E7F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E800);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E810);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E820);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E830);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E840);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E850);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E860);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E870);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E880);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E890);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E8A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E8B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E8C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E8D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E8E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E8F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E900);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E910);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E920);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E930);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E940);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E950);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E960);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E970);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E980);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E990);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E9A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E9B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E9C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E9D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E9E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041E9F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EA00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EA10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EA20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EA30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EA40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EA50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EA60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EA70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EA80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EA90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EAA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EAB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EAC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EAD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EAE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EAF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EB00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EB10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EB20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EB30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EB40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EB50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EB60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EB70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EB80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EB90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EBA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EBB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EBC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EBD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EBE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EBF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EC00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EC10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EC20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EC30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EC40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EC50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EC60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EC70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EC80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EC90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ECA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ECB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ECC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ECD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ECE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ECF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ED00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ED10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ED20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ED30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ED40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ED50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ED60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ED70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ED80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041ED90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EDA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EDB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EDC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EDD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EDE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EDF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EE00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EE10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EE20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EE30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EE40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EE50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EE60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EE70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EE80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EE90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EEA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EEB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EEC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EED0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EEE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EEF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EF00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EF10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EF20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EF30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EF40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EF50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EF60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EF70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EF80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EF90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EFA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EFB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EFC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EFD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EFE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041EFF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F000);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F010);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F020);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F030);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F040);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F050);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F060);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F070);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F080);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F098);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F0B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F0C8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F0D8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F0F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F108);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F120);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F138);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F158);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F170);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F188);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F1A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F1B8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F1D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F1E8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F200);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F218);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F230);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F248);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F260);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F270);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F288);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F2A8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F2C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F2D8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F2F8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F310);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F328);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F340);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F358);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F370);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F388);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F3A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F3B8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F3D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F3E8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F400);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F418);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F428);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F440);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F458);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F470);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F488);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F4A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F4B8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F4D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F4E8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F4F8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F508);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F518);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F528);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F538);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F550);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F568);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F580);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F5A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F5C8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F5F8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F620);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F648);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F670);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F698);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F6C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F6E8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F710);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F738);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F760);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F788);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F7B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F7D8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F800);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F828);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F850);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F878);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F8A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F8C8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F8F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F918);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F940);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F968);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F990);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F9B8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041F9E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FA08);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FA30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FA50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FA78);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FAA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FAC8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FAF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FB18);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FB40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FB68);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FB90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FBB8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FBE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FC08);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FC30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FC58);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FC80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FCA8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FCD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FCF8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FD20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FD48);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FD70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FD98);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FDC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FDE8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FE10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FE38);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FE60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FE88);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FEB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FED8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FF00);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FF28);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FF50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FF78);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FFA0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FFC8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_0041FFE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420000);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420020);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420040);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420060);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420080);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004200A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004200C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004200E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420100);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420120);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420140);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420168);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420188);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004201A8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004201C8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004201E8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420208);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420228);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420248);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420268);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420288);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004202A8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004202C8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004202E8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420308);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420328);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420348);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420368);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420388);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004203A8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004203C8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004203E8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420408);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420428);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420448);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420468);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420488);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004204A8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004204C8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004204E8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420508);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420528);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420548);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420568);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420588);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004205A8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004205C8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004205E8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420608);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420628);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420648);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420668);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420688);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004206A8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004206C8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004206E8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420708);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420728);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420750);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420770);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420790);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004207B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004207D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004207F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420810);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420830);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420850);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420870);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420898);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004208C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004208E8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420910);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420930);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420958);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420978);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004209A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004209C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004209E8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420A10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420A38);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420A58);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420A80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420AA8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420AD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420AF8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420B18);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420B40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420B68);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420B90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420BB8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420BE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420C08);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420C30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420C58);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420C80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420CA8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420CD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420CF8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420D18);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420D38);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420D60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420D80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420DA8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420DD0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420DF0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420E10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420E30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420E50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420E78);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420E98);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420EB0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420EC8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420EE0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420EF8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420F10);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420F20);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420F30);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420F40);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420F50);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420F60);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420F70);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420F80);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420F90);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420FA8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420FC0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00420FD8);

