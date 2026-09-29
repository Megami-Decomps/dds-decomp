#include "common.h"

extern s32 D_003BC5D0;

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D270);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D480);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D510);

void mnuTitleResetSequenceTimers(void) {
    s32 *timers = (s32 *)D_003BC5D0;

    timers[13] = 1;
    timers[5] = 0;
    timers[7] = 0;
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D660);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D808);


