#include "mnu.h"

extern s32 kwlnTaskGetUserValue();

extern s64 fileConsumeConfigTaskReady(void);

extern u8 D_0037C844[];

extern void mnuSetPopupEntryFlagged();

extern void mnuDrawBackdrop(s32, s32);

extern void func_0027E8D8(s32, s32, s32, u32, s32);

extern void mnuCreateStaffImageSprite(s32);

extern void func_002723B0(s32, u32);

/* Staff-display fields of CampMenuContext (code_00274B80). */
typedef struct StaffScreenContext {
    u8 pad00[0x6C];
    u32 displayVariant; /* 0x6C */
    u8 pad70[8];
    u32 actor;          /* 0x78 */
    u8 pad7C[0x98];
    u32 staffResource;  /* 0x114 */
    u8 pad118[0x20];
    u32 display;        /* 0x138 */
} StaffScreenContext;

INCLUDE_ASM(const s32, "game/code_00272798", func_00272798);

INCLUDE_ASM(const s32, "game/code_00272798", func_002728F8);

/* Submit a request to the active menu dispatcher in mode 2. */
s64 func_002729C8(s32 request) {
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
u32 func_00272A58(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuActivatePanelAndConfigureGridResources(((StaffScreenContext *)context)->display, ((StaffScreenContext *)context)->displayVariant, 0, 1);
    return 1;
}

/* Dispatch a callback; on idle, install the default entry unless busy. */
s64 mnuDispatchStaffMenuWithIdlePopup(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *dispatchEntry = (s32 *)(context + 0x54);
    s64 state = func_00285670(context + 8, dispatchEntry, 0, callback);
    if (state == 0) {
        if (fileConsumeConfigTaskReady() == 0) {
            mnuSetPopupEntryFlagged(dispatchEntry, D_0037C844);
        }
        return 0;
    }
    return state;
}

s64 mnuDrawStaffImageScreen(s32 arg0) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuDrawBackdrop(context + 0x13C, 0x20);
    func_0027E8D8(-0x10, -8, 0, ((StaffScreenContext *)context)->display, 0x54);
    mnuCreateStaffImageSprite(0x14);
    func_002723B0(2, ((StaffScreenContext *)context)->actor);
    return menuRunPanel(context, 1, arg0);
}

s64 func_00272B80(s32 request) {
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

