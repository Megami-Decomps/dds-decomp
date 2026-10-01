#include "common.h"

extern void func_00295D38();

extern void func_002958B0();

extern void mnuDrawListChildrenWithCountdown(s32, s32, s32, u8 *, s32);

extern void func_002960F0(s32, s32, s32, s32, u8 *, s32);

typedef struct EventSpriteObject {
    u8 pad00[8];
    s32 type;
} EventSpriteObject;

u32 func_00294730(EventSpriteObject *object) {
    u32 result;

    result = 0;
    if ((object->type == 1) || (object->type == 3)) {
        result = 0x3a;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00294730", func_00294758);

INCLUDE_ASM(const s32, "game/code_00294730", func_00294930);

extern struct {
    u8 pad00[0xCA];
    s16 unkCA;
    s16 unkCC;
    s16 unkCE;
    s16 unkD0;
    s16 unkD2;
    s16 unkD4;
    s16 unkD6;
} D_003D03F0;
extern u8 *D_00438FC8;


INCLUDE_ASM(const s32, "game/code_00294730", func_00294B40);

INCLUDE_ASM(const s32, "game/code_00294730", func_00294C68);

INCLUDE_ASM(const s32, "game/code_00294730", func_00294D50);

INCLUDE_ASM(const s32, "game/code_00294730", func_00294EB8);

INCLUDE_ASM(const s32, "game/code_00294730", func_00295030);

INCLUDE_ASM(const s32, "game/code_00294730", func_00295400);

INCLUDE_ASM(const s32, "game/code_00294730", func_002958B0);

INCLUDE_ASM(const s32, "game/code_00294730", func_00295D38);

typedef struct MenuDrawInner {
    u8 pad00[0x20];
    s32 active;
} MenuDrawInner;

typedef struct MenuDrawObject {
    u8 pad00[4];
    u32 flags;
    u8 pad08[0x10];
    MenuDrawInner *inner; /* 0x18 */
} MenuDrawObject;

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

INCLUDE_ASM(const s32, "game/code_00294730", func_002967A0);

INCLUDE_ASM(const s32, "game/code_00294730", func_002968B8);

INCLUDE_ASM(const s32, "game/code_00294730", func_002969D8);

void mnuDrawIconFixedEntry(s32 x, s32 y, s32 z, s32 unused, s32 scale, s32 option) {
    func_00306CD0(
        x + D_003D03F0.unkD4 * 16,
        y + D_003D03F0.unkD6 * 8,
        z, scale, 0, *(s32 *)(D_00438FC8 + 0x68),
        D_003D03F0.unkD2, option
    );
}

extern s32 func_0035C860(char *, const char *, ...);
extern s32 func_0019F798(s32, s32, s32, s32, char *, s32);
extern void frFontSetChainFlag(s32, u8);
extern void func_0019D550(s32, s32, s32);
extern void frFontQueueGlyphInSelectedSlot(s32);
extern char D_00437980[];

void func_00296B48(s32 x, s32 y, s32 z, s32 unused, s32 scale, s32 option) {
    char text[16];
    s32 value;
    s32 glyph;

    func_00306CD0(
        x + D_003D03F0.unkCC * 16,
        y + D_003D03F0.unkCE * 8,
        z, scale, 0, *(s32 *)(D_00438FC8 + 0x68),
        D_003D03F0.unkCA, option
    );
    value = (s32)((f32)(scale << 7) * 0.00390625f) | 0xA09DC300;
    func_0035C860(text, D_00437980, 0);
    glyph = func_0019F798(x + 0x1910, y + 0x290, z, value, text, 0);
    frFontSetChainFlag(glyph, 4);
    func_0019D550(glyph, 1, option);
    frFontQueueGlyphInSelectedSlot(glyph);
}

INCLUDE_ASM(const s32, "game/code_00294730", func_00296C58);

INCLUDE_ASM(const s32, "game/code_00294730", func_00296D90);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437968);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437970);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437978);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437980);

