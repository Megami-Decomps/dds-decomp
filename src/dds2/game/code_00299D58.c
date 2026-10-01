#include "mnu.h"
extern s32 mdlFlagTest(u32 flagId);

extern void mdlFlagSet(u32 flagId);

extern void dspSetActive(s32 index);

extern void dspStartEntry(s32 index);

/* DSP request bookkeeping lives in the menu scene's own state word. */
typedef struct MenuDspState {
    u8 pad00[4];
    u32 flags; /* 0x04 */
} MenuDspState;


extern u32 func_0029D790(u32, s32);

extern void func_0026C900(void);

extern s32 kwlnTaskGetUserValue();

extern void func_002C44E8(s32);

extern void mnuTitleRenderFadeAndPanels(s32);

extern s32 brsTaskIsUiUpdateAllowed(s32);

extern void brsDecaySharedAnimCounter(s32);

typedef struct MenuItem {
    u8 pad00[0x55];
    s8 selection;
} MenuItem;

typedef struct MenuItemScene {
    u8 pad00[4];
    u32 overlayFlags;
    u8 pad08[0x90];
    MenuItem **items;
    u8 pad9C[0x1A4];
    u32 resetStateA;
    u32 selectedAction;
    u8 pad248[4];
    s32 selectionApplied;
    u8 pad250[0x174];
    u32 resetStateB;
    s32 selectedExtent;
    u32 activeSlot;
    u32 slots[5];
} MenuItemScene;

/* Parked: build/parked/dds2/game/code_00299D58/brsMessageInputStep.c (delay-slot
   fill differs from retail by one word; every source shape tried agrees). */
INCLUDE_ASM(const s32, "game/code_00299D58", brsMessageInputStep);

s64 mnuStaffRunPanel1(s32 input) {
    s32 context = kwlnTaskGetUserValue();

    if (brsTaskIsUiUpdateAllowed(context) != 0) {
        mnuTitleRenderFadeAndPanels(context);
        return menuSetHandler(context, 1, input);
    }
}

s64 mnuStaffRunPanel2(s32 input) {
    s32 context = kwlnTaskGetUserValue();

    if (brsTaskIsUiUpdateAllowed(context) != 0) {
        brsDecaySharedAnimCounter(context);
        return menuSetHandler(context, 2, input);
    }
}

u32 func_00299EF0(void) {
    return 1;
}

u32 func_00299EF8(void) {
    return 1;
}

s64 mnuStaffRunPanel0(s32 input) {
    s32 context = kwlnTaskGetUserValue();
    func_002C44E8(0x33);
    return menuSetHandler(context, 0, input);
}

s64 func_00299F50(s32 input) {
    s32 context = kwlnTaskGetUserValue();
    mnuTitleRenderFadeAndPanels(context);
    return menuSetHandler(context, 1, input);
}

s64 func_00299FA0(s32 input) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, input);
}

u32 func_00299FD8(void) {
    return 1;
}

u32 func_00299FE0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00299D58", func_00299FE8);

/* Build the capped skill list for the currently selected menu entry. */
u32 mnuProcessItemSelection(u32 context) {
    u32 listState;
    s32 scene;

    scene = (s32)context;
    /* Keep these raw accesses: typed field accesses change the alias schedule. */
    listState = func_0029D790(**(u32 **)(scene + 0x9c), scene + 0x4e8);
    *(u32 *)(scene + 0x268) = listState;
    func_00299FE8(context);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00299D58", prfCapPresentMessages);

u32 func_0029A1E0(void) {
    return 0;
}

s32 mnuRequestContextLatchedSceneDsp(MenuDspState *state)
{
    if (mdlFlagTest(0xB8F) == 0) {
        if (state->flags & 1) {
            if (mdlFlagTest(0x915) != 0) {
                return 0;
            }
            dspSetActive(1);
            dspStartEntry(1);
            mdlFlagSet(0x915);
            state->flags |= 2;
            return 1;
        }
    }
    return 0;
}

s32 mnuRequestContextClearSceneDsp(MenuDspState *state)
{
    if (mdlFlagTest(0xB8F) == 0) {
        if (mdlFlagTest(0x290) != 0) {
            if (state->flags & 2) {
                return 0;
            }
            if (mdlFlagTest(0x817) != 0) {
                return 0;
            }
            dspSetActive(1);
            dspStartEntry(2);
            mdlFlagSet(0x817);
            return 1;
        }
        return 0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A2F8);

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A400);

s64 func_0029A588(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    mnuTitleRenderFadeAndPanels(context);
    return menuSetHandler(context, 1, request);
}

s64 func_0029A5D8(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(context, 2, request);
}

s32 mnuResetItemSelectionMarkers(void) {
    s32 *context = (s32 *)kwlnTaskGetUserValue();

    context[0x99] = 0;
    context[0xFA] = 0;
    return 1;
}

u32 func_0029A650(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A658);

u32 mnuKindIsSelectable(u32 kind) {
    if (kind == 1) {
        return 1;
    }
    return (kind ^ 8) < 1;
}

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A768);

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A898);

INCLUDE_SDATA(const s32, "game/code_00299D58", D_004379B0);

