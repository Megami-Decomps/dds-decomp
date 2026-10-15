#include "common.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "fr_font.h"
#include "dat_state.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "dat_command.h"

typedef s16 MenuIconPlacement[4];

enum {
    MENU_ICON_TEXTURE,
    MENU_ICON_FRAME,
    MENU_ICON_X,
    MENU_ICON_Y
};

extern MenuIconPlacement D_003D03F0[];
extern void effDrawTextureSlotWithBlendedCornerColors(s32, s32, s32, u32, s32,
                          struct EffectSlotSet *, s32, s32);
extern void mnuDrawType2PanelEffects(MenuTerminalContext *, s32, s32);
extern void mnuDrawShopCurrencyBalance(MenuTerminalContext *, s32);

extern void mnuDrawFadingValueListRow();

extern void mnuDrawCampItemListRow();

extern void mnuDrawListChildrenWithCountdown(s32, s32, s32, u8 *, s32);

extern void mnuDrawListFlagIndicatorsWithFade(s32, s32, s32, s32, u8 *, s32);





u32 evtSelectGraphicSlotBySpriteType(MenuTerminalContext *object) {
    u32 result;

    result = 0;
    if ((object->type == 1) || (object->type == 3)) {
        result = 0x3a;
    }
    return result;
}

void mnuDrawTerminalPanelTransition(MenuTerminalContext *object, s32 elapsedFrames) {
    s32 frames = elapsedFrames;
    s32 frameCount;
    f32 alpha;
    s32 scale;
    u32 index;
    MenuTerminalContext *resources;

    if (frames < 0) {
        frames = 0;
    }
    frameCount = frames;
    if (frameCount < 0) {
        frameCount = 0;
    }
    alpha = (f32)frameCount / 15.0f;
    if (alpha > 1.0f) {
        alpha = 1.0f;
    }
    scale = (s32)(alpha * 256.0f);

    effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[32][MENU_ICON_X] * 16,
                  D_003D03F0[32][MENU_ICON_Y] * 8, 0, scale, 0,
                  D_00438FC8->effectSlots[D_003D03F0[32][MENU_ICON_TEXTURE]],
                  D_003D03F0[32][MENU_ICON_FRAME], 0x53);
    mnuDrawAndAdvanceScrollingStrips(0, 0, 0, scale, &object->panelWork[0], 0x53);
    mnuDrawAndAdvanceScrollingStrips(0, 0xCF8, 0, scale, &object->panelWork[1], 0x53);
    mnuDrawType2PanelEffects(object, scale, 0x53);

    frameCount = frames - 5;
    if (frameCount < 0) {
        frameCount = 0;
    }
    alpha = (f32)frameCount / 15.0f;
    if (alpha > 1.0f) {
        alpha = 1.0f;
    }
    index = evtSelectGraphicSlotBySpriteType(object);
    resources = D_00438FC8;
    scale = (s32)(alpha * 256.0f);
    effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[index][MENU_ICON_X] * 16,
                  D_003D03F0[index][MENU_ICON_Y] * 8, 0, scale, 0,
                  resources->effectSlots[D_003D03F0[index][MENU_ICON_TEXTURE]],
                  D_003D03F0[index][MENU_ICON_FRAME], 0x53);
}

void mnuDrawSpriteMenuFadeOut(MenuTerminalContext *object, s32 elapsedFrames) {
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

    effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[32][MENU_ICON_X] * 16,
                  D_003D03F0[32][MENU_ICON_Y] * 8, 0, scale, 0,
                  D_00438FC8->effectSlots[D_003D03F0[32][MENU_ICON_TEXTURE]],
                  D_003D03F0[32][MENU_ICON_FRAME], 0x53);
    mnuDrawAndAdvanceScrollingStrips(0, 0, 0, scale, &object->panelWork[0], 0x53);
    mnuDrawAndAdvanceScrollingStrips(0, 0xCF8, 0, scale, &object->panelWork[1], 0x53);
    mnuDrawType2PanelEffects(object, scale, 0x53);

    frameCount = (s32)elapsed;
    alpha = (f32)frameCount / 20.0f;
    if (alpha > 1.0f) {
        alpha = 1.0f;
    }
    alpha = 1.0f - alpha;
    mnuDrawShopCurrencyBalance(object, 0xA09DC300 | (s32)(alpha * 128.0f));

    index = evtSelectGraphicSlotBySpriteType(object);
    resources = D_00438FC8;
    scale = (s32)(alpha * 256.0f);
    effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[index][MENU_ICON_X] * 16,
                  D_003D03F0[index][MENU_ICON_Y] * 8, 0, scale, 0,
                  resources->effectSlots[D_003D03F0[index][MENU_ICON_TEXTURE]],
                  D_003D03F0[index][MENU_ICON_FRAME], 0x53);
}





void mnuDrawShopPanelWithTypeGraphic(s32 x, s32 y, s32 depth, MenuTerminalContext *object,
                   s32 scale, s32 option) {
    u32 index;

    effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[32][MENU_ICON_X] * 16, D_003D03F0[32][MENU_ICON_Y] * 8,
                  0, scale, 0, D_00438FC8->effectSlots[D_003D03F0[32][MENU_ICON_TEXTURE]],
                  D_003D03F0[32][MENU_ICON_FRAME], option);
    mnuDrawAndAdvanceScrollingStrips(0, 0, 0, scale, &object->panelWork[0], option);
    mnuDrawAndAdvanceScrollingStrips(0, 0xCF8, 0, scale, &object->panelWork[1], option);
    mnuDrawType2PanelEffects(object, scale, option);
    index = evtSelectGraphicSlotBySpriteType(object);
    effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[index][MENU_ICON_X] * 16, D_003D03F0[index][MENU_ICON_Y] * 8,
                  0, scale, 0, D_00438FC8->effectSlots[D_003D03F0[index][MENU_ICON_TEXTURE]],
                  D_003D03F0[index][MENU_ICON_FRAME], option);
}


void mnuDrawPulsingMenuIcon(MenuTerminalContext *object, s32 amplitude, s32 drawArg) {
    struct EffectSlotSet *texture = D_00438FC8->effectSlots[0];
    s32 alpha;

    alpha = (s32)((f32)amplitude * sdfSinPoly((object->pulseFrame / 120.0f) * 6.2831853f));
    effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[1][MENU_ICON_X] << 4, D_003D03F0[1][MENU_ICON_Y] << 3,
                  0, alpha, 0, texture, D_003D03F0[1][MENU_ICON_FRAME], drawArg);
    object->pulseFrame++;
    if (object->pulseFrame >= 120.0f) {
        object->pulseFrame = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00294730", mnuDrawTerminalPanelListRows);

void mnuDrawWindowBorderAndScrollBar(s32 x, s32 y, s32 unused, MenuTerminalContext *object, s32 scale, s32 option) {
    struct EffectSlotSet *texture = D_00438FC8->effectSlots[0];
    s32 i;

    effDrawTextureSlotWithBlendedCornerColors(x + (D_003D03F0[57][MENU_ICON_X] << 4),
                  y + (D_003D03F0[57][MENU_ICON_Y] << 3),
                  0, scale, 0, texture, D_003D03F0[57][MENU_ICON_FRAME], option);
    for (i = 0; i < object->window->list->visibleCount; i++) {
        effDrawTextureSlotWithBlendedCornerColors(x + (D_003D03F0[18][MENU_ICON_X] << 4),
                      y + (D_003D03F0[18][MENU_ICON_Y] << 3),
                      0, scale, 0, texture, D_003D03F0[18][MENU_ICON_FRAME], option);
        y += 0xB0;
    }
    effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[17][MENU_ICON_X] << 4, D_003D03F0[17][MENU_ICON_Y] << 3,
                  0, scale, 0, texture, D_003D03F0[17][MENU_ICON_FRAME], option);
    mnuDrawListScrollbar(0x2D0, 0x450, 0, scale, object->window->list, object->windowResource, option);
}


extern s32 evtGetIndexedEventRecordId(s32);
extern s32 ptyCountBulletItem(s32);
extern s32 ptyGetCombinedRecordAndSlotValue(s32, s32);
extern s32 sprintf(char *, const char *, ...);
extern u32 itfCreateTightFont3TextGlyph(s32, s32, s32, u32, char *, s32);
extern char D_00437968[];

/* Draw the terminal value panel for the selected item: its frames, then by
 * item kind its power (kind 1) or per-stat bonuses (kind 3), and the count. */
void mnuDrawTerminalPanelValues(s32 x, s32 y, s32 z, MenuTerminalContext *object, s32 scale,
                                s32 showFrame, s32 option) {
    struct EffectSlotSet *texture = D_00438FC8->effectSlots[0];
    char text[16];
    s32 count;
    u32 color;
    CampWindowParams *item;
    struct FrFontGlyph *glyph;
    s32 value;
    s32 i;
    s32 id;
    s32 textX;
    s32 row;

    if (showFrame != 0) {
        effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[55][MENU_ICON_X] << 4, D_003D03F0[55][MENU_ICON_Y] << 3,
                      0, scale, 0, texture, D_003D03F0[55][MENU_ICON_FRAME], option);
    }
    effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[23][MENU_ICON_X] << 4, D_003D03F0[23][MENU_ICON_Y] << 3,
                  0, scale, 0, texture, D_003D03F0[23][MENU_ICON_FRAME], option);
    effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[24][MENU_ICON_X] << 4, D_003D03F0[24][MENU_ICON_Y] << 3,
                  0, scale, 0, texture, D_003D03F0[24][MENU_ICON_FRAME], option);
    if (object->window->list->count == 0) {
        return;
    }
    item = &object->window->list->cursor->camp;
    id = item->id;
    count = datGameState->inventory.counts[id];
    color = (s32)((f32)(scale << 7) * 0.00390625f) | 0xA09DC300;
    switch (item->mode) {
    case 1: {
        s32 left;

        effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[54][MENU_ICON_X] << 4, D_003D03F0[54][MENU_ICON_Y] << 3,
                      0, scale, 0, texture, D_003D03F0[54][MENU_ICON_FRAME], option);
        value = datCommandRecords[evtGetIndexedEventRecordId(id)].hpPower;
        sprintf(text, D_00437968, value);
        left = 0x990;
        if (value / 100 == 0) {
            left = 0x9D0;
        }
        glyph = itfCreateFont2TextGlyph(left, 0xB18, z, color, text, NULL);
        frFontDrawGlyphChain(glyph, 1, option);
        frFontQueueGlyphForCurrentDrawBuffer(glyph);
        break;
    }
    case 3:
        for (i = 0, textX = 0x780; i < 5; i++, textX += 0x3F0) {
            value = ptyGetCombinedRecordAndSlotValue(id, i);
            if (value != 0) {
                row = 44 + i * 2;
                effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[row][MENU_ICON_X] << 4,
                                                          D_003D03F0[row][MENU_ICON_Y] << 3, 0, scale, 0, texture,
                                                          D_003D03F0[row][MENU_ICON_FRAME], option);
                sprintf(text, D_00437968, value);
                glyph = itfCreateFont2TextGlyph(textX, 0xB18, z, color, text, NULL);
                frFontDrawGlyphChain(glyph, 1, option);
                frFontQueueGlyphForCurrentDrawBuffer(glyph);
            } else {
                row = 45 + i * 2;
                effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[row][MENU_ICON_X] << 4,
                                                          D_003D03F0[row][MENU_ICON_Y] << 3, 0, scale, 0, texture,
                                                          D_003D03F0[row][MENU_ICON_FRAME], option);
            }
        }
        break;
    }
    if (item->mode == 1) {
        count = ptyCountBulletItem(id);
    }
    sprintf(text, D_00437968, count);
    glyph = (struct FrFontGlyph *)itfCreateTightFont3TextGlyph(0x1700, 0xA58, z, color, text, 0);
    frFontDrawGlyphChain(glyph, 1, option);
    frFontQueueGlyphForCurrentDrawBuffer(glyph);
}

INCLUDE_ASM(const s32, "game/code_00294730", mnuDrawMenuItemListRow);

INCLUDE_ASM(const s32, "game/code_00294730", mnuDrawCampItemListRow);

/* Shop list draw context: row-fade countdown and mode (same 0x10-byte buffer as DDS1). */
typedef struct MnuShopListContext {
    s32 countdown;
    s32 mode;
    s32 unk08;
    u16 extraOption;
    s16 pendingSelection;
} MnuShopListContext;

extern s32 itfSetTextDrawLimit(s32);
extern struct FrFontGlyph *itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, struct FrFontGlyph *);
extern char D_00437970[];

/* Draw the row label, and draw its numeric value when the list enables it. */
void mnuDrawFadingValueListRow(s32 x, s32 y, s32 z, struct MenuList *list,
                   struct MenuListNode *node, s32 drawArg) {
    MnuShopListContext *context = (MnuShopListContext *)list->context;
    s32 row = node->index - list->head->index;
    s32 rowOffset;
    s32 mode = context->mode;
    f32 fade = 0.0f;
    u32 style = 0xA09DC300;
    s32 selected = 0;
    s32 value;
    struct FrFontGlyph *glyph;
    char text[16];

    switch (mode) {
    case 1:
        fade = (f32)context->countdown / 15.0f;
        break;
    case 2:
        fade = (f32)context->countdown / 15.0f;
        fade = 1.0f - fade;
        break;
    }

    if (node->flags48 & 1) {
        style = 0xA09DC340;
    } else {
        struct MenuListNode *cursor = list->cursor;
        s32 opacity = (s32)(fade * 64.0f + 64.0f);

        if (cursor == node) {
            style = 0xA09DC380;
            selected = 1;
        } else {
            style |= opacity;
        }
    }

    itfSetTextDrawLimit(0x13);
    rowOffset = row * 22;
    glyph = itfCreateConvertedTextGlyph(0x420, (rowOffset + 0x87) << 3, z,
                                        style, (const u8 *)node->value, NULL);
    if (selected) {
        frFontSetChildChainFirstOption(glyph, 4);
    }
    frFontDrawGlyphChain(glyph, 1, drawArg);
    frFontQueueGlyphForCurrentDrawBuffer(glyph);
    itfSetTextDrawLimit(-1);

    if ((list->id & 1) == 0) {
        return;
    }

    if (list->cursor == node) {
        style = (s32)(fade * 64.0f + 64.0f) | 0xA09DC300;
    }
    value = (s32)node->sortKeyPrimary;
    sprintf(text, D_00437970, value);
    glyph = itfCreateFont2TextGlyph(0xFF0, (rowOffset + 0x8A) << 3, z, style, text, NULL);
    if (selected) {
        frFontSetChildChainFirstOption(glyph, 4);
    }
    frFontDrawGlyphChain(glyph, 1, drawArg);
    frFontQueueGlyphForCurrentDrawBuffer(glyph);
}


/* Draw the child and its container only while the child is active. */
void mnuDrawIfActive(s32 x, s32 y, s32 z, MenuWindowContainer *object, s32 drawArg) {
    struct MenuList *inner = object->list;

    if (inner->count != 0) {
        mnuDrawListChildrenWithCountdown(x, y, z, (u8 *)inner, drawArg);
        mnuDrawListFlagIndicatorsWithFade(x, y, z, 0, (u8 *)object, drawArg);
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

void mnuDrawListFlagIndicatorsWithFade(s32 x, s32 y, s32 z, s32 unused, u8 *objectData, s32 option) {
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
        effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[20][MENU_ICON_X] << 4,
                      D_003D03F0[20][MENU_ICON_Y] << 3,
                      0, (u32)(alpha * 256.0f), 0,
                      texture, D_003D03F0[20][MENU_ICON_FRAME], option);
    }
    if (flags & 2) {
        effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[21][MENU_ICON_X] << 4,
                      D_003D03F0[21][MENU_ICON_Y] << 3,
                      0, (u32)(alpha * 256.0f), 0,
                      texture, D_003D03F0[21][MENU_ICON_FRAME], option);
    }
}

struct FrFontGlyph;

extern s32 frFontDrawGlyphChain(struct FrFontGlyph *, s8, u32);
extern char D_00437978[]; /* "%d" */

/* Draw the selected row's icons and quantity at full opacity. */
void mnuDrawShopSelectedRowDetails(s32 x, s32 y, s32 depth, MenuTerminalContext *scene, s32 option) {
    char text[16];
    struct MenuList *list = scene->window->list;
    struct EffectSlotSet *texture = D_00438FC8->effectSlots[0];
    s32 row;
    s32 firstIndex;
    s32 textOffset;
    struct FrFontGlyph *glyph;

    if (list->count != 0) {
        firstIndex = list->head->index;
        row = list->cursor->index - firstIndex;
        effDrawTextureSlotWithBlendedCornerColors(0x600, (152 + row * 22) << 3, 0, 0x80, 0, texture, 0x20, option);
        effDrawTextureSlotWithBlendedCornerColors(0xD10, (152 + row * 22) << 3, 0, 0x80, 0, texture, 0x21, option);
        effDrawTextureSlotWithBlendedCornerColors((D_003D03F0[29][MENU_ICON_X] + 8) << 4,
                      (D_003D03F0[29][MENU_ICON_Y] + row * 22) << 3,
                      0, 0x100, 0, texture,
                      D_003D03F0[29][MENU_ICON_FRAME], option);
        if (scene->multiplier / 10 != 0) {
            textOffset = 0;
        } else {
            textOffset = 0x70;
        }
        sprintf(text, D_00437978, scene->multiplier);
        glyph = (struct FrFontGlyph *)itfCreateTightFont3TextGlyph(
            0xD30 + textOffset, (135 + row * 22) << 3, depth, 0xA09DC380, text, 0);
        frFontSetChildChainFirstOption(glyph, 4);
        frFontDrawGlyphChain(glyph, 1, option);
        frFontQueueGlyphForCurrentDrawBuffer(glyph);
    }
}

/* Fade the selected row's icons and quantity while preserving its row snapshot. */
void mnuDrawShopSelectedRowDetailsWithAlpha(s32 x, s32 y, s32 depth, MenuTerminalContext *scene,
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
        effDrawTextureSlotWithBlendedCornerColors((D_003D03F0[19][MENU_ICON_X] + 8) << 4,
                      (D_003D03F0[19][MENU_ICON_Y] + row * 22) << 3,
                      0, (u32)((1.0f - opacity) * 256.0f), 0, texture,
                      D_003D03F0[19][MENU_ICON_FRAME], option);
        effDrawTextureSlotWithBlendedCornerColors(0x600, (152 + row * 22) << 3, 0,
                      (u32)((1.0f - opacity) * 128.0f + 128.0f),
                      0, texture, 0x20, option);
        effDrawTextureSlotWithBlendedCornerColors(0xD10, (152 + row * 22) << 3, 0,
                      (u32)((1.0f - opacity) * 128.0f + 128.0f),
                      0, texture, 0x21, option);
        effDrawTextureSlotWithBlendedCornerColors((D_003D03F0[29][MENU_ICON_X] + 8) << 4,
                      (D_003D03F0[29][MENU_ICON_Y] + row * 22) << 3,
                      0, (u32)(opacity * 256.0f), 0, texture,
                      D_003D03F0[29][MENU_ICON_FRAME], option);
        color = (s32)(opacity * 128.0f) | 0xA09DC300;
        textOffset = 0x70;
        if (scene->multiplier / 10 != 0) {
            textOffset = 0;
        }
        sprintf(text, D_00437978, scene->multiplier);
        glyph = (struct FrFontGlyph *)itfCreateTightFont3TextGlyph(
            0xD30 + textOffset, (135 + row * 22) << 3, depth, color, text, 0);
        frFontSetChildChainFirstOption(glyph, 4);
        frFontDrawGlyphChain(glyph, 1, option);
        frFontQueueGlyphForCurrentDrawBuffer(glyph);
    }
}


void mnuDrawShopLimitArrows(s32 x, s32 y, s32 z, MenuTerminalContext *panel, s32 option) {
    struct EffectSlotSet *texture = D_00438FC8->effectSlots[0];
    s32 firstIndex = panel->window->list->head->index;
    s32 row = panel->window->list->cursor->index - firstIndex;

    if (panel->unkC7 != 1) {
        effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[30][MENU_ICON_X] << 4,
            (D_003D03F0[30][MENU_ICON_Y] + row * 22) << 3,
            0, 0x100, 0, texture, D_003D03F0[30][MENU_ICON_FRAME], option);
    }
    if (panel->multiplier != 1) {
        effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[31][MENU_ICON_X] << 4,
            (D_003D03F0[31][MENU_ICON_Y] + row * 22) << 3,
            0, 0x100, 0, texture, D_003D03F0[31][MENU_ICON_FRAME], option);
    }
}

void mnuDrawShopLimitArrowsScaled(s32 x, s32 y, s32 z, MenuTerminalContext *panel, s32 scale, s32 option) {
    struct EffectSlotSet *texture = D_00438FC8->effectSlots[0];
    s32 firstIndex = panel->window->list->head->index;
    s32 row = panel->window->list->cursor->index - firstIndex;

    if (panel->unkC7 != 1) {
        effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[30][MENU_ICON_X] << 4,
            (D_003D03F0[30][MENU_ICON_Y] + row * 22) << 3,
            0, scale, 0, texture, D_003D03F0[30][MENU_ICON_FRAME], option);
    }
    if (panel->multiplier != 1) {
        effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[31][MENU_ICON_X] << 4,
            (D_003D03F0[31][MENU_ICON_Y] + row * 22) << 3,
            0, scale, 0, texture, D_003D03F0[31][MENU_ICON_FRAME], option);
    }
}

extern char D_00437980[];

void mnuDrawShopPricePlaceholder(s32 x, s32 y, s32 depth, MenuTerminalContext *panel, s32 option) {
    char text[16];
    struct EffectSlotSet *texture = D_00438FC8->effectSlots[0];
    MenuWindowContainer *object = panel->window;
    struct MenuList *inner;
    struct FrFontGlyph *glyph;

    effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[25][MENU_ICON_X] * 16, D_003D03F0[25][MENU_ICON_Y] * 8,
                  0, 0x100, 0, texture, D_003D03F0[25][MENU_ICON_FRAME], option);
    effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[26][MENU_ICON_X] * 16, D_003D03F0[26][MENU_ICON_Y] * 8,
                  0, 0x100, 0, texture, D_003D03F0[26][MENU_ICON_FRAME], option);
    inner = object->list;
    if (inner->count != 0) {
        sprintf(text, D_00437980, 0);
        glyph = (struct FrFontGlyph *)itfCreateTightFont3TextGlyph(0x1910, 0x290, depth, 0xA09DC380, text, 0);
        frFontSetChildChainFirstOption(glyph, 4);
        frFontDrawGlyphChain(glyph, 1, option);
        frFontQueueGlyphForCurrentDrawBuffer(glyph);
    }
}

void mnuDrawIconFixedEntry(s32 x, s32 y, s32 z, s32 unused, s32 scale, s32 option) {
    effDrawTextureSlotWithBlendedCornerColors(
        x + D_003D03F0[26][MENU_ICON_X] * 16,
        y + D_003D03F0[26][MENU_ICON_Y] * 8,
        z, scale, 0, D_00438FC8->effectSlots[0],
        D_003D03F0[26][MENU_ICON_FRAME], option
    );
}
void mnuDrawShopZeroValueIconAndText(s32 x, s32 y, s32 z, s32 unused, s32 scale, s32 option) {
    char text[16];
    s32 value;
    struct FrFontGlyph *glyph;

    effDrawTextureSlotWithBlendedCornerColors(
        x + D_003D03F0[25][MENU_ICON_X] * 16,
        y + D_003D03F0[25][MENU_ICON_Y] * 8,
        z, scale, 0, D_00438FC8->effectSlots[0],
        D_003D03F0[25][MENU_ICON_FRAME], option
    );
    value = (s32)((f32)(scale << 7) * 0.00390625f) | 0xA09DC300;
    sprintf(text, D_00437980, 0);
    glyph = (struct FrFontGlyph *)itfCreateTightFont3TextGlyph(x + 0x1910, y + 0x290, z, value, text, 0);
    frFontSetChildChainFirstOption(glyph, 4);
    frFontDrawGlyphChain(glyph, 1, option);
    frFontQueueGlyphForCurrentDrawBuffer(glyph);
}

void mnuDrawShopSelectedRowTotal(s32 x, s32 y, s32 depth, MenuTerminalContext *panel, s32 option) {
    char text[16];
    struct EffectSlotSet *texture = D_00438FC8->effectSlots[0];
    MenuWindowContainer *object = panel->window;
    struct MenuList *inner;
    struct FrFontGlyph *glyph;

    effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[25][MENU_ICON_X] * 16, D_003D03F0[25][MENU_ICON_Y] * 8,
                  0, 0x100, 0, texture, D_003D03F0[25][MENU_ICON_FRAME], option);
    effDrawTextureSlotWithBlendedCornerColors(D_003D03F0[26][MENU_ICON_X] * 16, D_003D03F0[26][MENU_ICON_Y] * 8,
                  0, 0x100, 0, texture, D_003D03F0[26][MENU_ICON_FRAME], option);
    inner = object->list;
    if (inner->count != 0) {
        sprintf(text, D_00437980, inner->cursor->camp.value * panel->multiplier);
        glyph = (struct FrFontGlyph *)itfCreateTightFont3TextGlyph(0x1910, 0x290, depth, 0xA09DC380, text, 0);
        frFontSetChildChainFirstOption(glyph, 4);
        frFontDrawGlyphChain(glyph, 1, option);
        frFontQueueGlyphForCurrentDrawBuffer(glyph);
    }
}


void mnuDrawShopCurrencyBalance(MenuTerminalContext *state, s32 style) {
    char text[16];

    if (datGameState->header.currency != state->previousValue) {
        s32 transitionFrames = 20;
        s32 displayedValue;

        sndSetSequenceVolumePan(19, 127, 63);
        state->elapsedFrames++;
        displayedValue = state->previousValue +
                         ((datGameState->header.currency - state->previousValue) * state->elapsedFrames) /
                             transitionFrames;
        sprintf(text, D_00437980, displayedValue);
        if (state->elapsedFrames == transitionFrames) {
            state->previousValue = datGameState->header.currency;
            state->elapsedFrames = 0;
        }
    } else {
        sprintf(text, D_00437980, datGameState->header.currency);
    }

    {
        struct FrFontGlyph *glyph = (struct FrFontGlyph *)itfCreateTightFont3TextGlyph(0x1910, 0x1D0, 0, style, text, 0);

        frFontDrawGlyphChain(glyph, 1, 0x53);
        frFontQueueGlyphForCurrentDrawBuffer(glyph);
    }
}

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437968);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437970);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437978);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437980);

