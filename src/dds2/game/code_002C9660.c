#include "common.h"

extern u64 func_002D3D70(u64);

extern u64 func_003292A8(u64);

extern u64 func_003298F8(u64);

extern s64 func_0033E9F8(u64);

extern u64 func_0033EB30(s64);

extern s32 D_00437D84;

extern u32 D_00437CE8;

extern u32 D_00437CEC;

extern u32 D_00437CF0;

extern u32 D_00437CFC;

extern u32 D_00437D08;

extern u32 D_00437D14;

extern u32 D_00437D18;

extern u32 D_00437D1C;

extern u32 D_00437D20;

extern u32 D_00437D40;

extern u32 D_00437D44;

extern u32 D_00437D3C;

extern u64 func_0019F5E8(s32, s32, u64, u64, u64, u64);

extern u32 D_0043900C;

extern u32 func_0019CE78(u32, u32, u32, u32, u32);

extern s32 D_00437CD8;

extern s32 D_00437CE4;

extern s32 D_00437D38;

extern u32 D_00437D48;

extern u32 D_00437CD0;

extern u32 D_00437CF4;

extern u32 D_00437CF8;

extern u32 D_00437D34;

extern u32 D_00437D04;

extern s32 D_00435DD0;

extern u32 D_00437DEC;

extern s32 D_00439058;

extern u32 D_00437E08;

extern u32 func_002DDF48(u32);

extern s8 D_00437CD4;

extern s32 func_002CB130(void);

extern void func_002C8FD8(s32 arg0);

extern void *func_002CABC0(void);

extern void func_002CB2C0(void);

extern s32 func_002CB100(void);

extern s32 func_002C9140(s32 arg0);

extern u32 D_00439020;

extern void func_002CB678(void);

extern void *func_002CB710(u32 arg0);

extern void func_002CB740(void);

extern s32 D_00437D10;

extern void func_002C9710(void *arg0, s32 arg1);

extern void func_002C9328(u32 arg0, void *arg1);

extern void *func_002CB988(void);

extern void *func_002CBCA0(void);

extern void *func_002CC038(void);

extern void func_002CC0F8(void);

extern void func_001004A0(void);

extern void func_002C92D0(u32 arg0);

extern void func_002CC6E0(void);

extern char D_0042B720[];

typedef struct KwlnTask KwlnTask;

extern KwlnTask *func_00101740(const char *name);

/* Loader context at D_0037D4A0. */
typedef struct LoadCtx374A0 {
    u8 unk0[4]; /* 0x00 */
    u32 unk4;   /* 0x04 */
    u8 unk8[4]; /* 0x08 */
    u32 unkC;   /* 0x0C */
    u8 unk10;   /* 0x10 */
    u8 pad11[3]; /* 0x11 */
    u32 unk14;  /* 0x14 */
} LoadCtx374A0;

extern LoadCtx374A0 D_003E7FD8;

/* Far scalar: incomplete array forces non-small-data addressing. */
extern u32 D_003E8008[];

extern u32 D_003E9150[];

/* Callback table at D_0037E14C (0x28 bytes per entry). */
typedef struct Cb3714C {
    void (*cb)(void *arg); /* 0x00 */
    u8 pad4[0x24];         /* 0x04 */
} Cb3714C;

extern Cb3714C D_003E916C[];

extern void func_002D31C0(void *src);

extern void func_002D4818(void *dst, void *src);

void func_002C9660(void) {
    func_0034F490();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9678);

void func_002C96D0(void) {
    if (D_00437CD8 != 0) {
        kwlnTaskDestroyWithHierarchy(D_00437CD8, 1);
        D_00437CD8 = 0;
        D_00437CD4 = 0;
    }
}

s8 func_002C9700(void) {
    return D_00437CD4;
}

void func_002C9708(void) {
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9710);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9748);

u32 func_002C9788(void) {
    return 0x1e840;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9798);

u8 func_002C97D8(s32 arg0) {
    return arg0 != 0 && D_00437CE4 == 1;
}

u8 func_002C97F8(s32 arg0) {
    return arg0 != 0 && D_00437CE4 == 1;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9818);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9860);

void func_002C98B8(s32 arg0, s32 arg1, u32 arg2, u32 arg3) {
    func_0019D1D0(1);
    D_0043900C = func_0019CE78(arg3, 0, 0, 0, 0);
    func_0019D1E0(1);
    func_0019D058(D_0043900C, 1);
    func_0019D100(D_0043900C, arg0 << 4, arg1 << 3);
    func_0019D178(D_0043900C, arg2);
    func_0019D550(D_0043900C, 0, 0x56);
    func_0019C5B0(D_0043900C);
    func_0019D1F8(0x54);
}

void func_002C9970(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_0019F5E8(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_0019D530(temp_v0, 1);
    func_0019C5B0(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C99C0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9AF0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9BC0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9BD0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9CF8);

void func_002CA1D8(u32 arg0) {
    s32 temp_v0;

    temp_v0 = D_00437D38;
    D_00437D38 = arg0;
    if (temp_v0 == 0) {
        D_00437D48 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CA1F0);

void func_002CA638(void) {
    func_002C91B8(D_00437CD0);
    func_002C91E8(D_00437CD0, 0);
    func_002C91E8(D_00437CD0, 1);
    func_002C91E8(D_00437CD0, 2);
    func_002C91E8(D_00437CD0, 3);
    func_002C91E8(D_00437CD0, 4);
    func_002C91E8(D_00437CD0, 5);
    func_002C91E8(D_00437CD0, 6);
    func_002C91E8(D_00437CD0, 7);
    func_002C91E8(D_00437CD0, 8);
    func_002C91E8(D_00437CD0, 9);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CA6D0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CA7B0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CA828);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CA888);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CA9C0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAA08);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAA50);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAA90);

void *func_002CAAD0(void) {
    func_002CA1D8(0);
    D_00437CF8 = 0;
    return func_002CB130;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAAF8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAB40);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAB80);

void *func_002CABC0(void) {
    func_002CA638();
    func_002CA1D8(0);
    D_00437D3C = 1;
    D_00437D40 = 0;
    func_002C8FD8(D_00437CD0);
    return func_002CB2C0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAC00);

u32 func_002CAC80(void) {
    func_002CA1D8(0);
    D_00437CF8 = 0;
    D_00437CF4 = 0;
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CACA8);

void func_002CACF0(void) {
    func_002CACA8();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAD08);

void func_002CAD60(void) {
    func_002CA638();
    func_002CA1D8(1);
    D_00437D3C = 0;
    D_00437CF8 = 0;
    func_002CAC00();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAD90);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAE58);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAEA8);

void func_002CAED0(void) {
    func_002CAC00();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAEE8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAF10);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAF48);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB090);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB100);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB130);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB1F8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB2C0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB4D0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB5A0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB610);

void *func_002CB660(u32 arg0) {
    D_00439020 = arg0;
    D_00437D40 = 0;
    return func_002CB678;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB678);

void *func_002CB710(u32 arg0) {
    D_00439020 = arg0;
    D_00437D40 = 0;
    func_002C8FD8(D_00437CD0);
    return func_002CB740;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB740);

void *func_002CB948(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    func_002C9710(&buf[1], D_00437D10);
    func_002C9328(D_00437CD0, buf);
    return func_002CB988;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB988);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CBA10);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CBA90);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CBBC8);

void *func_002CBC60(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    func_002C9710(&buf[1], D_00437D10);
    func_002C9328(D_00437CD0, buf);
    return func_002CBCA0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CBCA0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CBD40);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CBDD0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CBE70);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CBEE0);

void *func_002CBF80(void) {
    u8 buf[0x50];
    u32 entry = D_00437CD0;
    s32 v = func_002C9280(entry);

    buf[0] = 0x2F;
    func_002C9710(&buf[1], v);
    func_002C9328(entry, buf);
    return func_002CC038;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CBFD0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC038);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC098);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC0F8);

void *func_002CC140(void) {
    func_002C96D0();
    return func_002CC0F8;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC168);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B6A8);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B6B8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC1E0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC210);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC488);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC4F0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC530);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC5D0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC638);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC6A0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC6E0);

void *func_002CC760(void) {
    func_002CA1D8(5);
    func_001004A0();
    func_002C92D0(D_00437CD0);
    return func_002CC6E0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC798);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC7F8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC8E0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC988);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC9F0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CCAA8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CCAD0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CD028);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CDF38);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CDFD8);

void func_002CE1A8(void) {
    func_002CA1D8(0);
    D_00437D04 = 0;
    D_00437CE8 = 0;
    D_00437D18 = 0xffffffff;
    D_00437D34 = 0;
    D_00437D3C = 0;
    D_00437D40 = 0;
    D_00437CF8 = 0;
    D_00437D08 = 0;
    D_00437CF0 = 0;
    D_00437CFC = 0;
    D_00437CEC = 0;
    D_00437D14 = 0;
    D_00437D1C = 0;
    D_00437D20 = 0;
    D_00437D44 = 0;
    D_00437D48 = 0;
    func_002CA638();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CE208);

void func_002CE738(void) {
    func_002CE758();
}

void func_002CE750(void) {
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CE758);

u32 func_002CE920(void) {
    return 1;
}

s32 func_002CE928(void) {
    return func_00101740(D_0042B720) != NULL;
}

u32 func_002CE950(void) {
    return D_00437D34;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CE958);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CEE70);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CEEC0);

u32 func_002CF928(void) {
    return D_00437D04;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CF930);

void func_002CF958(u32 arg0) {
    D_003E8008[0] = arg0;
    D_003E7FD8.unkC = 0x80;
    D_003E7FD8.unk4 = 0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CF978);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CF9D0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFA58);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFA98);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFC38);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFD48);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFD80);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B720);

INCLUDE_RODATA(const s32, "game/code_002C9660", jtbl_0042B730);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B770);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B7C0);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B868);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFF38);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0108);

void func_002D0148(void) {
    D_003E7FD8.unk14 = 0;
    D_003E7FD8.unk10 = 0;
    D_003E7FD8.unk4 = 0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0160);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D02D0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0340);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0358);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0470);

u32 func_002D0490(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B8F8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0498);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0678);

void func_002D06B0(void) {
    *(u8 *)(D_00437D84 + 0x31) = 0;
    func_002D0678();
}

void func_002D06D0(void) {
    *(u8 *)(D_00437D84 + 0x31) = 1;
    func_002D0678();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D06F0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0730);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0770);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0798);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D07D8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0810);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D08A0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D08F0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0920);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0968);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D09C8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0A20);

void func_002D0A90(void) {
    if (D_00437D84 != 0) {
        func_003298C0(*(u32 *)(D_00437D84 + 0x3c));
        D_00437D84 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0AB8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0AD8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0B08);

void func_002D0D00(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_00435DD0;
    *(u32 *)(D_00435DD0 + 0x20) = *(u32 *)(arg0 + 0x20);
    *(u32 *)(temp_v0 + 0x24) = *(u32 *)(arg0 + 0x24);
    *(u32 *)(temp_v0 + 0x28) = *(u32 *)(arg0 + 0x28);
    *(u32 *)(temp_v0 + 0x2c) = *(u32 *)(arg0 + 0x2c);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0D28);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0D90);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0E98);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0EB0);

void func_002D0EC8(void) {
    *(u32 *)(D_00435DD0 + 0xa54) = D_00437DEC;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0ED8);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B970);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B980);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B998);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B9B0);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B9D0);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B9F0);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BA10);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BA30);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BA50);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BA70);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BA90);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BAB0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0FB8);

void func_002D1038(u32 arg0) {
    func_002D0FB8(arg0, D_00435DD0 + 0xa54);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D1058);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D11F8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D1300);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D13B0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D13F0);

u32 func_002D1428(s32 arg0) {
    if (arg0 < 4) {
        return *(u32 *)((s32)arg0 * 4 + D_00439058 + 0x10);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BAF0);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BB00);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D1450);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D18B8);

u32 func_002D1910(void) {
    u32 temp_v0;

    temp_v0 = 0xffffffff;
    if ((*(u32 *)(D_00439058 + 0x28) & 0x80000000) == 0) {
        temp_v0 = 0;
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D1930);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D27A0);

void func_002D2C50(void) {
    func_002E6390();
    func_002F1C98();
    func_002F4570();
    func_002D2CA8();
}

void func_002D2C80(u32 arg0) {
    D_00437E08 = D_00437E08 | arg0;
}

void func_002D2C90(u32 arg0) {
    D_00437E08 = D_00437E08 & ~arg0;
}

void func_002D2CA8(void) {
    D_00437E08 = 0;
}

u32 func_002D2CB0(s32 arg0) {
    return D_003E9150[arg0];
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D2CC8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D2D48);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D2EB8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D2FB0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3128);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D31C0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3410);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3468);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3490);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D34B8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3528);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3590);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D35D0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3610);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3688);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D36D0);

void func_002D3710(void *arg0) {
    u16 idx = *(u16 *)((u8 *)arg0 + 4);
    void *data = *(void **)((u8 *)arg0 + 8);

    D_003E916C[idx].cb(data);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3748);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3788);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D37C8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3808);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3848);

void func_002D38E8(u64 arg0, u64 arg1, u16 arg2) {
    s64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;
    u64 temp_v3;

    temp_v0 = func_0033E9F8(arg1);
    if (temp_v0 != 0) {
        temp_v1 = func_0033EB30(temp_v0);
        temp_v2 = func_003292A8(temp_v1);
        temp_v3 = func_003298F8(temp_v2);
        func_0033EB10(temp_v0, temp_v3, temp_v1);
        func_0033EAE0(temp_v0);
        func_002D3848(arg0, temp_v3, temp_v1, arg2);
        func_003297C8(temp_v2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D39C8);

void func_002D3A68(u64 arg0, u64 arg1, u16 arg2) {
    s64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;
    u64 temp_v3;

    temp_v0 = func_0033E9F8(arg1);
    if (temp_v0 != 0) {
        temp_v1 = func_0033EB30(temp_v0);
        temp_v2 = func_003292A8(temp_v1);
        temp_v3 = func_003298F8(temp_v2);
        func_0033EB10(temp_v0, temp_v3, temp_v1);
        func_0033EAE0(temp_v0);
        func_002D39C8(arg0, temp_v3, temp_v1, arg2);
        func_003297C8(temp_v2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3B48);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3CB8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3D70);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3DF8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3E98);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3F08);

void func_002D3F80(u32 arg0) {
    s32 temp_v0;

    memset(arg0, 0, 0x90);
    temp_v0 = (s32)arg0;
    *(u32 *)(temp_v0 + 0x84) = 1;
    *(u8 *)(temp_v0 + 0x88) = 8;
    *(u8 *)(temp_v0 + 0x89) = 0;
    *(u8 *)(temp_v0 + 0x8a) = 0;
    func_002D3F08(arg0);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3FC8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4000);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4040);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4088);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D40D8);

void func_002D4120(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4138);

void func_002D4380(u32 arg0, u32 arg1) {
    func_002D4138(arg1);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4398);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4548);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D45B8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D46A0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D46F0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4818);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D48D0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D49B8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4A98);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4AB0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4AC8);

u32 func_002D4AD0(s32 arg0) {
    return *(u32 *)(arg0 + 100);
}

void func_002D4AD8(void *dst, void *src) {
    s128 vec;
    func_002D31C0(src);
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(&vec) : "memory");
    func_002D4818(dst, &vec);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4B10);

void func_002D4B60(u64 arg0, u64 arg1) {
    u64 temp_v0;

    temp_v0 = func_002D3D70(arg1);
    func_002D4B10(arg0, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4B90);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4BD8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4CF0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4E60);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4F10);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5010);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D50D8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D55B0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5AA8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5C30);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5C70);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5CB8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5D08);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5D50);

s32 func_002D5D90(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0;
    for (temp_v0 = *(s32 *)(arg0 + 0x8c); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0xac)) {
        temp_v1 = temp_v1 + 1;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5DC8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5EC0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5FB8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6020);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BB28);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6058);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6160);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6270);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D62D8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6538);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6598);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6610);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6690);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6710);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6808);

void func_002D6900(s32 arg0, u32 arg1) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x48) != 0) {
        func_002DE090(*(s32 *)(arg0 + 0x48));
    }
    temp_v0 = func_002DDF48(arg1);
    *(u32 *)(arg0 + 0x48) = temp_v0;
}

void func_002D6950(s32 arg0) {
    if (*(s32 *)(arg0 + 0x4c) != 0) {
        func_002DC040(*(s32 *)(arg0 + 0x4c));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6980);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D69B8);

void func_002D7398(u32 arg0) {
    func_002D6980();
    func_002D69B8(arg0);
}

void func_002D73C0(s32 arg0) {
    func_002DC0C0(*(u32 *)(arg0 + 0x4c));
}

void func_002D73D8(s32 arg0) {
    func_002DC0F0(*(u32 *)(arg0 + 0x4c));
}

void func_002D73F0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D73F8);

void func_002D7418(s32 arg0) {
    u32 temp_v0;
    s32 temp_v1;
    u32 temp_v2;

    temp_v0 = *(u32 *)(arg0 + 8);
    temp_v2 = 0;
    temp_v1 = *(s32 *)(arg0 + 0x18);
    if (temp_v0 != 0) {
        do {
            temp_v2 = temp_v2 + 1;
            *(u32 *)(temp_v1 + 0x10) = 0xffffffff;
            temp_v1 = temp_v1 + 0x20;
        } while (temp_v2 < temp_v0);
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D7458);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D7770);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D78E8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D7A58);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D7AC8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D7B58);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D81B0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D8A38);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D8AD0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D91A0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D9228);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D9A60);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D9AF8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DA2D8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DA358);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DAA58);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DAAE0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DB200);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DB288);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DB2C0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DB2F8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DB3E0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DBE78);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DBED8);

void func_002DC028(s32 arg0) {
    func_003297C8(*(u32 *)(arg0 + 0x28));
}

void func_002DC040(s32 arg0) {
    *(u32 *)(arg0 + 0x10) = 0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DC048);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DC0A8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DC0C0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DC0D8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DC0F0);
INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CD0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CD4);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CD5);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CD8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CE0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CE4);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CE8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CEC);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CF0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CF4);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CF8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CFC);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D00);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D04);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D08);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D0C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D10);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D14);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D18);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D1C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D20);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D24);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D28);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D2C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D30);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D34);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D38);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D3C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D40);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D44);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D48);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D4C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D50);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D54);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D58);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D5C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D60);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D64);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D68);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D6C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D70);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D74);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D78);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D7C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D80);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D84);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D88);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D8C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D90);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D94);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D98);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DA0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DA8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DB0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DB8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DC0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DD0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DD8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DE0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DE5);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DE8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DEC);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DF0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DF8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E00);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E08);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E10);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E18);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E20);


INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E28);

