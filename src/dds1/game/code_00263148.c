#include "mnu.h"

extern u32 ptyBuildProfileCapSkillList(u32, s32);

extern s32 mdlFlagTest(u32);

extern s32 kwlnTaskGetUserValue();

extern void dspSetActive(s32);

extern void dspStartEntry(s32);

extern s32 ptyGetCurrentProfileRecord(void *);

extern s32 prfGetCapValue(u16);

extern s32 ptyTestProfileFlag0(void *, u16);

extern void ptyApplyProfile(void *, u16);

extern void mdlFlagSet(s32);

extern void mnuRefreshPanelLayer(s32 arg0);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern void func_0024DD78(void);
typedef struct MenuItem {
    u8 pad00[0x16];
    s8 components[5]; /* Summed when bounding the available selection extent. */
    u8 pad1B[0x3A];
    s8 selection;
} MenuItem;

/* The selected item and the multiplier used to derive its available extent. */
typedef struct MenuItemSelectionData {
    MenuItem *item;
    s32 extentFactor;
} MenuItemSelectionData;

typedef struct MenuItemScene {
    u8 pad00[4];
    u32 overlayFlags;
    u8 pad08[0x90];
    MenuItemSelectionData *selectionData;
    u8 pad9C[0x1A4];
    u32 resetStateA;
    u32 pendingSkillCount; /* Decremented as prfCapPresentMessages presents skills. */
    u8 pad248[4];
    s32 selectionApplied;
    u8 pad250[0x174];
    u32 resetStateB;
    s32 selectedExtent;
    u32 activeSlot;
    u32 slots[5];
    u8 pad3E4[0x92C];
    u32 panelGroup; /* 0xD10: passed to mnuSetPanelGroupSelection */
    u8 padD14[0x864];
    s32 unk1578; /* Nonzero enables the panel-group selection reset. */
} MenuItemScene;


/* Apply a capped selected profile and record whether the selection was applied. */
void kwlnItemApplySelection(MenuItemScene *scene) {
    MenuItem *item = scene->selectionData->item;
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

/* Build the capped skill list and cache its pending count; return 1. */
u32 mnuProcessItemSelection(u32 context) {
    u32 skillCount;
    MenuItemScene *scene;

    scene = (MenuItemScene *)context;
    skillCount = ptyBuildProfileCapSkillList((u32)scene->selectionData->item, (s32)scene + 0x4c4);
    scene->pendingSkillCount = skillCount;
    kwlnItemApplySelection(scene);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00263148", prfCapPresentMessages);

s32 kwlnItemDismissOverlay(MenuItemScene *scene) {
    if (scene->overlayFlags & 1) {
        dspSetActive(1);
        dspStartEntry(1);
        scene->overlayFlags &= ~1;
        return 1;
    }
    return 0;
}

/* Record the first visit to the item-selection scene. */
u32 mnuMarkItemSelectionSceneVisited(void) {
    s64 alreadyVisited;

    alreadyVisited = mdlFlagTest(0x911);
    if (alreadyVisited == 0) {
        mdlFlagSet(0x911);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00263148", prfCapTaskStep);

s64 func_00263570(s32 request) {
    s32 context = kwlnTaskGetUserValue();
    mnuRefreshPanelLayer(context);
    return menuRunPanel(context, 1, request);
}

s64 func_002635C0(s32 request) {
    s32 context = kwlnTaskGetUserValue();
    func_0024DD78();
    return menuRunPanel(context, 2, request);
}

/* Clear the scene's two selection-processing markers; return 1. */
s32 mnuResetItemSelectionMarkers(void) {
    MenuItemScene *scene = (MenuItemScene *)kwlnTaskGetUserValue();
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

extern s32 brsAdvanceSkillPackagePanel(s32);
extern void func_00263B78(s32, s32);

s64 mnuAdvanceSkillPackageToItemPanel(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    if (brsAdvanceSkillPackagePanel(context) != 0) {
        return 0;
    }
    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 0);
    return menuRunPanel(context, 1, request);
}

s64 mnuAdvanceSkillPanelToNextMenu(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    if (brsAdvanceSkillPackagePanel(context) != 0) {
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

    scene = (MenuItemScene *)kwlnTaskGetUserValue();
    sum = 0;
    remaining = 4;
    extent = scene->selectionData->extentFactor * 3;
    byteCursor = scene->selectionData->item->components;
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
    if (scene->unk1578 != 0) {
        mnuSetPanelGroupSelection(scene->panelGroup, 0);
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

void mnuRefreshPartyUnitVitalsPanels(u32 unit, u32 menu) {
    ptyRecomputeMaxVitals(unit, (s32)menu + 0x3d0);
    mnuRefreshSelectedUnitPanels(unit, menu);
}

INCLUDE_SDATA(const s32, "game/code_00263148", D_003BC550);

