#include "common.h"

typedef struct MantraPulseEntry {
    s32 unk_0;
    s32 active;
} MantraPulseEntry;

typedef struct MantraPulseGrid {
    u8 pad00[4];
    MantraPulseEntry *entries;
    u8 pad08[0xC];
    s32 stride;
} MantraPulseGrid;

extern void itfDspDrawStrip(s32, s32, s32, s32, s32);
extern void mnuDrawSelectedMantraEntry();
extern void mnuChooseDisplaySpriteKindFromEntryFlags(s32, s32, s32);
extern void func_0024E260(s32, s32, s32, s32, s32, s32);

void mnuDrawMantraPulseStripAndKind(void *object, s32 scale, s32 context) {
    itfDspDrawStrip(0, 0, 0, scale, context);
    mnuDrawSelectedMantraEntry(object, scale, context);
    mnuChooseDisplaySpriteKindFromEntryFlags(object, scale, context);
}

INCLUDE_ASM(const s32, "game/code_00257200", func_00257270);

INCLUDE_ASM(const s32, "game/code_00257200", func_002573E8);

extern void *func_002CB3B8(s32, s32);
extern void func_002593E0(s32, MantraPulseGrid *, MantraPulseEntry *);
extern s32 mnuSceneResourceContext;

void mnuAdvanceMantraPulseGridEntries(s32 argument, MantraPulseGrid *grid) {
    MantraPulseEntry *entry;
    s32 row;
    s32 col;

    func_002CB3B8(mnuSceneResourceContext, 1);
    for (row = 0; row < 0x11; row++) {
        entry = grid->entries + row * grid->stride;
        for (col = 0; col < 15; col++) {
            if (entry[col].active != 0) {
                func_002593E0(argument, grid, &entry[col]);
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00257200", func_00257718);

typedef struct MantraPulseDisplayWork {
    u8 pad00[0x5AC];
    u8 flags;
} MantraPulseDisplayWork;

extern void *func_002CB3B8(s32, s32);
extern void func_0024EDC0(s32, s32, s32, s32, s32, s32, f32, f32, s32);
extern void func_00259890(s32, s32, s32, s32, s32, MantraPulseGrid *, f32, f32, MantraPulseEntry *, s32);
extern void func_00259B40(s32, s32, s32, s32, s32, MantraPulseGrid *, f32, f32, MantraPulseEntry *, s32);
extern s32 mnuSceneResourceContext;

/* One local `n` serves as both the per-row column countdown in the two entry
 * loops and the final-row bound in the band loop: one declared type, one
 * neutral name, as a C89 programmer with all declarations at the top would
 * write. That reuse is what retail's bytes require. */
void func_002579B0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4,
                   MantraPulseGrid *grid, s32 arg6, f32 scaleX, f32 scaleY) {
    MantraPulseDisplayWork *display;
    MantraPulseEntry *entry;
    s32 row;
    s32 n;

    display = func_002CB3B8(mnuSceneResourceContext, 1);
    for (row = 0; row < 0xD; row++) {
        func_0024EDC0(arg0, arg1, arg2, arg3, row + 0x1F, 0x20, scaleX, scaleY, arg6);
    }
    for (row = 0; row < 0x11; row++) {
        entry = grid->entries + row * grid->stride;
        n = 0xE;
        do {
            if (entry->active != 0) {
                func_00259890(arg0, arg1, arg2, arg3, arg4, grid, scaleX, scaleY, entry, arg6);
            }
            entry++;
            n--;
        } while (n >= 0);
    }
    n = (display->flags & 1) != 0 ? 0x1E : 0x1A;
    for (row = 0; row <= n; row++) {
        func_0024EDC0(arg0, arg1, arg2, arg3, row, 0x20, scaleX, scaleY, arg6);
    }
    for (row = 0; row < 0x11; row++) {
        entry = grid->entries + row * grid->stride;
        n = 0xE;
        do {
            if (entry->active != 0) {
                func_00259B40(arg0, arg1, arg2, arg3, arg4, grid, scaleX, scaleY, entry, arg6);
            }
            entry++;
            n--;
        } while (n >= 0);
    }
}

typedef struct {
    u8 pad00[0x484];
    MantraPulseGrid *grid; /* 0x484 */
    u8 pad488[8];
    s32 field_0x490;
} DisplayGridWork;

void func_00257BD8(DisplayGridWork *work, s32 argument) {
    mnuAdvanceMantraPulseGridEntries(argument, work->grid);
    mnuAdvanceWrappingFrame(&work->field_0x490);
}

INCLUDE_ASM(const s32, "game/code_00257200", mnuDrawMantraPulseFrame);

void func_00257DF0(s32 context, s32 layer) {
    s32 x = *(s16 *)(context + 0x59C) - *(s16 *)(context + 0x5A0) - 9;
    s32 y = *(s16 *)(context + 0x59E) - *(s16 *)(context + 0x5A2) + 0x45;

    func_0024E260(x, y, 0, 0x80, 0x3E, layer);
    func_0024E260(x, y, 0, 0x80, 0x3D, layer);
}

typedef struct {
    s32 x;
    s32 y;
} SceneCoordPair;

typedef struct {
    u8 pad00[0x49C];
    SceneCoordPair points[10]; /* 0x49C */
    u8 pad4EC[0xB0];
    s16 entryX;                /* 0x59C */
    s16 entryY;                /* 0x59E */
} SceneCoordWork;

void mnuCopySceneCoordinates(SceneCoordWork *work) {
    s32 x = work->entryX;
    s32 y = work->entryY;
    SceneCoordPair *point = work->points;
    s32 remaining = 9;

    do {
        point->x = x;
        point->y = y;
        point++;
        remaining--;
    } while (remaining >= 0);
}

void mnuAdvanceWrappingFrame(s32 *frame) {
    s32 previous;

    previous = *frame;
    *frame = previous + 1;
    if (0x3c < previous + 1) {
        *frame = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00257200", func_00257ED0);

INCLUDE_RODATA(const s32, "game/code_00257200", D_003AF950);

