#include "common.h"

extern u32 D_003BDA34;

extern u32 D_003BD378;

extern u32 D_003BD374;

extern u32 D_003BDA2C;

extern u32 D_003BDA28;

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DDC98);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DDCF0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DDD38);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DDD60);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DDD88);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DDDA8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DDDD0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DDDF0);

void func_002DDE80(void) {
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DDE88);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DDF58);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DDFB0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE010);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE0D8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE118);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE1A0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE268);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE2C0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE320);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE350);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE398);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE408);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE450);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE488);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE558);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE600);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE700);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE7D8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE868);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE8C8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE918);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE980);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DEA18);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DEAC0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DEB40);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DF128);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DF710);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DFC80);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DFEC8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E0150);

void func_002E02B0(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x44) & 0x10) != 0) {
        func_002DFC80();
        return;
    }
    func_002E0150();
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E02D8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E03C0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E04E0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E0540);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E0618);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E06F0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E0820);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1168);

void func_002E11E0(u32 arg0) {
    D_003BDA28 = arg0;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E11E8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1210);

void func_002E1218(void) {
    D_003BDA28 = 0;
    D_003BDA2C = 0;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1228);

u32 func_002E12C0(void) {
    return 0x50;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E12C8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1370);

u32 func_002E13E0(u32 arg0, s32 arg1) {
    func_002D45B0(arg0, (arg1 >> 4) - 2);
    return arg0;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1410);

s32 func_002E1420(s32 arg0) {
    return arg0 + 0x20;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1428);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1478);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E14D8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E15D0);

void func_002E1708(u32 arg0) {
    D_003BD374 = arg0;
}

void func_002E1710(u32 arg0) {
    D_003BD378 = arg0;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1718);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1938);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1BF0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1CC8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1D08);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1D60);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1EB8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1F78);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1FF8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2088);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2110);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E21A0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2600);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2680);

s32 func_002E27C8(s32 arg0) {
    return arg0 * 0x40 + 0x40;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E27D8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2A00);

u32 func_002E2B90(s32 arg0) {
    return (arg0 * 0x54 + 0x4bU) & 0xfffffff0;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2BB8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2DE8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2F40);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2F68);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E31A0);

u32 func_002E3368(s32 arg0) {
    return (arg0 * 0x6c + 0x4bU) & 0xfffffff0;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3390);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E35C8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3970);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E39C8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3B40);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3B60);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3B80);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3BA8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3BD8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3CE0);

u32 func_002E3D18(void) {
    func_002E3CE0();
    return D_003BDA34;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3D38);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3D48);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3D68);

void func_002E3D98(u32 arg0) {
    func_002E3D68();
    func_002D0918(*(u32 *)((s32)arg0 + 0x18));
    func_002CFF98(arg0);
}

void func_002E3DC8(s32 arg0) {
    *(u16 *)(arg0 + 0x10) = 0;
    *(u16 *)(arg0 + 0x12) = 0;
    memset(*(u32 *)(arg0 + 0x1c), 0,
                  (s32)*(s16 *)(arg0 + 0xc) * (s32)*(s16 *)(arg0 + 0xe) * 2);
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3E00);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3E18);

INCLUDE_RODATA(const s32, "game/code_002DDC98", D_003B4548);

