#include "mnu.h"
#include "mnu_list.h"
#include "mnu_shop.h"

extern s32 mnuUseStaffItem(s32, s32);

extern s32 kwlnTaskGetUserValue();

extern u8 *datGameState;

extern s32 sdfAllocGeneralBlock(s32);
extern void *sdfResourceRetainAddress(s32);
extern void *memset(void *, s32, u32);
extern void func_00272D50(s32);
extern void mnuForwardDupArg(MenuWindowContainer *, s32, s32, s32, s32);
extern void mnuActivatePanelAndConfigureGridResources(s32, s32, s32, s32);
extern s32 mnuSeekListNode(s32, s32);


typedef struct {
    u32 allocation; /* 0x00 */
    u32 pad04;       /* 0x04 */
    MenuWindowContainer *windows[3]; /* 0x08 */
    u8 pad14[0x14];
    s32 selection; /* 0x28 */
} StaffWindowResources;


/* Inventory quantities are byte entries indexed by each caller's original item ID. */
typedef struct SaveItemCounts {
    u8 pad00[0x12A0];
    u8 counts[0x100];
} SaveItemCounts;


/* One staff display work allocation owns the window resources and party target list. */
typedef struct {
    u8 pad00[0x54];
    s32 popupState;
    u8 pad58[0x14];
    s32 unk6C;
    u8 pad70[4];
    s32 unk74;  /* 0x74 */
    s32 group; /* 0x78: staff image group */
    u8 pad7C[0x5C];
    s32 unkD8;  /* 0xD8 */
    u32 unkDC;
    u8 padE0[0x38];
    u32 spriteResource;
    void *panelLayout;
    u8 pad120[8];
    MenuWindowContainer *activeWindow; /* 0x128 */
    u8 pad12C[0xC];
    s32 unk138; /* 0x138 */
    u8 pad13C[0x69C];
    struct MenuList *selectionList; /* 0x7D8 */
    u8 pad7DC[0x130];
    StaffWindowResources *resources; /* 0x90C */
} StaffDisplayContext;

extern void mnuDestroyWindowContainer(MenuWindowContainer *);


INCLUDE_ASM(const s32, "game/code_00272D50", func_00272D50);

void mnuReleaseStaffPrimaryWindows(StaffDisplayContext *context) {
    StaffWindowResources *resources;

    resources = context->resources;
    mnuDestroyWindowContainer(resources->windows[0]);
    mnuDestroyWindowContainer(resources->windows[1]);
}

extern void mnuRemoveWindowListCursorNode(MenuWindowContainer *);

/* Refresh the bullet-item window if inventory is empty; return whether the
 * window still has entries. */
s32 mnuIsStaffWindowReadyForItem(s32 itemId, s32 context) {
    StaffWindowResources *resources = ((StaffDisplayContext *)context)->resources;

    if (((SaveItemCounts *)datGameState)->counts[itemId & 0xFFFF] == 0) {
        mnuRemoveWindowListCursorNode(resources->windows[0]);
    }
    return resources->windows[0]->list->count != 0;
}

typedef struct MenuWindowSpriteGroup MenuWindowSpriteGroup;
extern s32 mnuCreateWindowContainer(s32, s32, s32, s32, s32);
extern void mnuSetWindowContainerState(MenuWindowContainer *, u32);
extern void mnuSetWindowPanelBounds(MenuWindowContainer *, const void *, u32, u32, u32, u32);
extern void mnuInitializeWindowEntryPlacement(s32, MenuWindowContainer *, s32, s32, s32);
extern void func_00272BC0(s32, s32, s32, struct MenuList *, struct MenuListNode *, s32);
extern s32 mnuIsBulletItemId(s32);
extern struct MenuListNode *mnuAppendWindowListNode(MenuWindowContainer *, s32);
extern char *D_003BAA84;
extern void mnuAttachWindowTextureState(MenuWindowContainer *, u32, u32, u32, u32);
extern void mnuConfigureWindowSpriteSlots(MenuWindowSpriteGroup *, u32);

/* Create the extra staff window and populate it with owned bullet items. */
void mnuCreateStaffBulletItemWindow(StaffDisplayContext *context) {
    StaffWindowResources *resources = context->resources;
    MenuWindowContainer *window;
    struct MenuListNode *node;
    s32 itemId = 1;
    s32 textOffset = 25;
    u32 quantity;

    window = (MenuWindowContainer *)mnuCreateWindowContainer(0, 0x60, 0x10, 8, 0x15);
    mnuSetWindowContainerState(window, 0x100);
    mnuSetWindowPanelBounds(window, context->panelLayout, 0x30, 0x530, -0x90, 0xA10);
    mnuInitializeWindowEntryPlacement(0, window, context->unk74, 10, 16);
    window->list->context = context;
    window->list->drawCallback = func_00272BC0;
    do {
        if (((SaveItemCounts *)datGameState)->counts[itemId] != 0 && mnuIsBulletItemId(itemId)) {
            node = mnuAppendWindowListNode(window, (s32)(D_003BAA84 + textOffset));
            quantity = ((SaveItemCounts *)datGameState)->counts[itemId];
            node->sortKeySecondary = itemId;
            node->sortKeyPrimary = quantity;
        }
        itemId++;
        textOffset += 25;
    } while (itemId < 0xC0);
    resources->windows[2] = window;
    mnuForwardDupArg(window, context->unk74, 0, context->unkD8, 15);
    mnuAttachWindowTextureState(resources->windows[2], -0xE0, 0x370, 0, context->unkDC);
    mnuConfigureWindowSpriteSlots(resources->windows[2]->textures, context->spriteResource);
}

void mnuReleaseStaffExtraWindow(StaffDisplayContext *context) {
    mnuDestroyWindowContainer(context->resources->windows[2]);
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273220);

void func_00273390(u32 context) {
    mnuSetStaffDisplayMode(2, context);
}

void func_002733B0() {
}

s32 mnuInitializeStaffDisplayResources(void) {
    StaffDisplayContext *context;
    StaffWindowResources *resources;
    s32 handle;

    context = (StaffDisplayContext *)kwlnTaskGetUserValue();
    handle = sdfAllocGeneralBlock(0x2C);
    resources = sdfResourceRetainAddress(handle);
    context->resources = resources;
    memset(resources, 0, 0x2C);
    resources->allocation = handle;
    func_00273390((u32)context);
    func_00272D50((s32)context);
    mnuForwardDupArg(context->activeWindow, context->unk74, 0, 0, 0);
    mnuActivatePanelAndConfigureGridResources(context->unk138, context->unkD8, 0, 1);
    mnuSeekListNode(0, (s32)context->activeWindow->list);
    return 1;
}

s32 mnuStaffFreeDisplayResources(void) {
    StaffDisplayContext *context = (StaffDisplayContext *)kwlnTaskGetUserValue();
    StaffWindowResources *resources = context->resources;
    mnuReleaseStaffPrimaryWindows(context);
    func_002733B0(context);
    sdfReleaseResourceAllocation(resources->allocation);
    return 1;
}

extern s32 mnuMapPadMaskToFlags(s32);
extern void mnuSetPopupEntry(s32, s32);
extern void mnuSetPopupEntryFlagged(s32, s32);
extern void func_0027C788(MenuWindowContainer *);
extern void mnuRetreatWindowListSelection(MenuWindowContainer *);
extern void mnuAdvanceWindowListSelection(MenuWindowContainer *);
extern void mnuClearWindowPanelTransitionFlag(MenuWindowContainer *);
extern void mnuPlayInputSound(s32, s32, u32 *);
extern char D_0037C9C8[];
extern char D_0037C9E4[];
extern char D_0037CA00[];
extern char D_0037C990[];

/* Handle staff-item selection and window navigation while the popup is idle. */
s32 func_002734C0(s32 callback) {
    StaffDisplayContext *context;
    StaffWindowResources *resources;
    MenuWindowContainer *window;
    s32 *popup;
    s32 itemKind;
    u32 input;
    s32 result;

    context = (StaffDisplayContext *)kwlnTaskGetUserValue();
    popup = &context->popupState;
    resources = context->resources;
    input = mnuMapPadMaskToFlags(0x33);
    result = menuRunPanel(context, 0, (void *)callback);
    if (result != 0) {
        return result;
    }
    if (*popup == 0) {
        if (input & 1) {
            itemKind = context->activeWindow->list->cursor->index;
            switch (itemKind) {
            case 0:
                resources->selection = 0;
                mnuSetPopupEntry((s32)popup, (s32)D_0037C9C8);
                break;
            case 1:
                mnuSetPopupEntry((s32)popup, (s32)D_0037C9E4);
                break;
            default:
                mnuSetPopupEntry((s32)popup, (s32)D_0037CA00);
                break;
            }
            mnuSeekListNode(0, (s32)resources->windows[0]->list);
            mnuSeekListNode(0, (s32)resources->windows[1]->list);
        }
        if (input & 2) {
            mnuSetPopupEntryFlagged((s32)popup, (s32)D_0037C990);
            mnuActivatePanelAndConfigureGridResources((s32)context->unk138,
                                                      context->unk6C, 0, 1);
        }
        window = context->activeWindow;
        if (window != NULL) {
            if ((input & 0x300000) == 0) {
                func_0027C788(window);
            }
            if (input & 0x10) {
                mnuRetreatWindowListSelection(window);
            }
            if (input & 0x20) {
                mnuAdvanceWindowListSelection(window);
            }
            mnuClearWindowPanelTransitionFlag(window);
            mnuPlayInputSound(0, input, &window->list->stateFlags);
        }
    }
    return 0;
}

extern void func_00272778(s32);
extern void mnuCreateStaffImageSprite(s32);
extern void func_00272518(s32, s32, s32, s32, s32, s32, s32);
extern void func_00272668(s32, s32, s32, s32, s32, s32);
extern void mnuDrawWindowContainer(s32, s32, s32, MenuWindowContainer *, s32);
extern void func_002723B0(s32, s32);
extern u8 D_0037C860[];
extern s32 D_003BAA9C;

s32 mnuStaffDrawImagePanelA(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_00272778(callback);
    mnuCreateStaffImageSprite(4);
    func_00272668(1, ((StaffDisplayContext *)context)->activeWindow->list->cursor->index, (s32)D_0037C860, context, 1, 0x53);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, ((StaffDisplayContext *)context)->activeWindow, 0x53);
    func_002723B0(0, ((StaffDisplayContext *)context)->group);
    return menuRunPanel((void *)context, 1, (void *)callback);
}

s32 mnuStaffRunPanel2b(void *request) {
    s32 state = kwlnTaskGetUserValue();

    return menuRunPanel((void *)state, 2, request);
}

extern s32 btlItemApplyDirectEffect(s32, s32, s32, s32);
extern s32 ptySkillApplyFieldUseEffect(s32, s32, s32, s32);
extern s32 evtGetIndexedEventRecordId(s32);
extern void ptyAdjustItemQuantity(s32, s32);
extern void mnuInitPartyPanelSlots(s32);
extern void mnuUpdateHandleStates(s32);
extern void func_00280048(s32);

/* Use a field item: resolve its direct effect (or field-use skill) against the
 * active unit row; on success consume one from the inventory and refresh the
 * party panels. Returns 1 when the item was consumed. */
s32 mnuUseStaffItem(s32 itemId, s32 context) {
    s32 partyPanel = context + 0x15C;
    s32 targetUnit = datGameState + ((StaffDisplayContext *)context)->selectionList->cursor->index * 0x1A4 + 0xA60;
    s32 result = btlItemApplyDirectEffect(partyPanel, itemId & 0xFFFF, targetUnit, targetUnit);

    if (result != 1) {
        if (result == 2) {
            return 0;
        }
        if (ptySkillApplyFieldUseEffect(partyPanel, evtGetIndexedEventRecordId(itemId) & 0xFFFF, targetUnit, targetUnit) == 0) {
            return 0;
        }
    }
    ptyAdjustItemQuantity(itemId, -1);
    mnuInitPartyPanelSlots(context + 0x7EC);
    mnuUpdateHandleStates(partyPanel);
    func_00280048(partyPanel);
    return 1;
}

/* On successful item use, publish the remaining count and record the selected item. */
void mnuRefreshStaffItemSelection(s32 selection, s32 context) {
    StaffWindowResources *resources;
    s32 consumed;

    resources = ((StaffDisplayContext *)context)->resources;
    consumed = mnuUseStaffItem(selection, context);
    if (consumed != 0) {
        resources->windows[0]->list->cursor->sortKeyPrimary =
                  ((SaveItemCounts *)datGameState)->counts[selection];
        resources->selection = selection;
    }
    func_00283BF0(context + 0x914, 1);
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002738A0);

/* Refresh the description panel from the selected node in one of the staff-menu windows. */
void func_00273A30(StaffDisplayContext *context, s32 windowIndex) {
    StaffWindowResources *resources = context->resources;
    struct MenuListNode *node = resources->windows[windowIndex]->list->cursor;
    s32 emptyLabel = 0;
    s32 selectedLabel = node->sortKeySecondary;

    if (node->sortKeyPrimary != 0) {
        func_00272518(1, selectedLabel, D_003BAA9C, (s32)context, 1, 1, 0x53);
    } else {
        func_00272668(1, emptyLabel, 0, (s32)context, 1, 0x53);
    }
}
