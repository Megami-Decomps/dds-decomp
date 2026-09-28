#include "common.h"

extern s32 func_002A46C8(s32);

extern u32 D_00438FE8;

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

extern s32 func_00101958();

extern s32 D_004379F8;

extern u32 D_00454D30[];

extern u32 D_00438FEC;

extern u32 D_00455D70[];

extern char D_00428680[]; /* "titleProc" */

extern char D_00429938[]; /* "staffImageProc" */

extern char D_00429968[]; /* "staffProc" */

extern u8 D_003E5608[];

extern u8 D_003803C8[];

extern u32 D_00437ACC;

extern u8 D_00457E60[];

extern char D_0042A418[];

extern u32 D_00457E48[];

extern s32 func_002A88A0();

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029BC58);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029BEB8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029BFB8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C078);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C0D0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C120);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C3F0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C450);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C4D8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C618);

void func_0029C800(void) {
}

void func_0029C808(void) {
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C810);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C848);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C860);

void func_0029C878(void) {
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C880);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029CA68);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029CB70);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029CC90);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029CD60);

void func_0029CDD8(void) {
    func_0029C810();
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029CDF0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029CE30);

u32 func_0029CE80(void) {
    return 1;
}

u32 func_0029CE88(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029CE90);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029CF00);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029CF88);

u32 func_0029D000(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029D008);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029D1C8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029D278);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029D2D8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029D3D8);

s32 func_0029D508(u8 *entry) {
    s32 step = func_0029D1C8(entry);
    *(u16 *)(entry + 0x14) += step;
    func_003144E8(entry);
    return step;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029D550);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029D5B8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029D790);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029D900);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029D970);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029DA58);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029DA98);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029DB58);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029DF18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428410);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428420);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004284E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428550);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428560);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428570);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029DFB0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029E220);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029E478);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029E548);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029E820);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029EE80);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029F440);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029FA98);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029FBE0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A0148);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A0278);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A05C0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A08D8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A0EE8);

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

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1040);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1070);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A10A0);

u32 func_002A1118(void) {
    return 0x608;
}

u32 func_002A1120(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) + 1;
    return 0;
}

void func_002A1150(void) {
    func_00328E48(func_00101958());
    D_004379F8 = 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1178);

void func_002A11E8(s32 effect) {
    s32 context = func_00101958(D_004379F8);
    if (func_00342688() != 0) {
        func_00342690();
    }
    func_00342630(effect, 0x7f);
    *(s32 *)(context + 4) = 0;
}

void func_002A1248(s32 arg0) {
    *(s32 *)func_00101958(D_004379F8) = arg0;
}

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428590);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004285A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004285B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004285C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004285D0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004285E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004285F0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428600);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1278);

void func_002A1308(void) {
    func_00342690();
}

void func_002A1320(void) {
    func_00342688();
}

void func_002A1338(void) {
}

s32 func_002A1340(void) {
    return *(s32 *)(func_00101958(D_004379F8) + 4);
}

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

s32 func_002A13E0(void) {
    s32 value;
    value = func_0010D7D0(0);
    if (func_00342688() != 0) {
        func_00342690();
    }
    func_00342630(value, 0x7f);
    return 1;
}

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

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1480);

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

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1558);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A15B0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1678);

void func_002A1760(u32 arg0) {
    sceSdRemoteInit();
    func_002A15B0(arg0);
    D_00437A2C = 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1790);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1820);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A18B0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1928);

void func_002A1DD8(void) {
    for (;;) {
        func_00328988(1);
        WaitSema(D_00438FE8);
        func_002A1928();
        SignalSema(D_00438FE8);
    }
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1E08);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1E58);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1F50);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1FA0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1FF0);

void func_002A2070(void) {
    D_00438FEC = func_002C80C8();
    D_00454D30[9] = 1;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A20A0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2198);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2200);

u32 func_002A22F0(void) {
    if (D_00454D30[9] == 1) {
        func_002A20A0(D_00454D30);
    }
    return D_00454D30[9];
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2330);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2388);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2408);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2440);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A24A0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2500);

void func_002A2550(void) {
    WaitSema(D_00438FE8);
    func_002A2500();
    SignalSema(D_00438FE8);
}

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428650);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2580);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2628);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A27A8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2928);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2998);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A29D8);

void func_002A2A00(void) {
    u32 *temp_v0 = D_00455D70;
    u32 temp_v1 = temp_v0[8];

    if (temp_v1 == 0) {
        return;
    }
    func_003298C0(temp_v1);
    temp_v0[8] = 0;
}

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

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2AC0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2BD0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428680);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2C28);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A30C0);

u32 func_002A3A50(void) {
    func_002A2AC0();
    return 0xffffffff;
}

s32 func_002A3A70(void) {
    func_002A2BD0();
    kwlnTaskDestroyWithHierarchyByName(D_00428680, 1);
    return 0;
}

u32 func_002A3AA0(void) {
    func_002A2AC0(0);
    return 0;
}

void func_002A3AC0(void) {
    func_0023A9A8();
    func_00117998();
    func_00117908();
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3AE8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3B10);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3B28);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3BE0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3C58);

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

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3CE0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3D70);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3DE8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3E38);

u8 func_002A3EA8(void) {
    return *(s32 *)(D_00437A40 + 8) != 0;
}

void func_002A3EB8(void) {
    if (*(s32 *)(D_00437A40 + 8) != 0) {
        func_003054E8(*(s32 *)(D_00437A40 + 8));
        *(u32 *)(D_00437A40 + 8) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3EF0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3F28);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3F60);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3F98);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3FE0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A40C8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4110);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A41C0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4208);

void func_002A4380(s32 arg0) {
    *(u8 *)(arg0 + 8) = 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4388);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A44C0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4670);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A46C8);

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

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4770);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A47E8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4870);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A48F0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A49C0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4A68);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4B70);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4D28);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4DF0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4E48);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4F20);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5040);

void func_002A50E8(s32 arg0, s32 arg1, u8 arg2) {
    *(u8 *)(arg0 + arg1) = arg2;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A50F8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5128);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5260);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A55B8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5890);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A58C0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A58D8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A58E8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5A20);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5A78);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5B08);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5C40);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5C58);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5E00);

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

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5F80);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004287E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004287F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428810);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428820);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428840);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428860);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428870);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428880);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428898);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004288A8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004288B8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004288C8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004288D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004288E8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004288F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428908);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428920);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428930);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428948);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428958);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428968);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428978);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428998);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004289A8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004289C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004289D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004289E8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004289F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428A10);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428A28);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428A38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428A48);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428A60);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428A70);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428A80);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428A90);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428AA0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428AB8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428AC8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428AD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428AE8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428AF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428B08);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428B20);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428B38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428B50);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428B68);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428B78);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428B90);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428BA0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428BC0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428BD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428BF0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C08);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C28);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C48);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C58);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C68);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C78);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C88);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C98);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428CB0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428CD0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428CE0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428CF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428D10);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428D30);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428D40);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428D60);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428D70);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428D80);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428D90);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428DA0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428DB0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428DC8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428DD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428DE8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428E00);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428E18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428E30);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428E40);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428E58);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428E70);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428E88);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428EA0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428EB8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428EC8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428ED8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428EE8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F00);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F28);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F50);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F60);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F70);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F80);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F90);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428FA0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428FB0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428FC8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428FD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428FE8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428FF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429008);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429020);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429030);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429040);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429050);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429068);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429078);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429088);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429098);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004290A8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004290C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004290D0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004290E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004290F0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429100);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429118);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429130);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429140);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429150);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429160);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429170);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429180);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004291A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004291B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004291C8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004291D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004291E8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004291F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429208);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429218);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429228);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429238);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429250);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429260);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429278);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429288);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004292A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004292B8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004292C8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004292E8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004292F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429318);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429330);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429350);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429368);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429390);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004293A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004293B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004293C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004293E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429408);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429420);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429430);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429448);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429458);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429468);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429480);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429498);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004294A8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004294C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004294D0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004294E8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004294F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429508);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429518);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429530);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429540);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429560);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429570);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429588);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004295A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004295B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004295C8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004295E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004295F0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429600);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429620);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429638);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429650);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429660);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429678);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429688);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004296A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004296B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004296C8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004296D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004296F0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429708);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429720);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429730);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429740);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429750);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429760);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429778);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429788);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429798);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004297A8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004297C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004297D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004297E8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004297F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429808);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429830);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429840);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429858);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429868);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429878);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429888);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429898);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004298B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004298C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004298D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004298E8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5FC8);

void func_002A6000(void) {
    func_0019F048();
}

void func_002A6018(void) {
    func_0019F078();
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6030);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6180);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6480);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6580);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429938);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6858);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6C28);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6C70);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6D28);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6D68);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6F88);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7260);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7350);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A73C0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7560);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A75A8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7730);

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

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7900);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7938);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7980);

u32 func_002A7A10(void) {
    func_002A7980();
    return 0xffffffff;
}

s32 func_002A7A30(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00429938, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00429968, 1);
    return 0;
}

s32 movieDraw(void) {
    func_00345BA0(D_003E5608, D_003803C8);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7A98);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7AF0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7B28);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429968);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429978);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429998);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004299B8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004299D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004299F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429A18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429A38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429A58);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429A78);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429A98);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429AB8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429AD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429AF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429B18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429B38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429B58);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429B78);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429B98);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429BB8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429BD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429BF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429C18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429C38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429C58);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429C78);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429C98);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429CB8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429CD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429CF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429D18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429D38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429D58);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429D78);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429D98);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429DB8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429DD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429DF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429E18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429E38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429E58);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429E78);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429E98);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429EB8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429ED8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429EF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429F18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429F38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429F58);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429F78);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429F98);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429FB8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429FD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429FF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A018);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A038);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A058);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A078);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A098);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A0B8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A0D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A0F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A118);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A138);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A158);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A178);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A198);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A1B8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A1D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A1F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A218);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A238);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A258);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A278);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A298);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A2B8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A2D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A2F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A318);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A338);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7D28);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7DB0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7E60);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7F98);

void func_002A7FD0(void) {
    if (D_00437ACC == 0) {
        return;
    }
    func_00346988(D_003E5608);
    kwlnTaskDestroyWithHierarchy(D_00437ACC, 0);
    D_00437ACC = 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A8008);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A8028);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A8038);

s32 func_002A8048(void) {
    func_00345488(0x3c / D_00435BAC);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A8080);

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

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A8120);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A81C8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A8208);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A8268);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A85C0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A8610);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A87F0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A88A0);

void func_002A8AE8(void) {
    func_002A8268();
    D_00457E48[0] = kwlnTaskCreate(D_0042A418, 0x2b02, 1, 0, func_002A88A0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A8B38);

void func_002A8B78(void) {
    D_00457E60[2] = 1;
    D_00457E60[3] = 1;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A8B90);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A8BC8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A418);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A8C80);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9068);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9130);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A91A0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9200);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A440);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A450);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A460);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A470);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A480);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A490);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A4A8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A4C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A4D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A4F0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A500);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A518);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A530);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A540);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A558);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A570);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A588);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A598);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A5B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A5C8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A5E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A5F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A608);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A620);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A638);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A650);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A668);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A680);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A690);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A6B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A6C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A6D0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A6E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A6F0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A700);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A710);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A720);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A730);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A740);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A750);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A760);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A770);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A780);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A790);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A7A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A7B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A7D0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A7E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A7F0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A800);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A810);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A820);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A830);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A840);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A850);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A870);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A888);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A8A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A8B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A8C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A8D0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A8E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A8F0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A900);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A910);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A920);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A930);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A940);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A950);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9258);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A92D8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9368);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A93F8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9460);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A94D0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9558);

void func_002A95B0(u32 arg0, u32 *arg1, u32 arg2, u32 arg3) {
    func_002BCD90(arg0, arg3, *arg1, 1, arg1[1], 0x2d, arg1[1], 0x1d);
    func_002BC498(arg0, arg1[1]);
    func_002BC5D0(arg0, arg1 + 4);
    func_002BC600(arg0, arg1 + 0xc);
    func_002BC630(arg0, arg1 + 0x14);
    func_002BCA98(arg0);
    func_002BE6E8(arg0, arg1[1]);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9640);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9788);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9820);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9908);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9A40);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9AB8);

void func_002A9BC8(s32 arg0, u32 arg1, u32 arg2, s32 arg3, u32 arg4,
                                    u32 arg5) {
    func_00306F80(arg0 + 0x60, arg1, arg2, 1, *(u32 *)(*(s32 *)(arg3 + 0x30) + 100), 10,
                                arg5);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9BF8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9E00);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9F08);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9F78);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9FF0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA068);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA0D8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA1C8);

u32 func_002AA278(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    func_002C1B70(temp_v0 + 0xaa50, 0x53);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA2A8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042AA08);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042AA18);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA360);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA498);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA4D8);

u8 func_002AA510(void) {
    s64 temp_v0;

    temp_v0 = func_00105B68();
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA530);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA740);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA7A0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA9D8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AAC70);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AAC98);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AACB8);

void func_002AAE80(u32 arg0) {
    func_002AACB8(0, arg0);
}

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042AA48);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042AC40);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042AC70);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379C0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379C8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379D0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379D8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379E0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379E8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379F0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379F8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379FC);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A00);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A08);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A10);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A18);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A1C);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A20);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A28);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A2C);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A30);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A34);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A38);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A3C);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A40);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A48);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A50);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A58);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A60);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A68);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A70);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A78);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A80);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A88);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A90);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A98);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AA0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AA8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AB0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AB4);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AB8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AC0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AC8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437ACC);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AD0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AD4);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AD8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437ADC);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AE0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AE4);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AE8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AF0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AF8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B00);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B08);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B10);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B18);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B20);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B28);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B30);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B38);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B40);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B48);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B50);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B58);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B60);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B68);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B6C);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B6E);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B72);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B73);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B78);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B80);

