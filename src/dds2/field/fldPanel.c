#include "common.h"

extern u32 D_00436204;

extern void *kwlnTaskGetUserValue();

extern s32 kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

extern void *kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);

extern void fldInitializeTitleBannerTask(void);

void fldReleasePanelState(void) {
    u64 state;

    state = kwlnTaskGetUserValue();
    sdfReleaseChipBlock(state);
    D_00436204 = 0;
}

void fldCreateInputPanelTask(void) {
    D_00436204 = kwlnTaskCreate("inputpanel", 0x2B0B, 1, 1, fldInitializeTitleBannerTask, fldReleasePanelState, 0);
}

void fldDestroyPanelTaskIfPresent(void) {
    if (D_00436204 != 0) {
        kwlnTaskDestroyWithHierarchy(D_00436204, 1);
    }
}

void *func_00144270(s32 value) {
    void *panelState;

    panelState = kwlnTaskGetUserValue(D_00436204);
    *(s16 *)((char *)panelState + 4) = value;
    return panelState;
}

void *func_001442A0(s32 value) {
    void *panelState;

    panelState = kwlnTaskGetUserValue(D_00436204);
    *(s16 *)((char *)panelState + 2) = value;
    return panelState;
}

INCLUDE_SDATA(const s32, "field/fldPanel", D_00436204);

