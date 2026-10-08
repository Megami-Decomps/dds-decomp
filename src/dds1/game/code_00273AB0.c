#include "mnu.h"
#include "mnu_shop.h"
#include "mnu_list.h"
#include "kwln.h"
#include "dat_state.h"

/* Legacy staff callbacks retain their word-address interfaces; the exit
 * callback forwards its task pointer to the task-user-value provider. */
#define MNU_STAFF_INPUT_CONFIRM 1
#define MNU_STAFF_INPUT_CANCEL 2
#define MNU_STAFF_INPUT_PREVIOUS_ROW 0x10
#define MNU_STAFF_INPUT_NEXT_ROW 0x20
#define MNU_STAFF_INPUT_PREVIOUS_PAGE 0x100
#define MNU_STAFF_INPUT_NEXT_PAGE 0x200

extern void func_0024DD78(void);
extern s32 evtGetMessageWindowControlState(void);

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
extern void mnuStepPartyPanelListFromInput(s32 mode, MenuPageWindow *window);
extern void mnuClearListFlags(s32 which, MenuPageWindow *window);
extern void func_00283BF0(u32 *out, u32 value);
extern u32 mnuSetPartyEntryMenuValue(DatPartyRecord *, u32);
extern void mnuSetPopupEntryFlagged();
extern void mnuPlayInputSound(s32, s32, u32 *);
extern void mnuSetPopupEntry(s32, s32);
extern u8 D_0037C860[];
extern char D_0037CA38[];
extern char D_0037C9AC[];
extern char D_0037C9E4[];

/* The allocated staff resource record begins with three owned windows; the
 * display callbacks use the following selection and input-state words. */
typedef struct StaffImageChoices {
    u32 allocation;
    u32 pad04;
    MenuWindowContainer *primaryObject;
    MenuWindowContainer *secondaryObject;
    MenuWindowContainer *list;
    s32 currentSelection; /* 0x14 */
    s32 inputState; /* 0x18: reset before input-state polling; set before cancel popup */
    s32 itemToInsert; /* 0x1C: func_00273220 insertion key */
    s32 inventoryItem; /* 0x20: func_00273220 inventory lookup */
    s32 listState; /* 0x24 */
    s32 pendingItem; /* 0x28: successful staff-item use, cleared by exit */
} StaffImageChoices;
typedef char StaffImageChoices_size_check[
    sizeof(StaffImageChoices) == 0x2C ? 1 : -1];

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
    MenuWindowContainer *activeWindow; /* 0x128 */
    u8 pad12C[0x30];
    MenuPageWindow pageWindow; /* 0x15C: primary staff-page owner */
    u8 padPageEnd[0x10C];
    MenuPanelGroup *panelHandle; /* 0x8F8 */
    MenuSpriteState *spriteHandle; /* 0x8FC */
    u8 pad900[0xC];
    StaffImageChoices *menu; /* 0x90C */
    s32 displayMode; /* 0x910 */
    MenuGradientFade transitionFade; /* 0x914 */
    u8 pad920[4];
} StaffImageContext;
typedef char StaffImageContext_size_check[
    sizeof(StaffImageContext) == 0x924 ? 1 : -1];
typedef char StaffImageContext_page_list_check[
    (u32)&((StaffImageContext *)0)->pageWindow.lists[0] == 0x7D8 ? 1 : -1];
extern s32 func_00273220(s32 itemToInsert, s32 inventoryItem, StaffImageContext *context);

extern void mnuSelectPage(MenuPageWindow *, s32);
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
extern void mnuAdvanceWindowListSelection(MenuWindowContainer *);
extern void mnuRetreatWindowListSelection(MenuWindowContainer *);
extern void mnuClearWindowPanelTransitionFlag(MenuWindowContainer *);
extern void func_0027C788(MenuWindowContainer *);
extern void mnuResetListNodeFadeCounters(s32);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern void mnuClearListFlagsOneAndTwo();

s32 mnuStaffImageEnterA(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;

    func_00272778(task);
    mnuCreateStaffImageSprite(5);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)menu->primaryObject, 0x53);
    if (menu->primaryObject->list->count != 0) {
        func_00273A30(context, 0);
    } else {
        func_002BF4E0(0x550, 0x5D8, 0, menu->primaryObject->fade, 1, ((StaffImageContext *)context)->spriteArg2, 0x10, 0x53);
        func_00272668(1, 0, 0, context, 1, 0x53);
    }
    func_002723B0(0, ((StaffImageContext *)context)->group);
    return menuRunPanel((void *)context, 1, (void *)task);
}

extern s32 mnuIsStaffWindowReadyForItem(s32 itemId, s32 context);

s32 mnuStaffImageExitA(KwlnTask *task) {
    StaffImageContext *context = (StaffImageContext *)kwlnTaskGetUserValue(task);
    StaffImageChoices *menu = context->menu;
    s32 item;

    func_0024DD78();
    if (evtGetMessageWindowControlState() == 0) {
        item = menu->pendingItem;
        if (item != 0) {
            menu->pendingItem = 0;
            if (mnuIsStaffWindowReadyForItem((u16)item, (s32)context) == 0) {
                mnuSetPopupEntry((s32)&context->popupState, (s32)D_0037C9AC);
            }
        }
    }
    return menuRunPanel(context, 2, task);
}


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
    MenuPageWindow *window;

    state = menuRunPanel((void *)context, 0, (void *)task);
    if (state != 0) {
        return state;
    }
    window = &((StaffImageContext *)context)->pageWindow;
    mnuStepPartyPanelListFromInput(4, window);
    if (buttons & MNU_STAFF_INPUT_CONFIRM) {
        menu->currentSelection = ((StaffImageContext *)context)->pageWindow.lists[0]->cursor->index;
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
    func_00272668(1, ((StaffImageContext *)context)->activeWindow->list->cursor->index, (s32)D_0037C860, context, 1, 0x53);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)((StaffImageContext *)context)->activeWindow, 0x53);
    func_002723B0(0, ((StaffImageContext *)context)->group);
    return menuRunPanel((void *)context, 1, (void *)task);
}

/* Run the label-image state's teardown phase and return its scheduler word. */
s32 mnuExitStaffImageAndSelectionLabel(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel((void *)context, 2, (void *)task);
}

s32 mnuHandleSecondaryStaffObjectInput(s32 task) {
    StaffImageContext *context = (StaffImageContext *)kwlnTaskGetUserValue((KwlnTask *)task);
    s32 *popup = &context->popupState;
    StaffImageChoices *menu = context->menu;
    s32 buttons = mnuMapPadMaskToFlags(0x33);
    s32 state;
    MenuWindowContainer *window;

    state = menuRunPanel(context, 0, (void *)task);
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
        mnuPlayInputSound(0, buttons, &window->list->stateFlags);
    }
    return 0;
}

s32 mnuStaffImageEnterD(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;

    func_00272778(task);
    mnuCreateStaffImageSprite(9);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)menu->secondaryObject, 0x53);
    if (menu->secondaryObject->list->count != 0) {
        func_00273A30(context, 1);
    } else {
        func_002BF4E0(0x550, 0x5D8, 0, menu->secondaryObject->fade, 1, ((StaffImageContext *)context)->spriteArg2, 0x10, 0x53);
        func_00272668(1, 0, 0, context, 1, 0x53);
    }
    func_002723B0(2, ((StaffImageContext *)context)->group);
    return menuRunPanel((void *)context, 1, (void *)task);
}

/* Run the secondary-image state's teardown phase and return its scheduler word. */
s32 mnuStaffImageExitD(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel((void *)context, 2, (void *)task);
}

u32 func_00274040(void) {
    return 1;
}

u32 func_00274048(void) {
    return 1;
}

extern s32 evtGetIndexedEventRecordId(s32);
extern s32 mnuGetAbilityTargetCategory(u16);
extern s32 mnuUseStaffItem(s32, s32);
extern s32 mnuIsStaffWindowReadyForItem(s32, s32);
extern char D_0037C9C8[];

s32 func_00274050(KwlnTask *task) {
    u32 userValue = kwlnTaskGetUserValue(task);
    StaffImageContext *staff = (StaffImageContext *)userValue;
    StaffImageChoices *menu = staff->menu;
    u32 buttons = mnuMapPadMaskToFlags(3);
    s32 state;
    s32 itemId = 0;
    s32 itemCountIsZero = 0;

    state = menuRunPanel(staff, 0, task);
    if (state != 0) {
        return state;
    }
    mnuStepPartyPanelListFromInput(4, &staff->pageWindow);
    if (menu->primaryObject->list->count != 0) {
        itemId = menu->primaryObject->list->cursor->sortKeySecondary;
    }
    if (mnuGetAbilityTargetCategory((u16)evtGetIndexedEventRecordId(itemId)) == 2) {
        staff->pageWindow.flags |= 0x10;
    }
    if (mnuGetAbilityTargetCategory((u16)evtGetIndexedEventRecordId(itemId)) == 3) {
        staff->pageWindow.flags |= 0x20;
    }
    if (buttons & MNU_STAFF_INPUT_CONFIRM) {
        buttons = mnuUseStaffItem(itemId, userValue) == 0 ? 0x8000 : 0;
        menu->primaryObject->list->cursor->sortKeyPrimary = datGameState->inventory.counts[itemId];
        if (mnuIsStaffWindowReadyForItem((u16)itemId, userValue) == 0) {
            mnuSetPopupEntryFlagged(&staff->popupState, D_0037C9AC);
            mnuClearListFlags(0, &staff->pageWindow);
        } else {
            itemCountIsZero = datGameState->inventory.counts[itemId] == 0;
        }
    }
    if ((buttons & MNU_STAFF_INPUT_CANCEL) != 0 || itemCountIsZero != 0) {
        mnuSetPopupEntryFlagged(&staff->popupState, D_0037C9C8);
        mnuClearListFlags(0, &staff->pageWindow);
    }
    mnuPlayInputSound(0, buttons, 0);
    return 0;
}

s32 mnuStaffImageEnterB(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;

    func_00272778(task);
    mnuCreateStaffImageSprite(6);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)menu->primaryObject, 0x53);
    if (menu->primaryObject->list->count != 0) {
        func_00273A30(context, 0);
    } else {
        func_002BF4E0(0x550, 0x5D8, 0, menu->primaryObject->fade, 1, ((StaffImageContext *)context)->spriteArg2, 0x10, 0x53);
        func_00272668(1, 0, 0, context, 1, 0x53);
    }
    func_002723B0(0, ((StaffImageContext *)context)->group);
    return menuRunPanel((void *)context, 1, (void *)task);
}

/* Run the alternate primary-image state's teardown phase and return its scheduler word. */
s32 mnuStaffImageExitB(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel((void *)context, 2, (void *)task);
}

/* Build the staff value page for the current page-selection cursor. */
s32 mnuInitializeStaffValuePage(s32 unused) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;
    s32 index = ((StaffImageContext *)context)->pageWindow.lists[0]->cursor->index;

    mnuSelectPage(&((StaffImageContext *)context)->pageWindow, index);
    mnuCreateStaffBulletItemWindow(context);
    mnuReleaseStaffMenuResources(context + 0x60);
    mnuSetWindowResource(index, context + 0x15C, ((StaffImageContext *)context)->spriteScene, ((StaffImageContext *)context)->windowParam);
    mnuAttachPartyIconBundle(index, context + 0x15C, ((StaffImageContext *)context)->spriteScene);
    ((StaffImageContext *)context)->panelHandle = mnuCreatePanelGroup(((StaffImageContext *)context)->spriteScene);
    ((StaffImageContext *)context)->spriteHandle =
        mnuCreateSpriteState((struct EffectSlotSet *)((StaffImageContext *)context)->spriteArg0,
                             (struct EffectSlotSet *)((StaffImageContext *)context)->spriteArg1,
                             (struct EffectSlotSet *)((StaffImageContext *)context)->spriteScene);
    ((StaffImageContext *)context)->pageWindow.flags |= 0x400;
    ((StaffImageContext *)context)->pageWindow.flags &= ~0x100;
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
    func_00283BF0((u32 *)(context + 0x914), 0);
    mnuReleaseStaffMenuTextureHandles(context + 0x60);
    return 1;
}

extern u16 mnuGetPartyEntryMenuValue(DatPartyRecord *);
extern void evtCopyEntryStringToActiveWindow(s32, void *);
extern void dspStartEntry(s32);
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

    func_00283BF0((u32 *)(scene + 0x914), 1);
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
    struct MenuList *window = menu->list->list;
    struct MenuListNode *head = window->head;
    s32 savedNodeIndex;
    s32 savedRowCount;
    s32 i;

    if (head != 0) {
        savedNodeIndex = head->index;
        savedRowCount = window->windowOffset;
    } else {
        savedNodeIndex = 0;
        savedRowCount = 0;
    }
    if (buttons & MNU_STAFF_INPUT_PREVIOUS_PAGE) {
        pageChanged = 1;
        mnuReleaseStaffValuePageResources(task);
        mnuRetreatListCursorDefault(((StaffImageContext *)context)->pageWindow.lists[0]);
    }
    if (buttons & MNU_STAFF_INPUT_NEXT_PAGE && pageChanged == 0) {
        pageChanged = 1;
        mnuReleaseStaffValuePageResources(task);
        mnuAdvanceListCursorDefault(((StaffImageContext *)context)->pageWindow.lists[0]);
    }
    mnuClearListFlagsOneAndTwo(((StaffImageContext *)context)->pageWindow.lists[0]);
    if (pageChanged == 0) {
        return 0;
    }
    mnuInitializeStaffValuePage(task);
    if (savedNodeIndex != 0 || savedRowCount != 0) {
        mnuSeekListNode(savedNodeIndex, (s32)menu->list->list);
        if (savedRowCount > 0) {
            for (i = savedRowCount; i != 0; i--) {
                mnuAdvanceWindowListSelection(menu->list);
            }
        }
        mnuResetListNodeFadeCounters((s32)menu->list->list);
    }
    sndSetSequenceVolumePan(4, 0x7F, 0x3F);
    return 1;
}

s32 func_00274768(s32 task) {
    StaffImageContext *context = (StaffImageContext *)kwlnTaskGetUserValue();
    StaffImageChoices *menu = context->menu;
    u32 buttons = mnuMapPadMaskToFlags(0x33);
    DatPartyRecord *party = &datGameState->party[context->pageWindow.lists[0]->cursor->index];
    s32 state;
    MenuWindowContainer *list;

    state = menuRunPanel(context, 0, (void *)task);
    if (state != 0) {
        return state;
    }
    menu->inputState = 0;
    if (evtGetMessageWindowControlState() != 0) {
        return 0;
    }
    func_00283BF0((u32 *)&context->transitionFade, 0);
    if (menu->listState == 0) {
        if (mnuHandleStaffValuePageInput(task) != 0) {
            return 0;
        }

        list = menu->list;
        if (!(buttons & 0x300000)) {
            func_0027C788(list);
        }
        if (buttons & MNU_STAFF_INPUT_PREVIOUS_ROW) {
            mnuRetreatWindowListSelection(list);
        }
        if (buttons & MNU_STAFF_INPUT_NEXT_ROW) {
            mnuAdvanceWindowListSelection(list);
        }
        mnuClearWindowPanelTransitionFlag(list);

        if (buttons & MNU_STAFF_INPUT_CONFIRM) {
            struct MenuList *items = menu->list->list;

            if (items->count != 0) {
                s32 bulletId = items->cursor->sortKeySecondary;
                struct MenuList *selectedList;
                s32 inventoryCount;

                mnuSwapEquippedBullet((s32)context, party, bulletId);
                mnuSetPartyEntryMenuValue(party, bulletId);
                selectedList = menu->list->list;
                inventoryCount = datGameState->inventory.counts[bulletId];
                selectedList->cursor->sortKeyPrimary = inventoryCount;
                menu->listState = 1;
            } else {
                buttons = 0;
            }
        }
        if (buttons & MNU_STAFF_INPUT_CANCEL) {
            menu->inputState = 1;
            mnuSetPopupEntryFlagged((s32)&context->popupState, (s32)D_0037C9E4);
        }
        mnuPlayInputSound(0, buttons, (u32 *)list->list);
    } else {
        if (func_00273220(menu->itemToInsert, menu->inventoryItem, context) == 0) {
            mnuClearListFlags(0, &context->pageWindow);
            mnuSetPopupEntryFlagged((s32)&context->popupState, (s32)D_0037C9AC);
        } else {
            menu->listState = 0;
        }
    }
    return 0;
}

/* Draw the selected party member's value page and advance its primary dispatch. */
s32 mnuDrawStaffPartyValuePage(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    StaffImageChoices *menu = ((StaffImageContext *)context)->menu;
    s32 index = ((StaffImageContext *)context)->pageWindow.lists[0]->cursor->index;
    s32 partyEntry = (s32)&datGameState->party[index];
    MenuWindowContainer *list;
    struct MenuList *window;

    mnuDrawStaffPanelGridBackdrop(1, (StaffSlots *)(context + 0x60));
    mnuDrawStaffCampScreen(1, task);
    mnuCreateStaffImageSprite(8);
    mnuDrawAndAdvancePanelGroup(0xEB0, 0x518, 0, (void *)partyEntry,
                  ((StaffImageContext *)context)->panelHandle, 0x53);
    if (mdlFlagTest(0x901) != 0) {
        mnuDrawPartyInfoSprites(0, 0, 0, partyEntry,
                      ((StaffImageContext *)context)->spriteHandle, 0x53);
    }
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)menu->list, 0x53);
    list = menu->list;
    window = list->list;
    if (window->count != 0) {
        struct MenuListNode *node = window->cursor;
        s32 selectedLabel = node->sortKeySecondary;

        if (node->sortKeyPrimary != 0) {
            func_00272518(1, selectedLabel, D_003BAA9C, context, 1, 1, 0x53);
        } else {
            func_00272668(1, 0, 0, context, 1, 0x53);
        }
    } else {
        func_002BF4E0(0x550, 0x5D8, 0, list->fade, 1,
                      ((StaffImageContext *)context)->spriteArg2, 0x10, 0x53);
        func_00272668(1, 0, 0, context, 1, 0x53);
    }
    func_002723B0(1, ((StaffImageContext *)context)->group);
    return menuRunPanel((void *)context, 1, (void *)task);
}

/* Request message-window mode one before the value-page teardown phase. */
s32 mnuExitStaffValuePage(s32 task) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel((void *)context, 2, (void *)task);
}

u32 func_00274B78(void) {
    return 1;
}
