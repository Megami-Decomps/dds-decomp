#include "common.h"

INCLUDE_ASM(const s32, "game/code_00110DC0", func_00110DC0);

INCLUDE_ASM(const s32, "game/code_00110DC0", func_00110E28);

void func_00110EA0(s32 object, u32 value) {
    if (object != 0) {
        **(u32 **)((s32)object + 0x18) = value;
    }
}

INCLUDE_ASM(const s32, "game/code_00110DC0", func_00110EB8);

INCLUDE_ASM(const s32, "game/code_00110DC0", func_00110ED0);

INCLUDE_ASM(const s32, "game/code_00110DC0", func_00110F80);

INCLUDE_ASM(const s32, "game/code_00110DC0", func_00110FF0);

INCLUDE_ASM(const s32, "game/code_00110DC0", func_00111050);

u32 func_001110D0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00110DC0", func_001110D8);
