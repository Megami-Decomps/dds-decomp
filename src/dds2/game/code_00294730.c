#include "common.h"
#include "dat_state.h"
#include "mnu_list.h"
#include "mnu_shop.h"

typedef s16 MenuIconPlacement[4];

enum {
    MENU_ICON_TEXTURE,
    MENU_ICON_FRAME,
    MENU_ICON_X,
    MENU_ICON_Y
};

extern MenuIconPlacement D_003D03F0[];
extern void func_00306CD0(s32, s32, s32, u32, s32,
                          struct EffectSlotSet *, s32, s32);
extern void func_00294680(MenuTerminalContext *, s32, s32);
extern void func_00296D90(MenuTerminalContext *, s32);

extern void func_00295D38();

extern void func_002958B0();

extern void mnuDrawListChildrenWithCountdown(s32, s32, s32, u8 *, s32);

extern void func_002960F0(s32, s32, s32, s32, u8 *, s32);





u32 evtSelectGraphicSlotBySpriteType(MenuTerminalContext *object) {
    u32 result;

    result = 0;
    if ((object->type == 1) || (object->type == 3)) {
        result = 0x3a;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00294730", func_00294758);

void func_00294930(MenuTerminalContext *object, s32 elapsedFrames) {
    f32 elapsed = (f32)elapsedFrames - 0.0f;
    s32 frameCount = (s32)elapsed;
    f32 alpha;
    s32 scale;
    u32 index;
    MenuTerminalContext *resources;

    if (frameCount < 0) {
        frameCount = 0;
    }
    alpha = (f32)frameCount / 20.0f;
    if (alpha > 1.0f) {
        alpha = 1.0f;
    }
    if (alpha < 0.0f) {
        alpha = 0.0f;
    }
    alpha = 1.0f - alpha;
    scale = (s32)(alpha * 256.0f);

    func_00306CD0(D_003D03F0[32][MENU_ICON_X] * 16,
                  D_003D03F0[32][MENU_ICON_Y] * 8, 0, scale, 0,
                  D_00438FC8->effectSlots[D_003D03F0[32][MENU_ICON_TEXTURE]],
                  D_003D03F0[32][MENU_ICON_FRAME], 0x53);
    func_0026BEC0(0, 0, 0, scale, &object->panelWork[0], 0x53);
    func_0026BEC0(0, 0xCF8, 0, scale, &object->panelWork[1], 0x53);
    func_00294680(object, scale, 0x53);

    frameCount = (s32)elapsed;
    alpha = (f32)frameCount / 20.0f;
    if (alpha > 1.0f) {
        alpha = 1.0f;
    }
    alpha = 1.0f - alpha;
    func_00296D90(object, 0xA09DC300 | (s32)(alpha * 128.0f));

    index = evtSelectGraphicSlotBySpriteType(object);
    resources = D_00438FC8;
    scale = (s32)(alpha * 256.0f);
    func_00306CD0(D_003D03F0[index][MENU_ICON_X] * 16,
                  D_003D03F0[index][MENU_ICON_Y] * 8, 0, scale, 0,
                  resources->effectSlots[D_003D03F0[index][MENU_ICON_TEXTURE]],
                  D_003D03F0[index][MENU_ICON_FRAME], 0x53);
}





void func_00294B40(s32 x, s32 y, s32 depth, MenuTerminalContext *object,
                   s32 scale, s32 option) {
    u32 index;

    func_00306CD0(D_003D03F0[32][MENU_ICON_X] * 16, D_003D03F0[32][MENU_ICON_Y] * 8,
                  0, scale, 0, D_00438FC8->effectSlots[D_003D03F0[32][MENU_ICON_TEXTURE]],
                  D_003D03F0[32][MENU_ICON_FRAME], option);
    func_0026BEC0(0, 0, 0, scale, &object->panelWork[0], option);
    func_0026BEC0(0, 0xCF8, 0, scale, &object->panelWork[1], option);
    func_00294680(object, scale, option);
    index = evtSelectGraphicSlotBySpriteType(object);
    func_00306CD0(D_003D03F0[index][MENU_ICON_X] * 16, D_003D03F0[index][MENU_ICON_Y] * 8,
                  0, scale, 0, D_00438FC8->effectSlots[D_003D03F0[index][MENU_ICON_TEXTURE]],
                  D_003D03F0[index][MENU_ICON_FRAME], option);
}

extern f32 sdfSinPoly(f32);

void mnuDrawPulsingMenuIcon(MenuTerminalContext *object, s32 amplitude, s32 drawArg) {
    struct EffectSlotSet *texture = D_00438FC8->effectSlots[0];
    s32 alpha;

    alpha = (s32)((f32)amplitude * sdfSinPoly((object->pulseFrame / 120.0f) * 6.2831853f));
    func_00306CD0(D_003D03F0[1][MENU_ICON_X] << 4, D_003D03F0[1][MENU_ICON_Y] << 3,
                  0, alpha, 0, texture, D_003D03F0[1][MENU_ICON_FRAME], drawArg);
    object->pulseFrame++;
    if (object->pulseFrame >= 120.0f) {
        object->pulseFrame = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00294730", func_00294D50);

extern void func_002B9A40(s32, s32, s32, s32, struct MenuList *, void *, s32);
void func_00294EB8(s32 x, s32 y, s32 unused, MenuTerminalContext *object, s32 scale, s32 option) {
    struct EffectSlotSet *texture = D_00438FC8->effectSlots[0];
    s32 i;

    func_00306CD0(x + (D_003D03F0[57][MENU_ICON_X] << 4),
                  y + (D_003D03F0[57][MENU_ICON_Y] << 3),
                  0, scale, 0, texture, D_003D03F0[57][MENU_ICON_FRAME], option);
    for (i = 0; i < object->window->list->visibleCount; i++) {
        func_00306CD0(x + (D_003D03F0[18][MENU_ICON_X] << 4),
                      y + (D_003D03F0[18][MENU_ICON_Y] << 3),
                      0, scale, 0, texture, D_003D03F0[18][MENU_ICON_FRAME], option);
        y += 0xB0;
    }
    func_00306CD0(D_003D03F0[17][MENU_ICON_X] << 4, D_003D03F0[17][MENU_ICON_Y] << 3,
                  0, scale, 0, texture, D_003D03F0[17][MENU_ICON_FRAME], option);
    func_002B9A40(0x2D0, 0x450, 0, scale, object->window->list, (void *)object->windowResource, option);
}


INCLUDE_ASM(const s32, "game/code_00294730", func_00295030);

INCLUDE_ASM(const s32, "game/code_00294730", func_00295400);

INCLUDE_ASM(const s32, "game/code_00294730", func_002958B0);

INCLUDE_ASM(const s32, "game/code_00294730", func_00295D38);


/* Draw the child and its container only while the child is active. */
void mnuDrawIfActive(s32 x, s32 y, s32 z, MenuWindowContainer *object, s32 drawArg) {
    struct MenuList *inner = object->list;

    if (inner->count != 0) {
        mnuDrawListChildrenWithCountdown(x, y, z, (u8 *)inner, drawArg);
        func_002960F0(x, y, z, 0, (u8 *)object, drawArg);
        object->flags |= 4;
    }
}


/* Tick the list's countdown, then run its draw callback on up to `count` linked children. */
void mnuDrawListChildrenWithCountdown(s32 x, s32 y, s32 z, u8 *object, s32 drawArg) {
    struct MenuList *list = (struct MenuList *)object;
    struct MenuListNode *child;
    s32 i;
    s32 *delay = list->context;

    if (delay != NULL) {
        if (*delay != 0) {
            *delay = *delay - 1;
        }
    }
    i = 0;
    child = list->head;
    while (i < list->visibleCount && child != NULL) {
        if (list->drawCallback != NULL) {
            list->drawCallback(x, y, z, list, child, drawArg);
        }
        i++;
        child = child->next;
    }
}

void func_002960F0(s32 x, s32 y, s32 z, s32 unused, u8 *objectData, s32 option) {
    MenuWindowContainer *object = (MenuWindowContainer *)objectData;
    struct MenuList *inner = object->list;
    struct EffectSlotSet *texture = D_00438FC8->effectSlots[0];
    MenuTerminalWindowState *state = inner->context;
    s32 mode = state->command.mode;
    s32 flags = inner->flags;
    f32 alpha = 0.0f;

    switch (mode) {
    case 1:
        alpha = (f32)state->command.value / 15.0f;
        alpha = 1.0f - alpha;
        break;
    case 2:
        alpha = (f32)state->command.value / 15.0f;
        break;
    }
    if (flags & 1) {
        func_00306CD0(D_003D03F0[20][MENU_ICON_X] << 4,
                      D_003D03F0[20][MENU_ICON_Y] << 3,
                      0, (u32)(alpha * 256.0f), 0,
                      texture, D_003D03F0[20][MENU_ICON_FRAME], option);
    }
    if (flags & 2) {
        func_00306CD0(D_003D03F0[21][MENU_ICON_X] << 4,
                      D_003D03F0[21][MENU_ICON_Y] << 3,
                      0, (u32)(alpha * 256.0f), 0,
                      texture, D_003D03F0[21][MENU_ICON_FRAME], option);
    }
}

INCLUDE_ASM(const s32, "game/code_00294730", func_00296298);

struct FrFontGlyph;

extern s32 func_0035C860(char *, const char *, ...);
extern u32 func_0019F798(s32, s32, s32, u32, char *, s32);
extern void frFontSetChainFlag(struct FrFontGlyph *, u8);
extern s32 func_0019D550(struct FrFontGlyph *, s8, u32);
extern s32 frFontQueueGlyphInSelectedSlot(struct FrFontGlyph *);
extern char D_00437978[]; /* "%d" */

/* Fade the selected row's icons and quantity while preserving its row snapshot. */
void func_00296430(s32 x, s32 y, s32 depth, MenuTerminalContext *scene,
                   u32 alpha, s32 option) {
    char text[16];
    struct MenuList *list = scene->window->list;
    struct EffectSlotSet *texture = D_00438FC8->effectSlots[0];
    f32 opacity;
    s32 row;
    s32 firstIndex;
    s32 textOffset;
    u32 color;
    struct FrFontGlyph *glyph;

    if (list->count != 0) {
        firstIndex = list->head->index;
        opacity = (f32)alpha * 0.00390625f;
        row = list->cursor->index - firstIndex;
        func_00306CD0((D_003D03F0[19][MENU_ICON_X] + 8) << 4,
                      (D_003D03F0[19][MENU_ICON_Y] + row * 22) << 3,
                      0, (u32)((1.0f - opacity) * 256.0f), 0, texture,
                      D_003D03F0[19][MENU_ICON_FRAME], option);
        func_00306CD0(0x600, (152 + row * 22) << 3, 0,
                      (u32)((1.0f - opacity) * 128.0f + 128.0f),
                      0, texture, 0x20, option);
        func_00306CD0(0xD10, (152 + row * 22) << 3, 0,
                      (u32)((1.0f - opacity) * 128.0f + 128.0f),
                      0, texture, 0x21, option);
        func_00306CD0((D_003D03F0[29][MENU_ICON_X] + 8) << 4,
                      (D_003D03F0[29][MENU_ICON_Y] + row * 22) << 3,
                      0, (u32)(opacity * 256.0f), 0, texture,
                      D_003D03F0[29][MENU_ICON_FRAME], option);
        color = (s32)(opacity * 128.0f) | 0xA09DC300;
        textOffset = 0x70;
        if (scene->multiplier / 10 != 0) {
            textOffset = 0;
        }
        func_0035C860(text, D_00437978, scene->multiplier);
        glyph = (struct FrFontGlyph *)func_0019F798(
            0xD30 + textOffset, (135 + row * 22) << 3, depth, color, text, 0);
        frFontSetChainFlag(glyph, 4);
        func_0019D550(glyph, 1, option);
        frFontQueueGlyphInSelectedSlot(glyph);
    }
}


void func_002967A0(s32 x, s32 y, s32 z, MenuTerminalContext *panel, s32 option) {
    struct EffectSlotSet *texture = D_00438FC8->effectSlots[0];
    s32 firstIndex = panel->window->list->head->index;
    s32 row = panel->window->list->cursor->index - firstIndex;

    if (panel->unkC7 != 1) {
        func_00306CD0(D_003D03F0[30][MENU_ICON_X] << 4,
            (D_003D03F0[30][MENU_ICON_Y] + row * 22) << 3,
            0, 0x100, 0, texture, D_003D03F0[30][MENU_ICON_FRAME], option);
    }
    if (panel->multiplier != 1) {
        func_00306CD0(D_003D03F0[31][MENU_ICON_X] << 4,
            (D_003D03F0[31][MENU_ICON_Y] + row * 22) << 3,
            0, 0x100, 0, texture, D_003D03F0[31][MENU_ICON_FRAME], option);
    }
}

void func_002968B8(s32 x, s32 y, s32 z, MenuTerminalContext *panel, s32 scale, s32 option) {
    struct EffectSlotSet *texture = D_00438FC8->effectSlots[0];
    s32 firstIndex = panel->window->list->head->index;
    s32 row = panel->window->list->cursor->index - firstIndex;

    if (panel->unkC7 != 1) {
        func_00306CD0(D_003D03F0[30][MENU_ICON_X] << 4,
            (D_003D03F0[30][MENU_ICON_Y] + row * 22) << 3,
            0, scale, 0, texture, D_003D03F0[30][MENU_ICON_FRAME], option);
    }
    if (panel->multiplier != 1) {
        func_00306CD0(D_003D03F0[31][MENU_ICON_X] << 4,
            (D_003D03F0[31][MENU_ICON_Y] + row * 22) << 3,
            0, scale, 0, texture, D_003D03F0[31][MENU_ICON_FRAME], option);
    }
}

extern char D_00437980[];

void func_002969D8(s32 x, s32 y, s32 depth, MenuTerminalContext *panel, s32 option) {
    char text[16];
    struct EffectSlotSet *texture = D_00438FC8->effectSlots[0];
    MenuWindowContainer *object = panel->window;
    struct MenuList *inner;
    struct FrFontGlyph *glyph;

    func_00306CD0(D_003D03F0[25][MENU_ICON_X] * 16, D_003D03F0[25][MENU_ICON_Y] * 8,
                  0, 0x100, 0, texture, D_003D03F0[25][MENU_ICON_FRAME], option);
    func_00306CD0(D_003D03F0[26][MENU_ICON_X] * 16, D_003D03F0[26][MENU_ICON_Y] * 8,
                  0, 0x100, 0, texture, D_003D03F0[26][MENU_ICON_FRAME], option);
    inner = object->list;
    if (inner->count != 0) {
        func_0035C860(text, D_00437980, 0);
        glyph = (struct FrFontGlyph *)func_0019F798(0x1910, 0x290, depth, 0xA09DC380, text, 0);
        frFontSetChainFlag(glyph, 4);
        func_0019D550(glyph, 1, option);
        frFontQueueGlyphInSelectedSlot(glyph);
    }
}

void mnuDrawIconFixedEntry(s32 x, s32 y, s32 z, s32 unused, s32 scale, s32 option) {
    func_00306CD0(
        x + D_003D03F0[26][MENU_ICON_X] * 16,
        y + D_003D03F0[26][MENU_ICON_Y] * 8,
        z, scale, 0, D_00438FC8->effectSlots[0],
        D_003D03F0[26][MENU_ICON_FRAME], option
    );
}

extern void sndSetSequenceVolumePan(s32, s32, s32);


void func_00296B48(s32 x, s32 y, s32 z, s32 unused, s32 scale, s32 option) {
    char text[16];
    s32 value;
    struct FrFontGlyph *glyph;

    func_00306CD0(
        x + D_003D03F0[25][MENU_ICON_X] * 16,
        y + D_003D03F0[25][MENU_ICON_Y] * 8,
        z, scale, 0, D_00438FC8->effectSlots[0],
        D_003D03F0[25][MENU_ICON_FRAME], option
    );
    value = (s32)((f32)(scale << 7) * 0.00390625f) | 0xA09DC300;
    func_0035C860(text, D_00437980, 0);
    glyph = (struct FrFontGlyph *)func_0019F798(x + 0x1910, y + 0x290, z, value, text, 0);
    frFontSetChainFlag(glyph, 4);
    func_0019D550(glyph, 1, option);
    frFontQueueGlyphInSelectedSlot(glyph);
}

void func_00296C58(s32 x, s32 y, s32 depth, MenuTerminalContext *panel, s32 option) {
    char text[16];
    struct EffectSlotSet *texture = D_00438FC8->effectSlots[0];
    MenuWindowContainer *object = panel->window;
    struct MenuList *inner;
    struct FrFontGlyph *glyph;

    func_00306CD0(D_003D03F0[25][MENU_ICON_X] * 16, D_003D03F0[25][MENU_ICON_Y] * 8,
                  0, 0x100, 0, texture, D_003D03F0[25][MENU_ICON_FRAME], option);
    func_00306CD0(D_003D03F0[26][MENU_ICON_X] * 16, D_003D03F0[26][MENU_ICON_Y] * 8,
                  0, 0x100, 0, texture, D_003D03F0[26][MENU_ICON_FRAME], option);
    inner = object->list;
    if (inner->count != 0) {
        func_0035C860(text, D_00437980, inner->cursor->camp.value * panel->multiplier);
        glyph = (struct FrFontGlyph *)func_0019F798(0x1910, 0x290, depth, 0xA09DC380, text, 0);
        frFontSetChainFlag(glyph, 4);
        func_0019D550(glyph, 1, option);
        frFontQueueGlyphInSelectedSlot(glyph);
    }
}


void func_00296D90(MenuTerminalContext *state, s32 style) {
    char text[16];

    if (datGameState->header.currency != state->previousValue) {
        s32 transitionFrames = 20;
        s32 displayedValue;

        sndSetSequenceVolumePan(19, 127, 63);
        state->elapsedFrames++;
        displayedValue = state->previousValue +
                         ((datGameState->header.currency - state->previousValue) * state->elapsedFrames) /
                             transitionFrames;
        func_0035C860(text, D_00437980, displayedValue);
        if (state->elapsedFrames == transitionFrames) {
            state->previousValue = datGameState->header.currency;
            state->elapsedFrames = 0;
        }
    } else {
        func_0035C860(text, D_00437980, datGameState->header.currency);
    }

    {
        struct FrFontGlyph *glyph = (struct FrFontGlyph *)func_0019F798(0x1910, 0x1D0, 0, style, text, 0);

        func_0019D550(glyph, 1, 0x53);
        frFontQueueGlyphInSelectedSlot(glyph);
    }
}

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437968);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437970);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437978);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437980);

