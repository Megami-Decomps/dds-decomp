#include "common.h"

extern u32 ptyBuildProfileCapSkillList(u32, s32);

extern s32 mdlFlagTest(u32);

extern s32 func_00101A70();

extern void func_0024DDC0(s32);

extern void func_0024DA58(s32);

extern s32 ptyGetCurrentProfileRecord(void *);

extern s32 prfGetCapValue(u16);

extern s32 ptyTestProfileFlag0(void *, u16);

extern void ptyApplyProfile(void *, u16);

extern void mdlFlagSet(s32);

extern void mnuRefreshPanelLayer(s32 arg0);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern void func_0024DD78(void);
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


void kwlnItemApplySelection(MenuItemScene *scene) {
    MenuItem *item = *scene->items;
    s32 *data = (s32 *)ptyGetCurrentProfileRecord(item);
    s8 selection = item->selection;
    if (selection != 0 && prfGetCapValue((u16)selection) == *data &&
        ptyTestProfileFlag0(item, (u16)(s8)item->selection) == 0) {
        ptyApplyProfile(item, (u16)(s8)item->selection);
        scene->selectionApplied = 1;
        if (mdlFlagTest(0x910) == 0) {
            scene->overlayFlags |= 1;
            mdlFlagSet(0x910);
        }
    } else {
        scene->selectionApplied = 0;
    }
}

/* Build the capped skill list for the currently selected menu entry. */
u32 mnuProcessItemSelection(u32 context) {
    u32 listState;
    s32 scene;

    scene = (s32)context;
    /* Required to match: typed MenuItemScene field accesses change this
     * compiler's alias scheduling and overrun the next retail function. */
    listState = ptyBuildProfileCapSkillList(**(u32 **)(scene + 0x98), scene + 0x4c4);
    *(u32 *)(scene + 0x244) = listState;
    kwlnItemApplySelection(context);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00263148", prfCapPresentMessages);

s32 kwlnItemDismissOverlay(MenuItemScene *scene) {
    if (scene->overlayFlags & 1) {
        func_0024DDC0(1);
        func_0024DA58(1);
        scene->overlayFlags &= ~1;
        return 1;
    }
    return 0;
}

/* Record the first visit to the item-selection scene. */
u32 func_002633D8(void) {
    s64 alreadyVisited;

    alreadyVisited = mdlFlagTest(0x911);
    if (alreadyVisited == 0) {
        mdlFlagSet(0x911);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00263148", prfCapTaskStep);

static inline s64 menuRunPanel(s32 context, u64 mode, u64 arg) {
    return func_00285670(context + 8, (s32 *)(context + 0x54), mode, arg);
}

s64 func_00263570(s32 request) {
    s32 context = func_00101A70();
    mnuRefreshPanelLayer(context);
    return menuRunPanel(context, 1, request);
}

s64 func_002635C0(s32 request) {
    s32 context = func_00101A70();
    func_0024DD78();
    return menuRunPanel(context, 2, request);
}

s32 mnuResetItemSelectionMarkers(void) {
    MenuItemScene *scene = (MenuItemScene *)func_00101A70();
    scene->resetStateA = 0;
    scene->resetStateB = 0;
    return 1;
}

u32 func_00263638(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00263148", func_00263640);

INCLUDE_ASM(const s32, "game/code_00263148", func_00263728);

INCLUDE_ASM(const s32, "game/code_00263148", func_00263838);

void mnuDrawItemPanelBackdrop(s32 scene) {
    mnuDrawBackdrop(scene + 0xd1c, 0x20);
}

INCLUDE_ASM(const s32, "game/code_00263148", func_00263A00);

INCLUDE_ASM(const s32, "game/code_00263148", func_00263B78);

extern s32 func_002624C0(s32);
extern void func_00263B78(s32, s32);

s64 func_00263C98(s32 request) {
    s32 context = func_00101A70();

    if (func_002624C0(context) != 0) {
        return 0;
    }
    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 0);
    return menuRunPanel(context, 1, request);
}

s64 func_00263D10(s32 request) {
    s32 context = func_00101A70();

    if (func_002624C0(context) != 0) {
        return 0;
    }
    func_0024DD78();
    return menuRunPanel(context, 2, request);
}

/* Bound the selection extent by the remaining capacity after five components. */
u32 mnuInitializeItemSelectionExtent(void) {
    s8 component;
    MenuItemScene *scene;
    u32 *slot;
    s8 *byteCursor;
    s32 remaining;
    s32 extent;
    s32 sum;

    scene = (MenuItemScene *)func_00101A70();
    sum = 0;
    remaining = 4;
    extent = (*(s32 **)((s32)scene + 0x98))[1] * 3;
    byteCursor = (s8 *)(**(s32 **)((s32)scene + 0x98) + 0x16);
    do {
        component = *byteCursor;
        byteCursor = byteCursor + 1;
        remaining = remaining - 1;
        sum = sum + component;
    } while (-1 < remaining);
    scene->activeSlot = 0;
    remaining = 4;
    slot = &scene->slots[4];
    if (0x1ef - sum < extent) {
        extent = 0x1ef - sum;
    }
    scene->selectedExtent = extent;
    do {
        remaining = remaining - 1;
        *slot = 0;
        slot = slot + -1;
    } while (-1 < remaining);
    if (*(s32 *)((s32)scene + 0x1578) != 0) {
        func_002830F0(*(u32 *)((s32)scene + 0xd10), 0);
    }
    return 1;
}

u32 func_00263E30(void) {
    return 1;
}

void mnuClearItemSelectionSlots(MenuItemScene *scene) {
    s32 remaining;
    u32 *slot;

    scene->activeSlot = 0;
    slot = &scene->slots[4];
    remaining = 4;
    do {
        remaining = remaining - 1;
        *slot = 0;
        slot = slot + -1;
    } while (-1 < remaining);
}

void func_00263E70(u32 unit, u32 menu) {
    ptyRecomputeMaxVitals(unit, (s32)menu + 0x3d0);
    mnuRefreshSelectedUnitPanels(unit, menu);
}

INCLUDE_SDATA(const s32, "game/code_00263148", D_003BC550);

