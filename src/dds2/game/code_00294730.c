#include "common.h"

extern void func_00295D38();

extern void func_002958B0();

extern void mnuDrawListChildrenWithCountdown(s32, s32, s32, u8 *, s32);

extern void func_002960F0(s32, s32, s32, s32, u8 *, s32);

typedef struct EventSpriteObject {
    u8 pad00[8];
    s32 type;
} EventSpriteObject;

u32 evtSelectGraphicSlotBySpriteType(EventSpriteObject *object) {
    u32 result;

    result = 0;
    if ((object->type == 1) || (object->type == 3)) {
        result = 0x3a;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00294730", func_00294758);

INCLUDE_ASM(const s32, "game/code_00294730", func_00294930);

typedef s16 MenuIconPlacement[4];

enum {
    MENU_ICON_TEXTURE,
    MENU_ICON_FRAME,
    MENU_ICON_X,
    MENU_ICON_Y
};

extern MenuIconPlacement D_003D03F0[];
typedef struct MenuDrawResources {
    u8 pad00[0x68];
    s32 textures[0];
} MenuDrawResources;

extern MenuDrawResources *D_00438FC8;
extern s32 func_00306CD0(s32, s32, s32, s32, s32, s32, s32, s32);
struct BlendDispatchWork;
extern void func_00294680(struct BlendDispatchWork *, s32, s32);


void func_00294B40(s32 x, s32 y, s32 depth, EventSpriteObject *object,
                   s32 scale, s32 option) {
    u32 index;

    func_00306CD0(D_003D03F0[32][MENU_ICON_X] * 16, D_003D03F0[32][MENU_ICON_Y] * 8,
                  0, scale, 0, D_00438FC8->textures[D_003D03F0[32][MENU_ICON_TEXTURE]],
                  D_003D03F0[32][MENU_ICON_FRAME], option);
    func_0026BEC0(0, 0, 0, scale, (u8 *)object + 0xE8, option);
    func_0026BEC0(0, 0xCF8, 0, scale, (u8 *)object + 0x17C, option);
    func_00294680((struct BlendDispatchWork *)object, scale, option);
    index = evtSelectGraphicSlotBySpriteType(object);
    func_00306CD0(D_003D03F0[index][MENU_ICON_X] * 16, D_003D03F0[index][MENU_ICON_Y] * 8,
                  0, scale, 0, D_00438FC8->textures[D_003D03F0[index][MENU_ICON_TEXTURE]],
                  D_003D03F0[index][MENU_ICON_FRAME], option);
}

INCLUDE_ASM(const s32, "game/code_00294730", func_00294C68);

INCLUDE_ASM(const s32, "game/code_00294730", func_00294D50);

INCLUDE_ASM(const s32, "game/code_00294730", func_00294EB8);

INCLUDE_ASM(const s32, "game/code_00294730", func_00295030);

INCLUDE_ASM(const s32, "game/code_00294730", func_00295400);

INCLUDE_ASM(const s32, "game/code_00294730", func_002958B0);

INCLUDE_ASM(const s32, "game/code_00294730", func_00295D38);

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
    u8 pad08[0x10];
    MenuDrawInner *inner; /* 0x18 */
} MenuDrawObject;

typedef struct MenuDrawValuePanel {
    u8 pad00[0x80];
    MenuDrawObject *object;
    u8 pad84[0xC];
    s32 multiplier;
    u8 pad94[0x33];
    s8 unkC7;
} MenuDrawValuePanel;

/* Draw the child and its container only while the child is active. */
void mnuDrawIfActive(s32 x, s32 y, s32 z, MenuDrawObject *object, s32 drawArg) {
    MenuDrawInner *inner = object->inner;

    if (inner->active != 0) {
        mnuDrawListChildrenWithCountdown(x, y, z, (u8 *)inner, drawArg);
        func_002960F0(x, y, z, 0, (u8 *)object, drawArg);
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

INCLUDE_ASM(const s32, "game/code_00294730", func_002960F0);

INCLUDE_ASM(const s32, "game/code_00294730", func_00296298);

INCLUDE_ASM(const s32, "game/code_00294730", func_00296430);

void func_002967A0(s32 x, s32 y, s32 z, MenuDrawValuePanel *panel, s32 option) {
    s32 texture = D_00438FC8->textures[0];
    s32 firstIndex = panel->object->inner->first->index;
    s32 row = panel->object->inner->item->index - firstIndex;

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

void func_002968B8(s32 x, s32 y, s32 z, MenuDrawValuePanel *panel, s32 scale, s32 option) {
    s32 texture = D_00438FC8->textures[0];
    s32 firstIndex = panel->object->inner->first->index;
    s32 row = panel->object->inner->item->index - firstIndex;

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

INCLUDE_ASM(const s32, "game/code_00294730", func_002969D8);

void mnuDrawIconFixedEntry(s32 x, s32 y, s32 z, s32 unused, s32 scale, s32 option) {
    func_00306CD0(
        x + D_003D03F0[26][MENU_ICON_X] * 16,
        y + D_003D03F0[26][MENU_ICON_Y] * 8,
        z, scale, 0, D_00438FC8->textures[0],
        D_003D03F0[26][MENU_ICON_FRAME], option
    );
}

extern s32 func_0035C860(char *, const char *, ...);
extern s32 func_0019F798(s32, s32, s32, s32, char *, s32);
extern void frFontSetChainFlag(s32, u8);
extern void func_0019D550(s32, s32, s32);
extern void frFontQueueGlyphInSelectedSlot(s32);
extern char D_00437980[];
extern s32 datGameState;
extern void sndSetSequenceVolumePan(s32, s32, s32);

void func_00296B48(s32 x, s32 y, s32 z, s32 unused, s32 scale, s32 option) {
    char text[16];
    s32 value;
    s32 glyph;

    func_00306CD0(
        x + D_003D03F0[25][MENU_ICON_X] * 16,
        y + D_003D03F0[25][MENU_ICON_Y] * 8,
        z, scale, 0, D_00438FC8->textures[0],
        D_003D03F0[25][MENU_ICON_FRAME], option
    );
    value = (s32)((f32)(scale << 7) * 0.00390625f) | 0xA09DC300;
    func_0035C860(text, D_00437980, 0);
    glyph = func_0019F798(x + 0x1910, y + 0x290, z, value, text, 0);
    frFontSetChainFlag(glyph, 4);
    func_0019D550(glyph, 1, option);
    frFontQueueGlyphInSelectedSlot(glyph);
}

void func_00296C58(s32 x, s32 y, s32 depth, MenuDrawValuePanel *panel, s32 option) {
    char text[16];
    s32 texture = D_00438FC8->textures[0];
    MenuDrawObject *object = panel->object;
    MenuDrawInner *inner;
    s32 glyph;

    func_00306CD0(D_003D03F0[25][MENU_ICON_X] * 16, D_003D03F0[25][MENU_ICON_Y] * 8,
                  0, 0x100, 0, texture, D_003D03F0[25][MENU_ICON_FRAME], option);
    func_00306CD0(D_003D03F0[26][MENU_ICON_X] * 16, D_003D03F0[26][MENU_ICON_Y] * 8,
                  0, 0x100, 0, texture, D_003D03F0[26][MENU_ICON_FRAME], option);
    inner = object->inner;
    if (inner->active != 0) {
        func_0035C860(text, D_00437980, inner->item->value * panel->multiplier);
        glyph = func_0019F798(0x1910, 0x290, depth, 0xA09DC380, text, 0);
        frFontSetChainFlag(glyph, 4);
        func_0019D550(glyph, 1, option);
        frFontQueueGlyphInSelectedSlot(glyph);
    }
}

typedef struct MenuValueTransitionState {
    u8 pad00[0xB0];
    s32 previousValue;
    s32 elapsedFrames;
} MenuValueTransitionState;

void func_00296D90(MenuValueTransitionState *state, s32 style) {
    char text[16];

    if (*(s32 *)(datGameState + 0x3C) != state->previousValue) {
        s32 transitionFrames = 20;
        s32 displayedValue;

        sndSetSequenceVolumePan(19, 127, 63);
        state->elapsedFrames++;
        displayedValue = state->previousValue +
                         ((*(s32 *)(datGameState + 0x3C) - state->previousValue) * state->elapsedFrames) /
                             transitionFrames;
        func_0035C860(text, D_00437980, displayedValue);
        if (state->elapsedFrames == transitionFrames) {
            state->previousValue = *(s32 *)(datGameState + 0x3C);
            state->elapsedFrames = 0;
        }
    } else {
        func_0035C860(text, D_00437980, *(s32 *)(datGameState + 0x3C));
    }

    {
        s32 glyph = func_0019F798(0x1910, 0x1D0, 0, style, text, 0);

        func_0019D550(glyph, 1, 0x53);
        frFontQueueGlyphInSelectedSlot(glyph);
    }
}

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437968);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437970);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437978);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437980);
