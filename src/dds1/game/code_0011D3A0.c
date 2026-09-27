#include "common.h"

extern u32 D_003BABE0;

extern s32 D_003BABD8;

extern u32 D_003BAC00;

extern u32 D_003BABF8;

extern u32 D_003BABD0;

extern u64 func_0010FDC0(void);
extern s64 func_00110400(u64);
extern u64 func_00110458(u64);
extern s64 func_001104B0(u64);
extern u64 func_00110AB0(u64, u64);
extern s64 func_00113E30(u64);

extern s32 D_003BAA00;

extern u64 func_002D3FD0(u64);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011D3A0);

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

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011DAA0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011DAC0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011DC50);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011DC70);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011DE00);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011DE20);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011DEB0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E080);

u32 func_0011E278(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E280);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E540);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E668);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E6A0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E6C0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E810);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E960);

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

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121048);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001210A0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121148);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001211A0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121248);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001212A0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121348);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001213A0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121448);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001214A0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001214F0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121550);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001215F8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121650);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001216F8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121750);

u8 func_001217F0(u32 arg0) {
    return (*(u8 *)(((s32)arg0 >> 3) + D_003BAA00 + 0x15970) >> (arg0 & 7)) & 1;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121818);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121870);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001218C8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121920);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121970);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001219C8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121A10);

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

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123378);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001233D0);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FBC0);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FBD0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001238B8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123990);

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

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123DE0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123E00);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123EA8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123EE8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123FB8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001242B8);

u32 func_001243C0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001243C8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124488);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001244D0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124500);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124590);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001245C0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124680);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001246C8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001246F8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124728);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124740);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124788);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001247B8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001247F0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124808);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124850);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124880);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001248B8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001248D0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124900);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001249E0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124E18);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124E90);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124EB8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124EE0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124F08);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124F58);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00125140);

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

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00125D90);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00125DA8);

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

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00126098);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001260A8);

void func_00126108(u32 arg0) {
    D_003BABE0 = arg0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00126110);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00126140);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FD30);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FD40);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00126198);
