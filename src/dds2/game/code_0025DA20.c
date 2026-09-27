#include "common.h"

extern s32 D_00437C9C;

extern s32 func_002C4BA8(u16);

extern u64 func_0011D360(u64, s32);

extern s32 D_00435E5C;

extern s64 func_002ACF38(void);

extern s32 func_002A46C8(s32);

extern u32 D_00438FE8;

extern u32 D_00438FC8;

extern u32 *D_00437960;

extern u64 func_00279DC8(u32, u64, u64, u64, u64, u64);
extern u64 func_0027A628(u32, u64, u64);
extern s32 func_00291400(u64, u64);

extern u64 func_00312810(u32, u64);

extern u32 func_00284AE0(void);

extern u32 func_002843C0(u32);

extern s32 func_0026FDC8(u32, u32);

extern s32 D_0043789C;

extern s32 D_00437898;
extern s32 func_0026CD50(u32);

extern s32 func_0023A170(u32);

extern u8 func_00264B58(void);

extern s8 D_003CD8D8[34];

extern s64 func_0025CF70(void);

extern s32 func_00101820(u32);

extern s32 D_00435DD0;

extern s32 func_00101958(void);

extern s64 func_0026C768(void);

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern s32 func_00268BE0(void);

extern u32 func_00343ED0(u32, u32 *, u32);

extern s32 D_00437880;

extern u8 D_00437884;

extern u32 D_00437924;

extern u8 D_0043798A;

extern u8 D_0043798B;

extern u32 func_0029D790(u32, s32);

extern u64 func_0010D650(u64);

extern u64 func_00342688(void);

extern s32 func_002A2330(void);

extern u32 D_00437A2C;

extern s32 D_00437A40;

extern u16 D_00435BAC;

extern u32 *D_00437AB0;

extern s32 func_0032CD98(void);

extern u32 D_00437AE8;

extern s32 func_00105B68(void);

extern s32 func_002C6CE8(void);

extern s32 func_002B86E8(u32);

extern u32 func_002BC120(u32);

extern u32 func_002B9FF8(u32);

extern u32 func_00304998(u32);

extern s32 D_00435E20;

extern s32 D_00435E1C;

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025DA20);

void func_0025DAB0(void) {
    s64 temp_v0;

    temp_v0 = func_0025CF70();
    if (temp_v0 != 0) {
        kwlnTaskDestroyWithHierarchy(temp_v0, 0);
        return;
    }
}

void func_0025DAE8(void) {
    s64 temp_v0;

    while (temp_v0 = func_00101820(0x3ec), temp_v0 != 0) {
        kwlnTaskDestroyWithHierarchy(temp_v0, 0);
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025DB20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025DB98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025DD68);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025DE08);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025DFE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E048);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E240);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E288);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E338);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E390);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E460);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E6F0);

u32 func_0025E7B0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E7B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E7D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E858);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E8D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E980);

void func_0025EBC8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x2034);
    if (temp_v0 != 0) {
        *(u32 *)(temp_v0 + 0x28) = 0;
        while (temp_v0 = *(s32 *)(temp_v0 + 0x7c), temp_v0 != 0) {
            *(u32 *)(temp_v0 + 0x28) = 0;
        }
    }
    *(u32 *)(arg0 + 0x240c) = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025EC00);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025ED10);

void func_0025EE00(s32 arg0) {
    func_0025EC00();
    if (*(s32 *)(arg0 + 0x23cc) == 1) {
        func_00137888();
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025EE40);

void func_0025EE90(s32 arg0) {
    func_0019C5B0(*(u32 *)(arg0 + 0x2410));
    *(u32 *)(arg0 + 0x2410) = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025EEC0);

void func_0025EEE8(s32 arg0) {
    if ((*(s32 *)(arg0 + 0x2430) == 0) || (*(s32 *)(arg0 + 0x2430) == 5)) {
        *(u32 *)(arg0 + 0x2430) = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025EF10);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025EFD8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F0B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F130);

void func_0025F2B0(s32 arg0) {
    func_0025F130(*(u32 *)(arg0 + 0x2438));
}

void func_0025F2C8(void) {
}

void func_0025F2D0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x243c) = (*(u32 *)(arg0 + 0x243c) & 0xfffffffc) | (arg1 & 3);
}

u32 func_0025F2F0(s32 arg0) {
    return *(u32 *)(arg0 + 0x243c) & 3;
}

void func_0025F300(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x243c) = (*(u32 *)(arg0 + 0x243c) & 0xfffffff3) | ((arg1 & 3) << 2);
}

u32 func_0025F320(s32 arg0) {
    return (*(u32 *)(arg0 + 0x243c) & 0xc) >> 2;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F330);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F440);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F490);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F4F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F568);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F5D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F640);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F708);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F7F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F868);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A00);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A10);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A20);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A30);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A40);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A50);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A60);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A70);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A80);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A90);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424AC0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F8B8);

void func_0025FA10(s32 arg0) {
    func_00304A38(*(u32 *)(arg0 + 0x3c));
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FA28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FC08);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FCD8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FD78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FE70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FF18);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260020);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260138);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002601D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260250);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260380);

u32 func_00260458(void) {
    return 0;
}

u32 func_00260460(void) {
    return 0;
}

s32 func_00260468(void) {
    u16 temp_v0;
    u16 *puVar2;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = 0;
    temp_v1 = 4;
    puVar2 = (u16 *)(D_00435DD0 + 0xa60);
    do {
        temp_v0 = *puVar2;
        puVar2 = puVar2 + 0xe2;
        temp_v1 = temp_v1 - 1;
        temp_v2 = temp_v2 + (temp_v0 & 1);
    } while (-1 < temp_v1);
    return temp_v2;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002604A0);

void func_00260538(void) {
    u16 temp_v0;
    s8 *pcVar2;
    u32 temp_v1;

    temp_v1 = 0;
    pcVar2 = D_003CD8D8;
    do {
        temp_v0 = *(u16 *)pcVar2;
        pcVar2 = (s8 *)((s32)pcVar2 + 8);
        temp_v1 = temp_v1 + 1;
        *(u8 *)((u32)temp_v0 + D_00435DD0 + 0x1340) = 0;
    } while (temp_v1 < 3);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260570);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260620);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002606A0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BA0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BC0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BD0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260708);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260808);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260848);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260880);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002608E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260918);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260950);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260A58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260B50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260B90);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260C28);

s32 func_00260C38(s32 arg0, s32 arg1) {
    return arg1 * 2 + arg0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260C48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260CE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260D50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260DA0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260DF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260FE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261018);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261040);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002610C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002610E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261198);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261290);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261310);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002613C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261480);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261538);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261670);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261850);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002619A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261B98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261D78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261E10);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261E40);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261E80);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261F08);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261F48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262190);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002621E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262230);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262270);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002622A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262330);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262598);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002625F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262638);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262678);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002626B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262740);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002629A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262A00);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262A48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262A88);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262AC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262B50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262DB8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262E10);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262E58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262E98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262EF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00262F78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263180);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002631D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263220);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263270);

u32 func_00263378(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263380);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002633F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263448);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263490);

u32 func_002635E0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002635E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263658);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002636B0);

u32 func_002636F8(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    *(u32 *)(temp_v0 + 0xb8) = 0;
    func_002B8988(*(u32 *)(*(s32 *)(temp_v0 + 0x7c) + 0x18));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263728);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002637E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263838);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263880);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002638B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263920);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263B98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263BF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263C38);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263D40);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263DD0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263E60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263EB0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263F10);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263F50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263FB0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264120);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264240);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002642B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264300);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264378);

s64 func_002643F8(u64 arg0) {
    s32 temp_v0;
    s64 temp_v1;
    s32 *piVar3;

    temp_v0 = func_00101958();
    piVar3 = (s32 *)(temp_v0 + 0x58);
    temp_v1 = func_002C4038(temp_v0 + 0xc, piVar3, 0, arg0);
    if (temp_v1 == 0) {
        if ((*piVar3 == 0) && (temp_v1 = func_0026C768(), temp_v1 == 0)) {
            func_002C42C0(piVar3, *(u32 *)(temp_v0 + 0x5c));
        }
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264480);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002646C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264710);

u32 func_00264848(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264850);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264AB8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264B10);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264B58);

u32 func_00264C00(void) {
    u8 temp_v0;
    s32 temp_v1;

    temp_v1 = func_00101958();
    temp_v0 = func_00264B58();
    *(u8 *)(temp_v1 + 0xcc) = temp_v0;
    if ((*(s32 *)(temp_v1 + 200) == 0) && (*(s8 *)(temp_v1 + 0xcd) == '\x01')) {
        func_0026C5B8(0x22);
    }
    return 1;
}

u32 func_00264C58(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264C60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264CE0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264D38);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264D80);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264E18);

u32 func_00264EF8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264F00);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264F98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00264FF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00265038);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00265130);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00265178);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002651C0);

u32 func_002652D8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002652E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00265360);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002653B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00265408);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00265500);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002655C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002657F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00265850);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00265898);

u32 func_00265980(void) {
    return 1;
}

u32 func_00265988(s32 arg0) {
    u32 temp_v0;
    s64 temp_v1;

    if (((*(s32 *)(arg0 + 8) == 2) && (temp_v1 = func_0023A170(4), temp_v1 != 0)) &&
          (temp_v1 = func_0023A170(0x290), temp_v1 == 0)) {
        func_0023A0F0(0x290);
        temp_v0 = 1;
    }
    else {
        temp_v0 = 0;
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002659E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00265A60);

void func_00265AB8(void) {
    func_0026C948(1);
    func_0026C5B8(0xe);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00265AD8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00265E78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00265EE8);

u32 func_00265F30(void) {
    func_0026C948(1);
    func_0026C5B8(0xd);
    return 1;
}

u32 func_00265F58(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00265F60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00265FE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00266038);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00266080);

u32 func_002660D8(void) {
    func_00105A00(0, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00266108);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00266188);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002661D8);

u32 func_00266210(void) {
    return 1;
}

u32 func_00266218(void) {
    return 1;
}

u32 func_00266220(void) {
    return 0;
}

u32 func_00266228(void) {
    return 0;
}

u32 func_00266230(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00266238);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00266320);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002663D8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424D50);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424D60);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424D70);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424D80);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424D90);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424DA0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424DB0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424DC0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424DD0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424DE0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424E10);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00266460);

void func_002665B0(s32 arg0) {
    func_00304A38(*(u32 *)(arg0 + 0x3c));
}

u8 func_002665C8(void) {
    s64 temp_v0;

    temp_v0 = func_0023A170(0x31);
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002665E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00266808);

void func_002668C0(s32 arg0) {
    if (*(s32 *)(arg0 + 0x3f4) == 0) {
        func_00304EE0(*(u32 *)(arg0 + 100));
        func_00304EE0(*(u32 *)(arg0 + 0x68));
        func_00304EE0(*(u32 *)(arg0 + 0x6c));
        func_00304EE0(*(u32 *)(arg0 + 0x70));
        return;
    }
    func_00304EE0(*(u32 *)(arg0 + 100));
    func_00304EE0(*(u32 *)(arg0 + 0x68));
}

void func_00266928(s32 arg0, u32 arg1) {
    if (*(s32 *)(arg0 + 0x3f4) == 0) {
        func_00304FB0(*(u32 *)(arg0 + 100));
        func_00304FB0(*(u32 *)(arg0 + 0x68), arg1);
        func_00304FB0(*(u32 *)(arg0 + 0x6c), arg1);
        func_00304FB0(*(u32 *)(arg0 + 0x70), arg1);
        return;
    }
    func_00304FB0(*(u32 *)(arg0 + 100));
    func_00304FB0(*(u32 *)(arg0 + 0x68), arg1);
}

void func_002669B0(u32 arg0) {
    func_00266928(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002669C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00266A48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00266AF8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00266B48);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424E60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00266C08);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00266F70);

void func_00267008(s32 arg0) {
    if (arg0 != 0) {
        func_002C21F8();
        func_002C21F8((s32)arg0 + 0x50);
        func_00328E48(arg0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267050);

void func_002670C8(s32 arg0) {
    s32 temp_v0;

    for (temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x7c) + 0x10); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0x58)
            ) {
        func_00267008(*(u32 *)(temp_v0 + 0x70));
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267110);

void func_002671E8(s32 arg0) {
    func_002B81C8(*(u32 *)(arg0 + 0x7c));
}

void func_00267200(s32 arg0) {
    func_00267008(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x7c) + 0x1c) + 0x70));
    func_002B86E8(*(u32 *)(arg0 + 0x7c));
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267238);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267358);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002673B8);

u32 func_002674F8(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267500);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002675C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267680);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002676F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267768);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002678C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267938);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002679E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267A00);

void func_00267A80(u32 *arg0) {
    func_002B2860(arg0 + 2);
    func_002A9788(arg0 + 2);
    func_00303D58(arg0[1]);
    func_003297C8(*arg0);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267AC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267B40);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267C48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267D30);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267DA0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267DE0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267E00);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267EA0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267F68);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00267F88);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00268090);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002680E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00268128);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424E98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00268178);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00268278);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00268318);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424F00);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424F10);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424F20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00268380);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00268470);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002684B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002684F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00268550);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00268588);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002685C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002685F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002686D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002686F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00268838);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002689A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002689D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00268AA0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00268B48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00268BE0);

u8 func_00268C08(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_00268BE0();
    return *(u8 *)(temp_v0 * 0xa0 + *(s32 *)(*(s32 *)(arg0 + 100) + 0x18) + 0x14);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00268C48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00268CC0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00268EC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002690A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00269230);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424F40);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424F58);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424F88);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00269418);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00269478);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00269638);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002698A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00269978);

void func_00269AF8(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(arg1 + 0xcc);
    *(u32 *)(arg1 + 0xcc) = arg0;
    *(u32 *)(arg1 + 0xd0) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00269B08);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00269B80);

u32 func_00269C48(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00269C50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00269E98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00269F28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00269F70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00269FC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026A048);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026A138);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026A170);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026A1B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026A258);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026A2E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026A3F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026A468);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026A4B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026A528);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026A598);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026A728);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026A808);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026A890);

u32 func_0026A8D8(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    func_002B8988(*(u32 *)(temp_v0 + 0x78));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026A900);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026A998);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026AA78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026AAC0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026AB38);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026ABB0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026AC90);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026AD00);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026AD38);

u32 func_0026ADC0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026ADC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026AEB0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026AF20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026AF68);

s64 func_0026AFE0(u64 arg0) {
    s32 temp_v0;
    s64 temp_v1;
    s32 *piVar3;

    temp_v0 = func_00101958();
    piVar3 = (s32 *)(temp_v0 + 0x54);
    temp_v1 = func_002C4038(temp_v0 + 8, piVar3, 0, arg0);
    if (temp_v1 == 0) {
        if ((*piVar3 == 0) && (temp_v1 = func_0026C768(), temp_v1 == 0)) {
            func_002C42B0(piVar3, *(u32 *)(temp_v0 + 0x58));
        }
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026B068);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026B0D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026B120);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026B1C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026B260);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026B358);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026B3F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026B430);

u32 func_0026B4A0(void) {
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424FC8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424FF8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026B4A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026B5F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026B648);

void func_0026B680(s32 arg0) {
    func_0026C728();
    func_0026C538(*(u32 *)(arg0 + 0x60));
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026B6A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026B8F0);

u32 func_0026B930(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026B938);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026B9D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026BA20);

u32 func_0026BA68(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    if (*(s32 *)(temp_v0 + 0xe4) == 0) {
        func_00105AB8(0, 0, 0, 0xf);
    }
    else {
        func_00105AB8(0, 0, 0, 0xf);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026BAB8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026BAF8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026BB78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026BBC8);

u32 func_0026BC00(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    *(u32 *)(temp_v0 + 0x98) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026BC28);

u32 func_0026BC68(void) {
    return 0;
}

u32 func_0026BC70(void) {
    return 0;
}

u32 func_0026BC78(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026BC80);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026BD38);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026BD50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026BE28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026BEB0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026BEC0);

u32 func_0026C168(void) {
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C170);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C1D0);

void func_0026C240(void) {
    func_00106810(0, 0, 0);
    func_00106C28(0);
    func_00106E60(0);
    func_00196FE8();
    func_00197070();
    func_00197388();
    func_00197128();
    func_00197320();
}

void func_0026C298(void) {
    func_00241898();
    func_0026C240();
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C2B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C2D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C318);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C388);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C3E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C408);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C458);

void func_0026C4F0(u32 arg0, u32 *arg1) {
    u32 temp_v0;

    temp_v0 = func_00343ED0(arg0, arg1 + 1, 0);
    *arg1 = temp_v0;
}

void func_0026C520(u32 *arg0) {
    func_003297C8(*arg0);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C538);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C580);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C5B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C618);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C648);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C660);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C668);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C6A0);

u32 func_0026C6A8(s32 arg0) {
    u32 temp_v0;

    temp_v0 = 0;
    if (-1 < D_00437880) {
        func_001A2E88(D_00437880, 0);
        if (arg0 != 0) {
            func_001A34D0(D_00437880);
        }
        func_001A3CC8(D_00437880, 0);
        func_0026C948(1);
        D_00437884 = 0;
        temp_v0 = 1;
    }
    return temp_v0;
}

void func_0026C710(void) {
    func_0026C6A8(1);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C728);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C768);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C7B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C7F8);

void func_0026C8E8(u32 arg0) {
    func_0026C7F8(arg0, 1);
}

void func_0026C900(void) {
    func_0026C8E8(1);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C918);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C940);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C948);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026C9B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026CA00);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026CA60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026CA70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026CA80);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026CAA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026CAD0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026CB00);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026CB48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026CB98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026CC88);

void func_0026CD20(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_00308808(arg0, arg1, 0, arg2, arg3, 0x30303040, 0x53);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026CD50);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425018);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026CE90);

void func_0026CF08(u32 arg0) {
    if (D_00437898 != 0) {
        func_0026CF48();
    }
    D_00437898 = func_0026CD50(arg0);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026CF48);

s32 func_0026CF70(s32 arg0) {
    return *(s32 *)(D_00437898 + 4) + ((arg0 << 0x10) >> 0xb);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026CF88);

u32 func_0026D020(void) {
    return *(u32 *)(D_00437898 + 8);
}

void func_0026D030(u32 arg0) {
    if (D_0043789C != 0) {
        func_0026D070();
    }
    D_0043789C = func_0026CD50(arg0);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026D070);

s32 func_0026D098(s32 arg0) {
    return *(s32 *)(D_0043789C + 4) + ((arg0 << 0x10) >> 0xb);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026D0B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026D148);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026D168);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026D4C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026D590);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026D710);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026D7E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026D988);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026DA90);

void func_0026DB20(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026DB28);

void func_0026DB48(u32 arg0, u8 arg1) {
    func_003163A0(arg0, arg1, 0xf);
}

void func_0026DB68(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026DB70);

void func_0026DB90(u32 arg0) {
    func_003163A0(arg0, 0, 0);
}

void func_0026DBB0(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026DBB8);

void func_0026DBD8(u32 arg0) {
    func_003163A0(arg0, 0, 1);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026DBF8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026DC48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026DE08);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026DF48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026E060);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026E198);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026E2D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026E470);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026E4C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026E508);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026E560);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026E5F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026E650);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026E688);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026E6E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026E788);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026E998);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026EBA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026EDB0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026EEE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026F008);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026F138);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026F190);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026F1F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026F570);

u32 func_0026F5B0(s32 arg0) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(arg0 + 0x10);
    func_00328E48();
    return temp_v0;
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004250D8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004250E8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004250F8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425108);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026F5D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026F680);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026F700);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026F778);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026F7E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026F870);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026F8A0);

void func_0026FAA8(s32 arg0) {
    *(u32 *)(arg0 + 0x2c) = 0x1e;
}

u32 func_0026FAB8(void) {
    return 0;
}

u32 func_0026FAC0(void) {
    return 0;
}

void func_0026FAC8(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026FAD0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026FB68);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026FBD8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026FC40);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026FC88);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026FDC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0026FE18);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00270008);

void func_00270050(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 0);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

void func_00270078(u32 arg0, s8 arg1) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 0);
    temp_v0 = *(s32 *)(temp_v0 + 0x20);
    *(u32 *)(temp_v0 + 4) = (*(u32 *)(temp_v0 + 4) & 0xffffff0f) | (((s32)arg1 & 0xfU) << 4);
    *(u8 *)(temp_v0 + 5) = 5;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002700D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00270100);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00270128);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00270160);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002701D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00270210);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00270390);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00270568);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00270848);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00270C78);

void func_00270CC0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 4);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

void func_00270CE8(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 4);
    **(u16 **)(temp_v0 + 0x20) = 6;
}

void func_00270D10(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 4);
    **(u16 **)(temp_v0 + 0x20) = 5;
}

void func_00270D38(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 4);
    **(u16 **)(temp_v0 + 0x20) = 2;
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004251C8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425258);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00270D60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00270DB0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00270DD8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00270F10);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00270FA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00271020);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00271068);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002710A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002710D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002711F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00271250);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00271290);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002712E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00271348);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00271368);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00271510);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00272BD0);

void func_00272C18(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 6);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425338);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425348);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425370);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425380);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004253A8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004253B8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425408);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425420);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425448);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425458);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425490);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00272C40);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00272C78);

void func_00272CB0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 6);
    temp_v0 = *(s32 *)(temp_v0 + 0x20);
    *(u16 *)(temp_v0 + 0xc) = 10;
    *(u16 *)(temp_v0 + 2) = *(u16 *)(temp_v0 + 2) ^ 1;
}

void func_00272CE8(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 6);
    temp_v0 = *(s32 *)(temp_v0 + 0x20);
    *(u16 *)(temp_v0 + 0xc) = 10;
    *(u16 *)(temp_v0 + 0xe) = *(u16 *)(temp_v0 + 0xe) ^ 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00272D20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00272D80);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00272DA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00272F08);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00273450);

void func_00273498(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 7);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002734C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00273530);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002735A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002735F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00273640);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00273668);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00273828);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00273F88);

void func_00273FD0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 8);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00273FF8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00274070);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002740E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00274158);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002741A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002741D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002743B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002745E0);

void func_00274628(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 9);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00274650);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00274690);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002746F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00274760);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00274788);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002747B0);

u32 func_002747F0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 9);
    func_0026FAA8(*(s32 *)(temp_v0 + 0x20) + 0xc);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00274820);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00274890);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002748D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00274A70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00274D88);

void func_00274DD0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 10);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

void func_00274DF8(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 10);
    temp_v0 = *(s32 *)(temp_v0 + 0x20);
    *(u32 *)(temp_v0 + 0xc) = 0xf;
    *(u16 *)(temp_v0 + 2) = *(u16 *)(temp_v0 + 2) ^ 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00274E30);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00274E88);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00274EA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00274FF8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00275218);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00275358);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00275510);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00275598);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002755B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002756E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002758B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002759F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00275B70);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425828);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00275CE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00277F38);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00278D80);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00278DC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00278DF8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00278E50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00278EA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00278EE0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00278F18);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00278F60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00278FA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00278FF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279028);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279080);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002790B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002790F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279148);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279180);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002792D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279308);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279440);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279488);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002795E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279628);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279778);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002797C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002797F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279848);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002798A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002798D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279910);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279968);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002799A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002799D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279C38);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279C58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279DC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279F30);

void func_00279F70(u32 *arg0) {
    *arg0 = (*arg0 & 0xfffc03ff) | 0x800;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00279F90);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004258B8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004258F0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425928);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027A080);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027A198);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027A4C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027A628);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027A798);

void func_0027A7F0(void) {
}

void func_0027A7F8(void) {
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425988);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004259C0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004259F8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425A30);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027A800);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027AF40);

void func_0027AFD8(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027AFE0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425A98);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425AB8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425AC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027B678);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027C078);

void func_0027C108(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027C110);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027C2F0);

void func_0027C418(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002843C0(1);
    *(u32 *)(arg1 + 0x24) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027C448);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027C468);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027C558);

void func_0027CD78(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002843C0(2);
    *(u32 *)(arg1 + 0x24) = temp_v0;
    *(u8 *)(arg1 + 0x20) = 0;
    *(u8 *)(arg1 + 0x21) = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027CDB0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027CDD0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425B68);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425B78);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425B88);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027D3D8);

void func_0027DE30(void) {
}

void func_0027DE38(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027DE40);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425BC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027E0E0);

void func_0027E208(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002843C0(0);
    *(u32 *)(arg1 + 0x24) = temp_v0;
    temp_v0 = func_00284AE0();
    *(u32 *)(arg1 + 0x28) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027E240);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027E270);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027E360);

void func_0027FB60(void) {
}

void func_0027FB68(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027FB70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027FC90);

void func_0027FDB8(void) {
}

void func_0027FDC0(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0027FDC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00280390);

void func_002803A0(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002803A8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425C98);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425CA8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425CB8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002805E0);

void func_002817B8(u32 arg0, s32 arg1) {
    *(u8 *)(arg1 + 0x20) = 0;
}

void func_002817C0(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002817C8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425CF8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00281B60);

void func_00281C88(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002843C0(1);
    *(u32 *)(arg1 + 0x24) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00281CB8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00281CD8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00281DC0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00282B50);

void func_00282B60(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00282B68);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425D68);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00283090);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00283F90);

void func_00283FA0(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00283FA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00284298);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002843C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002844E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00284508);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00284818);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00284AE0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00284B48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00284B70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00284F00);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002850B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00285120);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00285148);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00285500);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002856E0);

u8 func_00285748(s32 arg0) {
    return *(s32 *)(arg0 + 0x6c) != 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00285758);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00285788);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00285800);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002858A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00285A60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00285AC0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00285BD8);

void func_00285CB8(s32 arg0) {
    if (arg0 != 0) {
        func_003297C8(*(u32 *)arg0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00285CE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00285D78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00285E98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002860D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00286270);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002862B0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00425DD8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00286350);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00286410);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002864D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00286530);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002865A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00286618);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00286670);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002866C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00286700);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00286738);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00286A58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00286BA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00286E20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00286E98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00286F18);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00286F90);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00286FD8);

void func_00287008(void) {
    func_003126D0(D_00437924);
    D_00437924 = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00287030);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00426060);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00287078);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00287600);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00287638);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00287670);

u64 func_00287768(void) {
    u64 temp_v0;

    temp_v0 = func_00312810(D_00437924, 0xffffffffffffffff);
    func_00288920(temp_v0);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00287798);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002877E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00287848);

u64 func_00287900(void) {
    u64 temp_v0;

    temp_v0 = func_00312810(D_00437924, 0xffffffffffffffff);
    func_0028B1B0(temp_v0);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00287930);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00287AF8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00287C20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00288158);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002882B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002884C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002885E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00288710);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00426280);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00288748);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00288920);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00288A70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00288BD8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00288DD0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00288F60);

u32 func_00289058(s32 arg0) {
    return *(u32 *)(*(s32 *)(*(s32 *)(arg0 + 4) + 0x1c) + 0x70);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00289068);

u32 func_002890A8(s32 arg0) {
    return **(u32 **)(*(s32 *)(arg0 + 4) + 0x1c);
}

u32 func_002890B8(s32 arg0) {
    func_002B8D10(*(u32 *)(arg0 + 4));
    func_002B8F98(*(u32 *)(arg0 + 4));
    return 1;
}

u32 func_002890F0(s32 arg0) {
    func_002B8CF0(*(u32 *)(arg0 + 4));
    func_002B8F98(*(u32 *)(arg0 + 4));
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004262B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00289128);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002891C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002893A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00289550);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00289710);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00289928);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00289B40);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00289BA0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00289DC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00289ED0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00289F58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00289FA0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00426370);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00426380);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028A018);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028A0F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028A178);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004263E0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004263F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028A1D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028B1B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028B318);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028B738);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028BAF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028BB80);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028C8F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028CBF8);

void func_0028CCA8(s32 arg0) {
    *(u16 *)(arg0 + 0x822) = 0;
    *(u8 *)(arg0 + 0x821) = 1;
}

void func_0028CCB8(s32 arg0) {
    *(u16 *)(arg0 + 0x822) = 0;
    *(u8 *)(arg0 + 0x821) = 2;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028CCC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028CD50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028D070);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028D2F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028D7C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028DC08);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028DE10);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028DFA0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028E0E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028E350);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028E568);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028E638);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00427218);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00427258);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028E858);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028EB38);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00427278);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004272C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028EF50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028F128);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004272F8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00427330);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00427340);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00427360);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00427380);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028F380);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028F570);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028F770);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00427428);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028F7B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028F8A8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00427488);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004274B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028F9A0);

u32 func_0028FD08(void) {
    return 0;
}

void func_0028FD10(u32 arg0) {
    func_0028FD30(arg0, 0xffffffffffffffff, 0xffffffffffffffff);
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00427560);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028FD30);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0028FEF0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004275B0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004275E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00290240);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00290328);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00290410);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00427668);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002906E0);

u32 func_00290A70(s32 arg0) {
    return *(u32 *)(arg0 + 0x7a0);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00290A78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00290B28);

u16 func_00290B98(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(**(s32 **)(*(s32 *)(arg0 + 4) + 0x1c) * 4 + arg0 + 0x7ac) + 8);
    if (*(s32 *)(arg0 + 0x7a4) != 0) {
        return *(u16 *)(*(s16 *)(*(s32 *)(arg0 + 0x7a4) + 2) * 2 + temp_v0);
    }
    return *(u16 *)(*(s16 *)(*(s32 *)(arg0 + 0x7a0) + 2) * 2 + temp_v0);
}

u16 func_00290BF0(s32 arg0, s32 arg1) {
    return *(u16 *)
                    (((arg1 << 0x10) >> 0xf) +
                    *(s32 *)(*(s32 *)(**(s32 **)(*(s32 *)(arg0 + 4) + 0x1c) * 4 + arg0 + 0x7ac) + 8));
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00290C20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00290E48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00291038);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004276C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00291118);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002911D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00291288);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00291338);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00291400);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00291460);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00291510);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00291590);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002917C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00291A20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00291C68);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00291DD0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00292458);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00427720);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00427740);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00292478);

void func_00292998(s32 arg0) {
    u32 temp_v0;
    s32 temp_v1;
    u64 temp_v2;

    temp_v0 = *(u32 *)(arg0 + 0xbec);
    func_0026D098(0);
    temp_v1 = func_00291400(0, 8);
    temp_v2 = func_0027A628(temp_v0, 8, 0);
    func_0027A798(temp_v2, 7, 0);
    temp_v2 = func_00279DC8(temp_v0, 8, 1, 0, 0, 0);
    func_00279F30(temp_v2, 0, 0, 0, 0x80, 0x53, 0, 0);
    func_0027A798(temp_v2, 8, 0);
    *(u16 *)(temp_v1 + 2) = (*(u16 *)(temp_v1 + 2) & 0xfff0) | 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00292A60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00292B90);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00292BB0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00292C58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00292CF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00292EA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00292FF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00293148);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002932B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00293360);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002933A8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004277A0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00427858);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00427888);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004278C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002933F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00293DB0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00293FD0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00294060);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00294420);

void func_00294488(void) {
    if (D_00437960 != (u32 *)0x0) {
        func_003297C8(*D_00437960);
    }
    D_00437960 = (u32 *)0x0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002944B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00294538);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00294580);

void func_002945B8(u32 arg0) {
    D_00438FC8 = arg0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002945C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00294680);

u32 func_00294730(s32 arg0) {
    u32 temp_v0;

    temp_v0 = 0;
    if ((*(s32 *)(arg0 + 8) == 1) || (*(s32 *)(arg0 + 8) == 3)) {
        temp_v0 = 0x3a;
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00294758);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00294930);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00294B40);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00294C68);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00294D50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00294EB8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00295030);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00295400);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002958B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00295D38);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00295F88);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00296018);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002960F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00296298);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00296430);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002967A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002968B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002969D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00296AF8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00296B48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00296C58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00296D90);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00296E98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00297000);

void func_002971C0(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 0x30);
    if (puVar1 != (u32 *)0x0) {
        *puVar1 = arg1;
        puVar1[1] = 1;
    }
}

void func_002971E0(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 0x30);
    if (puVar1 != (u32 *)0x0) {
        *puVar1 = arg1;
        puVar1[1] = 2;
    }
}

void func_00297200(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 0x30);
    if (puVar1 != (u32 *)0x0) {
        *puVar1 = arg1;
        puVar1[1] = 1;
    }
}

void func_00297220(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 0x30);
    if (puVar1 != (u32 *)0x0) {
        *puVar1 = arg1;
        puVar1[1] = 2;
    }
}

void func_00297240(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xc0) = arg1;
    *(u32 *)(arg0 + 0xbc) = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00297250);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00297320);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00297898);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00297970);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00298570);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00298648);

void func_00298E20(void) {
    func_00299748();
    D_0043798A = 0;
    D_0043798B = 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00298E48);

u32 func_00298E50(void) {
    D_0043798A = 1;
    return 1;
}

void func_00298E60(void) {
    func_00299868();
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00298E78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00298E80);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00298EA8);

void func_00298F08(s32 arg0) {
    func_0011A0D0(*(u32 *)(arg0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00298F20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299018);

void func_00299180(u32 arg0, u32 arg1, u32 arg2) {
    func_00298EA8(arg1);
    func_00298F08(arg1);
    func_00299018(arg0, arg2);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002991D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299280);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428358);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299320);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002993D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299488);

void func_00299518(u32 arg0, u32 arg1, u32 arg2) {
    func_00299488(arg0, arg1, 2);
    func_00299488(arg0, arg2, 1);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299558);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299578);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002996B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299748);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002997F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299868);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002998A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002998D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299988);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299A00);

void func_00299A38(u32 arg0, s32 arg1) {
    func_002C4430(arg1 + 0x584);
    func_002BCA98(arg1 + 0x690);
    func_002BCAB0(arg1 + 0x690);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299A70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299B20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299B98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299D58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299E30);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299E90);

u32 func_00299EF0(void) {
    return 1;
}

u32 func_00299EF8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299F00);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299F50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299FA0);

u32 func_00299FD8(void) {
    return 1;
}

u32 func_00299FE0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00299FE8);

u32 func_0029A088(u32 arg0) {
    u32 temp_v0;
    s32 temp_v1;

    temp_v1 = (s32)arg0;
    temp_v0 = func_0029D790(**(u32 **)(temp_v1 + 0x9c), temp_v1 + 0x4e8);
    *(u32 *)(temp_v1 + 0x268) = temp_v0;
    func_00299FE8(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029A0C8);

u32 func_0029A1E0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029A1E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029A270);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029A2F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029A400);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029A588);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029A5D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029A620);

u32 func_0029A650(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029A658);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029A748);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029A768);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029A898);

void func_0029AA48(s32 arg0) {
    func_002B7F80(arg0 + 0xad40, 0x20);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029AA68);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029AC20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029AD98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029AE10);

u32 func_0029AE70(void) {
    s8 temp_v0;
    s32 temp_v1;
    u32 *puVar3;
    s8 *pcVar4;
    s32 temp_v2;
    s32 temp_v3;
    s32 temp_v4;

    temp_v1 = func_00101958();
    temp_v4 = 0;
    temp_v2 = 4;
    temp_v3 = (*(s32 **)(temp_v1 + 0x9c))[1] * 3;
    pcVar4 = (s8 *)(**(s32 **)(temp_v1 + 0x9c) + 0x16);
    do {
        temp_v0 = *pcVar4;
        pcVar4 = pcVar4 + 1;
        temp_v2 = temp_v2 - 1;
        temp_v4 = temp_v4 + temp_v0;
    } while (-1 < temp_v2);
    *(u32 *)(temp_v1 + 0x3f0) = 0;
    temp_v2 = 4;
    puVar3 = (u32 *)(temp_v1 + 0x404);
    if (0x1ef - temp_v4 < temp_v3) {
        temp_v3 = 0x1ef - temp_v4;
    }
    *(s32 *)(temp_v1 + 0x3ec) = temp_v3;
    do {
        temp_v2 = temp_v2 - 1;
        *puVar3 = 0;
        puVar3 = puVar3 + -1;
    } while (-1 < temp_v2);
    if (*(s32 *)(temp_v1 + 0xb6e4) != 0) {
        func_002C0CF8(*(u32 *)(temp_v1 + 0xad34), 0);
    }
    return 1;
}

u32 func_0029AF40(void) {
    return 1;
}

void func_0029AF48(s32 arg0) {
    s32 temp_v0;
    u32 *puVar2;

    *(u32 *)(arg0 + 0x3f0) = 0;
    puVar2 = (u32 *)(arg0 + 0x404);
    temp_v0 = 4;
    do {
        temp_v0 = temp_v0 - 1;
        *puVar2 = 0;
        puVar2 = puVar2 + -1;
    } while (-1 < temp_v0);
}

void func_0029AF80(u32 arg0, u32 arg1) {
    func_00314298(arg0, (s32)arg1 + 0x3f4);
    func_00299A38(arg0, arg1);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029AFC0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029B008);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029B320);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029B378);

u32 func_0029B3C0(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    func_002C0CF8(*(u32 *)(temp_v0 + 0xad34), 0xffffffffffffffff);
    func_0026C5B8(0x17);
    func_0026C648(0);
    func_0026C618(0xa3);
    return 1;
}

u32 func_0029B410(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029B418);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029B600);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029B658);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029B6A0);

u32 func_0029B778(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029B780);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029B810);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029B868);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029B8B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029B950);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029BB28);

u32 func_0029BBC0(void) {
    func_0026C710();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029BBE0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029BC58);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428388);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428398);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004283B0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004283C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029BEB8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029BFB8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029C078);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029C0D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029C120);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029C3F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029C450);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029C4D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029C618);

void func_0029C800(void) {
}

void func_0029C808(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029C810);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029C848);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029C860);

void func_0029C878(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029C880);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029CA68);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029CB70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029CC90);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029CD60);

void func_0029CDD8(void) {
    func_0029C810();
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029CDF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029CE30);

u32 func_0029CE80(void) {
    return 1;
}

u32 func_0029CE88(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029CE90);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029CF00);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029CF88);

u32 func_0029D000(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029D008);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029D1C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029D278);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029D2D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029D3D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029D508);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029D550);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029D5B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029D790);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029D900);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029D970);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029DA58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029DA98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029DB58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029DF18);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428410);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428420);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004284E0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428550);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428560);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428570);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029DFB0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029E220);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029E478);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029E548);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029E820);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029EE80);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029F440);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029FA98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0029FBE0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A0148);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A0278);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A05C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A08D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A0EE8);

void func_002A1000(u32 arg0) {
    func_00341E20(arg0, 0x7f, 0x3f);
}

void func_002A1020(void) {
}

void func_002A1028(void) {
}

void func_002A1030(void) {
}

void func_002A1038(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1040);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1070);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A10A0);

u32 func_002A1118(void) {
    return 0x608;
}

u32 func_002A1120(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) + 1;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1150);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1178);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A11E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1248);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428590);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004285A0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004285B0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004285C0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004285D0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004285E0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004285F0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428600);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1278);

void func_002A1308(void) {
    func_00342690();
}

void func_002A1320(void) {
    func_00342688();
}

void func_002A1338(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1340);

u32 func_002A1368(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_00341BB8(temp_v0);
    return 1;
}

u32 func_002A1390(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_00341E20(temp_v0, 0x7f, 0x3f);
    return 1;
}

u32 func_002A13C0(void) {
    func_00341CF8();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A13E0);

u32 func_002A1430(void) {
    func_00342690();
    return 1;
}

u32 func_002A1450(void) {
    u64 temp_v0;

    temp_v0 = func_00342688();
    func_0010D818(temp_v0);
    return 1;
}

u32 func_002A1478(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1480);

u32 func_002A14D0(void) {
    func_002A2388();
    return 1;
}

u32 func_002A14F0(void) {
    func_002A2408();
    func_002A2550();
    return 1;
}

u32 func_002A1518(void) {
    func_002A2440();
    return 1;
}

u8 func_002A1538(void) {
    s64 temp_v0;

    temp_v0 = func_002A2330();
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1558);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A15B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1678);

void func_002A1760(u32 arg0) {
    sceSdRemoteInit();
    func_002A15B0(arg0);
    D_00437A2C = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1790);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1820);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A18B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1928);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1DD8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1E08);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1E58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1F50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1FA0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A1FF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A2070);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A20A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A2198);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A2200);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A22F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A2330);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A2388);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A2408);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A2440);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A24A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A2500);

void func_002A2550(void) {
    WaitSema(D_00438FE8);
    func_002A2500();
    SignalSema(D_00438FE8);
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428650);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A2580);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A2628);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A27A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A2928);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A2998);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A29D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A2A00);

void func_002A2A40(void) {
    WaitSema(D_00438FE8);
    func_002A29D8();
    SignalSema(D_00438FE8);
}

void func_002A2A70(void) {
    WaitSema(D_00438FE8);
    func_002A2A00();
    SignalSema(D_00438FE8);
}

void func_002A2AA0(void) {
    func_002A3D70();
    func_002A3E38(1);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A2AC0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A2BD0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428680);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A2C28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A30C0);

u32 func_002A3A50(void) {
    func_002A2AC0();
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A3A70);

u32 func_002A3AA0(void) {
    func_002A2AC0(0);
    return 0;
}

void func_002A3AC0(void) {
    func_0023A9A8();
    func_00117998();
    func_00117908();
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A3AE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A3B10);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A3B28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A3BE0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A3C58);

u32 func_002A3C78(void) {
    return **(u32 **)(*(s32 *)(D_00437A40 + 0x24) + 0x1c);
}

void func_002A3C90(s32 arg0) {
    func_002B8968(*(u32 *)(D_00437A40 + 0x24));
    if (0 < arg0) {
        do {
            arg0 = arg0 - 1;
            func_002B8CF0(*(u32 *)(D_00437A40 + 0x24));
        } while (arg0 != 0);
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A3CE0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A3D70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A3DE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A3E38);

u8 func_002A3EA8(void) {
    return *(s32 *)(D_00437A40 + 8) != 0;
}

void func_002A3EB8(void) {
    if (*(s32 *)(D_00437A40 + 8) != 0) {
        func_003054E8(*(s32 *)(D_00437A40 + 8));
        *(u32 *)(D_00437A40 + 8) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A3EF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A3F28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A3F60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A3F98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A3FE0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A40C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A4110);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A41C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A4208);

void func_002A4380(s32 arg0) {
    *(u8 *)(arg0 + 8) = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A4388);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A44C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A4670);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A46C8);

s32 func_002A4728(void) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = 0;
    temp_v0 = 0;
    do {
        temp_v1 = temp_v0 + 1;
        temp_v0 = func_002A46C8(temp_v0);
        temp_v2 = temp_v2 + temp_v0;
        temp_v0 = temp_v1;
    } while (temp_v1 < 3);
    return temp_v2;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A4770);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A47E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A4870);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A48F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A49C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A4A68);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A4B70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A4D28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A4DF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A4E48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A4F20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A5040);

void func_002A50E8(s32 arg0, s32 arg1, u8 arg2) {
    *(u8 *)(arg0 + arg1) = arg2;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A50F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A5128);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A5260);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A55B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A5890);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A58C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A58D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A58E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A5A20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A5A78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A5B08);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A5C40);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A5C58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A5E00);

void func_002A5EE8(u32 arg0, s32 arg1) {
    s32 temp_v0;

    func_002A7AF0();
    temp_v0 = D_00437A40;
    if (D_00437A40 != 0) {
        *(u32 *)(D_00437A40 + 0x10c) = 1;
        if (arg1 == 0) {
            *(u32 *)(temp_v0 + 0x110) = 0x80;
        }
        else {
            *(u32 *)(temp_v0 + 0x110) = 0;
        }
        *(u32 *)(D_00437A40 + 0x114) = 0;
    }
}

void func_002A5F40(void) {
    func_002A7FD0();
    if (D_00437A40 != 0) {
        *(u32 *)(D_00437A40 + 0x10c) = 0;
    }
}

u32 func_002A5F68(void) {
    u32 temp_v0;

    temp_v0 = 0;
    if (D_00437A40 != 0) {
        temp_v0 = *(u32 *)(D_00437A40 + 0x10c);
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A5F80);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004287E0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004287F8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428810);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428820);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428840);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428860);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428870);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428880);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428898);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004288A8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004288B8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004288C8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004288D8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004288E8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004288F8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428908);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428920);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428930);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428948);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428958);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428968);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428978);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428998);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004289A8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004289C0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004289D8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004289E8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004289F8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428A10);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428A28);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428A38);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428A48);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428A60);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428A70);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428A80);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428A90);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428AA0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428AB8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428AC8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428AD8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428AE8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428AF8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428B08);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428B20);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428B38);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428B50);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428B68);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428B78);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428B90);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428BA0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428BC0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428BD8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428BF0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428C08);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428C18);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428C28);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428C38);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428C48);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428C58);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428C68);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428C78);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428C88);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428C98);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428CB0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428CD0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428CE0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428CF8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428D10);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428D30);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428D40);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428D60);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428D70);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428D80);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428D90);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428DA0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428DB0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428DC8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428DD8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428DE8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428E00);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428E18);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428E30);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428E40);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428E58);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428E70);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428E88);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428EA0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428EB8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428EC8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428ED8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428EE8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428F00);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428F18);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428F28);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428F38);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428F50);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428F60);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428F70);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428F80);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428F90);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428FA0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428FB0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428FC8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428FD8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428FE8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00428FF8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429008);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429020);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429030);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429040);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429050);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429068);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429078);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429088);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429098);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004290A8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004290C0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004290D0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004290E0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004290F0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429100);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429118);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429130);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429140);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429150);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429160);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429170);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429180);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004291A0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004291B0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004291C8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004291D8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004291E8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004291F8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429208);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429218);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429228);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429238);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429250);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429260);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429278);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429288);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004292A0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004292B8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004292C8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004292E8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004292F8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429318);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429330);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429350);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429368);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429390);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004293A0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004293B0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004293C0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004293E0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429408);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429420);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429430);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429448);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429458);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429468);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429480);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429498);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004294A8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004294C0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004294D0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004294E8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004294F8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429508);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429518);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429530);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429540);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429560);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429570);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429588);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004295A0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004295B0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004295C8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004295E0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004295F0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429600);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429620);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429638);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429650);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429660);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429678);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429688);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004296A0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004296B0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004296C8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004296D8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004296F0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429708);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429720);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429730);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429740);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429750);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429760);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429778);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429788);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429798);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004297A8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004297C0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004297D8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004297E8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004297F8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429808);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429830);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429840);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429858);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429868);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429878);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429888);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429898);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004298B0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004298C0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004298D8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004298E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A5FC8);

void func_002A6000(void) {
    func_0019F048();
}

void func_002A6018(void) {
    func_0019F078();
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A6030);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A6180);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A6480);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A6580);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429938);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A6858);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A6C28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A6C70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A6D28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A6D68);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A6F88);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A7260);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A7350);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A73C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A7560);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A75A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A7730);

void func_002A78B0(void) {
    s64 temp_v0;

    D_00435BAC = 2;
    func_002A2408();
    func_002A2550();
    func_002A6018();
    do {
        temp_v0 = func_0032CD98();
    } while (temp_v0 != 0);
    func_003298C0(*D_00437AB0);
    D_00437AB0 = (u32 *)0x0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A7900);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A7938);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A7980);

u32 func_002A7A10(void) {
    func_002A7980();
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A7A30);

INCLUDE_ASM(const s32, "game/code_0025DA20", movieDraw);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A7A98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A7AF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A7B28);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429968);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429978);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429998);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004299B8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004299D8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_004299F8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429A18);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429A38);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429A58);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429A78);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429A98);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429AB8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429AD8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429AF8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429B18);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429B38);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429B58);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429B78);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429B98);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429BB8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429BD8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429BF8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429C18);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429C38);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429C58);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429C78);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429C98);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429CB8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429CD8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429CF8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429D18);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429D38);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429D58);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429D78);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429D98);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429DB8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429DD8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429DF8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429E18);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429E38);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429E58);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429E78);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429E98);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429EB8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429ED8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429EF8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429F18);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429F38);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429F58);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429F78);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429F98);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429FB8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429FD8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00429FF8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A018);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A038);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A058);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A078);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A098);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A0B8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A0D8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A0F8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A118);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A138);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A158);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A178);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A198);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A1B8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A1D8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A1F8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A218);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A238);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A258);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A278);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A298);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A2B8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A2D8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A2F8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A318);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A338);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A7D28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A7DB0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A7E60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A7F98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A7FD0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A8008);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A8028);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A8038);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A8048);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A8080);

u32 func_002A80C0(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_002A7AF0(temp_v0);
    D_00437AE8 = 0;
    return 1;
}

u32 func_002A80F0(void) {
    func_002A7FD0();
    D_00437AE8 = 0;
    func_001065B0(0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A8120);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A81C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A8208);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A8268);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A85C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A8610);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A87F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A88A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A8AE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A8B38);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A8B78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A8B90);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A8BC8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A418);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A8C80);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9068);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9130);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A91A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9200);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A440);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A450);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A460);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A470);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A480);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A490);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A4A8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A4C0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A4D8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A4F0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A500);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A518);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A530);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A540);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A558);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A570);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A588);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A598);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A5B0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A5C8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A5E0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A5F8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A608);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A620);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A638);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A650);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A668);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A680);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A690);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A6B0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A6C0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A6D0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A6E0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A6F0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A700);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A710);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A720);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A730);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A740);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A750);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A760);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A770);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A780);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A790);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A7A0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A7B0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A7D0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A7E0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A7F0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A800);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A810);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A820);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A830);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A840);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A850);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A870);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A888);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A8A0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A8B0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A8C0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A8D0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A8E0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A8F0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A900);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A910);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A920);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A930);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A940);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042A950);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9258);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A92D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9368);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A93F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9460);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A94D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9558);

void func_002A95B0(u32 arg0, u32 *arg1, u32 arg2, u32 arg3) {
    func_002BCD90(arg0, arg3, *arg1, 1, arg1[1], 0x2d, arg1[1], 0x1d);
    func_002BC498(arg0, arg1[1]);
    func_002BC5D0(arg0, arg1 + 4);
    func_002BC600(arg0, arg1 + 0xc);
    func_002BC630(arg0, arg1 + 0x14);
    func_002BCA98(arg0);
    func_002BE6E8(arg0, arg1[1]);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9640);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9788);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9820);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9908);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9A40);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9AB8);

void func_002A9BC8(s32 arg0, u32 arg1, u32 arg2, s32 arg3, u32 arg4,
                                    u32 arg5) {
    func_00306F80(arg0 + 0x60, arg1, arg2, 1, *(u32 *)(*(s32 *)(arg3 + 0x30) + 100), 10,
                                arg5);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9BF8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9E00);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9F08);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9F78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002A9FF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AA068);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AA0D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AA1C8);

u32 func_002AA278(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    func_002C1B70(temp_v0 + 0xaa50, 0x53);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AA2A8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AA08);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AA18);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AA360);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AA498);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AA4D8);

u8 func_002AA510(void) {
    s64 temp_v0;

    temp_v0 = func_00105B68();
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AA530);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AA740);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AA7A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AA9D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AAC70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AAC98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AACB8);

void func_002AAE80(u32 arg0) {
    func_002AACB8(0, arg0);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AAEA0);

u32 func_002AAF40(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    *(u32 *)(temp_v0 + 0xb1d0) = 1;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AAF70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AB0E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AB1B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AB1E8);

u32 func_002AB240(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    func_002BB498(*(u32 *)(temp_v0 + 0x118), *(u32 *)(temp_v0 + 0x60), 0, 1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AB278);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AB2E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AB368);

u32 func_002AB3A0(void) {
    return 1;
}

u32 func_002AB3A8(void) {
    func_002AAEA0();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AB3C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AB448);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AB518);

u32 func_002AB550(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AB558);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AB598);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AB650);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AB690);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AB890);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AB8C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AB8F0);

void func_002ABCD0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xaa48);
    func_002B9520(*(u32 *)(temp_v0 + 8));
    func_002B9520(*(u32 *)(temp_v0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002ABD08);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002ABD60);

void func_002ABEB0(s32 arg0) {
    func_002B9520(*(u32 *)(*(s32 *)(arg0 + 0xaa48) + 0x10));
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002ABED8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AC050);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AC408);

void func_002AC660(s32 arg0) {
    func_002B9520(*(u32 *)(*(s32 *)(arg0 + 0xaa48) + 0x14));
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AC688);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AC750);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AC8F0);

void func_002ACA98(s32 arg0) {
    func_002B9520(*(u32 *)(*(s32 *)(arg0 + 0xaa48) + 0x18));
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002ACAC0);

void func_002ACB18(u32 arg0) {
    func_002A9460(2, arg0);
}

void func_002ACB38(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002ACB40);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002ACBF8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002ACC50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002ACE58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002ACF00);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002ACF38);

void func_002AD030(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s64 temp_v1;

    temp_v0 = *(s32 *)(arg1 + 0xaa48);
    temp_v1 = func_002ACF38();
    if (temp_v1 != 0) {
        *(u32 *)(*(s32 *)(*(s32 *)(*(s32 *)(temp_v0 + 8) + 0x18) + 0x1c) + 0x60) =
                  (u32)*(u8 *)(arg0 + D_00435DD0 + 0x1340);
        *(s32 *)(temp_v0 + 0x38) = arg0;
    }
    func_002C1B68(arg1 + 0xaa50, 1);
}

u32 func_002AD0A8(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    func_002BAF50(*(u32 *)(*(s32 *)(temp_v0 + 0xaa48) + 8), temp_v0 + 0xb10c);
    return 1;
}

u32 func_002AD0E8(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    func_002BAF50(*(u32 *)(temp_v0 + 0x108), temp_v0 + 0xb10c);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AD118);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AD330);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AD3B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AD4C0);

u32 func_002AD508(void) {
    return 1;
}

u32 func_002AD510(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AD518);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AD618);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AD6C0);

u32 func_002AD6F8(void) {
    return 1;
}

u32 func_002AD700(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AD708);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AD808);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AD8B0);

u32 func_002AD8E8(void) {
    return 1;
}

u32 func_002AD8F0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AD8F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AD9F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002ADAA0);

u32 func_002ADAD8(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    func_002BAF50(*(u32 *)(*(s32 *)(temp_v0 + 0xaa48) + 0xc), temp_v0 + 0xb10c);
    return 1;
}

u32 func_002ADB18(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    func_002BAF50(*(u32 *)(temp_v0 + 0x108), temp_v0 + 0xb10c);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002ADB48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002ADC70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002ADD68);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002ADDA0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002ADF90);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AE078);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AE0B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AE1F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AE2D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AE408);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AE580);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AE7C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AE888);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AEA58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AEAA0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AEC10);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AECF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AEEA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AF020);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AF2E0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AA48);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AC40);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AC70);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042ACA0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042ACC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AF5E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AF898);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AF8E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AFA58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AFB38);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AFC58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AFDD0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002AFE18);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B0170);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B0228);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B0278);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B0578);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B05C8);

u32 func_002B0610(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = func_00101958();
    temp_v0 = *(s32 *)(temp_v1 + 0xaa48);
    func_002C1B68(temp_v1 + 0xaa50, 1);
    func_0026C918(0, D_00435E5C +
                                    *(s32 *)(*(s32 *)(*(s32 *)(*(s32 *)(temp_v0 + 0x18) + 0x18) + 0x1c) + 100) * 0x19);
    func_0026C5B8(8);
    func_0026C648(0);
    func_0026C618(0xf);
    return 1;
}

u32 func_002B06A0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B06A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B0898);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B09C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B0A18);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B0A60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B0AD8);

u32 func_002B0B88(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B0B90);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B0CB0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B0D00);

u32 func_002B0D48(void) {
    return 1;
}

void func_002B0D50(u32 arg0) {
    func_002A9460(4, arg0);
}

void func_002B0D70(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B0D78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B0D90);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B0FA0);

void func_002B1150(s32 arg0) {
    func_002B9520(*(u32 *)(*(s32 *)(arg0 + 0xaa48) + 8));
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B1178);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B12B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B15F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B1780);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B17C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B18A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B18E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B1B90);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B1BF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B1C68);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B1EA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B2338);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B2408);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B2698);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B2790);

u8 func_002B27C8(void) {
    s64 temp_v0;

    temp_v0 = func_002C6CE8();
    return temp_v0 != 1;
}

void func_002B27F0(u32 arg0) {
    func_002A9460(3, arg0);
}

void func_002B2810(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B2818);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B2860);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B28A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B2970);

void func_002B29C8(u32 arg0) {
    func_002B28A8(arg0, 1);
}

void func_002B29E0(void) {
    func_002B2970();
}

void func_002B29F8(u32 arg0) {
    func_002B28A8(arg0, 0);
}

void func_002B2A10(void) {
    func_002B2970();
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B2A28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B2B48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B2C50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B2C88);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B2E38);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B2FF0);

void func_002B3120(s32 arg0) {
    *(u32 *)
      (*(s32 *)(**(s32 **)(*(s32 *)(arg0 + 0xa914) + 0x1c) * 0x2138 + arg0 + 0x3d8) + 0x60) =
              0x100;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B3150);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B3260);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B3400);

void func_002B34E0(s32 arg0, u32 *arg1) {
    s32 temp_v0;

    temp_v0 = 0x100 - *(s32 *)(*(s32 *)(**(s32 **)(*(s32 *)(arg0 + 0xa690) + 0x1c) * 0x2138 + arg0
                                                                      + 0x154) + 0x60);
    func_00306CD0(0xa0, 0xa30, 0, temp_v0, 1, arg1[1], 0x55, 0x53);
    func_00306CD0(0x30, 0xaf8, 0, temp_v0, 1, *arg1, 0x1a, 0x53);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B3580);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B3648);

void func_002B3720(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5) {
    func_002C16F0(0, 0, 0, arg0, *(u8 *)((s32)arg0 + 0x55), arg2, arg5);
    func_002C3E08(0xe80, 0x5b8, 0, arg3, arg5);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B3788);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B3940);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B39F0);

u32 func_002B3A58(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B3A60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B3CA0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B3E80);

u32 func_002B40B8(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    temp_v0 = *(s32 *)(temp_v0 + 0xaa48);
    func_002B9520(*(u32 *)(temp_v0 + 0x24));
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B40F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B4180);

void func_002B4270(u32 arg0) {
    func_002A9460(1, arg0);
}

void func_002B4290(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B4298);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B45D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B4730);

void func_002B47F8(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B4810);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B4848);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B4C48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B4CC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B4DE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B4E58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B5028);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B5128);

void func_002B5160(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    *(u32 *)(*(s32 *)(temp_v0 + 0xaa48) + 0x34) = 0xffffffff;
}

u32 func_002B5190(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    return ~*(u32 *)(*(s32 *)(temp_v0 + 0xaa48) + 0x34) >> 0x1f;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B51C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B5240);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B5358);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B53B8);

void func_002B5430(s32 arg0, s32 arg1) {
    *(u16 *)(arg1 * 2 + arg0 + 0x22) = 0;
    func_003144E8();
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B5450);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B5580);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B5778);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B57A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B5980);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B5A30);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B5BE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B5DB0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B5DE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B5F00);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B5FA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B60E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B61C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B61F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B62A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B6308);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B63F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B6498);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B66D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B6800);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B6838);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B6898);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B6B00);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B6C70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B6D08);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B6D78);

u32 func_002B6FA8(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    temp_v0 = *(s32 *)(temp_v0 + 0xaa48);
    func_002B9520(*(u32 *)(temp_v0 + 0x24));
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B6FE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B7060);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B70C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B7228);

void func_002B7588(s32 arg0) {
    s32 temp_v0;

    for (temp_v0 = **(s32 **)(arg0 + 0x28c); temp_v0 < 3; temp_v0 = temp_v0 + 1) {
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B75C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B76B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B76E8);

void func_002B7730(u32 arg0, u32 *arg1) {
    *arg1 = *arg1 | arg0;
}

void func_002B7740(s32 arg0, s32 arg1) {
    u32 *puVar1;
    s32 temp_v0;
    u32 *puVar3;
    s32 temp_v1;
    s32 temp_v2;
    u32 temp_v3;

    temp_v3 = 0;
    temp_v2 = 0;
    do {
        puVar3 = (u32 *)(arg1 + 0x40);
        temp_v0 = temp_v2 << 2;
        temp_v1 = 3;
        do {
            puVar1 = (u32 *)(temp_v0 + arg0);
            temp_v0 = temp_v0 + 4;
            temp_v1 = temp_v1 - 1;
            *puVar3 = *puVar1;
            puVar3 = puVar3 + 1;
        } while (-1 < temp_v1);
        temp_v3 = temp_v3 + 1;
        arg1 = arg1 + 0x10;
        temp_v2 = temp_v2 + 4;
    } while (temp_v3 < 2);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B7790);

void func_002B77A0(s32 arg0) {
    func_003059E0(*(u32 *)(arg0 + 8), *(u32 *)(arg0 + 0x1c),
                                *(u32 *)(arg0 + 0x3c), 0, 4);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B77D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B7850);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B78C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B7908);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B7958);

void func_002B7A80(s32 arg0, s32 arg1) {
    *(u32 *)(arg1 * 4 + arg0 + 0x60) = 0;
    *(s32 *)(arg0 + 0x160) = *(s32 *)(arg0 + 0x160) - 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B7AA0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B7C10);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B7E60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B7F80);

void func_002B8140(u32 *arg0) {
    *arg0 = *arg0 & 0xfffffffb;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B8158);

u32 func_002B81C8(u32 arg0) {
    s64 temp_v0;

    do {
        temp_v0 = func_002B86E8(arg0);
    } while (temp_v0 != 0);
    func_00328E48(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B8208);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B82A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B8350);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B83A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B86E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B8860);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B88A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B88F0);

void func_002B8968(u32 arg0) {
    func_002B88F0(0, arg0);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B8988);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B89A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B89E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B8A50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B8BA8);

void func_002B8CF0(u32 arg0) {
    func_002B8A50(arg0, 0, 0);
}

void func_002B8D10(u32 arg0) {
    func_002B8BA8(arg0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B8D30);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B8E30);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B8F98);

u32 func_002B8FB8(u32 *arg0) {
    return *arg0 & 2;
}

s32 func_002B8FC8(s32 arg0) {
    return *(s32 *)(arg0 + 0x28) * *(s32 *)(arg0 + 0xc);
}

void func_002B8FD8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x10);
    if (temp_v0 != 0) {
        *(u32 *)(temp_v0 + 0x50) = 0;
        while (temp_v0 = *(s32 *)(temp_v0 + 0x58), temp_v0 != 0) {
            *(u32 *)(temp_v0 + 0x50) = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9010);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9058);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9138);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9188);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9218);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9460);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9490);

void func_002B9520(u32 arg0) {
    s32 temp_v0;

    func_002B81C8(*(u32 *)((s32)arg0 + 0x18));
    temp_v0 = *(s32 *)((s32)arg0 + 0x90);
    if (temp_v0 != 0) {
        func_002B99D8(temp_v0);
    }
    func_00328E48(arg0);
}

void func_002B9560(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x3c) = arg1;
}

void func_002B9568(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x94) = arg1;
}

void func_002B9570(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6, u32 arg7,
                                    u32 arg8) {
    *(u32 *)(arg0 + 0x2c) = arg1;
    *(u32 *)(arg0 + 0x4c) = arg8;
    *(u32 *)(arg0 + 0x30) = arg2;
    *(u32 *)(arg0 + 0x34) = arg3;
    *(u32 *)(arg0 + 0x38) = arg5;
    *(u32 *)(arg0 + 0x48) = arg4;
    *(u32 *)(arg0 + 0x3c) = arg6;
    *(u32 *)(arg0 + 0x40) = arg7;
    *(u32 *)(arg0 + 0x44) = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B95A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B95D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B95E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9678);

void func_002B96D8(s32 arg0) {
    *(u32 *)(arg0 + 4) = *(u32 *)(arg0 + 4) & 0xfffffffb;
}

void func_002B96F0(s32 arg0) {
    func_002B82A0(*(u32 *)(arg0 + 0x18));
}

void func_002B9708(s32 arg0) {
    func_002B83A0(*(u32 *)(arg0 + 0x18));
}

void func_002B9720(s32 arg0) {
    func_002B86E8(*(u32 *)(arg0 + 0x18));
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9738);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9788);

void func_002B97D8(u32 arg0) {
    func_002B9738(arg0, 0);
}

void func_002B97F0(u32 arg0) {
    func_002B9788(arg0, 0);
}

void func_002B9808(s32 arg0) {
    func_002B8F98(*(u32 *)(arg0 + 0x18));
}

void func_002B9820(s32 arg0) {
    func_002B8FB8(*(u32 *)(arg0 + 0x18));
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9838);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9918);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B99D8);

void func_002B9A38(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9A40);

void func_002B9BB0(s32 arg0, s32 arg1, u32 arg2, s32 arg3, u32 arg4) {
    func_002B9A40(arg0 - 0xf0, arg1 - 8, arg2, *(u32 *)(arg3 + 0x94),
                                *(u32 *)(arg3 + 0x18), *(u32 *)(arg3 + 0x90), arg4);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9BE0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9CD8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9CF8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9DD8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9EA0);

void func_002B9FB8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x18) + 0x20);
    if (0 < temp_v0) {
        do {
            temp_v0 = temp_v0 - 1;
        } while (temp_v0 != 0);
    }
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AD38);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AD78);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AD88);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AD98);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042ADA8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042ADB8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042ADC8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042ADD8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042ADE8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042ADF8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AE08);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AE18);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AE28);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AE48);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AE58);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AE68);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AE80);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AE90);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AEA0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AED0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AEE8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AF00);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002B9FF8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BA268);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BA308);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BA378);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AF48);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BA518);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BA660);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BA738);

void func_002BA7A8(void) {
    func_002BA738();
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BA7C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BA7E8);

void func_002BA890(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = *(s32 *)(arg0 + 0x1c);
    temp_v1 = *(s32 *)(arg0 + 0x1c);
    while (temp_v0 = temp_v2, temp_v0 != 0) {
        temp_v1 = temp_v0;
        temp_v2 = *(s32 *)(temp_v0 + 0x5c);
    }
    *(s32 *)(arg0 + 0x10) = temp_v1;
}

void func_002BA8C8(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = *(s32 *)(arg0 + 0x1c);
    temp_v1 = *(s32 *)(arg0 + 0x1c);
    while (temp_v0 = temp_v2, temp_v0 != 0) {
        temp_v1 = temp_v0;
        temp_v2 = *(s32 *)(temp_v0 + 0x58);
    }
    *(s32 *)(arg0 + 0x14) = temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BA900);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BA978);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BAA28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BAA50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BAA80);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BAAA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BAAD8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BAB00);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BAB30);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BAC58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BACA0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BACF0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BAD20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BAE08);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BAE98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BAF10);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BAF50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BB0D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BB0E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BB290);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BB320);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BB370);

void func_002BB418(u32 arg0) {
    func_002BB320();
    func_00328E48(arg0);
}

void func_002BB440(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x34);
    *(s32 *)(arg0 + 0x2c) = temp_v0;
    *(u32 *)(arg0 + 0x30) = *(u32 *)(arg0 + 0x38);
    *(u32 *)(arg0 + 0x34) = 0;
    if (temp_v0 != 0) {
        func_00305B00(temp_v0, *(u32 *)(arg0 + 0x38), *(u32 *)(arg0 + 0x44), 0, 10, 2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BB498);

u8 func_002BB500(s32 arg0) {
    return *(s32 *)(arg0 + 0x2c) != 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BB510);

void func_002BB850(s32 arg0, u32 arg1, u32 arg2, u32 arg3, s32 arg4
                                    ) {
    u32 temp_v0;

    temp_v0 = func_00305348(arg1, arg2, 1);
    *(u32 *)(arg0 + 0xe4) = temp_v0;
    temp_v0 = func_00305348(arg1, arg3, 1);
    *(u32 *)(arg0 + 0xe8) = temp_v0;
    if (-1 < arg4) {
        temp_v0 = func_00305348(arg1, arg4, 1);
        *(u32 *)(arg0 + 0xec) = temp_v0;
    }
}

void func_002BB8D8(s32 arg0) {
    u32 temp_v0;
    u32 *puVar2;
    u32 *puVar3;
    u32 *puVar4;
    u32 temp_v1;

    puVar2 = (u32 *)(arg0 + 0x7c);
    puVar3 = (u32 *)(arg0 + 0x164);
    puVar4 = (u32 *)(arg0 + 0x160);
    temp_v1 = 0;
    do {
        if (puVar2[0x38] != 0) {
            func_003054E8(puVar2[0x38]);
        }
        if (puVar2[0x39] != 0) {
            func_003054E8(puVar2[0x39]);
        }
        if (puVar2[0x3a] != 0) {
            func_003054E8(puVar2[0x3a]);
        }
        temp_v0 = *puVar2;
        temp_v1 = temp_v1 + 1;
        puVar2[0x38] = 0;
        *puVar4 = 0;
        *puVar2 = temp_v0 & 0xffffffbf;
        puVar2 = puVar2 + 0x84e;
        *puVar3 = 0;
        puVar3 = puVar3 + 0x84e;
        puVar4 = puVar4 + 0x84e;
    } while (temp_v1 < 5);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BB998);

void func_002BB9C8(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BB9D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BBA38);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BBE78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BBF38);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BBFC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BC078);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BC0A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BC120);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BC258);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BC2B8);

void func_002BC3C8(s32 arg0, s32 arg1, u32 arg2) {
    u32 temp_v0;

    temp_v0 = func_002BC120(arg2);
    *(u32 *)(arg0 * 0x2138 + arg1 + 0x158) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BC410);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BC460);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BC498);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BC580);

void func_002BC5D0(s32 arg0, u32 *arg1) {
    u32 temp_v0;
    u32 *puVar2;
    u32 temp_v1;

    puVar2 = (u32 *)(arg0 + 0x24);
    temp_v1 = 0;
    do {
        temp_v0 = *arg1;
        arg1 = arg1 + 1;
        temp_v1 = temp_v1 + 1;
        *puVar2 = temp_v0;
        puVar2 = puVar2 + 1;
    } while (temp_v1 < 8);
}

void func_002BC600(s32 arg0, u32 *arg1) {
    u32 temp_v0;
    u32 *puVar2;
    u32 temp_v1;

    puVar2 = (u32 *)(arg0 + 0x44);
    temp_v1 = 0;
    do {
        temp_v0 = *arg1;
        arg1 = arg1 + 1;
        temp_v1 = temp_v1 + 1;
        *puVar2 = temp_v0;
        puVar2 = puVar2 + 1;
    } while (temp_v1 < 8);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BC630);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BC690);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BC9A8);

void func_002BCA98(u32 arg0) {
    func_002BC9A8(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BCAB0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BCBD8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BCCB0);

void func_002BCCF0(u32 arg0, u32 arg1) {
    func_002BCCB0();
    func_002BCBD8(arg0, arg1);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BCD28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BCD90);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BCE50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BCF60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BCFC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BD090);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BD1D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BD2E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BD358);

void func_002BD3A8(s32 arg0) {
    u32 *puVar1;
    u32 temp_v0;

    puVar1 = (u32 *)(arg0 + 0x7c);
    temp_v0 = 0;
    do {
        temp_v0 = temp_v0 + 1;
        *puVar1 = *puVar1 & 0xfffffffe;
        puVar1 = puVar1 + 0x84e;
    } while (temp_v0 < 5);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BD3E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BD480);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BD5C8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AF90);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AFA0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AFB8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042AFD8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BD710);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BD9E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BDA50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BDA78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BDAA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BDC38);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BE080);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BE138);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BE240);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BE438);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BE590);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BE628);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BE6E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BE730);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BED10);

void func_002BEE38(u32 *arg0) {
    arg0[3] = 0x18;
    arg0[4] = 5;
    *arg0 = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BEE50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BEF90);

void func_002BEFB0(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 1;
    temp_v0 = arg1 * 0x2138 + arg0 + 0x168;
    do {
        temp_v1 = temp_v1 - 1;
        func_002BEF90(temp_v0);
        temp_v0 = temp_v0 + 0x1024;
    } while (-1 < temp_v1);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BF000);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BF238);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BF478);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BF660);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B028);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B048);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B098);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B0D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BF830);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002BFEA0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C0330);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C03B0);

void func_002C04C0(s32 arg0) {
    if (*(s32 *)(arg0 + 4) < 0x100) {
        *(s32 *)(arg0 + 4) = *(s32 *)(arg0 + 4) + 8;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C04E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C0630);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C0718);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C0740);

void func_002C07A0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x88);
    if (temp_v0 != 0) {
        func_002BA308(temp_v0);
    }
    func_00328E48(arg0);
}

void func_002C07D8(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    *(u32 *)(arg0 + 0x18) = arg1;
    *(u32 *)(arg0 + 0x1c) = arg2;
    func_00307388(arg0 + 0x20, arg3, arg4);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C0800);

void func_002C08B0(s32 arg0) {
    u32 temp_v0;

    temp_v0 = func_002B9FF8(5);
    *(u32 *)(arg0 + 0x88) = temp_v0;
}

void func_002C08E0(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    *(u32 *)(arg0 + 0x68) = arg1;
    *(u32 *)(arg0 + 0x6c) = arg2;
    func_00307388(arg0 + 0x60, arg3, arg4);
}

void func_002C0908(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    *(u32 *)(arg0 + 0x78) = arg1;
    *(u32 *)(arg0 + 0x7c) = arg2;
    func_00307388(arg0 + 0x70, arg3, 0);
    *(u32 *)(arg0 + 0x80) = arg4;
}

void func_002C0950(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C0958);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C0B80);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C0C40);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C0C98);

void func_002C0CF8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002C0D00(s32 arg0) {
    *(u32 *)(arg0 + 0x24) = 0xffffffff;
}

u32 func_002C0D10(s32 arg0) {
    return *(u32 *)(arg0 + 0x24);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C0D18);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C0F00);

void func_002C0F48(s32 arg0, s32 arg1, u32 arg2) {
    func_002C2AA0(*(u32 *)(arg1 * 4 + arg0 + 0x10), arg2);
}

void func_002C0F70(s32 arg0, u64 arg1) {
    u32 temp_v0;
    u64 temp_v1;
    s32 temp_v2;
    u32 *puVar5;
    s32 temp_v3;

    puVar5 = (u32 *)(arg0 + 0x10);
    temp_v3 = 0;
    do {
        temp_v2 = temp_v3 + 1;
        temp_v1 = func_0011D360(arg1, temp_v3);
        temp_v0 = *puVar5;
        puVar5 = puVar5 + 1;
        func_002C2AA0(temp_v0, temp_v1);
        temp_v3 = temp_v2;
    } while (temp_v2 < 5);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C0FD8);

void func_002C1050(void) {
    func_00328E48();
}

void func_002C1068(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u16 arg4, u32 arg5, u32 arg6) {
    s32 temp_v0;

    temp_v0 = func_002C4BA8(arg4);
    func_00306CD0(arg0, arg1, arg2, arg3, 1, arg5, temp_v0 + 8, arg6);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C10F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C1660);

void func_002C16D8(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C16F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C1B58);

void func_002C1B68(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C1B70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C1C20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C1CD0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C1D10);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C1DC8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C1E48);

void func_002C1F68(s32 arg0) {
    u32 temp_v0;

    temp_v0 = func_00304998(3);
    *(u32 *)(arg0 + 0x38) = temp_v0;
    temp_v0 = func_00304998(3);
    *(u32 *)(arg0 + 0x3c) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C1FA0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C1FF0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B118);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B130);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B140);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B150);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B160);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B180);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B220);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B270);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C2128);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C21F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C2258);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C22D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C24E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C2680);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C26D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C2920);

void func_002C2A88(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C2A90);

void func_002C2AA0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002C2AA8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x28) = arg1;
}

void func_002C2AB0(s32 arg0, s32 arg1) {
    if (*(s32 *)(arg0 + 0x20) != arg1) {
        *(u32 *)(arg0 + 0xa4) = 0x100;
    }
    *(s32 *)(arg0 + 0x20) = arg1;
}

void func_002C2AC8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x1c) = arg1;
}

void func_002C2AD0(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C2AE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C3010);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C32A0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C32B0);

void func_002C3390(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C33A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C33C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C3E08);

void func_002C3E58(u32 arg0) {
    memset(arg0, 0, 0x4c);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C3E78);

void func_002C3FC8(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)arg0;
    while (temp_v0 != 0) {
        func_002C3E78(1, 0, arg0, arg1);
        temp_v0 = *(s32 *)arg0;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C4020);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C4038);

u8 func_002C42A0(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x44) == arg1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C42B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C42C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C42D8);

void func_002C42F0(s32 arg0, u32 arg1) {
    if (*(s32 *)(arg0 + 0x44) != 0) {
        func_002C42C0(arg1, *(s32 *)(arg0 + 0x44));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C4328);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C4430);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C44E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C47C8);

void func_002C48C8(u32 arg0, u32 arg1) {
    func_002C47C8(*(u32 *)((s32)arg0 + 0x90), arg0, arg1);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C48F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C49F0);

void func_002C4B40(u32 arg0) {
    func_002C49F0(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C4B58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C4BA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C4C28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C4C60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C4CA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C4D78);

u8 func_002C4DB0(u32 arg0) {
    return *(u8 *)((arg0 & 0xffff) * 0x38 + D_00435E20 + 3);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C4DD0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C4E58);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C4EB8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C4F50);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C4FB8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5030);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C50C0);

void func_002C5128(u16 arg0) {
    func_00119548(arg0);
}

u32 func_002C5140(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5148);

u8 func_002C5338(u32 arg0) {
    return *(s8 *)((arg0 & 0xffff) * 2 + D_00435E1C) == '\x01';
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5358);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5428);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5480);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5498);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C54B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C54C8);

u32 func_002C5570(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 0x52) = arg1;
    return 1;
}

u16 func_002C5580(s32 arg0) {
    return *(u16 *)(arg0 + 0x52);
}

u32 func_002C5588(u32 arg0, u32 arg1) {
    *(s16 *)((s32)arg0 + 0x1b2) = (s16)arg1;
    func_002C5678(arg1);
    func_003144E8(arg0);
    return 1;
}

u16 func_002C55C0(s32 arg0) {
    return *(u16 *)(arg0 + 0x1b2);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C55C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5618);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5678);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C56A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C56D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5700);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5758);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B2E8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B300);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B350);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B370);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B3D0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B440);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B4C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C57D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C59B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5A28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5C28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5C78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5CD0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5D20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5DE0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5EA8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C5F78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6008);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6348);

u8 func_002C6480(void) {
    return D_00437C9C != 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6490);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C64B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C64C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C64D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6578);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6610);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6670);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C66D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C66F8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6720);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6758);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6790);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6958);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6988);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6A20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6A88);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6AC0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6AE8);

u32 func_002C6B28(u32 *arg0) {
    return *arg0 & 1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6B38);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6BB8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6BD8);

void func_002C6CC8(u16 arg0, u32 arg1) {
    func_002C6BD8(arg0, 0xffffffffffffffff, arg1);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6CE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6E20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6EE0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6F60);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C6F98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C70D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C7168);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C72E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C73C0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C7428);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C7508);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C7530);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C7540);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C7568);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C76A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C76F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C7730);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C7740);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B500);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B518);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B528);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B538);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B548);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B558);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C7750);

u32 func_002C79B8(void) {
    func_002C7750();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C79D8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B5A8);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B5B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C7A60);

void func_002C7C00(void) {
    func_0023A9A8();
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C7C18);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C7C40);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002C7C88);

void func_002C7CE8(void) {
    func_002C7C88();
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_0042B610);
