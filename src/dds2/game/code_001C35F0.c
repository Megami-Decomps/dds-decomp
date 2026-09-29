#include "common.h"

extern void func_001C35F0(s32, s32, s32);

extern u32 D_004367CC;

extern u32 func_00101958(s64);

extern s64 func_00101740(u32);

extern s32 func_001AA6F8(void);

extern s32 func_001AA6F8(void);

extern s32 func_001AA6F8(void);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C35F0);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C3750);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C3850);

void func_001C3978(u8 *object) {
    s32 count = 0;
    u8 slot = 0;
    u8 *node = *(u8 **)(func_001AA6F8() + 0x24C);
    u8 *entry;
    s32 offset;
    for (; node != 0; node = *(u8 **)(node + 0x364)) {
        if (func_001BB970(node) != 0) {
            slot = *(u8 *)(object + 0x11C);
            if (*(s64 *)(object + 0x108) == *(s64 *)(node + 0x108)) {
                break;
            }
            count++;
        }
    }
    if (count < 3) {
        entry = (u8 *)func_00101958(func_00101740(D_004367CC));
        offset = slot * 0x290 + 0x10;
        entry += offset;
        *(u8 *)(entry + 0x10) = 2;
        *(u8 *)(entry + 0x11) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C3A38);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C3BB0);

void func_001C3D20(u8 *context, s8 mode) {
    u8 *entry = context + 0x80;
    s32 modeZeroState = 3;
    s32 modeNonzeroState = 4;
    s32 i = 2;
    do {
        s32 state = entry[0xC];
        if (state == 1 || state == 2) {
            entry[0xC] = mode == 0 ? modeZeroState : modeNonzeroState;
        }
        i--;
        entry += 0x290;
    } while (i >= 0);
}

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C3D70);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C3DB0);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C3EC0);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C43F8);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416840);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416858);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416870);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C4520);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C4900);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C4C58);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C50A0);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416898);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C53A0);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C5610);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C5868);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C5D10);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C6010);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C6320);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C6648);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_004168C8);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_004168D8);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C68D0);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C6B98);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C7020);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C7760);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C7BA8);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C7D48);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C7DB8);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C7F10);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416920);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416938);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416948);
