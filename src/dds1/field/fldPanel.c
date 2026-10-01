#include "common.h"

extern u32 D_003BAE74;
extern void *kwlnTaskGetUserValue(void);
extern void sdfReleaseChipBlock(void *);

extern s32 kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

extern void *kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);

extern void fldInitializeTitleBannerTask(void);

void fldReleasePanelState(void) {
    void *panelState;

    panelState = kwlnTaskGetUserValue();
    sdfReleaseChipBlock(panelState);
    D_003BAE74 = 0;
}

void fldCreateInputPanelTask(void) {
    D_003BAE74 = kwlnTaskCreate("inputpanel", 0x2B0B, 1, 1, fldInitializeTitleBannerTask, fldReleasePanelState, 0);
}

void fldDestroyPanelTaskIfPresent(void) {
    if (D_003BAE74 != 0) {
        kwlnTaskDestroyWithHierarchy(D_003BAE74, 1);
    }
}

INCLUDE_ASM(const s32, "field/fldPanel", func_00141190);

INCLUDE_ASM(const s32, "field/fldPanel", func_001411C0);

INCLUDE_SDATA(const s32, "field/fldPanel", D_003BAE74);

