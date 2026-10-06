#include "common.h"
#include "dat_state.h"

typedef s16 BrsIconRecord[4];

enum {
    BRS_ICON_ID = 1,
    BRS_ICON_X = 2,
    BRS_ICON_Y = 3,
};

extern BrsIconRecord D_0036C728[];
extern u32 D_003BC520;
extern void func_002BF4E0(s32, s32, s32, s32, s32, u32, s32, s32);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025DF68);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025E108);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025E308);

extern f32 sdfSinPoly(f32);

void func_0025E420(u8 *object, s32 scale, s32 mode) {
    u32 texture = D_003BC520;
    s8 phase = (s8)object[0xB2];
    f32 angle = (f32)phase / 120.0f * 6.2831852f;
    s32 offset = (s32)((f32)scale * sdfSinPoly(angle));

    func_002BF4E0(D_0036C728[1][BRS_ICON_X] << 4,
                  D_0036C728[1][BRS_ICON_Y] << 3,
                  0, offset, 0, texture, D_0036C728[1][BRS_ICON_ID], mode);

    object[0xB2]++;
    if ((f32)(s8)object[0xB2] >= 120.0f) {
        object[0xB2] = 0;
    }
}

void mnuDrawStatusIconAndCompanion(s32 x, s32 y, s32 z, void *context, s32 width, s32 mode) {
    s32 iconIndex;
    u32 layer = D_003BC520;

    iconIndex = 0x25;
    if (*(s16 *)((u8 *)context + 0x90) == 0) {
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

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025E6B0);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025E820);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025ECD0);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025F138);

extern void mnuDrawListChildrenWithCountdown(s32, s32, s32, u8 *, s32);
extern void func_0025F4E0(s32, s32, s32, s32, u8 *, s32);

typedef struct MenuDrawValueItem {
    s32 index;
    u8 pad04[0x5C];
    s32 value;
} MenuDrawValueItem;

typedef struct MenuDrawInner {
    u8 pad00[4];
    s32 flags;
    u8 pad08[0x10];
    MenuDrawValueItem *first;
    MenuDrawValueItem *item;
    s32 active;
    u8 pad24[0xC];
    s32 *delay;
} MenuDrawInner;

typedef struct MenuDrawObject {
    u8 pad00[4];
    u32 flags;
    u8 pad08[0xC];
    MenuDrawInner *inner; /* 0x14 */
} MenuDrawObject;

typedef struct MenuDrawValuePanel {
    u8 pad00[0x70];
    MenuDrawObject *object;
    u8 pad74[0xC];
    s32 multiplier;
    u8 pad84[0x2F];
    s8 unkB3;
} MenuDrawValuePanel;

/* Draw the child and its container only while the child is active. */
void mnuDrawIfActive(s32 x, s32 y, s32 z, MenuDrawObject *object, s32 drawArg) {
    MenuDrawInner *inner = object->inner;

    if (inner->active != 0) {
        mnuDrawListChildrenWithCountdown(x, y, z, (u8 *)inner, drawArg);
        func_0025F4E0(x, y, z, 0, (u8 *)object, drawArg);
        object->flags |= 4;
    }
}

typedef struct MenuChild {
    u8 pad00[0x58];
    struct MenuChild *next; /* 0x58 */
} MenuChild;

typedef struct MenuDrawList {
    u8 pad00[0xC];
    s32 count;                 /* 0x0C */
    u8 pad10[8];
    MenuChild *first;          /* 0x18 */
    u8 pad1C[0x10];
    void (*draw)(s32, s32, s32, struct MenuDrawList *, MenuChild *, s32); /* 0x2C */
    s32 *delay;                /* 0x30: countdown ticking once per draw */
} MenuDrawList;

/* Tick the list's countdown, then run its draw callback on up to `count` linked children. */
void mnuDrawListChildrenWithCountdown(s32 x, s32 y, s32 z, u8 *object, s32 drawArg) {
    MenuDrawList *list = (MenuDrawList *)object;
    MenuChild *child;
    s32 i;

    if (list->delay != NULL) {
        if (*list->delay != 0) {
            *list->delay = *list->delay - 1;
        }
    }
    i = 0;
    child = list->first;
    while (i < list->count && child != NULL) {
        if (list->draw != NULL) {
            list->draw(x, y, z, list, child, drawArg);
        }
        i++;
        child = child->next;
    }
}

void func_0025F4E0(s32 x, s32 y, s32 z, s32 unused, u8 *objectData, s32 option) {
    MenuDrawObject *object = (MenuDrawObject *)objectData;
    MenuDrawInner *inner = object->inner;
    u32 texture = D_003BC520;
    s32 mode = inner->delay[1];
    s32 flags = inner->flags;
    f32 alpha = 0.0f;

    switch (mode) {
    case 1:
        alpha = (f32)inner->delay[0] / 15.0f;
        alpha = 1.0f - alpha;
        break;
    case 2:
        alpha = (f32)inner->delay[0] / 15.0f;
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

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025F7F0);

void func_0025FB30(s32 x, s32 y, s32 z, MenuDrawValuePanel *panel, s32 option) {
    u32 texture = D_003BC520;
    s32 firstIndex = panel->object->inner->first->index;
    s32 row = panel->object->inner->item->index - firstIndex;

    if (panel->unkB3 != 1) {
        func_002BF4E0(D_0036C728[30][BRS_ICON_X] << 4, (D_0036C728[30][BRS_ICON_Y] + row * 21) << 3,
            0, 0x100, 0, texture, D_0036C728[30][BRS_ICON_ID], option);
    }
    if (panel->multiplier != 1) {
        func_002BF4E0(D_0036C728[31][BRS_ICON_X] << 4, (D_0036C728[31][BRS_ICON_Y] + row * 21) << 3,
            0, 0x100, 0, texture, D_0036C728[31][BRS_ICON_ID], option);
    }
}

void func_0025FC38(s32 x, s32 y, s32 z, MenuDrawValuePanel *panel, s32 scale, s32 option) {
    u32 texture = D_003BC520;
    s32 firstIndex = panel->object->inner->first->index;
    s32 row = panel->object->inner->item->index - firstIndex;

    if (panel->unkB3 != 1) {
        func_002BF4E0(D_0036C728[30][BRS_ICON_X] << 4, (D_0036C728[30][BRS_ICON_Y] + row * 21) << 3,
            0, scale, 0, texture, D_0036C728[30][BRS_ICON_ID], option);
    }
    if (panel->multiplier != 1) {
        func_002BF4E0(D_0036C728[31][BRS_ICON_X] << 4, (D_0036C728[31][BRS_ICON_Y] + row * 21) << 3,
            0, scale, 0, texture, D_0036C728[31][BRS_ICON_ID], option);
    }
}

extern s32 func_003014F0(char *, const char *, ...);
extern s32 func_00197A98(s32, s32, s32, s32, char *, s32);
extern void frFontSetChainFlag(s32, u8);
extern void func_001958A0(s32, s32, s32);
extern void frFontQueueGlyphInSelectedSlot(s32);
extern char D_003BC508[];

void func_0025FD50(s32 x, s32 y, s32 depth, MenuDrawValuePanel *panel, s32 option) {
    char text[16];
    u32 texture = D_003BC520;
    MenuDrawObject *object = panel->object;
    MenuDrawInner *inner;
    s32 glyph;

    func_002BF4E0(D_0036C728[25][BRS_ICON_X] << 4, D_0036C728[25][BRS_ICON_Y] << 3,
                  0, 0x100, 0, texture, D_0036C728[25][BRS_ICON_ID], option);
    func_002BF4E0(D_0036C728[26][BRS_ICON_X] << 4, D_0036C728[26][BRS_ICON_Y] << 3,
                  0, 0x100, 0, texture, D_0036C728[26][BRS_ICON_ID], option);
    inner = object->inner;
    if (inner->active != 0) {
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
    s32 glyph;

    func_002BF4E0(x + (D_0036C728[25][BRS_ICON_X] << 4), y + (D_0036C728[25][BRS_ICON_Y] << 3), z, scale, 0, D_003BC520, D_0036C728[25][BRS_ICON_ID], option);
    value = (s32)((f32)(scale << 7) * 0.00390625f) | 0xA09DC300;
    func_003014F0(text, D_003BC508, 0);
    glyph = func_00197A98(x + 0x17C0, y + 0x380, z, value, text, 0);
    frFontSetChainFlag(glyph, 4);
    func_001958A0(glyph, 1, option);
    frFontQueueGlyphInSelectedSlot(glyph);
}

void func_0025FFC8(s32 x, s32 y, s32 depth, MenuDrawValuePanel *panel, s32 option) {
    char text[16];
    u32 texture = D_003BC520;
    MenuDrawObject *object = panel->object;
    MenuDrawInner *inner;
    s32 glyph;

    func_002BF4E0(D_0036C728[25][BRS_ICON_X] << 4, D_0036C728[25][BRS_ICON_Y] << 3,
                  0, 0x100, 0, texture, D_0036C728[25][BRS_ICON_ID], option);
    func_002BF4E0(D_0036C728[26][BRS_ICON_X] << 4, D_0036C728[26][BRS_ICON_Y] << 3,
                  0, 0x100, 0, texture, D_0036C728[26][BRS_ICON_ID], option);
    inner = object->inner;
    if (inner->active != 0) {
        func_003014F0(text, D_003BC508, inner->item->value * panel->multiplier);
        glyph = func_00197A98(0x17C0, 0x380, depth, 0xA09DC380, text, 0);
        frFontSetChainFlag(glyph, 4);
        func_001958A0(glyph, 1, option);
        frFontQueueGlyphInSelectedSlot(glyph);
    }
}

typedef struct MenuValueTransitionState {
    u8 pad00[0x9C];
    s32 previousValue;
    s32 elapsedFrames;
} MenuValueTransitionState;

void func_00260100(MenuValueTransitionState *state, s32 style) {
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
        s32 glyph = func_00197A98(0x17C0, 0x2B8, 0, style, text, 0);

        func_001958A0(glyph, 1, 0x53);
        frFontQueueGlyphInSelectedSlot(glyph);
    }
}

INCLUDE_SDATA(const s32, "game/code_0025DF68", D_003BC4F0);

INCLUDE_SDATA(const s32, "game/code_0025DF68", D_003BC4F8);

INCLUDE_SDATA(const s32, "game/code_0025DF68", D_003BC500);

INCLUDE_SDATA(const s32, "game/code_0025DF68", D_003BC508);

