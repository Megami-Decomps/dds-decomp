#include "common.h"

extern u32 fldInputPanelTaskHandle;
extern void *kwlnTaskGetUserValue();
extern void sdfReleaseChipBlock(void *);

extern s32 kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

extern void *kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);

extern void fldInitializeTitleBannerTask(void);

void fldReleasePanelState(void) {
    void *panelState;

    panelState = kwlnTaskGetUserValue();
    sdfReleaseChipBlock(panelState);
    fldInputPanelTaskHandle = 0;
}

void fldCreateInputPanelTask(void) {
    fldInputPanelTaskHandle = kwlnTaskCreate("inputpanel", 0x2B0B, 1, 1, fldInitializeTitleBannerTask, fldReleasePanelState, 0);
}

void fldDestroyPanelTaskIfPresent(void) {
    if (fldInputPanelTaskHandle != 0) {
        kwlnTaskDestroyWithHierarchy(fldInputPanelTaskHandle, 1);
    }
}

void *func_00141190(s32 value) {
    u16 *panelState;

    panelState = kwlnTaskGetUserValue(fldInputPanelTaskHandle);
    panelState[2] = value;
    return panelState;
}

void *func_001411C0(s32 value) {
    u16 *panelState;

    panelState = kwlnTaskGetUserValue(fldInputPanelTaskHandle);
    panelState[1] = value;
    return panelState;
}

INCLUDE_SDATA(const s32, "field/fldPanel", fldInputPanelTaskHandle);

