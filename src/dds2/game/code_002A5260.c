#include "common.h"

/* Sliding menu bar: direction flag and 0..max position */
typedef struct { s32 active; s32 pos; } SlideBar;

/* Title menu's selected page, sequence timers and draw-task state. */
typedef struct MenuTitleState {
    u8 pad00[0x14];
    s32 selectedPage;  /* 0x14 */
    u8 pad18[4];
    s32 sequenceTimer; /* 0x1C */
    u8 pad20[4];
    s32 overlayHandle; /* 0x24 */
    s32 phase;         /* 0x28 */
    u8 pad2C[0xE0];
    u32 movieDrawActive; /* 0x10C */
    u32 drawAlpha;       /* 0x110 */
    u32 movieFrame;      /* 0x114 */
} MenuTitleState;

extern s32 mnuMovieMenuState;

extern u16 mnuMovieTaskState;

extern u32 *mnuMovieWork;

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern char D_00429938[]; /* "staffImageProc" */

extern char D_00429968[]; /* "staffProc" */

extern u8 mnuMovieDrawContext[];

extern u8 D_003803C8[];

extern u32 mnuMovieDrawTask;

extern u8 D_00437AC0[];

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
extern s32 mnuIsAnyMenuInputPressed(void);
extern void sndSetSequenceVolumePan(s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5260);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A55B8);

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

s32 func_002A58E8(void) {
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
            func_002A50E8(mnuMovieMenuState + 0x100, 0, 1);
            return 0;
        }
        sndSetSequenceVolumePan(8, 127, 63);
        func_002A50E8(mnuMovieMenuState + 0x100, 0, 0);
        return 1;
    case 1:
        if (state->selectedPage < 90) {
            state->selectedPage++;
        } else {
            state->selectedPage = 0;
        }
        if (mnuIsAnyMenuInputPressed() != 0) {
            sndSetSequenceVolumePan(8, 127, 63);
            func_002A50E8(mnuMovieMenuState + 0x100, 0, 0);
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
    s32 *title = (s32 *)mnuMovieMenuState;

    switch (title[0x28 / 4]) {
    case 0:
        func_002A5260(0, title[0x14 / 4]);
        return;
    case 1:
        func_002A5260(1, title[0x14 / 4]);
        break;
    case 2:
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5A78);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5B08);

void mnuTitleResetSequenceTimers(void) {
    MenuTitleState *title = (MenuTitleState *)mnuMovieMenuState;
    title->phase = 1;
    title->sequenceTimer = title->selectedPage = 0;
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5C58);

void mnuDrawTitleSceneForPhase(void) {
    switch (((MenuTitleState *)mnuMovieMenuState)->phase) {
    case 1:
        func_002A5260(2, ((MenuTitleState *)mnuMovieMenuState)->selectedPage);
        func_002A55B8(0, ((MenuTitleState *)mnuMovieMenuState)->selectedPage);
        func_002A50E8(mnuMovieMenuState + 0x100, 1, 1);
        return;
    case 0:
        func_002A55B8(1, ((MenuTitleState *)mnuMovieMenuState)->selectedPage);
        func_002A50E8(mnuMovieMenuState + 0x100, 1, 1);
        return;
    case 2:
        mnuCallInitWide(0, 0, 0, ((MenuTitleState *)mnuMovieMenuState)->overlayHandle, 0x53);
        return;
    case 3:
    case 4:
        func_002A55B8(2, ((MenuTitleState *)mnuMovieMenuState)->selectedPage);
        func_002A50E8(mnuMovieMenuState + 0x100, 1, 0);
        break;
    }
}

void mnuArmTitleMovieDrawAndResetFrame(u32 arg0, s32 arg1) {
    u32 work;

    func_002A7AF0();
    work = mnuMovieMenuState;
    if (mnuMovieMenuState != 0) {
        ((MenuTitleState *)mnuMovieMenuState)->movieDrawActive = 1;
        if (arg1 == 0) {
            ((MenuTitleState *)work)->drawAlpha = 0x80;
        }
        else {
            ((MenuTitleState *)work)->drawAlpha = 0;
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

extern void sdfSetGridScaledDrawBounds(u32, u32, u32, u32, u32);

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
    mnuMovieWork[1] = effLoadIndexedResource(D_00437AC0, "staff_01.spr", 0);
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

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A6580);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429938);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A6858);

void mnuFadeSetState(u32 *state, u32 mode) {
    switch (mode) {
    case 2: state[1] = 0; mode = 0; break;
    case 3: mode = 1; state[1] = 0x200; break;
    }
    state[0] = mode;
}

extern void func_00306CD0(s32, s32, s32, s32, s32, s32, s32, s32);

void mnuAdvanceSpriteSlideBar(SlideBar *bar) {
    u32 sprite = mnuMovieWork[1];

    if (bar->active == 0 && bar->pos == 0) {
        return;
    }
    func_00306CD0(0, 0, 0, bar->pos / 2, 0, sprite, 9, 0x53);
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

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A7260);

void mnuTitleSetPaletteTransition(u32 *state, s32 mode) {
    switch (mode) {
    case 2:
        state[1] = 0;
        mode = 0;
        state[2] = 0;
        func_002A7260(state, 0);
        break;
    case 3:
        state[1] = 0x200;
        mode = 1;
        break;
    }
    state[0] = mode;
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A73C0);

void mnuFadeSetStateB(u32 *state, u32 mode) {
    switch (mode) {
    case 2: state[1] = 0; mode = 0; break;
    case 3: mode = 1; state[1] = 0x200; break;
    }
    state[0] = mode;
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A75A8);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A7730);

void mnuFinishStaffMovieAndFreeState(void) {
    s64 pending;

    mnuMovieTaskState = 2;
    mnuMarkTitleStreamResetPending();
    mnuResetTitleStreamLocked();
    func_002A6018();
    do {
        pending = sdfCheckPendingWorkWithInterrupts();
    } while (pending != 0);
    sdfQueueNonzeroResourceId(*mnuMovieWork);
    mnuMovieWork = (u32 *)0x0;
}

void mnuReleaseMovieResourceAfterPendingWork(void) {
    effDestroyResourceSlotSet(mnuMovieWork[1]);
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    func_003458E8(0);
}

extern u32 D_00437AB4;

extern u32 D_00437AB8;

void mnuInitializeMovieRollViewport(void) {
    D_00437AB8 = 4;
    D_00437AB4 = 0;
    mnuLoadMovieRollSprite();
    func_003458E8(1);
    sdfSetGridScaledDrawBounds(0x80, 0x60, 0x180, 0x100, 0x80808080);
}

extern u32 D_00435CBC;

extern void func_002A6580();

typedef struct {
    u32 handle;
    u32 unk4;
    u32 unk8;
    u32 unkC;
} StaffTaskState;

void mnuCreateStaffTask(void) {
    u32 handle;

    D_00435CBC = 0x80000000;
    handle = func_003292A8(0xD8);
    mnuMovieWork = sdfResourceRetainAddress(handle);
    memset(mnuMovieWork, 0, 0xD8);
    ((StaffTaskState *)mnuMovieWork)->handle = handle;
    ((StaffTaskState *)mnuMovieWork)->unk8 = 0;
    ((StaffTaskState *)mnuMovieWork)->unkC = 0;
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
    func_00345BA0(mnuMovieDrawContext, D_003803C8);
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

void func_002A7AF0(index)
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
    func_002A7B28(mnuMovieDrawContext, D_003803C8);
    D_00437AD0++;
    if (D_00437AD0 == 0x1E) {
        func_0035B6E0(D_0042A380, procedure);
        return -1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A7DB0);

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

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A7E60);

void func_002A7F98(s32 index) {
    u32 *entry = (u32 *)&D_003E4C48[index];
    func_002A7E60(*entry, entry + 1);
}

void mnuStopMovieDrawTask(void) {
    if (mnuMovieDrawTask == 0) {
        return;
    }
    func_00346988(mnuMovieDrawContext);
    kwlnTaskDestroyWithHierarchy(mnuMovieDrawTask, 0);
    mnuMovieDrawTask = 0;
}

s32 func_002A8008(void) {
    return func_00346A60(mnuMovieDrawContext);
}

extern u8 D_003E563C[];

u8 func_002A8028(void) {
    return D_003E563C[0];
}

u8 func_002A8038(void) {
    return mnuMovieDrawContext[0];
}

