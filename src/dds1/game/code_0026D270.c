#include "common.h"
#include "mnu_movie_menu_selection_index.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "mnu.h"
#include "mnu_list.h"

extern MovieMenuState *mnuMovieMenuState;
extern s8 dds3PadButtonState[];
extern void mnuClearListFlagsOneAndTwo(u32 *);
extern MnuMovieMenuSelectionIndex mnuGetMovieMenuSelectionIndex(void);

extern void mnuDrawSprite(s32 x, s32 y, s32 depth, s32 alpha, s32 drawMode, s32 spriteIndex,
                          s32 drawContext);

void mnuDrawMovieMenuBackdrop(void) {
    f32 factor;
    f32 alpha;

    switch (mnuMovieMenuState->transitionMode) {
    case MNU_MOVIE_PHASE_OPENING_ANIMATION:
        if (mnuMovieMenuState->cursor < 10) {
            factor = mnuMovieMenuState->cursor / 10.0f;
            alpha = sdfSin(factor * 1.5707963f) * 128.0f;
        } else {
            factor = (30 - mnuMovieMenuState->cursor) / 20.0f;
            alpha = factor * 64.0f + 48.0f;
        }
        mnuDrawSprite(0, 0, 0, (s32)alpha, 0, 6, 0x53);
        if (mnuMovieMenuState->cursor < 5) {
            factor = 0.0f;
        } else if (mnuMovieMenuState->cursor < 10) {
            factor = (mnuMovieMenuState->cursor - 5) / 5.0f;
            factor = sdfSin(factor * 1.5707963f);
        } else {
            factor = (30 - mnuMovieMenuState->cursor) / 20.0f;
        }
        mnuDrawSprite(0, 0, 0, (s32)(factor * 128.0f), 0, 6, 0x53);
        return;
    case MNU_MOVIE_PHASE_FADE_IN:
    case MNU_MOVIE_PHASE_SELECTION:
        factor = mnuMovieMenuState->cursor / 90.0f;
        factor = sdfSin(factor * 6.2831852f - 1.5707963f);
        factor = (factor + 1.0f) * 0.5f;
        mnuDrawSprite(0, 0, 0, (s32)(factor * 64.0f + 48.0f), 0, 6, 0x53);
        break;
    }
}


/* Draw a list row's sprite pair: the base sprite, then the highlight on the cursor node. */
mnuDrawMovieMenuListItem(s32 x, s32 y, s32 unused, struct MenuList *list, struct MenuListNode *node, s32 priority) {
    s32 index = node->index;

    mnuDrawSprite(0, 0, 0, 0x80, 0, index + 0x10, priority);
    if (list->cursor == node) {
        mnuDrawSprite(0, 0, 0, 0x80, 0, index + 0x13, priority);
    }
}

s32 mnuHandleMovieMenuInput(void) {
    s32 moved = 0;
    MnuMovieMenuSelectionIndex selection;

    if (dds3PadButtonState[0x26] & 2) {
        if (mnuRetreatListCursorDefault(mnuMovieMenuState->selectionList) != NULL) {
            p2sdPlaySE(0, 127, 63);
        }
        moved = 1;
    } else if (dds3PadButtonState[0x27] & 2) {
        if (mnuAdvanceListCursorDefault(mnuMovieMenuState->selectionList) != NULL) {
            p2sdPlaySE(0, 127, 63);
        }
        moved = 1;
    }
    if ((dds3PadButtonState[0x26] == 0) & (dds3PadButtonState[0x27] == 0)) {
        mnuClearListFlagsOneAndTwo(&mnuMovieMenuState->selectionList->stateFlags);
    }
    if (dds3PadButtonState[0x21] < 0) {
        selection = mnuGetMovieMenuSelectionIndex();
        if (selection < 0) {
            return MNU_MOVIE_INPUT_CONFIRMED;
        }
        if (selection >= 2) {
            if (selection == 2) {
                p2sdPlaySE(2, 127, 63);
            }
        } else {
            p2sdPlaySE(0x310001, 127, 63);
        }
        return MNU_MOVIE_INPUT_CONFIRMED;
    }
    if (dds3PadButtonState[0x23] < 0) {
        p2sdPlaySE(10, 127, 63);
        return MNU_MOVIE_INPUT_CANCELLED;
    }
    return moved ? MNU_MOVIE_INPUT_MOVED : MNU_MOVIE_INPUT_IDLE;
}

void mnuTitleResetSequenceTimers(void) {
    mnuMovieMenuState->transitionMode = MNU_MOVIE_PHASE_FADE_IN;
    mnuMovieMenuState->cursor = 0;
    mnuMovieMenuState->idleTicks = 0;
}

/* Step the movie menu: fade in, wait, then poll the selection until it
 * confirms (phase 3, returns 1 when done) or cancels or idles out (phase 4,
 * returns 2). */
s32 mnuUpdateMovieMenuInputState(void) {
    s32 result;

    switch (mnuMovieMenuState->transitionMode) {
    case MNU_MOVIE_PHASE_FADE_IN:
        if (mnuMovieMenuState->cursor < 23) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->transitionMode = MNU_MOVIE_PHASE_SELECTION;
        }
        if (mnuMovieMenuState->cursor >= 4) {
            result = mnuHandleMovieMenuInput();
            switch (result) {
            case MNU_MOVIE_INPUT_IDLE:
            default:
                mnuMovieMenuState->idleTicks = 0;
                break;
            case MNU_MOVIE_INPUT_CANCELLED:
                mnuMovieMenuState->transitionMode = MNU_MOVIE_PHASE_CANCEL_EXIT;
                mnuMovieMenuState->cursor = 0;
                break;
            case MNU_MOVIE_INPUT_CONFIRMED:
                mnuMovieMenuState->transitionMode = MNU_MOVIE_PHASE_CONFIRM_EXIT;
                mnuMovieMenuState->cursor = 0;
                break;
            }
        }
        break;
    case MNU_MOVIE_PHASE_OPENING_ANIMATION:
        if (mnuMovieMenuState->cursor < 14) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->transitionMode = MNU_MOVIE_PHASE_SELECTION;
        }
        break;
    case MNU_MOVIE_PHASE_SELECTION:
        result = mnuHandleMovieMenuInput();
        switch (result) {
        case MNU_MOVIE_INPUT_CONFIRMED:
            mnuMovieMenuState->transitionMode = MNU_MOVIE_PHASE_CONFIRM_EXIT;
            mnuMovieMenuState->cursor = 0;
            break;
        case MNU_MOVIE_INPUT_CANCELLED:
            mnuMovieMenuState->transitionMode = MNU_MOVIE_PHASE_CANCEL_EXIT;
            mnuMovieMenuState->cursor = 0;
            break;
        case MNU_MOVIE_INPUT_IDLE:
            if (mnuMovieMenuState->idleTicks < 450) {
                mnuMovieMenuState->idleTicks++;
            } else {
                mnuMovieMenuState->transitionMode = MNU_MOVIE_PHASE_CANCEL_EXIT;
                mnuMovieMenuState->idleTicks = 0;
                mnuMovieMenuState->cursor = 0;
            }
            break;
        default:
            mnuMovieMenuState->idleTicks = 0;
            break;
        }
        break;
    case MNU_MOVIE_PHASE_CONFIRM_EXIT:
        if (mnuMovieMenuState->cursor < 10) {
            mnuMovieMenuState->cursor++;
            break;
        }
        return MNU_MOVIE_UPDATE_CONFIRMED;
    case MNU_MOVIE_PHASE_CANCEL_EXIT:
        if (mnuMovieMenuState->cursor < 10) {
            mnuMovieMenuState->cursor++;
            break;
        }
        return MNU_MOVIE_UPDATE_CANCELLED;
    }
    return MNU_MOVIE_UPDATE_RUNNING;
}


extern void mnuCallInitWide(s32, s32, s32, s32, s32);

void mnuDrawMovieChoiceTransition(void) {
    s32 i;
    s32 elapsed;
    s32 alpha;
    f32 factor;

    switch (mnuMovieMenuState->transitionMode) {
    case MNU_MOVIE_PHASE_FADE_IN:
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
            factor = sdfSin(factor * 1.5707963f);
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
    case MNU_MOVIE_PHASE_OPENING_ANIMATION:
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
    case MNU_MOVIE_PHASE_SELECTION:
        mnuCallInitWide(0, 0, 0, (s32)mnuMovieMenuState->selectionList, 0x53);
        return;
    case MNU_MOVIE_PHASE_CONFIRM_EXIT:
    case MNU_MOVIE_PHASE_CANCEL_EXIT:
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
