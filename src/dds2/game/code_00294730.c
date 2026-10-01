#include "common.h"

extern void func_00295D38();

extern void func_002958B0();

extern void func_00296018(s32, s32, s32, u8 *, s32);

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
        func_00296018(x, y, z, (u8 *)inner, drawArg);
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
void func_00296018(s32 x, s32 y, s32 z, u8 *object, s32 drawArg) {
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

INCLUDE_ASM(const s32, "game/code_00294730", func_00296AF8);

INCLUDE_ASM(const s32, "game/code_00294730", func_00296B48);

INCLUDE_ASM(const s32, "game/code_00294730", func_00296C58);

INCLUDE_ASM(const s32, "game/code_00294730", func_00296D90);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437968);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437970);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437978);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437980);

