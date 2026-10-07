#include "common.h"
#include "mnu.h"
#include "mnu_list.h"

extern MovieMenuState *mnuMovieMenuState;
extern s8 D_00324510[];
extern struct MenuListNode *mnuRetreatListCursorDefault(u32);
extern struct MenuListNode *mnuAdvanceListCursorDefault(u32);
extern void mnuClearListFlagsOneAndTwo(u32 *);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern u32 func_0026BED0(void);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D270);

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D480);

s32 func_0026D510(void) {
    s32 moved = 0;
    s32 selection;

    if (D_00324510[0x26] & 2) {
        if (mnuRetreatListCursorDefault((u32)mnuMovieMenuState->selectionList) != NULL) {
            sndSetSequenceVolumePan(0, 127, 63);
        }
        moved = 1;
    } else if (D_00324510[0x27] & 2) {
        if (mnuAdvanceListCursorDefault((u32)mnuMovieMenuState->selectionList) != NULL) {
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

INCLUDE_ASM(const s32, "game/code_0026D270", func_0026D808);


