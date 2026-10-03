#include "mnu.h"

extern s32 kwlnTaskGetUserValue();

extern s64 fileConsumeConfigTaskReady(void);

extern u8 D_003E7034[];

extern void mnuSetPopupEntryFlagged();

extern u8 D_003E7200[];

extern u8 D_003E73F8[];

extern s32 mnuUseFieldSkillOnParty(s32, s32, s32);
extern u32 mnuMapPadMaskToFlags(s32);
extern s32 func_002A9AB8(s32);
extern void mnuSetPopupEntry(s32, s32);
extern void func_002B9808(s32);
extern void mnuRetreatWindowListSelection(s32);
extern void mnuAdvanceWindowListSelection(s32);
extern void mnuClearWindowPanelTransitionFlag(s32);
extern void mnuPlayInputSound(s32, u32, s32);
extern u8 D_003E6F38[];

/* Sub-object reached through the context's +0x104 chain. */
typedef struct {
    u8 pad0[0x48]; /* 0x0 */
    u32 flags;     /* 0x48 */
    u8 pad4C[0x14];
    u32 commandIndex;
} MtrSub;

typedef struct {
    u8 pad0[0x14]; /* 0x0 */
    MtrSub *sub;   /* 0x14 */
    u8 pad18[4];
    MtrSub *entry;
} MtrMid;

typedef struct {
    u8 pad0[0x18]; /* 0x0 */
    MtrMid *mid;   /* 0x18 */
} MtrRoot;

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
    MtrRoot *skillFlagRoot; /* 0x104 */
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
    if (mnuUseFieldSkillOnParty(0xA928 + context, context + 0x284, 0) == 0) {
        ((CampVisualWork *)context)->skillFlagRoot->mid->sub->flags |= 1;
    } else {
        ((CampVisualWork *)context)->skillFlagRoot->mid->sub->flags &= ~1;
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
s32 func_002AAF70(s32 callback) {
    CampVisualWork *context;
    s32 *popup;
    u32 input;
    s32 state;

    context = (CampVisualWork *)kwlnTaskGetUserValue();
    input = mnuMapPadMaskToFlags(0x33);
    popup = &context->popup;
    state = func_002C4038((s32)context->dispatchState, popup, 0, callback);
    if (state != 0) {
        return state;
    }
    state = func_002A9AB8(callback);
    if (state == 0) {
        return state;
    }
    if (*popup == 0) {
        if (input & 1) {
            MtrSub *entry = context->skillFlagRoot->mid->entry;

            if ((entry->flags & 1) == 0) {
                u32 index = entry->commandIndex + 1;

                mnuSetPopupEntry((s32)popup, (s32)(D_003E6F38 + index * 0x1C));
                context->titleFadingOut = 1;
            } else {
                input = 0x8000;
            }
        }
        if (input & 2) {
            mnuSetPopupEntry((s32)popup, (s32)D_003E6F38);
        }
    }
    if ((input & 0x300000) == 0) {
        func_002B9808((s32)context->skillFlagRoot);
    }
    if (input & 0x10) {
        mnuRetreatWindowListSelection((s32)context->skillFlagRoot);
    }
    if (input & 0x20) {
        mnuAdvanceWindowListSelection((s32)context->skillFlagRoot);
    }
    mnuClearWindowPanelTransitionFlag((s32)context->skillFlagRoot);
    mnuPlayInputSound(0, input, (s32)context->skillFlagRoot->mid);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB0E0);

s32 mnuFinishStaffConfigPopup(s32 callback) {
    s32 context;

    context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, callback);
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
    s32 state = func_002C4038(context + 8, dispatchEntry, 0, callback);
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
    return menuSetHandler(context, 1, callback);
}

s32 mnuFinishStaffImagePopup(s32 callback) {
    s32 context;

    context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, callback);
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
    state = menuSetHandler(context, 0, callback);
    if (state != 0) {
        return state;
    }
    mnuUseFieldSkillOnParty(0xA928 + context, context + 0x284, 1);
    mnuSetPopupEntry(context + 0x54, D_003E7034);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB448);

s32 mnuFinishFieldSkillPopup(s32 callback) {
    s32 context;

    context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, callback);
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

typedef struct SaveItemCounts {
    u8 pad00[0x1340];
    u8 counts[0x100];
} SaveItemCounts;

extern SaveItemCounts *datGameState;
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
        if (datGameState->counts[mantraId] != 0) {
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

