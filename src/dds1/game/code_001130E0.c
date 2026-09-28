#include "common.h"

extern u32 D_003BA9D0;

extern u64 func_0010FDC0(void);

extern s32 func_00110A48(u64, u64, u64);

u32 func_001130E0(s32 arg0) {
    return **(u32 **)(arg0 + 0x18);
}

u32 func_001130F0(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 8);
}

void func_00113100(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113110);

INCLUDE_ASM(const s32, "game/code_001130E0", func_001131E0);

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113338);

void func_00113438(s32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    func_00111908(arg0, 0x2000);
    *(u32 *)(temp_v0 + 0x1c) = arg1;
    *(u32 *)(temp_v0 + 0x20) = 0;
}

void func_00113478(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    *(u32 *)(temp_v0 + 0x20) = 0x1e;
    *(u32 *)(temp_v0 + 0x1c) = 0;
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113490);

void func_00113538(u32 arg0) {
    s32 *piVar1;

    func_00113AA8();
    func_0010F5E8(arg0);
    piVar1 = *(s32 **)((s32)arg0 + 0x18);
    if (*piVar1 != -1) {
        *piVar1 = -1;
    }
    if (piVar1[2] != 0) {
        func_00222200(piVar1[2]);
        piVar1[2] = 0;
    }
    func_00111B40(arg0);
    func_00111840(piVar1[3]);
    func_002CFF98(*(u32 *)((s32)arg0 + 0x18));
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_001135B0);

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113888);

void func_00113AA8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    if (*(s32 *)(temp_v0 + 0x10) != -1) {
        func_001166F0(arg0, 10);
        *(u32 *)(temp_v0 + 0x10) = 0xffffffff;
    }
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113AF0);

u32 func_00113CD8(s32 arg0) {
    return **(u32 **)(arg0 + 0x18);
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113CE8);

void func_00113DA8(void) {
    func_00110928();
}

void func_00113DC0(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0xc) = arg1;
}

void func_00113DD0(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 4) = arg1;
}

u32 func_00113DE0(u64 arg0) {
    s32 temp_v0;
    u64 temp_v1;

    temp_v1 = func_0010FDC0();
    temp_v0 = func_00110A48(temp_v1, arg0, 6);
    return *(u32 *)(*(s32 *)(temp_v0 + 0x18) + 4);
}

void func_00113E20(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 8) = arg1;
}

u32 func_00113E30(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 8);
}

void func_00113E40(u32 arg0) {
    D_003BA9D0 = arg0;
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113E48);

void func_00113EE8(s32 arg0) {
    u32 *puVar1;

    func_0010F5E8();
    puVar1 = *(u32 **)(arg0 + 0x18);
    func_00111840(*puVar1);
    func_002CFF98(puVar1);
}

INCLUDE_RODATA(const s32, "game/code_001130E0", D_0039F720);

INCLUDE_RODATA(const s32, "game/code_001130E0", D_0039F730);

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113F28);

INCLUDE_ASM(const s32, "game/code_001130E0", func_001141C0);

INCLUDE_ASM(const s32, "game/code_001130E0", func_001143B0);

INCLUDE_ASM(const s32, "game/code_001130E0", func_001143D8);

INCLUDE_ASM(const s32, "game/code_001130E0", func_00114508);
