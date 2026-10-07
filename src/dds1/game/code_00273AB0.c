#include "mnu.h"
#include "kwln.h"
#include "dat_state.h"

/* Staff callbacks receive a task handle as an integer word. Preserve the
 * native parameter widths and the short-arity task-user-value calls. */
#define MNU_STAFF_INPUT_CONFIRM 1
#define MNU_STAFF_INPUT_CANCEL 2
#define MNU_STAFF_INPUT_PREVIOUS_ROW 0x10
#define MNU_STAFF_INPUT_NEXT_ROW 0x20
#define MNU_STAFF_INPUT_PREVIOUS_PAGE 0x100
#define MNU_STAFF_INPUT_NEXT_PAGE 0x200

extern void func_0024DD78(void);

extern u32 kwlnTaskGetUserValue();

extern void func_00272778(s32);
extern void mnuCreateStaffImageSprite(s32);
extern void func_00272668(s32, s32, s32, s32, s32, s32);
extern void mnuDrawWindowContainer(s32, s32, s32, s32, s32);
extern void func_002BF4E0(s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_002723B0(s32, s32);
extern void func_00273A30(s32, s32);
extern void mnuDrawStaffPanelGridBackdrop(s32, StaffSlots *);
extern void mnuDrawStaffCampScreen(s32, s32);
extern void func_00272518(s32, s32, s32, s32, s32, s32, s32);
extern s32 mdlFlagTest(s32);
extern s32 D_003BAA9C;
extern s32 mnuMapPadMaskToFlags();
extern void mnuStepPartyPanelListFromInput();
extern void mnuClearListFlags();
extern void mnuSetPopupEntryFlagged();
extern void mnuPlayInputSound(s32, s32, u32 *);
extern void mnuSetPopupEntry(s32, s32);
extern u8 D_0037C860[];
extern char D_0037CA38[];
extern char D_0037C9AC[];

/* Prefixes of the menu list and node used to read the current page index.
 * The primary list implementation also stores its cursor at +0x1C. */
typedef struct MenuListNode {
    s32 index;
} MenuListNode;

typedef struct MenuList {
    u8 pad00[0x1C];
    MenuListNode *cursor;
} MenuList;

typedef struct StaffImageNode {
    s32 label;
    u8 pad04[0x5C];
    u32 sortKeyPrimary;
    s32 sortKeySecondary;
} StaffImageNode;

typedef struct StaffImageWindow {
    u32 flags; /* the input-sound provider tests these state bits as u32 */
    u8 pad04[0x14];
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
    u8 pad00[0x54];
    s32 popupState;
    u8 pad58[0x10];
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
    MenuList *selection; /* 0x7D8: page-selection list */
    u8 pad7DC[0x11C];
    MenuPanelGroup *panelHandle; /* 0x8F8 */
    MenuSpriteState *spriteHandle; /* 0x8FC */
    u8 pad900[0xC];
    StaffImageChoices *menu; /* 0x90C */
} StaffImageContext;

extern void mnuSelectPage(void *, u32);
extern void mnuCreateStaffBulletItemWindow();
extern void mnuReleaseStaffMenuResources();
extern void mnuSetWindowResource();
extern void mnuAttachPartyIconBundle();
extern void func_00276720();
extern void mnuReleaseStaffExtraWindow();
extern void mnuReleasePageHandlesAndClearSelection();
extern void mnuClearEntries();
extern void mnuReleasePartyIconBundles();
extern void mnuReleaseStaffMenuTextureHandles();
extern void mnuSeekListNode(s32, s32);
extern void mnuAdvanceWindowListSelection(StaffImageList *);
extern void mnuRetreatWindowListSelection(StaffImageList *);
extern void mnuClearWindowPanelTransitionFlag(StaffImageList *);
extern void func_0027C788(StaffImageList *);
extern void mnuResetListNodeFadeCounters(s32);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern void mnuRetreatListCursorDefault();
extern void mnuAdvanceListCursorDefault();
extern void mnuClearListFlagsOneAndTwo();

s32 mnuStaffImageEnterA(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;

    func_00272778(task);
    mnuCreateStaffImageSprite(5);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)menu->primaryObject, 0x53);
    if (menu->primaryObject->window->panelActive != 0) {
        func_00273A30(context, 0);
    } else {
        func_002BF4E0(0x550, 0x5D8, 0, menu->primaryObject->spriteAlpha, 1, ((StaffImageContext *)context)->spriteArg2, 0x10, 0x53);
        func_00272668(1, 0, 0, context, 1, 0x53);
    }
    func_002723B0(0, ((StaffImageContext *)context)->group);
    return menuRunPanel(context, 1, task);
}

INCLUDE_ASM(const s32, "game/code_00273AB0", mnuStaffImageExitA);


u32 func_00273C40(void) {
    return 1;
}

u32 func_00273C48(void) {
    return 1;
}

/* Once the popup is idle, confirmation captures the page-selection cursor index. */
s32 mnuPollStaffValueSelectionConfirmation(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = mnuMapPadMaskToFlags(3);
    s32 state;
    s32 window;

    state = func_00285670(context + 8, popup, 0, task);
    if (state != 0) {
        return state;
    }
    window = (s32)&((StaffImageContext *)context)->windowFlags;
    mnuStepPartyPanelListFromInput(4, window);
    if (buttons & MNU_STAFF_INPUT_CONFIRM) {
        menu->currentSelection = ((StaffImageContext *)context)->selection->cursor->index;
        mnuSetPopupEntryFlagged(popup, D_0037CA38);
    }
    if (buttons & MNU_STAFF_INPUT_CANCEL) {
        mnuClearListFlags(0, window);
        mnuSetPopupEntryFlagged(popup, D_0037C9AC);
    }
    mnuPlayInputSound(0, buttons, 0);
    return 0;
}

s32 mnuPrepareStaffImageAndSelectionLabel(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    func_00272778(task);
    mnuCreateStaffImageSprite(7);
    func_00272668(1, ((StaffImageContext *)context)->activeWindow->window->selectedNode->label, (s32)D_0037C860, context, 1, 0x53);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)((StaffImageContext *)context)->activeWindow, 0x53);
    func_002723B0(0, ((StaffImageContext *)context)->group);
    return menuRunPanel(context, 1, task);
}

/* Run the label-image state's teardown phase and return its scheduler word. */
s32 mnuExitStaffImageAndSelectionLabel(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel(context, 2, task);
}

s32 mnuHandleSecondaryStaffObjectInput(s32 task) {
    StaffImageContext *context = (StaffImageContext *)kwlnTaskGetUserValue((KwlnTask *)task);
    s32 *popup = &context->popupState;
    StaffImageChoices *menu = context->menu;
    s32 buttons = mnuMapPadMaskToFlags(0x33);
    s32 state;
    StaffImageList *window;

    state = func_00285670((s32)context + 8, popup, 0, task);
    if (state != 0) {
        return state;
    }
    if (buttons & MNU_STAFF_INPUT_CONFIRM) {
        buttons = 0;
    }
    if (buttons & MNU_STAFF_INPUT_CANCEL) {
        mnuSetPopupEntry((s32)popup, (s32)D_0037C9AC);
    }
    window = menu->secondaryObject;
    if (window != NULL) {
        if (!(buttons & 0x300000)) {
            func_0027C788(window);
        }
        if (buttons & MNU_STAFF_INPUT_PREVIOUS_ROW) {
            mnuRetreatWindowListSelection(window);
        }
        if (buttons & MNU_STAFF_INPUT_NEXT_ROW) {
            mnuAdvanceWindowListSelection(window);
        }
        mnuClearWindowPanelTransitionFlag(window);
        mnuPlayInputSound(0, buttons, &window->window->flags);
    }
    return 0;
}

s32 mnuStaffImageEnterD(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;

    func_00272778(task);
    mnuCreateStaffImageSprite(9);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)menu->secondaryObject, 0x53);
    if (menu->secondaryObject->window->panelActive != 0) {
        func_00273A30(context, 1);
    } else {
        func_002BF4E0(0x550, 0x5D8, 0, menu->secondaryObject->spriteAlpha, 1, ((StaffImageContext *)context)->spriteArg2, 0x10, 0x53);
        func_00272668(1, 0, 0, context, 1, 0x53);
    }
    func_002723B0(2, ((StaffImageContext *)context)->group);
    return menuRunPanel(context, 1, task);
}

/* Run the secondary-image state's teardown phase and return its scheduler word. */
s32 mnuStaffImageExitD(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel(context, 2, task);
}

u32 func_00274040(void) {
    return 1;
}

u32 func_00274048(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274050);

s32 mnuStaffImageEnterB(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;

    func_00272778(task);
    mnuCreateStaffImageSprite(6);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)menu->primaryObject, 0x53);
    if (menu->primaryObject->window->panelActive != 0) {
        func_00273A30(context, 0);
    } else {
        func_002BF4E0(0x550, 0x5D8, 0, menu->primaryObject->spriteAlpha, 1, ((StaffImageContext *)context)->spriteArg2, 0x10, 0x53);
        func_00272668(1, 0, 0, context, 1, 0x53);
    }
    func_002723B0(0, ((StaffImageContext *)context)->group);
    return menuRunPanel(context, 1, task);
}

/* Run the alternate primary-image state's teardown phase and return its scheduler word. */
s32 mnuStaffImageExitB(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel(context, 2, task);
}

/* Build the staff value page for the current page-selection cursor. */
s32 mnuInitializeStaffValuePage(s32 unused) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;
    s32 index = ((StaffImageContext *)context)->selection->cursor->index;

    mnuSelectPage(context + 0x15C, index);
    mnuCreateStaffBulletItemWindow(context);
    mnuReleaseStaffMenuResources(context + 0x60);
    mnuSetWindowResource(index, context + 0x15C, ((StaffImageContext *)context)->spriteScene, ((StaffImageContext *)context)->windowParam);
    mnuAttachPartyIconBundle(index, context + 0x15C, ((StaffImageContext *)context)->spriteScene);
    ((StaffImageContext *)context)->panelHandle = mnuCreatePanelGroup(((StaffImageContext *)context)->spriteScene);
    ((StaffImageContext *)context)->spriteHandle =
        mnuCreateSpriteState((struct EffectSlotSet *)((StaffImageContext *)context)->spriteArg0,
                             (struct EffectSlotSet *)((StaffImageContext *)context)->spriteArg1,
                             (struct EffectSlotSet *)((StaffImageContext *)context)->spriteScene);
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
        mnuDestroyPanelGroup(((StaffImageContext *)context)->panelHandle);
        ((StaffImageContext *)context)->panelHandle = 0;
    }
    if (((StaffImageContext *)context)->spriteHandle != 0) {
        mnuFreeSpriteStateWork(((StaffImageContext *)context)->spriteHandle);
        ((StaffImageContext *)context)->spriteHandle = 0;
    }
    func_00283BF0(context + 0x914, 0);
    mnuReleaseStaffMenuTextureHandles(context + 0x60);
    return 1;
}

extern u16 mnuGetPartyEntryMenuValue(DatPartyRecord *);
extern void evtCopyEntryStringToActiveWindow(s32, void *);
extern void dspStartEntry(s32);
extern void func_00283BF0(s32, s32);
extern void ptyAdjustItemQuantity(s32, s32);
extern u8 *D_003BAA70;
extern u8 *D_003BAA84;


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
void mnuSwapEquippedBullet(s32 scene, DatPartyRecord *unit, s32 itemId) {
    MnuEquipContext *equipContext = ((MnuEquipScene *)scene)->context;
    s32 equipped = mnuGetPartyEntryMenuValue(unit);

    func_00283BF0(scene + 0x914, 1);
    if (equipped != itemId) {
        /* Actor names use 17-byte records; item names use 25-byte records. */
        evtCopyEntryStringToActiveWindow(0, D_003BAA70 + unit->unitId * 17);
        evtCopyEntryStringToActiveWindow(1, D_003BAA84 + equipped * 25);
        evtCopyEntryStringToActiveWindow(2, D_003BAA84 + itemId * 25);
        dspStartEntry(0);
        if (equipped != 0) {
            ptyAdjustItemQuantity(equipped, 1);
        }
        ptyAdjustItemQuantity(itemId, -1);
        equipContext->previousItem = equipped;
        equipContext->selectedItem = itemId;
    } else {
        evtCopyEntryStringToActiveWindow(0, D_003BAA84 + equipped * 25);
        dspStartEntry(1);
        equipContext->previousItem = 0;
        equipContext->selectedItem = 0;
    }
}

/* Change pages only once per input sample, then restore the saved node and
 * row count on the new page. Returns one when the page changed, zero otherwise. */
s32 mnuHandleStaffValuePageInput(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;
    s32 pageChanged = 0;
    u32 buttons = mnuMapPadMaskToFlags(0x300);
    StaffImageWindow *window = menu->list->window;
    s32 *cursor = window->cursor;
    s32 savedNodeIndex;
    s32 savedRowCount;
    s32 i;

    if (cursor != 0) {
        savedNodeIndex = *cursor;
        savedRowCount = window->rowCount;
    } else {
        savedNodeIndex = 0;
        savedRowCount = 0;
    }
    if (buttons & MNU_STAFF_INPUT_PREVIOUS_PAGE) {
        pageChanged = 1;
        mnuReleaseStaffValuePageResources(task);
        mnuRetreatListCursorDefault(((StaffImageContext *)context)->selection);
    }
    if (buttons & MNU_STAFF_INPUT_NEXT_PAGE && pageChanged == 0) {
        pageChanged = 1;
        mnuReleaseStaffValuePageResources(task);
        mnuAdvanceListCursorDefault(((StaffImageContext *)context)->selection);
    }
    mnuClearListFlagsOneAndTwo(((StaffImageContext *)context)->selection);
    if (pageChanged == 0) {
        return 0;
    }
    mnuInitializeStaffValuePage(task);
    if (savedNodeIndex != 0 || savedRowCount != 0) {
        mnuSeekListNode(savedNodeIndex, (s32)menu->list->window);
        if (savedRowCount > 0) {
            for (i = savedRowCount; i != 0; i--) {
                mnuAdvanceWindowListSelection(menu->list);
            }
        }
        mnuResetListNodeFadeCounters((s32)menu->list->window);
    }
    sndSetSequenceVolumePan(4, 0x7F, 0x3F);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274768);

/* Draw the selected party member's value page and advance its primary dispatch. */
s32 mnuDrawStaffPartyValuePage(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;
    s32 index = ((StaffImageContext *)context)->selection->cursor->index;
    s32 partyEntry = (s32)&datGameState->party[index];
    StaffImageList *list;
    StaffImageWindow *window;

    mnuDrawStaffPanelGridBackdrop(1, (StaffSlots *)(context + 0x60));
    mnuDrawStaffCampScreen(1, task);
    mnuCreateStaffImageSprite(8);
    func_00283110(0xEB0, 0x518, 0, (void *)partyEntry,
                  ((StaffImageContext *)context)->panelHandle, 0x53);
    if (mdlFlagTest(0x901) != 0) {
        func_002833B0(0, 0, 0, partyEntry,
                      (s32)((StaffImageContext *)context)->spriteHandle, 0x53);
    }
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)menu->list, 0x53);
    list = menu->list;
    window = list->window;
    if (window->panelActive != 0) {
        StaffImageNode *node = window->selectedNode;
        s32 selectedLabel = node->sortKeySecondary;

        if (node->sortKeyPrimary != 0) {
            func_00272518(1, selectedLabel, D_003BAA9C, context, 1, 1, 0x53);
        } else {
            func_00272668(1, 0, 0, context, 1, 0x53);
        }
    } else {
        func_002BF4E0(0x550, 0x5D8, 0, list->spriteAlpha, 1,
                      ((StaffImageContext *)context)->spriteArg2, 0x10, 0x53);
        func_00272668(1, 0, 0, context, 1, 0x53);
    }
    func_002723B0(1, ((StaffImageContext *)context)->group);
    return menuRunPanel(context, 1, task);
}

/* Request message-window mode one before the value-page teardown phase. */
s32 mnuExitStaffValuePage(s32 task) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, task);
}

u32 func_00274B78(void) {
    return 1;
}
