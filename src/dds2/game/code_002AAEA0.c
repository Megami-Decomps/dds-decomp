#include "mnu_input.h"
#include "kwln.h"
#include "mnu.h"
#include "mnu_staff.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "dat_state.h"
#include "mnu_scroll_panel.h"


extern s64 fileConsumeConfigTaskReady(void);

extern u8 D_003E7034[];


extern u8 D_003E7200[];

extern u8 D_003E73F8[];

extern void func_002B9808(MenuWindowContainer *);

extern void mnuPlayInputSound(s32, s32, u32 *);
extern u8 D_003E6F38[];




u32 mnuPrepareCampFieldSkillDisplay(KwlnTask *task) {
    s32 context;

    context = kwlnTaskGetUserValue(task);
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

u32 mnuStartCampTitleFadeOut(KwlnTask *task) {
    s32 context;

    context = kwlnTaskGetUserValue(task);
    ((MenuStaffContext *)context)->titleFadingOut = 1;
    return 1;
}

/* Handle the selected field skill after both dispatch and resource readiness. */
s32 mnuHandleCampFieldSkillInput(KwlnTask *callback) {
    MenuStaffContext *context;
    s32 *popup;
    u32 input;
    s32 state;

    context = (MenuStaffContext *)kwlnTaskGetUserValue(callback);
    input = mnuMapPadMaskToFlags(0x33);
    popup = &context->popupState;
    state = func_002C4038(&context->transitionWork, popup, 0, (void *)callback);
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

                mnuSetPopupEntry(popup, (D_003E6F38 + index * 0x1C));
                context->titleFadingOut = 1;
            } else {
                input = 0x8000;
            }
        }
        if (input & 2) {
            mnuSetPopupEntry(popup, D_003E6F38);
        }
    }
    if ((input & 0x300000) == 0) {
        func_002B9808(context->skillWindow);
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

extern void func_002AAE80(KwlnTask *);
extern void func_002AA9D8(s32, u32, u32, u32, u32, u32, u32, u32, u32);
extern char D_003E69B0[];
extern char D_003E6F18[];

s32 func_002AB0E0(KwlnTask *task) {
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue(task);
    s32 state;
    s32 layer = 0x53;

    func_002AAE80(task);
    state = mnuInitializeCampMenuWhenResourcesReady(task);
    if (state == 0) {
        return state;
    }
    mnuCreateStaffImageSprite(0);
    func_002AA9D8(0, context->skillWindow->list->cursor->sortKeyPrimary, (u32)D_003E69B0, (u32)context,
                  1, 0, (u32)D_003E6F18, 8, layer);
    mnuUpdateAndDrawWindowTransition(0x1E0, 0x350, 0, &context->fade, layer);
    mnuDrawStaffGridLabelsForKind(0, context->resources.baseResources[0]);
    return menuSetHandler((void *)context, 1, (void *)task);
}

s32 mnuFinishStaffConfigPopup(KwlnTask *callback) {
    s32 context;

    context = kwlnTaskGetUserValue(callback);
    return menuSetHandler((void *)context, 2, (void *)callback);
}

u32 mnuOpenCampConfigPanelTasks(KwlnTask *task) {
    s32 context;

    context = kwlnTaskGetUserValue(task);
    mnuSwitchCampVisualCategory(5, context);
    mnuConfigurePanelResource(((MenuStaffContext *)context)->scrollPanel,
                              ((MenuStaffContext *)context)->contextTitle, 0, 0);
    mnuCreateConfigTasks(0);
    return 1;
}

/* Switch the staff display to the alternate resource at context + 0x60. */
u32 mnuConfigureCampDrawContextPanel(KwlnTask *task) {
    s32 context;

    context = kwlnTaskGetUserValue(task);
    mnuConfigurePanelResource(((MenuStaffContext *)context)->scrollPanel,
                              ((MenuStaffContext *)context)->resources.baseResources[0], 0, 1);
    return 1;
}

/* Dispatch a callback; on idle, install the default entry unless busy. */
s32 mnuDispatchStaffMenuWithIdlePopup(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);
    s32 *dispatchEntry = &((MenuStaffContext *)context)->popupState;
    s32 state = func_002C4038(&((MenuStaffContext *)context)->transitionWork, dispatchEntry, 0, (void *)callback);
    if (state == 0) {
        if (fileConsumeConfigTaskReady() == 0) {
            mnuSetPopupEntryFlagged(dispatchEntry, D_003E7034);
        }
        return 0;
    }
    return state;
}

s32 mnuDrawStaffImageScreen(KwlnTask *callback) {
    s32 context;

    context = kwlnTaskGetUserValue(callback);
    mnuDrawCampIconBackdrop(context + 0x11C, 0x20);
    func_002BB510(-0x10, -8, 0, ((MenuStaffContext *)context)->scrollPanel, 0x54);
    mnuCreateStaffImageSprite(0x18);
    mnuDrawStaffGridLabelsForKind(2, ((MenuStaffContext *)context)->resources.baseResources[0]);
    return menuSetHandler((void *)context, 1, (void *)callback);
}

s32 mnuFinishStaffImagePopup(KwlnTask *callback) {
    s32 context;

    context = kwlnTaskGetUserValue(callback);
    return menuSetHandler((void *)context, 2, (void *)callback);
}

u32 func_002AB3A0(void) {
    return 1;
}

u32 func_002AB3A8(KwlnTask *task) {
    mnuPrepareCampFieldSkillDisplay(task);
    return 1;
}

s32 mnuPollCampFieldSkillAndPopup(KwlnTask *callback) {
    s32 context;
    s32 state;

    context = kwlnTaskGetUserValue(callback);
    state = menuSetHandler((void *)context, 0, (void *)callback);
    if (state != 0) {
        return state;
    }
    mnuUseFieldSkillOnParty(&((MenuStaffContext *)context)->partyPanel, &((MenuStaffContext *)context)->partyWindow, 1);
    mnuSetPopupEntry(&((MenuStaffContext *)context)->popupState, D_003E7034);
    return 0;
}

s32 func_002AB448(KwlnTask *task) {
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue(task);
    s32 state;
    s32 layer = 0x53;

    func_002AAE80(task);
    state = mnuInitializeCampMenuWhenResourcesReady(task);
    if (state == 0) {
        return state;
    }
    mnuCreateStaffImageSprite(0);
    func_002AA9D8(0, context->skillWindow->list->cursor->sortKeyPrimary, (u32)D_003E69B0, (u32)context,
                  1, 0, (u32)D_003E6F18, 8, layer);
    mnuUpdateAndDrawWindowTransition(0x1E0, 0x350, 0, &context->fade, layer);
    mnuDrawStaffGridLabelsForKind(0, context->resources.baseResources[0]);
    return menuSetHandler((void *)context, 1, (void *)task);
}

s32 mnuFinishFieldSkillPopup(KwlnTask *callback) {
    s32 context;

    context = kwlnTaskGetUserValue(callback);
    return menuSetHandler((void *)context, 2, (void *)callback);
}

u32 func_002AB550(void) {
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
extern u16 D_003E6730[];
extern s32 mdlFlagTest(s32);

s32 mtrHasEnoughOwnedMantras(void) {
    s32 owned = 0;
    s32 required = D_00437B6E;
    s32 i;

    required -= D_00437B88;
    required--;

    if (mdlFlagTest(0xBA0) == 0) {
        required--;
    }
    for (i = 0; i < D_00437B6E; i++) {
        s32 mantraId = D_003E6730[i];
        if (datGameState->inventory.counts[mantraId] != 0) {
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
        if (mantraId == *(u16 *)(D_003E7200 + i * 0x1C)) {
            return i + 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB690);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437B88);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437B90);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437B98);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BA0);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BA8);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BB0);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BB8);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BC0);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BC8);

