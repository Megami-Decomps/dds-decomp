#include "kwln.h"
#include "mnu.h"
#include "mnu_staff.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "dat_state.h"
#include "dat_command.h"

/* Staff callbacks receive a task handle as an integer word. Preserve the
 * native parameter widths and the short-arity task-user-value calls. */
#define MNU_STAFF_INPUT_CONFIRM 1
#define MNU_STAFF_INPUT_CANCEL 2
#define MNU_STAFF_INPUT_PREVIOUS_ROW 0x10
#define MNU_STAFF_INPUT_NEXT_ROW 0x20
#define MNU_STAFF_INPUT_PREVIOUS_PAGE 0x100
#define MNU_STAFF_INPUT_NEXT_PAGE 0x200

extern u32 kwlnTaskGetUserValue();

extern void func_0026C900(void);
extern void func_002AAE80(s32);
extern void mnuCreateStaffImageSprite(s32);
extern void func_002AAC98(s32, s32, s32, s32, s32, s32);
extern void func_002AA7A0(s32, s32);
extern u8 D_003E7050[];
extern char D_003E7207[20];
extern char D_003E7202[];
extern void func_002ABEB0(s32);
extern void func_002AC660(s32);
extern void func_002ACA98(s32);
extern void func_002B2C88(s32, s32, s32, s32);
extern void mnuClearPageSelectionHandles(s32);
extern void mnuClearEntries(s32);
extern void func_002C1B68(u32 *, u32);
extern void mnuReleaseStaffMenuTextureHandles(s32);
extern void func_002C2AA8(struct MenuPanelItem *, u32);
extern char D_00437BD0[];
extern char D_00437BD8[];
extern s32 D_003E7400[];
extern s32 func_002BDA50();
extern s32 func_002BDA78();
extern s32 itfDrawGridWithResolvedSlot(s32, s32, s32, s32, s32, s32, s32);
extern s32 func_0035C860(char *, const char *, ...);
extern s32 func_0019F5E8(s32, s32, s32, s32, s32, s32);
extern void frFontSetChainFlag(s32, s32);
typedef struct FrFontGlyph FrFontGlyph;
extern s32 func_0019D550(FrFontGlyph *, s8, u32);
extern s32 frFontQueueGlyphInSelectedSlot(FrFontGlyph *);
extern s32 evtGetIndexedEventRecordId(s32);
extern s32 D_00435E5C;
extern s32 D_00435E48;
extern u16 mnuGetPartyEntryMenuValue(DatPartyRecord *);
extern u16 mnuGetPartyEntryCurrentId(DatPartyRecord *);
extern void evtCopyEntryStringToActiveWindow(s32, s32);
extern s32 dspStartEntry(s32);
extern void ptyAdjustItemQuantity();
extern void mnuRefreshStaffWindowDescription();
extern void func_00306CD0(s32, s32, s32, u32, s32, struct EffectSlotSet *, s32, s32);
extern char D_003E74F8[];
extern char D_003E7514[];
extern char D_003E7530[];
extern char D_003E7434[];
extern char D_003E7488[];
extern s32 mnuMapPadMaskToFlags(s32);
extern void mnuPlayInputSound(s32, s32, u32 *);
extern s32 mnuUpdateStaffEntrySelectionFlags(s32, s32, MenuStaffContext *);
extern u32 mnuSetPartyEntryCurrentId(u32, u32);
extern void func_002B9808(MenuWindowContainer *);
extern void mnuRetreatWindowListSelection(MenuWindowContainer *);
extern void mnuAdvanceWindowListSelection(MenuWindowContainer *);
extern void mnuHandlePanelListPageJumpInput();
extern void mnuClearWindowPanelTransitionFlag(MenuWindowContainer *);
extern void mnuRetreatListCursorDefault(struct MenuList *);
extern void mnuAdvanceListCursorDefault(struct MenuList *);
extern void mnuClearListFlagsOneAndTwo(u32 *);
extern s32 mnuSeekListNode(s32, struct MenuList *);
extern void mnuResetListNodeFadeCounters(struct MenuList *);
extern void sndSetSequenceVolumePan();
extern void mnuSelectPage(u32 *, s32);
extern void mnuCreateStaffBulletItemWindow(MenuStaffContext *);
extern void mnuCreateOrderedStaffItemWindow(void *);
extern void mnuCreateOwnedCatalogItemWindow(void *);
extern s32 mdlFlagTest();
extern void mnuDrawCampIconBackdropByKind(s32, s32);
extern void mnuDrawAndAdvancePanelGroup(s32, s32, s32, DatPartyRecord *, MenuPanelGroup *, s32, s32);
extern void func_002AAC70(u32, u32, u32, u32, u32, u32, u32);
extern s32 D_00435E70;
extern void func_002BB9C8(MenuSprites *, u32);
extern void mnuReleaseStaffMenuResources(s32 *);
extern void mnuSetWindowResource(s32, u32 *, s32, s32, s32, s32, s32);
extern void mnuSetIndexedWindowPageSpriteFlags(s32, u32 *, s32, s32);

typedef struct MenuListNode MenuListNode;
typedef struct MenuList MenuList;

/* Prepare the primary staff object, then enter the image state. */
s32 mnuStaffImageEnterA(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    MenuWindowContainer *object;

    func_002AAE80(task);
    mnuCreateStaffImageSprite(5);
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, &((MenuStaffContext *)context)->fade, 0x53);
    object = menu->windows[0];
    if (object->list->count != 0) {
        mnuRefreshStaffWindowDescription(context, 0);
    } else {
        if (menu->secondListState == 0) {
            func_00306CD0(0x390, 0x570, 0, object->state, 1, (struct EffectSlotSet *)((MenuStaffContext *)context)->spriteArg2, 0x11, 0x53);
        }
        func_002AAC98(0, 0, 0, context, 1, 0x53);
    }
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return menuSetHandler((void *)context, 1, (void *)task);
}

/* Request value one from the message-window worker, then run the teardown phase. */
s32 mnuStaffImageExitA(s32 task) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((void *)context, 2, (void *)task);
}

u32 func_002AD508(void) {
    return 1;
}

u32 func_002AD510(void) {
    return 1;
}

/* Once the popup is idle, confirmation captures the page-selection cursor index. */
s32 mnuStaffImageInputA(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = mnuMapPadMaskToFlags(3);
    s32 state;

    state = func_002C4038(&((MenuStaffContext *)context)->transitionWork, popup, 0, (void *)task);
    if (state != 0) {
        return state;
    }
    mnuStepPartyPanelListFromInput(4, &((MenuStaffContext *)context)->partyWindow);
    if (buttons & MNU_STAFF_INPUT_CONFIRM) {
        menu->currentSelection = ((MenuStaffContext *)context)->partyWindow.lists[0]->cursor->index;
        mnuSetPopupEntryFlagged(popup, D_003E74F8);
    }
    if (buttons & MNU_STAFF_INPUT_CANCEL) {
        mnuClearActionFlags(0, &((MenuStaffContext *)context)->partyWindow);
        mnuSetPopupEntryFlagged(popup, D_003E7434);
    }
    mnuPlayInputSound(0, buttons, 0);
    return 0;
}

/* Set up a staff image and its associated menu resources before entering the state. */
s32 mnuPrepareStaffImageAndSelectionLabel(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    func_002AAE80(task);
    mnuCreateStaffImageSprite(7);
    func_002AAC98(0,
        ((MenuStaffContext *)context)->activeWindow->list->cursor->sortKeyPrimary,
        (s32)D_003E7050, context, 1, 0x53);
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, &((MenuStaffContext *)context)->fade, 0x53);
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return menuSetHandler((void *)context, 1, (void *)task);
}

/* Run the label-image state's teardown phase and return its scheduler word. */
s32 mnuExitStaffImageAndSelectionLabel(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler((void *)context, 2, (void *)task);
}

u32 func_002AD6F8(void) {
    return 1;
}

u32 func_002AD700(void) {
    return 1;
}

s32 mnuPollStaffSlotSelectionConfirmation(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = mnuMapPadMaskToFlags(3);
    s32 state;

    state = func_002C4038(&((MenuStaffContext *)context)->transitionWork, popup, 0, (void *)task);
    if (state != 0) {
        return state;
    }
    mnuStepPartyPanelListFromInput(4, &((MenuStaffContext *)context)->partyWindow);
    if (buttons & MNU_STAFF_INPUT_CONFIRM) {
        menu->currentSelection = ((MenuStaffContext *)context)->partyWindow.lists[0]->cursor->index;
        mnuSetPopupEntryFlagged(popup, D_003E7514);
    }
    if (buttons & MNU_STAFF_INPUT_CANCEL) {
        mnuClearActionFlags(0, &((MenuStaffContext *)context)->partyWindow);
        mnuSetPopupEntryFlagged(popup, D_003E7434);
    }
    mnuPlayInputSound(0, buttons, 0);
    return 0;
}

/* The three staff image states share the same setup, but select different images. */
s32 func_002AD808(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    func_002AAE80(task);
    mnuCreateStaffImageSprite(9);
    func_002AAC98(0,
        ((MenuStaffContext *)context)->activeWindow->list->cursor->sortKeyPrimary,
        (s32)D_003E7050, context, 1, 0x53);
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, &((MenuStaffContext *)context)->fade, 0x53);
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return menuSetHandler((void *)context, 1, (void *)task);
}

/* Run this image variant's teardown phase and return its scheduler word. */
s32 func_002AD8B0(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler((void *)context, 2, (void *)task);
}

u32 func_002AD8E8(void) {
    return 1;
}

u32 func_002AD8F0(void) {
    return 1;
}

s32 mnuPollStaffValueSelectionConfirmation(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = mnuMapPadMaskToFlags(3);
    s32 state;

    state = func_002C4038(&((MenuStaffContext *)context)->transitionWork, popup, 0, (void *)task);
    if (state != 0) {
        return state;
    }
    mnuStepPartyPanelListFromInput(4, &((MenuStaffContext *)context)->partyWindow);
    if (buttons & MNU_STAFF_INPUT_CONFIRM) {
        menu->currentSelection = ((MenuStaffContext *)context)->partyWindow.lists[0]->cursor->index;
        mnuSetPopupEntryFlagged(popup, D_003E7530);
    }
    if (buttons & MNU_STAFF_INPUT_CANCEL) {
        mnuClearActionFlags(0, &((MenuStaffContext *)context)->partyWindow);
        mnuSetPopupEntryFlagged(popup, D_003E7434);
    }
    mnuPlayInputSound(0, buttons, 0);
    return 0;
}

s32 func_002AD9F8(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    func_002AAE80(task);
    mnuCreateStaffImageSprite(11);
    func_002AAC98(0,
        ((MenuStaffContext *)context)->activeWindow->list->cursor->sortKeyPrimary,
        (s32)D_003E7050, context, 1, 0x53);
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, &((MenuStaffContext *)context)->fade, 0x53);
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return menuSetHandler((void *)context, 1, (void *)task);
}

/* Run this image variant's teardown phase and return its scheduler word. */
s32 func_002ADAA0(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler((void *)context, 2, (void *)task);
}

u32 mnuRefreshSecondaryStaffObject(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuBeginWindowFadeTransition(((MenuStaffChoices *)((MenuStaffContext *)context)->menu)->windows[1], &((MenuStaffContext *)context)->fade);
    return 1;
}

u32 mnuRefreshActiveStaffWindow(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuBeginWindowFadeTransition(((MenuStaffContext *)context)->activeWindow, &((MenuStaffContext *)context)->fade);
    return 1;
}

/* Handle input on the secondary object; its window supplies the sound flags. */
s32 mnuHandleSecondaryStaffObjectInput(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = mnuMapPadMaskToFlags(0xc33);
    s32 state;
    MenuWindowContainer *object;

    state = func_002C4038(&((MenuStaffContext *)context)->transitionWork, popup, 0, (void *)task);
    if (state != 0) {
        return state;
    }
    if (buttons & MNU_STAFF_INPUT_CONFIRM) {
        buttons = 0;
    }
    if (buttons & MNU_STAFF_INPUT_CANCEL) {
        mnuSetPopupEntryFlagged(popup, D_003E7434);
    }
    object = menu->windows[1];
    if (object != 0) {
        if (!(buttons & 0x300000)) {
            func_002B9808(object);
        }
        if (buttons & MNU_STAFF_INPUT_PREVIOUS_ROW) {
            mnuRetreatWindowListSelection(object);
        }
        if (buttons & MNU_STAFF_INPUT_NEXT_ROW) {
            mnuAdvanceWindowListSelection(object);
        }
        mnuHandlePanelListPageJumpInput(object, &buttons);
        mnuClearWindowPanelTransitionFlag(object);
        mnuPlayInputSound(0, buttons, &object->list->stateFlags);
    }
    return 0;
}

s32 mnuStaffImageEnterD(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    MenuWindowContainer *object;

    func_002AAE80(task);
    mnuCreateStaffImageSprite(0xD);
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, &((MenuStaffContext *)context)->fade, 0x53);
    object = menu->windows[1];
    if (object->list->count != 0) {
        mnuRefreshStaffWindowDescription(context, 1);
    } else {
        func_00306CD0(0x390, 0x570, 0, object->state, 1, (struct EffectSlotSet *)((MenuStaffContext *)context)->spriteArg2, 0x11, 0x53);
        func_002AAC98(0, 0, 0, context, 1, 0x53);
    }
    func_002AA7A0(2, ((MenuStaffContext *)context)->group);
    return menuSetHandler((void *)context, 1, (void *)task);
}

/* Run the secondary-image state's teardown phase and return its scheduler word. */
s32 mnuStaffImageExitD(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler((void *)context, 2, (void *)task);
}

extern char D_003E7450[];

s32 func_002ADDA0(s32 task) {
    s32 item = 0;
    s32 itemExhausted = 0;
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue();
    MenuStaffChoices *menu = context->menu;
    s32 *popup = &context->popupState;
    u32 buttons = mnuMapPadMaskToFlags(3);
    s32 state;
    MenuPageWindow *party;

    state = func_002C4038(&context->transitionWork, popup, 0, (void *)task);
    if (state != 0) {
        return state;
    }
    party = &context->partyWindow;
    mnuStepPartyPanelListFromInput(4, party);
    if (menu->windows[0]->list->count != 0) {
        item = menu->windows[0]->list->cursor->sortKeySecondary;
    }
    if (mnuGetAbilityTargetCategory(evtGetIndexedEventRecordId(item)) == 2) {
        party->flags |= 0x10;
    }
    if (mnuGetAbilityTargetCategory(evtGetIndexedEventRecordId(item)) == 3) {
        party->flags |= 0x20;
    }
    if (buttons & MNU_STAFF_INPUT_CONFIRM) {
        buttons = mnuUseStaffItem(item, context) ? 0 : 0x8000;
        menu->windows[0]->list->cursor->sortKeyPrimary = datGameState->inventory.counts[item];
        if (mnuIsStaffWindowReadyForItem(item & 0xFFFF, context) == 0) {
            mnuSetPopupEntryFlagged(popup, D_003E7434);
            mnuClearActionFlags(0, party);
            mnuBeginWindowFadeTransition(0, &context->fade);
        } else {
            itemExhausted = datGameState->inventory.counts[item] == 0;
        }
    }
    if ((buttons & MNU_STAFF_INPUT_CANCEL) || itemExhausted) {
        mnuSetPopupEntry(popup, D_003E7450);
        mnuClearActionFlags(0, party);
    }
    mnuPlayInputSound(0, buttons, 0);
    return 0;
}


s32 mnuStaffImageEnterB(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    MenuWindowContainer *object;

    func_002AAE80(task);
    mnuCreateStaffImageSprite(6);
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, &((MenuStaffContext *)context)->fade, 0x53);
    object = menu->windows[0];
    if (object->list->count != 0) {
        mnuRefreshStaffWindowDescription(context, 0);
    } else {
        func_002AAC98(0,
            ((MenuStaffContext *)context)->activeWindow->list->cursor->sortKeyPrimary,
            (s32)D_003E7050, context, 1, 0x53);
    }
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return menuSetHandler((void *)context, 1, (void *)task);
}

/* Run the alternate primary-image state's teardown phase and return its scheduler word. */
s32 mnuStaffImageExitB(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler((void *)context, 2, (void *)task);
}

s32 mnuInitializeSelectedStaffPage(s32 unused) {
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)context->menu;
    u32 *window = &context->partyWindow.flags;
    s32 index = context->partyWindow.lists[0]->cursor->index;

    mnuSelectPage(window, index);
    mnuCreateStaffBulletItemWindow(context);
    mnuReleaseStaffMenuResources(&context->group);
    mnuSetWindowResource(index, window, context->group, context->spriteArg1, context->windowResource, 0, 0);
    mnuSetIndexedWindowPageSpriteFlags(index, window, 1, 0);
    context->panelHandle = mnuCreatePanelGroup(context->spriteArg0, context->spriteArg1, 0);
    context->spriteHandle = mnuCreateSpriteState((struct EffectSlotSet *)context->spriteArg0,
                                                 (struct EffectSlotSet *)context->spriteArg1,
                                                 (struct EffectSlotSet *)context->group);
    context->partyWindow.flags |= 0x200;
    context->partyWindow.flags &= ~0x80;
    func_002B2C88((s32)window, 1, 0, 0);
    menu->firstListState = 0;
    mnuBeginWindowFadeTransition(menu->windows[2], &context->fade);
    return 1;
}

/* Release the current entry list, its panel group and its auxiliary resource. */
s32 mnuReleaseSelectedStaffPageResources(s32 unused) {
    s32 context = kwlnTaskGetUserValue();
    s32 entryList = context + 0x284;
    mnuBeginWindowFadeTransition(((MenuStaffContext *)context)->activeWindow, &((MenuStaffContext *)context)->fade);
    func_002ABEB0(context);
    func_002B2C88(entryList, 0, 0, 0);
    mnuClearPageSelectionHandles(entryList);
    mnuClearEntries(entryList);
    if (((MenuStaffContext *)context)->panelHandle != 0) {
        mnuDestroyPanelGroup(((MenuStaffContext *)context)->panelHandle);
        ((MenuStaffContext *)context)->panelHandle = 0;
    }
    if (((MenuStaffContext *)context)->spriteHandle != 0) {
        mnuFreeSpriteStateWork(((MenuStaffContext *)context)->spriteHandle);
        ((MenuStaffContext *)context)->spriteHandle = 0;
    }
    func_002C1B68(&((MenuStaffContext *)context)->unkAA50, 0);
    mnuReleaseStaffMenuTextureHandles((s32)&((MenuStaffContext *)context)->group);
    return 1;
}

void mnuPrepareStaffSelectionChangeDialog(s32 context, DatPartyRecord *entry, s32 target) {
    u8 *menu = ((MenuStaffContext *)context)->menu;
    s32 current = mnuGetPartyEntryMenuValue(entry);

    func_002C1B68(&((MenuStaffContext *)context)->unkAA50, 1);
    if (current != target) {
        evtCopyEntryStringToActiveWindow(0, D_00435E48 + entry->unitId * 0x11);
        evtCopyEntryStringToActiveWindow(1, D_00435E5C + current * 0x19);
        evtCopyEntryStringToActiveWindow(2, D_00435E5C + target * 0x19);
        dspStartEntry(0);
        if (current != 0) {
            ptyAdjustItemQuantity(current, 1);
        }
        ptyAdjustItemQuantity(target, -1);
        ((MenuStaffChoices *)menu)->previous = current;
        ((MenuStaffChoices *)menu)->requested = target;
    } else {
        evtCopyEntryStringToActiveWindow(0, D_00435E5C + current * 0x19);
        dspStartEntry(1);
        ((MenuStaffChoices *)menu)->previous = 0;
        ((MenuStaffChoices *)menu)->requested = 0;
    }
}

/* Switch pages and replay the first list's saved cursor steps. */
s32 mnuStaffListInput(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 pageChanged = 0;
    u32 buttons = mnuMapPadMaskToFlags(0x300);
    MenuList *window = menu->windows[2]->list;
    MenuListNode *cursor = window->head;
    s32 savedNodeIndex;
    s32 savedWindowOffset;
    s32 i;

    if (cursor != 0) {
        savedNodeIndex = cursor->index;
        savedWindowOffset = window->windowOffset;
    } else {
        savedNodeIndex = 0;
        savedWindowOffset = 0;
    }
    if (buttons & MNU_STAFF_INPUT_PREVIOUS_PAGE) {
        pageChanged = 1;
        mnuReleaseSelectedStaffPageResources(task);
        mnuRetreatListCursorDefault(((MenuStaffContext *)context)->partyWindow.lists[0]);
    }
    if (buttons & MNU_STAFF_INPUT_NEXT_PAGE && pageChanged == 0) {
        pageChanged = 1;
        mnuReleaseSelectedStaffPageResources(task);
        mnuAdvanceListCursorDefault(((MenuStaffContext *)context)->partyWindow.lists[0]);
    }
    mnuClearListFlagsOneAndTwo(&((MenuStaffContext *)context)->partyWindow.lists[0]->stateFlags);
    if (pageChanged == 0) {
        return 0;
    }
    mnuInitializeSelectedStaffPage(task);
    if (savedNodeIndex != 0 || savedWindowOffset != 0) {
        mnuSeekListNode(savedNodeIndex, menu->windows[2]->list);
        if (savedWindowOffset > 0) {
            for (i = savedWindowOffset; i != 0; i--) {
                mnuAdvanceWindowListSelection(menu->windows[2]);
            }
        }
        mnuResetListNodeFadeCounters(menu->windows[2]->list);
    }
    sndSetSequenceVolumePan(4, 0x7F, 0x3F);
    return 1;
}

/* Handle staff-item selection, confirmation and popup input. */
s32 func_002AE580(KwlnTask *task) {
    extern u32 kwlnTaskGetUserValue(KwlnTask *);
    extern void mnuHandlePanelListPageJumpInput(u32, u32);
    extern s32 func_002ABED8(s32, s32, MenuStaffContext *);
    extern u32 mnuSetPartyEntryMenuValue(DatPartyRecord *, u32);
    extern char D_003E7434[];
    extern char D_003E746C[];

    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue(task);
    MenuStaffChoices *menu = (MenuStaffChoices *)context->menu;
    s32 *popup = &context->popupState;
    MenuWindowContainer *window;
    s32 buttons = mnuMapPadMaskToFlags(0xC33);
    s32 state;
    s32 input;
    DatPartyRecord *party =
        &datGameState->party[context->partyWindow.lists[0]->cursor->index];
    MenuWindowContainer *countWindow;

    state = func_002C4038(&context->transitionWork, popup, 0, task);
    if (state != 0) {
        return state;
    }
    menu->thirdListEnabled = 0;
    if (evtGetMessageWindowControlState() != 0) {
        return 0;
    }
    func_002C1B68(&context->unkAA50, 0);

    if (menu->firstListState == 0) {
        if (mnuStaffListInput((s32)task) == 0) {
            window = menu->windows[2];
            if ((buttons & 0x300000) == 0) {
                func_002B9808(window);
            }
            if (buttons & MNU_STAFF_INPUT_PREVIOUS_ROW) {
                mnuRetreatWindowListSelection(window);
            }
            if (buttons & MNU_STAFF_INPUT_NEXT_ROW) {
                mnuAdvanceWindowListSelection(window);
            }
            mnuHandlePanelListPageJumpInput((u32)window, (u32)&buttons);
            mnuClearWindowPanelTransitionFlag(window);
            input = buttons;

            if (input & MNU_STAFF_INPUT_CONFIRM) {
                if (menu->windows[2]->list->count != 0) {
                    u32 itemId = menu->windows[2]->list->cursor->sortKeySecondary;

                    mnuPrepareStaffSelectionChangeDialog(
                        (s32)context, party, itemId);
                    mnuSetPartyEntryMenuValue(party, itemId);
                    countWindow = menu->windows[2];
                    /* Snapshot input before publishing the remaining item count. */
                    input = buttons;
                    countWindow->list->cursor->sortKeyPrimary =
                        datGameState->inventory.counts[itemId];
                    menu->firstListState = 1;
                } else {
                    buttons = 0;
                    input = 0;
                }
            }
            if (input & MNU_STAFF_INPUT_CANCEL) {
                menu->thirdListEnabled = 1;
                mnuSetPopupEntryFlagged(popup, D_003E746C);
                input = buttons;
            }
            mnuPlayInputSound(0, input, &window->list->stateFlags);
        }
    } else {
        if (func_002ABED8(menu->previous, menu->requested, context) == 0) {
            mnuClearActionFlags(0, &context->partyWindow);
            mnuSetPopupEntryFlagged(popup, D_003E7434);
        } else {
            menu->firstListState = 0;
        }
    }
    return 0;
}


void mnuDrawStaffCaption(s32 entryId, u8 *panel) {
    char captionText[16];
    s32 fontHandle;

    itfDrawGridWithResolvedSlot(0x1C0, 0xA10, 0, 0, ((MenuStaffContext *)panel)->spriteArg2, 2, 0x53);
    if (entryId != 0) {
        func_0035C860(captionText, D_00437BD0, datCommandRecords[evtGetIndexedEventRecordId(entryId)].hpPower);
        fontHandle = func_0019F5E8(0x620, 0xA20, 0, 0xA09DC380, (s32)captionText, 0);
        frFontSetChainFlag(fontHandle, 4);
        func_0019D550((FrFontGlyph *)fontHandle, 1, 0x53);
        frFontQueueGlyphInSelectedSlot((FrFontGlyph *)fontHandle);
    }
}

/* Draw the selected party member's value page and advance its dispatch. */
s32 mnuDrawStaffPartyValuePage(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = ((MenuStaffContext *)context)->menu;
    s32 index = ((MenuStaffContext *)context)->partyWindow.lists[0]->cursor->index;
    DatPartyRecord *partyEntry = &datGameState->party[index];
    MenuWindowContainer *window;
    MenuList *list;

    mnuDrawCampIconBackdropByKind(1, task);
    mnuCreateStaffImageSprite(8);
    mnuApplyPackedGroupValues(((MenuStaffContext *)context)->panelHandle, partyEntry->itemId);
    mnuDrawAndAdvancePanelGroup(0xEB0, 0x518, 0, partyEntry,
                               ((MenuStaffContext *)context)->panelHandle, 0, 0x53);
    mnuUpdateAndDrawWindowTransition(0x1E0, 0x350, 0, &((MenuStaffContext *)context)->fade, 0x53);
    window = menu->windows[2];
    list = window->list;
    if (list->count != 0) {
        MenuListNode *node = list->cursor;
        s32 selectedLabel = node->sortKeySecondary;

        if (node->sortKeyPrimary != 0) {
            func_002AAC70(1, selectedLabel, D_00435E70, context, 1, 1, 0x53);
        } else {
            func_002AAC98(1, 0, 0, context, 1, 0x53);
        }
        mnuDrawStaffCaption(selectedLabel, (u8 *)context);
    } else {
        func_00306CD0(0x390, 0x570, 0, window->state, 1,
                     (struct EffectSlotSet *)((MenuStaffContext *)context)->spriteArg2, 0x11, 0x53);
        func_002AAC98(1, 0, 0, context, 1, 0x53);
        mnuDrawStaffCaption(0, (u8 *)context);
    }
    func_002AA7A0(1, ((MenuStaffContext *)context)->group);
    return menuSetHandler((void *)context, 1, (void *)task);
}

/* Request value one from the message-window worker before the teardown phase. */
s32 func_002AEA58(s32 task) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((void *)context, 2, (void *)task);
}

s32 mnuInitializeStaffPageWithSlotAsset(s32 unused) {
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue();
    u32 *window = &context->partyWindow.flags;
    MenuStaffChoices *menu = (MenuStaffChoices *)context->menu;
    s32 index = context->partyWindow.lists[0]->cursor->index;
    MenuPageSlot *slot = &context->partyWindow.slots[index];

    mnuSelectPage(window, index);
    mnuCreateOrderedStaffItemWindow(context);
    mnuReleaseStaffMenuResources(&context->group);
    mnuSetWindowResource(index, window, context->group, context->spriteArg1, context->windowResource, context->spriteArg0,
                         context->spriteArg2);
    mnuSetIndexedWindowPageSpriteFlags(index, window, 0, 2);
    if (mdlFlagTest(0x990) != 0) {
        func_002BB9C8(slot->windowSprites, 1);
    }
    context->panelHandle = mnuCreatePanelGroup(context->spriteArg0, context->spriteArg1, 0);
    context->spriteHandle = mnuCreateSpriteState((struct EffectSlotSet *)context->spriteArg0,
                                                 (struct EffectSlotSet *)context->spriteArg1,
                                                 (struct EffectSlotSet *)context->group);
    context->partyWindow.flags |= 0x200;
    context->partyWindow.flags &= ~0x80;
    func_002B2C88((s32)window, 1, 0, 0);
    menu->secondListReset = 0;
    mnuBeginWindowFadeTransition(menu->windows[3], &context->fade);
    return 1;
}

/* Variant cleanup for the adjacent menu state; keep the same release ordering. */
s32 mnuReleaseStaffSelectionPageResources(s32 unused) {
    s32 context = kwlnTaskGetUserValue();
    s32 entryList = context + 0x284;
    mnuBeginWindowFadeTransition(((MenuStaffContext *)context)->activeWindow, &((MenuStaffContext *)context)->fade);
    func_002AC660(context);
    func_002B2C88(entryList, 0, 0, 0);
    mnuClearPageSelectionHandles(entryList);
    mnuClearEntries(entryList);
    if (((MenuStaffContext *)context)->panelHandle != 0) {
        mnuDestroyPanelGroup(((MenuStaffContext *)context)->panelHandle);
        ((MenuStaffContext *)context)->panelHandle = 0;
    }
    if (((MenuStaffContext *)context)->spriteHandle != 0) {
        mnuFreeSpriteStateWork(((MenuStaffContext *)context)->spriteHandle);
        ((MenuStaffContext *)context)->spriteHandle = 0;
    }
    func_002C1B68(&((MenuStaffContext *)context)->unkAA50, 0);
    mnuReleaseStaffMenuTextureHandles((s32)&((MenuStaffContext *)context)->group);
    return 1;
}

void mnuStaffEntrySwapLabels(s32 context, u8 *entry, s32 target) {
    u8 *menu = ((MenuStaffContext *)context)->menu;
    s32 current = mnuGetPartyEntryCurrentId((DatPartyRecord *)entry);

    func_002C1B68(&((MenuStaffContext *)context)->unkAA50, 1);
    if (target == 0) {
        evtCopyEntryStringToActiveWindow(0, D_00435E48 + *(u16 *)(entry + 4) * 0x11);
        evtCopyEntryStringToActiveWindow(1, D_00435E5C + current * 0x19);
        dspStartEntry(6);
        ((MenuStaffChoices *)menu)->alternatePrevious = current;
        ((MenuStaffChoices *)menu)->alternateRequested = 0;
    } else if (current != target) {
        if (current != 0) {
            evtCopyEntryStringToActiveWindow(0, D_00435E48 + *(u16 *)(entry + 4) * 0x11);
            evtCopyEntryStringToActiveWindow(1, D_00435E5C + current * 0x19);
            evtCopyEntryStringToActiveWindow(2, D_00435E5C + target * 0x19);
            dspStartEntry(3);
        } else {
            evtCopyEntryStringToActiveWindow(0, D_00435E48 + *(u16 *)(entry + 4) * 0x11);
            evtCopyEntryStringToActiveWindow(1, D_00435E5C + target * 0x19);
            dspStartEntry(4);
        }
        ((MenuStaffChoices *)menu)->alternatePrevious = current;
        ((MenuStaffChoices *)menu)->alternateRequested = target;
    } else {
        evtCopyEntryStringToActiveWindow(0, D_00435E5C + current * 0x19);
        dspStartEntry(5);
        ((MenuStaffChoices *)menu)->alternatePrevious = 0;
        ((MenuStaffChoices *)menu)->alternateRequested = 0;
    }
}

/* Preserve the second list's position while switching its selected page.
 * Previous-page input wins when both page-direction flags are present. */
s32 mnuHandleStaffSelectionListNavigation(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 pageChanged = 0;
    u32 buttons = mnuMapPadMaskToFlags(0x300);
    MenuList *window = menu->windows[3]->list;
    MenuListNode *cursor = window->head;
    s32 savedNodeIndex;
    s32 savedWindowOffset;
    s32 i;

    if (cursor != 0) {
        savedNodeIndex = cursor->index;
        savedWindowOffset = window->windowOffset;
    } else {
        savedNodeIndex = 0;
        savedWindowOffset = 0;
    }
    if (buttons & MNU_STAFF_INPUT_PREVIOUS_PAGE) {
        pageChanged = 1;
        mnuReleaseStaffSelectionPageResources(task);
        mnuRetreatListCursorDefault(((MenuStaffContext *)context)->partyWindow.lists[0]);
    }
    if (buttons & MNU_STAFF_INPUT_NEXT_PAGE && pageChanged == 0) {
        pageChanged = 1;
        mnuReleaseStaffSelectionPageResources(task);
        mnuAdvanceListCursorDefault(((MenuStaffContext *)context)->partyWindow.lists[0]);
    }
    mnuClearListFlagsOneAndTwo(&((MenuStaffContext *)context)->partyWindow.lists[0]->stateFlags);
    if (pageChanged == 0) {
        return 0;
    }
    mnuInitializeStaffPageWithSlotAsset(task);
    if (savedNodeIndex != 0 || savedWindowOffset != 0) {
        mnuSeekListNode(savedNodeIndex, menu->windows[3]->list);
        if (savedWindowOffset > 0) {
            for (i = savedWindowOffset; i != 0; i--) {
                mnuAdvanceWindowListSelection(menu->windows[3]);
            }
        }
        mnuResetListNodeFadeCounters(menu->windows[3]->list);
    }
    sndSetSequenceVolumePan(4, 0x7F, 0x3F);
    return 1;
}

s32 func_002AF020(KwlnTask *task) {
    extern u32 kwlnTaskGetUserValue(KwlnTask *);
    extern void mnuHandlePanelListPageJumpInput(u32, u32);

    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue(task);
    MenuStaffChoices *menu = (MenuStaffChoices *)context->menu;
    MenuWindowContainer *window;
    DatPartyRecord *party;
    s32 buttons = mnuMapPadMaskToFlags(0xC33);
    s32 result;
    s32 eligible;
    s32 selectionId;

    party = &datGameState->party[context->partyWindow.lists[0]->cursor->index];
    result = menuSetHandler(context, 0, task);
    if (result != 0) {
        return result;
    }

    menu->thirdListEnabled = 0;
    if (evtGetMessageWindowControlState() != 0) {
        return 0;
    }
    func_002C1B68(&context->unkAA50, 0);

    if (menu->secondListReset == 0) {
        if (mnuHandleStaffSelectionListNavigation((s32)task) == 0) {
            window = menu->windows[3];
            if ((buttons & 0x300000) == 0) {
                func_002B9808(window);
            }
            if (buttons & MNU_STAFF_INPUT_PREVIOUS_ROW) {
                mnuRetreatWindowListSelection(window);
            }
            if (buttons & MNU_STAFF_INPUT_NEXT_ROW) {
                mnuAdvanceWindowListSelection(window);
            }
            mnuHandlePanelListPageJumpInput((u32)window, (u32)&buttons);
            mnuClearWindowPanelTransitionFlag(window);

            if (buttons & MNU_STAFF_INPUT_CONFIRM) {
                if (menu->windows[3]->list->count != 0) {
                    eligible = 1;
                    if (menu->windows[3]->list->cursor->index == 0) {
                        selectionId = 0;
                        if (party->itemId == 0) {
                            eligible = 0;
                        }
                    } else {
                        selectionId = menu->windows[3]->list->cursor->sortKeySecondary;
                        eligible = selectionId != 0;
                        if ((menu->windows[3]->list->cursor->flags48 & 1) != 0 &&
                            selectionId != mnuGetPartyEntryCurrentId(party)) {
                            eligible = 0;
                        }
                    }

                    if (eligible != 0) {
                        mnuStaffEntrySwapLabels((s32)context, (u8 *)party, selectionId);
                        mnuSetPartyEntryCurrentId((u32)party, (u32)selectionId);
                        menu->windows[3]->list->cursor->sortKeyPrimary = datGameState->inventory.counts[selectionId];
                        mnuInitPartyPanelSlots(&context->partyPanel);
                        func_002BCAB0(&context->partyWindow);
                        menu->secondListReset = 1;
                    } else {
                        buttons = 0x8000;
                    }
                } else {
                    buttons = 0;
                }
            }

            if (buttons & MNU_STAFF_INPUT_CANCEL) {
                menu->thirdListEnabled = 1;
                mnuSetPopupEntryFlagged(&context->popupState, D_003E7488);
            }
            mnuPlayInputSound(0, buttons, &window->list->stateFlags);
        }
    } else {
        if (mnuUpdateStaffEntrySelectionFlags(menu->alternatePrevious,
                menu->alternateRequested, context) == 0) {
            mnuClearActionFlags(0, &context->partyWindow);
            mnuSetPopupEntryFlagged(&context->popupState, D_003E7434);
        } else {
            menu->secondListReset = 0;
        }
    }

    return 0;
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AF2E0);

s32 func_002AF5E0(KwlnTask *task) {
    extern u32 kwlnTaskGetUserValue(KwlnTask *);
    extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);
    extern void func_002BDAA8(s32, s32, s32, s32, s32, s32);
    extern s32 func_002AF2E0(s32, s32, s32, MenuStaffContext *);
    extern char D_0042AD08[];
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue(task);
    const u8 *slotsCaption = (const u8 *)D_0042AD08;
    MenuStaffChoices *menu = context->menu;
    DatPartyRecord *party = &datGameState->party[context->partyWindow.lists[0]->cursor->index];
    MenuList *list;
    s32 selectionId;
    s32 owned;
    FrFontGlyph *glyph;

    mnuDrawCampIconBackdropByKind(1, (s32)task);
    mnuCreateStaffImageSprite(10);
    if (menu->windows[3]->list->count != 0) {
        selectionId = menu->windows[3]->list->cursor->sortKeySecondary;
    } else {
        selectionId = 0;
    }
    mnuApplyPackedGroupValues(context->panelHandle, selectionId);
    mnuDrawAndAdvancePanelGroup(0xEB0, 0x518, 0, party, context->panelHandle, 1, 0x53);
    list = menu->windows[3]->list;
    if (list->cursor->index == 0) {
        list->stateFlags |= 0x10;
    } else {
        list->stateFlags &= ~0x10;
    }
    mnuUpdateAndDrawWindowTransition(0x1E0, 0x350, 0, &context->fade, 0x53);
    if (menu->windows[3]->list->count != 0) {
        owned = menu->windows[3]->list->cursor->sortKeyPrimary;
        selectionId = menu->windows[3]->list->cursor->sortKeySecondary;
        if (owned != 0) {
            func_002AAC70(1, selectionId, D_00435E70, (s32)context, 1, 1, 0x53);
        } else {
            func_002AAC98(1, 0, 0, (s32)context, 1, 0x53);
        }
        func_002AF2E0(0, 0, selectionId, context);
        if (owned != 0 && mdlFlagTest(0x990) != 0) {
            glyph = (FrFontGlyph *)itfCreateConvertedTextGlyph(0x2B0, 0xB80, 0, 0xA09DC340, slotsCaption, 0);
            func_0019D550(glyph, 1, 0x53);
            frFontQueueGlyphInSelectedSlot(glyph);
            func_002BDAA8(0x770, 0xB98, 0x100, selectionId, context->spriteArg0, 0x2C);
        }
    } else {
        func_00306CD0(0x390, 0x570, 0, menu->windows[3]->state, 1,
                     (struct EffectSlotSet *)context->spriteArg2, 0x11, 0x53);
        func_002AAC98(1, 0, 0, (s32)context, 1, 0x53);
        func_002AF2E0(0, 0, 0, context);
    }
    func_002AA7A0(1, context->group);
    return menuSetHandler(context, 1, task);
}

/* Request value one from the message-window worker before the value-page teardown. */
s32 mnuExitStaffValuePage(s32 task) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((void *)context, 2, (void *)task);
}

s32 mnuInitializeStaffValuePage(s32 unused) {
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue();
    u32 *window = &context->partyWindow.flags;
    MenuStaffChoices *menu = (MenuStaffChoices *)context->menu;
    s32 index = context->partyWindow.lists[0]->cursor->index;
    MenuPageSlot *slot = &context->partyWindow.slots[index];

    mnuSelectPage(window, index);
    mnuCreateOwnedCatalogItemWindow(context);
    mnuReleaseStaffMenuResources(&context->group);
    mnuSetWindowResource(index, window, context->group, context->spriteArg1, context->windowResource, context->spriteArg0,
                         context->spriteArg2);
    mnuSetIndexedWindowPageSpriteFlags(index, window, 0, 2);
    if (mdlFlagTest(0x990) != 0) {
        func_002BB9C8(slot->windowSprites, 1);
    }
    context->panelHandle = mnuCreatePanelGroup(context->spriteArg0, context->spriteArg1, context->spriteArg2);
    context->spriteHandle = mnuCreateSpriteState((struct EffectSlotSet *)context->spriteArg0,
                                                 (struct EffectSlotSet *)context->spriteArg1,
                                                 (struct EffectSlotSet *)context->group);
    context->partyWindow.flags |= 0x200;
    context->partyWindow.flags &= ~0x80;
    func_002B2C88((s32)window, 1, 0, 0);
    menu->thirdListState = 0;
    if (menu->thirdListEnabled != 0) {
        menu->thirdListReset = 0;
    }
    mnuBeginWindowFadeTransition(menu->windows[4], &context->fade);
    return 1;
}

/* Third menu-state cleanup uses the matching state-specific pre-release. */
s32 mnuReleaseStaffValuePageResources(s32 unused) {
    s32 context = kwlnTaskGetUserValue();
    s32 entryList = context + 0x284;
    mnuBeginWindowFadeTransition(((MenuStaffContext *)context)->activeWindow, &((MenuStaffContext *)context)->fade);
    func_002ACA98(context);
    func_002B2C88(entryList, 0, 0, 0);
    mnuClearPageSelectionHandles(entryList);
    mnuClearEntries(entryList);
    if (((MenuStaffContext *)context)->panelHandle != 0) {
        mnuDestroyPanelGroup(((MenuStaffContext *)context)->panelHandle);
        ((MenuStaffContext *)context)->panelHandle = 0;
    }
    if (((MenuStaffContext *)context)->spriteHandle != 0) {
        mnuFreeSpriteStateWork(((MenuStaffContext *)context)->spriteHandle);
        ((MenuStaffContext *)context)->spriteHandle = 0;
    }
    func_002C1B68(&((MenuStaffContext *)context)->unkAA50, 0);
    mnuReleaseStaffMenuTextureHandles((s32)&((MenuStaffContext *)context)->group);
    return 1;
}

void mnuPrepareStaffValueChangeDialog(s32 context, u8 *entry, s32 unused, s32 flag) {
    char valueText[16];
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 current;
    s32 base;

    func_002C1B68(&((MenuStaffContext *)context)->unkAA50, 1);
    current = mnuGetPartyEntryCurrentId((DatPartyRecord *)entry);
    evtCopyEntryStringToActiveWindow(0, D_00435E5C + current * 0x19);
    evtCopyEntryStringToActiveWindow(1, D_003E7400[menu->thirdListIndex]);
    func_0035C860(valueText, D_00437BD8, menu->thirdListValue);
    evtCopyEntryStringToActiveWindow(2, (s32)valueText);
    base = func_002BDA50(current);
    func_0035C860(valueText, D_00437BD8, func_002BDA78(current) - base);
    evtCopyEntryStringToActiveWindow(3, (s32)valueText);
    if (flag == 0) {
        dspStartEntry(9);
    } else {
        dspStartEntry(0xA);
    }
}

/* Preserve the third list's position while switching its selected page.
 * Returns one after rebuilding the page, zero when neither direction wins. */
s32 mnuHandleStaffValuePageInput(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 pageChanged = 0;
    u32 buttons = mnuMapPadMaskToFlags(0x300);
    MenuList *window = menu->windows[4]->list;
    MenuListNode *cursor = window->head;
    s32 savedNodeIndex;
    s32 savedWindowOffset;
    s32 i;

    if (cursor != 0) {
        savedNodeIndex = cursor->index;
        savedWindowOffset = window->windowOffset;
    } else {
        savedNodeIndex = 0;
        savedWindowOffset = 0;
    }
    if (buttons & MNU_STAFF_INPUT_PREVIOUS_PAGE) {
        pageChanged = 1;
        mnuReleaseStaffValuePageResources(task);
        mnuRetreatListCursorDefault(((MenuStaffContext *)context)->partyWindow.lists[0]);
    }
    if (buttons & MNU_STAFF_INPUT_NEXT_PAGE && pageChanged == 0) {
        pageChanged = 1;
        mnuReleaseStaffValuePageResources(task);
        mnuAdvanceListCursorDefault(((MenuStaffContext *)context)->partyWindow.lists[0]);
    }
    mnuClearListFlagsOneAndTwo(&((MenuStaffContext *)context)->partyWindow.lists[0]->stateFlags);
    if (pageChanged == 0) {
        return 0;
    }
    mnuInitializeStaffValuePage(task);
    if (savedNodeIndex != 0 || savedWindowOffset != 0) {
        mnuSeekListNode(savedNodeIndex, menu->windows[4]->list);
        if (savedWindowOffset > 0) {
            for (i = savedWindowOffset; i != 0; i--) {
                mnuAdvanceWindowListSelection(menu->windows[4]);
            }
        }
        mnuResetListNodeFadeCounters(menu->windows[4]->list);
    }
    sndSetSequenceVolumePan(4, 0x7F, 0x3F);
    return 1;
}

typedef struct MenuRequirementRecord {
    u8 pad00[5];
    s8 requiredCount;
} MenuRequirementRecord;

extern s32 D_00435E3C;

/* A zero entry has no requirement. Keep the signed requiredCount comparison:
 * the available count comes from a byte-sized game-state entry. */
s32 mnuIsStaffRequirementUnmet(s32 entryId) {
    s32 requirementIndex = entryId - 0xC0;
    u8 availableCount;

    if (entryId == 0) {
        return 0;
    }
    availableCount = datGameState->itemRequirementCounts[requirementIndex];
    return availableCount < ((MenuRequirementRecord *)D_00435E3C)[requirementIndex].requiredCount;
}

extern s32 evtGetMessageWindowControlState(void);
extern s32 func_002ACAC0(s32, MenuStaffContext *);
extern char D_003E74A4[];
extern char D_003E754C[];
extern char D_003E7568[];

s32 func_002AFE18(s32 task) {
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue();
    MenuStaffChoices *menu = context->menu;
    MenuWindowContainer *window;
    DatPartyRecord *party;
    u32 input = mnuMapPadMaskToFlags(0xCF3);
    s32 result;
    s32 current;

    party = &datGameState->party[context->partyWindow.lists[0]->cursor->index];
    result = menuSetHandler(context, 0, (void *)task);
    if (result != 0) {
        return result;
    }
    menu->thirdListEnabled = 0;
    if (evtGetMessageWindowControlState() != 0) {
        return 0;
    }
    func_002C1B68(&context->unkAA50, 0);
    if (menu->thirdListState == 0) {
        window = menu->windows[4];
        if (mnuHandleStaffValuePageInput(task) != 0) {
            return 0;
        }
        if (window->list->cursor != window->list->first) {
            if (input & 0xC0) {
                menu->thirdListReset ^= 1;
            }
            if (input & 0xC31) {
                menu->thirdListReset = 0;
            }
        } else {
            input &= ~0x40;
            input &= ~0x80;
        }
        if (!(input & 0x300000)) {
            func_002B9808(window);
        }
        if (input & 0x10) {
            mnuRetreatWindowListSelection(window);
        }
        if (input & 0x20) {
            mnuAdvanceWindowListSelection(window);
        }
        mnuHandlePanelListPageJumpInput((u32)window, (u32)&input);
        mnuClearWindowPanelTransitionFlag(window);
        if (input & 1) {
            current = mnuGetPartyEntryCurrentId(party);
            if (current != 0) {
                if (menu->windows[4]->list->cursor->index == 0) {
                    if (func_002BDA50(current) != 0) {
                        mnuSetPopupEntry(&context->popupState, D_003E7568);
                    } else {
                        func_002C1B68(&context->unkAA50, 1);
                        evtCopyEntryStringToActiveWindow(0, D_00435E5C + current * 0x19);
                        dspStartEntry(0xB);
                    }
                } else if (menu->windows[4]->list->count != 0) {
                    if (mnuIsStaffRequirementUnmet(current) != 0) {
                        mnuSetPopupEntry(&context->popupState, D_003E754C);
                    } else {
                        func_002C1B68(&context->unkAA50, 1);
                        dspStartEntry(7);
                    }
                } else {
                    input = 0;
                }
            } else if (menu->windows[4]->list->cursor->index == 0) {
                func_002C1B68(&context->unkAA50, 1);
                dspStartEntry(0xC);
            } else {
                input = 0x8000;
            }
        }
        if (input & 2) {
            menu->thirdListEnabled = 1;
            mnuSetPopupEntryFlagged(&context->popupState, D_003E74A4);
        }
        mnuPlayInputSound(0, input, &window->list->stateFlags);
    } else {
        if (func_002ACAC0(menu->thirdListState, context) == 0) {
            mnuSetPopupEntryFlagged(&context->popupState, D_003E7434);
            mnuClearActionFlags(0, &context->partyWindow);
            mnuBeginWindowFadeTransition(0, &context->fade);
        } else {
            menu->thirdListState = 0;
        }
    }
    return 0;
}

void mnuSetPanelItemsFromRow(MenuPanelGroup *config, s32 rowIndex) {
    char *activeSlots = D_003E7207 + rowIndex * 0x1C;
    char *entryFlags;
    struct MenuPanelItem **configEntry;
    u32 lastActiveSlot = 0;
    s32 i;

    for (i = 0; i < sizeof(D_003E7207); i++, activeSlots++) {
        if (*activeSlots) {
            lastActiveSlot = i;
        }
    }
    configEntry = config->entries;
    entryFlags = D_003E7202 + rowIndex * 0x1C;
    for (i = 4; i >= 0; i--, entryFlags++) {
        func_002C2AA8(*configEntry++, *entryFlags ? lastActiveSlot : 0);
    }
}

void mnuClearStaffSceneConfigEntries(MenuPanelGroup *config) {
    s32 i;
    for (i = 0; i < 5; i++) {
        func_002C2AA8(config->entries[i], 0);
    }
}

INCLUDE_RODATA(const s32, "game/code_002AD3B8", D_0042ACA0);

INCLUDE_RODATA(const s32, "game/code_002AD3B8", D_0042ACC8);

INCLUDE_RODATA(const s32, "game/code_002AD3B8", D_0042AD08);

INCLUDE_SDATA(const s32, "game/code_002AD3B8", D_00437BD0);

INCLUDE_SDATA(const s32, "game/code_002AD3B8", D_00437BD8);

