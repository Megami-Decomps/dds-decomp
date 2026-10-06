#include "mnu.h"
#include "mnu_result.h"


extern void mnuRefreshPanelLayer(BrsSkillPackageWork *);

extern s32 kwlnTaskGetUserValue();
extern s32 mnuMapPadMaskToFlags(s32);
extern s32 brsTaskIsUiUpdateAllowed(s32);
extern void func_002650C0(void *);
extern s32 brsPollResultCounterCompletion(void);
extern void mnuClearTitleState(BrsSkillPackageWork *);
extern void mnuSetPopupEntry(s32 *, void *);
extern u8 D_00324530[];
extern u8 D_0036D3EC[];

s32 brsMessageInputStep(u64 input) {
    BrsSkillPackageWork *context;
    s32 *window;
    s32 buttons;
    s32 result;

    context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    window = &context->transition.state;
    buttons = mnuMapPadMaskToFlags(0x33);
    if (brsTaskIsUiUpdateAllowed((s32)context) == 0) {
        return 0;
    }
    result = menuRunPanel((s32)context, 0, input);
    if (result != 0) {
        return result;
    }
    if (*window == 0) {
        func_002650C0(context);
        if (brsPollResultCounterCompletion() != 0 &&
            ((buttons & 1) != 0 || (D_00324530[3] & 2) != 0)) {
            mnuClearTitleState(context);
            mnuSetPopupEntry(window, D_0036D3EC);
        }
    }
    return 0;
}


s32 mnuStaffRunPanel1(s32 input) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    if (brsTaskIsUiUpdateAllowed((s32)context) != 0) {
        mnuRefreshPanelLayer(context);
        return menuRunPanel((s32)context, 1, input);
    }
}

extern void brsDecaySharedAnimCounter(BrsSkillPackageWork *);

s32 mnuStaffRunPanel2(s32 input) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    if (brsTaskIsUiUpdateAllowed((s32)context) != 0) {
        brsDecaySharedAnimCounter(context);
        return menuRunPanel((s32)context, 2, input);
    }
}

u32 func_00263050(void) {
    return 1;
}

u32 func_00263058(void) {
    return 1;
}

s32 mnuStaffRunPanel0(s32 input) {
    s32 context = kwlnTaskGetUserValue();
    mnuMapPadMaskToFlags(0x33);
    return menuRunPanel(context, 0, input);
}

s32 mnuRefreshAndDispatchCurrentPanel(s32 input) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    mnuRefreshPanelLayer(context);
    return menuRunPanel((s32)context, 1, input);
}

s32 func_00263100(s32 input) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel(context, 2, input);
}

u32 func_00263138(void) {
    return 1;
}

u32 func_00263140(void) {
    return 1;
}
