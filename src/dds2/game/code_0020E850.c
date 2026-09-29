#include "common.h"

extern u32 D_00436CB0;

extern void btlCmdSimpleB(s32, s32);

extern void btlCmdSimpleA(s32, s32);

extern void btlCmdSimpleD(s32, s32);

extern void btlCmdSimpleE(s32, s32);

extern void btlCmdSimpleJ(s32, s32);

extern void btlCmdSimpleC(s32, s32);

extern s32 func_00210EA0(s32 context, s32 actor, u32 mask);

extern s32 D_00436CB8;

extern s32 func_00213818(s32, s32);

extern s32 func_00328E18(s32);

extern void func_00328E48(s32);

extern void func_002152D8(s32, s32);

extern void func_00215C70(s32, s32);

extern void func_00215F28(s32, s32);

extern void func_00216760(s32, s32);

extern void func_00216888(s32, s32);

extern void func_00216D30();

extern void func_00216D10();

extern void func_00216CA0();

extern void func_00216988();

extern void func_00216E98(s32, s32);

extern void func_00216ED0(s32, s32);

extern void func_00216D50();

typedef struct EffChildCounters {
    u8 pad00[0x318];
    u8 firstCountdown;
    u8 secondCountdown;
} EffChildCounters;

typedef struct EffCounterOwner {
    u8 pad00[0x20];
    EffChildCounters *child;
} EffCounterOwner;

void func_0020E850(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020E858);

void func_0020EA10(void) {
}

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020EA18);

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020EB40);

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020EBD0);

INCLUDE_RODATA(const s32, "game/code_0020E850", D_00419910);

INCLUDE_RODATA(const s32, "game/code_0020E850", D_00419920);

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020EC20);

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020F048);

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020F200);

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020F3B0);

s32 func_0020F5C8(s32 arg0, s32 arg1) {
    return arg1 + (((*(s32 *)(arg0 + 0x110) >> 9) ^ 1U) & 1);
}

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020F5E0);

void func_0020F9B0(u8 *arg0) {
    u8 *unit = *(u8 **)(arg0 + 0x20);
    u8 count = unit[0x338];
    if (count != 0) {
        unit[0x338] = count - 1;
    }
}

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020F9D0);

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020FA98);

void func_0020FEF8(u8 *arg0) {
    u8 *unit = *(u8 **)(arg0 + 0x20);
    u8 count = unit[0x339];
    if (count != 0) {
        unit[0x339] = count - 1;
    }
}

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020FF18);

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020FFB8);

INCLUDE_ASM(const s32, "game/code_0020E850", func_002100B8);

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210148);

INCLUDE_ASM(const s32, "game/code_0020E850", func_002101C8);

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210258);

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210360);

INCLUDE_ASM(const s32, "game/code_0020E850", func_002103F8);

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210498);

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210530);

INCLUDE_ASM(const s32, "game/code_0020E850", func_002105F0);

u32 func_00210688(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210690);

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210720);

INCLUDE_ASM(const s32, "game/code_0020E850", func_002107C0);

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210850);

INCLUDE_ASM(const s32, "game/code_0020E850", func_002108E8);

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210980);

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210A28);

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210AA8);

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210B78);

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210C10);

u32 func_00210CB0(u32 limit) {
    D_00436CB0 = D_00436CB0 * 0x41c64e6d + 0x3039;
    return (D_00436CB0 >> 0x10) * (limit & 0xffff) >> 0x10;
}

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210CE0);

s32 btlAllocAndCheck(s32 object) {
    s32 allocation = func_00328E18(0x10);
    s32 actor = *(s32 *)(object + 0x18);

    D_00436CB8 = allocation;
    *(s32 *)allocation = object;
    if (func_00213818(actor, 0) != 0) {
        func_00328E48(D_00436CB8);
        return 1;
    }
    func_00328E48(D_00436CB8);
    return 0;
}

u32 func_00210DB0(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0xb;
    *(u32 *)(arg0 + 0x24) = 0xc2;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210DC8);

extern s8 D_00436CAC;

void func_00210E48(void) {
    u8 *node = *(u8 **)(func_001AA6F8() + 0x248);
    if (node != 0) {
        do {
            if (*(s32 *)(node + 0x18) != 0) {
                node[0x14E] = 0;
                *(s32 *)(node + 0x170) = 0;
            }
            node = *(u8 **)(node + 0x178);
        } while (node != 0);
    }
    D_00436CAC = 0;
}

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210EA0);

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210F10);

INCLUDE_ASM(const s32, "game/code_0020E850", func_00210F58);

INCLUDE_ASM(const s32, "game/code_0020E850", func_00211018);

void btlCmdWithArgA(s32 arg0) {
    func_002152D8(arg0, 0);
}

void btlCmdWithArgB(s32 arg0) {
    func_00215C70(arg0, 0);
}

void btlCmdWithArgC(s32 arg0) {
    func_00215F28(arg0, 0);
}

void func_002110E8(void) {
    func_00216D50();
}

void func_00211108(s32 arg0) {
    func_00216ED0(arg0, 0);
}

extern void func_00215D78(s32, s32);

void btlCmdSimpleA(s32 context, s32 value) {
    func_00215D78(context, value);
}

void btlCmdWithArgD(s32 arg0) {
    func_00216760(arg0, 0);
}

void btlCmdWithArgE(s32 arg0) {
    func_00216888(arg0, 0);
}

extern void func_00217028(s32, s32);

void btlCmdSimpleB(s32 context, s32 value) {
    func_00217028(context, value);
}

extern void func_002160A0(s32, s32);

void btlCmdSimpleC(s32 context, s32 value) {
    func_002160A0(context, value);
}

extern void func_002161B0(s32, s32);

void btlCmdSimpleD(s32 context, s32 value) {
    func_002161B0(context, value);
}

extern void func_002162C0(s32, s32);

void btlCmdSimpleE(s32 context, s32 value) {
    func_002162C0(context, value);
}

void btlCmdSimpleG(void) {
    func_00216D10();
}

void func_00211228(void) {
    func_00216D30();
}

void btlCmdSimpleH(void) {
    func_00216CA0();
}

void btlCmdSimpleI(void) {
    func_00216988();
}

void btlCmdWithArgF(s32 arg0) {
    func_00216E98(arg0, 0);
}

extern void func_00216F08(s32, s32);

void btlCmdSimpleJ(s32 context, s32 value) {
    func_00216F08(context, value);
}

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436C78);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436C80);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436C88);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436C90);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436C98);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436CA0);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436CA8);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436CAC);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436CB0);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436CB4);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436CB8);
