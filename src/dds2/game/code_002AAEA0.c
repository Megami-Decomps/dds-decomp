#include "mnu_input.h"
#include "kwln.h"
#include "mnu.h"
#include "mnu_staff.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "dat_state.h"
#include "mnu_scroll_panel.h"
#include "mdl_flag_api.h"
#include "sdk_libc.h"
#include "fr_font.h"
#include "fr_font_draw_glyph.h"
#include "ui_color.h"
#include "eff.h"
/* Declared before first use (were implicit int). */
extern void dds3ConfigProcessStart(s32 mode);
extern void mnuDrawCampIconBackdrop(MenuCampEffect *set, s32 surface);
extern void mnuSwitchCampVisualCategory(s32 nextCategory, u8 *menuBytes);


extern s32 checkConfigState(void);

extern u8 mnuCampCommandReturnPopupEntry[];


extern u8 mnuRingCustomizationItemRows[];

extern u8 D_003E73F8[];

extern void mnuClearWindowListSelectionFlags(MenuWindowContainer *);

extern void mnuPlayInputSound(s32, s32, u32 *);
extern u8 mnuCampCommandDispatchEntries[];




u32 mnuPrepareCampFieldSkillDisplay(dds3ProcessID_t *task) {
    s32 context;

    context = dds3GetProcessData(task);
    if (mnuUseFieldSkillOnParty(&((MenuStaffContext *)context)->partyPanel, &((MenuStaffContext *)context)->partyWindow, 0) == 0) {
        ((MenuStaffContext *)context)->skillWindow->list->last->flags48 |= 1;
    } else {
        ((MenuStaffContext *)context)->skillWindow->list->last->flags48 &= ~1;
    }
    ((MenuStaffContext *)context)->titleOpacity = 0;
    ((MenuStaffContext *)context)->titleFadingOut = 0;
    ((MenuStaffContext *)context)->titleSlide = -0x32;
    return 1;
}

u32 mnuStartCampTitleFadeOut(dds3ProcessID_t *task) {
    s32 context;

    context = dds3GetProcessData(task);
    ((MenuStaffContext *)context)->titleFadingOut = 1;
    return 1;
}

/* Handle the selected field skill after both dispatch and resource readiness. */
s32 mnuHandleCampFieldSkillInput(dds3ProcessID_t *callback) {
    MenuStaffContext *context;
    s32 *popup;
    u32 input;
    s32 state;

    context = (MenuStaffContext *)dds3GetProcessData(callback);
    input = mnuMapPadMaskToFlags(0x33);
    popup = &context->popupState;
    state = mnuDispatchPopupTransition(&context->transitionWork, popup, 0, (void *)callback);
    if (state != 0) {
        return state;
    }
    state = mnuInitializeCampMenuWhenResourcesReady(callback);
    if (state == 0) {
        return state;
    }
    if (*popup == 0) {
        if (input & 1) {
            struct MenuListNode *entry = context->skillWindow->list->cursor;

            if ((entry->flags48 & 1) == 0) {
                u32 index = entry->sortKeyPrimary + 1;

                mnuSetPopupEntry(popup, (mnuCampCommandDispatchEntries + index * 0x1C));
                context->titleFadingOut = 1;
            } else {
                input = 0x8000;
            }
        }
        if (input & 2) {
            mnuSetPopupEntry(popup, mnuCampCommandDispatchEntries);
        }
    }
    if ((input & 0x300000) == 0) {
        mnuClearWindowListSelectionFlags(context->skillWindow);
    }
    if (input & 0x10) {
        mnuRetreatWindowListSelection(context->skillWindow);
    }
    if (input & 0x20) {
        mnuAdvanceWindowListSelection(context->skillWindow);
    }
    mnuClearWindowPanelTransitionFlag(context->skillWindow);
    mnuPlayInputSound(0, input, &context->skillWindow->list->stateFlags);
    return 0;
}

extern void mnuDrawCampGridFrameAndLabel(s32, u32, u32, u32, u32, u32, u32, u32, u32);
extern char mnuCampCommandHelpTextBank[];
extern char mnuCampFieldSkillHelpConditionFlags[];

s32 mnuDrawCampFieldSkillPage(dds3ProcessID_t *task) {
    MenuStaffContext *context = (MenuStaffContext *)dds3GetProcessData(task);
    s32 state;
    s32 layer = 0x53;

    mnuDrawCampScreenDefault(task);
    state = mnuInitializeCampMenuWhenResourcesReady(task);
    if (state == 0) {
        return state;
    }
    mnuCreateStaffImageSprite(0);
    mnuDrawCampGridFrameAndLabel(0, context->skillWindow->list->cursor->sortKeyPrimary, (u32)mnuCampCommandHelpTextBank, (u32)context,
                  1, 0, (u32)mnuCampFieldSkillHelpConditionFlags, 8, layer);
    mnuUpdateAndDrawWindowTransition(0x1E0, 0x350, 0, &context->fade, layer);
    mnuDrawStaffGridLabelsForKind(0, context->resources.baseResources[0]);
    return menuSetHandler((void *)context, 1, (void *)task);
}

s32 mnuFinishStaffConfigPopup(dds3ProcessID_t *callback) {
    s32 context;

    context = dds3GetProcessData(callback);
    return menuSetHandler((void *)context, 2, (void *)callback);
}

u32 mnuOpenCampConfigPanelTasks(dds3ProcessID_t *task) {
    s32 context;

    context = dds3GetProcessData(task);
    mnuSwitchCampVisualCategory(5, context);
    mnuConfigurePanelResource(((MenuStaffContext *)context)->scrollPanel,
                              ((MenuStaffContext *)context)->contextTitle, 0, 0);
    dds3ConfigProcessStart(0);
    return 1;
}

/* Switch the staff display to the alternate resource at context + 0x60. */
u32 mnuConfigureCampDrawContextPanel(dds3ProcessID_t *task) {
    s32 context;

    context = dds3GetProcessData(task);
    mnuConfigurePanelResource(((MenuStaffContext *)context)->scrollPanel,
                              ((MenuStaffContext *)context)->resources.baseResources[0], 0, 1);
    return 1;
}

/* Dispatch a callback; on idle, install the default entry unless busy. */
s32 mnuDispatchStaffMenuWithIdlePopup(dds3ProcessID_t *callback) {
    s32 context = dds3GetProcessData(callback);
    s32 *dispatchEntry = &((MenuStaffContext *)context)->popupState;
    s32 state = mnuDispatchPopupTransition(&((MenuStaffContext *)context)->transitionWork, dispatchEntry, 0, (void *)callback);
    if (state == 0) {
        if (checkConfigState() == 0) {
            mnuSetPopupEntryFlagged(dispatchEntry, mnuCampCommandReturnPopupEntry);
        }
        return 0;
    }
    return state;
}

s32 mnuDrawStaffImageScreen(dds3ProcessID_t *callback) {
    s32 context;

    context = dds3GetProcessData(callback);
    mnuDrawCampIconBackdrop(context + 0x11C, 0x20);
    mnuDrawStaffScrollPanel(-0x10, -8, 0, ((MenuStaffContext *)context)->scrollPanel, 0x54);
    mnuCreateStaffImageSprite(0x18);
    mnuDrawStaffGridLabelsForKind(2, ((MenuStaffContext *)context)->resources.baseResources[0]);
    return menuSetHandler((void *)context, 1, (void *)callback);
}

s32 mnuFinishStaffImagePopup(dds3ProcessID_t *callback) {
    s32 context;

    context = dds3GetProcessData(callback);
    return menuSetHandler((void *)context, 2, (void *)callback);
}

/* Registered field-skill display leave hook: no side effects; returns one. */
u32 mnuCampFieldSkillDisplayLeaveNoOp(void) {
    return 1;
}

u32 mnuRunCampFieldSkillDisplayTask(dds3ProcessID_t *task) {
    mnuPrepareCampFieldSkillDisplay(task);
    return 1;
}

s32 mnuPollCampFieldSkillAndPopup(dds3ProcessID_t *callback) {
    s32 context;
    s32 state;

    context = dds3GetProcessData(callback);
    state = menuSetHandler((void *)context, 0, (void *)callback);
    if (state != 0) {
        return state;
    }
    mnuUseFieldSkillOnParty(&((MenuStaffContext *)context)->partyPanel, &((MenuStaffContext *)context)->partyWindow, 1);
    mnuSetPopupEntry(&((MenuStaffContext *)context)->popupState, mnuCampCommandReturnPopupEntry);
    return 0;
}

s32 mnuDrawStaffCampImageScreen(dds3ProcessID_t *task) {
    MenuStaffContext *context = (MenuStaffContext *)dds3GetProcessData(task);
    s32 state;
    s32 layer = 0x53;

    mnuDrawCampScreenDefault(task);
    state = mnuInitializeCampMenuWhenResourcesReady(task);
    if (state == 0) {
        return state;
    }
    mnuCreateStaffImageSprite(0);
    mnuDrawCampGridFrameAndLabel(0, context->skillWindow->list->cursor->sortKeyPrimary, (u32)mnuCampCommandHelpTextBank, (u32)context,
                  1, 0, (u32)mnuCampFieldSkillHelpConditionFlags, 8, layer);
    mnuUpdateAndDrawWindowTransition(0x1E0, 0x350, 0, &context->fade, layer);
    mnuDrawStaffGridLabelsForKind(0, context->resources.baseResources[0]);
    return menuSetHandler((void *)context, 1, (void *)task);
}

s32 mnuFinishFieldSkillPopup(dds3ProcessID_t *callback) {
    s32 context;

    context = dds3GetProcessData(callback);
    return menuSetHandler((void *)context, 2, (void *)callback);
}

/* Always allow entry to the registered staff-display return popup. */
u32 mnuAllowStaffDisplayReturn(void) {
    return 1;
}

s32 mtrMantraIdIsValid(s32 mantraId) {
    u32 i;

    for (i = 0; i < 5; i++) {
        if (mantraId == D_003E73F8[i]) {
            return 1;
        }
    }
    return 0;
}


extern u16 D_00437B6E;
extern u8 D_00437B88;
extern u16 mnuRingItemDisplayOrder[];

s32 mtrHasEnoughOwnedMantras(void) {
    s32 owned = 0;
    s32 required = D_00437B6E;
    s32 i;

    required -= D_00437B88;
    required--;

    if (evtBitCheck(0xBA0) == 0) {
        required--;
    }
    for (i = 0; i < D_00437B6E; i++) {
        s32 mantraId = mnuRingItemDisplayOrder[i];
        if (DDS3_GBWK->inventory.counts[mantraId] != 0) {
            if (mtrMantraIdIsValid(mantraId) == 0) {
                owned++;
            }
        }
    }
    if (owned < required) {
        return 0;
    }
    return 1;
}

s32 mtrMantraFindIndex(s32 mantraId) {
    u32 i;

    for (i = 0; i < 0x12; i++) {
        if (mantraId == *(u16 *)(mnuRingCustomizationItemRows + i * 0x1C)) {
            return i + 1;
        }
    }
    return 0;
}

extern struct _FRQ *itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, struct _FRQ *);
extern void mnuDrawCampGridResourceSlot(s32, u32, u32, s32, u32, u32);
extern void mnuDispatchEntryWords(EffectSlotSet *, s32, struct MenuListNode *);
extern void mnuDrawRepeatedPanelSprites(s32, s32, s32, s32, s32, EffectSlotSet *, s32, s32);
extern void itfGridCopyEntryQuad(s32 resource, s32 entryIndex);
extern char D_00437BB8[];
extern char D_00437BC0[];
extern char D_00437BC8[];

/* Draw one camp item row: the owned-count label and quantity when the item is
 * held, otherwise an empty placeholder bar under the highlight index. */
void mnuDrawCampGridEntry(s32 x, s32 y, s32 depth, s32 offset, struct MenuList *list,
                          struct MenuListNode *node, s32 texture) {
    s32 scale = list->scale;
    MenuStaffContext *context;
    u32 color;
    s32 markerY;
    char buffer[32];

    context = (MenuStaffContext *)list->context;
    color = fclBlendColor(0xA09DC380, 0xA09DC300, scale);
    mnuDrawCampGridResourceSlot(x, y, depth, (s32)list, (u32)node, texture);
    markerY = y + 0x40;
    if (DDS3_GBWK->inventory.counts[node->sortKeySecondary] != 0) {
        struct _FRQ *glyph;

        if (node->flags48 & 1) {
            color = 0xA09DC320;
        }
        glyph = itfCreateFont2TextGlyph(x + offset + 0x840, y + 0x20, depth, color, D_00437BB8, 0);
        frSetFontColorNo(glyph, 0);
        sprintf(buffer, D_00437BC0, DDS3_GBWK->inventory.counts[node->sortKeySecondary]);
        glyph = itfCreateFont2TextGlyph(x + offset + 0x910, y + 0x20, depth, color, buffer, glyph);
        frFontDrawGlyphChain(glyph, 1, texture);
        frDestroyFontRequestAfter(glyph);
    } else {
        EffectSlotSet *resource = context->resources.baseResources[1];
        s32 entryIndex;
        s32 markerX;

        node->value = D_00437BC8;
        {
            s32 highlight = list->cursor == node;

            entryIndex = highlight + 0x1B;
        }
        markerX = x + offset + 0x1C0;
        mnuDispatchEntryWords(resource, entryIndex, node);
        mnuDrawRepeatedPanelSprites(markerX, markerY, depth, scale, 8,
                                    resource, entryIndex, texture);
        itfGridCopyEntryQuad((s32)resource, entryIndex);
    }
}

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437B88);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437B90);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437B98);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BA0);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BA8);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BB0);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BB8);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BC0);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BC8);

