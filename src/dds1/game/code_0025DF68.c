#include "common.h"

typedef struct IconEntry {
    s16 pad0;
    s16 id;
    s16 x;
    s16 y;
} IconEntry;

extern IconEntry D_0036C728[];
extern u32 D_003BC520;
extern s32 func_002BF4E0(s32, s32, s32, s32, s32, u32, s32, s32);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025DF68);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025E108);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025E308);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025E420);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025E508);

void mnuDrawIconTriple(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c) {
    u32 layer = D_003BC520;

    func_002BF4E0(x + (D_0036C728[18].x << 4), y + (D_0036C728[18].y << 3), 0, b, 0, layer, D_0036C728[18].id, c);
    func_002BF4E0(D_0036C728[17].x << 4, D_0036C728[17].y << 3, 0, b, 0, layer, D_0036C728[17].id, c);
    func_002BF4E0(D_0036C728[22].x << 4, D_0036C728[22].y << 3, 0, b, 0, layer, D_0036C728[22].id, c);
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
    u8 pad00[0x18];
    MenuDrawValueItem *first;
    MenuDrawValueItem *item;
    s32 active;
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

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025F4E0);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025F680);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025F7F0);

void func_0025FB30(s32 x, s32 y, s32 z, MenuDrawValuePanel *panel, s32 option) {
    u32 texture = D_003BC520;
    s32 firstIndex = panel->object->inner->first->index;
    s32 row = panel->object->inner->item->index - firstIndex;

    if (panel->unkB3 != 1) {
        func_002BF4E0(D_0036C728[30].x << 4, (D_0036C728[30].y + row * 21) << 3,
            0, 0x100, 0, texture, D_0036C728[30].id, option);
    }
    if (panel->multiplier != 1) {
        func_002BF4E0(D_0036C728[31].x << 4, (D_0036C728[31].y + row * 21) << 3,
            0, 0x100, 0, texture, D_0036C728[31].id, option);
    }
}

void func_0025FC38(s32 x, s32 y, s32 z, MenuDrawValuePanel *panel, s32 scale, s32 option) {
    u32 texture = D_003BC520;
    s32 firstIndex = panel->object->inner->first->index;
    s32 row = panel->object->inner->item->index - firstIndex;

    if (panel->unkB3 != 1) {
        func_002BF4E0(D_0036C728[30].x << 4, (D_0036C728[30].y + row * 21) << 3,
            0, scale, 0, texture, D_0036C728[30].id, option);
    }
    if (panel->multiplier != 1) {
        func_002BF4E0(D_0036C728[31].x << 4, (D_0036C728[31].y + row * 21) << 3,
            0, scale, 0, texture, D_0036C728[31].id, option);
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

    func_002BF4E0(D_0036C728[25].x << 4, D_0036C728[25].y << 3,
                  0, 0x100, 0, texture, D_0036C728[25].id, option);
    func_002BF4E0(D_0036C728[26].x << 4, D_0036C728[26].y << 3,
                  0, 0x100, 0, texture, D_0036C728[26].id, option);
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
    func_002BF4E0(x + (D_0036C728[26].x << 4), y + (D_0036C728[26].y << 3), z, b, 0, D_003BC520, D_0036C728[26].id, c);
}

extern s32 datGameState;
extern void sndSetSequenceVolumePan(s32, s32, s32);

void mnuDrawIconFixedEntryWithBadge(s32 x, s32 y, s32 z, s32 unused, s32 scale, s32 option) {
    char text[16];
    s32 value;
    s32 glyph;

    func_002BF4E0(x + (D_0036C728[25].x << 4), y + (D_0036C728[25].y << 3), z, scale, 0, D_003BC520, D_0036C728[25].id, option);
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

    func_002BF4E0(D_0036C728[25].x << 4, D_0036C728[25].y << 3,
                  0, 0x100, 0, texture, D_0036C728[25].id, option);
    func_002BF4E0(D_0036C728[26].x << 4, D_0036C728[26].y << 3,
                  0, 0x100, 0, texture, D_0036C728[26].id, option);
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

    if (*(s32 *)(datGameState + 0x3C) != state->previousValue) {
        s32 transitionFrames = 20;
        s32 displayedValue;

        sndSetSequenceVolumePan(19, 127, 63);
        state->elapsedFrames++;
        displayedValue = state->previousValue +
                         ((*(s32 *)(datGameState + 0x3C) - state->previousValue) * state->elapsedFrames) /
                             transitionFrames;
        func_003014F0(text, D_003BC508, displayedValue);
        if (state->elapsedFrames == transitionFrames) {
            state->previousValue = *(s32 *)(datGameState + 0x3C);
            state->elapsedFrames = 0;
        }
    } else {
        func_003014F0(text, D_003BC508, *(s32 *)(datGameState + 0x3C));
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
