#include "mnu.h"

extern void func_0024DD78(void);

extern s32 kwlnTaskGetUserValue();

extern void func_00272778(s32);
extern void mnuCreateStaffImageSprite(s32);
extern void func_00272668(s32, s32, s32, s32, s32, s32);
extern void mnuDrawWindowContainer(s32, s32, s32, s32, s32);
extern void func_002BF4E0(s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_002723B0(s32, s32);
extern void func_00273A30(s32, s32);
extern u32 mnuMapPadMaskToFlags();
extern void mnuUpdateWindowListFromInput();
extern void mnuClearListFlags();
extern void mnuSetPopupEntryFlagged();
extern void mnuPlayInputSound();
extern u8 D_0037C860[];
extern char D_0037CA38[];
extern char D_0037C9AC[];

typedef struct StaffImageNode {
    s32 label;
} StaffImageNode;

typedef struct StaffImageWindow {
    u8 pad00[0x18];
    s32 *cursor; /* 0x18 */
    StaffImageNode *selectedNode; /* 0x1C */
    s32 panelActive; /* 0x20: selects the alternate panel drawing path */
    s32 rowCount; /* 0x24 */
} StaffImageWindow;

typedef struct StaffImageList {
    u8 pad00[0x14];
    StaffImageWindow *window; /* 0x14 */
    u8 pad18[0x70];
    s32 spriteAlpha; /* 0x88 */
} StaffImageList;

/* Staff menu state: selected objects and the current selection. */
typedef struct StaffImageChoices {
    u8 pad00[8];
    StaffImageList *primaryObject;   /* 0x08 */
    StaffImageList *secondaryObject; /* 0x0C */
    StaffImageList *list; /* 0x10 */
    s32 currentSelection; /* 0x14 */
    u8 pad18[0xC];
    s32 listState; /* 0x24 */
} StaffImageChoices;

typedef struct StaffImageContext {
    u8 pad00[0x68];
    s32 spriteArg1; /* 0x68 */
    u8 pad6C[8];
    s32 spriteArg0; /* 0x74 */
    s32 group; /* 0x78 */
    s32 spriteScene; /* 0x7C */
    s32 windowParam; /* 0x80 */
    u8 pad84[0x54];
    s32 spriteArg2; /* 0xD8 */
    u8 padDC[0x4C];
    StaffImageList *activeWindow; /* 0x128 */
    u8 pad12C[0x30];
    u32 windowFlags; /* 0x15C */
    u8 pad160[0x678];
    s32 selection; /* 0x7D8 */
    u8 pad7DC[0x11C];
    void *panelHandle; /* 0x8F8 */
    void *spriteHandle; /* 0x8FC */
    u8 pad900[0xC];
    StaffImageChoices *menu; /* 0x90C */
} StaffImageContext;

extern void mnuSelectPage(void *, u32);
extern void func_002730A0();
extern void mnuReleaseStaffMenuResources();
extern void mnuSetWindowResource();
extern void mnuAttachPartyIconBundle();
extern void *mnuCreatePanelGroup();
extern void *mnuCreateSpriteState();
extern void func_00276720();
extern void mnuReleaseStaffExtraWindow();
extern void mnuReleasePageHandlesAndClearSelection();
extern void mnuClearEntries();
extern void mnuReleasePartyIconBundles();
extern void mnuDestroyPanelGroup();
extern void mnuFreeSpriteStateWork();
extern void mnuReleaseStaffMenuTextureHandles();
extern void mnuSeekListNode(s32, s32);
extern void mnuAdvanceWindowListSelection(s32);
extern void mnuResetListNodeFadeCounters(s32);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern void mnuRetreatListCursorDefault();
extern void mnuAdvanceListCursorDefault();
extern void mnuClearListFlagsOneAndTwo();

s64 mnuStaffImageEnterA(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;

    func_00272778(callback);
    mnuCreateStaffImageSprite(5);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)menu->primaryObject, 0x53);
    if (menu->primaryObject->window->panelActive != 0) {
        func_00273A30(context, 0);
    } else {
        func_002BF4E0(0x550, 0x5D8, 0, menu->primaryObject->spriteAlpha, 1, ((StaffImageContext *)context)->spriteArg2, 0x10, 0x53);
        func_00272668(1, 0, 0, context, 1, 0x53);
    }
    func_002723B0(0, ((StaffImageContext *)context)->group);
    return menuRunPanel(context, 1, callback);
}

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00273B98);


u32 func_00273C40(void) {
    return 1;
}

u32 func_00273C48(void) {
    return 1;
}

s64 mnuPollStaffValueSelectionConfirmation(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = mnuMapPadMaskToFlags(3);
    s64 state;
    s32 window;

    state = func_00285670(context + 8, popup, 0, callback);
    if (state != 0) {
        return state;
    }
    window = (s32)&((StaffImageContext *)context)->windowFlags;
    mnuUpdateWindowListFromInput(4, window);
    if (buttons & 1) {
        menu->currentSelection = **(s32 **)(((StaffImageContext *)context)->selection + 0x1C);
        mnuSetPopupEntryFlagged(popup, D_0037CA38);
    }
    if (buttons & 2) {
        mnuClearListFlags(0, window);
        mnuSetPopupEntryFlagged(popup, D_0037C9AC);
    }
    mnuPlayInputSound(0, buttons, 0);
    return 0;
}

s64 mnuPrepareStaffImageAndSelectionLabel(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_00272778(callback);
    mnuCreateStaffImageSprite(7);
    func_00272668(1, ((StaffImageContext *)context)->activeWindow->window->selectedNode->label, (s32)D_0037C860, context, 1, 0x53);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)((StaffImageContext *)context)->activeWindow, 0x53);
    func_002723B0(0, ((StaffImageContext *)context)->group);
    return menuRunPanel(context, 1, callback);
}

s64 func_00273DE8(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel(context, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00273E20);

s64 mnuRunStaffImagePanelOnSecondaryObject(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;

    func_00272778(callback);
    mnuCreateStaffImageSprite(9);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)menu->secondaryObject, 0x53);
    if (menu->secondaryObject->window->panelActive != 0) {
        func_00273A30(context, 1);
    } else {
        func_002BF4E0(0x550, 0x5D8, 0, menu->secondaryObject->spriteAlpha, 1, ((StaffImageContext *)context)->spriteArg2, 0x10, 0x53);
        func_00272668(1, 0, 0, context, 1, 0x53);
    }
    func_002723B0(2, ((StaffImageContext *)context)->group);
    return menuRunPanel(context, 1, callback);
}

s64 func_00274008(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel(context, 2, callback);
}

u32 func_00274040(void) {
    return 1;
}

u32 func_00274048(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274050);

s64 mnuStaffImageEnterB(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;

    func_00272778(callback);
    mnuCreateStaffImageSprite(6);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)menu->primaryObject, 0x53);
    if (menu->primaryObject->window->panelActive != 0) {
        func_00273A30(context, 0);
    } else {
        func_002BF4E0(0x550, 0x5D8, 0, menu->primaryObject->spriteAlpha, 1, ((StaffImageContext *)context)->spriteArg2, 0x10, 0x53);
        func_00272668(1, 0, 0, context, 1, 0x53);
    }
    func_002723B0(0, ((StaffImageContext *)context)->group);
    return menuRunPanel(context, 1, callback);
}

s64 func_00274310(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel(context, 2, callback);
}

s32 mnuInitializeStaffValuePage(s32 unused) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;
    s32 index = **(s32 **)(((StaffImageContext *)context)->selection + 0x1C);

    mnuSelectPage(context + 0x15C, index);
    func_002730A0(context);
    mnuReleaseStaffMenuResources(context + 0x60);
    mnuSetWindowResource(index, context + 0x15C, ((StaffImageContext *)context)->spriteScene, ((StaffImageContext *)context)->windowParam);
    mnuAttachPartyIconBundle(index, context + 0x15C, ((StaffImageContext *)context)->spriteScene);
    ((StaffImageContext *)context)->panelHandle = mnuCreatePanelGroup(((StaffImageContext *)context)->spriteScene);
    ((StaffImageContext *)context)->spriteHandle = mnuCreateSpriteState(((StaffImageContext *)context)->spriteArg0, ((StaffImageContext *)context)->spriteArg1, ((StaffImageContext *)context)->spriteScene);
    ((StaffImageContext *)context)->windowFlags |= 0x400;
    ((StaffImageContext *)context)->windowFlags &= ~0x100;
    func_00276720(context + 0x15C, 1, 0, 0);
    menu->listState = 0;
    return 1;
}

s32 mnuReleaseStaffValuePageResources(s32 unused) {
    s32 context = kwlnTaskGetUserValue();

    mnuReleaseStaffExtraWindow(context);
    func_00276720(context + 0x15C, 0, 0, 0);
    mnuReleasePageHandlesAndClearSelection(context + 0x15C);
    mnuClearEntries(context + 0x15C);
    mnuReleasePartyIconBundles(context + 0x15C);
    if (((StaffImageContext *)context)->panelHandle != 0) {
        mnuDestroyPanelGroup((s32)((StaffImageContext *)context)->panelHandle);
        ((StaffImageContext *)context)->panelHandle = 0;
    }
    if (((StaffImageContext *)context)->spriteHandle != 0) {
        mnuFreeSpriteStateWork((s32)((StaffImageContext *)context)->spriteHandle);
        ((StaffImageContext *)context)->spriteHandle = 0;
    }
    func_00283BF0(context + 0x914, 0);
    mnuReleaseStaffMenuTextureHandles(context + 0x60);
    return 1;
}

extern u16 mnuGetPartyEntryMenuValue(s32);
extern void func_0024DD90(s32, void *);
extern void dspStartEntry(s32);
extern void func_00283BF0(s32, s32);
extern void func_00119900(s32, s32);
extern u8 *D_003BAA70;
extern u8 *D_003BAA84;

typedef struct MnuEquipUnit {
    u8 pad00[4];
    u16 unitId;              /* 0x04 */
} MnuEquipUnit;

typedef struct MnuEquipContext {
    u8 pad00[0x1C];
    s32 previousItem;        /* 0x1C */
    s32 selectedItem;        /* 0x20 */
} MnuEquipContext;

typedef struct MnuEquipScene {
    u8 pad00[0x90C];
    MnuEquipContext *context; /* 0x90C */
} MnuEquipScene;

/* Swap the equipped bullet item: update the actor/old/new message tokens,
 * adjust inventory counts, and latch the old/new IDs in the menu context. */
void mnuSwapEquippedBullet(s32 scene, u8 *unit, s32 itemId) {
    MnuEquipContext *equipContext = ((MnuEquipScene *)scene)->context;
    s32 equipped = mnuGetPartyEntryMenuValue((s32)unit);

    func_00283BF0(scene + 0x914, 1);
    if (equipped != itemId) {
        /* Actor names use 17-byte records; item names use 25-byte records. */
        func_0024DD90(0, D_003BAA70 + ((MnuEquipUnit *)unit)->unitId * 17);
        func_0024DD90(1, D_003BAA84 + equipped * 25);
        func_0024DD90(2, D_003BAA84 + itemId * 25);
        dspStartEntry(0);
        if (equipped != 0) {
            func_00119900(equipped, 1);
        }
        func_00119900(itemId, -1);
        equipContext->previousItem = equipped;
        equipContext->selectedItem = itemId;
    } else {
        func_0024DD90(0, D_003BAA84 + equipped * 25);
        dspStartEntry(1);
        equipContext->previousItem = 0;
        equipContext->selectedItem = 0;
    }
}

s32 mnuHandleStaffValuePageInput(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;
    s32 changed = 0;
    u32 buttons = mnuMapPadMaskToFlags(0x300);
    StaffImageWindow *window = menu->list->window;
    s32 *node = window->cursor;
    s32 first;
    s32 count;
    s32 i;

    if (node != 0) {
        first = *node;
        count = window->rowCount;
    } else {
        first = 0;
        count = 0;
    }
    if (buttons & 0x100) {
        changed = 1;
        mnuReleaseStaffValuePageResources(callback);
        mnuRetreatListCursorDefault(((StaffImageContext *)context)->selection);
    }
    if (buttons & 0x200 && changed == 0) {
        changed = 1;
        mnuReleaseStaffValuePageResources(callback);
        mnuAdvanceListCursorDefault(((StaffImageContext *)context)->selection);
    }
    mnuClearListFlagsOneAndTwo(((StaffImageContext *)context)->selection);
    if (changed == 0) {
        return 0;
    }
    mnuInitializeStaffValuePage(callback);
    if (first != 0 || count != 0) {
        mnuSeekListNode(first, (s32)menu->list->window);
        if (count > 0) {
            for (i = count; i != 0; i--) {
                mnuAdvanceWindowListSelection((s32)menu->list);
            }
        }
        mnuResetListNodeFadeCounters((s32)menu->list->window);
    }
    sndSetSequenceVolumePan(4, 0x7F, 0x3F);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274768);

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274978);

s64 func_00274B30(s32 selection) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, selection);
}

u32 func_00274B78(void) {
    return 1;
}
