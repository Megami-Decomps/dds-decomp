#include "common.h"

extern u32 D_00436418;

extern u32 D_0043641C;

extern u32 D_00436420;

extern u32 D_00436424;

extern u32 D_00436414;

void func_00169418(s32 arg0, u32 arg1, s32 arg2, s32 arg3) {
    D_00436420 = (u32)arg0;
    D_00436418 = D_00436420;
    if (arg2 != 0) {
        D_00436420 = (u32)arg2;
    }
    if (arg3 != 0) {
        arg0 = arg3;
    }
    D_0043641C = arg1;
    D_00436424 = (s32)arg0;
}

u32 func_00169438(void) {
    return D_00436418;
}

u32 func_00169440(void) {
    return D_0043641C;
}

u32 func_00169448(void) {
    return D_00436420;
}

u32 func_00169450(void) {
    return D_00436424;
}

INCLUDE_ASM(const s32, "game/code_00169418", func_00169458);

INCLUDE_ASM(const s32, "game/code_00169418", effBTLFieldColorGetBaseColor);

INCLUDE_ASM(const s32, "game/code_00169418", func_00169580);

INCLUDE_ASM(const s32, "game/code_00169418", func_001695A8);

u32 func_001695C8(void) {
    return 1;
}

void func_001695D0(u32 arg0) {
    D_00436414 = D_00436414 | arg0;
}

INCLUDE_ASM(const s32, "game/code_00169418", func_001695E0);

INCLUDE_ASM(const s32, "game/code_00169418", func_001695F8);

INCLUDE_ASM(const s32, "game/code_00169418", func_00169608);

INCLUDE_ASM(const s32, "game/code_00169418", func_00169610);

INCLUDE_ASM(const s32, "game/code_00169418", func_001696B8);
