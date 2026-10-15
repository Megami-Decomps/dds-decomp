#include "common.h"
#include "fld_title_banner_menu.h"
#include "sdf_chip.h"

#include "kwln.h"
#include "kwln_task_create.h"
#include "kwln_task_lifecycle.h"
extern u32 fldInputPanelTaskHandle;



extern s32 fldInitializeTitleBannerTask(KwlnTask *task);

void fldReleasePanelState(KwlnTask *task) {
    void *panelState;

    panelState = (void *)kwlnTaskGetUserValue(task);
    sdfReleaseChipBlock(panelState);
    fldInputPanelTaskHandle = 0;
}

void fldCreateInputPanelTask(void) {
    fldInputPanelTaskHandle = kwlnTaskCreate("inputpanel", 0x2B0B, 1, 1, fldInitializeTitleBannerTask, fldReleasePanelState, 0);
}

void fldDestroyPanelTaskIfPresent(void) {
    if (fldInputPanelTaskHandle != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)fldInputPanelTaskHandle, 1);
    }
}

void *fldSetTitleBannerPending(s32 pending) {
    FldTitleBannerMenu *menu;

    menu = (FldTitleBannerMenu *)kwlnTaskGetUserValue((KwlnTask *)fldInputPanelTaskHandle);
    menu->pending = pending;
    return menu;
}

void *fldSetTitleBannerChoice(s32 choice) {
    FldTitleBannerMenu *menu;

    menu = (FldTitleBannerMenu *)kwlnTaskGetUserValue((KwlnTask *)fldInputPanelTaskHandle);
    menu->choice = choice;
    return menu;
}

INCLUDE_SDATA(const s32, "field/fldPanel", fldInputPanelTaskHandle);

