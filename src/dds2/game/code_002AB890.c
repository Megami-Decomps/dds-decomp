#include "mnu.h"
#include "mnu_staff.h"
#include "mnu_list.h"
#include "dat_state.h"
#include "eff.h"


extern u32 kwlnTaskGetUserValue();

extern void sdfReleaseResourceAllocation(s32);


extern void func_002AAE80(s32);

extern void mnuCreateStaffImageSprite(s32);



extern void func_002AB690(s32, s32, s32, s32, s32, s32, s32);
extern void func_002AB8F0(s32);

extern u8 func_002BDA50(s32 index);


void func_002AB890(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_002AB690(arg0, arg1, arg2, 0, arg3, arg4, arg5);
}

void func_002AB8C0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_002AB690(arg0, arg1, arg2, 0x200, arg3, arg4, arg5);
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AB8F0);

void mnuDestroyResourceOwnerWindowContainers(MenuStaffContext *object) {
    MenuStaffChoices *resources;

    resources = (MenuStaffChoices *)object->menu;
    mnuDestroyWindowContainer(resources->windows[0]);
    mnuDestroyWindowContainer(resources->windows[1]);
}

extern void mnuRemoveWindowListCursorNode(MenuWindowContainer *);

s32 mnuIsStaffWindowReadyForItem(s32 itemId, MenuStaffContext *owner) {
    MenuStaffChoices *resources = (MenuStaffChoices *)owner->menu;

    if (datGameState->inventory.counts[itemId & 0xFFFF] == 0) {
        mnuRemoveWindowListCursorNode(resources->windows[0]);
    }
    return resources->windows[0]->list->count != 0;
}

extern char (*D_00435E5C)[25];
extern MenuWindowContainer *mnuCreateWindowContainer(s32, s32, s32, s32, s32);
extern void mnuSetWindowPanelBounds(MenuWindowContainer *, const void *, u32, u32, u32, u32);
extern void mnuSetWindowEntryParameters(u32, MenuWindowContainer *, u32, u32, u32);
extern void mnuInitializeBasicWindowLayout(MenuWindowContainer *, u32, u32);
extern void mnuCreateListWithDefaults(MenuWindowContainer *, u32, u32, u32, u32);
extern s32 mnuIsBulletItemId(s32);

void mnuCreateStaffBulletItemWindow(MenuStaffContext *owner) {
    MenuStaffChoices *resources = (MenuStaffChoices *)owner->menu;
    MenuWindowContainer *window;
    struct MenuListNode *node;
    s32 itemId = 1;
    s32 textOffset = 25;
    u32 quantity;

    window = mnuCreateWindowContainer(0, 0x160, 0x10, 8, 0x16);
    mnuSetWindowContainerState(window, 0x100);
    mnuSetWindowPanelBounds(window, owner->panelLayout, 0, 0, 0, 0);
    mnuSetWindowEntryParameters(0, window, owner->spriteArg0, 0xC, 7);
    window->list->context = owner;
    window->list->drawCallback = func_002AB890;
    do {
        if (datGameState->inventory.counts[itemId] != 0 && mnuIsBulletItemId(itemId)) {
            node = mnuAppendWindowListNode(window, (char *)D_00435E5C + textOffset);
            quantity = datGameState->inventory.counts[itemId];
            node->sortKeySecondary = itemId;
            node->sortKeyPrimary = quantity;
        }
        itemId++;
        textOffset += 25;
    } while (itemId < 0x100);
    resources->windows[2] = window;
    mnuInitializeBasicWindowLayout(window, owner->spriteArg2, 0x12);
    mnuCreateListWithDefaults(resources->windows[2], 0, 0, 0, owner->spriteArg0);
}

void func_002ABEB0(MenuStaffContext *object) {
    mnuDestroyWindowContainer(((MenuStaffChoices *)object->menu)->windows[2]);
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ABED8);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC050);

typedef struct MenuCatalogItem {
    u16 itemId;
    u8 pad02[0x1A];
} MenuCatalogItem;

extern const MenuCatalogItem D_003E7200[18];
extern char D_00437BC8[];
extern void mnuSetWindowContainerLayout(MenuWindowContainer *, u32, u32, u32, u32, u32, u32, u32, u32);
extern void func_002AC050();
extern s32 func_002C54B0(s32);
extern DatPartyRecord *mnuFindPartySlotByCurrentId(u32);
extern DatPartyRecord *mnuFindReserveSlotByCurrentId(u32);
extern s32 mtrMantraIdIsValid(s32);
extern s32 mdlFlagTest(s32);
extern void mnuSortItems(struct MenuList *, s32, s32);

/* The upper item-ID range shares ordered staff entries with mantra availability. */
void mnuCreateOrderedStaffItemWindow(MenuStaffContext *owner) {
    MenuStaffChoices *resources = (MenuStaffChoices *)owner->menu;
    MenuWindowContainer *window;
    struct MenuListNode *node;
    s32 itemId = 0xC1;
    u16 *ordinal = &owner->catalogOrdinals[itemId];
    u16 catalogOrdinal;
    s32 showUnknown;
    u32 frameResource;

    window = mnuCreateWindowContainer(0, 0x1C0, 0x10, 8, 0x16);
    mnuSetWindowContainerState(window, 0x100);
    mnuSetWindowPanelBounds(window, owner->panelLayout, 0, 0, 0, 0);
    mnuSetWindowEntryParameters(0, window, owner->spriteArg0, 0xC, 7);
    window->list->context = owner;
    window->list->drawCallback = func_002AC050;
    node = mnuAppendWindowListNode(window, D_00437BC8);
    node->sortKeyPrimary = 0;
    node->sortKeySecondary = 0;
    node->sortKeyTertiary = 0;
    do {
        if (func_002C54B0(itemId) != 0) {
            catalogOrdinal = *ordinal;
            if (catalogOrdinal != 0) {
                if (datGameState->inventory.counts[itemId] != 0) {
                    node = mnuAppendWindowListNode(window, D_00435E5C[itemId]);
                    node->sortKeyPrimary = datGameState->inventory.counts[itemId];
                    node->sortKeySecondary = itemId;
                    node->sortKeyTertiary = catalogOrdinal;
                    if (mnuFindPartySlotByCurrentId(itemId) != NULL ||
                        mnuFindReserveSlotByCurrentId(itemId) != NULL) {
                        node->flags48 |= 1;
                    } else if (func_002BDA50(itemId) != 0) {
                        node->flags48 |= 4;
                    }
                } else {
                    showUnknown = !mtrMantraIdIsValid(itemId);
                    if (itemId == 0xF7) {
                        showUnknown = 0;
                    }
                    if (!mdlFlagTest(0xBA0) && itemId == 0xF8) {
                        showUnknown = 0;
                    }
                    if (showUnknown) {
                        node = mnuAppendWindowListNode(window, D_00437BC8);
                        node->sortKeyPrimary = 0;
                        node->sortKeySecondary = 0;
                        node->sortKeyTertiary = catalogOrdinal;
                    }
                }
            }
        }
        itemId++;
        ordinal++;
    } while (itemId < 0x100);
    mnuSortItems(window->list, 2, 1);
    frameResource = owner->spriteArg2;
    resources->windows[3] = window;
    mnuInitializeBasicWindowLayout(window, frameResource, 0x10);
    mnuCreateListWithDefaults(resources->windows[3], 0, 0, 0, owner->spriteArg0);
}

void func_002AC660(MenuStaffContext *object) {
    mnuDestroyWindowContainer(((MenuStaffChoices *)object->menu)->windows[3]);
}

s32 mnuUpdateStaffEntrySelectionFlags(s32 previousIndex, s32 selectedIndex, MenuStaffContext *owner) {
    struct MenuListNode *node = ((MenuStaffChoices *)owner->menu)->windows[3]->list->first;
    s32 index;

    if (node != NULL) {
        do {
            index = node->sortKeySecondary;
            if (index == previousIndex) {
                node->flags48 &= ~1;
                if (func_002BDA50(index) != 0) {
                    node->flags48 |= 4;
                }
            }
            if (index == selectedIndex) {
                node->flags48 |= 1;
                node->flags48 &= ~4;
            }
            node = node->next;
        } while (node != NULL);
    }
    return 1;
}

extern void func_00306CD0(s32, s32, s32, u32, s32, EffectSlotSet *, s32, s32);
extern void itfGridCopyEntryQuad(s32, s32);

void func_002AC750(s32 x, s32 y, s32 depth, struct MenuList *list,
                   struct MenuListNode *node, s32 drawArg) {
    MenuStaffContext *owner = (MenuStaffContext *)list->context;
    s32 listScale = list->scale;
    s32 selected = 0;

    if (list->cursor == node) {
        if (!(list->stateFlags & 8)) {
            selected = 1;
        }
    }
    if (node == list->first) {
        if (selected) {
            EffectSlotSet *resources = (EffectSlotSet *)owner->spriteArg2;

            resources->workEntries[1].geometry.cornerColors[0] = 0x89FEFF80;
            resources->workEntries[1].geometry.cornerColors[1] = 0x89FEFF80;
            resources->workEntries[1].geometry.cornerColors[2] = 0x89FEFF80;
            resources->workEntries[1].geometry.cornerColors[3] = 0x89FEFF80;
            func_00306CD0(x + 0x1B0, y + 0x10, 0, listScale, 0,
                          resources, 1, 0x53);
            itfGridCopyEntryQuad((s32)(EffectSlotSet *)owner->spriteArg2, 1);

            func_00306CD0(x - 0x20, y - 8, 0, 0x100, 0,
                          (EffectSlotSet *)owner->spriteArg0, 0x1F, 0x53);
            func_00306CD0(x + 0x960, y - 8, 0, 0x100, 0,
                          (EffectSlotSet *)owner->spriteArg0, 0x1F, 0x53);
        } else {
            func_00306CD0(x + 0x1B0, y + 0x10, 0, listScale, 0,
                          (EffectSlotSet *)owner->spriteArg2, 1, 0x53);
            func_00306CD0(x + 0x60, y, 0, 0xFF, 0,
                          (EffectSlotSet *)owner->spriteArg0, 0xA, 0x53);
            func_00306CD0(x + 0x950, y, 0, 0xFF, 0,
                          (EffectSlotSet *)owner->spriteArg0, 0xA, 0x53);
        }
    } else {
        func_002AB890(x, y, depth, (s32)list, (s32)node, drawArg);
    }
}


/* Build the catalog window from the eighteen item records and owned quantities. */
void mnuCreateOwnedCatalogItemWindow(MenuStaffContext *owner) {
    MenuStaffChoices *resources = (MenuStaffChoices *)owner->menu;
    MenuWindowContainer *window;
    struct MenuListNode *node;
    const MenuCatalogItem *catalog;
    u32 ordinal = 0;
    u32 itemId;
    u32 frameResource;

    window = mnuCreateWindowContainer(0, 0x160, 0x10, 8, 0x16);
    mnuSetWindowContainerState(window, 0x100);
    mnuSetWindowPanelBounds(window, owner->panelLayout, 0, 0, 0, 0);
    mnuSetWindowEntryParameters(0, window, owner->spriteArg0, 0xC, 7);
    window->list->context = owner;
    window->list->drawCallback = func_002AC750;
    node = mnuAppendWindowListNode(window, D_00437BC8);
    node->sortKeyPrimary = 0;
    node->sortKeySecondary = 0;
    node->sortKeyTertiary = 0;
    catalog = D_003E7200;
    for (; ordinal < ARRAY_COUNT(D_003E7200); ordinal++) {
        itemId = catalog->itemId;
        catalog++;
        if (datGameState->inventory.counts[itemId] != 0) {
            node = mnuAppendWindowListNode(window, D_00435E5C[itemId]);
            node->sortKeyPrimary = datGameState->inventory.counts[itemId];
            node->sortKeySecondary = itemId;
            node->sortKeyTertiary = ordinal;
        }
    }
    frameResource = owner->spriteArg2;
    resources->windows[4] = window;
    mnuSetWindowContainerLayout(window, frameResource, 0x15, frameResource,
        0x410, 0x16, frameResource, 0x17, 0x3E0);
    mnuCreateListWithDefaults(resources->windows[4], 0, 0, 0, owner->spriteArg0);
}

void func_002ACA98(MenuStaffContext *object) {
    mnuDestroyWindowContainer(((MenuStaffChoices *)object->menu)->windows[4]);
}

s32 func_002ACAC0(s32 itemId, MenuStaffContext *owner) {
    MenuStaffChoices *resources = (MenuStaffChoices *)owner->menu;

    if (datGameState->inventory.counts[itemId] == 0) {
        mnuRemoveWindowListCursorNode(resources->windows[4]);
    }
    return resources->windows[4]->list->count != 0;
}

void func_002ACB18(u32 arg0) {
    mnuSwitchCampVisualCategory(2, arg0);
}

void func_002ACB38(s32 object) {
}

u32 mnuInitializeWindowOwnerResourceSet(void) {
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue();
    s32 handle = sdfAllocGeneralBlock(0x54);
    MenuStaffChoices *resource = sdfResourceRetainAddress(handle);

    context->menu = resource;
    memset(resource, 0, 0x54);
    resource->allocation = handle;
    func_002ACB18((u32)context);
    func_002AB8F0((s32)context);
    mnuConfigurePanelResource(context->unk118, context->spriteArg2, 0, 0);
    mnuBeginWindowFadeTransition(context->activeWindow, &context->fade);
    mnuSeekListNode(0, context->activeWindow->list);
    return 1;
}

/* Close the staff selection state: drop the owner's window containers, run
 * the owner's teardown hook, then close the party's resource menu. */
s32 mnuDestroyWindowOwnerResourceSet(void) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffContext *owner = (MenuStaffContext *)context;
    MenuStaffChoices *party = (MenuStaffChoices *)owner->menu;

    mnuDestroyResourceOwnerWindowContainers(owner);
    func_002ACB38(context);
    sdfReleaseResourceAllocation(party->allocation);
    return 1;
}

extern s32 mnuMapPadMaskToFlags(s32);
extern void mnuConfigurePanelResource(s32, s32, s32, s32);
extern void func_002B9808(s32);
extern void mnuRetreatWindowListSelection(s32);
extern void mnuAdvanceWindowListSelection(s32);
extern void mnuClearWindowPanelTransitionFlag(s32);
extern void mnuPlayInputSound(s32, s32, u32 *);
extern char D_003E7450[];
extern char D_003E746C[];
extern char D_003E7488[];
extern char D_003E74A4[];
extern char D_003E74C0[];
extern char D_003E7418[];
extern s32 evtGetMessageWindowControlState(void);
extern void func_002C1B68(u32 *, u32);
extern void mnuHandlePanelListPageJumpInput(u32, u32);
extern char D_003E7434[];
extern char D_003E74DC[];

/* Handle staff-item popup selection and idle-window navigation. */
s32 mnuHandleStaffPopupSelection(void *callback) {
    MenuStaffContext *context;
    MenuStaffChoices *resources;
    MenuWindowContainer *window;
    s32 *popup;
    u32 itemKind;
    u32 input;
    s32 result;

    context = (MenuStaffContext *)kwlnTaskGetUserValue();
    popup = &context->popupState;
    resources = (MenuStaffChoices *)context->menu;
    input = mnuMapPadMaskToFlags(0x33);
    result = func_002C4038(&context->transitionWork, popup, 0, callback);
    if (result != 0) {
        return result;
    }
    if (*popup == 0) {
        if (input & 1) {
            itemKind = context->activeWindow->list->cursor->sortKeyPrimary;
            switch (itemKind) {
            case 0:
                resources->secondListState = 0;
                mnuSetPopupEntry(popup, D_003E7450);
                break;
            case 1:
                mnuSetPopupEntry(popup, D_003E746C);
                break;
            case 2:
                mnuSetPopupEntry(popup, D_003E7488);
                break;
            case 3:
                mnuSetPopupEntry(popup, D_003E74A4);
                break;
            default:
                mnuSetPopupEntry(popup, D_003E74C0);
                break;
            }
            mnuSeekListNode(0, resources->windows[0]->list);
            mnuSeekListNode(0, resources->windows[1]->list);
        }
        if (input & 2) {
            mnuSetPopupEntryFlagged(popup, D_003E7418);
            mnuConfigurePanelResource(context->unk118, context->group, 0, 1);
            mnuBeginWindowFadeTransition(context->skillWindow, &context->fade);
        }
        window = context->activeWindow;
        if (window != NULL) {
            if ((input & 0x300000) == 0) {
                func_002B9808((s32)window);
            }
            if (input & 0x10) {
                mnuRetreatWindowListSelection((s32)window);
            }
            if (input & 0x20) {
                mnuAdvanceWindowListSelection((s32)window);
            }
            mnuClearWindowPanelTransitionFlag((s32)window);
            mnuPlayInputSound(0, input, &window->list->stateFlags);
        }
    }
    return 0;
}

extern void func_002AAC98(s32, s32, s32, s32, s32, s32);
extern void func_002AA7A0(s32, s32);
extern u8 D_003E7050[];

s32 func_002ACE58(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_002AAE80(callback);
    mnuCreateStaffImageSprite(4);
    func_002AAC98(0,
        ((MenuStaffContext *)context)->activeWindow->list->cursor->sortKeyPrimary,
        (s32)D_003E7050, context, 1, 0x53);
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, &((MenuStaffContext *)context)->fade, 0x53);
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return menuSetHandler((void *)context, 1, (void *)callback);
}

s32 mnuFinishStaffReturnPopup(s32 callback) {
    return menuSetHandler((void *)kwlnTaskGetUserValue(), 2, (void *)callback);
}

extern s32 evtGetIndexedEventRecordId(s32);
extern void ptyAdjustItemQuantity(s32, s32);


s32 mnuUseStaffItem(s32 itemId, MenuStaffContext *context) {
    MenuPageWindow *partyPanel = &context->partyWindow;
    DatPartyRecord *targetUnit = &datGameState->party[context->partyWindow.lists[0]->cursor->index];
    s32 result = func_002C5A28(partyPanel, itemId & 0xFFFF, targetUnit, targetUnit);

    if (result != 1) {
        if (result == 2) {
            return 0;
        }
        if (ptySkillApplyFieldUseEffect(partyPanel, evtGetIndexedEventRecordId(itemId) & 0xFFFF, targetUnit, targetUnit) == 0) {
            return 0;
        }
    }
    ptyAdjustItemQuantity(itemId, -1);
    mnuInitPartyPanelSlots(&context->partyPanel);
    func_002BCA98(partyPanel);
    func_002BCAB0(partyPanel);
    return 1;
}

/* On successful item use, publish the remaining count and record the selected item. */
void mnuApplyResourceSelection(s32 index, MenuStaffContext *context) {
    MenuStaffChoices *resources;
    s32 consumed;

    resources = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    consumed = mnuUseStaffItem(index, context);
    if (consumed != 0) {
        resources->windows[0]->list->cursor->sortKeyPrimary =
                  datGameState->inventory.counts[index];
        resources->secondListState = index;
    }
    func_002C1B68(&((MenuStaffContext *)context)->unkAA50, 1);
}

u32 func_002AD0A8(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuBeginWindowFadeTransition(((MenuStaffChoices *)((MenuStaffContext *)context)->menu)->windows[0], &((MenuStaffContext *)context)->fade);
    return 1;
}

u32 func_002AD0E8(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuBeginWindowFadeTransition(((MenuStaffContext *)context)->activeWindow, &((MenuStaffContext *)context)->fade);
    return 1;
}

s32 func_002AD118(void *callback) {
    MenuStaffContext *context;
    MenuStaffChoices *resources;
    MenuWindowContainer *window;
    struct MenuList *panel;
    struct MenuListNode *node;
    s32 *popup;
    s32 itemId;
    u32 input;
    s32 result;

    context = (MenuStaffContext *)kwlnTaskGetUserValue();
    resources = (MenuStaffChoices *)context->menu;
    popup = &context->popupState;
    input = mnuMapPadMaskToFlags(0xC33);
    result = func_002C4038(&context->transitionWork, popup, 0, callback);
    if (result != 0) {
        return result;
    }
    if (evtGetMessageWindowControlState() != 0) {
        return 0;
    }
    func_002C1B68(&context->unkAA50, 0);
    if (resources->secondListState == 0) {
        if (input & 1) {
            panel = resources->windows[0]->list;
            if (panel->count != 0) {
                node = panel->cursor;
                if (node->flags48 == 0) {
                    itemId = node->sortKeySecondary;
                    if (mnuGetAbilityByteCategory(evtGetIndexedEventRecordId(itemId)) == 0) {
                        mnuApplyResourceSelection(itemId, context);
                    } else {
                        mnuSetPopupEntry(popup, D_003E74DC);
                    }
                } else {
                    input = 0x8000;
                }
            } else {
                input = 0;
            }
        }
        if (input & 2) {
            mnuSetPopupEntryFlagged(popup, D_003E7434);
        }
    } else {
        if (mnuIsStaffWindowReadyForItem(resources->secondListState & 0xFFFF, context) == 0) {
            mnuSetPopupEntryFlagged(popup, D_003E7434);
            mnuClearActionFlags(0, &context->partyWindow);
            mnuBeginWindowFadeTransition(0, &context->fade);
        } else {
            resources->secondListState = 0;
        }
    }
    window = resources->windows[0];
    if (window != NULL) {
        if ((input & 0x300000) == 0) {
            func_002B9808((s32)window);
        }
        if (input & 0x10) {
            mnuRetreatWindowListSelection((s32)window);
        }
        if (input & 0x20) {
            mnuAdvanceWindowListSelection((s32)window);
        }
        mnuHandlePanelListPageJumpInput((u32)window, (u32)&input);
        mnuClearWindowPanelTransitionFlag((s32)window);
        mnuPlayInputSound(0, input, &window->list->stateFlags);
    }
    return 0;
}

extern s32 D_00435E70;
extern void func_002AAC70(s32, s32, s32, s32, s32, s32, s32);

/* Refresh a staff window's description from the selected item's label. */
void mnuRefreshStaffWindowDescription(MenuStaffContext *context, s32 windowIndex) {
    MenuStaffChoices *resources = (MenuStaffChoices *)context->menu;
    struct MenuListNode *node = resources->windows[windowIndex]->list->cursor;
    s32 emptyLabel = 0;
    s32 selectedLabel = node->sortKeySecondary;

    if (node->sortKeyPrimary != 0) {
        func_002AAC70(0, selectedLabel, D_00435E70, (s32)context, 1, 1, 0x53);
    } else {
        func_002AAC98(0, emptyLabel, 0, (s32)context, 1, 0x53);
    }
}
