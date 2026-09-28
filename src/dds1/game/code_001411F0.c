#include "common.h"

extern char D_003A0800[]; /* "fldTitle" */
extern char D_003A0828[]; /* "fldTitleMini" */

extern u64 func_0011E998(void);

extern s32 func_0013F848(s32);

extern s32 func_0013F6B8(s32);

extern s32 func_0013F5A8(s32);

extern s32 func_0013F4D8(s32);

extern s32 func_0010D428(s32);

extern s32 func_0010D6A0(void);
extern u64 func_0013BE90(u32);

extern u32 D_003BAFC8;
extern u32 D_003BAFCC;
extern u32 D_003BAFD0;
extern u32 D_003BAFD4;
extern u32 D_003BAFD8;
extern u32 D_003BAFDC;
extern u32 D_003BAFE0;
extern u32 D_003BAFE4;
extern s32 D_003BAFF4;

extern s32 D_003BAFBC;

extern s32 D_003BAFC4;

extern s32 D_003BAFB4;

extern u32 D_003BAFA0;
extern u32 D_003BAFA8;

extern u32 D_003BAFB0;

extern s32 D_003BAA00;

extern u32 D_003BAED8;

extern u32 D_003BAEDC;
extern u32 D_003BAEE0;
extern s32 D_003BAEE4;

extern u32 D_003BAEA8;

extern u32 D_003BAEA4;

extern u32 D_003BAE80;

extern u32 D_003BAE70;
extern s32 D_003BAE8C;
extern s32 D_003BAEB0;
extern s32 D_003BAEB4;
extern s32 D_0032E3B0[];
extern s32 D_003BAF8C;
extern s32 D_003BAF90;
extern s32 D_0032E474[];
extern s32 D_0033EB78[];
extern void func_002E96D8(s32 arg0);
extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);
extern s32 func_0010FD80(void);
extern s32 func_00110A38(s32 arg0);
extern void func_001313E0(void);
extern void func_001127C0(s32 arg0, s32 arg1);
extern void func_0014FB00(u32 arg0);
extern void func_00127898(u32 arg0, s32 arg1);
extern s32 func_001277A8(s32 arg0);
extern void func_00123E00(void);
extern void func_0012E9E8(s32 arg0);
extern void func_00228740(void);
extern s32 D_0032E5C4[];
extern s32 D_0032E4C4[];
extern u8 D_0034D8F0[];
extern void func_0024D9D8(void *arg0);
extern void func_0024DA58(s32 arg0);
extern void func_00123EA8(void);
extern char *func_0010D5A8(s32 idx);
extern s32 D_0032E478[];
extern f32 D_0034C8D0[];
extern f32 D_0034C8E0[];
extern s32 D_003D62C8[];
extern void *D_003D62A4[];
extern s32 D_003D62A8[];
extern s32 D_0032E3D8[];
extern s32 D_0032E400[];
extern s32 D_0032E408[];
extern void func_002E8DD0();
extern void func_002E8F78(s32 arg0, s32 arg1, s32 arg2);
extern void func_002E8D10(s32 arg0);
extern void *func_002CFEB8(s32 size);
extern void func_00101A68(s32 arg0, void *arg1);
extern void func_00141320(void);
extern s32 D_0032E3C0[];
extern s32 D_003BAE84;
extern s32 D_0032E570[];
extern void func_001130C8(s32 arg0);

typedef struct {
    s32 unk0;
    u8 pad4[0x10];
} FldEnt14; /* 0x14 bytes */
extern FldEnt14 D_003D62E0[];

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 pad4[0x10C];
} FldEnt110; /* 0x110 bytes */
extern FldEnt110 *D_003BAA48;
extern s32 func_0013C5D0(void);

typedef struct {
    s32 *unk0;
    u8 pad4[0x4C];
} FldTbl50; /* 0x50 bytes */
extern FldTbl50 D_003D46C0[];
extern s32 kwlnTaskGetTaskByName(void *name);
extern s32 kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);
extern u8 D_003BAE78[];
extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

typedef struct {
    s32 unk0;
    s32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} FldClear18; /* 0x18 bytes */
extern FldClear18 D_003D3FE0[];

extern u64 func_0010FDC0(void);
extern u32 *func_00110F80(u64 world, const char *name);
extern void func_003003F0(const char *fmt, ...);

extern u64 func_00101A70(void);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001411F0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00141320);


void *func_001413A8(s32 arg0) {
    s32 *temp_v0;

    temp_v0 = func_002CFEB8(0x10);
    temp_v0[0] = 0;
    temp_v0[1] = 0;
    temp_v0[2] = 0;
    temp_v0[3] = 0;
    func_00101A68(arg0, temp_v0);
    return func_00141320;
}

void func_001413F8(void) {
    u64 temp_v0;

    temp_v0 = func_00101A70();
    func_002CFF98(temp_v0);
    D_003BAE70 = 0;
}


void func_00141420(void) {
    if (D_003BAE70 == 0) {
        D_003BAE70 = kwlnTaskCreate(D_003BAE78, 0x2B0B, 1, 1, func_001413A8, func_001413F8, 0);
    }
}


void func_00141470(void) {
    if (D_003BAE70 != 0) {
        kwlnTaskDestroyWithHierarchy(D_003BAE70, 1);
    }
}


void func_001414A8(void) {
    FldClear18 *temp_v0 = D_003D3FE0;
    s32 temp_v1 = 7;

    do {
        temp_v1 -= 1;
        temp_v0->unk0 = 0;
        temp_v0->unk8 = 0;
        temp_v0->unk4 = 0;
        temp_v0 += 1;
    } while (temp_v1 >= 0);
    D_003BAE8C = 0;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_001414E0);


INCLUDE_ASM(const s32, "game/code_001411F0", func_00141540);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001415F8);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0470);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001419A8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00141A78);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00141B10);


void func_00141C00(void) {
    if (D_0032E3D8[0] == 0x80) {
        func_002E8D10(D_0033EB78[0] + 1);
    }
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00141C40);

void func_00141D18(void) {
    func_002E8D58(D_003BAE80);
    D_003BAE80 = 0;
}


void func_00141D40(void) {
    func_002E96D8(D_003BAE80);
    if (D_0032E3B0[10] >= 0x80) {
        D_0032E3B0[10] = 1;
    }
    D_003BAE80 = 0;
}

void func_00141D80(void) {
    func_002E8E00();
}


void func_00141D98(void) {
    func_002E8DD0(D_003BAE80);
    D_0032E3D8[0] = 0;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00141DC0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00141E88);


void func_00141F28(s32 arg0, s32 arg1) {
    if (D_0032E3C0[0] < 0x32) {
        func_002E8F78(D_003BAE84 + arg0, arg1, 0x3F);
    }
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00141F70);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00142028);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001420D8);

u32 func_00142158(void) {
    return D_003BAEA4;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00142160);

void func_001421D0(s32 arg0, s32 arg1) {
    func_002E8F78(arg0 * 0x10000 + arg1 + 0x30000000, 0x7f, 0x3f);
}

void func_00142200(s32 arg0, s32 arg1) {
    func_002E8DD0(arg0 * 0x10000 + arg1 + 0x30000000);
}

void func_00142228(void) {
    D_003BAEA8 = 0;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00142230);

u32 func_001422B8(void) {
    return D_003BAE80;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_001422C0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00142408);


void func_001425F8(s32 arg0, s32 arg1) {
    if (D_0032E3C0[0] < 0xC8) {
        s32 temp_a0 = arg0;
        s32 temp_a1 = arg1;
        s32 temp_v0 = temp_a0 + 8;

        D_003BAEE4 = temp_a1;
        func_00127898(arg0, temp_v0);
        {
            s32 temp_v1 = func_001277A8(temp_v0);
            s32 temp_4 = *(s32 *)(temp_v1 + 4);
            s32 temp_8 = *(s32 *)(temp_v1 + 8);

            D_003BAEE0 = temp_8;
            D_003BAEDC = temp_4;
        }
    }
}


void func_00142658(void) {
    s32 temp_v0 = D_0032E3C0[0];

    if (temp_v0 < 0xC8) {
        func_001422C0(temp_v0 % 100);
        func_00142408();
    }
}

void func_001426A8(void) {
    if (D_003BAEE4 != 0) {
        func_002D0A10(D_003BAEE4);
    }
    D_003BAEE4 = 0;
    D_003BAEDC = 0;
    D_003BAEE0 = 0;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_001426E0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00142758);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00142800);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001428C0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00142C00);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00142C78);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00142D18);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00142D78);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001447D0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00144D30);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00145B18);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00145D38);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00145DB0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00145EA0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00146080);

void func_001462A8(void) {
    func_002D5498();
    func_002D5498();
    func_00145DB0();
}

u32 func_001462D0(void) {
    return D_003BAED8;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_001462D8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00146900);


void func_00146A70(s32 arg0, s32 arg1, s32 arg2) {
    D_0032E3B0[4] = arg0;
    D_0032E3B0[6] = arg2;
    D_003BAEB4 = D_0032E3B0[5] = arg1;
    D_003BAEB0 = arg0 % 100;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00146AA8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00146B10);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00146BC0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00146C30);


void func_00146CA8(s32 arg0) {
    if (arg0 >= 0x40) {
        D_0032E474[0] = -1;
    } else {
        D_0032E474[0] = arg0;
    }
}


void func_00146CD0(s32 arg0) {
    if (arg0 >= 0x40) {
        D_0032E478[0] = -1;
    } else {
        D_0032E478[0] = arg0;
    }
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00146CF8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00146DC8);


INCLUDE_ASM(const s32, "game/code_001411F0", func_00146E98);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00146EE8);


INCLUDE_ASM(const s32, "game/code_001411F0", func_001470A0);


void func_001470E0(void) {
    if (D_003BAED8 == 1) {
        func_001447D0();
        func_00142D78();
    }
}

void func_00147118(void) {
    func_00146900();
    func_00144D30();
}

void func_00147138(void) {
    u64 *puVar1;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = 0;
    temp_v1 = D_003BAA00;
    do {
        puVar1 = (u64 *)(temp_v1 + 0x13f70);
        temp_v0 = 0x3f;
        do {
            temp_v0 = temp_v0 - 1;
            *puVar1 = 0;
            puVar1 = puVar1 + 1;
        } while (-1 < temp_v0);
        temp_v2 = temp_v2 + 1;
        temp_v1 = temp_v1 + 0x200;
    } while (temp_v2 < 0xd);
}

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A05D8);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0608);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0618);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00147188);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001474A0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00147580);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00147638);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00147750);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001477B8);


void func_00147898(f32 arg0, f32 arg1, f32 arg2) {
    D_003BAF8C = 1;
    D_0034C8D0[0] = arg0;
    D_0034C8D0[1] = arg1;
    D_0034C8D0[2] = arg2;
}


void func_001478B8(f32 arg0, f32 arg1, f32 arg2) {
    D_003BAF90 = 1;
    D_0034C8E0[0] = arg0;
    D_0034C8E0[1] = arg1;
    D_0034C8E0[2] = arg2;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_001478D8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00147938);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0650);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0668);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0680);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0698);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A06B0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001479B8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00147BB8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00147C20);


INCLUDE_ASM(const s32, "game/code_001411F0", func_00147CD8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00147D28);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00147DB0);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0718);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0728);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001480D0);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0758);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0768);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001486D0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00148C98);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00148CF0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00148D78);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0788);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0798);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00148FF0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001493D0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00149610);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00149748);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00149810);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00149A98);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00149B68);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00149FC0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014A060);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014A0E8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014A1A0);


s32 func_0014A250(void) {
    s32 temp_v0 = func_0013C5D0();

    if (temp_v0 < 0) {
        return 0;
    }
    return D_003D46C0[temp_v0].unk0[1];
}

void func_0014A298(u32 arg0) {
    D_003BAFB0 = arg0;
}


s32 func_0014A2A0(void) {
    return kwlnTaskGetTaskByName(D_003A0800) != 0;
}

void func_0014A2C8(void) {
    D_003BAFA8 = 0;
    D_003BAFA0 = 1;
}

INCLUDE_ASM(const s32, "game/code_001411F0", fldTitle);

void func_0014A930(void) {
    if (D_003BAFB4 != 0) {
        func_002D2D00(D_003BAFB4);
        D_003BAFB4 = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0800);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014A960);


void func_0014AA10(void) {
    if (func_0014A2A0() != 0) {
        kwlnTaskDestroyWithHierarchyByName(D_003A0800, 1);
    }
}


s32 func_0014AA50(void) {
    return kwlnTaskGetTaskByName(D_003A0828) != 0;
}

INCLUDE_ASM(const s32, "game/code_001411F0", fldTitleMini);

void func_0014B290(void) {
    if (D_003BAFC4 != 0) {
        func_002D2D00(D_003BAFC4);
        D_003BAFC4 = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0824);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0828);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014B2C0);

void func_0014B420(void) {
    if (D_003BAFBC == 0) {
        D_003BAFBC = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014B438);

void func_0014B4D0(void) {
    if (D_003BAFF4 != 0) {
        func_002D2D00(D_003BAFF4);
        D_003BAFF4 = 0;
    }
    func_0014FAB8(D_003BAFD0);
    D_003BAFD0 = 0;
    D_003BAFD4 = 0;
    func_002D0A10(D_003BAFC8);
    D_003BAFC8 = 0;
    D_003BAFCC = 0;
    func_0014FAB8(D_003BAFE0);
    D_003BAFE0 = 0;
    D_003BAFE4 = 0;
    func_002D0A10(D_003BAFD8);
    D_003BAFD8 = 0;
    D_003BAFDC = 0;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014B540);


void func_0014B5F8(void) {
    if (D_003BAFD0 != 0 && D_003BAFD4 != 0) {
        func_0014FB00(D_003BAFD0);
    }
    if (D_003BAFE0 != 0 && D_003BAFE4 != 0) {
        func_0014FB00(D_003BAFE0);
    }
}


void *func_0014B648(s32 arg0, s32 arg1) {
    FldEnt110 *temp_v0 = D_003BAA48;
    s32 temp_v1 = 0;

    while (temp_v1 < 8) {
        if (temp_v0->unk0 == arg0) {
            if (temp_v0->unk2 == arg1) {
                return temp_v0;
            }
        }
        temp_v1++;
        temp_v0++;
    }
    return NULL;
}

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A08E8);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A08F8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014B688);


void func_0014B858(void) {
    FldEnt14 *entry = D_003D62E0;
    s32 i = 0x10;

    do {
        s32 temp = entry->unk0;

        i--;
        if (temp != 0) {
            func_001130C8(temp);
            entry->unk0 = 0;
        }
        entry++;
    } while (i >= 0);
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014B8B0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014B988);


INCLUDE_ASM(const s32, "game/code_001411F0", func_0014BA10);
INCLUDE_ASM(const s32, "game/code_001411F0", func_0014BA50);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014BC28);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014BD58);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014BDF8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014C158);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014C210);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014C468);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014C5C8);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0918);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0928);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0978);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014C648);


s32 func_0014CAF8(void) {
    return D_003D62C8[0];
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014CB08);


void func_0014CFC0(void) {
    func_0012E9E8(0);
    D_0032E5C4[0] = 0;
    D_0032E3B0[0x46] = 0;
    func_0014B858();
    func_0014B4D0();
    func_00123E00();
    D_0032E3B0[0x45] = 1;
    *(s16 *)((u8 *)D_0032E3B0 + 0x104) = 0;
    D_003D62A8[0] = 0;
    D_0032E3B0[0x4E] = 1;
    func_00228740();
}


void func_0014D028(void) {
    func_0024D9D8(D_0034D8F0);
    func_0024DA58(5);
    func_00123EA8();
    D_0032E4C4[0] = 2;
}

void func_0014D068(void) {
    if (D_0032E3B0[0x45] == 2) {
        func_0024DD78();
        if (!func_0024DC08()) {
            func_0024DBB0();
            func_0024DBC8();
            func_00123E00();
            D_0032E3B0[0x45] = 0;
        }
    }
}


INCLUDE_ASM(const s32, "game/code_001411F0", func_0014D0D0);

void func_0014D0E8(void) {
    func_0014B5F8();
}


s32 func_0014D100(void) {
    return *(s16 *)((u8 *)D_003D62A4[0] + 0xC);
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014D110);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014D1E8);


INCLUDE_ASM(const s32, "game/code_001411F0", func_0014D2C0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014D320);


s32 func_0014D3C0(void) {
    s32 temp_v0;

    func_00123EA8();
    temp_v0 = func_00110A38(func_0010FD80());
    if (temp_v0 == 0) {
        return 1;
    }
    func_001313E0();
    return 1;
}


s32 func_0014D400(void) {
    s32 temp_v0;

    temp_v0 = func_00110A38(func_0010FD80());
    if (temp_v0 == 0) {
        return 1;
    }
    func_001127C0(temp_v0, 0);
    func_001313E0();
    func_00123E00();
    return 1;
}

u32 func_0014D458(void) {
    func_00125DD0(0x10);
    return 1;
}

u32 func_0014D478(void) {
    func_00125DE0(0x10);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014D498);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014D518);


s32 func_0014D6C0(void) {
    D_0032E400[0] = 3;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014D6D8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014D8B8);


s32 func_0014D9E0(void) {
    char *temp_v0;

    temp_v0 = func_0010D5A8(0);
    D_0032E3B0[0x14] = 5;
    strcpy((char *)D_0032E3B0 + 0x40, temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014DA20);

void func_0014DAF0(void) {
}

u32 func_0014DAF8(void) {
    return 1;
}

u32 func_0014DB00(void) {
    func_00127AC0();
    return 1;
}

s32 func_0014DB20(const char *name) {
    u32 *entry = func_00110F80(func_0010FDC0(), name);
    if (entry != 0) {
        return entry[1];
    }
    func_003003F0("field SEARCH_ID NotFound:[%s]\n", name);
    return -1;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014DB78);


s32 func_0014DBF8(void) {
    s32 temp_v0;

    temp_v0 = func_0010D428(0);
    D_0032E408[0] = temp_v0;
    return 1;
}


s32 func_0014DC20(void) {
    s32 temp_v0;

    D_0032E570[11] = func_0010D428(0);
    temp_v0 = func_0010D428(1);
    func_00132FD0(D_0032E570[11], temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014DC68);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014DCE0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014DEB8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014E0B0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014E270);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014E410);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014E4D0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014E5B0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014E638);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014E6C0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014E748);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014E7D0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014E870);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014E8F8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014E980);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014EA78);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014EB70);


INCLUDE_ASM(const s32, "game/code_001411F0", func_0014EC40);


INCLUDE_ASM(const s32, "game/code_001411F0", func_0014EC78);


INCLUDE_ASM(const s32, "game/code_001411F0", func_0014ECB0);


INCLUDE_ASM(const s32, "game/code_001411F0", func_0014ECE8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014ED20);

u32 func_0014EE20(void) {
    s32 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D6A0();
    temp_v1 = func_0013BE90(*(u32 *)(temp_v0 + 0xe4));
    func_00121750(temp_v1);
    return 1;
}


u32 func_0014EE50(void) {
    if (func_001217F0(func_0013BE90(*(s32 *)(func_0010D6A0() + 0xE4))) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_0014EEA0(void) {
    s32 scene;

    if (func_0013DF18()) {
        func_0013DF60(0);
        return 1;
    }
    scene = func_0013BEE8(*(s32 *)(func_0010D6A0() + 0xE4));
    if (scene) {
        func_0013DF60(scene);
    }
    return 1;
}

u32 func_0014EEF8(void) {
    s32 temp_v0;

    temp_v0 = func_0010D428(0);
    func_0013E5A8(temp_v0);
    return 1;
}

u32 func_0014EF20(void) {
    return 1;
}

u32 func_0014EF28(void) {
    s32 temp_v0;

    temp_v0 = func_0010D428(0);
    temp_v0 = func_0013F4D8(temp_v0);
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_0014EF58(void) {
    s32 temp_v0;

    temp_v0 = func_0010D428(0);
    temp_v0 = func_0013F5A8(temp_v0);
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_0014EF88(void) {
    s32 temp_v0;

    temp_v0 = func_0010D428(0);
    temp_v0 = func_0013F6B8(temp_v0);
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_0014EFB8(void) {
    s32 temp_v0;

    temp_v0 = func_0010D428(0);
    temp_v0 = func_0013F848(temp_v0);
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_0014EFE8(void) {
    s32 temp_v0;

    temp_v0 = func_0010D428(0);
    func_0013FA40(temp_v0);
    return 1;
}

u32 func_0014F010(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_0011E810(temp_v0, temp_v1);
    return 1;
}

u32 func_0014F050(void) {
    func_0011E960();
    return 1;
}

u32 func_0014F070(void) {
    u64 temp_v0;

    temp_v0 = func_0011E998();
    func_0010D5F0(temp_v0);
    return 1;
}
