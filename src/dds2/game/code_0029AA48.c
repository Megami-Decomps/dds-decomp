#include "common.h"

extern s32 func_00101958();

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

extern s32 func_002993D0(s32);

extern void func_0029AC20(s32, s32);

extern void func_0026C900(void);

void func_0029AA48(MenuLayoutContext *context) {
    func_002B7F80((s32)context->menuList, 0x20);
}

INCLUDE_ASM(const s32, "game/code_0029AA48", func_0029AA68);

INCLUDE_ASM(const s32, "game/code_0029AA48", func_0029AC20);

INCLUDE_ASM(const s32, "game/code_0029AA48", func_0029AD98);

INCLUDE_ASM(const s32, "game/code_0029AA48", func_0029AE10);

/* The five signed config bytes reserve space before the selected entry width. */
u32 mnuResetSelectionWidthsFromConfig(void) {
    s8 widthByte;
    MenuLayoutContext *context;
    u32 *widthSlot;
    s8 *configWidths;
    s32 remaining;
    s32 selectionWidth;
    s32 reservedWidth;

    context = (MenuLayoutContext *)func_00101958();
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
        func_002C0CF8(context->resource, 0);
    }
    return 1;
}

u32 func_0029AF40(void) {
    return 1;
}

void mnuClearSelectionWidths(MenuLayoutContext *context) {
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

void func_0029AF80(u32 arg0, u32 arg1) {
    func_00314298(arg0, (s32)arg1 + 0x3f4);
    mnuRefreshSelectedUnitPanels(arg0, arg1);
}
