#include "mnu.h"

extern void mnuRefreshPanelLayer(s32);

extern s32 kwlnTaskGetUserValue();
extern void mnuMapPadMaskToFlags(s32);
INCLUDE_ASM(const s32, "game/code_00262EB8", brsMessageInputStep);

extern s32 brsTaskIsUiUpdateAllowed(s32);

s64 mnuStaffRunPanel1(s32 input) {
    s32 context = kwlnTaskGetUserValue();

    if (brsTaskIsUiUpdateAllowed(context) != 0) {
        mnuRefreshPanelLayer(context);
        return menuRunPanel(context, 1, input);
    }
}

extern void brsDecaySharedAnimCounter(s32);

s64 mnuStaffRunPanel2(s32 input) {
    s32 context = kwlnTaskGetUserValue();

    if (brsTaskIsUiUpdateAllowed(context) != 0) {
        brsDecaySharedAnimCounter(context);
        return menuRunPanel(context, 2, input);
    }
}

u32 func_00263050(void) {
    return 1;
}

u32 func_00263058(void) {
    return 1;
}

s64 mnuStaffRunPanel0(s32 input) {
    s32 context = kwlnTaskGetUserValue();
    mnuMapPadMaskToFlags(0x33);
    return menuRunPanel(context, 0, input);
}

s64 mnuRefreshAndDispatchCurrentPanel(s32 input) {
    s32 context = kwlnTaskGetUserValue();
    mnuRefreshPanelLayer(context);
    return menuRunPanel(context, 1, input);
}

s64 func_00263100(s32 input) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel(context, 2, input);
}

u32 func_00263138(void) {
    return 1;
}

u32 func_00263140(void) {
    return 1;
}
