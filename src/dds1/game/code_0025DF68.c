#include "common.h"
#include "dat_state.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "itf.h"


typedef s16 BrsIconRecord[4];

enum {
    BRS_ICON_ID = 1,
    BRS_ICON_X = 2,
    BRS_ICON_Y = 3,
};

extern BrsIconRecord D_0036C728[];
extern u32 D_003BC520;
extern void func_002BF4E0(s32, s32, s32, s32, s32, u32, s32, s32);

/* Fade the fixed dispatch frame, then its five-tick-delayed foreground icon. */
void func_0025DF68(s32 contextAddress, s32 progress) {
    u32 texture = D_003BC520;
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
    func_002BF4E0(D_0036C728[32][BRS_ICON_X] << 4,
                  D_0036C728[32][BRS_ICON_Y] << 3,
                  0, scale, 0, texture, D_0036C728[32][BRS_ICON_ID], 0x53);
    func_002BF4E0(D_0036C728[27][BRS_ICON_X] << 4,
                  D_0036C728[27][BRS_ICON_Y] << 3,
                  0, scale, 0, texture, D_0036C728[27][BRS_ICON_ID], 0x53);
    func_002BF4E0(D_0036C728[28][BRS_ICON_X] << 4,
                  D_0036C728[28][BRS_ICON_Y] << 3,
                  0, scale, 0, texture, D_0036C728[28][BRS_ICON_ID], 0x53);
    ticks = progress - 5;
    if (ticks < 0) {
        ticks = 0;
    }
    fraction = (f32)ticks / 15.0f;
    if (fraction > 1.0f) {
        fraction = 1.0f;
    }
    scale = (s32)(fraction * 256.0f);
    func_002BF4E0(D_0036C728[0][BRS_ICON_X] << 4,
                  D_0036C728[0][BRS_ICON_Y] << 3,
                  0, scale, 0, texture, D_0036C728[0][BRS_ICON_ID], 0x53);
}

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025E108);

extern void func_0025E420(ShopScene *, s32, s32);

/* Draw the fixed shop frame, then its pulsing icon. */
void func_0025E308(s32 x, s32 y, s32 z, ShopScene *scene, s32 alpha, s32 mode) {
    u32 texture = D_003BC520;

    func_002BF4E0(D_0036C728[32][BRS_ICON_X] << 4,
                  D_0036C728[32][BRS_ICON_Y] << 3,
                  0, alpha, 0, texture, D_0036C728[32][BRS_ICON_ID], mode);
    func_002BF4E0(D_0036C728[27][BRS_ICON_X] << 4,
                  D_0036C728[27][BRS_ICON_Y] << 3,
                  0, alpha, 0, texture, D_0036C728[27][BRS_ICON_ID], mode);
    func_002BF4E0(D_0036C728[28][BRS_ICON_X] << 4,
                  D_0036C728[28][BRS_ICON_Y] << 3,
                  0, alpha, 0, texture, D_0036C728[28][BRS_ICON_ID], mode);
    func_002BF4E0(D_0036C728[0][BRS_ICON_X] << 4,
                  D_0036C728[0][BRS_ICON_Y] << 3,
                  0, alpha, 0, texture, D_0036C728[0][BRS_ICON_ID], mode);
    func_0025E420(scene, 0x100, mode);
}

extern f32 sdfSinPoly(f32);

void func_0025E420(ShopScene *object, s32 scale, s32 mode) {
    u32 texture = D_003BC520;
    s8 phase = object->pulseFrame;
    f32 angle = (f32)phase / 120.0f * 6.2831852f;
    s32 offset = (s32)((f32)scale * sdfSinPoly(angle));

    func_002BF4E0(D_0036C728[1][BRS_ICON_X] << 4,
                  D_0036C728[1][BRS_ICON_Y] << 3,
                  0, offset, 0, texture, D_0036C728[1][BRS_ICON_ID], mode);

    object->pulseFrame++;
    if ((f32)object->pulseFrame >= 120.0f) {
        object->pulseFrame = 0;
    }
}

void mnuDrawStatusIconAndCompanion(s32 x, s32 y, s32 z, ShopScene *context, s32 width, s32 mode) {
    s32 iconIndex;
    u32 layer = D_003BC520;

    iconIndex = 0x25;
    if (context->extraOption == 0) {
        iconIndex = 9;
    }
    func_002BF4E0(x + (D_0036C728[iconIndex][BRS_ICON_X] << 4), y + (D_0036C728[iconIndex][BRS_ICON_Y] << 3), z, width, 0, layer, D_0036C728[iconIndex][BRS_ICON_ID], mode);
    func_002BF4E0(D_0036C728[2][BRS_ICON_X] << 4, D_0036C728[2][BRS_ICON_Y] << 3, z, width, 0, layer, D_0036C728[2][BRS_ICON_ID], mode);
}

void mnuDrawIconTriple(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c) {
    u32 layer = D_003BC520;

    func_002BF4E0(x + (D_0036C728[18][BRS_ICON_X] << 4), y + (D_0036C728[18][BRS_ICON_Y] << 3), 0, b, 0, layer, D_0036C728[18][BRS_ICON_ID], c);
    func_002BF4E0(D_0036C728[17][BRS_ICON_X] << 4, D_0036C728[17][BRS_ICON_Y] << 3, 0, b, 0, layer, D_0036C728[17][BRS_ICON_ID], c);
    func_002BF4E0(D_0036C728[22][BRS_ICON_X] << 4, D_0036C728[22][BRS_ICON_Y] << 3, 0, b, 0, layer, D_0036C728[22][BRS_ICON_ID], c);
}

extern s32 ptyCountBulletItem(s32);
extern s32 func_003014F0(char *, const char *, ...);
extern FrFontGlyph *func_00197A98(s32, s32, s32, s32, char *, FrFontGlyph *);
extern s32 func_001958A0(FrFontGlyph *, s8, u32);
extern s32 frFontQueueGlyphInSelectedSlot(FrFontGlyph *);
extern char D_003BC4F0[];

/* Draw the selected item quantity; bullets include matching party slot values. */
void func_0025E6B0(s32 x, s32 y, s32 depth, ShopScene *scene, s32 alpha, s32 mode) {
    char text[16];
    u32 texture = D_003BC520;
    struct MenuList *list;

    func_002BF4E0(D_0036C728[23][BRS_ICON_X] << 4,
                  D_0036C728[23][BRS_ICON_Y] << 3,
                  0, alpha, 0, texture, D_0036C728[23][BRS_ICON_ID], mode);
    func_002BF4E0(D_0036C728[24][BRS_ICON_X] << 4,
                  D_0036C728[24][BRS_ICON_Y] << 3,
                  0, alpha, 0, texture, D_0036C728[24][BRS_ICON_ID], mode);
    list = scene->window->list;
    if (list->count != 0) {
        CampWindowParams *item = &list->cursor->camp;
        s32 quantity = datGameState->inventory.counts[item->id];
        u32 style = (s32)((f32)(alpha << 7) * 0.00390625f) | 0xA09DC300;
        FrFontGlyph *glyph;

        if (item->mode == 2) {
            quantity = ptyCountBulletItem(item->id);
        }
        func_003014F0(text, D_003BC4F0, quantity);
        glyph = func_00197A98(0x1A70, 0xAA0, depth, style, text, 0);
        func_001958A0(glyph, 1, mode);
        frFontQueueGlyphInSelectedSlot(glyph);
    }
}

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025E820);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025ECD0);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025F138);

extern void mnuDrawListChildrenWithCountdown(s32, s32, s32, struct MenuList *, s32);
extern void func_0025F4E0(s32, s32, s32, s32, MenuWindowContainer *, s32);


/* Draw the child and its container only while the child is active. */
void mnuDrawIfActive(s32 x, s32 y, s32 z, MenuWindowContainer *object, s32 drawArg) {
    struct MenuList *inner = object->list;

    if (inner->count != 0) {
        mnuDrawListChildrenWithCountdown(x, y, z, inner, drawArg);
        func_0025F4E0(x, y, z, 0, object, drawArg);
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

void func_0025F4E0(s32 x, s32 y, s32 z, s32 unused, MenuWindowContainer *object, s32 option) {
    struct MenuList *inner = object->list;
    u32 texture = D_003BC520;
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
        func_002BF4E0(D_0036C728[20][BRS_ICON_X] << 4, D_0036C728[20][BRS_ICON_Y] << 3,
                      0, (u32)(alpha * 256.0f), 0,
                      texture, D_0036C728[20][BRS_ICON_ID], option);
    }
    if (flags & 2) {
        func_002BF4E0(D_0036C728[21][BRS_ICON_X] << 4, D_0036C728[21][BRS_ICON_Y] << 3,
                      0, (u32)(alpha * 256.0f), 0,
                      texture, D_0036C728[21][BRS_ICON_ID], option);
    }
}

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025F680);

extern void frFontSetChainFlag(FrFontGlyph *, u8);

void func_0025F7F0(s32 x, s32 y, s32 depth, ShopScene *scene, u32 alpha, s32 option) {
    char text[16];
    struct MenuList *list = scene->window->list;
    u32 texture = D_003BC520;
    f32 opacity;
    s32 row;
    s32 firstIndex;
    u32 color;
    FrFontGlyph *glyph;

    if (list->count != 0) {
        firstIndex = list->head->index;
        opacity = (f32)alpha * 0.00390625f;
        row = list->cursor->index - firstIndex;
        func_002BF4E0(D_0036C728[19][BRS_ICON_X] << 4,
                      (D_0036C728[19][BRS_ICON_Y] + row * 21) << 3,
                      0, (u32)((1.0f - opacity) * 256.0f), 0, texture,
                      D_0036C728[19][BRS_ICON_ID], option);
        func_002BF4E0(0x3D0, (152 + row * 21) << 3, 0,
                      (u32)((1.0f - opacity) * 128.0f + 128.0f),
                      0, texture, 0x20, option);
        func_002BF4E0(0xC90, (152 + row * 21) << 3, 0,
                      (u32)((1.0f - opacity) * 128.0f + 128.0f),
                      0, texture, 0x21, option);
        func_002BF4E0(D_0036C728[29][BRS_ICON_X] << 4,
                      (D_0036C728[29][BRS_ICON_Y] + row * 21) << 3,
                      0, (u32)(opacity * 256.0f), 0, texture,
                      D_0036C728[29][BRS_ICON_ID], option);
        color = (s32)(opacity * 128.0f) | 0xA09DC300;
        func_003014F0(text, D_003BC4F0, scene->counter);
        glyph = func_00197A98(0xDD0, (155 + row * 21) << 3, depth,
                              color, text, 0);
        frFontSetChainFlag(glyph, 4);
        func_001958A0(glyph, 1, option);
        frFontQueueGlyphInSelectedSlot(glyph);
    }
}

void func_0025FB30(s32 x, s32 y, s32 z, ShopScene *panel, s32 option) {
    u32 texture = D_003BC520;
    s32 firstIndex = panel->window->list->head->index;
    s32 row = panel->window->list->cursor->index - firstIndex;

    if (panel->atLimit != 1) {
        func_002BF4E0(D_0036C728[30][BRS_ICON_X] << 4, (D_0036C728[30][BRS_ICON_Y] + row * 21) << 3,
            0, 0x100, 0, texture, D_0036C728[30][BRS_ICON_ID], option);
    }
    if (panel->counter != 1) {
        func_002BF4E0(D_0036C728[31][BRS_ICON_X] << 4, (D_0036C728[31][BRS_ICON_Y] + row * 21) << 3,
            0, 0x100, 0, texture, D_0036C728[31][BRS_ICON_ID], option);
    }
}

void func_0025FC38(s32 x, s32 y, s32 z, ShopScene *panel, s32 scale, s32 option) {
    u32 texture = D_003BC520;
    s32 firstIndex = panel->window->list->head->index;
    s32 row = panel->window->list->cursor->index - firstIndex;

    if (panel->atLimit != 1) {
        func_002BF4E0(D_0036C728[30][BRS_ICON_X] << 4, (D_0036C728[30][BRS_ICON_Y] + row * 21) << 3,
            0, scale, 0, texture, D_0036C728[30][BRS_ICON_ID], option);
    }
    if (panel->counter != 1) {
        func_002BF4E0(D_0036C728[31][BRS_ICON_X] << 4, (D_0036C728[31][BRS_ICON_Y] + row * 21) << 3,
            0, scale, 0, texture, D_0036C728[31][BRS_ICON_ID], option);
    }
}

extern char D_003BC508[];

void func_0025FD50(s32 x, s32 y, s32 depth, ShopScene *panel, s32 option) {
    char text[16];
    u32 texture = D_003BC520;
    MenuWindowContainer *object = panel->window;
    struct MenuList *inner;
    FrFontGlyph *glyph;

    func_002BF4E0(D_0036C728[25][BRS_ICON_X] << 4, D_0036C728[25][BRS_ICON_Y] << 3,
                  0, 0x100, 0, texture, D_0036C728[25][BRS_ICON_ID], option);
    func_002BF4E0(D_0036C728[26][BRS_ICON_X] << 4, D_0036C728[26][BRS_ICON_Y] << 3,
                  0, 0x100, 0, texture, D_0036C728[26][BRS_ICON_ID], option);
    inner = object->list;
    if (inner->count != 0) {
        func_003014F0(text, D_003BC508, 0);
        glyph = func_00197A98(0x17C0, 0x380, depth, 0xA09DC380, text, 0);
        frFontSetChainFlag(glyph, 4);
        func_001958A0(glyph, 1, option);
        frFontQueueGlyphInSelectedSlot(glyph);
    }
}

void mnuDrawIconFixedEntry(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c) {
    func_002BF4E0(x + (D_0036C728[26][BRS_ICON_X] << 4), y + (D_0036C728[26][BRS_ICON_Y] << 3), z, b, 0, D_003BC520, D_0036C728[26][BRS_ICON_ID], c);
}

extern void sndSetSequenceVolumePan(s32, s32, s32);

void mnuDrawIconFixedEntryWithBadge(s32 x, s32 y, s32 z, s32 unused, s32 scale, s32 option) {
    char text[16];
    s32 value;
    FrFontGlyph *glyph;

    func_002BF4E0(x + (D_0036C728[25][BRS_ICON_X] << 4), y + (D_0036C728[25][BRS_ICON_Y] << 3), z, scale, 0, D_003BC520, D_0036C728[25][BRS_ICON_ID], option);
    value = (s32)((f32)(scale << 7) * 0.00390625f) | 0xA09DC300;
    func_003014F0(text, D_003BC508, 0);
    glyph = func_00197A98(x + 0x17C0, y + 0x380, z, value, text, 0);
    frFontSetChainFlag(glyph, 4);
    func_001958A0(glyph, 1, option);
    frFontQueueGlyphInSelectedSlot(glyph);
}

void func_0025FFC8(s32 x, s32 y, s32 depth, ShopScene *panel, s32 option) {
    char text[16];
    u32 texture = D_003BC520;
    MenuWindowContainer *object = panel->window;
    struct MenuList *inner;
    FrFontGlyph *glyph;

    func_002BF4E0(D_0036C728[25][BRS_ICON_X] << 4, D_0036C728[25][BRS_ICON_Y] << 3,
                  0, 0x100, 0, texture, D_0036C728[25][BRS_ICON_ID], option);
    func_002BF4E0(D_0036C728[26][BRS_ICON_X] << 4, D_0036C728[26][BRS_ICON_Y] << 3,
                  0, 0x100, 0, texture, D_0036C728[26][BRS_ICON_ID], option);
    inner = object->list;
    if (inner->count != 0) {
        func_003014F0(text, D_003BC508, inner->cursor->camp.value * panel->counter);
        glyph = func_00197A98(0x17C0, 0x380, depth, 0xA09DC380, text, 0);
        frFontSetChainFlag(glyph, 4);
        func_001958A0(glyph, 1, option);
        frFontQueueGlyphInSelectedSlot(glyph);
    }
}


void func_00260100(ShopScene *state, s32 style) {
    char text[16];

    if (datGameState->header.currency != state->previousValue) {
        s32 transitionFrames = 20;
        s32 displayedValue;

        sndSetSequenceVolumePan(19, 127, 63);
        state->elapsedFrames++;
        displayedValue = state->previousValue +
                         ((datGameState->header.currency - state->previousValue) * state->elapsedFrames) /
                             transitionFrames;
        func_003014F0(text, D_003BC508, displayedValue);
        if (state->elapsedFrames == transitionFrames) {
            state->previousValue = datGameState->header.currency;
            state->elapsedFrames = 0;
        }
    } else {
        func_003014F0(text, D_003BC508, datGameState->header.currency);
    }

    {
        FrFontGlyph *glyph = func_00197A98(0x17C0, 0x2B8, 0, style, text, 0);

        func_001958A0(glyph, 1, 0x53);
        frFontQueueGlyphInSelectedSlot(glyph);
    }
}

INCLUDE_SDATA(const s32, "game/code_0025DF68", D_003BC4F0);

INCLUDE_SDATA(const s32, "game/code_0025DF68", D_003BC4F8);

INCLUDE_SDATA(const s32, "game/code_0025DF68", D_003BC500);

INCLUDE_SDATA(const s32, "game/code_0025DF68", D_003BC508);

