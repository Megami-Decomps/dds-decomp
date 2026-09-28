#include "common.h"

extern void func_0025DF68(s32, s32);

extern void func_00261760(s32);
extern s32 D_003BAA00;

extern void func_00260530(s32, u32);
extern void func_0025ECD0();
extern u8 D_0036AB64[];

extern void func_0025E108(s32, s32);

extern void func_002605B0(s32, u32);
extern void func_00260570(s32, u32);
extern void func_00260AB0(s32);
extern void func_0025F138();
extern s32 func_00245A40(s32);
extern void func_0024DED8(s32);
extern s32 func_0024DEF8(s32, s32);
extern void func_002E96D8(s32);

extern void func_00260670(s32 arg0);

extern void func_0024DD78(void);
extern void func_002858E8(s32, s32);
extern u8 D_0036AA68[];
extern u8 D_0036AA84[];
extern u8 D_0036AAA0[];
extern u8 D_0036AABC[];

extern s32 func_00105C48(void);

extern s64 func_0024DC08(void);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern s32 func_00101A70();

s32 func_00245C98(void) {
    s32 temp_v0 = func_00105C48();

    if (temp_v0 != 0) {
        return 0;
    }
    return func_0024DC08() == 0;
}

void func_00245CC8(s32 arg0) {
    if (*(s32 *)(arg0 + 0x84) == 2) {
        *(s32 *)(arg0 + 0x58) = (s32)D_0036AA68;
        func_002858E8(arg0 + 0x54, (s32)D_0036AA68 + 0xC4);
    }
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00245D08);

s32 func_00245DA0(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();

    if (temp_v0[43] == 1) {
        func_002605B0((s32)temp_v0, 4);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00245C98", D_003AF418);

INCLUDE_RODATA(const s32, "game/code_00245C98", D_003AF428);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00245DE0);

void func_00246088(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_00260670(temp_v0);
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void func_002460D8(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

void func_00246120(s32 arg0) {
    if (*(s32 *)(arg0 + 0x84) == 1) {
        *(s32 *)(arg0 + 0x58) = (s32)D_0036AA84;
        func_002858E8(arg0 + 0x54, (s32)D_0036AA84 + 0xA8);
    }
}

s32 func_00246160(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();

    if (temp_v0[43] == 1) {
        func_002453C8(temp_v0);
    }
    return 1;
}

s32 func_00246198(void) {
    s32 temp_v0 = func_00101A70();
    s32 temp_v1 = *(s32 *)(temp_v0 + 0xAC);

    if (temp_v1 == 5) {
        func_002605B0(temp_v0, 3);
    } else if (temp_v1 == 7) {
        s32 temp_v2;

        func_002605B0(temp_v0, 9);
        temp_v2 = *(s32 *)(*(s32 *)(temp_v0 + 0x70) + 0x14);
        *(s32 *)(temp_v2 + 0x2C) = (s32)func_0025F138;
        func_00260570(temp_v2, 10);
    }
    *(s16 *)(temp_v0 + 0xB0) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246220);

void func_00246460(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_00260AB0(temp_v0);
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void func_002464B0(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

void func_002464F8(s32 arg0) {
    if (*(s32 *)(arg0 + 0x84) == 1) {
        *(s32 *)(arg0 + 0x58) = (s32)D_0036AAA0;
        func_002858E8(arg0 + 0x54, (s32)D_0036AAA0 + 0x8C);
    }
}

s32 func_00246538(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();

    if (temp_v0[43] == 1) {
        func_002457E8(temp_v0);
    }
    return 1;
}

s32 func_00246570(void) {
    s32 temp_v0 = func_00101A70();
    s32 temp_v1 = *(s32 *)(temp_v0 + 0xAC);

    if (temp_v1 == 5) {
        func_002605B0(temp_v0, 3);
    } else if (temp_v1 == 7) {
        s32 temp_v2;

        func_002605B0(temp_v0, 9);
        temp_v2 = *(s32 *)(*(s32 *)(temp_v0 + 0x70) + 0x14);
        *(s32 *)(temp_v2 + 0x2C) = (s32)func_0025F138;
        func_00260570(temp_v2, 10);
    }
    *(s16 *)(temp_v0 + 0xB0) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_002465F8);

void func_00246838(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_00260AB0(temp_v0);
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void func_00246888(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

void func_002468D0(s32 arg0) {
    if (*(s32 *)(arg0 + 0x84) == 2) {
        *(s32 *)(arg0 + 0x58) = (s32)D_0036AABC;
        func_002858E8(arg0 + 0x54, (s32)D_0036AABC + 0x70);
    }
}

s32 func_00246910(void) {
    s32 temp_v0 = func_00101A70();

    if ((*(s32 *)(temp_v0 + 0xAC) == 1) && (func_00245A40(temp_v0) == 0)) {
        *(s32 *)(temp_v0 + 0x84) = 2;
    }
    return 1;
}

s32 func_00246968(void) {
    s32 temp_v0 = func_00101A70();
    s32 temp_v1 = *(s32 *)(temp_v0 + 0xAC);

    if (temp_v1 == 5) {
        func_002605B0(temp_v0, 3);
    } else if (temp_v1 == 7) {
        s32 temp_v2;

        func_002605B0(temp_v0, 9);
        temp_v2 = *(s32 *)(*(s32 *)(temp_v0 + 0x70) + 0x14);
        *(s32 *)(temp_v2 + 0x2C) = (s32)func_0025F138;
        func_00260570(temp_v2, 10);
    }
    *(s16 *)(temp_v0 + 0xB0) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_002469F0);

void func_00246BE8(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_00260AB0(temp_v0);
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void func_00246C38(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

u32 func_00246C80(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(u32 *)(temp_v0 + 0xa4) = 0;
    func_0027BB28(*(u32 *)(*(s32 *)(temp_v0 + 0x6c) + 0x14));
    return 1;
}

s64 func_00246CB0(u64 arg0) {
    s32 temp_v0 = func_00101A70();
    s64 temp_v1 = func_00285670(temp_v0 + 8, temp_v0 + 0x54, 0, arg0);

    if (temp_v1 == 0) {
        if ((*(s32 *)(temp_v0 + 0x54) == 0) && (func_0024DC08() == 0)) {
            s32 temp_v2 = *(s32 *)(temp_v0 + 0xA4);

            if ((f32)temp_v2 < 20.0f) {
                *(s32 *)(temp_v0 + 0xA4) = temp_v2 + 1;
            } else {
                func_002858E8(temp_v0 + 0x54, (s32)D_0036AB64);
            }
        }
        temp_v1 = 0;
    }
    return temp_v1;
}

void func_00246D68(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0025E108(temp_v0, *(s32 *)(temp_v0 + 0xA4));
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void func_00246DB8(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

s32 func_00246E00(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();

    temp_v0[32] = 1;
    func_00245190(-1, temp_v0);
    return 1;
}

s32 func_00246E38(void) {
    s32 temp_v0 = func_00101A70();

    if (*(s32 *)(temp_v0 + 0xAC) == 0xA) {
        s32 temp_v1;

        *(s16 *)(temp_v0 + 0xB0) = 0xA;
        func_002605B0(temp_v0, 6);
        temp_v1 = *(s32 *)(*(s32 *)(temp_v0 + 0x70) + 0x14);
        *(s32 *)(temp_v1 + 0x2C) = (s32)func_0025ECD0;
        func_00260530(temp_v1, 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246EA0);

void func_002470F8(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_00261760(temp_v0);
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void func_00247148(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247190);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002472D8);

void func_002473D0(s32 arg0) {
    s32 temp_v0 = *(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x70) + 0x14) + 0x1C) + 0x60;

    if ((u32)(*(s32 *)(temp_v0 + 4) - 0x60) < 0x20) {
        *(s32 *)(D_003BAA00 + 0xA50) += *(s32 *)temp_v0 * *(s32 *)(arg0 + 0x80);
    }
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247420);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247588);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247728);

void func_002478B0(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_002478F8);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247970);

s64 func_002479F0(u64 arg0) {
    s32 temp_v0;
    s64 temp_v1;
    s32 *piVar3;

    temp_v0 = func_00101A70();
    piVar3 = (s32 *)(temp_v0 + 0x54);
    temp_v1 = func_00285670(temp_v0 + 8, piVar3, 0, arg0);
    if (temp_v1 == 0) {
        if ((*piVar3 == 0) && (temp_v1 = func_0024DC08(), temp_v1 == 0)) {
            func_002858F8(piVar3, *(u32 *)(temp_v0 + 0x58));
        }
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247A78);

void func_00247CB0(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

u32 func_00247CF8(void) {
    func_00220110(0x323);
    func_00105AE0(0, 0, 0, 0xf);
    func_0024DED8(0);
    func_0024DEF8(0, 0);
    func_0024DEF8(1, 1);
    return 1;
}

u32 func_00247D50(void) {
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00245C98", D_003AF528);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247D58);

void func_00248088(s32 arg0) {
    s32 temp_v0 = func_00101A70();
    s32 temp_v1 = *(s32 *)(temp_v0 + 0xA4);

    if (temp_v1 != 0) {
        func_0025DF68(temp_v0, temp_v1);
    }
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void func_002480E8(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

u32 func_00248130(void) {
    func_0024DED8(0);
    func_0024DEF8(0, 1);
    func_0024DEF8(1, 0);
    func_002E96D8(0x300000);
    return 1;
}

u32 func_00248170(void) {
    func_00105AE0(0, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_002481A0);

void func_00248240(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void func_00248278(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

u32 func_002482B0(void) {
    return 1;
}

u32 func_002482B8(void) {
    return 1;
}

u32 func_002482C0(void) {
    return 0;
}

u32 func_002482C8(void) {
    return 0;
}

u32 func_002482D0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_002482D8);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002483C0);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00248468);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00248508);
