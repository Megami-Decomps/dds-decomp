#include "common.h"

extern s32 D_00435F68;

extern u64 func_0010FFE8(void);

extern s64 func_00110628(u64);

extern u64 func_00110680(u64);

extern s64 func_001106D8(u64);

extern u64 func_00110CD8(u64, u64);

extern s64 func_00114058(u64);

extern u64 func_0032CE80(u64);

extern s32 D_00435DD0;

extern u32 D_00435F60;

extern u32 D_00435F88;

extern u32 D_00435F90;

extern u32 D_00435F70;

extern s16 D_00389170[];

extern u32 D_0038A640[];

extern u32 D_00435F34;

extern u32 D_003898B8[];

extern void func_003298C0(u32 arg0);

extern u32 D_00435F0C;

extern u32 D_00389858[];

void func_00128390(u32 arg0);

extern u32 D_00389770[];

void func_00126730(void);

extern s32 D_0038989C[];

void func_00126958(void);

void func_00126A28(void);

void func_00126AF0(void);

extern char D_00412F30[];

extern s32 func_0010C100(const char *arg0);

extern char D_00412F58[];

extern char D_00412F40[];

extern s32 D_00435F7C;

extern u32 func_002AA4D8(void);

u8 func_001283A8(u32 arg0);

extern s32 D_00435F80;

extern u32 func_0014A250(void);

extern u32 D_00435F6C;

extern void func_001411F8(u32 arg0);

void func_0011F208(u32 *arg0, u32 arg1, u32 arg2) {
    arg0[4] = arg1;
    arg0[5] = arg2;
}

u64 func_0011F218(void) {
    u64 temp_v0;

    temp_v0 = func_0032CE80(0x20);
    func_0032CEC0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011F250);

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011F3D8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011F518);

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011F800);

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011F908);

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011F928);

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011FAB8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011FAD8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011FC68);

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011FC88);

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011FD18);

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011FEE8);

u32 func_001200E0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_001200E8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001203A8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001204D0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00120508);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00120528);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001206D0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00120820);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00120858);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412B90);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001208D0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00120A88);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412BB8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00120B88);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00122828);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00122A38);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00122B58);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00122E50);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00122F38);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00122FE0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123038);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001230E0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123138);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001231E0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123238);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001232E0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123338);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001233E0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123438);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123488);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001234E8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123590);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001235E8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123690);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001236E8);

u8 func_00123788(u32 arg0) {
    return (*(u8 *)(((s32)arg0 >> 3) + D_00435DD0 + 0x110d0) >> (arg0 & 7)) & 1;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_001237B0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123808);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123860);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001238B8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123908);

u32 func_00123960(s32 arg0, s32 arg1) {
    s16 *temp_v0 = D_00389170;
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

INCLUDE_ASM(const s32, "game/code_0011F208", func_001239A8);

s16 * func_00123A10(s32 arg0, s32 arg1) {
    s16 *temp_v0 = D_00389170;
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

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123A58);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123B88);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123DE8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123EE0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00124040);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00124110);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412CA8);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412CF0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001244A8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00124720);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001248E8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001249B8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00124A10);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00124BC0);

void func_00124BF0(s64 arg0) {
    u64 temp_v0;
    s64 temp_v1;
    u64 temp_v2;

    temp_v0 = func_0010FFE8();
    temp_v0 = func_00110CD8(temp_v0, 6);
    temp_v1 = func_00110628(temp_v0);
    if (temp_v1 == 0) {
        return;
    }
    func_001106B8(temp_v0);
    do {
        temp_v2 = func_00110680(temp_v0);
        temp_v1 = func_00114058(temp_v2);
        if (temp_v1 == 4) {
            if (arg0 == 0) {
                func_00114048(temp_v2, 3);
            }
            else {
                func_00114048(temp_v2, 0);
            }
        }
        temp_v1 = func_001106D8(temp_v0);
    } while (temp_v1 != 0);
    func_001101A8(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00124CC8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00124D70);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00124E28);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00124E80);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00124EB8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00124FA0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00125080);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00125180);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00125270);

u32 func_00125328(void) {
    u32 *temp_v0 = D_0038A640;

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

INCLUDE_ASM(const s32, "game/code_0011F208", func_00125380);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412D50);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412D60);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001258B8);

void func_00125AD8(void) {
    if (D_00435F34 != 0) {
        func_003298C0(D_00435F34);
        D_00435F34 = 0;
        D_003898B8[0] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00125B10);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00125D78);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00125E08);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00125E68);

void func_00125EE8(void) {
    func_0013B970();
    func_00120508();
    func_00125E68();
    func_0023A9A8();
    func_001284C8();
    func_00152C18();
}

u32 * func_00125F28(void) {
    return &D_00435F60;
}

u32 func_00125F38(void) {
    return *func_00125F28();
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00125F58);

void func_00126000(void) {
    if (D_00435F0C != 0) {
        func_00128390(0x40);
        func_001129E8(D_00435F0C, 0);
    }
    D_00389858[0] = 4;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00126040);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412EB0);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412EC0);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412ED0);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412EE0);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412EF0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00126110);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001265D0);

u32 func_001266D8(void) {
    return 0;
}

u32 func_001266E0(void) {
    return 100;
}

void func_001266E8(void) {
    if ((*(s32 *)(D_00435DD0 + 0xA58) & 8) != 0) {
        func_00126730();
        D_00389770[3] |= 1;
    }
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00126730);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00126778);

s32 func_00126810(void) {
    if ((*(s32 *)(D_00435DD0 + 0xA58) & 8) != 0) {
        return 1;
    }
    if (D_0038989C[0] != 0) {
        return 0;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00126840);

void func_00126910(void) {
    if ((*(s32 *)(D_00435DD0 + 0xA58) & 4) != 0) {
        func_00126958();
        D_00389770[3] |= 2;
    }
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00126958);

void func_00126998(void) {
    *(s32 *)(D_00435DD0 + 0xA58) |= 4;
    D_00389770[3] &= ~2;
}

u8 func_001269C8(void) {
    s32 temp_v0 = *(s32 *)(D_00435DD0 + 0xA58);
    temp_v0 &= 4;
    return temp_v0 != 0;
}

void func_001269E0(void) {
    if ((*(s32 *)(D_00435DD0 + 0xA58) & 2) != 0) {
        func_00126A28();
        D_00389770[3] |= 4;
    }
}

void func_00126A28(void) {
    *(s32 *)(D_00435DD0 + 0xA58) &= ~2;
    D_00389770[3] &= ~4;
}

void func_00126A58(void) {
    *(s32 *)(D_00435DD0 + 0xA58) = (*(s32 *)(D_00435DD0 + 0xA58) | 2) & ~1;
    D_00389770[3] &= ~4;
}

u8 func_00126A90(void) {
    s32 temp_v0 = *(s32 *)(D_00435DD0 + 0xA58);
    temp_v0 &= 2;
    return temp_v0 != 0;
}

void func_00126AA8(void) {
    if ((*(s32 *)(D_00435DD0 + 0xA58) & 1) != 0) {
        func_00126AF0();
        D_00389770[3] |= 8;
    }
}

void func_00126AF0(void) {
    *(s32 *)(D_00435DD0 + 0xA58) &= ~1;
    D_00389770[3] &= ~8;
}

void func_00126B20(void) {
    *(s32 *)(D_00435DD0 + 0xA58) = (*(s32 *)(D_00435DD0 + 0xA58) | 1) & ~2;
    D_00389770[3] &= ~8;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00126B58);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00126B70);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00126BA0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00126C80);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001272A8);

u8 func_00127320(void) {
    return func_0010C100(D_00412F30) != 0;
}

u8 func_00127348(void) {
    return func_0010C100(D_00412F58) != 0;
}

u8 func_00127370(void) {
    return func_0010C100(D_00412F40) != 0;
}

u8 func_00127398(void) {
    if (D_00435F7C > 0) {
        return 2;
    }
    if (func_002AA4D8() != 0) {
        return 1;
    }
    return func_001283A8(0x20) != 0 ? 0 : 3;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_001273E8);

u8 func_001275D0(void) {
    if (D_00435F80 > 0) {
        return 2;
    }
    return func_0014A250() != 0;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00127600);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001277D8);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412F10);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412F20);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412F30);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412F40);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412F58);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001278D0);

INCLUDE_ASM(const s32, "game/code_0011F208", fldProcDraw);

void func_00128340(u32 arg0) {
    u32 *flags = &D_00435F88;
    *flags |= arg0;
}

void func_00128358(u32 arg0) {
    u32 *flags = &D_00435F88;
    *flags &= ~arg0;
}

u8 func_00128370(u32 arg0) {
    return (D_00435F88 & arg0) != 0;
}

void func_00128380(u32 arg0) {
    D_00435F90 = D_00435F90 | arg0;
}

void func_00128390(u32 arg0) {
    D_00435F90 = D_00435F90 & ~arg0;
}

u8 func_001283A8(u32 arg0) {
    return (D_00435F90 & arg0) != 0;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_001283B8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001284C8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00128580);

void func_001285E8(void) {
    func_00141840();
}

void func_00128600(void) {
    func_00144EE0();
}

void func_00128618(void) {
    if (D_00435F68 != 0) {
        func_00128658();
        return;
    }
    func_00141898();
}

void func_00128648(u32 arg0, u32 arg1) {
    D_00435F68 = arg0;
    D_00435F6C = arg1;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00128658);

void func_001286C0(u32 arg0) {
    D_00435F70 = arg0;
}

void func_001286C8(void) {
    u32 temp_v0;

    temp_v0 = D_00435F70;
    if (temp_v0 != 0) {
        func_001411F8(temp_v0);
        D_00435F70 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_001286F8);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412FD0);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412FE0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00128750);
INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EB8);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EC0);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EC8);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435ED0);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435ED8);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EE0);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EE4);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EE8);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EEC);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EF0);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EF4);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EF8);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EFC);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F00);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F04);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F08);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F0C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F10);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F14);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F18);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F1C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F20);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F24);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F28);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F30);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F34);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F38);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F3C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F40);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F44);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F48);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F50);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F58);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F60);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F68);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F6C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F70);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F74);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F78);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F7C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F80);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F84);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F88);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F8C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F90);


INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F98);

