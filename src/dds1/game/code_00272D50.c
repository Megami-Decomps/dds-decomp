#include "mnu.h"

extern s32 mnuUseStaffItem(s32, s32);

extern s32 kwlnTaskGetUserValue();

extern u8 *datGameState;

extern s32 func_00285670(s32, s32 *, u64, u64);
extern s32 sdfAllocGeneralBlock(s32);
extern void *sdfResourceRetainAddress(s32);
extern void *memset(void *, s32, u32);
extern void func_00272D50(s32);
struct StaffWindowHeader;
extern void mnuForwardDupArg(struct StaffWindowHeader *, s32, s32, s32, s32);
extern void mnuActivatePanelAndConfigureGridResources(s32, s32, s32, s32);
extern s32 mnuSeekListNode(s32, s32);


typedef struct {
    u32 allocation; /* 0x00 */
    u32 pad04;       /* 0x04 */
    struct StaffWindowHeader *firstWindow; /* 0x08 */
    struct StaffWindowHeader *secondWindow; /* 0x0C */
    struct StaffWindowHeader *thirdWindow; /* 0x10 */
    u8 pad14[0x14];
    s32 selection; /* 0x28 */
} StaffWindowResources;

typedef struct StaffDescriptionNode {
    u8 pad00[0x60];
    s32 sortKeyPrimary;   /* 0x60 */
    s32 sortKeySecondary; /* 0x64 */
} StaffDescriptionNode;

typedef struct StaffDescriptionList {
    u8 pad00[0x1C];
    StaffDescriptionNode *selectedNode; /* 0x1C */
} StaffDescriptionList;

typedef struct StaffDescriptionWindow {
    u8 pad00[0x14];
    StaffDescriptionList *list; /* 0x14 */
} StaffDescriptionWindow;

typedef struct StaffDescriptionResources {
    u8 pad00[0x08];
    StaffDescriptionWindow *windows[3]; /* 0x08 */
} StaffDescriptionResources;

/* Inventory quantities are byte entries indexed by each caller's original item ID. */
typedef struct SaveItemCounts {
    u8 pad00[0x12A0];
    u8 counts[0x100];
} SaveItemCounts;

typedef struct StaffWindowNode {
    s32 label;
    u8 pad04[0x5C];
    u32 unk60; /* List payload word updated with the selected item's count. */
    u32 itemId;
} StaffWindowNode;

typedef struct StaffWindowData {
    u8 pad00[0x1C];
    StaffWindowNode *selectedNode;
    u32 remaining;
    u8 pad24[8];
    void (*drawCallback)(s32, s32, s32, struct StaffWindowData *, StaffWindowNode *, s32);
    void *drawContext;
} StaffWindowData;

typedef struct StaffWindowHeader {
    u8 pad00[0x14];
    StaffWindowData *data;
    u8 pad18[0x6C];
    struct MenuWindowSpriteGroup *textureGroup;
} StaffWindowHeader;

typedef struct StaffSelectionList {
    u8 pad00[0x1C];
    s32 *selectedIndex;
} StaffSelectionList;

/* One staff display work allocation owns the window resources and party target list. */
typedef struct {
    u8 pad00[0x6C];
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
    StaffWindowHeader *activeWindow; /* 0x128 */
    u8 pad12C[0xC];
    s32 unk138; /* 0x138 */
    u8 pad13C[0x69C];
    StaffSelectionList *selectionList; /* 0x7D8 */
    u8 pad7DC[0x130];
    StaffWindowResources *resources; /* 0x90C */
} StaffDisplayContext;

extern void mnuDestroyWindowContainer(StaffWindowHeader *);


INCLUDE_ASM(const s32, "game/code_00272D50", func_00272D50);

void mnuReleaseStaffPrimaryWindows(StaffDisplayContext *context) {
    StaffWindowResources *resources;

    resources = context->resources;
    mnuDestroyWindowContainer(resources->firstWindow);
    mnuDestroyWindowContainer(resources->secondWindow);
}

extern void func_0027C6A0(StaffWindowHeader *);

/* Refresh the bullet-item window if inventory is empty; return whether the
 * window still has entries. */
s32 mnuIsStaffWindowReadyForItem(s32 itemId, s32 context) {
    StaffWindowResources *resources = ((StaffDisplayContext *)context)->resources;

    if (((SaveItemCounts *)datGameState)->counts[itemId & 0xFFFF] == 0) {
        func_0027C6A0(resources->firstWindow);
    }
    return resources->firstWindow->data->remaining > 0;
}

typedef struct MenuWindowSpriteGroup MenuWindowSpriteGroup;
extern s32 mnuCreateWindowContainer(s32, s32, s32, s32, s32);
extern void mnuSetWindowContainerState(StaffWindowHeader *, u32);
extern void mnuSetWindowPanelBounds(StaffWindowHeader *, const void *, u32, u32, u32, u32);
extern void mnuInitializeWindowEntryPlacement(s32, StaffWindowHeader *, s32, s32, s32);
extern void func_00272BC0(s32, s32, s32, StaffWindowData *, StaffWindowNode *, s32);
extern s32 mnuIsBulletItemId(s32);
extern StaffWindowNode *mnuAppendWindowListNode(StaffWindowHeader *, s32);
extern char *D_003BAA84;
extern void mnuAttachWindowTextureState(StaffWindowHeader *, u32, u32, u32, u32);
extern void mnuConfigureWindowSpriteSlots(MenuWindowSpriteGroup *, u32);

/* Create the extra staff window and populate it with owned bullet items. */
void mnuCreateStaffBulletItemWindow(StaffDisplayContext *context) {
    StaffWindowResources *resources = context->resources;
    StaffWindowHeader *window;
    StaffWindowNode *node;
    s32 itemId = 1;
    s32 textOffset = 25;
    u32 quantity;

    window = (StaffWindowHeader *)mnuCreateWindowContainer(0, 0x60, 0x10, 8, 0x15);
    mnuSetWindowContainerState(window, 0x100);
    mnuSetWindowPanelBounds(window, context->panelLayout, 0x30, 0x530, -0x90, 0xA10);
    mnuInitializeWindowEntryPlacement(0, window, context->unk74, 10, 16);
    window->data->drawContext = context;
    window->data->drawCallback = func_00272BC0;
    do {
        if (((SaveItemCounts *)datGameState)->counts[itemId] != 0 && mnuIsBulletItemId(itemId)) {
            node = mnuAppendWindowListNode(window, (s32)(D_003BAA84 + textOffset));
            quantity = ((SaveItemCounts *)datGameState)->counts[itemId];
            node->itemId = itemId;
            node->unk60 = quantity;
        }
        itemId++;
        textOffset += 25;
    } while (itemId < 0xC0);
    resources->thirdWindow = window;
    mnuForwardDupArg(window, context->unk74, 0, context->unkD8, 15);
    mnuAttachWindowTextureState(resources->thirdWindow, -0xE0, 0x370, 0, context->unkDC);
    mnuConfigureWindowSpriteSlots(resources->thirdWindow->textureGroup, context->spriteResource);
}

void mnuReleaseStaffExtraWindow(StaffDisplayContext *context) {
    mnuDestroyWindowContainer(context->resources->thirdWindow);
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
    mnuSeekListNode(0, (s32)context->activeWindow->data);
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
extern void func_0027C788(s32);
extern void mnuRetreatWindowListSelection(s32);
extern void mnuAdvanceWindowListSelection(s32);
extern void mnuClearWindowPanelTransitionFlag(s32);
extern void mnuPlayInputSound(s32, s32, s32);
extern char D_0037C9C8[];
extern char D_0037C9E4[];
extern char D_0037CA00[];
extern char D_0037C990[];

/* Handle staff-item selection and window navigation while the popup is idle. */
s32 func_002734C0(s32 callback) {
    StaffDisplayContext *context;
    StaffWindowResources *resources;
    StaffWindowHeader *window;
    s32 *popup;
    s32 itemKind;
    u32 input;
    s32 result;

    context = (StaffDisplayContext *)kwlnTaskGetUserValue();
    popup = (s32 *)((u8 *)context + 0x54);
    resources = context->resources;
    input = mnuMapPadMaskToFlags(0x33);
    result = menuRunPanel((s32)context, 0, callback);
    if (result != 0) {
        return result;
    }
    if (*popup == 0) {
        if (input & 1) {
            itemKind = context->activeWindow->data->selectedNode->label;
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
            mnuSeekListNode(0, (s32)resources->firstWindow->data);
            mnuSeekListNode(0, (s32)resources->secondWindow->data);
        }
        if (input & 2) {
            mnuSetPopupEntryFlagged((s32)popup, (s32)D_0037C990);
            mnuActivatePanelAndConfigureGridResources((s32)context->unk138,
                                                      context->unk6C, 0, 1);
        }
        window = context->activeWindow;
        if (window != NULL) {
            if ((input & 0x300000) == 0) {
                func_0027C788((s32)window);
            }
            if (input & 0x10) {
                mnuRetreatWindowListSelection((s32)window);
            }
            if (input & 0x20) {
                mnuAdvanceWindowListSelection((s32)window);
            }
            mnuClearWindowPanelTransitionFlag((s32)window);
            mnuPlayInputSound(0, input, (s32)window->data);
        }
    }
    return 0;
}

extern void func_00272778(s32);
extern void mnuCreateStaffImageSprite(s32);
extern void func_00272518(s32, s32, s32, s32, s32, s32, s32);
extern void func_00272668(s32, s32, s32, s32, s32, s32);
extern void mnuDrawWindowContainer(s32, s32, s32, s32, s32);
extern void func_002723B0(s32, s32);
extern u8 D_0037C860[];
extern s32 D_003BAA9C;

s32 mnuStaffDrawImagePanelA(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_00272778(callback);
    mnuCreateStaffImageSprite(4);
    func_00272668(1, ((StaffDisplayContext *)context)->activeWindow->data->selectedNode->label, (s32)D_0037C860, context, 1, 0x53);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)((StaffDisplayContext *)context)->activeWindow, 0x53);
    func_002723B0(0, ((StaffDisplayContext *)context)->group);
    return menuRunPanel(context, 1, callback);
}

s32 mnuStaffRunPanel2b(u64 request) {
    s32 state = kwlnTaskGetUserValue();

    return menuRunPanel(state, 2, request);
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
    s32 targetUnit = datGameState + *(((StaffDisplayContext *)context)->selectionList->selectedIndex) * 0x1A4 + 0xA60;
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
        resources->firstWindow->data->selectedNode->unk60 =
                  (u32)((SaveItemCounts *)datGameState)->counts[selection];
        resources->selection = selection;
    }
    func_00283BF0(context + 0x914, 1);
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002738A0);

/* Refresh the description panel from the selected node in one of the staff-menu windows. */
void func_00273A30(StaffDisplayContext *context, s32 windowIndex) {
    StaffDescriptionResources *resources = (StaffDescriptionResources *)context->resources;
    StaffDescriptionNode *node = resources->windows[windowIndex]->list->selectedNode;
    s32 emptyLabel = 0;
    s32 selectedLabel = node->sortKeySecondary;

    if (node->sortKeyPrimary != 0) {
        func_00272518(1, selectedLabel, D_003BAA9C, (s32)context, 1, 1, 0x53);
    } else {
        func_00272668(1, emptyLabel, 0, (s32)context, 1, 0x53);
    }
}
