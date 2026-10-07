#include "mnu.h"
#include "mnu_staff.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "dat_state.h"

extern s32 kwlnTaskGetUserValue();

extern s64 fileConsumeConfigTaskReady(void);

extern u8 D_003E7034[];


extern u8 D_003E7200[];

extern u8 D_003E73F8[];

extern u32 mnuMapPadMaskToFlags(s32);
extern s32 func_002A9AB8(s32);
extern void func_002B9808(MenuWindowContainer *);
extern void mnuRetreatWindowListSelection(MenuWindowContainer *);
extern void mnuAdvanceWindowListSelection(MenuWindowContainer *);
extern void mnuClearWindowPanelTransitionFlag(MenuWindowContainer *);
extern void mnuPlayInputSound(s32, s32, u32 *);
extern u8 D_003E6F38[];


/* A focused view of the staff menu's CampVisualWork (code_002A9068). */
typedef struct CampVisualWork {
    u8 pad00[8];
    u8 dispatchState[0x4C];
    s32 popup;
    u8 pad58[8];
    u32 drawContext;        /* 0x60 */
    u8 pad64[0x8C];
    u32 panelResource;      /* 0xF0 */
    u8 padF4[0x10];
    MenuWindowContainer *skillFlagRoot; /* 0x104 */
    u8 pad108[0x10];
    u32 modelHandle;        /* 0x118 */
    u8 pad11C[0xB0B4];
    u32 titleFadingOut;    /* 0xB1D0 */
    u32 titleOpacity;      /* 0xB1D4 */
    u32 titleSlide;        /* 0xB1D8 */
} CampVisualWork;

u32 mnuPrepareCampFieldSkillDisplay(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    if (mnuUseFieldSkillOnParty(&((MenuStaffContext *)context)->partyPanel, &((MenuStaffContext *)context)->partyWindow, 0) == 0) {
        ((CampVisualWork *)context)->skillFlagRoot->list->last->flags48 |= 1;
    } else {
        ((CampVisualWork *)context)->skillFlagRoot->list->last->flags48 &= ~1;
    }
    ((CampVisualWork *)context)->titleOpacity = 0;
    ((CampVisualWork *)context)->titleFadingOut = 0;
    ((CampVisualWork *)context)->titleSlide = -0x32;
    return 1;
}

u32 mnuStartCampTitleFadeOut(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    ((CampVisualWork *)context)->titleFadingOut = 1;
    return 1;
}

/* Handle the selected field skill after both dispatch and resource readiness. */
s32 mnuHandleCampFieldSkillInput(s32 callback) {
    CampVisualWork *context;
    s32 *popup;
    u32 input;
    s32 state;

    context = (CampVisualWork *)kwlnTaskGetUserValue();
    input = mnuMapPadMaskToFlags(0x33);
    popup = &context->popup;
    state = func_002C4038(context->dispatchState, popup, 0, (void *)callback);
    if (state != 0) {
        return state;
    }
    state = func_002A9AB8(callback);
    if (state == 0) {
        return state;
    }
    if (*popup == 0) {
        if (input & 1) {
            struct MenuListNode *entry = context->skillFlagRoot->list->cursor;

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
        func_002B9808(context->skillFlagRoot);
    }
    if (input & 0x10) {
        mnuRetreatWindowListSelection(context->skillFlagRoot);
    }
    if (input & 0x20) {
        mnuAdvanceWindowListSelection(context->skillFlagRoot);
    }
    mnuClearWindowPanelTransitionFlag(context->skillFlagRoot);
    mnuPlayInputSound(0, input, &context->skillFlagRoot->list->stateFlags);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB0E0);

s32 mnuFinishStaffConfigPopup(s32 callback) {
    s32 context;

    context = kwlnTaskGetUserValue();
    return menuSetHandler((void *)context, 2, (void *)callback);
}

u32 mnuOpenCampConfigPanelTasks(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuSwitchCampVisualCategory(5, context);
    mnuConfigurePanelResource(((CampVisualWork *)context)->modelHandle, ((CampVisualWork *)context)->panelResource, 0, 0);
    mnuCreateConfigTasks(0);
    return 1;
}

/* Switch the staff display to the alternate resource at context + 0x60. */
u32 mnuConfigureCampDrawContextPanel(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuConfigurePanelResource(((CampVisualWork *)context)->modelHandle, ((CampVisualWork *)context)->drawContext, 0, 1);
    return 1;
}

/* Dispatch a callback; on idle, install the default entry unless busy. */
s32 mnuDispatchStaffMenuWithIdlePopup(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *dispatchEntry = (s32 *)(context + 0x54);
    s32 state = func_002C4038(((CampVisualWork *)context)->dispatchState, dispatchEntry, 0, (void *)callback);
    if (state == 0) {
        if (fileConsumeConfigTaskReady() == 0) {
            mnuSetPopupEntryFlagged(dispatchEntry, D_003E7034);
        }
        return 0;
    }
    return state;
}

s32 mnuDrawStaffImageScreen(s32 callback) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuDrawCampIconBackdrop(context + 0x11C, 0x20);
    func_002BB510(-0x10, -8, 0, ((CampVisualWork *)context)->modelHandle, 0x54);
    mnuCreateStaffImageSprite(0x18);
    func_002AA7A0(2, ((CampVisualWork *)context)->drawContext);
    return menuSetHandler((void *)context, 1, (void *)callback);
}

s32 mnuFinishStaffImagePopup(s32 callback) {
    s32 context;

    context = kwlnTaskGetUserValue();
    return menuSetHandler((void *)context, 2, (void *)callback);
}

u32 func_002AB3A0(void) {
    return 1;
}

u32 func_002AB3A8(void) {
    mnuPrepareCampFieldSkillDisplay();
    return 1;
}

s32 mnuPollCampFieldSkillAndPopup(s32 callback) {
    s32 context;
    s32 state;

    context = kwlnTaskGetUserValue();
    state = menuSetHandler((void *)context, 0, (void *)callback);
    if (state != 0) {
        return state;
    }
    mnuUseFieldSkillOnParty(&((MenuStaffContext *)context)->partyPanel, &((MenuStaffContext *)context)->partyWindow, 1);
    mnuSetPopupEntry(&((CampVisualWork *)context)->popup, D_003E7034);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB448);

s32 mnuFinishFieldSkillPopup(s32 callback) {
    s32 context;

    context = kwlnTaskGetUserValue();
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

