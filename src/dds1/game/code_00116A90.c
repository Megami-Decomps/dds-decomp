#include "common.h"

extern u64 dds3GetWorldSecondaryObject(void);
extern s32 func_00110A48(u64, u64, u64);

INCLUDE_ASM(const s32, "game/code_00116A90", func_00116A90);

void func_00116AD8(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x10) = arg1;
}

u32 func_00116AE8(u64 arg0) {
    s32 temp_v0;
    u64 temp_v1;

    temp_v1 = dds3GetWorldSecondaryObject();
    temp_v0 = func_00110A48(temp_v1, arg0, 6);
    return *(u32 *)(*(s32 *)(temp_v0 + 0x18) + 0x10);
}

INCLUDE_ASM(const s32, "game/code_00116A90", func_00116B28);

u32 func_00116B78(s32 arg0) {
    return *(u32 *)(arg0 + 0x18);
}

INCLUDE_ASM(const s32, "game/code_00116A90", func_00116B80);

INCLUDE_ASM(const s32, "game/code_00116A90", func_00116CE8);

INCLUDE_ASM(const s32, "game/code_00116A90", func_00116D38);
