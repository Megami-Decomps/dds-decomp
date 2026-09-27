#include "common.h"

extern u64 func_00101A70(void);

extern s32 func_0022F408(void);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022CBA0);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022CC40);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022CD30);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022CE68);

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

    temp_v0 = func_0022F408();
    if (temp_v0 != 0) {
        *(s32 *)(arg0 + 0x23c0) = *(s32 *)(arg0 + 0x23c0) + 1;
    }
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F2E0);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F408);

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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_002300B8);

void func_00230128(s32 arg0) {
    *(u8 *)(arg0 + 0x23c5) = 1;
}

void func_00230138(s32 arg0) {
    *(u8 *)(arg0 + 0x23c5) = 0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00230140);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00230438);

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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00231C18);

u32 func_00231CC0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00231CC8);

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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_002323E8);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003ADA98);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232438);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_002326F8);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232720);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_002329A0);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232A00);

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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232E00);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232E20);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003ADB20);
