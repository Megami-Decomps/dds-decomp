#include "common.h"

extern u64 dds3GetWorldSecondaryObject(void);

extern s32 func_00110C70(u64, u64, u64);

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116CF8);

void func_00116D40(s32 object, u32 value) {
    *(u32 *)(*(s32 *)(object + 0x18) + 0x10) = value;
}

u32 func_00116D50(u64 object) {
    s32 found;
    u64 world;

    world = dds3GetWorldSecondaryObject();
    found = func_00110C70(world, object, 6);
    return *(u32 *)(*(s32 *)(found + 0x18) + 0x10);
}

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116D90);

u32 func_00116DE0(s32 object) {
    return *(u32 *)(object + 0x18);
}

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116DE8);

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116F50);

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116FA0);
