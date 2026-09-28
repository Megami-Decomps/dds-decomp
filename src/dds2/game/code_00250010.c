#include "common.h"

extern u32 D_004373CC;

extern u16 D_004373C8;

extern void func_00135588(s32 arg0);

extern s16 func_00135598(void);

extern u16 D_00438FB0;

extern u16 D_00438FB2;

extern s16 D_00438FB4;

extern s16 D_00438FB6;

extern void func_00134A18(void);

extern void func_0012BC38(s32 arg0);

extern void func_0012D3E0(void);

extern void func_0032CEE8(s32 arg0, s32 arg1);

extern s32 func_0033D810();

typedef struct {
    s16 unk0;
    s8 unk2;
    u8 pad3[7];
} EvtTblEntry; /* 0xA bytes */

extern EvtTblEntry D_003C9730[];

extern s8 D_003C9732[];

extern void *func_00101958();

extern s32 func_001979E0(void);

INCLUDE_ASM(const s32, "game/code_00250010", func_00250010);

INCLUDE_ASM(const s32, "game/code_00250010", func_00250080);

void func_002500E0(u32 arg0) {
    D_004373CC = arg0;
}

void func_002500E8(s32 arg0, s32 arg1) {
    s16 temp_v0;

    temp_v0 = func_00135598();
    if (temp_v0 != arg1) {
        if (arg0 == 0) {
            func_00135588(arg1);
            D_004373C8 = 0;
        } else {
            D_00438FB2 = arg0;
            D_00438FB4 = temp_v0;
            D_00438FB6 = arg1;
            D_004373C8 = 1;
            D_00438FB0 = 0;
        }
    }
}

u16 func_00250150(void) {
    return D_004373C8;
}

void func_00250158(void) {
    if (D_004373C8 != 0) {
        D_00438FB0 += 1;
        func_00135588(D_00438FB4 + (s32)((f32)(D_00438FB6 - D_00438FB4) * ((f32)D_00438FB0 / (f32)D_00438FB2)));
        if (D_00438FB0 >= D_00438FB2) {
            D_004373C8 = 0;
        }
    }
}

s32 func_002501E0(void) {
    func_00250158();
    func_00134A18();
    if (D_004373CC != 0) {
        func_0012BC38(0x53);
        func_0012D3E0();
    }
    return 0;
}

void func_00250228(void) {
    D_004373C8 = 0;
    D_004373CC = 0;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00250238);

INCLUDE_ASM(const s32, "game/code_00250010", func_00250278);

INCLUDE_ASM(const s32, "game/code_00250010", func_002502E0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00250300);

INCLUDE_ASM(const s32, "game/code_00250010", func_00250338);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423390);

s32 func_00250508(s32 arg0, s32 arg1, s32 arg2) {
    func_0032CEE8(arg0, func_0033D810(arg1, arg2, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00250558);

INCLUDE_ASM(const s32, "game/code_00250010", func_00250718);

s32 func_00250880(s32 arg0, s32 arg1, s32 arg2) {
    func_0032CEE8(arg0, func_0033D810(arg1, arg2, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

INCLUDE_RODATA(const s32, "game/code_00250010", D_004233F0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423400);

INCLUDE_RODATA(const s32, "game/code_00250010", jtbl_00423410);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423428);

INCLUDE_ASM(const s32, "game/code_00250010", func_002508D0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00250A08);

INCLUDE_ASM(const s32, "game/code_00250010", func_00250B50);

INCLUDE_ASM(const s32, "game/code_00250010", func_00250BA0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00250D68);

INCLUDE_ASM(const s32, "game/code_00250010", func_00250F20);

INCLUDE_ASM(const s32, "game/code_00250010", func_00251180);

INCLUDE_ASM(const s32, "game/code_00250010", func_00251250);

INCLUDE_ASM(const s32, "game/code_00250010", func_002512B0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00251340);

INCLUDE_ASM(const s32, "game/code_00250010", func_00251440);

INCLUDE_ASM(const s32, "game/code_00250010", func_002514C8);

INCLUDE_ASM(const s32, "game/code_00250010", func_002515C8);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423668);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423678);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423688);

INCLUDE_ASM(const s32, "game/code_00250010", func_002517E0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00251868);

INCLUDE_ASM(const s32, "game/code_00250010", func_00251B20);

INCLUDE_ASM(const s32, "game/code_00250010", func_00251DE8);

INCLUDE_ASM(const s32, "game/code_00250010", func_00251ED0);

INCLUDE_ASM(const s32, "game/code_00250010", func_002520E8);

INCLUDE_ASM(const s32, "game/code_00250010", func_002521C8);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423AE0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423AF0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B00);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B10);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B20);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B30);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B40);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B50);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B60);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B70);

INCLUDE_ASM(const s32, "game/code_00250010", func_00252378);

INCLUDE_ASM(const s32, "game/code_00250010", func_002534B0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00253590);

INCLUDE_ASM(const s32, "game/code_00250010", func_002537A8);

INCLUDE_ASM(const s32, "game/code_00250010", func_00253938);

INCLUDE_ASM(const s32, "game/code_00250010", func_00253A98);

INCLUDE_ASM(const s32, "game/code_00250010", func_00253C08);

INCLUDE_ASM(const s32, "game/code_00250010", func_00253D08);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423EE0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423EF0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F00);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F10);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F20);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F30);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F40);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F50);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F60);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F70);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F80);

INCLUDE_ASM(const s32, "game/code_00250010", func_00253D80);

INCLUDE_ASM(const s32, "game/code_00250010", func_00253FF8);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423FE0);

INCLUDE_RODATA(const s32, "game/code_00250010", jtbl_00423FF0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00254200);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424020);

INCLUDE_ASM(const s32, "game/code_00250010", func_00254250);

INCLUDE_ASM(const s32, "game/code_00250010", func_00254458);

s32 func_00254620(s32 *arg0) {
    return D_003C9730[*arg0].unk0 != 0;
}

s32 func_00254648(s32 arg0) {
    return D_003C9732[*(s32 *)(arg0 + 0x23C8) + *(s32 *)(*(s32 *)(arg0 + 0x2308)) * 10];
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00254678);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424080);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424090);

INCLUDE_ASM(const s32, "game/code_00250010", func_00254940);

INCLUDE_ASM(const s32, "game/code_00250010", func_00254C90);

INCLUDE_ASM(const s32, "game/code_00250010", func_00254CE0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00254F80);

INCLUDE_ASM(const s32, "game/code_00250010", func_00255360);

INCLUDE_ASM(const s32, "game/code_00250010", func_00255538);

void func_00255648(void) {
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00255650);

INCLUDE_ASM(const s32, "game/code_00250010", func_00255818);

void func_002560A8(void) {
}

INCLUDE_ASM(const s32, "game/code_00250010", func_002560B0);

INCLUDE_ASM(const s32, "game/code_00250010", func_002566F8);

INCLUDE_ASM(const s32, "game/code_00250010", func_002567A8);

void func_00256888(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    *(s32 *)(arg0 + 0x23E4) = arg1;
    *(s32 *)(arg0 + 0x23E8) = arg2;
    *(s32 *)(arg0 + 0x23EC) = arg3;
}

INCLUDE_RODATA(const s32, "game/code_00250010", D_004241A0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00256898);

INCLUDE_ASM(const s32, "game/code_00250010", func_002569D0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_004241C0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00256AE0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00256CF0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00257028);

s32 func_002570B8(void) {
    void *temp_v0;

    temp_v0 = func_00101958();
    if (func_001979E0() == 0) {
        *(s32 *)((u8 *)temp_v0 + 0x228C) = 0;
        return -1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424210);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424220);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424230);

INCLUDE_ASM(const s32, "game/code_00250010", func_002570F8);

INCLUDE_ASM(const s32, "game/code_00250010", func_00257910);

INCLUDE_ASM(const s32, "game/code_00250010", func_002582D0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00258700);

INCLUDE_ASM(const s32, "game/code_00250010", func_00258850);

void func_002588A0(void) {
    func_0036A420();
}

INCLUDE_ASM(const s32, "game/code_00250010", func_002588B8);

INCLUDE_ASM(const s32, "game/code_00250010", func_00258920);

INCLUDE_ASM(const s32, "game/code_00250010", func_00258988);

INCLUDE_ASM(const s32, "game/code_00250010", func_002589F0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00258A58);

INCLUDE_ASM(const s32, "game/code_00250010", func_00258AC0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00258B28);

INCLUDE_ASM(const s32, "game/code_00250010", func_00258B90);

INCLUDE_ASM(const s32, "game/code_00250010", func_00258BF8);

INCLUDE_ASM(const s32, "game/code_00250010", func_00258C60);

INCLUDE_ASM(const s32, "game/code_00250010", func_00258CC8);

INCLUDE_ASM(const s32, "game/code_00250010", func_00259250);

INCLUDE_ASM(const s32, "game/code_00250010", func_00259298);

INCLUDE_ASM(const s32, "game/code_00250010", func_00259428);

INCLUDE_ASM(const s32, "game/code_00250010", func_002594A0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00259518);

INCLUDE_ASM(const s32, "game/code_00250010", func_002595A0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00259628);

INCLUDE_ASM(const s32, "game/code_00250010", func_002596B0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00259738);

INCLUDE_ASM(const s32, "game/code_00250010", func_002597C0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00259848);

INCLUDE_ASM(const s32, "game/code_00250010", func_002598D0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00259958);

INCLUDE_ASM(const s32, "game/code_00250010", func_002599E0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00259A60);

INCLUDE_ASM(const s32, "game/code_00250010", func_00259AE8);

u16 func_00259FF8(s32 arg0, s32 arg1) {
    if (*(s32 *)(*(s32 *)(arg0 + 0x74) + 0x14) == 4) {
        return *(u16 *)(arg1 * 0x10 + *(s32 *)(arg0 + 0x88));
    }
    return *(u16 *)(arg1 * 0x2c + *(s32 *)(arg0 + 0x8c));
}

s16 func_0025A048(s32 arg0, s32 arg1) {
    if (*(s32 *)(*(s32 *)(arg0 + 0x74) + 0x14) == 4) {
        return *(s16 *)(arg1 * 0x10 + *(s32 *)(arg0 + 0x88) + 6);
    }
    return *(s16 *)(arg1 * 0x2c + *(s32 *)(arg0 + 0x8c) + 6);
}

u16 func_0025A098(s32 arg0, s32 arg1) {
    if (*(s32 *)(*(s32 *)(arg0 + 0x74) + 0x14) == 4) {
        return *(u16 *)(arg1 * 0x10 + *(s32 *)(arg0 + 0x88) + 2);
    }
    return *(u16 *)(arg1 * 0x2c + *(s32 *)(arg0 + 0x8c) + 2);
}

u16 func_0025A0E8(s32 arg0, s32 arg1) {
    if (*(s32 *)(*(s32 *)(arg0 + 0x74) + 0x14) == 4) {
        return *(u16 *)(arg1 * 0x10 + *(s32 *)(arg0 + 0x88) + 4);
    }
    return *(u16 *)(arg1 * 0x2c + *(s32 *)(arg0 + 0x8c) + 4);
}

s32 func_0025A138(s32 arg0, s32 arg1) {
    if (*(s32 *)(*(s32 *)(arg0 + 0x74) + 0x14) == 4) {
        return *(s32 *)(arg0 + 0x88) + arg1 * 0x10 + 8;
    }
    return *(s32 *)(arg0 + 0x8c) + arg1 * 0x2c + 0xc;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_0025A188);

INCLUDE_ASM(const s32, "game/code_00250010", func_0025A280);

INCLUDE_ASM(const s32, "game/code_00250010", func_0025CAF8);

s32 func_0025CC90(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = 0xC7;
    if (arg0 != 0x31F) {
        temp_v0 = arg0 - 0x259;
        if (arg0 >= 0x320) {
            temp_v0 = (arg0 < 0x384) ? (arg0 - 0x258) : (arg0 - 0x29E);
        }
    }
    return ((temp_v0 + 0x100) << 0x10) + arg1;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_0025CCC8);

INCLUDE_ASM(const s32, "game/code_00250010", func_0025CD10);

INCLUDE_ASM(const s32, "game/code_00250010", func_0025CD50);

INCLUDE_ASM(const s32, "game/code_00250010", func_0025CDA8);

INCLUDE_ASM(const s32, "game/code_00250010", func_0025CE10);

INCLUDE_ASM(const s32, "game/code_00250010", func_0025CE68);

INCLUDE_ASM(const s32, "game/code_00250010", func_0025CEB0);

INCLUDE_ASM(const s32, "game/code_00250010", func_0025CF00);

INCLUDE_ASM(const s32, "game/code_00250010", func_0025CF48);

void func_0025CF70(u32 arg0) {
    u8 temp_v0 [32];

    func_0025CF48(arg0, temp_v0);
    func_00101740(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_00250010", func_0025CF98);

INCLUDE_ASM(const s32, "game/code_00250010", func_0025CFD0);

INCLUDE_ASM(const s32, "game/code_00250010", func_0025D008);

INCLUDE_ASM(const s32, "game/code_00250010", func_0025D0B8);

INCLUDE_ASM(const s32, "game/code_00250010", func_0025D140);
INCLUDE_SDATA(const s32, "game/code_00250010", D_004373C0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373C8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373CC);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373D0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373E0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373E8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373F0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373F8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437400);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437408);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437410);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437418);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437420);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437428);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437430);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437438);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437440);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437448);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437450);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437458);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437460);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437468);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437470);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437478);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437480);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437488);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437490);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437498);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374A0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374A8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374B0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374B8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374C0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374C8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374D0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374D8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374E0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374E8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374F0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374F8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437500);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437508);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437510);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437518);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437520);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437528);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437530);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437538);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437540);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437548);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437550);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437558);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437560);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437568);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437570);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437578);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437580);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437588);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437590);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437598);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375A0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375A8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375B0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375B8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375C0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375C8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375D0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375D8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375E0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375E8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375F0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375F8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437600);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437608);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437610);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437618);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437620);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437628);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437630);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437638);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437640);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437648);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437650);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437658);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437660);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437668);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437670);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437678);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437680);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437688);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437690);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437698);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376A0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376A8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376B0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376B8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376C0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376C8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376D8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376E0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376E8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376F0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376F8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437700);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437708);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437710);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437718);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437720);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437728);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437730);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437738);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437740);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437748);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437750);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437758);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437760);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437768);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437770);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437778);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437780);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437788);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437790);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437798);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377A0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377A8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377B0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377B8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377C0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377C8);


INCLUDE_SDATA(const s32, "game/code_00250010", D_004377D0);

