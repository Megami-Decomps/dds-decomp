#include "sdk_ps2.h"
#include "evt_message_window_api.h"
#include "dat_command.h"
#include "mnu_input.h"
#include "kwln.h"
#include "mnu.h"
#include "mnu_staff.h"
#include "sdf_resource.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "mnu_scroll_panel.h"
#include "dat_state.h"

typedef struct StaffDisplayContext StaffDisplayContext;
extern void mnuCreateStaffItemWindow(StaffDisplayContext *);


typedef struct {
    struct sdfMemHandle_t *allocation; /* 0x00 */
    u32 pad04;       /* 0x04 */
    MenuWindowContainer *windows[3]; /* 0x08 */
    u8 pad14[0x14];
    s32 selection; /* 0x28 */
} StaffWindowResources;


/* One staff display work allocation owns the window resources and party target list. */
struct StaffDisplayContext {
    u8 pad00[0x54];
    s32 popupState;
    u8 pad58[0x14];
    s32 unk6C;
    u8 pad70[4];
    s32 unk74;  /* 0x74 */
    struct EffectSlotSet *group; /* 0x78: staff image resources */
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
    u8 pad7DC[0x10];
    PartyPanel partyPanel; /* 0x7EC: five party display rows */
    u8 pad8F8[0x14];
    StaffWindowResources *resources; /* 0x90C */
    s32 displayMode; /* 0x910 */
    MenuGradientFade gradient; /* 0x914 */
    u8 pad920[4];
};
typedef char StaffDisplayContext_size[(sizeof(StaffDisplayContext) == 0x924) ? 1 : -1];



typedef struct MenuWindowSpriteGroup MenuWindowSpriteGroup;
extern void mnuDrawStaffItemListEntry(s32, s32, s32, struct MenuList *, struct MenuListNode *, s32);
extern u8 mnuIsBulletItemId(s32);
extern char *D_003BAA84;
extern u8 mnuIsSolarPricedItemId(s32);
extern u8 evtCheckValueThreshold(s32 itemId, s32 minimumQuantity);
extern void mnuAttachWindowTextureState(MenuWindowContainer *, u32, u32, u32, u32);

void mnuCreateStaffItemWindow(StaffDisplayContext *context) {
    StaffWindowResources *resources = context->resources;
    MenuWindowContainer *window;
    struct MenuListNode *node;
    s32 itemId;
    s32 textOffset;
    s32 secondTextOffset;
    s32 minimumQuantity;

    window = mnuCreateWindowContainer(0, 0x60, 0x10, 8, 0x15);
    mnuSetWindowFadeScale(window, 0x100);
    mnuSetWindowPanelBounds(window, context->panelLayout, 0x30, 0x530, -0x90, 0xA10);
    mnuInitializeWindowEntryPlacement(0, window, (struct EffectSlotSet *)context->unk74, 10, 0x10);
    window->list->context = context;
    window->list->drawCallback = mnuDrawStaffItemListEntry;

    itemId = 1;
    textOffset = 0x19;
    do {
        u8 itemFlags;
        u32 quantity;

        if (DDS3_GBWK->inventory.counts[itemId] != 0) {
            itemFlags = datItemSkillRecords[itemId].flags;
            if (((itemFlags & 3) != 0 || mnuIsSolarPricedItemId(itemId) != 0) &&
                mnuIsBulletItemId(itemId) == 0) {
                node = mnuAppendWindowListNode(window, D_003BAA84 + textOffset);
                quantity = DDS3_GBWK->inventory.counts[itemId];
                node->sortKeySecondary = itemId;
                node->sortKeyPrimary = quantity;
                if ((datItemSkillRecords[itemId].flags & 1) == 0 || mnuIsSolarPricedItemId(itemId) != 0) {
                    node->flags48 |= 1;
                }
            }
        }
        itemId++;
        textOffset += 0x19;
    } while (itemId < 0xC0);

    itemId = 1;
    minimumQuantity = 1;
    resources->windows[0] = window;
    mnuForwardDupArg(window, (struct EffectSlotSet *)context->unk74, 0,
                     (struct EffectSlotSet *)context->unkD8, 0xD);
    mnuAttachWindowTextureState(resources->windows[0], -0xE0, 0x370, 0, context->unkDC);
    mnuConfigureWindowSpriteSlots(resources->windows[0]->textures,
                                  (struct EffMappedResource *)context->spriteResource);

    window = mnuCreateWindowContainer(0, 0x1B0, 0x10, 8, 0x15);
    mnuSetWindowFadeScale(window, 0x100);
    mnuSetWindowPanelBounds(window, context->panelLayout, 0x30, 0x530, -0x90, 0xA10);
    mnuInitializeWindowEntryPlacement(0, window, (struct EffectSlotSet *)context->unk74, 10, 0x10);

    secondTextOffset = 0x19;
    do {
        if (evtCheckValueThreshold(itemId, minimumQuantity) != 0 &&
            (datItemSkillRecords[itemId].flags & 4) != 0) {
            node = mnuAppendWindowListNode(window, D_003BAA84 + secondTextOffset);
            node->sortKeyPrimary = minimumQuantity;
            node->sortKeySecondary = itemId;
        }
        itemId++;
        secondTextOffset += 0x19;
    } while (itemId < 0xC0);

    resources->windows[1] = window;
    mnuForwardDupArg(window, (struct EffectSlotSet *)context->unk74, 0,
                     (struct EffectSlotSet *)context->unkD8, 0xE);
    mnuAttachWindowTextureState(resources->windows[1], -0xE0, 0x370, 0, context->unkDC);
    mnuConfigureWindowSpriteSlots(resources->windows[1]->textures,
                                  (struct EffMappedResource *)context->spriteResource);
}

extern void mnuDestroyWindowContainer(MenuWindowContainer *window);

void mnuReleaseStaffPrimaryWindows(StaffDisplayContext *context) {
    StaffWindowResources *resources;

    resources = context->resources;
    mnuDestroyWindowContainer(resources->windows[0]);
    mnuDestroyWindowContainer(resources->windows[1]);
}

extern void mnuRemoveWindowListCursorNode(MenuWindowContainer *);

/* Remove the item when its inventory reaches zero; report whether any window
 * entries remain. */
MnuStaffWindowEntryPresence mnuHasStaffWindowEntriesForItem(
    s32 itemId, StaffDisplayContext *context) {
    StaffWindowResources *resources = context->resources;

    if (DDS3_GBWK->inventory.counts[itemId & 0xFFFF] == 0) {
        mnuRemoveWindowListCursorNode(resources->windows[0]);
    }
    return resources->windows[0]->list->count != 0;
}


/* Create the extra staff window and populate it with owned bullet items. */
void mnuCreateStaffBulletItemWindow(StaffDisplayContext *context) {
    StaffWindowResources *resources = context->resources;
    MenuWindowContainer *window;
    struct MenuListNode *node;
    s32 itemId = 1;
    s32 textOffset = 25;
    u32 quantity;

    window = mnuCreateWindowContainer(0, 0x60, 0x10, 8, 0x15);
    mnuSetWindowFadeScale(window, 0x100);
    mnuSetWindowPanelBounds(window, context->panelLayout, 0x30, 0x530, -0x90, 0xA10);
    mnuInitializeWindowEntryPlacement(0, window, (struct EffectSlotSet *)context->unk74, 10, 16);
    window->list->context = context;
    window->list->drawCallback = mnuDrawStaffItemListEntry;
    do {
        if (DDS3_GBWK->inventory.counts[itemId] != 0 && mnuIsBulletItemId(itemId)) {
            node = mnuAppendWindowListNode(window, D_003BAA84 + textOffset);
            quantity = DDS3_GBWK->inventory.counts[itemId];
            node->sortKeySecondary = itemId;
            node->sortKeyPrimary = quantity;
        }
        itemId++;
        textOffset += 25;
    } while (itemId < 0xC0);
    resources->windows[2] = window;
    mnuForwardDupArg(window, (struct EffectSlotSet *)context->unk74, 0,
                     (struct EffectSlotSet *)context->unkD8, 15);
    mnuAttachWindowTextureState(resources->windows[2], -0xE0, 0x370, 0, context->unkDC);
    mnuConfigureWindowSpriteSlots(resources->windows[2]->textures,
                                  (struct EffMappedResource *)context->spriteResource);
}

void mnuReleaseStaffExtraWindow(StaffDisplayContext *context) {
    mnuDestroyWindowContainer(context->resources->windows[2]);
}

/* Refresh the sorted bullet-item list after equipment changes. */
s32 mnuRefreshStaffBulletItemList(s32 itemId, s32 previousItemId, void *owner) {
    s32 previousCursorId = -1;
    s32 needsInsertion = 0;
    s32 insertBefore = 0;
    StaffWindowResources *resources = ((StaffDisplayContext *)owner)->resources;
    MenuWindowContainer *window;
    struct MenuListNode *node;
    u8 quantity;

    if (itemId == 0 && previousItemId == 0) {
        return 1;
    }
    if (DDS3_GBWK->inventory.counts[previousItemId] == 0) {
        window = resources->windows[2];
        previousCursorId = window->list->cursor->index;
        if (window->list->cursor == window->list->last) {
            previousCursorId = -2;
        }
        mnuRemoveWindowListCursorNode(resources->windows[2]);
        window = resources->windows[2];
    } else {
        window = resources->windows[2];
        node = window->list->cursor;
        quantity = DDS3_GBWK->inventory.counts[previousItemId];
        node->sortKeyPrimary = quantity;
    }

    for (node = window->list->first; node != NULL; node = node->next) {
        if ((u32)itemId == node->sortKeySecondary) {
            break;
        }
        if ((u32)itemId < node->sortKeySecondary) {
            insertBefore = 1;
            needsInsertion = 1;
            break;
        }
    }
    if (node == NULL || needsInsertion != 0) {
        node = mnuInsertWindowListNodeRelativeToAnchor(
            window, node, D_003BAA84 + itemId * 25, previousCursorId, insertBefore);
        window = resources->windows[2];
        quantity = DDS3_GBWK->inventory.counts[itemId];
        node->sortKeySecondary = itemId;
    } else {
        quantity = DDS3_GBWK->inventory.counts[itemId];
    }
    node->sortKeyPrimary = quantity;
    return window->list->count != 0;
}

struct StaffMenuWork;
extern void mnuSetStaffDisplayMode(s32 nextCategory, struct StaffMenuWork *menu);

void mnuSelectStaffDisplayCategory(u32 context) {
    mnuSetStaffDisplayMode(2, context);
}

void mnuNoOpStaffDisplayTeardown(StaffDisplayContext *context) {
}

s32 mnuInitializeStaffDisplayResources(dds3ProcessID_t *task) {
    StaffDisplayContext *context;
    StaffWindowResources *resources;
    struct sdfMemHandle_t *allocation;

    context = (StaffDisplayContext *)dds3GetProcessData(task);
    allocation = sdfMemAllocDirect(0x2C);
    resources = (void *)sdfMemLock(allocation);
    context->resources = resources;
    memset(resources, 0, 0x2C);
    resources->allocation = allocation;
    mnuSelectStaffDisplayCategory((u32)context);
    mnuCreateStaffItemWindow(context);
    mnuForwardDupArg(context->activeWindow, (struct EffectSlotSet *)context->unk74, 0, 0, 0);
    mnuActivatePanelAndConfigureGridResources(
        (MenuScrollPanel *)(u32)context->unk138,
        (struct EffectSlotSet *)(u32)context->unkD8, 0, 1);
    mnuSeekListNode(0, context->activeWindow->list);
    return 1;
}

s32 mnuStaffFreeDisplayResources(dds3ProcessID_t *task) {
    StaffDisplayContext *context = (StaffDisplayContext *)dds3GetProcessData(task);
    StaffWindowResources *resources = context->resources;
    mnuReleaseStaffPrimaryWindows(context);
    mnuNoOpStaffDisplayTeardown(context);
    sdfMemFreeDirect(resources->allocation);
    return 1;
}

extern void mnuClearWindowListSelectionFlags(MenuWindowContainer *);

extern void mnuPlayInputSound(s32, s32, u32 *);
extern MenuPopupEntry mnuCampItemUseListPopupEntry[];
extern MenuPopupEntry mnuCampAmmoPartySelectionPopupEntry[];
extern MenuPopupEntry mnuCampSpecialItemListPopupEntry[];
extern MenuPopupEntry mnuCampCommandSelectionPopupEntry[];

/* Handle staff-item selection and window navigation while the popup is idle. */
s32 mnuHandleStaffDisplayInput(dds3ProcessID_t *callback) {
    StaffDisplayContext *context;
    StaffWindowResources *resources;
    MenuWindowContainer *window;
    s32 *popup;
    s32 itemKind;
    u32 input;
    s32 result;

    context = (StaffDisplayContext *)dds3GetProcessData(callback);
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
                mnuSetPopupEntry(popup, mnuCampItemUseListPopupEntry);
                break;
            case 1:
                mnuSetPopupEntry(popup, mnuCampAmmoPartySelectionPopupEntry);
                break;
            default:
                mnuSetPopupEntry(popup, mnuCampSpecialItemListPopupEntry);
                break;
            }
            mnuSeekListNode(0, resources->windows[0]->list);
            mnuSeekListNode(0, resources->windows[1]->list);
        }
        if (input & 2) {
            mnuSetPopupEntryFlagged(popup, mnuCampCommandSelectionPopupEntry);
            mnuActivatePanelAndConfigureGridResources((MenuScrollPanel *)(u32)context->unk138,
                                                      (struct EffectSlotSet *)(u32)context->unk6C, 0, 1);
        }
        window = context->activeWindow;
        if (window != NULL) {
            if ((input & 0x300000) == 0) {
                mnuClearWindowListSelectionFlags(window);
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

extern void mnuCreateStaffImageSprite(s32);
extern void mnuDrawCampGridAndLabel(s32, s32, s32, s32, s32, s32, s32);
extern void mnuDrawCampGridAndLabelDefault(s32, s32, s32, s32, s32, s32);
extern u8 mnuCampItemCommandHelpTextBank[];
extern u8 *datItemMessages;

s32 mnuStaffDrawImagePanelA(dds3ProcessID_t *callback) {
    s32 context = dds3GetProcessData(callback);
    mnuDrawCampScreenDefault(callback);
    mnuCreateStaffImageSprite(4);
    mnuDrawCampGridAndLabelDefault(1, ((StaffDisplayContext *)context)->activeWindow->list->cursor->index, (s32)mnuCampItemCommandHelpTextBank, context, 1, 0x53);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, ((StaffDisplayContext *)context)->activeWindow, 0x53);
    mnuDrawStaffGridLabelsForKind(0, ((StaffDisplayContext *)context)->group);
    return menuRunPanel((void *)context, 1, (void *)callback);
}

s32 mnuStaffRunPanel2b(dds3ProcessID_t *request) {
    s32 state = dds3GetProcessData(request);

    return menuRunPanel((void *)state, 2, request);
}

extern s32 btlItemApplyDirectEffect(MenuPageWindow *, u16, s32, datUnitWork_t *);
extern u32 evtGetIndexedEventRecordId(s32);
extern void ptyAdjustItemQuantity(s32 itemId, s32 quantityDelta);
extern void mnuUpdatePartyPanelPercentDisplays(s32);

/* Use a field item: resolve its direct effect (or field-use skill) against the
 * active unit row; on success consume one from the inventory and refresh the
 * party panels. Returns 1 when the item was consumed. */
s32 mnuUseStaffItem(s32 itemId, StaffDisplayContext *context) {
    s32 partyPanel = (s32)context + 0x15C;
    s32 targetUnit = (s32)&DDS3_GBWK->party[context->selectionList->cursor->index];
    s32 result = btlItemApplyDirectEffect((MenuPageWindow *)partyPanel, itemId & 0xFFFF,
                                         targetUnit, (datUnitWork_t *)targetUnit);

    if (result != 1) {
        if (result == 2) {
            return 0;
        }
        if (ptySkillApplyFieldUseEffect((MenuPageWindow *)partyPanel, evtGetIndexedEventRecordId(itemId) & 0xFFFF, (datUnitWork_t *)targetUnit, (datUnitWork_t *)targetUnit) == 0) {
            return 0;
        }
    }
    ptyAdjustItemQuantity(itemId, -1);
    mnuInitPartyPanelSlots(&context->partyPanel);
    mnuUpdateHandleStates((MenuPageWindow *)partyPanel);
    mnuUpdatePartyPanelPercentDisplays(partyPanel);
    return 1;
}

extern void mnuSetStateValue(u32 *, u32);

/* On successful item use, publish the remaining count and record the selected item. */
void mnuRefreshStaffItemSelection(s32 selection, StaffDisplayContext *context) {
    StaffWindowResources *resources;
    s32 consumed;

    resources = context->resources;
    consumed = mnuUseStaffItem(selection, context);
    if (consumed != 0) {
        resources->windows[0]->list->cursor->sortKeyPrimary =
                  DDS3_GBWK->inventory.counts[selection];
        resources->selection = selection;
    }
    mnuSetStateValue(&((StaffDisplayContext *)context)->gradient.active, 1);
}

extern s32 mnuGetAbilityTargetCategory(u16);
extern MenuPopupEntry mnuCampItemTargetSelectionPopupEntry[];
extern MenuPopupEntry mnuCampItemCategoryPopupEntry[];

/* Use an eligible staff item and update its selection window while messages are idle. */
s32 mnuHandleStaffInventoryListInput(dds3ProcessID_t *task) {
    StaffDisplayContext *context;
    StaffWindowResources *resources;
    MenuWindowContainer *window;
    struct MenuList *list;
    struct MenuListNode *node;
    s32 *popup;
    s32 item;
    s32 result;
    u32 input;

    context = (StaffDisplayContext *)dds3GetProcessData(task);
    popup = &context->popupState;
    resources = context->resources;
    input = mnuMapPadMaskToFlags(0x33);
    result = menuRunPanel(context, 0, (void *)task);
    if (result != 0) {
        return result;
    }
    if (fclChkMessage() == EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL) {
        mnuSetStateValue(&context->gradient.active, 0);
        if (input & 1) {
            list = resources->windows[0]->list;
            if (list->count != 0) {
                node = list->cursor;
                if (node->flags48 == 0) {
                    item = node->sortKeySecondary;
                    if (mnuGetAbilityTargetCategory((u16)evtGetIndexedEventRecordId(item)) == 0) {
                        mnuRefreshStaffItemSelection(item, context);
                    } else {
                        mnuSetPopupEntry(popup, mnuCampItemTargetSelectionPopupEntry);
                    }
                } else {
                    input = 0x8000;
                }
            } else {
                input = 0;
            }
        }
        if (input & 2) {
            mnuSetPopupEntry(popup, mnuCampItemCategoryPopupEntry);
        }
        window = resources->windows[0];
        if (window != NULL) {
            if ((input & 0x300000) == 0) {
                mnuClearWindowListSelectionFlags(window);
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


/* Refresh the description panel from the selected node in one of the staff-menu windows. */
void mnuRefreshStaffWindowDescription(StaffDisplayContext *context, s32 windowIndex) {
    StaffWindowResources *resources = context->resources;
    struct MenuListNode *node = resources->windows[windowIndex]->list->cursor;
    s32 emptyLabel = 0;
    s32 selectedLabel = node->sortKeySecondary;

    if (node->sortKeyPrimary != 0) {
        mnuDrawCampGridAndLabel(1, selectedLabel, datItemMessages, (s32)context, 1, 1, 0x53);
    } else {
        mnuDrawCampGridAndLabelDefault(1, emptyLabel, 0, (s32)context, 1, 0x53);
    }
}
