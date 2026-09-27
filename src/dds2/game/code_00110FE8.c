#include "common.h"

INCLUDE_ASM(const s32, "game/code_00110FE8", func_00110FE8);

INCLUDE_ASM(const s32, "game/code_00110FE8", func_00111050);

void func_001110C8(s32 arg0, u32 arg1) {
    if (arg0 != 0) {
        **(u32 **)((s32)arg0 + 0x18) = arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_00110FE8", func_001110E0);

INCLUDE_ASM(const s32, "game/code_00110FE8", func_001110F8);

INCLUDE_ASM(const s32, "game/code_00110FE8", func_001111A8);

INCLUDE_ASM(const s32, "game/code_00110FE8", func_00111218);

INCLUDE_ASM(const s32, "game/code_00110FE8", func_00111278);

u32 func_001112F8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00110FE8", func_00111300);
