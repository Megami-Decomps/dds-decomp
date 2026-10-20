#include "common.h"
#include "fld_title_banner_menu.h"
#include "sdf_chip.h"

#include "kwln.h"
#include "kwln_task_create.h"
#include "kwln_task_lifecycle.h"
extern KwlnTask *fldInputPanelTaskHandle;



void fldReleasePanelState(KwlnTask *panelTask) {
    void *panelChipBlock;

    panelChipBlock = (void *)dds3GetProcessData(panelTask);
    sdfChipFreeDirect(panelChipBlock);
    fldInputPanelTaskHandle = 0;
}

void fldCreateInputPanelTask(void) {
    fldInputPanelTaskHandle = dds3StartProcess("inputpanel", 0x2B0B, 1, 1, fldInitializeTitleBannerTask, fldReleasePanelState, 0);
}

void fldDestroyPanelTaskIfPresent(void) {
    if (fldInputPanelTaskHandle != 0) {
        dds3KillProcess(fldInputPanelTaskHandle, 1);
    }
}

FldTitleBannerMenu *fldSetTitleBannerPending(s32 isPending) {
    FldTitleBannerMenu *menu;

    menu = (FldTitleBannerMenu *)dds3GetProcessData(fldInputPanelTaskHandle);
    menu->pending = isPending;
    return menu;
}

FldTitleBannerMenu *fldSetTitleBannerChoice(s32 choice) {
    FldTitleBannerMenu *menu;

    menu = (FldTitleBannerMenu *)dds3GetProcessData(fldInputPanelTaskHandle);
    menu->choice = choice;
    return menu;
}

INCLUDE_SDATA(const s32, "field/fldPanel", fldInputPanelTaskHandle);

