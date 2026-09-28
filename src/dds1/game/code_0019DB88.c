#include "common.h"

extern s32 func_002E92C0(u32);

extern u64 func_001F4398(void);

extern s32 func_0026A720(void);

extern u32 func_001F2378(void);

extern u32 D_003BA904;

extern s32 func_002D3EE8(void);

extern u32 D_003BB694;
extern u32 D_003BB698;
extern s8 D_00324530[];
extern u8 D_003583A0[];
extern void *D_00358408[];
extern void *D_00358450[];
extern s32 D_00358510[];
extern s32 D_00359A78[];
extern s32 D_00359A90[];
extern u8 D_0035F5D0[];
extern char D_003BB6B0[];
extern u64 func_0010FA80(void);
extern u32 func_001165A8(u64);

extern s32 func_001D6360(void);

extern s32 func_0021F600(u32);
extern void func_00215FE0(s32);

extern u32 D_003BB5EC;
extern u32 D_003BB5F0;

extern s32 func_00214868(void);

extern u64 func_001C85B8(u64, u64, u64);

extern s32 D_003BAA1C;

extern u32 func_001C1688(void);

extern u64 func_001978E8(s32, s32, u64, u64, u64, u64);

extern s32 D_003BB3D8;
extern u32 D_003BB3DC;

extern u32 D_003BD834;
extern u32 D_003BD838;
extern u32 D_003BD830;

extern u32 D_003BB3A8;
extern u32 D_003BB3A4;
extern u32 D_003BB3B0;
extern u32 D_003BB3AC;

extern u32 D_003BB3BC;

extern u32 D_003BB3C4;

extern u32 D_003BB3CC;

extern u32 func_00101A70(s64);

extern u32 D_003BB3C8;
extern s64 kwlnTaskGetTaskByName(u32);
extern s64 func_001019C8(s64);
extern s32 func_001ADCB8(s32);

extern u32 D_003BB3E0;

extern s32 func_001A8DD8(u32, u32);

extern s64 func_001A4078(s32, u64);
extern void func_001DEFE0(s32, s32, f32);
extern void func_001B83D8(s32, s32, s32);
extern void func_001D5DF8(s32, s32, f32, s32);

extern s32 func_001A17F0(void);

extern s32 D_003BAA68;

extern s32 D_003BAA14;
extern s32 D_003BAA28;

extern s32 D_003BAA10;
extern s32 D_003BAA20;
extern s32 D_003BAA30;
extern s32 D_003BAA4C;
extern s32 D_003BAA50;

extern s32 D_003BAA00;
extern s32 D_003BAA54;
extern s32 D_003BAA60;
extern s32 func_00119AA8();

extern s32 func_001A1B78(s32);

extern s32 D_003BB2E4;
extern s32 func_00101938(u32);

extern u64 D_003BB2E8;

extern s8 D_00358308[13];
extern u32 D_003BB240;
extern s32 D_003BB244;
extern s32 func_001986E0(u32);

void func_0019DB88(s32 arg0) {
    for (; arg0 != 0; arg0 = *(s32 *)(arg0 + 0x24)) {
        func_00195388(arg0);
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019DBC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019DCB8);

void func_0019DDA8(u32 *arg0, s32 arg1) {
    if (arg1 != 0) {
        *arg0 = 0x280;
        arg0[1] = 0xa10;
    }
    arg0[2] = 0;
    *(u16 *)(arg0 + 3) = 0xffff;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019DDD0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019DE18);

void func_0019DE58(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0;
    *(u32 *)(temp_v0 + 0) = 0;
    *(u32 *)(temp_v0 + 4) = 0;
    *(u32 *)(temp_v0 + 8) = 0;
    func_0019E048(arg0, 0, 0);
}

void func_0019DE88(s32 arg0) {
    s32 temp_v0;
    u32 *puVar2;

    puVar2 = (u32 *)(arg0 + 0x7c);
    temp_v0 = 0x1f;
    do {
        temp_v0 = temp_v0 - 1;
        *puVar2 = 0;
        puVar2 = puVar2 + -1;
    } while (-1 < temp_v0);
}

void func_0019DEB8(s32 arg0, s32 arg1) {
    if (arg1 == 0) {
        *(u8 *)arg0 = 0;
    }
    *(u16 *)(arg0 + 2) = 0;
    *(u16 *)(arg0 + 6) = 0;
    *(u16 *)(arg0 + 4) = 0x40;
    *(u32 *)(arg0 + 8) = 0;
}

void func_0019DED8(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    *(s16 *)(arg0 + 2) = arg1;
    *(s16 *)(arg0 + 4) = arg2;
    *(s16 *)(arg0 + 6) = arg3;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019DEE8);

void func_0019DF70(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = 0x1f;
    do {
        if (*arg0 != 0) {
            func_002D0918(arg0[0x20]);
            *arg0 = 0;
        }
        temp_v0 = temp_v0 - 1;
        arg0 = arg0 + 1;
    } while (-1 < temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019DFC0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E048);

void func_0019E0F8(u32 arg0) {
    func_0019E130();
    func_0019E320(arg0);
    func_0019E4F8(arg0);
    func_0019EB18(arg0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E130);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E320);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E4F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E7B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E888);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E9D8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019EA88);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019EB18);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019EBC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019ED78);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019EE58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F0F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F4C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F660);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F6A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F710);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F770);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F850);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F9E0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019FA70);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019FAD8);

void func_0019FB78(void) {
    if (D_003BB244 != 0) {
        func_002D2D00(D_003BB244);
        D_003BB244 = 0;
    }
    D_003BB240 = (D_003BB240 + 1) & 3;
    if (D_003BB240 != 3) {
        D_003BB244 = func_001986E0(*(u32 *)(D_00358308 + D_003BB240 * 4));
    }
}

s32 func_0019FBD8(void) {
    if (D_00324530[0] < 0) {
        func_0019FB78();
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019FC08);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1510);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1528);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1540);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1558);

void func_0019FCA8(void) {
    func_003003F0("********* AAAA ********\n");
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019FCC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019FE00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019FF60);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A04C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A0910);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A0A78);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A0AD0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A0B28);

void func_001A0CA0(void) {
    D_003BB2E8 = 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A0CB0);

void func_001A0CD8(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0xbe0;
    do {
        temp_v0 = temp_v1 + 1;
        func_0021F5C0(temp_v1);
        temp_v1 = temp_v0;
    } while (temp_v0 < 0xbff);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A0D18);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A0DC0);

void func_001A0E38(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A0E40);

void func_001A0ED0(void) {
    s64 temp_v0;

    temp_v0 = func_00101938(0x3f9);
    if (temp_v0 != 0) {
        kwlnTaskDestroyWithHierarchy(*(u32 *)(D_003BB2E4 + 0x29c), 1);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A0F10);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A1068);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A11F0);

void func_001A1410(void) {
    func_001F8280();
    func_001C44F8();
    func_001F2F00();
}

u8 func_001A1438(void) {
    return D_003BB2E4 != 0;
}

s32 func_001A1448(void) {
    if (func_001A1438() == 0) {
        return 0;
    }
    return (*(s32 *)(D_003BB2E4 + 0x1f4) & 0x6000000) == 0x6000000;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A1480);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A14C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A1530);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A1668);

s32 func_001A17F0(void) {
    return D_003BB2E4;
}

u16 func_001A17F8(s32 arg0) {
    return *(u16 *)(arg0 + 6);
}

u16 func_001A1800(s32 arg0) {
    return *(u16 *)(arg0 + 10);
}

void func_001A1808(void) {
    func_00118368();
}

void func_001A1820(void) {
    func_00118408();
}

void func_001A1838(void) {
    func_001190B0();
}

void func_001A1850(void) {
    func_001191B0();
}

void func_001A1868(void) {
    func_001192B0();
}

void func_001A1880(void) {
    func_001192D8();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A1898);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A18E8);

u16 func_001A1938(s32 arg0) {
    return *(u16 *)(arg0 + 0xe) & 0x7fff;
}

void func_001A1948(void) {
    func_00119018();
}

void func_001A1960(void) {
    func_00119098();
}

void func_001A1978(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x2f0) = arg1;
}

void func_001A1980(s32 arg0) {
    *(u32 *)(arg0 + 0x2f0) = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A1990);

s32 func_001A1B00(s32 arg0) {
    s32 temp_v0;

    if ((*(u32 *)(arg0 + 0x110) & 0x400) == 0) {
        temp_v0 = func_001A1B78(*(u8 *)(arg0 + 0x2c4));
        return temp_v0;
    }
    return arg0 + 0x120;
}

s32 func_001A1B38(void) {
    s32 temp_v0;

    temp_v0 = func_00119AA8();
    return D_003BAA00 + temp_v0 * 0x1a4 + 0xa60;
}

s32 func_001A1B78(s32 arg0) {
    return D_003BAA00 + arg0 * 0x1a4 + 0xa60;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A1BA0);

s32 func_001A1CB8(s32 arg0) {
    return func_00119AA8(*(u16 *)(arg0 + 0x124));
}

void func_001A1CD0(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A1CD8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A1D48);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A2258);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A2608);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A2978);

void func_001A29B8(void) {
    func_00119300();
}

void func_001A29D0(void) {
    func_00119368();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A29E8);

s8 func_001A2AE8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_003BAA54 + arg0 * 0x10;
    return *(s8 *)(temp_v0 - 0x1aa4);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A2B00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A2CC0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A2DE8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A2F00);

void func_001A2F50(s32 arg0) {
    func_001A2F68(arg0 + 0x120);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A2F68);

s32 func_001A2FD8(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return D_003BAA10 + arg1 * 0x270;
    }
    return D_003BAA20 + arg1 * 0x270;
}

s32 func_001A3020(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return (s32)D_003583A0;
    }
    return D_003BAA30 + arg1 * 24;
}

s32 func_001A3050(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return D_003BAA14 + arg1 * 0x74;
    }
    return D_003BAA28 + arg1 * 0x74;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A3098);

u16 func_001A30E0(s32 arg0) {
    return *(u16 *)(arg0 * 8 + D_003BAA68 + 2);
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1788);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A30F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A3360);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A3500);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A3638);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A3740);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A3CE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A3F38);

void func_001A4060(void) {
    func_00119750();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4078);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A40E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4130);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4240);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4328);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4520);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4598);

u32 func_001A4630(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    return *(u32 *)(temp_v0 + 0x254);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4650);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A46F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4750);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A47F0);

void func_001A4860(u32 arg0) {
    u32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    do {
        temp_v0 = temp_v1 + 1;
        func_001A49E8(arg0, temp_v1);
        temp_v1 = temp_v0;
    } while (temp_v0 < 7);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A48B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A48E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4948);

void func_001A49D0(s32 arg0, s32 arg1, u16 arg2) {
    *(u16 *)(arg1 * 6 + arg0 + 0x2c6) = arg2;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A49E8);

s16 func_001A4A18(s32 arg0, s32 arg1) {
    return *(s16 *)(arg1 * 6 + arg0 + 0x2c6);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4A30);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4C68);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4F48);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4FE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A5030);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A51C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A53D8);

u32 func_001A5578(s32 arg0, s64 arg1) {
    s64 temp_v0;

    if ((arg1 == 0) && (temp_v0 = func_001A4078(arg0 + 0x120, 0x254), temp_v0 != 0)) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A55A8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A5640);

void *func_001A5670(s32 arg0) {
    return D_00358450[*(u16 *)(arg0 + 0x124)];
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A18F8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1908);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1918);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A5690);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A57A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A5958);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A5C40);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6118);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6570);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A66B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6708);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6778);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6828);

f32 func_001A6958(void) {
    return 1.5f;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6968);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6A50);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6AA0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6B40);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6BE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6EC0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6F98);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A7180);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A73A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A7410);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A7548);

void func_001A7AD0(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A7AD8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A7C20);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A7D38);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A7DF0);

u32 func_001A7ED8(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A7EE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A7F88);

s32 func_001A8018(s32 arg0) {
    u16 temp_v0;
    u16 temp_v1;

    temp_v0 = *(u16 *)(arg0 + 0x126);
    temp_v1 = *(u16 *)(arg0 + 0x128);
    return temp_v0 * 100 / temp_v1 < 0x19;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8050);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8098);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8148);

s32 func_001A8178(s32 arg0) {
    if ((*(u16 *)(arg0 + 0x12e) & 0x1000) != 0) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8188);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8330);

s32 func_001A8410(s32 arg0) {
    u16 temp_v0;

    temp_v0 = *(u16 *)(D_003BAA50 + arg0 * 56 + 0x2e);
    return D_00358510[temp_v0 * 3];
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8448);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A84B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A84F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8538);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8640);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A87A0);

s32 func_001A8818(s32 arg0) {
    if ((*(u16 *)(arg0 + 0x12e) & 0x40) != 0) {
        return 0;
    }
    return (func_001A2978(arg0 + 0x120) & 0x40) < 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8850);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8A30);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8C68);

u32 func_001A8CC8(s32 arg0) {
    return ((*(s32 *)(arg0 + 0x110) >> 0x1a) ^ 1U) & 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8CE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8DD8);

u8 func_001A8EA8(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_001A8DD8(arg0, 0);
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8EC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8F10);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8FF0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A9068);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A90B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A90F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A91A8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A9200);

void func_001A92B8(u32 arg0) {
    func_001A9200(arg0, 1);
}

void func_001A92D0(u32 arg0) {
    func_001A9200(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A92E8);

void func_001A93A0(u32 arg0) {
    func_001A92E8(arg0, 1);
}

void func_001A93B8(u32 arg0) {
    func_001A92E8(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A93D0);

void func_001A9488(u32 arg0) {
    func_001A93D0(arg0, 1);
}

void func_001A94A0(u32 arg0) {
    func_001A93D0(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A94B8);

void func_001A9598(u32 arg0, u32 arg1) {
    func_001A94B8(arg0, arg1, 1);
}

void __udivdi3(u32 arg0, u32 arg1) {
    func_001A94B8(arg0, arg1, 0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A95C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A9780);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1D28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A99B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A9CD0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A9D38);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A9E50);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A9EF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A9F40);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AA030);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AA130);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AA548);

u32 func_001AA848(void) {
    func_001A17F0();
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AA868);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AA8B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AAA18);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AAAD8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AAB30);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AAB88);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AABE8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AAD08);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AAD50);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1FC0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1FD8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1FE8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AADF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AB150);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AB270);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AB558);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AB810);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ABDF8);

void func_001AC058(void) {
    func_002CFF98(D_003BB3E0);
    D_003BB3E0 = 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC080);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC398);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC4A8);

void func_001AC5F8(s32 arg0, s16 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5) {
    *(u8 *)(arg0 + 0) = 1;
    *(u8 *)(arg0 + 0x28) = arg4 * 8 + 0x18;
    *(u16 *)(arg0 + 2) = arg1;
    *(f32 *)(arg0 + 4) = arg5;
    *(u32 *)(arg0 + 0x18) = arg2;
    *(u32 *)(arg0 + 0x1c) = arg3;
    *(u32 *)(arg0 + 0x24) = 0;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2050);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2070);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2090);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A20A0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A20D0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2100);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2128);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2158);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2168);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A21A8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A21B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC628);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC6D8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC760);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC7D8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC920);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC9E8);

void func_001ACAE0(void) {
    s32 temp_v0;
    s32 buf[4];

    temp_v0 = func_001A17F0();
    func_001C4198(temp_v0, buf);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ACB08);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ACC20);

void func_001ACCF0(void) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 2;
    puVar1 = (u32 *)(D_003BAA00 + 0x2e9dc);
    do {
        temp_v0 = temp_v0 - 1;
        *puVar1 = 0;
        puVar1 = puVar1 + 1;
    } while (-1 < temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ACD30);

void func_001ACDF0(void) {
    s32 *temp_v0;
    s32 temp_v1;

    temp_v0 = D_00359A78;
    temp_v0 += 2;
    temp_v1 = 2;
    do {
        temp_v1 = temp_v1 - 1;
        *temp_v0 = 0;
        temp_v0 = temp_v0 - 1;
    } while (-1 < temp_v1);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ACE28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ACED0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ACF10);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ACFE0);

u32 func_001AD1B0(void) {
    s64 temp_v0;

    temp_v0 = func_001ADCB8(10);
    if (temp_v0 != 0) {
        temp_v0 = func_001019C8(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3C8);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

void func_001AD1F8(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(D_003BB3C8);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)func_00101A70(temp_v0);
        *puVar1 = 2;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AD230);

u32 func_001AD3A8(void) {
    s64 temp_v0;

    temp_v0 = func_001ADCB8(0xb);
    if (temp_v0 != 0) {
        temp_v0 = func_001019C8(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3CC);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

s32 func_001AD3F0(void) {
    if (func_001AD3A8() == 0) {
        return 0x80;
    }
    return *(s8 *)func_00101A70(kwlnTaskGetTaskByName(D_003BB3CC));
}

u32 func_001AD428(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(D_003BB3CC);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)func_00101A70(temp_v0);
        *puVar1 = 2;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AD468);

void func_001AD5A0(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(D_003BB3C4);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)func_00101A70(temp_v0);
        *puVar1 = 2;
    }
}

u32 func_001AD5D8(void) {
    s64 temp_v0;

    temp_v0 = func_001ADCB8(9);
    if (temp_v0 != 0) {
        temp_v0 = func_001019C8(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3C4);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AD620);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AD668);

void func_001AD6D8(void) {
    func_001A17F0();
    func_00101A70(func_001ADCB8(7));
    *(u8 *)(D_003BB3D8 + 0x48) = 0;
}

u32 func_001AD710(void) {
    s64 temp_v0;

    temp_v0 = func_001ADCB8(6);
    if (temp_v0 != 0) {
        temp_v0 = func_001019C8(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3BC);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AD758);

u32 func_001AD928(void) {
    s64 temp_v0;

    temp_v0 = func_001ADCB8(1);
    if (temp_v0 != 0) {
        temp_v0 = func_001019C8(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3AC);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AD970);

u32 func_001ADB30(void) {
    s64 temp_v0;

    temp_v0 = func_001ADCB8(0);
    if (temp_v0 != 0) {
        temp_v0 = func_001019C8(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3A8);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ADB78);

s32 func_001ADCB8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_003BB3D8 + arg0 * 4;
    return *(s32 *)temp_v0;
}

void func_001ADCD0(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = D_003BB3D8 + arg0 * 4;
    *(s32 *)temp_v0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ADCE8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ADE68);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AE1B8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2228);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2258);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2270);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2298);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A22C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AE250);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AE540);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AEDF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AEE78);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2450);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2460);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AF058);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AF5D0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2490);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A24A0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A24D0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2500);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2530);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AFF78);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B0460);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A25C0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A25D0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A25F8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2608);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A27C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B05D0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B0938);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B09A8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B0C70);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B0CB8);

u8 func_001B0D58(s32 arg0) {
    return (~*(u64 *)(arg0 + 0x110) & 0x201) == 0;
}

void func_001B0D70(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x2c) + 0x18);
    *(s16 *)(temp_v0 + 0x2ac) = arg1;
    *(s16 *)(temp_v0 + 0x2ae) = arg2;
    *(s16 *)(temp_v0 + 0x2b0) = arg3;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B0D88);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B0DD0);

void func_001B0E48(s32 arg0) {
    *(u32 *)(arg0 + 4) = 1;
    *(u32 *)(arg0 + 12) = 0x80;
    *(u32 *)(arg0 + 16) = 0;
    *(u32 *)(arg0 + 0) = 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B0E68);

void func_001B14E8(s64 arg0) {
    u32 temp_v0;

    temp_v0 = func_00101A70(arg0);
    func_002CFF98(temp_v0);
    func_001ADCD0(7, 0);
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2870);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2880);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2890);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A28C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B1518);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A28F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B19F8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2918);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2928);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2938);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B1C88);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2968);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B2390);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B29F8);

void func_001B2A98(s64 arg0) {
    u32 temp_v0;

    temp_v0 = func_00101A70(arg0);
    func_002CFF98(temp_v0);
    func_001ADCD0(6, 0);
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2988);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2998);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B2AC8);

void func_001B2D58(void) {
    func_002CFF98(D_003BD834);
    func_002CFF98(D_003BD838);
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A29F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B2D80);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2A30);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B3300);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B3DC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B3FB0);

void func_001B42D8(s64 arg0) {
    u32 temp_v0;

    temp_v0 = func_00101A70(arg0);
    func_002CFF98(temp_v0);
    func_001ADCD0(5, 0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B4308);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B47A8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B49E0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B4A20);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B4A70);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B4C98);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B4CE8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B4D58);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2A70);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2A80);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2A90);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2AA0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2AB0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2AC0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2AD0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2AE0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2AF0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B4F10);

void func_001B5370(void) {
    func_001A17F0();
    func_002CFF98(D_003BB3DC);
    D_003BB3DC = 0;
    func_001ADCD0(4, 0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B53A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B53E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B5480);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B54C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B5550);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2B20);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B55A8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B5910);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2B50);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B5970);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B5B68);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B5BC0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B5C28);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2B80);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B5CD8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2BD0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B60E8);

void func_001B62D0(void) {
    if (func_001ADCB8(2) != 0) {
        func_002CFF98(D_003BD830);
    }
    func_001ADCD0(2, 0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B6308);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B6498);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2C10);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2C28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B6850);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B6CF8);

void func_001B7178(s64 arg0) {
    u32 temp_v0;

    temp_v0 = func_00101A70(arg0);
    func_002D0918(*(s32 *)temp_v0);
    func_001ADCD0(3, 0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B71A8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B7238);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B74C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B7838);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B7880);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B7C90);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B7F50);

void func_001B83A0(void) {
    s64 temp_v0;
    u32 temp_v1;

    temp_v0 = kwlnTaskGetTaskByName(D_003BB3B0);
    temp_v1 = func_00101A70(temp_v0);
    func_002D0918(*(s32 *)(temp_v1 + 0x1200));
    func_001ADCD0(8, 0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B83D8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B8550);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B8650);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B8778);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B8838);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B89B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B8B20);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B8B70);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B8BB0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B8CB8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B91F0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2C70);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2C90);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2CB0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2CC0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2CD0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2CE8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2D00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B9318);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B96F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B9A50);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B9E98);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2D28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BA198);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BA408);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BA660);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BAB08);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BAE08);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BB118);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BB440);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2D58);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2D68);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BB6C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BB990);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BBE18);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BC540);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BC978);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BCB18);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BCB88);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BCCE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BCDC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BCE48);

void func_001BCE90(void) {
}

void func_001BCE98(void) {
}

void func_001BCEA0(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    func_00197220(0x13);
    temp_v0 = func_001978E8(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_001958A0(temp_v0, 1, 0x53);
    func_00194920(temp_v0);
    func_00197220(0xffffffffffffffff);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BCF28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BCFC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BD070);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BD0D0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2DB0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2DC8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2DD8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BD190);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BD2C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BD4F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BD658);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BD708);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BD750);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BDE98);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BDF60);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BE590);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BE8A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BEA80);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BEB58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BF040);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2E50);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2E60);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2E70);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2E80);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2E90);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2EA0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BF0F8);

void func_001BF3C8(s64 arg0) {
    u32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_00101A70(arg0);
    func_002CFF98(temp_v0);
    temp_v1 = func_001A17F0();
    *(u32 *)(temp_v1 + 0x2a8) = 0;
    func_001B2D58();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BF3F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BF448);

s64 func_001BF488(void) {
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(D_003BB3A4);
    if (temp_v0 == 0) {
        return temp_v0;
    }
    return *(s32 *)func_001BF448();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BF4C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BF790);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BF7C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BF8B0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2EE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BFAD0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2FB8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2FE8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BFDE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C0650);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3020);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3030);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C08B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C0DF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C1188);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3078);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C1290);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C13E8);

void func_001C1620(u32 arg0) {
    func_001DAE08(*(u32 *)((s32)arg0 + 0x10));
    func_001DAE08(*(u32 *)((s32)arg0 + 0xc));
    func_002CFF98(arg0);
}

void func_001C1658(s64 arg0) {
    u32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_00101A70(arg0);
    func_001C1620(temp_v0);
    temp_v1 = func_001A17F0();
    *(u32 *)(temp_v1 + 0x2ac) = 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C1688);

u32 func_001C16C8(void) {
    u32 *puVar1;

    puVar1 = (u32 *)func_001C1688();
    return *puVar1;
}

u32 func_001C16E8(void) {
    u32 *puVar1;

    puVar1 = (u32 *)(func_001C1688() + 0x10);
    return *puVar1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C1708);

void func_001C1820(void) {
    u32 *temp_v0;

    temp_v0 = (u32 *)func_001C1688();
    if (temp_v0 != 0) {
        *temp_v0 = 6;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C1850);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A30A8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A30D8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C1988);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3130);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3140);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A31A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C2158);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C27B0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3220);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C2938);

void func_001C2E60(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = arg1 * 0xa0 + *(s32 *)(arg0 + 0x18);
    *(s32 *)(temp_v0 + 0xc) = *(s32 *)(temp_v0 + 0x7c) << 4;
    *(s32 *)(temp_v0 + 0x10) = *(s32 *)(temp_v0 + 0x80) << 3;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C2E90);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C2FC0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C3040);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C32B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C3B28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C3F78);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4048);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C40A8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4100);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4198);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4278);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4418);

void func_001C4470(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 0x2b0) = 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4490);

void func_001C44D0(void) {
    func_001A17F0();
    func_001BCB88(0, 8);
}

void func_001C44F8(void) {
    func_001AC628();
}

u32 func_001C4510(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    return *(u32 *)(arg0 * 4 + temp_v0 + 0x4b0);
}

void func_001C4540(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4548);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C45F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4658);

void func_001C48A8(void) {
}

u32 func_001C48B0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C48B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4968);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C49C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4A18);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4A80);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4F48);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C50C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C5740);

void func_001C5838(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C5840);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C58A8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C5910);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C5B90);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C5DB8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3248);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3258);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3268);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3278);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A32F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C5F80);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C6888);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C6D48);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C6E88);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C6FD0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7028);

void func_001C71C0(s32 arg0) {
    func_00215FE0(arg0);
    *(u32 *)(arg0 + 0x1fc) = *(u32 *)(arg0 + 0x1fc) | 4;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C71F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7230);

void func_001C7280(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 0x20c) = arg0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C72A8);

void func_001C7328(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    func_001C7230(1);
    *(u32 *)(temp_v0 + 0x20c) = 0;
}

void func_001C7360(void) {
}

s32 func_001C7368(void) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(func_001A17F0() + 0x208);
    return D_00359A90[temp_v0 * 3];
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C73A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7418);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7478);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C74F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C75C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7648);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7690);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7980);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C79E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7B58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7CB0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7D60);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7DB8);

void func_001C7E40(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    func_001C75C8(temp_v0 + 0x2ec, 0x14);
    func_001C74F0(temp_v0 + 0x33c, 0x2d);
    func_001C74F0(temp_v0 + 0x3f0, 0xf);
    func_001C7690();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7E90);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7F40);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8000);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8070);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C80C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8198);

void func_001C8258(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) | 0xc;
}

void func_001C8280(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) & 0xfffffffb;
}

s32 func_001C82B0(void) {
    s32 temp_v0;
    s32 temp_v1;
    u32 temp_v2;

    temp_v0 = func_001A17F0();
    temp_v1 = *(s32 *)(temp_v0 + 0x42c);
    if (temp_v1 != 0) {
        return temp_v1;
    }
    temp_v2 = func_001C7418(*(u8 *)(temp_v0 + 0x2d4));
    return *(s32 *)temp_v2;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C82E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8330);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8478);

u32 func_001C8578(s32 *arg0) {
    u8 temp_v0;

    if (*arg0 == 0) {
        temp_v0 = (u8)arg0[2];
    }
    else {
        if ((*(u32 *)(*arg0 + 8) & 0x40) != 0) {
            return 1;
        }
        temp_v0 = (u8)arg0[2];
    }
    func_001C79E8(arg0[1], temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C85B8);

void func_001C8658(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) & 0xfffffffb;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8688);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C86E8);

u32 func_001C8780(u32 *arg0) {
    func_001C7CB0(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C87A0);

void func_001C8808(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) | 1;
}

void func_001C8818(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xfffffffe;
}

void func_001C8830(s32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(arg1 + 0x110);
    *(s32 *)(arg0 + 0x18) = arg1;
    if ((temp_v0 & 0x400) != 0) {
        if (0x17f < *(u16 *)(arg1 + 0x124)) {
            temp_v0 = *(u32 *)(arg0 + 8);
            goto LAB_001c8880;
        }
        *(u16 *)(arg0 + 4) =
                  (u16)*(u8 *)(((u32)*(u16 *)(arg1 + 0x124) * 0x14 -
                                                      (u32)*(u16 *)(arg1 + 0x124)) * 4 + D_003BAA1C + 0x15);
    }
    temp_v0 = *(u32 *)(arg0 + 8);
LAB_001c8880:
    *(u32 *)(arg0 + 8) = temp_v0 | 8;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A35A8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A35B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8890);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C89E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8B00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8C78);

void func_001C8D38(void) {
}

void func_001C8D40(void) {
}

void func_001C8D48(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xfffffdff;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8D60);

void func_001C8E60(s32 arg0) {
    *(u32 *)(arg0 + 8) = (*(u32 *)(arg0 + 8) | 0x10) & ~0x200;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8E78);

void func_001C8F88(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xffffffef;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8FA0);

void func_001C9090(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C9098);

void func_001C93A0(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C93A8);

void func_001C9628(u32 arg0) {
    func_001A17F0();
    *(u32 *)((s32)arg0 + 8) = *(u32 *)((s32)arg0 + 8) & 0xfffffffb;
    func_001BF4C0(arg0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C9660);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C97B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C9960);

void func_001C9C20(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xffffff7f;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C9C38);

void func_001C9E20(s32 arg0) {
    func_001B83D8(arg0, 0, 0);
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) | 0x20;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C9E58);

void func_001C9EC0(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C9EC8);

void func_001C9F58(s32 arg0) {
    func_001ACC20();
    func_001D2B18(arg0 + 0x20);
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x2f4) = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C9F90);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CA0C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CA158);

void func_001CA1F0(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CA1F8);

void func_001CB408(void) {
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3658);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3670);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CB410);

void func_001CCD10(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CCD18);

void func_001CE0C0(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CE0D8);

void func_001CE5D8(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CE5F0);

void func_001CEA58(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CEA70);

void func_001CEB78(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CEB80);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CED58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CEED0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CEFA8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CF050);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CF750);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CF7A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CFAB0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CFB10);

void func_001CFD70(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CFD78);

void func_001D0040(void) {
}

void func_001D0048(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_001C85B8(arg0, 0x1194, 1);
    func_001D4860(temp_v0);
    func_001D0728(arg0, 0x1a);
}

void func_001D0088(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0090);

void func_001D00D8(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D00E0);

void func_001D0148(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0150);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0210);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0498);

void func_001D0590(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0598);

void func_001D0668(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0680);

void func_001D06C0(void) {
}

void func_001D06C8(void) {
}

void func_001D06D0(void) {
    func_00214768();
}

void func_001D06E8(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_00214868();
    if (temp_v0 == 0) {
        func_001D0728(arg0, 6);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0728);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3738);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3748);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3758);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3768);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3778);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0768);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D07F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0870);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0918);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0958);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D09B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0BA0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0DD8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0E18);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0EA8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0F98);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D1118);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D1218);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D12A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D2B18);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D2BD0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D2C28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D2C78);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D2F08);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D2FD8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3078);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3148);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D31B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3238);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3338);

s32 func_001D33D0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0;
    func_001A4C68(*(s32 *)temp_v0, *(s32 *)(temp_v0 + 0x14), *(s16 *)(temp_v0 + 0x18));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3400);

u32 func_001D34D0(u32 *arg0) {
    if (0 < (s32)arg0[7]) {
        func_001A1978(*arg0, arg0[7]);
        func_001D5990(*arg0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3510);

u32 func_001D35E0(u32 *arg0) {
    func_001A1980(*arg0);
    func_001D5990(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3618);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3688);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3998);

u32 func_001D3A20(s32 arg0) {
    if ((*(u8 *)(*(s32 *)(arg0 + 4) * 8 + D_003BAA68 + 1) & 4) != 0) {
        func_00119900(*(s32 *)(arg0 + 4), 0xffffffffffffffff);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3A60);

u32 func_001D3AE8(s32 arg0) {
    func_00119900(*(u16 *)(arg0 + 4), 1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3B10);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3B98);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3C08);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3C90);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3D00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3D88);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3EB8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3F00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3FE8);

void func_001D43E0(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D43E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4448);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D44A8);

s32 func_001D4508(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_001A17F0();
    temp_v1 = 0;
    for (temp_v0 = *(s32 *)(temp_v0 + 0x230); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0x58)) {
        temp_v1 = temp_v1 + 1;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4558);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D45A8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D45F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4648);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4748);

u32 func_001D47D8(s32 arg0) {
    return *(u32 *)(arg0 + 0x54);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D47E0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4860);

void func_001D48C0(void) {
    D_003BB5EC = 0;
    D_003BB5F0 = 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D48D0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4A10);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4A98);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4AE0);

u32 func_001D4B28(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4B30);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4B78);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4BF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4C38);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4CA8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4DB8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4E60);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4E98);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D5238);

void func_001D52F8(u32 arg0, u32 arg1, u32 arg2) {
    s64 temp_v0;

    temp_v0 = func_0021F600(0xc0f);
    if (temp_v0 != 0) {
        func_002118D8(arg1, arg2);
        return;
    }
    func_00217068(arg1, arg2, 0);
}

void func_001D5358(u32 arg0, u32 arg1, u32 arg2) {
    s64 temp_v0;

    temp_v0 = func_0021F600(0xc0f);
    if (temp_v0 != 0) {
        func_00211708(arg1, arg2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D53B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D5440);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D54C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D5538);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3AD0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D5578);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D5990);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D5D58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D5DF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D5E70);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D5ED0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D5F98);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6018);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6050);

f32 func_001D60E0(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x110) & 2) == 0) {
        return 0.0f;
    }
    return *(f32 *)(*(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x320) + 0x8c) + 0x1c) + 0x1c);
}

void func_001D6110(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x110) & 2) != 0) {
        func_002DB538(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 800) + 0x8c) + 0x1c));
        return;
    }
}

u16 func_001D6148(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x110) & 2) == 0) {
        return 0;
    }
    return *(u16 *)(*(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x320) + 0x8c) + 0x1c) + 0x2e);
}

void func_001D6170(s32 arg0) {
    extern void func_002DB538(void *, float);

    if ((*(u32 *)(arg0 + 0x110) & 2) == 0) {
        return;
    }
    func_002DB538(*(void **)(*(s32 *)(*(s32 *)(arg0 + 0x320) + 0x8c) + 0x1c), 0.0f);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D61A8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6238);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6280);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6300);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6318);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6360);

void func_001D63E8(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_001D6360();
    if (temp_v0 == 0) {
        func_001F6498(arg0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6428);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6518);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D65A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D65D8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6640);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D66D0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D66E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6758);

void func_001D67F0(s32 arg0, s32 arg1) {
    func_00218440(*(s32 *)(*(s32 *)(arg0 + 0x320) + 0x8c), (arg1 & 0xffffff) | 0x80000000);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6820);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6898);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6918);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D69A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6A80);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3B70);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6DB8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6E48);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6FB0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D71B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D7258);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D74B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D7578);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D75B0);

u32 func_001D7658(u32 *arg0) {
    func_001D5F98(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D7678);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D76E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D7748);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D77D0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D79D8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D7A78);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D7B60);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D7C10);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D7CF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D7DA8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D7EA0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D7F90);

u32 func_001D8050(u32 *arg0) {
    func_001D54C0(*arg0);
    func_001D5238(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D8088);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D80F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D8190);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D87D0);

void func_001D88B0(s32 arg0) {
    if ((*(u32 *)(*(s32 *)(arg0 + 0xc) + 0x110) & 2) != 0) {
        func_00221FE8(*(u32 *)(*(s32 *)(arg0 + 0xc) + 800));
        return;
    }
}

u32 func_001D88E0(u32 *arg0) {
    if ((*(u64 *)(arg0[3] + 0x110) & 0x1000000002) == 0x1000000002) {
        func_00221D00(*(u32 *)(arg0[3] + 800), arg0[2], *arg0, arg0[1]);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D8930);

void func_001D89E0(s32 arg0) {
    if ((*(u32 *)(*(s32 *)(arg0 + 0x14) + 0x110) & 2) != 0) {
        func_00221FE8(*(u32 *)(*(s32 *)(arg0 + 0x14) + 800));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D8A10);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D8A50);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D8AF0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D8C48);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D8CF0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D8DE8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D8E80);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9038);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D90C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D91E0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9268);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9468);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D94F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9600);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9660);

u32 func_001D96F8(void) {
    func_001F7600();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9718);

u32 func_001D9760(void) {
    func_001F76F0();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9780);

u32 func_001D97D0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D97D8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9820);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9938);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D99B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9BA0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9C28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9E48);

u32 func_001D9EC0(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    *(u32 *)(temp_v0 + 0x110) = *(u32 *)(temp_v0 + 0x110) & 0xffffffef;
    func_001D6640(temp_v0, temp_v0 + 0x40);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9EF8);

u32 func_001D9F68(u32 *arg0) {
    func_001D6820(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9F88);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9FF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DA098);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DA128);

u32 func_001DA1B8(void) {
    func_001D5ED0();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DA1D8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DA220);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DA278);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DA2E0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DA3A8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DA468);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DA780);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DA8F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DA948);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DAA38);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DAB20);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DABA0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DABE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DAC40);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DACA0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DACF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DAD60);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DADC8);

void func_001DAE08(void) {
    func_002CFF98();
}

void func_001DAE20(s32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 4);
    *(s32 *)(arg0 + 4) = temp_v0 + 1;
    *(u32 *)(temp_v0 * 4 + *(s32 *)(arg0 + 8)) = arg1;
}

void func_001DAE40(s32 arg0) {
    *(u32 *)(arg0 + 4) = 0;
}

u32 func_001DAE48(s32 arg0) {
    return *(u32 *)(arg0 + 4);
}

u32 func_001DAE50(s32 arg0, s32 arg1) {
    return *(u32 *)(arg1 * 4 + *(s32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DAE68);

void func_001DAEE8(s32 arg0, s32 arg1, s32 arg2) {
    s32 *temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    if (arg1 == arg2) {
        return;
    }
    temp_v0 = *(s32 **)(arg0 + 8);
    temp_v1 = temp_v0[arg1];
    temp_v2 = temp_v0[arg2];
    temp_v0[arg1] = temp_v2;
    temp_v0[arg2] = temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DAF18);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DAF98);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB048);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB218);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB300);

u32 func_001DB358(void) {
    return 1;
}

u32 func_001DB360(void) {
    return 1;
}

u32 func_001DB368(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB370);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB440);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB4F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB590);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB630);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB698);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB7D0);

u32 func_001DB8F8(u32 *arg0) {
    func_001DB048(arg0[3], *arg0, arg0[1], arg0[2], arg0[4]);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB930);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB9D0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DBA10);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DBA78);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DBAF0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DBBF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DBCB0);

u32 func_001DBDF8(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    func_001DC858(temp_v0 + 0x70);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DBE20);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DBE68);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC0E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC220);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC270);

void func_001DC2A0(s32 arg0, f32 arg1) {
    *(f32 *)(arg0 + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC2A8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC338);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC3A0);

u32 func_001DC470(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    return *(u32 *)(temp_v0 + 0x174);
}

f32 func_001DC490(s32 arg0) {
    return *(f32 *)(arg0 + 0x128);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC498);

void func_001DC538(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 0x164) = 0;
    *(u32 *)(temp_v0 + 0x160) = *(u32 *)(temp_v0 + 0x160) | 0x400;
    func_001DAE40(*(u32 *)(temp_v0 + 0x188));
}

void func_001DC568(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 0x160) = *(u32 *)(temp_v0 + 0x160) & 0xffffdfff;
}

void func_001DC598(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 0x160) = *(u32 *)(temp_v0 + 0x160) | 0x2000;
}

u32 func_001DC5C0(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    return ((*(s32 *)(temp_v0 + 0x160) >> 0xd) ^ 1U) & 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC5F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC698);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC6F0);

s32 func_001DC740(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    return temp_v0 + 0x70;
}

void func_001DC760(void) {
    func_001F73E0();
}

void func_001DC778(void) {
    func_001F7428();
}

void func_001DC790(s32 arg0) {
    func_001F7470(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0xf4) + 0x18) + 0x110) & 0x600);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC7B8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3D50);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3D60);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3D70);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3D80);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3D90);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC858);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC9B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DCB10);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DCC38);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DCD00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DCD70);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DCE48);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DCEC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DCF48);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DCF88);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD068);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD128);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD198);

s32 func_001DD1C8(s32 arg0) {
    s32 temp_v1;

    temp_v1 = *(s32 *)(arg0 + 0x114);
    if (temp_v1 == 0) {
        return 0;
    }
    return ((*(s32 *)(D_003BAA50 + temp_v1 * 56 + 0x30) ^ 2) < 1U);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD200);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD270);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD2C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD310);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD348);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD378);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD3F8);

u8 func_001DD488(s32 arg0) {
    return *(s32 *)(arg0 + 0x114) == 0x5f;
}

u8 func_001DD498(s32 arg0) {
    return *(s32 *)(arg0 + 0x114) == 0x1a0;
}

void func_001DD4A8(void) {
}

void func_001DD4B0(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD4B8);

void func_001DD678(void) {
}

void func_001DD680(u32 arg0) {
    func_001DF358(arg0, arg0);
}

void func_001DD698(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD6A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD7E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD890);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DDE28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DDF20);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DE4A8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DE508);

void func_001DE5F0(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DE5F8);

void func_001DE958(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DE960);

void func_001DEA68(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DEA70);

void func_001DEBB0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0;
    if ((*(u32 *)(temp_v0 + 0xf0) & 0x10000) != 0) {
        return;
    }
    func_001E60C0(arg0, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DEBE0);

void func_001DEDB8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0;
    if ((*(u32 *)(temp_v0 + 0xf0) & 0x10000) != 0) {
        return;
    }
    func_001E6260(arg0, temp_v0);
}

void func_001DEDE8(u32 arg0) {
    func_001E6368(arg0, arg0);
}

void func_001DEE00(u32 arg0) {
    func_001E6580(arg0, arg0);
}

void func_001DEE18(u32 arg0) {
    func_001E4AC0(arg0, (s32)arg0 + 0x30, (s32)arg0 + 0xc0);
}

void func_001DEE38(void) {
    func_001E4E50();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DEE50);

void func_001DEEC0(void) {
}

void func_001DEEC8(u32 arg0) {
    if ((*(u32 *)(*(s32 *)(*(s32 *)((s32)arg0 + 0xf4) + 0x18) + 0x110) & 0x200) != 0) {
        func_001E4960(arg0, arg0);
        return;
    }
    if (*(s32 *)((s32)arg0 + 0x108) != 0x10) {
        func_001E4AA8(arg0, arg0);
        return;
    }
}

void func_001DEF20(void) {
}

void func_001DEF28(s32 arg0) {
    if (*(s32 *)(arg0 + 0xf4) != 0) {
        func_001EF158(*(s32 *)(arg0 + 0xf4));
        return;
    }
}

void func_001DEF58(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DEF60);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DEFA0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DEFE0);

void func_001DF358(s32 arg0, s32 arg1) {
    func_001DEFE0(arg0, arg1, 27.5f);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DF378);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DF410);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DF768);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DFA60);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DFAE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DFD70);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DFE28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E0100);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E0258);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E0398);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E0718);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E0B68);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E0DA0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E1288);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E16C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E1CF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E1FD8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E20C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E2578);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E2878);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E2970);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E2B98);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E2D20);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E2FF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E3310);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E37B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E3920);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E3E58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E4180);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E4578);

void func_001E4708(void) {
    func_001DF378();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E4720);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E4960);

void func_001E4AA8(s32 arg0, s32 arg1) {
    func_001DEFE0(arg0, arg1, 0.0f);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E4AC0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E4E50);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E5198);

void func_001E5460(u32 arg0) {
    func_001E2878(arg0, arg0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E5478);

void func_001E5700(u32 arg0) {
    func_001E5460(arg0);
}

void func_001E5718(void) {
    func_001E5478();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E5730);

void func_001E57F8(void) {
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3E40);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3F00);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3FC0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4000);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4180);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4190);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4310);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4320);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A43E0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A43F0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4400);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4408);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4468);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E5800);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E60C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6180);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6260);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6368);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6580);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6620);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6668);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6AC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6B58);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4668);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A46F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6BB0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E9DE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EB1B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EB368);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EBE88);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ECCA8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ED550);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ED5C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EDB20);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EE160);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EE658);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EEA78);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EEAE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EEC20);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EECF0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EED30);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EEE08);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EEED8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EEFB0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF098);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF158);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF1F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF390);

void func_001EF420(void) {
    u64 temp_v0;
    s32 temp_v1;
    u32 temp_v2;

    temp_v1 = func_001A17F0();
    temp_v0 = func_0010FA80();
    temp_v2 = func_001165A8(temp_v0);
    *(u32 *)(temp_v1 + 0x204) = temp_v2;
    D_003BB694 = 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF460);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF4A0);

void func_001EF4F8(void) {
    s64 temp_v0;
    s32 temp_v1;

    func_0021FE70();
    do {
        temp_v0 = func_002D3EE8();
    } while (temp_v0 != 0);
    func_0021FE38();
    do {
        temp_v0 = func_002D3EE8();
    } while (temp_v0 != 0);
    func_001EF4A0();
    temp_v1 = func_001A17F0();
    *(u32 *)(temp_v1 + 500) = *(u32 *)(temp_v1 + 500) & 0xfffffffd;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4AD8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF560);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF618);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF6B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF8B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF910);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF990);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF9D8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFA18);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFAD8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFB18);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFB58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFBD8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFC10);

void func_001EFC30(void) {
    func_001EFB18(0);
    func_00113E40(1);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFC50);

void func_001EFCE8(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    if ((((*(u32 *)(temp_v0 + 500) & 0x20000) != 0) && ((*(u32 *)(temp_v0 + 0x1f8) & 0x20) == 0)) &&
          ((*(u32 *)(temp_v0 + 0x1fc) & 0x4000000) == 0)) {
        func_00132BD0();
        func_00131F08();
        func_00132010();
    }
    func_001EFBD8();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFD58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFE08);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFF00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFF58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFFA8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0178);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0218);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F03A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0430);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0580);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F06E0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0920);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0998);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F09B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F09F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0A48);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0AC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0B28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0B90);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0BB0);

void func_001F0BE8(s32 arg0, s32 arg1) {
    if (*(s32 *)(arg0 + 0xc) < arg1) {
        *(s32 *)(arg0 + 0xc) = arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0C00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0C38);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0CA0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0FE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1050);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F10B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1110);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1280);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F12E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F13C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1420);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1470);

void func_001F15F8(s32 *arg0) {
    s32 temp_v0;
    s32 temp_v1;

    if (arg0[1] != 0) {
        func_00160B00(arg0[1]);
    }
    temp_v0 = *arg0;
    temp_v1 = arg0[2];
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) - 1;
    *(s32 *)(temp_v1 + 0x314) = *(s32 *)(temp_v1 + 0x314) - 1;
    func_001F1050(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1650);

void func_001F1710(s32 *arg0) {
    *(s32 *)(*arg0 + 8) = *(s32 *)(*arg0 + 8) + 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1728);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F17C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1888);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1910);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F19B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1A58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1AC0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1B28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1BB0);

u32 func_001F1C18(void) {
    func_00105888();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1C38);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1C78);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1CC8);

void func_001F1EC8(s32 *arg0) {
    s32 temp_v0;
    s32 temp_v1;

    if (arg0[1] != 0) {
        func_00160B00(arg0[1]);
    }
    temp_v0 = *arg0;
    temp_v1 = arg0[2];
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) - 1;
    *(s32 *)(temp_v1 + 0x314) = *(s32 *)(temp_v1 + 0x314) - 1;
    func_001F1050(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1F20);

u32 func_001F1FE0(u32 *arg0) {
    func_00105B48(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2000);

u32 func_001F2068(u8 *arg0) {
    func_00105B98(*arg0, arg0[1], arg0[2], *(u32 *)(arg0 + 4));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2098);

u32 func_001F2118(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) | 0x40000;
    func_0029B2B0();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2150);

u32 func_001F2198(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) & 0xfffbffff;
    func_0029B2E8();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F21D0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2218);

void func_001F22B0(s32 arg0, u16 arg1) {
    func_001608B8(*(u32 *)(arg0 + 0x10), arg1);
}

void func_001F22D0(s32 arg0, u16 arg1) {
    func_00160858(*(u32 *)(arg0 + 0x10), arg1);
}

s32 func_001F22F0(s32 arg0) {
    if (*(s32 *)(arg0 + 4) != 0) {
        return 1;
    }
    return *(u32 *)(arg0 + 8) != 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2310);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2378);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F23E0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2430);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F24A8);

void func_001F25C8(void) {
    func_0029B1D8();
}

void func_001F25E0(void) {
    func_00161A18();
    func_00292C40();
    D_003BA904 = D_003BA904 & 0xdfffffff;
}

void func_001F2618(void) {
    func_001F2648();
    func_0029B320();
    func_00161A18();
    func_00292C40();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2648);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2688);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2788);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F27C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2818);

void func_001F2B60(s32 arg0) {
    *(u8 *)(arg0 + 0x10) = 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2B70);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2BB0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2C00);

u32 func_001F2E20(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u8 *)(temp_v0 + 0x584) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2E48);

u32 func_001F2E90(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u8 *)(temp_v0 + 0x584) = 1;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2EB8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4CD8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4CF0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4D08);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4D20);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4D38);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4D50);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4D68);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4D80);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4D98);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4DB0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4DC8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4DE0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4DF8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4E10);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4E28);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4E40);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4E58);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4E70);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4E88);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4EA0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4EC0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4EE0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4F00);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4F18);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4F30);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4F48);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4F60);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4F78);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4F90);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4FA8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4FC0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4FD8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4FF0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2F00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2FE8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3048);

void func_001F30B8(s32 arg0, u32 arg1) {
    u32 temp_v0;
    s32 temp_v1;
    u32 *puVar3;

    temp_v1 = func_001A17F0();
    puVar3 = (u32 *)func_001F2378();
    temp_v0 = *puVar3;
    puVar3[5] = arg1;
    *(u32 **)(arg0 * 4 + temp_v1 + 0x4b8) = puVar3;
    *puVar3 = temp_v0 | 10;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3118);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3188);

void func_001F3200(u32 arg0) {
    func_002944D8(*(u32 *)arg0);
    func_002CFF98(arg0);
}

void func_001F3230(void) {
}

void func_001F3238(u32 arg0) {
    func_002E8F78(arg0, 0x58, 0x3f);
}

void func_001F3258(u32 arg0) {
    func_002E8F78(arg0, 0x7f, 0x3f);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3278);

u8 func_001F3390(void) {
    s32 temp_v0;

    temp_v0 = func_0026A720();
    return temp_v0 - 2U < 2;
}

void func_001F33B8(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    if ((*(u32 *)(temp_v0 + 500) & 0x10000) != 0) {
        func_0026A778();
        return;
    }
}

void func_001F33F0(void) {
    func_0026A840();
}

void func_001F3408(void) {
    func_0026A840();
    func_002E8E28();
    func_002E8E00();
}

void func_001F3430(void) {
    func_001F33F0();
}

void func_001F3448(void) {
    func_001F3408();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3460);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3498);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F34E0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3548);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F35A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3690);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3710);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3778);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F38C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3978);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F39C0);

s32 func_001F3A30(void) {
    return (s8)func_002E97E0();
}

s32 func_001F3A58(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)arg0;
    if ((temp_v0 & 1) != 0) {
        return 1;
    }
    return (temp_v0 & 8) > 0;
}

void func_001F3A78(s32 arg0, s32 arg1) {
    func_003014F0(arg1, "%sMIDI%04X.SMG", D_003BB6B0, (arg0 + 0x200) & 0xffff);
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A5158);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A5168);

void func_001F3AA8(s32 arg0, s32 arg1) {
    func_003014F0(arg1, "MDD_%03X.ADB", *(u16 *)(arg0 + 0x124));
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3AD0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3B38);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3B90);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3BF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3C38);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A5178);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A5188);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A5198);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3CC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3DF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3E70);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3F40);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4038);

void func_001F4078(void) {
    u64 temp_v0;

    temp_v0 = func_001F4398();
    func_001D4860(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4098);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F40F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F41C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4398);

void func_001F4430(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(s32 *)(temp_v0 + 0x264) = -1;
    *(s32 *)(temp_v0 + 0x268) = -1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4458);

u8 func_001F44A0(void) {
    s64 temp_v0;

    temp_v0 = func_002E92C0(0x10000);
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F44C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4720);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4798);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F47E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4828);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4950);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F49D0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4AA8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4B98);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4BF0);

u32 func_001F4C90(void) {
    func_001F3460(0x1c);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4CB0);

u32 func_001F4CF0(void) {
    func_001F33F0();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4D10);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4D50);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F5028);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F53C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F53F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F5410);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F55D0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F57D0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F5A20);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F5B50);

void func_001F5D00(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F5D08);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F5ED8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F6030);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A5410);

