#include "common.h"

/* Transition host: two optional callbacks at +0xC4 and the flag at +0xCC that
   picks which value they are called with. */
typedef struct TransitionHost {
    u8 pad00[0xC4];
    void (*callbacks[2])(s32, struct TransitionHost *); /* 0xC4 */
    u32 unkCC; /* 0xCC */
} TransitionHost;

typedef struct MenuFadeHost {
    u8 pad00[0x7C];
    s32 reduced;      /* 0x7C */
    u8 pad80[0xE0];
    s32 fadeColor;    /* 0x160 */
} MenuFadeHost;

extern void sndStartTrackExtended(s32);

extern void func_002E9708(void);

extern void func_002E96D8(s32);

extern void func_002E9730(void);


INCLUDE_ASM(const s32, "game/code_0024A728", func_0024A728);

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024A930);

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024AB28);

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024AB70);

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024ACD8);

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024AE18);

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024AF58);

typedef struct GridPanelHost {
    u8 pad00[0x64];
    s32 grid;           /* 0x64 */
    u8 pad68[0x38];
    s32 settings[1];    /* 0xA0 */
} GridPanelHost;

extern void itfSetGridEntryQuantizedAndRefresh(s32, s32, s32, s32, s32, s32);
extern void effConfigureWithDefaultSetting(s32, s32, s32, s32, s32, s32);

/* Reset grid entry 0x1A, then configure it from the panel's setting slot chosen by `kind`. */
void func_0024B090(u32 kind, GridPanelHost *host) {
    s32 flags = 0;
    s32 value = 0;
    s32 slot = 0;

    switch (kind) {
    case 2:
        flags = 2;
        value = 4;
        slot = 3;
        break;
    case 3:
        flags = 2;
        value = 7;
        slot = 2;
        break;
    case 4:
        value = 4;
        slot = 3;
        break;
    }
    itfSetGridEntryQuantizedAndRefresh(host->grid, 0x1A, 0, 0, 0, 0);
    effConfigureWithDefaultSetting(host->grid, 0x1A, host->settings[slot], 0, value, flags);
}

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024B168);

typedef struct {
    u8 pad00[0xC4];
    u32 callback;         /* 0xC4 */
    u32 previousCallback; /* 0xC8 */
} SceneTransition;

void evtRememberDispatchCallback(u32 callback, SceneTransition *transition) {
    u32 previous;

    previous = transition->callback;
    transition->callback = callback;
    transition->previousCallback = previous;
}

/* Call each registered transition callback once; the flag at +0xCC picks the
   index they receive. */
void mnuDispatchTransitionHostCallbacks(TransitionHost *host) {
    u32 i;

    for (i = 0; i < 2; i++) {
        if (host->callbacks[i] != NULL) {
            host->callbacks[i](host->unkCC ? 1 : (s32)i, host);
        }
    }
}

void mnuApplyFadeTrackMode(s32 mode, MenuFadeHost *host) {
    if (mode == 0) {
        if (host->reduced == 0) {
            sndStartTrackExtended(host->fadeColor);
        } else {
            func_002E9708();
        }
    } else if (host->reduced == 0) {
        func_002E96D8(host->fadeColor);
    } else {
        func_002E9730();
    }
}

INCLUDE_RODATA(const s32, "game/code_0024A728", D_003AF6B0);

INCLUDE_RODATA(const s32, "game/code_0024A728", D_003AF6E0);

INCLUDE_RODATA(const s32, "game/code_0024A728", D_003AF6F0);

INCLUDE_RODATA(const s32, "game/code_0024A728", D_003AF700);

