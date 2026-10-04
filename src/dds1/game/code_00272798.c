#include "mnu.h"

extern s32 kwlnTaskGetUserValue();

extern s64 fileConsumeConfigTaskReady(void);

extern u8 D_0037C844[];

extern void mnuSetPopupEntryFlagged();

extern void mnuDrawBackdrop(s32, s32);

extern void func_0027E8D8(s32, s32, s32, u32, s32);

extern void mnuCreateStaffImageSprite(s32);

extern void func_002723B0(s32, u32);
extern u32 mnuMapPadMaskToFlags(s32);
extern s32 func_002719F0(s32);
extern void mnuSetPopupEntry(s32, s32);
extern void func_0027C788(s32);
extern void mnuRetreatWindowListSelection(s32);
extern void mnuAdvanceWindowListSelection(s32);
extern void mnuClearWindowPanelTransitionFlag(s32);
extern void mnuPlayInputSound(s32, u32, s32);
extern u8 D_0037C748[];

typedef struct CampSkillEntry {
    u8 pad00[0x48];
    u32 flags;
    u8 pad4C[0x14];
    u32 commandIndex;
} CampSkillEntry;

typedef struct CampSkillList {
    u8 pad00[0x1C];
    CampSkillEntry *entry;
} CampSkillList;

typedef struct CampSkillPanel {
    u8 pad00[0x14];
    CampSkillList *list;
} CampSkillPanel;


/* Staff-display fields of CampMenuContext (code_00274B80). */
typedef struct StaffScreenContext {
    u8 pad00[8];
    u8 dispatchState[0x4C];
    s32 popup;
    u8 pad58[0x14];
    u32 displayVariant; /* 0x6C */
    u8 pad70[8];
    u32 actor;          /* 0x78 */
    u8 pad7C[0x98];
    u32 staffResource;  /* 0x114 */
    u8 pad118[0xC];
    CampSkillPanel *skillPanel;
    u8 pad128[0x10];
    u32 display;        /* 0x138 */
} StaffScreenContext;


s32 func_00272798(s32 callback) {
    StaffScreenContext *context;
    s32 *popup;
    u32 input;
    s32 state;

    context = (StaffScreenContext *)kwlnTaskGetUserValue();
    input = mnuMapPadMaskToFlags(0x33);
    popup = &context->popup;
    state = func_00285670((s32)context->dispatchState, popup, 0, callback);
    if (state != 0) {
        return state;
    }
    state = func_002719F0(callback);
    if (state == 0) {
        return state;
    }
    if (*popup == 0) {
        if (input & 1) {
            CampSkillEntry *entry = context->skillPanel->list->entry;

            if ((entry->flags & 1) == 0) {
                u32 index = entry->commandIndex + 1;

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
        func_0027C788((s32)context->skillPanel);
    }
    if (input & 0x10) {
        mnuRetreatWindowListSelection((s32)context->skillPanel);
    }
    if (input & 0x20) {
        mnuAdvanceWindowListSelection((s32)context->skillPanel);
    }
    mnuClearWindowPanelTransitionFlag((s32)context->skillPanel);
    mnuPlayInputSound(0, input, (s32)context->skillPanel->list);
    return 0;
}
INCLUDE_ASM(const s32, "game/code_00272798", func_002728F8);

/* Submit a request to the active menu dispatcher in mode 2. */
s32 func_002729C8(s32 request) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel(context, 2, request);
}

s32 mnuStartStaffDisplay(void) {
    u8 *context = (u8 *)kwlnTaskGetUserValue();
    mnuSetStaffDisplayMode(5, context);
    mnuActivatePanelAndConfigureGridResources(((StaffScreenContext *)context)->display, ((StaffScreenContext *)context)->staffResource, 0, 1);
    mnuCreateConfigTasks(0);
    return 1;
}

/* Switch the staff display to the alternate resource at context + 0x6C. */
u32 mnuConfigureCampDrawContextPanel(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuActivatePanelAndConfigureGridResources(((StaffScreenContext *)context)->display, ((StaffScreenContext *)context)->displayVariant, 0, 1);
    return 1;
}

/* Dispatch a callback; on idle, install the default entry unless busy. */
s32 mnuDispatchStaffMenuWithIdlePopup(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *dispatchEntry = (s32 *)(context + 0x54);
    s32 state = func_00285670(context + 8, dispatchEntry, 0, callback);
    if (state == 0) {
        if (fileConsumeConfigTaskReady() == 0) {
            mnuSetPopupEntryFlagged(dispatchEntry, D_0037C844);
        }
        return 0;
    }
    return state;
}

s32 mnuDrawStaffImageScreen(s32 callback) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuDrawBackdrop(context + 0x13C, 0x20);
    func_0027E8D8(-0x10, -8, 0, ((StaffScreenContext *)context)->display, 0x54);
    mnuCreateStaffImageSprite(0x14);
    func_002723B0(2, ((StaffScreenContext *)context)->actor);
    return menuRunPanel(context, 1, callback);
}

s32 func_00272B80(s32 request) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel(context, 2, request);
}

u32 func_00272BB8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00272798", func_00272BC0);

INCLUDE_SDATA(const s32, "game/code_00272798", D_003BC6C8);

INCLUDE_SDATA(const s32, "game/code_00272798", D_003BC6D0);

INCLUDE_SDATA(const s32, "game/code_00272798", D_003BC6D8);

