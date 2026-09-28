#include "common.h"

extern s32 D_003BC5D0;

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D270);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D480);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D510);

void titleResetSequenceTimers(void) {
    s32 *temp_v0 = (s32 *)D_003BC5D0;

    temp_v0[13] = 1;
    temp_v0[5] = 0;
    temp_v0[7] = 0;
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D660);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D808);


