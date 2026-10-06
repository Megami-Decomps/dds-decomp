#include "common.h"
#include "mnu.h"

extern MovieMenuState *mnuMovieMenuState;

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D270);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D480);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D510);

void mnuTitleResetSequenceTimers(void) {
    mnuMovieMenuState->unk34 = 1;
    mnuMovieMenuState->cursor = 0;
    mnuMovieMenuState->unk1C = 0;
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D660);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D808);


