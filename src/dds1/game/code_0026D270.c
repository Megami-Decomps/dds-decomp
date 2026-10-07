#include "common.h"
#include "mnu.h"
#include "mnu_list.h"

extern MovieMenuState *mnuMovieMenuState;
extern s8 D_00324510[];
extern void mnuClearListFlagsOneAndTwo(u32 *);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern u32 func_0026BED0(void);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D270);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D480);

s32 func_0026D510(void) {
    s32 moved = 0;
    s32 selection;

    if (D_00324510[0x26] & 2) {
        if (mnuRetreatListCursorDefault(mnuMovieMenuState->selectionList) != NULL) {
            sndSetSequenceVolumePan(0, 127, 63);
        }
        moved = 1;
    } else if (D_00324510[0x27] & 2) {
        if (mnuAdvanceListCursorDefault(mnuMovieMenuState->selectionList) != NULL) {
            sndSetSequenceVolumePan(0, 127, 63);
        }
        moved = 1;
    }
    if ((D_00324510[0x26] == 0) & (D_00324510[0x27] == 0)) {
        mnuClearListFlagsOneAndTwo(&mnuMovieMenuState->selectionList->stateFlags);
    }
    if (D_00324510[0x21] < 0) {
        selection = func_0026BED0();
        if (selection < 0) {
            return 1;
        }
        if (selection >= 2) {
            if (selection == 2) {
                sndSetSequenceVolumePan(2, 127, 63);
            }
        } else {
            sndSetSequenceVolumePan(0x310001, 127, 63);
        }
        return 1;
    }
    if (D_00324510[0x23] < 0) {
        sndSetSequenceVolumePan(10, 127, 63);
        return 2;
    }
    return moved ? -1 : 0;
}

void mnuTitleResetSequenceTimers(void) {
    mnuMovieMenuState->unk34 = 1;
    mnuMovieMenuState->cursor = 0;
    mnuMovieMenuState->unk1C = 0;
}

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D660);

extern void mnuDrawSprite(s32, s32, s32, s32, s32, s32, s32);
extern f32 sdfSinPoly(f32);
extern void mnuCallInitWide(s32, s32, s32, s32, s32);

void func_0026D808(void) {
    s32 i;
    s32 elapsed;
    s32 alpha;
    f32 factor;

    switch (mnuMovieMenuState->unk34) {
    case 1:
        if (mnuMovieMenuState->cursor < 3) {
            factor = 1.0f;
        } else if (mnuMovieMenuState->cursor < 20) {
            factor = (mnuMovieMenuState->cursor - 3) / 17.0f;
            factor = 1.0f - factor;
        } else {
            factor = 0.0f;
        }
        mnuDrawSprite(0, 0, 0, (s32)(factor * 128.0f), 0, 6, 0x53);

        if (mnuMovieMenuState->cursor < 5) {
            factor = mnuMovieMenuState->cursor / 5.0f;
            factor = sdfSinPoly(factor * 1.5707963f);
        } else if (mnuMovieMenuState->cursor < 20) {
            factor = (mnuMovieMenuState->cursor - 5) / 15.0f;
            factor = 1.0f - factor;
        } else {
            factor = 0.0f;
        }
        mnuDrawSprite(0, 0, 0, (s32)(factor * 128.0f), 0, 6, 0x53);

        for (i = 0; i < 3; i++) {
            elapsed = mnuMovieMenuState->cursor - i * 5 - 3;
            if (elapsed < 0) {
                elapsed = 0;
            }
            if (elapsed < 10) {
                factor = elapsed / 10.0f;
            } else {
                factor = 1.0f;
            }
            alpha = (s32)(factor * 128.0f);
            mnuDrawSprite(0, 0, 0, alpha, 0, i + 16, 0x53);
            if (i == mnuMovieMenuState->selectionList->cursor->index) {
                mnuDrawSprite(0, 0, 0, alpha, 0, i + 19, 0x53);
            }
        }
        break;
    case 0:
        for (i = 0; i < 3; i++) {
            elapsed = mnuMovieMenuState->cursor - i * 2;
            if (elapsed < 0) {
                elapsed = 0;
            }
            if (elapsed < 10) {
                factor = elapsed / 10.0f;
            } else {
                factor = 1.0f;
            }
            alpha = (s32)(factor * 128.0f);
            mnuDrawSprite(0, 0, 0, alpha, 0, i + 16, 0x53);
            if (i == mnuMovieMenuState->selectionList->cursor->index) {
                mnuDrawSprite(0, 0, 0, alpha, 0, i + 19, 0x53);
            }
        }
        break;
    case 2:
        mnuCallInitWide(0, 0, 0, (s32)mnuMovieMenuState->selectionList, 0x53);
        return;
    case 3:
    case 4:
        factor = (10 - mnuMovieMenuState->cursor) / 10.0f;
        if (factor < 0.0f) {
            factor = 0.0f;
        }
        alpha = (s32)(factor * 128.0f);
        for (i = 0; i < 3; i++) {
            mnuDrawSprite(0, 0, 0, alpha, 0, i + 16, 0x53);
            if (i == mnuMovieMenuState->selectionList->cursor->index) {
                mnuDrawSprite(0, 0, 0, alpha, 0, i + 19, 0x53);
            }
        }
        break;
    }
}


