#include "common.h"

extern u32 D_003BABE0;

extern s32 D_003BABD8;

extern u32 D_003BAC00;

extern u32 D_003BABF8;
extern s32 D_003BABF0;

extern u32 D_003BABD0;

extern u64 func_0010FDC0(void);
extern s64 func_00110400(u64);
extern u64 func_00110458(u64);
extern s64 func_001104B0(u64);
extern u64 func_00110AB0(u64, u64);
extern s64 func_00113E30(u64);

extern s32 D_003BAA00;

extern u64 func_002D3FD0(u64);
extern u32 D_003BABDC;
extern s32 D_003BAB08;
extern s32 D_003BAB0C;
extern s32 D_003BAB10;
extern s32 D_003BAB14;
extern s32 D_003BAB18;
extern s32 D_003BAB1C;
extern s32 D_003BAB20;
extern u32 D_0032E3B0[];
extern s32 D_0032E4DC[];
extern u8 D_00324F88[];
extern u8 D_003257F8[];
extern u8 D_00324530[];
extern char D_0039FA00[];
extern char D_0039FCA0[];
extern char D_0039FCB0[];
extern char D_0039FCC8[];
extern void func_0011D998(f32 *arg0, u8 *arg1);
extern void func_0011DAC0(u32, u32, u32, u32, u32, u32, u8 *);
extern void func_0011DC70(u32 *arg0, s32 arg1, u8 *arg2);
extern void func_0011E540(void);
extern void func_0012E6F0(void);
extern void func_0013E5A8(u32 arg0);
extern u32 func_001462D0(void);
extern s32 func_0010BED8(const char *arg0);
extern void func_002D8C88(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);
extern u32 D_003BAB34;
extern u32 D_003BAB54;
extern u32 D_0032E498[];
extern u32 D_0032E4EC[];
extern void func_002D0A10(u32 arg0);
extern s32 D_003BABEC;
extern u32 D_0032F1A0[];
extern s16 D_0032DDB0[];
extern u32 func_00272228(void);
void func_001244D0(void);
void func_001246C8(void);
void func_00124788(void);
void func_00124850(void);
void func_00125DE0(u32 arg0);
u8 func_00125DF8(u32 arg0);

void func_0011D3A0(u32 *arg0, u32 arg1, u32 arg2) {
    arg0[4] = arg1;
    arg0[5] = arg2;
}

u64 func_0011D3B0(void) {
    u64 temp_v0;

    temp_v0 = func_002D3FD0(0x20);
    func_002D4010(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011D3E8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011D570);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011D6B0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011D998);

void func_0011DAA0(f32 *arg0) {
    func_0011D998(arg0, D_00324530);
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011DAC0);

void func_0011DC50(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f) {
    func_0011DAC0(a, b, c, d, e, f, D_00324530);
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011DC70);

void func_0011DE00(u32 *arg0, s32 arg1) {
    func_0011DC70(arg0, arg1, D_00324530);
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011DE20);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011DEB0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E080);

u32 func_0011E278(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E280);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E540);

void func_0011E668(void) {
    kwlnTaskCreate((s32)D_0039FA00, 0x2AF8, 0, 0, (s32)func_0011E540, 0, 0);
}

void func_0011E6A0(void) {
    kwlnTaskDestroyWithHierarchyByName(D_0039FA00, 1);
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E6C0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E810);

void func_0011E960(void) {
    D_003BAB20 = 0;
    D_003BAB08 = -999;
    D_003BAB0C = -999;
    D_003BAB10 = -999;
    D_003BAB14 = -999;
    D_003BAB18 = -999;
    D_003BAB1C = -999;
    func_0012E6F0();
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E998);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FA00);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011EA10);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011EBC8);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FA28);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011ECC8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00120968);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00120AE8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00120C08);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00120EC8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00120FA0);

u8 func_00121048(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    u8 *temp_v1;
    u32 temp_v2;

    if (arg0 < 0x28) {
        temp_v0 = arg0 % 100;
        temp_v1 = (u8 *)(arg1 * 30 + temp_v0 * 1920);
        temp_v1 += D_003BAA00;
        temp_v2 = *(u16 *)(temp_v1 + 0x1370);
        return (temp_v2 >> arg2) & 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001210A0);

u8 func_00121148(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    u8 *temp_v1;
    u32 temp_v2;

    if (arg0 < 0x28) {
        temp_v0 = arg0 % 100;
        temp_v1 = (u8 *)(arg1 * 30 + temp_v0 * 1920);
        temp_v1 += D_003BAA00;
        temp_v2 = *(u16 *)(temp_v1 + 0x1372);
        return (temp_v2 >> arg2) & 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001211A0);

u8 func_00121248(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    u8 *temp_v1;
    u32 temp_v2;

    if (arg0 < 0x28) {
        temp_v0 = arg0 % 100;
        temp_v1 = (u8 *)(arg1 * 30 + temp_v0 * 1920);
        temp_v1 += D_003BAA00;
        temp_v2 = *(u16 *)(temp_v1 + 0x1374);
        return (temp_v2 >> arg2) & 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001212A0);

u8 func_00121348(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    u8 *temp_v1;
    u32 temp_v2;

    if (arg0 < 0x28) {
        temp_v0 = arg0 % 100;
        temp_v1 = (u8 *)(arg1 * 30 + temp_v0 * 1920);
        temp_v1 += D_003BAA00;
        temp_v2 = *(u16 *)(temp_v1 + 0x1376);
        return (temp_v2 >> arg2) & 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001213A0);

u8 func_00121448(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    u8 *temp_v1;
    u32 temp_v2;

    if (arg0 < 0x28) {
        temp_v0 = arg0 % 100;
        temp_v1 = (u8 *)(arg1 * 30 + temp_v0 * 1920);
        temp_v1 += D_003BAA00;
        temp_v2 = *(u16 *)(temp_v1 + 0x1378);
        return (temp_v2 >> arg2) & 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001214A0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001214F0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121550);

u8 func_001215F8(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    u8 *temp_v1;
    u32 temp_v2;

    if (arg0 < 0x28) {
        temp_v0 = arg0 % 100;
        temp_v1 = (u8 *)(arg1 * 30 + temp_v0 * 1920);
        temp_v1 += D_003BAA00;
        temp_v2 = *(u16 *)(temp_v1 + 0x138A);
        return (temp_v2 >> arg2) & 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121650);

u8 func_001216F8(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    u8 *temp_v1;
    u32 temp_v2;

    if (arg0 < 0x28) {
        temp_v0 = arg0 % 100;
        temp_v1 = (u8 *)(arg1 * 30 + temp_v0 * 1920);
        temp_v1 += D_003BAA00;
        temp_v2 = *(u16 *)(temp_v1 + 0x138C);
        return (temp_v2 >> arg2) & 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121750);

u8 func_001217F0(u32 arg0) {
    return (*(u8 *)(((s32)arg0 >> 3) + D_003BAA00 + 0x15970) >> (arg0 & 7)) & 1;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121818);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121870);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001218C8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121920);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121970);

u32 func_001219C8(s32 arg0, s32 arg1) {
    s16 *temp_v0 = D_0032DDB0;
    s32 temp_v1 = 0;
    s32 temp_v2 = 0;

    do {
        if (temp_v0[0] == arg0 && temp_v0[1] == arg1) {
            return temp_v1;
        }
        temp_v1++;
        temp_v2++;
        temp_v0 += 8;
    } while (temp_v2 < 0x280);
    return temp_v1;
}

s16 * func_00121A10(s32 arg0, s32 arg1) {
    s16 *temp_v0 = D_0032DDB0;
    s32 temp_v1 = 0;

    do {
        if (temp_v0[0] == arg0 && temp_v0[1] == arg1) {
            return temp_v0;
        }
        temp_v1++;
        temp_v0 += 8;
    } while (temp_v1 < 0x60);
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121A58);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121B88);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121DE0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121ED8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122030);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122100);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FB18);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FB60);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122498);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122710);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001228D8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001229A8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122A00);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122BB0);

void func_00122BE0(s64 arg0) {
    u64 temp_v0;
    s64 temp_v1;
    u64 temp_v2;

    temp_v0 = func_0010FDC0();
    temp_v0 = func_00110AB0(temp_v0, 6);
    temp_v1 = func_00110400(temp_v0);
    if (temp_v1 == 0) {
        return;
    }
    func_00110490(temp_v0);
    do {
        temp_v2 = func_00110458(temp_v0);
        temp_v1 = func_00113E30(temp_v2);
        if (temp_v1 == 4) {
            if (arg0 == 0) {
                func_00113E20(temp_v2, 3);
            }
            else {
                func_00113E20(temp_v2, 0);
            }
        }
        temp_v1 = func_001104B0(temp_v0);
    } while (temp_v1 != 0);
    func_0010FF80(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122CB8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122D60);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122E08);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122ED0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122F08);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122FF0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001230D0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001231D0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001232C0);

u32 func_00123378(void) {
    u32 *temp_v0 = D_0032F1A0;

    if (temp_v0[0xD] == 1) {
        return 1;
    }
    if (temp_v0[0x17] == 3) {
        return 3;
    }
    if (temp_v0[0x17] == 4) {
        return 4;
    }
    if (temp_v0[0x17] == 5) {
        return 5;
    }
    if (temp_v0[0x17] == 6) {
        return 2;
    }
    return temp_v0[0xD] != 0 ? 2 : 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001233D0);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FBC0);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FBD0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001238B8);

void func_00123990(void) {
    if (D_003BAB54 != 0) {
        func_002D0A10(D_003BAB54);
        D_003BAB54 = 0;
        D_0032E4EC[0] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001239C8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123C30);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123CC0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123D20);

void func_00123D98(void) {
    func_00138D88();
    func_0011E6A0();
    func_00123D20();
    func_0021FE38();
    func_00125F18();
}

u32 * func_00123DD0(void) {
    return &D_003BABD0;
}

u32 func_00123DE0(void) {
    return *func_00123DD0();
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123E00);

void func_00123EA8(void) {
    if (D_003BAB34 != 0) {
        func_00125DE0(0x40);
        func_001127C0(D_003BAB34, 0);
    }
    D_0032E498[0] = 4;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123EE8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123FB8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001242B8);

u32 func_001243C0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001243C8);

void func_00124488(void) {
    if ((*(s32 *)(D_003BAA00 + 0xA58) & 8) != 0) {
        func_001244D0();
        D_0032E3B0[3] |= 1;
    }
}

void func_001244D0(void) {
    *(s32 *)(D_003BAA00 + 0xA58) &= ~8;
    D_0032E3B0[3] &= ~1;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124500);

s32 func_00124590(void) {
    if ((*(s32 *)(D_003BAA00 + 0xA58) & 8) != 0) {
        return 1;
    }
    if (D_0032E4DC[0] != 0) {
        return 0;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001245C0);

void func_00124680(void) {
    if ((*(s32 *)(D_003BAA00 + 0xA58) & 4) != 0) {
        func_001246C8();
        D_0032E3B0[3] |= 2;
    }
}

void func_001246C8(void) {
    *(s32 *)(D_003BAA00 + 0xA58) &= ~4;
    D_0032E3B0[3] &= ~2;
}

void func_001246F8(void) {
    *(s32 *)(D_003BAA00 + 0xA58) |= 4;
    D_0032E3B0[3] &= ~2;
}

u8 func_00124728(void) {
    s32 temp_v0 = *(s32 *)(D_003BAA00 + 0xA58);
    temp_v0 &= 4;
    return temp_v0 != 0;
}

void func_00124740(void) {
    if ((*(s32 *)(D_003BAA00 + 0xA58) & 2) != 0) {
        func_00124788();
        D_0032E3B0[3] |= 4;
    }
}

void func_00124788(void) {
    *(s32 *)(D_003BAA00 + 0xA58) &= ~2;
    D_0032E3B0[3] &= ~4;
}

void func_001247B8(void) {
    *(s32 *)(D_003BAA00 + 0xA58) = (*(s32 *)(D_003BAA00 + 0xA58) | 2) & ~1;
    D_0032E3B0[3] &= ~4;
}

u8 func_001247F0(void) {
    s32 temp_v0 = *(s32 *)(D_003BAA00 + 0xA58);
    temp_v0 &= 2;
    return temp_v0 != 0;
}

void func_00124808(void) {
    if ((*(s32 *)(D_003BAA00 + 0xA58) & 1) != 0) {
        func_00124850();
        D_0032E3B0[3] |= 8;
    }
}

void func_00124850(void) {
    *(s32 *)(D_003BAA00 + 0xA58) &= ~1;
    D_0032E3B0[3] &= ~8;
}

void func_00124880(void) {
    *(s32 *)(D_003BAA00 + 0xA58) = (*(s32 *)(D_003BAA00 + 0xA58) | 1) & ~2;
    D_0032E3B0[3] &= ~8;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001248B8);

void func_001248D0(void) {
    func_002D8C88(D_003257F8, (s32)D_00324F88, (s32)(D_00324F88 + 0xC0), (s32)(D_00324F88 + 0x100), (s32)(D_00324F88 + 0xE0));
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124900);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001249E0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124E18);

u8 func_00124E90(void) {
    return func_0010BED8(D_0039FCA0) != 0;
}

u8 func_00124EB8(void) {
    return func_0010BED8(D_0039FCC8) != 0;
}

u8 func_00124EE0(void) {
    return func_0010BED8(D_0039FCB0) != 0;
}

u8 func_00124F08(void) {
    if (D_003BABEC > 0) {
        return 2;
    }
    if (func_00272228() != 0) {
        return 1;
    }
    return func_00125DF8(0x20) != 0 ? 0 : 3;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124F58);

u8 func_00125140(void) {
    if (D_003BABF0 > 0) {
        return 2;
    }
    return func_001462D0() != 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00125170);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00125348);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC40);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC50);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC60);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC70);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC80);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC90);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FCA0);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FCB0);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FCC8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldProcSequence);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldProcDraw);

void func_00125D90(u32 arg0) {
    u32 *flags = &D_003BABF8;
    *flags |= arg0;
}

void func_00125DA8(u32 arg0) {
    u32 *flags = &D_003BABF8;
    *flags &= ~arg0;
}

u8 func_00125DC0(u32 arg0) {
    return (D_003BABF8 & arg0) != 0;
}

void func_00125DD0(u32 arg0) {
    D_003BAC00 = D_003BAC00 | arg0;
}

void func_00125DE0(u32 arg0) {
    D_003BAC00 = D_003BAC00 & ~arg0;
}

u8 func_00125DF8(u32 arg0) {
    return (D_003BAC00 & arg0) != 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00125E08);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00125F18);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00125FD0);

void func_00126038(void) {
    func_0013EC10();
}

void func_00126050(void) {
    func_00141D18();
}

void func_00126068(void) {
    if (D_003BABD8 != 0) {
        func_001260A8();
        return;
    }
    func_0013EC68();
}

void func_00126098(u32 arg0, u32 arg1) {
    D_003BABD8 = arg0;
    D_003BABDC = arg1;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001260A8);

void func_00126108(u32 arg0) {
    D_003BABE0 = arg0;
}

void func_00126110(void) {
    u32 temp_v0;

    temp_v0 = D_003BABE0;
    if (temp_v0 != 0) {
        func_0013E5A8(temp_v0);
        D_003BABE0 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00126140);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FD30);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FD40);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00126198);



INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAAE0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAAE8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAAF0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAAF8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB00);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB08);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB0C);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB10);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB14);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB18);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB1C);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB20);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB24);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB28);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB2C);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB30);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB34);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB38);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB3C);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB40);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB50);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB54);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB58);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB5C);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB60);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB64);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB68);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB70);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB78);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB80);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB88);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB90);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB98);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABA0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABA8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABB0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABB8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABC0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABD0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABD8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABDC);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABE0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABE4);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABE8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABEC);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABF0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABF4);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABF8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABFC);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAC00);


INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAC08);

