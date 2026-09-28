#include "common.h"

extern s32 D_003BC5D0;

extern void func_0026C7E0();

typedef struct {
    s32 pad00[2];
    s32 task;             /* 0x08 */
    s32 pad0C[2];
    s32 word14;
    s32 pad18;
    s32 word1C;
    s32 pad20;
    s32 transitionOffset; /* 0x24 */
    s32 mode;             /* 0x28 */
    s32 pad2C[2];
    s32 word34;
    s32 word38;
} MenuState;

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026BD80);

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026BE38);

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026BEB0);

u32 func_0026BED0(void) {
    return **(u32 **)(*(s32 *)(D_003BC5D0 + 0x2c) + 0x1c);
}

void func_0026BEE8(s32 arg0) {
    func_0027BB08(*(u32 *)(D_003BC5D0 + 0x2c));
    if (0 < arg0) {
        do {
            arg0 = arg0 - 1;
            func_0027BE90(*(u32 *)(D_003BC5D0 + 0x2c));
        } while (arg0 != 0);
    }
}

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026BF38);

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026BFC8);

u32 func_0026C040(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026C048);

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026C098);

u8 func_0026C108(void) {
    return ((MenuState *)D_003BC5D0)->task != 0;
}

void func_0026C118(void) {
    if (*(s32 *)(D_003BC5D0 + 8) != 0) {
        func_002BDD60(*(s32 *)(D_003BC5D0 + 8));
        *(u32 *)(D_003BC5D0 + 8) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026C150);

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026C188);

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026C1C0);

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026C1F8);

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026C230);

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026C290);

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026C350);

void func_0026C4A8(void) {
    MenuState *state = (MenuState *)D_003BC5D0;

    state->word14 = 0;
    state->word38 = 0;
}

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026C4B8);

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026C7E0);

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026CA18);

void func_0026CAB0(void) {
    func_0026C7E0();
}

void mnuSwapStateWords(void) {
    MenuState *state = (MenuState *)D_003BC5D0;

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

void func_0026D138(void) {
    MenuState *state = (MenuState *)D_003BC5D0;

    state->word34 = 0;
    state->word14 = 0;
    state->word1C = 0;
}

void func_0026D150(void) {
    MenuState *state = (MenuState *)D_003BC5D0;

    state->word34 = 0;
    state->word14 = 0;
}

INCLUDE_ASM(const s32, "game/code_0026BD80", func_0026D160);

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

