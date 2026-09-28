#include "common.h"

extern u8 D_0043798A;

extern s32 kwlnFadeIsActive(void);

extern s32 func_002C6CE8(void);

extern void func_00297240(s32, u32);

extern void func_00297200(s32, u32);

extern void func_002971C0(s32, u32);

extern s8 D_0043798B;

extern s8 D_00437989;

extern s8 D_00437988;

INCLUDE_ASM(const s32, "game/code_00296E98", func_00296E98);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00297000);

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

INCLUDE_ASM(const s32, "game/code_00296E98", func_00297250);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00297320);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00297898);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00297970);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00298570);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00298648);

void func_00298E20(void) {
    func_00299748();
    D_0043798A = 0;
    D_0043798B = 1;
}

s8 func_00298E48(void) {
    return D_0043798B;
}

u32 func_00298E50(void) {
    D_0043798A = 1;
    return 1;
}

void func_00298E60(void) {
    func_00299868();
}

s8 func_00298E78(void) {
    return D_00437989;
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00298E80);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00298EA8);

void func_00298F08(s32 arg0) {
    func_0011A0D0(*(u32 *)(arg0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00298F20);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299018);

void func_00299180(u32 arg0, u32 arg1, u32 arg2) {
    func_00298EA8(arg1);
    func_00298F08(arg1);
    func_00299018(arg0, arg2);
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_002991D0);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299280);

INCLUDE_RODATA(const s32, "game/code_00296E98", D_00428358);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299320);

INCLUDE_ASM(const s32, "game/code_00296E98", func_002993D0);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299488);

void func_00299518(u32 arg0, u32 arg1, u32 arg2) {
    func_00299488(arg0, arg1, 2);
    func_00299488(arg0, arg2, 1);
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299558);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299578);

INCLUDE_ASM(const s32, "game/code_00296E98", func_002996B8);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299748);

INCLUDE_ASM(const s32, "game/code_00296E98", func_002997F8);

s32 func_00299868(void) {
    s32 state = D_00437988;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_00437988 = 0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_002998A0);

INCLUDE_ASM(const s32, "game/code_00296E98", func_002998D8);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299988);

s32 func_00299A00(void) {
    if (kwlnFadeIsActive() != 0) {
        return 0;
    }
    return func_002C6CE8() != 1;
}

void func_00299A38(u32 arg0, s32 arg1) {
    initPartyPanelSlots(arg1 + 0x584);
    func_002BCA98(arg1 + 0x690);
    func_002BCAB0(arg1 + 0x690);
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299A70);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299B20);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299B98);

INCLUDE_RODATA(const s32, "game/code_00296E98", D_00428388);

INCLUDE_RODATA(const s32, "game/code_00296E98", D_00428398);

INCLUDE_RODATA(const s32, "game/code_00296E98", D_004283B0);

INCLUDE_RODATA(const s32, "game/code_00296E98", D_004283C0);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_00437988);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_00437989);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_0043798A);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_0043798B);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_00437990);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_00437998);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_004379A0);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_004379A8);

