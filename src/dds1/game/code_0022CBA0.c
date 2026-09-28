#include "common.h"

extern void *func_00101A70(void);

extern s32 D_003BAA00;
extern char D_003ADB20[]; /* "EventViewer" */
extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);
s32 func_0022F408(s32 arg0);
void func_00232720(void);
void func_00134C68(void);
void func_00101A80(s32 arg0, s32 arg1);
s32 func_00235560(void);
void func_0022C408(u64 arg0);
void *func_002329A0(s32 arg0);
extern u32 D_003BA904;
s32 func_0022BE40(s32 arg0);
void func_0022E5A0(s32 arg0, void *arg1);
void func_0022FF30(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_0010FD80(void);
void func_001109B8(s32 arg0, u32 arg1);
f32 func_00112ED8(s32 arg0);
s32 func_00106488(f32 arg0);
u16 func_0022FF98(s32 arg0);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022CBA0);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022CC40);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022CD30);

void func_0022CE68(s32 arg0) {
    s32 s;
    s32 v;

    v = *(s32 *)(arg0 + 0x2024);
    if (v != 0) {
        s = v;
    } else {
        s = *(s32 *)(arg0 + 0x202c);
    }
    if (s == 0) {
        return;
    }
    func_001109B8(func_0010FD80(), s);
    func_00106488(func_00112ED8(s));
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022CED0);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022D420);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022D528);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022E098);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022E288);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022E5A0);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022EA18);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022EB10);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022EE90);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022EF38);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F038);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F1C0);

void func_0022F2A8(s32 arg0) {
    s64 temp_v0;

    temp_v0 = func_0022F408(arg0);
    if (temp_v0 != 0) {
        *(s32 *)(arg0 + 0x23c0) = *(s32 *)(arg0 + 0x23c0) + 1;
    }
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F2E0);

s32 func_0022F408(s32 arg0) {
    return (*(s32 *)(arg0 + 4) & 0x10) > 0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F418);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F550);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F778);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F7F8);

void func_0022F9F0(void) {
}

void func_0022F9F8(s32 arg0) {
    s32 temp_v0;

    if ((*(s32 *)(arg0 + 0x18) < *(s32 *)(arg0 + 0x14) - 3) && (0 < *(s32 *)(arg0 + 0x23f0)))
    {
        func_00195868(*(u32 *)(arg0 + 0x2410));
        temp_v0 = *(s32 *)(arg0 + 0x23f0) + 1;
        *(s32 *)(arg0 + 0x23f0) = temp_v0;
        if (0x1d < temp_v0) {
            *(u32 *)(arg0 + 0x23f0) = 0;
        }
    }
}

void func_0022FA60(void) {
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022FA68);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022FB30);

void func_0022FDE8(u32 arg0) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 4) & 1) != 0) {
        func_0022FB30(0, *(u32 *)(temp_v0 + 0x18), arg0);
        return;
    }
    func_0022FB30(1, *(u32 *)(temp_v0 + 0x18), arg0);
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022FE30);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022FEB0);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022FF30);

u16 func_0022FF98(s32 arg0) {
    u16 temp_v0;
    s32 temp_v1;

    temp_v1 = *(s32 *)(arg0 + 0x227c) - 1;
    if (*(s32 *)(arg0 + 0x227c) == 0) {
        *(u32 *)(arg0 + 0x2280) = 0;
        return 0;
    }
    *(s32 *)(arg0 + 0x227c) = temp_v1;
    temp_v0 = *(u16 *)(temp_v1 * 8 + arg0 + 0x223c);
    *(u32 *)(arg0 + 0x2280) = (u32)temp_v0;
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022FFC8);

void func_002300B8(s32 arg0) {
    s32 v0;
    s32 v1;

    v0 = *(s32 *)(arg0 + 8);
    if (v0 == 0) {
        return;
    }
    v1 = *(s32 *)(v0 + 0x104);
    if (v1 == -1) {
        return;
    }
    func_0019BC98(v1, 1);
    v0 = *(s32 *)(arg0 + 8);
    func_0019B4A0(*(s32 *)(v0 + 0x104));
    v0 = *(s32 *)(arg0 + 8);
    func_0019B300(*(s32 *)(v0 + 0x104), 0);
    v0 = *(s32 *)(arg0 + 8);
    func_0019BFD8(*(s32 *)(v0 + 0x104));
    *(u8 *)(arg0 + 0x23c5) = 0;
    *(u8 *)(arg0 + 0x23c4) = 0;
}

void func_00230128(s32 arg0) {
    *(u8 *)(arg0 + 0x23c5) = 1;
}

void func_00230138(s32 arg0) {
    *(u8 *)(arg0 + 0x23c5) = 0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00230140);

s32 func_00230438(u32 arg0) {
    u32 idx;
    u32 lo;

    idx = (arg0 << 16) >> 28;
    lo = arg0 & 0xfff;
    if (idx == 0) {
        return 1;
    }
    return (*(s32 *)(D_003BAA00 + idx * 4 + 0x35c) ^ lo) == 0;
}

u32 func_00230470(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD2B0);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD2C0);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD2D0);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD2E0);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00230478);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00230660);

u32 func_00230A38(u32 arg0, u32 arg1, u32 arg2) {
    func_0022FF30(5, 0x90, 0x48, arg2);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD4B0);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD4C0);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD4D0);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD4E0);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD4F0);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD500);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD510);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD520);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD530);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD540);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD550);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD560);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD570);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00230A68);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00231840);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00231950);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00231BB0);

u32 func_00231C18(u32 arg0, u32 arg1, u32 arg2) {
    s32 p;
    u32 v;
    s32 idx;

    v = *(u32 *)(arg2 + 0x2310);
    p = func_0022BE40(arg2);
    if (p != 0) {
        idx = *(s32 *)(*(s32 *)(arg2 + 0x2308)) - 1;
        if ((u32)idx < 0x11u) {
            switch (idx) {
            case 3:
                *(u16 *)(p + 0xa) = v;
                break;
            case 1:
                *(u16 *)(p + 0xc) = v;
                break;
            case 0:
                *(u16 *)(p + 0x10) = v;
                break;
            case 11:
            case 15:
            case 16:
                *(u16 *)(p + 0x14) = v;
                break;
            }
        }
        func_0022E5A0(*(s32 *)(arg2 + 0x18), (void *)arg2);
        func_0022FF98(arg2);
        return 0;
    }
}

u32 func_00231CC0(void) {
    return 0;
}

u32 func_00231CC8(u32 arg0, u32 arg1, u32 arg2) {
    s32 p;

    p = func_0022BE40(arg2);
    if (p != 0) {
        *(s32 *)(p + 0xc) = *(s32 *)(arg2 + 0x2310);
        func_0022E5A0(*(s32 *)(arg2 + 0x18), (void *)arg2);
        func_0022FF98(arg2);
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00231D18);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00231E10);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00231E90);

u32 func_00231EF8(u32 arg0, u32 arg1, u32 arg2) {
    func_0022FF98(arg2);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00231F18);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00231FF0);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232048);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232108);

u32 func_002323E8(u32 arg0, u32 arg1, u32 arg2) {
    if (func_0022BE40(arg2) != 0) {
        *(s32 *)(arg2 + 0x22ac) = 0;
        *(s32 *)(arg2 + 0x22b4) = 0;
        func_0022FF30(0xa, 0x9c, 0x54, arg2);
        return 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003ADA98);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232438);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_002326F8);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232720);

void *func_002329A0(s32 arg0) {
    void *temp_v0;

    temp_v0 = func_00101A70();
    func_0022E5A0(*(s32 *)((u8 *)temp_v0 + 0x18), temp_v0);
    func_00101A80(arg0, func_00235560());
    D_003BA904 |= 0x2000000;
    return (void *)func_00232720;
}

void *func_00232A00(void) {
    u64 temp_v0;

    temp_v0 = func_00101A70();
    func_00134C68();
    func_0022C408(temp_v0);
    D_003BA904 |= 0x2000000;
    return (void *)func_002329A0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232A50);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232B30);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232BC0);

void func_00232D08(void) {
    u64 temp_v0;

    temp_v0 = func_00101A70();
    func_00232BC0(temp_v0);
}

void func_00232D28(void) {
    u64 temp_v0;

    temp_v0 = func_00101A70();
    func_00232BC0(temp_v0);
}

void func_00232D48(void) {
    func_00243A58();
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232D60);

void func_00232E00(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003ADB20, 1);
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232E20);







INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003ADB20);


INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBE78);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBE7A);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBE7C);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBE80);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBE88);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBE90);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBE94);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBE98);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEA0);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEA8);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEB0);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEB8);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEC0);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEC8);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBED0);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBED8);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEE0);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEE8);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEF0);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEF8);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF00);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF08);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF10);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF18);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF20);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF28);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF30);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF38);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF40);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF48);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF50);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF58);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF60);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF68);


INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF70);

