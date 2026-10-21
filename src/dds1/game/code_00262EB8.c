#include "mnu_input.h"
#include "kwln.h"
#include "mnu.h"
#include "mnu_result.h"
#include "brs_ui_update_allowed_api.h"


extern void mnuRefreshPanelLayer(BrsSkillPackageWork *);

extern void brsNoOpMessageInputCallback(void *);
extern s32 brsPollResultCounterCompletion(void);
extern void mnuClearTitleState(BrsSkillPackageWork *);
extern u8 D_00324530[];
extern u8 D_0036D3EC[];

s32 brsMessageInputStep(KwlnTask *input) {
    BrsSkillPackageWork *context;
    s32 *transitionState;
    s32 buttons;
    s32 result;

    context = (BrsSkillPackageWork *)dds3GetProcessData(input);
    transitionState = &context->transition.state;
    buttons = mnuMapPadMaskToFlags(0x33);
    if (brsTaskIsUiUpdateAllowed(context) == 0) {
        return 0;
    }
    result = menuRunPanel(context, 0, input);
    if (result != 0) {
        return result;
    }
    if (*transitionState == 0) {
        brsNoOpMessageInputCallback(context);
        if (brsPollResultCounterCompletion() != 0 &&
            ((buttons & 1) != 0 || (D_00324530[3] & 2) != 0)) {
            mnuClearTitleState(context);
            mnuSetPopupEntry(transitionState, D_0036D3EC);
        }
    }
    return 0;
}


s32 mnuStaffRunPanel1(KwlnTask *task) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)dds3GetProcessData(task);

    if (brsTaskIsUiUpdateAllowed(context) != 0) {
        mnuRefreshPanelLayer(context);
        return menuRunPanel(context, 1, (void *)task);
    }
}

extern void brsDecaySharedAnimCounter(BrsSkillPackageWork *);

s32 mnuStaffRunPanel2(KwlnTask *task) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)dds3GetProcessData(task);

    if (brsTaskIsUiUpdateAllowed(context) != 0) {
        brsDecaySharedAnimCounter(context);
        return menuRunPanel(context, 2, (void *)task);
    }
}

/* Registered result message enter hook: no side effects; returns one. */
u32 mnuResultMessageEnterNoOp(void) {
    return 1;
}

/* Registered result message leave hook: no side effects; returns one. */
u32 mnuResultMessageLeaveNoOp(void) {
    return 1;
}

s32 mnuStaffRunPanel0(KwlnTask *task) {
    s32 context = dds3GetProcessData(task);
    mnuMapPadMaskToFlags(0x33);
    return menuRunPanel((void *)context, 0, (void *)task);
}

s32 mnuRefreshAndDispatchCurrentPanel(KwlnTask *task) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)dds3GetProcessData(task);
    mnuRefreshPanelLayer(context);
    return menuRunPanel(context, 1, (void *)task);
}

s32 mnuRunResultPanelMode2(KwlnTask *input) {
    s32 context = dds3GetProcessData(input);
    return menuRunPanel((void *)context, 2, (void *)input);
}

/* Registered result selection enter hook: no side effects; returns one. */
u32 mnuResultSelectionEnterNoOp(void) {
    return 1;
}

/* Registered result selection leave hook: no side effects; returns one. */
u32 mnuResultSelectionLeaveNoOp(void) {
    return 1;
}
