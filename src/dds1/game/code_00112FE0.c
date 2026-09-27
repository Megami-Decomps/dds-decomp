#include "common.h"

u32 func_00112FE0(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x84);
}

void func_00112FF0(s32 arg0, s32 arg1) {
    if (*(s32 *)(*(s32 *)(arg0 + 0x18) + 0x84) != arg1) {
        *(s32 *)(*(s32 *)(arg0 + 0x18) + 0x84) = arg1;
    }
}

u32 func_00113008(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 0xc);
}

INCLUDE_ASM(const s32, "game/code_00112FE0", func_00113018);
