#include "fr_font.h"
#include "mnu_input.h"
#include "itf_draw_grid.h"
#include "kwln.h"
#include "mnu.h"
#include "mnu_staff.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "mnu_camp_work.h"
#include "dat_state.h"
#include "eff.h"

extern void mnuCreateConfigTasks(s32 mode);

typedef struct FrFontGlyph FrFontGlyph;
extern u32 func_001978E8(s32, s32, s32, u32, char *, s32);
extern void frFontSetChildChainFirstOption(FrFontGlyph *, u8);
extern s32 frFontDrawGlyphChain(FrFontGlyph *, s8, u32);
extern s32 func_003014F0(char *, const char *, ...);
extern char D_003BC6C8[];
extern const char D_003BC6D0[];
extern char D_003BC6D8[];


extern s64 fileConsumeConfigTaskReady(void);

extern u8 D_0037C844[];

extern void mnuSetPopupEntryFlagged();

extern void mnuDrawBackdrop(s32, s32);

extern void func_0027E8D8(s32, s32, s32, u32, s32);

extern void mnuCreateStaffImageSprite(s32);

extern s32 func_002719F0(s32);
extern void mnuSetPopupEntry(s32, s32);
extern void func_0027C788(MenuWindowContainer *);

extern void mnuPlayInputSound(s32, s32, u32 *);
extern u8 D_0037C748[];





s32 mnuHandleCampFieldSkillInput(KwlnTask *callback) {
    StaffMenuWork *context;
    s32 *popup;
    u32 input;
    s32 state;

    context = (StaffMenuWork *)kwlnTaskGetUserValue(callback);
    input = mnuMapPadMaskToFlags(0x33);
    popup = &context->value54;
    state = func_00285670(&context->panel, popup, 0, (void *)callback);
    if (state != 0) {
        return state;
    }
    state = func_002719F0(callback);
    if (state == 0) {
        return state;
    }
    if (*popup == 0) {
        if (input & 1) {
            MenuWindowContainer *skillPanel = (MenuWindowContainer *)context->images[0];
            struct MenuListNode *entry = skillPanel->list->cursor;

            if ((entry->flags48 & 1) == 0) {
                u32 index = entry->sortKeyPrimary + 1;

                mnuSetPopupEntry((s32)popup, (s32)(D_0037C748 + index * 0x1C));
            } else {
                input = 0x8000;
            }
        }
        if (input & 2) {
            mnuSetPopupEntry((s32)popup, (s32)D_0037C748);
        }
    }
    if ((input & 0x300000) == 0) {
        func_0027C788((MenuWindowContainer *)context->images[0]);
    }
    if (input & 0x10) {
        mnuRetreatWindowListSelection((MenuWindowContainer *)context->images[0]);
    }
    if (input & 0x20) {
        mnuAdvanceWindowListSelection((MenuWindowContainer *)context->images[0]);
    }
    mnuClearWindowPanelTransitionFlag((MenuWindowContainer *)context->images[0]);
    mnuPlayInputSound(0, input,
                      &((MenuWindowContainer *)context->images[0])->list->stateFlags);
    return 0;
}
extern void func_00272778(KwlnTask *);
extern void func_00272668(s32, s32, const void *, StaffMenuWork *, s32, s32);
extern void mnuDrawStaffCampSlotsAndCurrency(s32, s32, s32, struct EffectSlotSet *, struct EffectSlotSet *, s32);
extern const u8 D_0037C3A8[];

s32 func_002728F8(KwlnTask *task) {
    StaffMenuWork *context = (StaffMenuWork *)kwlnTaskGetUserValue(task);
    s32 state;

    func_00272778(task);
    state = func_002719F0((s32)task);
    if (state == 0) {
        return state;
    }
    mnuCreateStaffImageSprite(0);
    func_00272668(0, ((MenuWindowContainer *)context->images[0])->list->cursor->sortKeyPrimary, D_0037C3A8, context, 1, 0x53);
    mnuDrawStaffCampSlotsAndCurrency(0, 0, 0, context->staffSlots.baseResources[5], context->staffSlots.baseResources[1], 0x53);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (MenuWindowContainer *)context->images[0], 0x53);
    mnuDrawStaffGridLabelsForKind(0, context->staffSlots.baseResources[6]);
    return menuRunPanel((void *)context, 1, (void *)task);
}

/* Submit a request to the active menu dispatcher in mode 2. */
s32 func_002729C8(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);
    return menuRunPanel((void *)context, 2, (void *)request);
}

s32 mnuStartStaffDisplay(KwlnTask *task) {
    StaffMenuWork *context = (StaffMenuWork *)kwlnTaskGetUserValue(task);
    mnuSetStaffDisplayMode(5, context);
    mnuActivatePanelAndConfigureGridResources(context->scrollPanel, context->singleResource, 0, 1);
    mnuCreateConfigTasks(0);
    return 1;
}

/* Switch the staff display to the fourth base resource. */
u32 mnuConfigureCampDrawContextPanel(KwlnTask *task) {
    StaffMenuWork *context = (StaffMenuWork *)kwlnTaskGetUserValue(task);
    mnuActivatePanelAndConfigureGridResources(context->scrollPanel,
                                               (s32)context->staffSlots.baseResources[3], 0, 1);
    return 1;
}

/* Dispatch a callback; on idle, install the default entry unless busy. */
s32 mnuDispatchStaffMenuWithIdlePopup(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);
    s32 *dispatchEntry = (s32 *)(context + 0x54);
    s32 state = menuRunPanel((void *)context, 0, (void *)callback);
    if (state == 0) {
        if (fileConsumeConfigTaskReady() == 0) {
            mnuSetPopupEntryFlagged(dispatchEntry, D_0037C844);
        }
        return 0;
    }
    return state;
}

s32 mnuDrawStaffImageScreen(KwlnTask *callback) {
    StaffMenuWork *context = (StaffMenuWork *)kwlnTaskGetUserValue(callback);
    mnuDrawBackdrop((s32)(context->background), 0x20);
    func_0027E8D8(-0x10, -8, 0, context->scrollPanel, 0x54);
    mnuCreateStaffImageSprite(0x14);
    mnuDrawStaffGridLabelsForKind(2, context->staffSlots.baseResources[6]);
    return menuRunPanel((void *)context, 1, (void *)callback);
}

s32 func_00272B80(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);
    return menuRunPanel((void *)context, 2, (void *)request);
}

u32 func_00272BB8(void) {
    return 1;
}

/* Draw the item quantity when present, otherwise show both staff-item slots. */
void func_00272BC0(s32 x, s32 y, s32 z, struct MenuList *list,
                  struct MenuListNode *node, s32 drawArg) {
    StaffMenuWork *context = (StaffMenuWork *)list->context;
    u32 itemId = node->sortKeySecondary;
    u8 quantity = datGameState->inventory.counts[itemId];
    u32 color = 0xA09DC380;

    if (quantity != 0) {
        FrFontGlyph *label;
        FrFontGlyph *count;
        char buffer[0x20];

        if (node->flags48 & 1) {
            color = 0xA09DC340;
        }
        label = (FrFontGlyph *)func_001978E8(x + 0x810, y + 0x10, z, color,
                                          D_003BC6C8, 0);
        frFontSetChildChainFirstOption(label, 0);
        func_003014F0(buffer, D_003BC6D0,
                       datGameState->inventory.counts[node->sortKeySecondary]);
        count = (FrFontGlyph *)func_001978E8(x + 0x910, y + 0x20, z, color,
                                          buffer, (s32)label);
        frFontDrawGlyphChain(count, 1, drawArg);
        frFontQueueGlyphForCurrentDrawBuffer(count);
    } else {
        s32 selected = (list->cursor == node);

        node->value = D_003BC6D8;
        itfDrawGridWithResolvedSlot(x + 0x80, y + 0x30, z, 1,
                                   context->staffSlots.baseResources[2], selected + 9, drawArg);
        itfDrawGridWithResolvedSlot(x + 0x7F0, y + 0x30, z, 1,
                                   context->staffSlots.baseResources[2], selected + 0xB, drawArg);
    }
}

INCLUDE_SDATA(const s32, "game/code_00272798", D_003BC6C8);

INCLUDE_SDATA(const s32, "game/code_00272798", D_003BC6D0);

INCLUDE_SDATA(const s32, "game/code_00272798", D_003BC6D8);

