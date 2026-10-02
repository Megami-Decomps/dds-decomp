#include "common.h"
#include "mnu.h"

extern s32 kwlnTaskGetUserValue();

typedef struct MenuLayoutContext {
    u8 pad00[0x9C];
    s32 *config;
    u8 padA0[0x34C];
    s32 selectionWidth;
    u32 margin;
    u32 widths[5];
    u8 pad408[0xA92C];
    s32 resource;
    u8 padAD38[8];
    u8 menuList[0x9A4];
    s32 visible;
} MenuLayoutContext;

extern s32 brsAdvanceSkillPackagePanel(s32);

extern void func_0029AC20(s32, s32);

extern void func_0026C900(void);

void func_0029AA48(MenuLayoutContext *context) {
    mnuDrawCampIconBackdrop((s32)context->menuList, 0x20);
}

INCLUDE_ASM(const s32, "game/code_0029AA48", func_0029AA68);

INCLUDE_ASM(const s32, "game/code_0029AA48", func_0029AC20);

s64 mnuAdvanceSkillPackageToItemPanel(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    if (brsAdvanceSkillPackagePanel(context) != 0) {
        return 0;
    }
    func_0029AA48((MenuLayoutContext *)context);
    func_0029AC20(context, 0);
    return menuSetHandler(context, 1, request);
}

s64 mnuAdvanceSkillPanelToNextMenu(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    if (brsAdvanceSkillPackagePanel(context) != 0) {
        return 0;
    }
    func_0026C900();
    return menuSetHandler(context, 2, request);
}

/* The five signed config bytes reserve space before the selected entry width. */
u32 mnuResetSelectionWidthsFromConfig(void) {
    s8 widthByte;
    MenuLayoutContext *context;
    u32 *widthSlot;
    s8 *configWidths;
    s32 remaining;
    s32 selectionWidth;
    s32 reservedWidth;

    context = (MenuLayoutContext *)kwlnTaskGetUserValue();
    reservedWidth = 0;
    remaining = 4;
    selectionWidth = context->config[1] * 3;
    configWidths = (s8 *)(*context->config + 0x16);
    do {
        widthByte = *configWidths;
        configWidths = configWidths + 1;
        remaining = remaining - 1;
        reservedWidth = reservedWidth + widthByte;
    } while (-1 < remaining);
    context->margin = 0;
    remaining = 4;
    widthSlot = &context->widths[4];
    if (0x1ef - reservedWidth < selectionWidth) {
        selectionWidth = 0x1ef - reservedWidth;
    }
    context->selectionWidth = selectionWidth;
    do {
        remaining = remaining - 1;
        *widthSlot = 0;
        widthSlot = widthSlot + -1;
    } while (-1 < remaining);
    if (context->visible != 0) {
        mnuSetPanelGroupSelection(context->resource, 0);
    }
    return 1;
}

u32 func_0029AF40(void) {
    return 1;
}

void mnuClearItemSelectionSlots(MenuLayoutContext *context) {
    s32 remaining;
    u32 *destination;

    context->margin = 0;
    destination = &context->widths[4];
    remaining = 4;
    do {
        remaining = remaining - 1;
        *destination = 0;
        destination = destination + -1;
    } while (-1 < remaining);
}

void mnuRefreshPartyUnitVitalsPanels(u32 arg0, u32 arg1) {
    func_00314298(arg0, (s32)arg1 + 0x3f4);
    mnuRefreshSelectedUnitPanels(arg0, arg1);
}
