#include "common.h"
#include "kwln.h"
#include "sdf.h"
#include "mnu.h"
#include "eff.h"
#include "mnu_list.h"

extern KwlnTask *kwlnTaskCreate();
extern void sdfCancelAndReleasePacWork(void *);
extern SdfMemBlock *sdfAllocGeneralBlock(s32);
extern u32 sdfResourceRetainAddress(SdfMemBlock *);
extern void sdfQueueNonzeroResourceId(s32);
extern u32 effLoadIndexedResource(const char *, const char *, s32);
extern u32 effDestroyResourceSlotSet(u32);

/* Title menu's selected page, sequence timers and draw-task state. */
typedef struct MenuTitleState {
    u8 pad00[0x14];
    s32 selectedPage;  /* 0x14 */
    u8 pad18[4];
    s32 sequenceTimer; /* 0x1C */
    u8 pad20[4];
    struct MenuList *selectionList; /* 0x24 */
    s32 phase;         /* 0x28 */
    u8 pad2C[0xD4];
    u8 movie[0xC];     /* 0x100 */
    u32 movieDrawActive; /* 0x10C */
    u32 drawAlpha;       /* 0x110 */
    u32 movieFrame;      /* 0x114 */
} MenuTitleState;

extern MenuTitleState *mnuMovieMenuState;

extern u16 mnuMovieTaskState;

/* Complete A8..BB scroll control retained by the staff-task allocation. */
typedef struct StaffScrollTransition {
    s32 mode;
    s32 opacity;
    s32 scrolling;
    s32 y;
    s32 countdown;
} StaffScrollTransition;
typedef struct {
    u32 mode;           /* 0x00 */
    s32 opacity;        /* 0x04: signed fade clamps */
    s32 phase;          /* 0x08 */
    s32 spriteIndices[4]; /* 0x0C */
} MnuTitlePaletteTransition;

typedef struct {
    SdfMemBlock *handle;
    u32 sprite;
    s32 state;
    s32 frame;
    s32 movieFrame; /* 0x10: staff movie fade-out frame count. */
    s32 fadeFrame; /* 0x14: staff movie decoder-ready hold count. */
    SlideBar slideBar; /* 0x18 */
    u32 backdropState[0x1B]; /* 0x20: opaque renderer state */
    MnuTitlePaletteTransition paletteTransition; /* 0x8C */
    StaffScrollTransition scrollTransition; /* 0xA8 */
    s32 scrollPaused; /* 0xBC: suppresses staff text and frame advancement. */
    u8 padC0[0x14];
    s32 streamPhase;
} StaffTaskState;

extern StaffTaskState *mnuMovieWork;
/* Retail 0x003E4A7C is a scalar in non-small .data. */
extern s32 D_003E4A7C __attribute__((section(".data")));
extern u32 D_00437AB8;
extern void func_002A6F88(u32 *);

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern char D_00429938[]; /* "staffImageProc" */

extern char D_00429968[]; /* "staffProc" */

extern u8 mnuMovieDrawContext[];

extern SdfPoolNode D_003803C8;

extern KwlnTask *mnuMovieDrawTask;

extern char D_00437AC0[];

extern void mnuStopTitleMovieDraw(void);

extern void func_003458E8(u32);

extern void func_002A5A78();

void mnuMarkTitleStreamResetPending(void);

void mnuResetTitleStreamLocked(void);

extern void func_002A5260(s32, s32);

extern void func_002A55B8(s32, s32);

extern void func_002A50E8(s32, s32, u8);

extern void mnuCallInitWide(s32, s32, s32, s32, s32);

extern void *memset(void *, s32, u32);
extern void func_00345BA0(void *, SdfPoolNode *);
extern void func_002A7B28(void *, SdfPoolNode *);
extern s32 mnuIsAnyMenuInputPressed(void);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern void mnuDrawSprite(s32, s32, s32, s32, s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5260);

void func_002A55B8(s32 mode, s32 frame) {
    s32 i;
    s32 elapsed;
    s32 alpha;
    f32 factor;

    switch (mode) {
    case 0:
        for (i = 0; i < 3; i++, frame -= 5) {
            elapsed = frame - 13;
            if (elapsed < 0) {
                elapsed = 0;
            }
            if (elapsed < 10) {
                factor = elapsed / 10.0f;
            } else {
                factor = 1.0f;
            }
            alpha = (s32)(factor * 128.0f);
            mnuDrawSprite(0, 0, 0, alpha, 0, i + 12, 0x53);
            if (i == ((MenuTitleState *)mnuMovieMenuState)->selectionList->cursor->index) {
                mnuDrawSprite(0, 0, 0, alpha, 0, i + 6, 0x53);
            }
        }
        break;
    case 1:
        for (i = 0; i < 3; i++) {
            elapsed = frame - i * 2;
            if (elapsed < 0) {
                elapsed = 0;
            }
            if (elapsed < 10) {
                factor = elapsed / 10.0f;
            } else {
                factor = 1.0f;
            }
            alpha = (s32)(factor * 128.0f);
            mnuDrawSprite(0, 0, 0, alpha, 0, i + 12, 0x53);
            if (i == ((MenuTitleState *)mnuMovieMenuState)->selectionList->cursor->index) {
                mnuDrawSprite(0, 0, 0, alpha, 0, i + 6, 0x53);
            }
        }
        break;
    case 2:
        factor = (10 - frame) / 10.0f;
        if (factor < 0.0f) {
            factor = 0.0f;
        }
        alpha = (s32)(factor * 128.0f);
        for (i = 0; i < 3; i++) {
            mnuDrawSprite(0, 0, 0, alpha, 0, i + 12, 0x53);
            if (i == ((MenuTitleState *)mnuMovieMenuState)->selectionList->cursor->index) {
                mnuDrawSprite(0, 0, 0, alpha, 0, i + 6, 0x53);
            }
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5890);

void mnuClearGlobalMenuStateFields(void) {
    MenuTitleState *state = (MenuTitleState *)mnuMovieMenuState;

    state->phase = 0;
    state->selectedPage = 0;
    state->sequenceTimer = 0;
}

void mnuResetTitlePageAndPhase(void) {
    MenuTitleState *state = (MenuTitleState *)mnuMovieMenuState;

    state->phase = 0;
    state->selectedPage = 0;
}

s32 mnuPollMovieMenuInputAndTimeout(void) {
    MenuTitleState *state = (MenuTitleState *)mnuMovieMenuState;
    switch (state->phase) {
    case 0:
        if (mnuIsAnyMenuInputPressed() == 0) {
            MenuTitleState *opening = (MenuTitleState *)mnuMovieMenuState;
            if (opening->selectedPage < 30) {
                opening->selectedPage++;
            } else {
                opening->phase = 1;
                opening->selectedPage = 0;
            }
            func_002A50E8((s32)&mnuMovieMenuState->movie, 0, 1);
            return 0;
        }
        sndSetSequenceVolumePan(8, 127, 63);
        func_002A50E8((s32)&mnuMovieMenuState->movie, 0, 0);
        return 1;
    case 1:
        if (state->selectedPage < 90) {
            state->selectedPage++;
        } else {
            state->selectedPage = 0;
        }
        if (mnuIsAnyMenuInputPressed() != 0) {
            sndSetSequenceVolumePan(8, 127, 63);
            func_002A50E8((s32)&mnuMovieMenuState->movie, 0, 0);
            return 1;
        }
        state = (MenuTitleState *)mnuMovieMenuState;
        state->sequenceTimer++;
        if (state->sequenceTimer >= 601) {
            state->sequenceTimer = 0;
            return -1;
        }
        break;
    case 2:
        if (state->selectedPage < 0) {
            state->selectedPage++;
        } else {
            return 1;
        }
        break;
    }
    return 0;
}

void mnuUpdateTitlePageByMode(void) {
    MenuTitleState *title = mnuMovieMenuState;

    switch (title->phase) {
    case 0:
        func_002A5260(0, title->selectedPage);
        return;
    case 1:
        func_002A5260(1, title->selectedPage);
        break;
    case 2:
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5A78);

extern s8 D_0037F510[];
extern struct MenuListNode *mnuRetreatListCursorDefault(u32);
extern struct MenuListNode *mnuAdvanceListCursorDefault(u32);
extern void mnuClearListFlagsOneAndTwo(u32 *);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern u32 func_002A3C78(void);

s32 func_002A5B08(void) {
    s32 moved = 0;
    s32 selection;

    if (D_0037F510[0x26] & 2) {
        if (mnuRetreatListCursorDefault((u32)mnuMovieMenuState->selectionList) != NULL) {
            sndSetSequenceVolumePan(0, 127, 63);
        }
        moved = 1;
    } else if (D_0037F510[0x27] & 2) {
        if (mnuAdvanceListCursorDefault((u32)mnuMovieMenuState->selectionList) != NULL) {
            sndSetSequenceVolumePan(0, 127, 63);
        }
        moved = 1;
    }
    if ((D_0037F510[0x26] == 0) & (D_0037F510[0x27] == 0)) {
        mnuClearListFlagsOneAndTwo(&mnuMovieMenuState->selectionList->stateFlags);
    }
    if (D_0037F510[0x21] < 0) {
        selection = func_002A3C78();
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
    if (D_0037F510[0x23] < 0) {
        sndSetSequenceVolumePan(10, 127, 63);
        return 2;
    }
    return moved ? -1 : 0;
}

void mnuTitleResetSequenceTimers(void) {
    MenuTitleState *title = (MenuTitleState *)mnuMovieMenuState;
    title->phase = 1;
    title->sequenceTimer = title->selectedPage = 0;
}

extern s32 func_002A5C58(void);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5C58);

void mnuDrawTitleSceneForPhase(void) {
    switch (((MenuTitleState *)mnuMovieMenuState)->phase) {
    case 1:
        func_002A5260(2, ((MenuTitleState *)mnuMovieMenuState)->selectedPage);
        func_002A55B8(0, ((MenuTitleState *)mnuMovieMenuState)->selectedPage);
        func_002A50E8((s32)&mnuMovieMenuState->movie, 1, 1);
        return;
    case 0:
        func_002A55B8(1, ((MenuTitleState *)mnuMovieMenuState)->selectedPage);
        func_002A50E8((s32)&mnuMovieMenuState->movie, 1, 1);
        return;
    case 2:
        mnuCallInitWide(0, 0, 0, (s32)((MenuTitleState *)mnuMovieMenuState)->selectionList, 0x53);
        return;
    case 3:
    case 4:
        func_002A55B8(2, ((MenuTitleState *)mnuMovieMenuState)->selectedPage);
        func_002A50E8((s32)&mnuMovieMenuState->movie, 1, 0);
        break;
    }
}

void mnuArmTitleMovieDrawAndResetFrame(u32 arg0, s32 arg1) {
    MenuTitleState *work;

    mnuRequestIndexedMovieResource();
    work = mnuMovieMenuState;
    if (mnuMovieMenuState != 0) {
        ((MenuTitleState *)mnuMovieMenuState)->movieDrawActive = 1;
        if (arg1 == 0) {
            work->drawAlpha = 0x80;
        }
        else {
            work->drawAlpha = 0;
        }
        ((MenuTitleState *)mnuMovieMenuState)->movieFrame = 0;
    }
}

void mnuStopTitleMovieDraw(void) {
    mnuStopMovieDrawTask();
    if (mnuMovieMenuState != 0) {
        ((MenuTitleState *)mnuMovieMenuState)->movieDrawActive = 0;
    }
}

u32 mnuIsTitleMovieDrawActive(void) {
    u32 state;

    state = 0;
    if (mnuMovieMenuState != 0) {
        state = ((MenuTitleState *)mnuMovieMenuState)->movieDrawActive;
    }
    return state;
}

extern void sdfSetGridScaledDrawBounds(s32, s32, s32, s32, u32);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5F80);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004287E0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004287F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428810);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428820);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428840);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428860);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428870);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428880);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428898);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004288A8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004288B8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004288C8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004288D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004288E8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004288F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428908);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428920);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428930);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428948);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428958);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428968);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428978);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428998);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004289A8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004289C0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004289D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004289E8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004289F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428A10);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428A28);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428A38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428A48);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428A60);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428A70);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428A80);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428A90);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428AA0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428AB8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428AC8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428AD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428AE8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428AF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428B08);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428B20);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428B38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428B50);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428B68);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428B78);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428B90);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428BA0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428BC0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428BD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428BF0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C08);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C28);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C48);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C58);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C68);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C78);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C88);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C98);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428CB0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428CD0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428CE0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428CF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428D10);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428D30);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428D40);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428D60);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428D70);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428D80);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428D90);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428DA0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428DB0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428DC8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428DD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428DE8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428E00);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428E18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428E30);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428E40);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428E58);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428E70);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428E88);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428EA0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428EB8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428EC8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428ED8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428EE8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F00);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F28);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F50);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F60);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F70);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F80);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F90);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428FA0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428FB0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428FC8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428FD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428FE8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428FF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429008);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429020);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429030);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429040);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429050);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429068);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429078);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429088);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429098);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004290A8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004290C0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004290D0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004290E0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004290F0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429100);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429118);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429130);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429140);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429150);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429160);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429170);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429180);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004291A0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004291B0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004291C8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004291D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004291E8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004291F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429208);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429218);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429228);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429238);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429250);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429260);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429278);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429288);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004292A0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004292B8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004292C8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004292E8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004292F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429318);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429330);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429350);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429368);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429390);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004293A0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004293B0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004293C0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004293E0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429408);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429420);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429430);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429448);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429458);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429468);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429480);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429498);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004294A8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004294C0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004294D0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004294E8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004294F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429508);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429518);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429530);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429540);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429560);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429570);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429588);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004295A0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004295B0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004295C8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004295E0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004295F0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429600);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429620);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429638);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429650);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429660);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429678);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429688);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004296A0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004296B0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004296C8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004296D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004296F0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429708);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429720);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429730);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429740);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429750);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429760);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429778);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429788);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429798);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004297A8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004297C0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004297D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004297E8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004297F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429808);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429830);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429840);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429858);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429868);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429878);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429888);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429898);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004298B0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004298C0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004298D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004298E8);

void mnuLoadMovieRollSprite(void) {
    mnuMovieWork->sprite = effLoadIndexedResource(D_00437AC0, "staff_01.spr", 0);
}

void func_002A6000(void) {
    mnuLoadStaffFonts();
}

void func_002A6018(void) {
    mnuUnloadStaffFonts();
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A6030);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A6180);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A6480);

extern s32 func_002A6580(void);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A6580);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429938);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A6858);

void mnuFadeSetState(SlideBar *state, u32 mode) {
    switch (mode) {
    case 2: state->pos = 0; mode = 0; break;
    case 3: mode = 1; state->pos = 0x200; break;
    }
    state->active = mode;
}

extern void func_00306CD0(s32, s32, s32, u32, s32, void *, s32, s32);

void mnuAdvanceSpriteSlideBar(SlideBar *bar) {
    u32 sprite = mnuMovieWork->sprite;

    if (bar->active == 0 && bar->pos == 0) {
        return;
    }
    func_00306CD0(0, 0, 0, bar->pos / 2, 0, (void *)sprite, 9, 0x53);
    if (bar->active == 0) {
        bar->pos -= 8;
    } else {
        bar->pos += 8;
    }
    if (bar->pos < 0) {
        bar->pos = 0;
    }
    if (bar->pos > 0x200) {
        bar->pos = 0x200;
    }
}

void mnuFadeSetStateOff(u32 *state, u32 mode) {
    switch (mode) {
    case 2: state[1] = 0; mode = 0; break;
    case 3: state[1] = 0; mode = 1; break;
    }
    state[0] = mode;
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A6D68);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A6F88);

extern void func_002A7260(MnuTitlePaletteTransition *transition, s32 randomize);
INCLUDE_ASM(const s32, "game/code_002A5260", func_002A7260);

void mnuTitleSetPaletteTransition(MnuTitlePaletteTransition *state, s32 mode) {
    switch (mode) {
    case 2:
        state->opacity = 0;
        mode = 0;
        state->phase = 0;
        func_002A7260(state, 0);
        break;
    case 3:
        state->opacity = 0x200;
        mode = 1;
        break;
    }
    state->mode = mode;
}

void func_002A73C0(MnuTitlePaletteTransition *state) {
    void *sprite = (void *)mnuMovieWork->sprite;
    s32 frame;
    s32 index;
    s32 alpha;

    if (state->mode == 0 && state->opacity == 0) {
        return;
    }
    frame = mnuMovieWork->frame;
    index = state->phase;
    frame %= 256;
    alpha = state->opacity * (255 - frame) / 512;
    func_00306CD0(0, 0, 0, alpha, 0, sprite, state->spriteIndices[index], 0x53);
    alpha = state->opacity * frame / 512;
    index = (state->phase + 1) % 4;
    func_00306CD0(0, 0, 0, alpha, 0, sprite, state->spriteIndices[index], 0x53);
    if (frame == 255) {
        if (index == 0) {
            func_002A7260(state, 1);
        }
        state->phase = index;
    }
    if (state->mode == 0) {
        state->opacity -= 8;
    } else {
        state->opacity += 8;
    }
    if (state->opacity < 0) {
        state->opacity = 0;
    }
    if (state->opacity > 512) {
        state->opacity = 512;
    }
}

void mnuFadeSetStateB(StaffScrollTransition *state, u32 mode) {
    switch (mode) {
    case 2: state->opacity = 0; mode = 0; break;
    case 3: mode = 1; state->opacity = 0x200; break;
    }
    state->mode = mode;
}


extern void uiDrawTexturedSurfaceAtFarDepth(u32);
extern void uiDrawSurfaceAtNearDepth(u32);
extern void func_00308F78(s32, u32);
struct EffRandState;
extern u32 effMiscRand(struct EffRandState *);
void func_002A75A8(StaffScrollTransition *state) {
    EffectSlotSet *sprites = (EffectSlotSet *)mnuMovieWork->sprite;
    s32 fade = state->opacity / 2;

    uiDrawTexturedSurfaceAtFarDepth(0x53);
    func_00306CD0(0, 0, 0, fade, 0x60, sprites, 4, 0x53);
    func_00308F78(1, 0x53);
    if (state->scrolling != 0) {
        func_00306CD0(0, state->y, 0, fade, 0x60, sprites, 7, 0x53);
    }
    uiDrawSurfaceAtNearDepth(0x53);
    if (state->scrolling == 0) {
        if (state->countdown == 0) {
            if (effMiscRand(0) % 180U == 0) {
                state->scrolling = 1;
                state->y = (sprites->workEntries[7].sourceHeight + 0x1C0) << 3;
            }
        } else {
            state->countdown--;
        }
    } else {
        state->y -= 0xA0;
        if (state->y < -(sprites->workEntries[7].sourceHeight << 3)) {
            state->scrolling = 0;
            state->countdown = 120;
        }
    }
    if (state->mode == 0) {
        state->opacity -= 8;
    } else {
        state->opacity += 8;
    }
    if (state->opacity < 0) {
        state->opacity = 0;
    }
    if (state->opacity > 0x200) {
        state->opacity = 0x200;
    }
}


s32 func_002A7730(void) {
    s32 seconds;
    s32 endSeconds;

    if (mnuMovieWork->frame == 0) {
        mnuFadeSetState(&mnuMovieWork->slideBar, 2);
        mnuFadeSetStateOff(mnuMovieWork->backdropState, 2);
        mnuTitleSetPaletteTransition(&mnuMovieWork->paletteTransition, 2);
        mnuFadeSetStateB(&mnuMovieWork->scrollTransition, 2);
    }
    seconds = mnuMovieWork->frame / 60;
    endSeconds = D_003E4A7C / 60;
    if (endSeconds + 5 < seconds) {
        mnuFadeSetState(&mnuMovieWork->slideBar, 0);
        mnuFadeSetStateOff(mnuMovieWork->backdropState, 0);
        mnuTitleSetPaletteTransition(&mnuMovieWork->paletteTransition, 0);
        mnuFadeSetStateB(&mnuMovieWork->scrollTransition, 0);
    } else {
        mnuFadeSetStateOff(mnuMovieWork->backdropState, 1);
        mnuTitleSetPaletteTransition(&mnuMovieWork->paletteTransition, 1);
        mnuFadeSetStateB(&mnuMovieWork->scrollTransition, 1);
    }
    if (D_00437AB8 == 4 || D_00437AB8 == 5) {
        mnuFadeSetState(&mnuMovieWork->slideBar, 2);
    } else {
        mnuFadeSetState(&mnuMovieWork->slideBar, 3);
    }
    mnuAdvanceSpriteSlideBar(&mnuMovieWork->slideBar);
    func_002A6F88(mnuMovieWork->backdropState);
    func_002A73C0(&mnuMovieWork->paletteTransition);
    func_002A75A8(&mnuMovieWork->scrollTransition);
    func_002A6858();
    return 0;
}

void mnuFinishStaffMovieAndFreeState(void) {
    s64 pending;

    mnuMovieTaskState = 2;
    mnuMarkTitleStreamResetPending();
    mnuResetTitleStreamLocked();
    func_002A6018();
    do {
        pending = sdfCheckPendingWorkWithInterrupts();
    } while (pending != 0);
    sdfQueueNonzeroResourceId((s32)mnuMovieWork->handle);
    mnuMovieWork = NULL;
}

void mnuReleaseMovieResourceAfterPendingWork(void) {
    effDestroyResourceSlotSet(mnuMovieWork->sprite);
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    func_003458E8(0);
}

extern u32 D_00437AB4;

void mnuInitializeMovieRollViewport(void) {
    D_00437AB8 = 4;
    D_00437AB4 = 0;
    mnuLoadMovieRollSprite();
    func_003458E8(1);
    sdfSetGridScaledDrawBounds(0x80, 0x60, 0x180, 0x100, 0x80808080);
}

extern u32 D_00435CBC;


void mnuCreateStaffTask(void) {
    SdfMemBlock *handle;

    D_00435CBC = 0x80000000;
    handle = sdfAllocGeneralBlock(0xD8);
    mnuMovieWork = (StaffTaskState *)sdfResourceRetainAddress(handle);
    memset(mnuMovieWork, 0, 0xD8);
    mnuMovieWork->handle = handle;
    mnuMovieWork->state = 0;
    mnuMovieWork->frame = 0;
    mnuMovieTaskState = 1;
    frFontUploadClearedTexture();
    kwlnTaskCreate(D_00429968, 0x408, 0, 0, func_002A6580, mnuFinishStaffMovieAndFreeState, 0);
}

u32 mnuStartStaffMovieRequest(void) {
    mnuCreateStaffTask();
    return 0xffffffff;
}

s32 mnuStopStaffTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00429938, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00429968, 1);
    return 0;
}

s32 mnuMovieDraw(void) {
    func_00345BA0(mnuMovieDrawContext, &D_003803C8);
    return 0;
}

extern char D_0042A338[]; /* "mnuMovieDraw" */

void mnuStartMovieDrawTaskForResource(u32 resource, void *data) {
    if (mnuMovieDrawTask == 0) {
        func_00346778(mnuMovieDrawContext, data, resource);
        mnuMovieDrawTask = kwlnTaskCreate(D_0042A338, 0x2afb, 1, 1, mnuMovieDraw, 0, 0);
    }
}

extern struct {
    u32 handle;
    u8 data[20];
} D_003E4C48[];

void mnuRequestIndexedMovieResource(index)
s32 index;
{
    u32 *entry = (u32 *)&D_003E4C48[index];
    mnuStartMovieDrawTaskForResource(*entry, entry + 1);
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A7B28);

extern char D_0042A348[];

extern char D_0042A380[];

extern u32 D_00437AD0;

s32 mnuMovieDrawNextProc(s32 procedure) {
    if (mnuMovieDrawTask == 0) {
        func_0035B6E0(D_0042A348);
        return -1;
    }
    func_002A7B28(mnuMovieDrawContext, &D_003803C8);
    D_00437AD0++;
    if (D_00437AD0 == 0x1E) {
        func_0035B6E0(D_0042A380, procedure);
        return -1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A7DB0);

typedef struct MovieDrawParams {
    s16 x, y;
    u32 unk04;
    u16 width, height;
    u32 unk0C;
    u8 unk10[4];
} MovieDrawParams;

typedef struct MoviePlaybackContext {
    u8 pad00[0x38];
    u8 unk38;
    u8 pad39[0x23];
    u32 *unk5C;
    u16 unk60, unk62;
} MoviePlaybackContext;

extern char *strcpy(char *, const char *);
extern MovieDrawParams D_00457DB0;
extern char D_00457DC8[];
extern char D_0042A3B0[];
extern u32 D_00437AD4;
extern u32 *D_00437AD8;
extern u32 D_00437ADC, D_00437AE0, D_00437AE4;
extern s32 func_002A7DB0();

/* Retain the current movie parameters while its stream is stopped, then queue
 * the replacement playback task. */
KwlnTask *mnuRequestMoviePlayback(const char *file, const MovieDrawParams *params) {
    MoviePlaybackContext *context;

    D_00457DB0 = *params;
    if (mnuMovieDrawTask != 0) {
        context = (MoviePlaybackContext *)mnuMovieDrawContext;
        D_00437AD4 = 1;
        D_00437AD8 = context->unk5C;
        D_00437ADC = context->unk60;
        D_00437AE0 = context->unk62;
        D_00437AE4 = context->unk38;
        strcpy(D_00457DC8, file);
        context->unk5C = 0;
        sdfCancelAndReleasePacWork(context);
        D_00457DB0.unk0C = D_00437AD8[3];
        D_00437AD0 = 0;
        return kwlnTaskCreate(D_0042A3B0, 0x2AFB, 0, 0, func_002A7DB0, 0, 0);
    } else {
        D_00437AD4 = 0;
        func_00346778(mnuMovieDrawContext, &D_00457DB0, file);
        mnuMovieDrawTask = kwlnTaskCreate(D_0042A338, 0x2AFB, 1, 1, mnuMovieDraw, 0, 0);
    }
    return mnuMovieDrawTask;
}

void func_002A7F98(s32 index) {
    u32 *entry = (u32 *)&D_003E4C48[index];
    mnuRequestMoviePlayback((const char *)*entry, (const MovieDrawParams *)(entry + 1));
}

void mnuStopMovieDrawTask(void) {
    if (mnuMovieDrawTask == 0) {
        return;
    }
    sdfCancelAndReleasePacWork(mnuMovieDrawContext);
    kwlnTaskDestroyWithHierarchy(mnuMovieDrawTask, 0);
    mnuMovieDrawTask = 0;
}

s32 mnuCheckMovieDecoderStatus(void) {
    return sdfPacCheckDecoderStatus(mnuMovieDrawContext);
}

extern u8 D_003E563C[];

u8 func_002A8028(void) {
    return D_003E563C[0];
}

u8 func_002A8038(void) {
    return mnuMovieDrawContext[0];
}

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429968);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429978);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429998);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004299B8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004299D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004299F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429A18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429A38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429A58);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429A78);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429A98);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429AB8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429AD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429AF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429B18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429B38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429B58);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429B78);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429B98);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429BB8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429BD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429BF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429C18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429C38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429C58);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429C78);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429C98);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429CB8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429CD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429CF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429D18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429D38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429D58);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429D78);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429D98);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429DB8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429DD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429DF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429E18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429E38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429E58);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429E78);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429E98);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429EB8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429ED8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429EF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429F18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429F38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429F58);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429F78);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429F98);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429FB8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429FD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429FF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A018);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A038);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A058);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A078);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A098);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A0B8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A0D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A0F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A118);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A138);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A158);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A178);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A198);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A1B8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A1D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A1F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A218);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A238);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A258);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A278);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A298);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A2B8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A2D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A2F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A318);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A338);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A348);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A380);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A3B0);
