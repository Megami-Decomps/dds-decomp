#include "common.h"
#include "fld_title_banner_menu.h"
#include "sdf_chip.h"

#include "kwln.h"
#include "kwln_task_create.h"
#include "kwln_task_lifecycle.h"
extern dds3ProcessID_t *inputpanel_id;



void fldInputPanelShutdown(dds3ProcessID_t *panelTask) {
    FldTitleBannerMenu *menu;

    menu = (FldTitleBannerMenu *)dds3GetProcessData(panelTask);
    sdfChipFreeDirect(menu);
    inputpanel_id = 0;
}

void fldInitInputPanel(void) {
    inputpanel_id = dds3StartProcess("inputpanel", 0x2B0B, 1, 1, fldInputPanelProcess1, fldInputPanelShutdown, 0);
}

void fldFreeInputPanel(void) {
    if (inputpanel_id != 0) {
        dds3KillProcess(inputpanel_id, 1);
    }
}

void fldInputPanelSetDraw(FldTitleBannerDrawRequest request) {
    FldTitleBannerMenu *menu;

    menu = (FldTitleBannerMenu *)dds3GetProcessData(inputpanel_id);
    menu->drawRequestState = request;
}

void fldInputPanelSetMessage(s32 choice) {
    FldTitleBannerMenu *menu;

    menu = (FldTitleBannerMenu *)dds3GetProcessData(inputpanel_id);
    menu->choice = choice;
}

INCLUDE_SDATA(const s32, "field/fldPanel", inputpanel_id);
