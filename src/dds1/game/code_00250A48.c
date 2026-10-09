#include "common.h"
#include "mnu_scene_work.h"
#include "mnu_mantra_grid.h"

typedef s16 MnuVariantSpritePlacement[6];
extern MnuVariantSpritePlacement D_0036B7F0[];
extern s8 D_00324510[64];
extern s32 func_002CBE18(SdfGrid *grid);
extern s32 func_002CBF60(SdfGrid *grid);
extern s32 func_00252F88(SdfGrid *grid);
extern s32 func_00253018(SdfGrid *grid);
extern void sndSetSequenceVolumePan(s32 trackId, s32 volume, s32 pan);

extern u32 *mnuGetSelectedNodeValue(void);
extern void mnuStopResourceAnimation(void);
extern void mnuResetResourceAnimation(void);
extern void mnuReinitializeSceneGrid(MenuSceneWork *work);

void mnuBeginTransition(MenuSceneWork *work, s32 mode) {
    MnuSceneListNode *transition = mnuAppendDisplayListNode(&work->sceneTransitionList);

    if (transition != NULL) {
        if (work->transitionBlendCounter >= 6) {
            mnuStopResourceAnimation();
            mnuReinitializeSceneGrid(work);
        } else if (work->transitionBlendCounter < -5) {
            mnuResetResourceAnimation();
            mnuReinitializeSceneGrid(work);
        }
        transition->payload.transition.mode = mode;
        if (mode == 1) {
            work->transitionBlendCounter = 10;
            transition->payload.transition.fromValue = *mnuGetSelectedNodeValue();
            mnuStopResourceAnimation();
            transition->payload.transition.toValue = *mnuGetSelectedNodeValue();
            mnuResetResourceAnimation();
            return;
        }
        if (mode == 2) {
            work->transitionBlendCounter = -10;
            transition->payload.transition.fromValue = *mnuGetSelectedNodeValue();
            mnuResetResourceAnimation();
            transition->payload.transition.toValue = *mnuGetSelectedNodeValue();
            mnuStopResourceAnimation();
        }
    }
}

void func_00250B60(MenuSceneWork *work) {
    s32 feedback = 0;
    s8 direction;
    MnuMantraGridEntry *entry;

    if (D_00324510[0x26] != 0) {
        work->cursorInputMask = 0x80;
    } else if (D_00324510[0x27] != 0) {
        work->cursorInputMask = 0x40;
    } else if (D_00324510[0x24] != 0) {
        work->cursorInputMask = 0x20;
    } else if (D_00324510[0x25] != 0) {
        work->cursorInputMask = 0x10;
    } else {
        work->cursorInputMask = 0;
    }
    direction = work->cursorInputMask;
    if (work->cursorMoving == 0) {
        if ((D_00324510[0x26] & 2) && (direction & 0x80)) {
            if (func_002CBE18(work->gridHandle) != 0) {
                feedback = 1;
            }
        } else if ((D_00324510[0x27] & 2) && (direction & 0x40)) {
            if (func_002CBF60(work->gridHandle) != 0) {
                feedback = 1;
            }
        } else if ((D_00324510[0x24] & 2) && (direction & 0x20)) {
            if (func_00252F88(work->gridHandle) != 0) {
                feedback = 1;
            }
        } else if ((D_00324510[0x25] & 2) && (direction & 0x10)) {
            if (func_00253018(work->gridHandle) != 0) {
                feedback = 1;
            }
        }
    }
    if (D_00324510[0x21] < 0 && work->transitionBlendCounter == 0) {
        entry = (MnuMantraGridEntry *)work->gridHandle->cursor->value;
        if (entry != NULL &&
            work->entryPosition.x == D_0036B7F0[entry->sceneId][2] &&
            work->entryPosition.y == D_0036B7F0[entry->sceneId][3]) {
            if (entry->state == MNU_MANTRA_GRID_ENTRY_FIRST_PATH_REJECTED) {
                feedback = 3;
            } else {
                feedback = 2;
                work->scenePhase = 4;
            }
        }
    } else if (D_00324510[0x23] < 0 && work->transitionBlendCounter == 0) {
        feedback = 3;
        work->scenePhase = 2;
    } else if (D_00324510[0x28] < 0) {
        feedback = 4;
        mnuBeginTransition(work, 1);
    } else if (D_00324510[0x2A] < 0) {
        feedback = 4;
        mnuBeginTransition(work, 2);
    } else if (D_00324510[0x20] < 0 && work->transitionBlendCounter == 0) {
        feedback = 2;
        work->scenePhase = 5;
    }
    switch (feedback) {
    case 1:
        return sndSetSequenceVolumePan(2, 127, 63);
    case 2:
        return sndSetSequenceVolumePan(8, 127, 63);
    case 3:
        return sndSetSequenceVolumePan(10, 127, 63);
    case 4:
        sndSetSequenceVolumePan(4, 127, 63);
        break;
    }
}


