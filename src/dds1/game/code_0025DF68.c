#include "common.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "dat_state.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "itf.h"
#include "fr_font.h"
#include "fr_font_draw_glyph.h"


typedef s16 BrsIconRecord[4];

enum {
    BRS_ICON_ID = 1,
    BRS_ICON_X = 2,
    BRS_ICON_Y = 3,
};

extern BrsIconRecord mnuCommonIconPlacements[];
extern struct EffectSlotSet *D_003BC520;
extern void effDrawSpriteWithCornerFade(s32, s32, s32, u32, s32, struct EffectSlotSet *, s32, s32);

/* Fade the fixed dispatch frame, then its five-tick-delayed foreground icon. */
void mnuDrawDispatchProgressIcons(s32 contextAddress, s32 progress) {
    struct EffectSlotSet *texture = D_003BC520;
    s32 ticks;
    s32 scale;
    f32 fraction;

    if (progress < 0) {
        progress = 0;
    }
    ticks = progress;
    if (ticks < 0) {
        ticks = 0;
    }
    fraction = (f32)ticks / 15.0f;
    if (fraction > 1.0f) {
        fraction = 1.0f;
    }
    scale = (s32)(fraction * 256.0f);
    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[32][BRS_ICON_X] << 4,
                  mnuCommonIconPlacements[32][BRS_ICON_Y] << 3,
                  0, scale, 0, texture, mnuCommonIconPlacements[32][BRS_ICON_ID], 0x53);
    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[27][BRS_ICON_X] << 4,
                  mnuCommonIconPlacements[27][BRS_ICON_Y] << 3,
                  0, scale, 0, texture, mnuCommonIconPlacements[27][BRS_ICON_ID], 0x53);
    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[28][BRS_ICON_X] << 4,
                  mnuCommonIconPlacements[28][BRS_ICON_Y] << 3,
                  0, scale, 0, texture, mnuCommonIconPlacements[28][BRS_ICON_ID], 0x53);
    ticks = progress - 5;
    if (ticks < 0) {
        ticks = 0;
    }
    fraction = (f32)ticks / 15.0f;
    if (fraction > 1.0f) {
        fraction = 1.0f;
    }
    scale = (s32)(fraction * 256.0f);
    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[0][BRS_ICON_X] << 4,
                  mnuCommonIconPlacements[0][BRS_ICON_Y] << 3,
                  0, scale, 0, texture, mnuCommonIconPlacements[0][BRS_ICON_ID], 0x53);
}

extern void mnuDrawShopCurrencyBalance(ShopScene *, s32);
extern void mnuDrawPulsingMenuIcon(ShopScene *, s32, s32);

/* Fade the shop frame, then its currency and pulsing foreground icon. */
void mnuStepShopWindowTransition(ShopScene *scene, s32 frame) {
    f32 elapsed = (f32)frame - 0.0f;
    struct EffectSlotSet *texture = D_003BC520;
    s32 ticks = (s32)elapsed;
    f32 fraction;
    s32 backgroundScale;
    s32 foregroundScale;

    if (ticks < 0) {
        ticks = 0;
    }
    fraction = (f32)ticks / 20.0f;
    if (fraction > 1.0f) {
        fraction = 1.0f;
    }
    if (fraction < 0.0f) {
        fraction = 0.0f;
    }
    fraction = 1.0f - fraction;
    backgroundScale = (s32)(fraction * 256.0f);
    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[32][BRS_ICON_X] << 4,
                  mnuCommonIconPlacements[32][BRS_ICON_Y] << 3,
                  0, backgroundScale, 0, texture, mnuCommonIconPlacements[32][BRS_ICON_ID], 0x53);
    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[27][BRS_ICON_X] << 4,
                  mnuCommonIconPlacements[27][BRS_ICON_Y] << 3,
                  0, backgroundScale, 0, texture, mnuCommonIconPlacements[27][BRS_ICON_ID], 0x53);
    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[28][BRS_ICON_X] << 4,
                  mnuCommonIconPlacements[28][BRS_ICON_Y] << 3,
                  0, backgroundScale, 0, texture, mnuCommonIconPlacements[28][BRS_ICON_ID], 0x53);

    ticks = (s32)elapsed;
    fraction = (f32)ticks / 20.0f;
    if (fraction > 1.0f) {
        fraction = 1.0f;
    }
    fraction = 1.0f - fraction;
    mnuDrawShopCurrencyBalance(scene, (s32)(fraction * 128.0f) | 0xA09DC300);
    {
        s32 x = mnuCommonIconPlacements[0][BRS_ICON_X] << 4;
        s32 y = mnuCommonIconPlacements[0][BRS_ICON_Y] << 3;
        s32 icon = mnuCommonIconPlacements[0][BRS_ICON_ID];

        foregroundScale = (s32)(fraction * 256.0f);
        effDrawSpriteWithCornerFade(x, y, 0, foregroundScale, 0, texture, icon, 0x53);
    }
    mnuDrawPulsingMenuIcon(scene, foregroundScale, 0x53);
}

/* Draw the fixed shop frame, then its pulsing icon. */
void mnuDrawShopPanelWithPulsingIcon(s32 x, s32 y, s32 z, ShopScene *scene, s32 alpha, s32 mode) {
    struct EffectSlotSet *texture = D_003BC520;

    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[32][BRS_ICON_X] << 4,
                  mnuCommonIconPlacements[32][BRS_ICON_Y] << 3,
                  0, alpha, 0, texture, mnuCommonIconPlacements[32][BRS_ICON_ID], mode);
    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[27][BRS_ICON_X] << 4,
                  mnuCommonIconPlacements[27][BRS_ICON_Y] << 3,
                  0, alpha, 0, texture, mnuCommonIconPlacements[27][BRS_ICON_ID], mode);
    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[28][BRS_ICON_X] << 4,
                  mnuCommonIconPlacements[28][BRS_ICON_Y] << 3,
                  0, alpha, 0, texture, mnuCommonIconPlacements[28][BRS_ICON_ID], mode);
    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[0][BRS_ICON_X] << 4,
                  mnuCommonIconPlacements[0][BRS_ICON_Y] << 3,
                  0, alpha, 0, texture, mnuCommonIconPlacements[0][BRS_ICON_ID], mode);
    mnuDrawPulsingMenuIcon(scene, 0x100, mode);
}


void mnuDrawPulsingMenuIcon(ShopScene *object, s32 scale, s32 mode) {
    struct EffectSlotSet *texture = D_003BC520;
    s8 phase = object->pulseFrame;
    f32 angle = (f32)phase / 120.0f * 6.2831852f;
    s32 offset = (s32)((f32)scale * sdfSinPoly(angle));

    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[1][BRS_ICON_X] << 4,
                  mnuCommonIconPlacements[1][BRS_ICON_Y] << 3,
                  0, offset, 0, texture, mnuCommonIconPlacements[1][BRS_ICON_ID], mode);

    object->pulseFrame++;
    if ((f32)object->pulseFrame >= 120.0f) {
        object->pulseFrame = 0;
    }
}

void mnuDrawStatusIconAndCompanion(s32 x, s32 y, s32 z, ShopScene *context, s32 width, s32 mode) {
    s32 iconIndex;
    struct EffectSlotSet *layer = D_003BC520;

    iconIndex = 0x25;
    if (context->extraOption == 0) {
        iconIndex = 9;
    }
    effDrawSpriteWithCornerFade(x + (mnuCommonIconPlacements[iconIndex][BRS_ICON_X] << 4), y + (mnuCommonIconPlacements[iconIndex][BRS_ICON_Y] << 3), z, width, 0, layer, mnuCommonIconPlacements[iconIndex][BRS_ICON_ID], mode);
    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[2][BRS_ICON_X] << 4, mnuCommonIconPlacements[2][BRS_ICON_Y] << 3, z, width, 0, layer, mnuCommonIconPlacements[2][BRS_ICON_ID], mode);
}

void mnuDrawIconTriple(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c) {
    struct EffectSlotSet *layer = D_003BC520;

    effDrawSpriteWithCornerFade(x + (mnuCommonIconPlacements[18][BRS_ICON_X] << 4), y + (mnuCommonIconPlacements[18][BRS_ICON_Y] << 3), 0, b, 0, layer, mnuCommonIconPlacements[18][BRS_ICON_ID], c);
    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[17][BRS_ICON_X] << 4, mnuCommonIconPlacements[17][BRS_ICON_Y] << 3, 0, b, 0, layer, mnuCommonIconPlacements[17][BRS_ICON_ID], c);
    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[22][BRS_ICON_X] << 4, mnuCommonIconPlacements[22][BRS_ICON_Y] << 3, 0, b, 0, layer, mnuCommonIconPlacements[22][BRS_ICON_ID], c);
}

#include "pty_bullet_item_count_api.h"
extern s32 sprintf(char *, const char *, ...);
extern FrFontGlyph *itfCreateTightFont3TextGlyph(s32, s32, s32, s32, char *, FrFontGlyph *);
extern char D_003BC4F0[];

/* Draw the selected item quantity; bullets include matching party slot values. */
void mnuDrawShopItemQuantity(s32 x, s32 y, s32 depth, ShopScene *scene, s32 alpha, s32 mode) {
    char text[16];
    struct EffectSlotSet *texture = D_003BC520;
    struct MenuList *list;

    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[23][BRS_ICON_X] << 4,
                  mnuCommonIconPlacements[23][BRS_ICON_Y] << 3,
                  0, alpha, 0, texture, mnuCommonIconPlacements[23][BRS_ICON_ID], mode);
    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[24][BRS_ICON_X] << 4,
                  mnuCommonIconPlacements[24][BRS_ICON_Y] << 3,
                  0, alpha, 0, texture, mnuCommonIconPlacements[24][BRS_ICON_ID], mode);
    list = scene->window->list;
    if (list->count != 0) {
        CampWindowParams *item = &list->cursor->camp;
        s32 quantity = datGameState->inventory.counts[item->id];
        u32 style = (s32)((f32)(alpha << 7) * 0.00390625f) | 0xA09DC300;
        FrFontGlyph *glyph;

        if (item->mode == 2) {
            quantity = ptyCountBulletItem(item->id);
        }
        sprintf(text, D_003BC4F0, quantity);
        glyph = itfCreateTightFont3TextGlyph(0x1A70, 0xAA0, depth, style, text, 0);
        frFontDrawGlyphChain(glyph, 1, mode);
        frFontQueueGlyphForCurrentDrawBuffer(glyph);
    }
}

INCLUDE_ASM(const s32, "game/code_0025DF68", mnuDrawMenuItemListRow);

extern FrFontGlyph *itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, FrFontGlyph *);
extern char D_003BC500[];
void mnuDrawCampItemListRow(s32 x, s32 y, s32 z, struct MenuList *list, struct MenuListNode *node, s32 mode) {
    char text[16];
    struct EffectSlotSet *layer = D_003BC520;
    MnuShopListContext *context = list->context;
    s32 first = list->head->index;
    s32 index = node->index;
    s32 row = index - first;
    f32 fade = 1.0f;
    f32 pulse;
    s32 selected = 0;
    u32 color;
    FrFontGlyph *glyph;

    switch (context->mode) {
    case 1: {
        f32 position = (f32)row / (f32)list->visibleCount;
        f32 progress = (f32)context->countdown / 10.0f;

        position = position * 0.5f + 0.5f;
        fade = progress + progress;
        fade *= position;
        if (fade > 1.0f) {
            fade = 1.0f;
        }
        fade = 1.0f - fade;
        break;
    }
    case 2:
        fade = (f32)context->countdown * 0.25f;
        break;
    }
    if (node == list->cursor) {
        context->unk08++;
        if (context->unk08 > 60) {
            context->unk08 = 0;
        }
        pulse = sdfSinPoly((f32)context->unk08 / 60.0f * 3.1415926f) * fade;
        index = list->cursor->index;
        row = index - first;
        effDrawSpriteWithCornerFade(0x3D0, (row * 21 + 0x98) << 3, 0, (u32)(fade * 256.0f), 0, layer, 0x20, mode);
        effDrawSpriteWithCornerFade(0xC90, (row * 21 + 0x98) << 3, 0, (u32)(fade * 256.0f), 0, layer, 0x21, mode);
        effDrawSpriteWithCornerFade(mnuCommonIconPlacements[19][BRS_ICON_X] << 4, (mnuCommonIconPlacements[19][BRS_ICON_Y] + row * 21) << 3, 0,
                                    (u32)(fade * 256.0f), 0, layer, mnuCommonIconPlacements[19][BRS_ICON_ID], mode);
        effDrawSpriteWithCornerFade(mnuCommonIconPlacements[19][BRS_ICON_X] << 4, (mnuCommonIconPlacements[19][BRS_ICON_Y] + row * 21) << 3, 0,
                                    (u32)(pulse * 256.0f), 0, layer, mnuCommonIconPlacements[19][BRS_ICON_ID], mode);
        selected = 1;
    }
    color = 0xA09DC300;
    if (node->flags48 & 1) {
        color |= (s32)(fade * 64.0f);
    } else {
        color |= (s32)(fade * 128.0f);
    }
    glyph = itfCreateConvertedTextGlyph(0x570, (row * 21 + 0x99) << 3, z, color, (const u8 *)node->value, NULL);
    if (selected) {
        frFontSetChildChainFirstOption(glyph, 4);
    }
    if (list->id & 1) {
        sprintf(text, D_003BC500, node->camp.value);
        glyph = itfCreateFont2TextGlyph(0xFF0, (row * 21 + 0x9D) << 3, z, color, text, glyph);
        if (selected) {
            frFontSetChildChainFirstOption(glyph, 4);
        }
    }
    frFontDrawGlyphChain(glyph, 1, mode);
    frFontQueueGlyphForCurrentDrawBuffer(glyph);
}

/* Draw the row label, and draw its numeric value when the list enables it. */
void mnuDrawShopRowTextAndValue(s32 x, s32 y, s32 z, struct MenuList *list,
                   struct MenuListNode *node, s32 drawArg) {
    MnuShopListContext *context = (MnuShopListContext *)list->context;
    s32 row = node->index - list->head->index;
    s32 rowOffset;
    s32 mode = context->mode;
    f32 fade = 0.0f;
    u32 style = 0xA09DC300;
    s32 selected = 0;
    s32 value;
    FrFontGlyph *glyph;
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

    rowOffset = row * 21;
    glyph = itfCreateConvertedTextGlyph(0x570, (rowOffset + 0x99) << 3, z,
                                        style, (const u8 *)node->value, NULL);
    if (selected) {
        frFontSetChildChainFirstOption(glyph, 4);
    }
    frFontDrawGlyphChain(glyph, 1, drawArg);
    frFontQueueGlyphForCurrentDrawBuffer(glyph);

    if ((list->id & 1) == 0) {
        return;
    }

    if (list->cursor == node) {
        style = (s32)(fade * 64.0f + 64.0f) | 0xA09DC300;
    }
    value = (s32)node->sortKeyPrimary;
    sprintf(text, D_003BC500, value);
    glyph = itfCreateFont2TextGlyph(0xFF0, (rowOffset + 0x9D) << 3, z, style, text, NULL);
    if (selected) {
        frFontSetChildChainFirstOption(glyph, 4);
    }
    frFontDrawGlyphChain(glyph, 1, drawArg);
    frFontQueueGlyphForCurrentDrawBuffer(glyph);
}


extern void mnuDrawListChildrenWithCountdown(s32, s32, s32, struct MenuList *, s32);
extern void mnuDrawListFlagIndicatorsWithFade(s32, s32, s32, s32, MenuWindowContainer *, s32);


/* Draw the child and its container only while the child is active. */
void mnuDrawIfActive(s32 x, s32 y, s32 z, MenuWindowContainer *object, s32 drawArg) {
    struct MenuList *inner = object->list;

    if (inner->count != 0) {
        mnuDrawListChildrenWithCountdown(x, y, z, inner, drawArg);
        mnuDrawListFlagIndicatorsWithFade(x, y, z, 0, object, drawArg);
        object->flags |= 4;
    }
}


/* Tick the list's countdown, then run its draw callback on up to `count` linked children. */
void mnuDrawListChildrenWithCountdown(s32 x, s32 y, s32 z, struct MenuList *list, s32 drawArg) {
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

void mnuDrawListFlagIndicatorsWithFade(s32 x, s32 y, s32 z, s32 unused, MenuWindowContainer *object, s32 option) {
    struct MenuList *inner = object->list;
    struct EffectSlotSet *texture = D_003BC520;
    s32 *delay = inner->context;
    s32 mode = delay[1];
    s32 flags = inner->flags;
    f32 alpha = 0.0f;

    switch (mode) {
    case 1:
        alpha = (f32)delay[0] / 15.0f;
        alpha = 1.0f - alpha;
        break;
    case 2:
        alpha = (f32)delay[0] / 15.0f;
        break;
    }
    if (flags & 1) {
        effDrawSpriteWithCornerFade(mnuCommonIconPlacements[20][BRS_ICON_X] << 4, mnuCommonIconPlacements[20][BRS_ICON_Y] << 3,
                      0, (u32)(alpha * 256.0f), 0,
                      texture, mnuCommonIconPlacements[20][BRS_ICON_ID], option);
    }
    if (flags & 2) {
        effDrawSpriteWithCornerFade(mnuCommonIconPlacements[21][BRS_ICON_X] << 4, mnuCommonIconPlacements[21][BRS_ICON_Y] << 3,
                      0, (u32)(alpha * 256.0f), 0,
                      texture, mnuCommonIconPlacements[21][BRS_ICON_ID], option);
    }
}

/* Draw the selected shop row's two markers, its icon and its count text. */
void mnuDrawShopSelectedRowDetails(s32 x, s32 y, s32 depth, ShopScene *scene, s32 option) {
    char text[16];
    struct EffectSlotSet *texture = D_003BC520;
    struct MenuList *list = scene->window->list;
    s32 row;
    s32 firstIndex;
    FrFontGlyph *glyph;

    if (list->count != 0) {
        firstIndex = list->head->index;
        row = list->cursor->index - firstIndex;
        effDrawSpriteWithCornerFade(0x3D0, (152 + row * 21) << 3, 0, 0x80, 0, texture, 0x20, option);
        effDrawSpriteWithCornerFade(0xC90, (152 + row * 21) << 3, 0, 0x80, 0, texture, 0x21, option);
        effDrawSpriteWithCornerFade(mnuCommonIconPlacements[29][BRS_ICON_X] << 4,
                      (mnuCommonIconPlacements[29][BRS_ICON_Y] + row * 21) << 3,
                      0, 0x100, 0, texture,
                      mnuCommonIconPlacements[29][BRS_ICON_ID], option);
        sprintf(text, D_003BC4F0, scene->counter);
        glyph = itfCreateTightFont3TextGlyph(0xDD0, (155 + row * 21) << 3, depth,
                              0xA09DC380, text, 0);
        frFontSetChildChainFirstOption(glyph, 4);
        frFontDrawGlyphChain(glyph, 1, option);
        frFontQueueGlyphForCurrentDrawBuffer(glyph);
    }
}

void mnuDrawShopSelectedRowDetailsWithAlpha(s32 x, s32 y, s32 depth, ShopScene *scene, u32 alpha, s32 option) {
    char text[16];
    struct MenuList *list = scene->window->list;
    struct EffectSlotSet *texture = D_003BC520;
    f32 opacity;
    s32 row;
    s32 firstIndex;
    u32 color;
    FrFontGlyph *glyph;

    if (list->count != 0) {
        firstIndex = list->head->index;
        opacity = (f32)alpha * 0.00390625f;
        row = list->cursor->index - firstIndex;
        effDrawSpriteWithCornerFade(mnuCommonIconPlacements[19][BRS_ICON_X] << 4,
                      (mnuCommonIconPlacements[19][BRS_ICON_Y] + row * 21) << 3,
                      0, (u32)((1.0f - opacity) * 256.0f), 0, texture,
                      mnuCommonIconPlacements[19][BRS_ICON_ID], option);
        effDrawSpriteWithCornerFade(0x3D0, (152 + row * 21) << 3, 0,
                      (u32)((1.0f - opacity) * 128.0f + 128.0f),
                      0, texture, 0x20, option);
        effDrawSpriteWithCornerFade(0xC90, (152 + row * 21) << 3, 0,
                      (u32)((1.0f - opacity) * 128.0f + 128.0f),
                      0, texture, 0x21, option);
        effDrawSpriteWithCornerFade(mnuCommonIconPlacements[29][BRS_ICON_X] << 4,
                      (mnuCommonIconPlacements[29][BRS_ICON_Y] + row * 21) << 3,
                      0, (u32)(opacity * 256.0f), 0, texture,
                      mnuCommonIconPlacements[29][BRS_ICON_ID], option);
        color = (s32)(opacity * 128.0f) | 0xA09DC300;
        sprintf(text, D_003BC4F0, scene->counter);
        glyph = itfCreateTightFont3TextGlyph(0xDD0, (155 + row * 21) << 3, depth,
                              color, text, 0);
        frFontSetChildChainFirstOption(glyph, 4);
        frFontDrawGlyphChain(glyph, 1, option);
        frFontQueueGlyphForCurrentDrawBuffer(glyph);
    }
}

void mnuDrawShopLimitArrows(s32 x, s32 y, s32 z, ShopScene *panel, s32 option) {
    struct EffectSlotSet *texture = D_003BC520;
    s32 firstIndex = panel->window->list->head->index;
    s32 row = panel->window->list->cursor->index - firstIndex;

    if (panel->atLimit != 1) {
        effDrawSpriteWithCornerFade(mnuCommonIconPlacements[30][BRS_ICON_X] << 4, (mnuCommonIconPlacements[30][BRS_ICON_Y] + row * 21) << 3,
            0, 0x100, 0, texture, mnuCommonIconPlacements[30][BRS_ICON_ID], option);
    }
    if (panel->counter != 1) {
        effDrawSpriteWithCornerFade(mnuCommonIconPlacements[31][BRS_ICON_X] << 4, (mnuCommonIconPlacements[31][BRS_ICON_Y] + row * 21) << 3,
            0, 0x100, 0, texture, mnuCommonIconPlacements[31][BRS_ICON_ID], option);
    }
}

void mnuDrawShopLimitArrowsScaled(s32 x, s32 y, s32 z, ShopScene *panel, s32 scale, s32 option) {
    struct EffectSlotSet *texture = D_003BC520;
    s32 firstIndex = panel->window->list->head->index;
    s32 row = panel->window->list->cursor->index - firstIndex;

    if (panel->atLimit != 1) {
        effDrawSpriteWithCornerFade(mnuCommonIconPlacements[30][BRS_ICON_X] << 4, (mnuCommonIconPlacements[30][BRS_ICON_Y] + row * 21) << 3,
            0, scale, 0, texture, mnuCommonIconPlacements[30][BRS_ICON_ID], option);
    }
    if (panel->counter != 1) {
        effDrawSpriteWithCornerFade(mnuCommonIconPlacements[31][BRS_ICON_X] << 4, (mnuCommonIconPlacements[31][BRS_ICON_Y] + row * 21) << 3,
            0, scale, 0, texture, mnuCommonIconPlacements[31][BRS_ICON_ID], option);
    }
}

extern char D_003BC508[];

void mnuDrawShopPricePlaceholder(s32 x, s32 y, s32 depth, ShopScene *panel, s32 option) {
    char text[16];
    struct EffectSlotSet *texture = D_003BC520;
    MenuWindowContainer *object = panel->window;
    struct MenuList *inner;
    FrFontGlyph *glyph;

    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[25][BRS_ICON_X] << 4, mnuCommonIconPlacements[25][BRS_ICON_Y] << 3,
                  0, 0x100, 0, texture, mnuCommonIconPlacements[25][BRS_ICON_ID], option);
    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[26][BRS_ICON_X] << 4, mnuCommonIconPlacements[26][BRS_ICON_Y] << 3,
                  0, 0x100, 0, texture, mnuCommonIconPlacements[26][BRS_ICON_ID], option);
    inner = object->list;
    if (inner->count != 0) {
        sprintf(text, D_003BC508, 0);
        glyph = itfCreateTightFont3TextGlyph(0x17C0, 0x380, depth, 0xA09DC380, text, 0);
        frFontSetChildChainFirstOption(glyph, 4);
        frFontDrawGlyphChain(glyph, 1, option);
        frFontQueueGlyphForCurrentDrawBuffer(glyph);
    }
}

void mnuDrawIconFixedEntry(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c) {
    effDrawSpriteWithCornerFade(x + (mnuCommonIconPlacements[26][BRS_ICON_X] << 4), y + (mnuCommonIconPlacements[26][BRS_ICON_Y] << 3), z, b, 0, D_003BC520, mnuCommonIconPlacements[26][BRS_ICON_ID], c);
}
void mnuDrawIconFixedEntryWithBadge(s32 x, s32 y, s32 z, s32 unused, s32 scale, s32 option) {
    char text[16];
    s32 value;
    FrFontGlyph *glyph;

    effDrawSpriteWithCornerFade(x + (mnuCommonIconPlacements[25][BRS_ICON_X] << 4), y + (mnuCommonIconPlacements[25][BRS_ICON_Y] << 3), z, scale, 0, D_003BC520, mnuCommonIconPlacements[25][BRS_ICON_ID], option);
    value = (s32)((f32)(scale << 7) * 0.00390625f) | 0xA09DC300;
    sprintf(text, D_003BC508, 0);
    glyph = itfCreateTightFont3TextGlyph(x + 0x17C0, y + 0x380, z, value, text, 0);
    frFontSetChildChainFirstOption(glyph, 4);
    frFontDrawGlyphChain(glyph, 1, option);
    frFontQueueGlyphForCurrentDrawBuffer(glyph);
}

void mnuDrawShopSelectedRowTotal(s32 x, s32 y, s32 depth, ShopScene *panel, s32 option) {
    char text[16];
    struct EffectSlotSet *texture = D_003BC520;
    MenuWindowContainer *object = panel->window;
    struct MenuList *inner;
    FrFontGlyph *glyph;

    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[25][BRS_ICON_X] << 4, mnuCommonIconPlacements[25][BRS_ICON_Y] << 3,
                  0, 0x100, 0, texture, mnuCommonIconPlacements[25][BRS_ICON_ID], option);
    effDrawSpriteWithCornerFade(mnuCommonIconPlacements[26][BRS_ICON_X] << 4, mnuCommonIconPlacements[26][BRS_ICON_Y] << 3,
                  0, 0x100, 0, texture, mnuCommonIconPlacements[26][BRS_ICON_ID], option);
    inner = object->list;
    if (inner->count != 0) {
        sprintf(text, D_003BC508, inner->cursor->camp.value * panel->counter);
        glyph = itfCreateTightFont3TextGlyph(0x17C0, 0x380, depth, 0xA09DC380, text, 0);
        frFontSetChildChainFirstOption(glyph, 4);
        frFontDrawGlyphChain(glyph, 1, option);
        frFontQueueGlyphForCurrentDrawBuffer(glyph);
    }
}


void mnuDrawShopCurrencyBalance(ShopScene *state, s32 style) {
    char text[16];

    if (datGameState->header.currency != state->previousValue) {
        s32 transitionFrames = 20;
        s32 displayedValue;

        sndSetSequenceVolumePan(19, 127, 63);
        state->elapsedFrames++;
        displayedValue = state->previousValue +
                         ((datGameState->header.currency - state->previousValue) * state->elapsedFrames) /
                             transitionFrames;
        sprintf(text, D_003BC508, displayedValue);
        if (state->elapsedFrames == transitionFrames) {
            state->previousValue = datGameState->header.currency;
            state->elapsedFrames = 0;
        }
    } else {
        sprintf(text, D_003BC508, datGameState->header.currency);
    }

    {
        FrFontGlyph *glyph = itfCreateTightFont3TextGlyph(0x17C0, 0x2B8, 0, style, text, 0);

        frFontDrawGlyphChain(glyph, 1, 0x53);
        frFontQueueGlyphForCurrentDrawBuffer(glyph);
    }
}

INCLUDE_SDATA(const s32, "game/code_0025DF68", D_003BC4F0);

INCLUDE_SDATA(const s32, "game/code_0025DF68", D_003BC4F8);

INCLUDE_SDATA(const s32, "game/code_0025DF68", D_003BC500);

INCLUDE_SDATA(const s32, "game/code_0025DF68", D_003BC508);

