#include "common.h"
#include "mnu.h"

extern s32 kwlnTaskGetUserValue();

typedef struct MenuLayoutContext {
    u8 pad00[0x94];
    s32 backdropContext;
    u8 pad98[4];
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
    s32 iconFade;
    u32 suppressDelta;
} MenuLayoutContext;

extern s32 brsAdvanceSkillPackagePanel(s32);

extern void func_0029AC20(s32, s32);

extern void func_0026C900(void);

void func_0029AA48(MenuLayoutContext *context) {
    mnuDrawCampIconBackdrop((s32)context->menuList, 0x20);
}

struct FrFontGlyph;

extern void func_00306CD0(s32, s32, s32, u32, s32, s32, s32, s32);
extern u32 uiBlendColors(u32, u32, s32);
extern s32 func_0035C860(char *, const char *, ...);
extern u32 func_0019F6C8(s32, s32, s32, u32, char *, s32);
extern void frFontSetChainFlag(struct FrFontGlyph *, u8);
extern s32 func_0019D550(struct FrFontGlyph *, s8, u32);
extern s32 frFontQueueGlyphInSelectedSlot(struct FrFontGlyph *);
extern u32 mnuKindIsSelectable(u32);
extern char D_004379B0[];

void func_0029AA68(MenuLayoutContext *context) {
    char text[16];
    s32 iconFade = context->iconFade;
    u8 *item = (u8 *)context->config[0];
    s32 delta;
    s32 x;
    struct FrFontGlyph *glyph;
    u32 color;

    func_00306CD0(0x1710, 0x8C0, 0, iconFade, 1,
                  context->backdropContext, 8, 0x53);
    delta = context->suppressDelta != 0 ? 0 : context->selectionWidth - context->margin;
    x = 0x19C0;
    if (delta / 100 <= 0) {
        if (delta / 10 > 0) {
            x = 0x1A50;
        } else {
            x = 0x1AA0;
        }
    }
    color = uiBlendColors(0xA09DC380, 0xA09DC300, iconFade);
    func_0035C860(text, D_004379B0, delta);
    glyph = (struct FrFontGlyph *)func_0019F6C8(x, 0x8E8, 0, color, text, 0);
    frFontSetChainFlag(glyph, 3);
    func_0019D550(glyph, 1, 0x53);
    frFontQueueGlyphInSelectedSlot(glyph);
    if (mnuKindIsSelectable(*(u16 *)(item + 4)) != 0) {
        if (context->iconFade < 0x100) {
            context->iconFade += 0x20;
        }
        if (context->iconFade > 0x100) {
            context->iconFade = 0x100;
        }
    } else {
        context->iconFade = 0;
    }
}

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
