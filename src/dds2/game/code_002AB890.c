#include "mnu.h"

extern s32 datGameState;

extern u32 kwlnTaskGetUserValue();

extern void sdfReleaseResourceAllocation(s32);

extern s32 func_002C4038(s32, s32 *, u64, u64);

extern void func_002AAE80(s32);

extern void mnuCreateStaffImageSprite(s32);


/* Inventory quantities are byte entries indexed by each caller's original item ID. */
typedef struct SaveItemCounts {
    u8 pad00[0x1340];
    u8 counts[0x100];
} SaveItemCounts;

/* The menu buffer carries its allocation ID and five owned window handles. */
typedef struct MenuResourceSet {
    s32 allocation;
    u8 pad04[4];
    u32 windows[5];
    u8 pad1C[0x1C];
    s32 selection;
} MenuResourceSet;

extern void func_002AB690(s32, s32, s32, s32, s32, s32, s32);
extern void func_002AB8F0(s32);

extern void mnuBeginWindowFadeTransition(s32, s32);
extern u8 func_002BDA50(s32 index);


void func_002AB890(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_002AB690(arg0, arg1, arg2, 0, arg3, arg4, arg5);
}

void func_002AB8C0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_002AB690(arg0, arg1, arg2, 0x200, arg3, arg4, arg5);
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AB8F0);

void mnuDestroyResourceOwnerWindowContainers(MenuStaffContext *object) {
    MenuResourceSet *resources;

    resources = (MenuResourceSet *)object->menu;
    mnuDestroyWindowContainer(resources->windows[0]);
    mnuDestroyWindowContainer(resources->windows[1]);
}

extern void func_002B9720(s32);

s32 mnuIsStaffWindowReadyForItem(s32 itemId, MenuStaffContext *owner) {
    MenuResourceSet *resources = (MenuResourceSet *)owner->menu;

    if (((SaveItemCounts *)datGameState)->counts[itemId & 0xFFFF] == 0) {
        func_002B9720(resources->windows[0]);
    }
    return ((MenuStaffList *)resources->windows[0])->window->panelActive != 0;
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ABD60);

void func_002ABEB0(MenuStaffContext *object) {
    mnuDestroyWindowContainer(((MenuResourceSet *)object->menu)->windows[2]);
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ABED8);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC050);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC408);

void func_002AC660(MenuStaffContext *object) {
    mnuDestroyWindowContainer(((MenuResourceSet *)object->menu)->windows[3]);
}

s32 mnuUpdateStaffEntrySelectionFlags(s32 previousIndex, s32 selectedIndex, MenuStaffContext *owner) {
    MenuStaffNode *node = ((MenuStaffList *)((MenuResourceSet *)owner->menu)->windows[3])->window->head;
    s32 index;

    if (node != NULL) {
        do {
            index = node->entryIndex;
            if (index == previousIndex) {
                node->flags &= ~1;
                if (func_002BDA50(index) != 0) {
                    node->flags |= 4;
                }
            }
            if (index == selectedIndex) {
                node->flags |= 1;
                node->flags &= ~4;
            }
            node = node->next;
        } while (node != NULL);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC750);

typedef struct MenuCatalogItem {
    u16 itemId;
    u8 pad02[0x1A];
} MenuCatalogItem;

extern const MenuCatalogItem D_003E7200[18];
extern char (*D_00435E5C)[25];
extern char D_00437BC8[];
extern void func_002AC750();
extern s32 mnuCreateWindowContainer(s32, s32, s32, s32, s32);
extern void mnuSetWindowContainerState(MenuStaffList *, u32);
extern void mnuSetWindowPanelBounds(MenuStaffList *, const void *, u32, u32, u32, u32);
extern void mnuSetWindowEntryParameters(u32, MenuStaffList *, u32, u32, u32);
extern MenuStaffNode *mnuAppendWindowListNode(MenuStaffList *, s32);
extern void mnuSetWindowContainerLayout(MenuStaffList *, u32, u32, u32, u32, u32, u32, u32, u32);
extern void mnuCreateListWithDefaults(MenuStaffList *, u32, u32, u32, u32);

/* Build the catalog window from the eighteen item records and owned quantities. */
void mnuCreateOwnedCatalogItemWindow(MenuStaffContext *owner) {
    MenuResourceSet *resources = (MenuResourceSet *)owner->menu;
    MenuStaffList *window;
    MenuStaffNode *node;
    const MenuCatalogItem *catalog;
    u32 ordinal = 0;
    u32 itemId;
    u32 frameResource;

    window = (MenuStaffList *)mnuCreateWindowContainer(0, 0x160, 0x10, 8, 0x16);
    mnuSetWindowContainerState(window, 0x100);
    mnuSetWindowPanelBounds(window, owner->panelLayout, 0, 0, 0, 0);
    mnuSetWindowEntryParameters(0, window, owner->spriteArg0, 0xC, 7);
    window->window->owner = owner;
    window->window->drawEntry = func_002AC750;
    node = mnuAppendWindowListNode(window, (s32)D_00437BC8);
    node->label = 0;
    node->entryIndex = 0;
    node->catalogOrdinal = 0;
    catalog = D_003E7200;
    for (; ordinal < ARRAY_COUNT(D_003E7200); ordinal++) {
        itemId = catalog->itemId;
        catalog++;
        if (((SaveItemCounts *)datGameState)->counts[itemId] != 0) {
            node = mnuAppendWindowListNode(window, (s32)D_00435E5C[itemId]);
            node->label = ((SaveItemCounts *)datGameState)->counts[itemId];
            node->entryIndex = itemId;
            node->catalogOrdinal = ordinal;
        }
    }
    frameResource = owner->spriteArg2;
    resources->windows[4] = (u32)window;
    mnuSetWindowContainerLayout(window, frameResource, 0x15, frameResource,
        0x410, 0x16, frameResource, 0x17, 0x3E0);
    mnuCreateListWithDefaults((MenuStaffList *)resources->windows[4], 0, 0, 0, owner->spriteArg0);
}

void func_002ACA98(MenuStaffContext *object) {
    mnuDestroyWindowContainer(((MenuResourceSet *)object->menu)->windows[4]);
}

s32 func_002ACAC0(s32 itemId, MenuStaffContext *owner) {
    MenuResourceSet *resources = (MenuResourceSet *)owner->menu;

    if (((SaveItemCounts *)datGameState)->counts[itemId] == 0) {
        func_002B9720(resources->windows[4]);
    }
    return ((MenuStaffList *)resources->windows[4])->window->panelActive != 0;
}

void func_002ACB18(u32 arg0) {
    mnuSwitchCampVisualCategory(2, arg0);
}

void func_002ACB38(s32 object) {
}

u32 mnuInitializeWindowOwnerResourceSet(void) {
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue();
    s32 handle = sdfAllocGeneralBlock(0x54);
    MenuResourceSet *resource = sdfResourceRetainAddress(handle);

    context->menu = (u8 *)resource;
    memset(resource, 0, 0x54);
    resource->allocation = handle;
    func_002ACB18((u32)context);
    func_002AB8F0((s32)context);
    /* This input word at +0x118 is not identified in the shared context yet. */
    mnuConfigurePanelResource(*(s32 *)((u8 *)context + 0x118), context->spriteArg2, 0, 0);
    mnuBeginWindowFadeTransition((s32)context->activeWindow, (s32)context->tail);
    mnuSeekListNode(0, context->activeWindow->window);
    return 1;
}

/* Close the staff selection state: drop the owner's window containers, run
 * the owner's teardown hook, then close the party's resource menu. */
s32 mnuDestroyWindowOwnerResourceSet(void) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffContext *owner = (MenuStaffContext *)context;
    MenuResourceSet *party = (MenuResourceSet *)owner->menu;

    mnuDestroyResourceOwnerWindowContainers(owner);
    func_002ACB38(context);
    sdfReleaseResourceAllocation(party->allocation);
    return 1;
}

extern s32 mnuMapPadMaskToFlags(s32);
extern void mnuSetPopupEntry(s32, s32);
extern void mnuSetPopupEntryFlagged(s32, s32);
extern void mnuConfigurePanelResource(s32, s32, s32, s32);
extern void func_002B9808(s32);
extern void mnuRetreatWindowListSelection(s32);
extern void mnuAdvanceWindowListSelection(s32);
extern void mnuClearWindowPanelTransitionFlag(s32);
extern void mnuPlayInputSound(s32, s32, MenuStaffWindow *);
extern char D_003E7450[];
extern char D_003E746C[];
extern char D_003E7488[];
extern char D_003E74A4[];
extern char D_003E74C0[];
extern char D_003E7418[];
extern s32 evtGetMessageWindowControlState(void);
extern void func_002C1B68(s32, s32);
extern s32 mnuGetAbilityByteCategory(u16);
extern void mnuClearActionFlags(s32, u8 *);
extern void mnuHandlePanelListPageJumpInput(u32, u32);
extern char D_003E7434[];
extern char D_003E74DC[];

/* Handle staff-item popup selection and idle-window navigation. */
s32 mnuHandleStaffPopupSelection(u64 callback) {
    MenuStaffContext *context;
    MenuResourceSet *resources;
    MenuStaffList *window;
    s32 *popup;
    u32 itemKind;
    u32 input;
    s32 result;

    context = (MenuStaffContext *)kwlnTaskGetUserValue();
    popup = (s32 *)((u8 *)context + 0x54);
    resources = (MenuResourceSet *)context->menu;
    input = mnuMapPadMaskToFlags(0x33);
    result = func_002C4038((s32)((u8 *)context + 8), popup, 0, callback);
    if (result != 0) {
        return result;
    }
    if (*popup == 0) {
        if (input & 1) {
            itemKind = context->activeWindow->window->selectedNode->label;
            switch (itemKind) {
            case 0:
                resources->selection = 0;
                mnuSetPopupEntry((s32)popup, (s32)D_003E7450);
                break;
            case 1:
                mnuSetPopupEntry((s32)popup, (s32)D_003E746C);
                break;
            case 2:
                mnuSetPopupEntry((s32)popup, (s32)D_003E7488);
                break;
            case 3:
                mnuSetPopupEntry((s32)popup, (s32)D_003E74A4);
                break;
            default:
                mnuSetPopupEntry((s32)popup, (s32)D_003E74C0);
                break;
            }
            mnuSeekListNode(0, ((MenuStaffList *)resources->windows[0])->window);
            mnuSeekListNode(0, ((MenuStaffList *)resources->windows[1])->window);
        }
        if (input & 2) {
            mnuSetPopupEntryFlagged((s32)popup, (s32)D_003E7418);
            mnuConfigurePanelResource(*(s32 *)((u8 *)context + 0x118), context->group, 0, 1);
            mnuBeginWindowFadeTransition(*(s32 *)((u8 *)context + 0x104), (s32)context->tail);
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
            mnuPlayInputSound(0, input, window->window);
        }
    }
    return 0;
}

extern void func_002AAC98(s32, s32, s32, s32, s32, s32);
extern void mnuUpdateAndDrawWindowTransition(s32, s32, s32, s32, s32);
extern void func_002AA7A0(s32, s32);
extern u8 D_003E7050[];

s32 func_002ACE58(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_002AAE80(callback);
    mnuCreateStaffImageSprite(4);
    func_002AAC98(0,
        ((MenuStaffContext *)context)->activeWindow->window->selectedNode->label,
        (s32)D_003E7050, context, 1, 0x53);
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, (s32)((MenuStaffContext *)context)->tail, 0x53);
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return menuSetHandler(context, 1, callback);
}

s32 mnuFinishStaffReturnPopup(s32 callback) {
    return menuSetHandler(kwlnTaskGetUserValue(), 2, callback);
}

extern s32 func_002C5A28(s32, s32, s32, s32);
extern s32 evtGetIndexedEventRecordId(s32);
extern s32 ptySkillApplyFieldUseEffect(s32, s32, s32, s32);
extern void ptyAdjustItemQuantity(s32, s32);
extern void mnuInitPartyPanelSlots(s32);
extern void func_002BCA98(s32);
extern void func_002BCAB0(s32);

typedef struct StaffUseSelectionList {
    u8 pad00[0x1C];
    s32 *selectedIndex; /* 0x1C */
} StaffUseSelectionList;

typedef struct StaffUseContext {
    u8 pad00[0xA914];
    StaffUseSelectionList *list; /* 0xA914 */
} StaffUseContext;

s32 mnuUseStaffItem(itemId, context)
s32 itemId;
s32 context;
{
    s32 partyPanel = context + 0x284;
    s32 targetUnit = datGameState + *(((StaffUseContext *)context)->list->selectedIndex) * 0x1C4 + 0xA60;
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
    mnuInitPartyPanelSlots(context + 0xA928);
    func_002BCA98(partyPanel);
    func_002BCAB0(partyPanel);
    return 1;
}

/* On successful item use, publish the remaining count and record the selected item. */
void mnuApplyResourceSelection(s32 index, s32 context) {
    MenuResourceSet *resources;
    s32 consumed;

    resources = (MenuResourceSet *)((MenuStaffContext *)context)->menu;
    consumed = mnuUseStaffItem();
    if (consumed != 0) {
        ((MenuStaffList *)resources->windows[0])->window->selectedNode->label =
                  (u32)((SaveItemCounts *)datGameState)->counts[index];
        resources->selection = index;
    }
    func_002C1B68(context + 0xaa50, 1);
}

u32 func_002AD0A8(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuBeginWindowFadeTransition(((MenuResourceSet *)((MenuStaffContext *)context)->menu)->windows[0], context + 0xb10c);
    return 1;
}

u32 func_002AD0E8(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuBeginWindowFadeTransition((u32)((MenuStaffContext *)context)->activeWindow, (s32)((MenuStaffContext *)context)->tail);
    return 1;
}

s32 func_002AD118(u64 callback) {
    MenuStaffContext *context;
    MenuResourceSet *resources;
    MenuStaffList *window;
    MenuStaffWindow *panel;
    MenuStaffNode *node;
    s32 *popup;
    s32 itemId;
    u32 input;
    s32 result;

    context = (MenuStaffContext *)kwlnTaskGetUserValue();
    resources = (MenuResourceSet *)context->menu;
    popup = (s32 *)((u8 *)context + 0x54);
    input = mnuMapPadMaskToFlags(0xC33);
    result = func_002C4038((s32)((u8 *)context + 8), popup, 0, callback);
    if (result != 0) {
        return result;
    }
    if (evtGetMessageWindowControlState() != 0) {
        return 0;
    }
    func_002C1B68((s32)((u8 *)context + 0xAA50), 0);
    if (resources->selection == 0) {
        if (input & 1) {
            panel = ((MenuStaffList *)resources->windows[0])->window;
            if (panel->panelActive != 0) {
                node = panel->selectedNode;
                if (node->flags == 0) {
                    itemId = node->entryIndex;
                    if (mnuGetAbilityByteCategory(evtGetIndexedEventRecordId(itemId)) == 0) {
                        mnuApplyResourceSelection(itemId, (s32)context);
                    } else {
                        mnuSetPopupEntry((s32)popup, (s32)D_003E74DC);
                    }
                } else {
                    input = 0x8000;
                }
            } else {
                input = 0;
            }
        }
        if (input & 2) {
            mnuSetPopupEntryFlagged((s32)popup, (s32)D_003E7434);
        }
    } else {
        if (mnuIsStaffWindowReadyForItem(resources->selection & 0xFFFF, context) == 0) {
            mnuSetPopupEntryFlagged((s32)popup, (s32)D_003E7434);
            mnuClearActionFlags(0, (u8 *)&context->windowFlags);
            mnuBeginWindowFadeTransition(0, (s32)context->tail);
        } else {
            resources->selection = 0;
        }
    }
    window = (MenuStaffList *)resources->windows[0];
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
        mnuPlayInputSound(0, input, window->window);
    }
    return 0;
}

extern s32 D_00435E70;
extern void func_002AAC70(s32, s32, s32, s32, s32, s32, s32);

/* Refresh a staff window's description from the selected item's label. */
void mnuRefreshStaffWindowDescription(MenuStaffContext *context, s32 windowIndex) {
    MenuResourceSet *resources = (MenuResourceSet *)context->menu;
    MenuStaffNode *node = ((MenuStaffList *)resources->windows[windowIndex])->window->selectedNode;
    s32 emptyLabel = 0;
    s32 selectedLabel = node->entryIndex;

    if (node->label != 0) {
        func_002AAC70(0, selectedLabel, D_00435E70, (s32)context, 1, 1, 0x53);
    } else {
        func_002AAC98(0, emptyLabel, 0, (s32)context, 1, 0x53);
    }
}
