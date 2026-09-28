#include "common.h"

extern s32 func_0023A170(u32);

INCLUDE_ASM(const s32, "game/code_002651C0", func_002651C0);

u32 func_002652D8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002651C0", func_002652E0);

INCLUDE_ASM(const s32, "game/code_002651C0", func_00265360);

INCLUDE_ASM(const s32, "game/code_002651C0", func_002653B8);

INCLUDE_ASM(const s32, "game/code_002651C0", func_00265408);

INCLUDE_ASM(const s32, "game/code_002651C0", func_00265500);

INCLUDE_ASM(const s32, "game/code_002651C0", func_002655C0);

INCLUDE_ASM(const s32, "game/code_002651C0", func_002657F8);

INCLUDE_ASM(const s32, "game/code_002651C0", func_00265850);

INCLUDE_ASM(const s32, "game/code_002651C0", func_00265898);

u32 func_00265980(void) {
    return 1;
}

u32 func_00265988(s32 arg0) {
    u32 temp_v0;
    s64 temp_v1;

    if (((*(s32 *)(arg0 + 8) == 2) && (temp_v1 = func_0023A170(4), temp_v1 != 0)) &&
          (temp_v1 = func_0023A170(0x290), temp_v1 == 0)) {
        func_0023A0F0(0x290);
        temp_v0 = 1;
    }
    else {
        temp_v0 = 0;
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002651C0", func_002659E0);

INCLUDE_ASM(const s32, "game/code_002651C0", func_00265A60);
