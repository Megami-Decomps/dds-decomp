#include "common.h"

extern s32 D_00435E38;

extern s32 func_0011AE90(void);

extern u32 D_00435E88;

extern s32 D_00435DEC;

extern s32 D_00435DD0;

typedef struct Entry1A4 {
    u16 unk0; /* 0x0 */
    u8 pad2[2]; /* 0x2 */
    u16 unk4; /* 0x4 */
    u8 pad6[14]; /* 0x6 */
    u16 unk14; /* 0x14 */
    u8 pad16[398]; /* 0x16 */
} Entry1A4;

typedef struct TableEntry32 {
    u16 unk0; /* 0x0 */
    u16 unk2; /* 0x2 */
} TableEntry32;

extern TableEntry32 D_00386248[];

typedef struct Entry4 {
    u16 unk0; /* 0x0 */
    u8 unk2; /* 0x2 */
    u8 pad3; /* 0x3 */
} Entry4;

extern Entry4 D_003862C8[];

extern void func_0011A118(s32 arg0, s32 arg1);

extern void func_0011CA88(Entry1A4 *arg0);

extern s32 D_0043E5C8[];

extern s32 func_0010D818(s32 arg0);

extern s32 D_0043E5CC[];

extern s32 func_0010D650(s32 idx);

extern s32 func_00119A78(s32 arg0, s32 arg1);

extern s32 D_00435E20;

extern s32 D_0043E5C4[];

extern s32 D_0043E5C0[];

extern s32 D_0043E5D0[];

extern void func_0010D830(f32 arg0);

extern s32 func_003412A0(u32 arg0, u32 arg1);

extern u8 func_001AA308(void);

extern f32 func_001AD978(void);

extern u32 func_001ADA10(void);

extern s32 D_00435E8C;

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A118);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A1D0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A220);

u8 func_0011A288(s32 arg0) {
    if ((*(u16 *)arg0 & 0x20) == 0) {
        return 0;
    }
    return *(u8 *)(D_00435DEC + *(u16 *)(arg0 + 4) * 76 + 4);
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A2C8);

s8 func_0011A318(s32 arg0) {
    return *(s8 *)(arg0 + D_00435DD0 + 0xa76);
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A328);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A510);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A700);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A788);

void func_0011A7F8(s32 arg0, s32 arg1) {
    *(s32 *)(arg0 + 0x10) = *(s32 *)(arg0 + 0x10) + arg1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A808);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A938);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A9C0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AA58);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AB38);

u16 func_0011AE60(s32 arg0) {
    return *(u16 *)(arg0 * 8 + D_00435E38 + 2);
}

u16 func_0011AE78(s32 arg0) {
    s32 temp = *(u16 *)(arg0 + 0x14);

    return temp < 100 ? temp : 99;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AE90);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AEE0);

u8 func_0011B260(void) {
    s64 temp_v0;

    temp_v0 = func_0011AE90();
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B280);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B2C0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B328);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B4B0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B6F0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B9A0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011BC80);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C0B0);

void func_0011C328(u32 arg0) {
    func_0011C0B0(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C340);

void func_0011C680(u32 arg0) {
    func_0011C340(arg0, 0);
}

u32 func_0011C698(void) {
    return 0;
}

u32 func_0011C6A0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C6A8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C868);

void func_0011C978(s32 arg0) {
    *(u16 *)(arg0 + 0x52) = D_00386248[*(u16 *)(arg0 + 4)].unk0;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C998);

void func_0011CA28(void) {
    Entry4 *p = D_003862C8;
    u32 i = 0;

    do {
        u16 a0 = p->unk0;
        u8 a1 = p->unk2;

        p++;
        if (a0 != 0) {
            func_0011A118(a0, a1);
        }
        i++;
    } while (i < 2);
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011CA88);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011CFE8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D050);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D0D8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D130);

void func_0011D2A0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_003412A0(0, 4);
    *(s32 *)(arg0 + 0x1b4) = 0x12 - temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D2D8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D360);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D3B8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D3E8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D438);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D4C8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D558);

u32 func_0011D588(void) {
    return D_00435E88;
}

void func_0011D590(void) {
    func_0010BF48(D_00435E88);
    D_00435E88 = 0;
}

s32 func_0011D5B8(void) {
    func_0010D818(*(u16 *)(D_0043E5C8[0] + 0x14));
    return 1;
}

s32 func_0011D5E0(void) {
    func_0010D818(*(u16 *)(D_0043E5CC[0] + 0x14));
    return 1;
}

s32 func_0011D608(void) {
    func_0010D818(*(u16 *)(D_0043E5C8[0] + 6));
    return 1;
}

s32 func_0011D630(void) {
    func_0010D818(*(u16 *)(D_0043E5CC[0] + 6));
    return 1;
}

s32 func_0011D658(void) {
    func_0010D818(*(u16 *)(D_0043E5C8[0] + 8));
    return 1;
}

s32 func_0011D680(void) {
    func_0010D818(*(u16 *)(D_0043E5CC[0] + 8));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D6A8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D758);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D808);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D8B0);

s32 func_0011D958(void) {
    s32 val = func_0010D650(0);

    func_0010D818(func_00119A78(D_0043E5C8[0], val));
    return 1;
}

s32 func_0011D990(void) {
    s32 val = func_0010D650(0);

    func_0010D818(func_00119A78(D_0043E5CC[0], val));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D9C8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DA50);

s32 func_0011DAE0(void) {
    func_0010D818(*(u8 *)(D_00435E20 + D_0043E5C4[0] * 56 + 0x2d));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DB20);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DBA0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DC38);

s32 func_0011DCC0(void) {
    func_0010D818(*(s16 *)(D_00435E20 + D_0043E5C4[0] * 56 + 0x36));
    return 1;
}

s32 func_0011DD00(void) {
    u16 bits = *(u16 *)D_0043E5C0 | 1;

    *(u16 *)D_0043E5C0 = bits;
    D_0043E5C0[4] = func_0010D650(0);
    return 1;
}

s32 func_0011DD40(void) {
    func_0010D818(D_0043E5D0[0]);
    return 1;
}

s32 func_0011DD68(void) {
    func_0010D818((((*(u16 *)D_0043E5C8[0]) >> 5) ^ 1) & 1);
    return 1;
}

s32 func_0011DDA0(void) {
    s32 v0 = func_0010D650(0);
    s32 val = func_003412A0(0, v0 * 2);

    func_0010D830((f32)(val - v0 + 100) / 100.0f);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DE08);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DE58);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DEA8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DEF8);

s32 func_0011DF60(void) {
    s32 v0 = func_001AA308();
    f32 val = 0.0f;

    if (v0 != 0) {
        val = func_001AD978();
    }
    func_0010D830(val);
    return 1;
}

s32 func_0011DFA0(void) {
    s32 v0 = func_001AA308();
    s32 val = 0;

    if (v0 != 0) {
        val = func_001ADA10();
    }
    func_0010D818(val);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DFE0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E018);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E050);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E128);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E160);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E198);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E1D0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E208);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E268);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E2A8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E3A0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E430);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E498);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E528);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E728);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E848);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E930);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011EBC8);

void func_0011EBE0(void) {
}

void func_0011EBE8(void) {
}

void func_0011EBF0(void) {
}

void func_0011EBF8(void) {
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011EC00);
