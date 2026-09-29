#include "common.h"

extern void mnuRefreshPanelLayer(s32);

extern s32 func_00101A70();
extern void func_00285B20(s32);
extern void func_00285670(s32, s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_00262EB8", brsMessageInputStep);

extern s32 brsTaskIsUiUpdateAllowed(s32);

void mnuStaffRunPanel1(s32 input) {
    s32 context = func_00101A70();

    if (brsTaskIsUiUpdateAllowed(context) != 0) {
        mnuRefreshPanelLayer(context);
        func_00285670(context + 8, context + 0x54, 1, input);
    }
}

extern void brsDecaySharedAnimCounter(s32);

void mnuStaffRunPanel2(s32 input) {
    s32 context = func_00101A70();

    if (brsTaskIsUiUpdateAllowed(context) != 0) {
        brsDecaySharedAnimCounter(context);
        func_00285670(context + 8, context + 0x54, 2, input);
    }
}

u32 func_00263050(void) {
    return 1;
}

u32 func_00263058(void) {
    return 1;
}

void func_00263060(s32 input) {
    s32 context = func_00101A70();
    func_00285B20(0x33);
    func_00285670(context + 8, context + 0x54, 0, input);
}

void func_002630B0(s32 input) {
    s32 context = func_00101A70();
    mnuRefreshPanelLayer(context);
    func_00285670(context + 8, context + 0x54, 1, input);
}

void func_00263100(s32 input) {
    s32 context = func_00101A70();
    func_00285670(context + 8, context + 0x54, 2, input);
}

u32 func_00263138(void) {
    return 1;
}

u32 func_00263140(void) {
    return 1;
}
