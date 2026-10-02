#include "common.h"

extern s32 mnuMovieMenuState;

extern u32 effLoadIndexedResource(const char *, const char *, u32);
extern void effRequestResourceByMode(const char *, const char *, u32, u32 *);

extern char D_003BC5D8[];
extern char D_00379E50[];

extern void func_0026C7E0();

typedef struct {
    s32 pad00;
    u32 firstResource;    /* 0x04: released by mnuReleaseMenuResourceSlots */
    s32 spriteHandle;     /* 0x08: released through effDestroyResourceSlotSet */
    u32 secondResource;   /* 0x0C: released by mnuReleaseMenuResourceSlots */
    s32 pad10;
    s32 word14;
    s32 pad18;
    s32 word1C;
    s32 pad20;
    s32 transitionOffset; /* 0x24 */
    s32 mode;             /* 0x28 */
    u32 linkedState;       /* 0x2C: passed to the func_0027Bxxx helpers */
    s32 pad30;
    s32 word34;
    s32 word38;
} MenuState;

extern s8 D_00324510[];

extern void mnuReleaseMenuResourceSlots(void);

extern s32 mnuCreateListState(s32, s32, s32);

extern s32 mnuListAppendNode(s32, s32);

extern void func_0026D480();
extern void sndSetSequenceVolumePan(s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_0026BD80", mnuDrawSprite);

void mnuRecreateMenuSelectionList(void) {
    s32 i;
    s32 node;

    if (((MenuState *)mnuMovieMenuState)->linkedState != 0) {
        mnuDestroyListState(((MenuState *)mnuMovieMenuState)->linkedState);
    }
    node = mnuCreateListState(0, 3, 0);
    ((MenuState *)mnuMovieMenuState)->linkedState = node;
    ((MenuState *)node)->linkedState = (s32)func_0026D480;
    for (i = 0; i < 3; i++) {
        mnuListAppendNode(((MenuState *)mnuMovieMenuState)->linkedState, 0);
    }
}

u32 mnuDestroyMovieMenuSelectionList(void) {
    return mnuDestroyListState(((MenuState *)mnuMovieMenuState)->linkedState);
}

u32 func_0026BED0(void) {
    return **(u32 **)(((MenuState *)mnuMovieMenuState)->linkedState + 0x1c);
}

void mnuSelectMenuListCursorByAdvance(s32 advanceCount) {
    mnuSelectFirstListNode(((MenuState *)mnuMovieMenuState)->linkedState);
    if (0 < advanceCount) {
        do {
            advanceCount = advanceCount - 1;
            mnuAdvanceListCursorDefault(((MenuState *)mnuMovieMenuState)->linkedState);
        } while (advanceCount != 0);
    }
}

s32 mnuIsAnyMenuInputPressed(void) {
    if (D_00324510[0x21] < 0 || D_00324510[0x23] < 0 ||
        D_00324510[0x22] < 0 || D_00324510[0x20] < 0 ||
        D_00324510[0x2a] < 0 || D_00324510[0x2b] < 0 ||
        D_00324510[0x28] < 0 || D_00324510[0x29] < 0 ||
        D_00324510[0x2d] < 0 || D_00324510[0x2c] < 0) {
        return 1;
    }
    return 0;
}

void func_0026BFC8(void) {
    if (((MenuState *)mnuMovieMenuState)->firstResource == 0) {
        ((MenuState *)mnuMovieMenuState)->firstResource =
            effLoadIndexedResource(D_003BC5D8, D_00379E50, 0);
    }
    if (((MenuState *)mnuMovieMenuState)->secondResource == 0) {
        const char *name = D_00379E50;
        name += 0x20;
        ((MenuState *)mnuMovieMenuState)->secondResource =
            effLoadIndexedResource(D_003BC5D8, name, 0);
    }
}

u32 func_0026C040(void) {
    return 1;
}

void mnuReleaseMenuResourceSlots(void) {
    MenuState *state = (MenuState *)mnuMovieMenuState;
    if (state->firstResource != 0) {
        effDestroyResourceSlotSet(state->firstResource);
        state = (MenuState *)mnuMovieMenuState;
        state->firstResource = 0;
    }
    if (state->secondResource != 0) {
        effDestroyResourceSlotSet(state->secondResource);
        state = (MenuState *)mnuMovieMenuState;
        state->secondResource = 0;
    }
}

void func_0026C098(s32 mode) {
    u32 *handle = &((MenuState *)mnuMovieMenuState)->spriteHandle;

    if (*handle == 0) {
        if (mode != 0) {
            const char *name = D_00379E50;
            name += 0x10;
            ((MenuState *)mnuMovieMenuState)->spriteHandle =
                effLoadIndexedResource(D_003BC5D8, name, 0);
        } else {
            const char *name = D_00379E50;
            name += 0x10;
            effRequestResourceByMode(D_003BC5D8, name, 0,
                                     handle);
        }
    }
}

u8 mnuHasSpriteHandle(void) {
    return ((MenuState *)mnuMovieMenuState)->spriteHandle != 0;
}

void mnuReleaseSpriteHandle(void) {
    if (((MenuState *)mnuMovieMenuState)->spriteHandle != 0) {
        effDestroyResourceSlotSet(((MenuState *)mnuMovieMenuState)->spriteHandle);
        ((MenuState *)mnuMovieMenuState)->spriteHandle = 0;
    }
}

extern void mnuDrawSprite(s32, s32, s32, s32, s32, s32, s32);

/* These entry points differ only in the selected menu sound identifier. */
void mnuStartMovieMenuSfx16(s32 parameter) {
    mnuDrawSprite(0, 0, 0, parameter, 0, 0x16, 0x53);
}

void mnuStartMovieMenuSfx17(s32 parameter) {
    mnuDrawSprite(0, 0, 0, parameter, 0, 0x17, 0x53);
}

void mnuStartMovieMenuSfx18(s32 parameter) {
    mnuDrawSprite(0, 0, 0, parameter, 0, 0x18, 0x53);
}

void func_0026C1F8(s32 parameter) {
    mnuDrawSprite(0, 0, 0, parameter, 0, 0x1A, 0x53);
}

extern void sdfSubmitGsAlphaOneRegisterPacket(s32, s32);
extern void sdfSubmitGsTestOneRegisterPacket(s32, s32);
extern void uiDrawUniformColorRect(s32, s32, s32, s32, s32, s32, s32);

void mnuDrawMovieMenuBackgroundQuad(s32 parameter) {
    sdfSubmitGsAlphaOneRegisterPacket(0x44, 0x3E);
    sdfSubmitGsTestOneRegisterPacket(0x3000D, 0x3E);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, parameter, 0x3E);
}

void mnuDrawMovieMenuSpriteLayers(s32 context) {
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0x80, context);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 0, context);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 2, context);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 1, context);
    mnuDrawSprite(1, -10, 0, 0x80, 0, 4, context);
}

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026C350);

void func_0026C4A8(void) {
    MenuState *state = (MenuState *)mnuMovieMenuState;

    state->word14 = 0;
    state->word38 = 0;
}

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026C4B8);

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026C7E0);

void mnuStartMovieMenuSfxGroup(void) {
    mnuDrawSprite(0, 0, 0, 0x80, 0, 6, 0x53);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 0xD, 0x53);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 0xE, 0x53);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 0xF, 0x53);
}

void func_0026CAB0(void) {
    func_0026C7E0();
}

void mnuSwapStateWords(void) {
    MenuState *state = (MenuState *)mnuMovieMenuState;

    switch (state->mode) {
    case 0:
        state->transitionOffset = 0;
        state->mode = 1;
        break;
    case 1:
        state->mode = 0;
        state->transitionOffset = 0x6a4;
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026CB10);

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026CD88);

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026D108);

void mnuClearGlobalMenuStateFields(void) {
    MenuState *state = (MenuState *)mnuMovieMenuState;

    state->word34 = 0;
    state->word14 = 0;
    state->word1C = 0;
}

void mnuResetTitlePageAndPhase(void) {
    MenuState *state = (MenuState *)mnuMovieMenuState;

    state->word34 = 0;
    state->word14 = 0;
}

s32 mnuPollMovieMenuInputAndTimeout(void) {
    MenuState *state = (MenuState *)mnuMovieMenuState;
    switch (state->word34) {
    case 0:
        if (mnuIsAnyMenuInputPressed() == 0) {
            MenuState *opening = (MenuState *)mnuMovieMenuState;
            if (opening->word14 < 30) {
                opening->word14++;
            } else {
                opening->word34 = 1;
                opening->word14 = 0;
            }
            return 0;
        }
        sndSetSequenceVolumePan(8, 127, 63);
        return 1;
    case 1:
        if (state->word14 < 90) {
            state->word14++;
        } else {
            state->word14 = 0;
        }
        if (mnuIsAnyMenuInputPressed() != 0) {
            sndSetSequenceVolumePan(8, 127, 63);
            return 1;
        }
        state = (MenuState *)mnuMovieMenuState;
        state->word1C++;
        if (state->word1C >= 601) {
            state->word1C = 0;
            return -1;
        }
        break;
    case 2:
        if (state->word14 < 0) {
            state->word14++;
        } else {
            return 1;
        }
        break;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0026BD80", D_003AFE40);

INCLUDE_RODATA(const s32, "game/code_0026BD80", D_003AFE50);

INCLUDE_RODATA(const s32, "game/code_0026BD80", D_003AFE80);

INCLUDE_SDATA(const s32, "game/code_0026BD80", D_003BC5D8);

INCLUDE_SDATA(const s32, "game/code_0026BD80", D_003BC5E0);

INCLUDE_SDATA(const s32, "game/code_0026BD80", D_003BC5E8);

INCLUDE_SDATA(const s32, "game/code_0026BD80", D_003BC5F0);

INCLUDE_SDATA(const s32, "game/code_0026BD80", D_003BC5F8);

INCLUDE_SDATA(const s32, "game/code_0026BD80", D_003BC600);

INCLUDE_SDATA(const s32, "game/code_0026BD80", D_003BC608);

