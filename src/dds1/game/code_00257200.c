#include "common.h"
#include "mnu_profile_progress.h"
#include "mnu_mantra_grid.h"
#include "mnu_scene_work.h"

extern void itfDspDrawStrip(s32, s32, s32, s32, s32);
extern void mnuDrawSelectedMantraEntry(MenuSceneWork *, s32, s32);
extern void mnuChooseDisplaySpriteKindFromEntryFlags(MenuSceneWork *, s32, s32);
extern void func_0024E260(s32, s32, s32, s32, s32, s32);

void mnuDrawMantraPulseStripAndKind(MenuSceneWork *object, s32 scale, s32 context) {
    itfDspDrawStrip(0, 0, 0, scale, context);
    mnuDrawSelectedMantraEntry(object, scale, context);
    mnuChooseDisplaySpriteKindFromEntryFlags(object, scale, context);
}

INCLUDE_ASM(const s32, "game/code_00257200", func_00257270);

typedef struct MantraPulseDisplayWork {
    u8 pad00[0x484];
    SdfGrid *grid;
    u8 pad488[8];
    s32 frame;
    u8 pad494[0x10C];
    s16 scrollX;
    s16 scrollY;
    u8 pad5A4[8];
    u8 flags;
} MantraPulseDisplayWork;

extern s16 D_0036B7F0[][6];
extern void sdfSubmitGsTestOneRegisterPacket();
extern void uiDrawUniformColorRect(s32, s32, s32, s32, s32, s32, s32);
extern void uiDrawActiveSurfaceRegion(s32);
extern void sdfDispatchSurfaceWithPreparedTexturePacket(s32);
extern void sdfSubmitGsAlphaOneRegisterPacket(u32, u32);

/* Draw the two mantra-entry passes, then restore the surface's GS state. */
void mnuDrawMantraPulseGridPasses(MantraPulseDisplayWork *work, s32 surface) {
    SdfGrid *grid;
    SdfGridCell *cell;
    MnuMantraGridEntry *entry;
    s32 row;
    s32 col;
    s32 x;
    s32 y;

    grid = work->grid;
    x = -9 - work->scrollX;
    y = 0x45 - work->scrollY;
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    uiDrawUniformColorRect(0, 0, -1, 0x2000, 0xE00, 0, surface);
    sdfSubmitGsTestOneRegisterPacket(0x3000DL, surface);
    uiDrawActiveSurfaceRegion(surface);
    for (row = 0; row < 0x11; row++) {
        cell = grid->cells + row * grid->width;
        for (col = 0; col < 15; col++) {
            if (cell[col].value != 0) {
                entry = (MnuMantraGridEntry *)(u32)cell[col].value;
                grid->drawCell(x + D_0036B7F0[entry->sceneId][2],
                               y + D_0036B7F0[entry->sceneId][3],
                               0, grid, &cell[col], surface);
            }
        }
    }
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    uiDrawUniformColorRect(0, 0, -1, 0x2000, 0x190, 0, surface);
    uiDrawUniformColorRect(0, 0xA50, -1, 0x2000, 0x4B0, 0, surface);
    sdfDispatchSurfaceWithPreparedTexturePacket(surface);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surface);
    sdfSubmitGsTestOneRegisterPacket(0x50000, surface);
    for (row = 0; row < 0x11; row++) {
        cell = grid->cells + row * grid->width;
        for (col = 0; col < 15; col++) {
            if (cell[col].value != 0) {
                entry = (MnuMantraGridEntry *)(u32)cell[col].value;
                grid->drawCell(x + D_0036B7F0[entry->sceneId][2],
                               y + D_0036B7F0[entry->sceneId][3],
                               1, grid, &cell[col], surface);
            }
        }
    }
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surface);
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0, surface);
    sdfSubmitGsTestOneRegisterPacket(0x5100DL, surface);
}

extern void *func_002CB3B8(s32, s32);
extern void func_002593E0(MnuProfileProgress *, SdfGrid *, SdfGridCell *);
extern s32 mnuSceneResourceContext;

void mnuAdvanceMantraPulseGridEntries(MnuProfileProgress *selection, SdfGrid *grid) {
    SdfGridCell *cell;
    s32 row;
    s32 col;

    func_002CB3B8(mnuSceneResourceContext, 1);
    for (row = 0; row < 0x11; row++) {
        cell = grid->cells + row * grid->width;
        for (col = 0; col < 15; col++) {
            if (cell[col].value != 0) {
                func_002593E0(selection, grid, &cell[col]);
            }
        }
    }
}


extern void *func_002CB3B8(s32, s32);
extern void func_0024EDC0(s32, s32, s32, s32, s32, s32, f32, f32, s32);
extern void func_00259498(s32, s32, s32, s32, MnuProfileProgress *, SdfGrid *, SdfGridCell *, s32);
extern void func_00259890(s32, s32, s32, s32, MnuProfileProgress *, SdfGrid *, f32, f32, SdfGridCell *, s32);
extern void func_00259B40(s32, s32, s32, s32, MnuProfileProgress *, SdfGrid *, f32, f32, SdfGridCell *, s32);
extern void func_00257ED0(s32, s32, s32, s32, MantraPulseDisplayWork *, s32);
extern void func_00258B90(s32, s32, s32, s32, void *, s32);
extern s32 mnuSceneResourceContext;

/* Draw the mantra pulse band and both entry passes at unit scale, placing the
 * selected entry with its offsets from the display table. */
void func_00257718(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                   MnuProfileProgress *selection, SdfGrid *grid,
                   s32 arg6) {
    MantraPulseDisplayWork *display;
    SdfGridCell *cell;
    s32 row;
    s32 n;

    display = func_002CB3B8(mnuSceneResourceContext, 1);
    for (row = 0; row < 0xD; row++) {
        func_0024EDC0(arg0, arg1, arg2, arg3, row + 0x1F, 0x20, 1.0f, 1.0f, arg6);
    }
    for (row = 0; row < 0x11; row++) {
        cell = grid->cells + row * grid->width;
        n = 0xE;
        do {
            if (cell->value != 0) {
                func_00259498(arg0, arg1, arg2, arg3, selection, grid, cell, arg6);
            }
            cell++;
            n--;
        } while (n >= 0);
    }
    n = (display->flags & 1) != 0 ? 0x1E : 0x1A;
    for (row = 0; row <= n; row++) {
        func_0024EDC0(arg0, arg1, arg2, arg3, row, 0x20, 1.0f, 1.0f, arg6);
    }
    func_00257ED0(0, 0, arg2, arg3, display, arg6);
    func_00258B90(arg0 + D_0036B7F0[selection->profileId][2],
                  arg1 + D_0036B7F0[selection->profileId][3],
                  arg2, arg3, (u8 *)display + 0x488, arg6);
    for (row = 0; row < 0x11; row++) {
        cell = grid->cells + row * grid->width;
        n = 0xE;
        do {
            if (cell->value != 0) {
                func_00259B40(arg0, arg1, arg2, arg3, selection, grid, 1.0f, 1.0f, cell, arg6);
            }
            cell++;
            n--;
        } while (n >= 0);
    }
}

/* One local `n` serves as both the per-row column countdown in the two entry
 * loops and the final-row bound in the band loop: one declared type, one
 * neutral name, as a C89 programmer with all declarations at the top would
 * write. That reuse is what retail's bytes require. */
void func_002579B0(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                   MnuProfileProgress *selection, SdfGrid *grid, s32 arg6,
                   f32 scaleX, f32 scaleY) {
    MantraPulseDisplayWork *display;
    SdfGridCell *cell;
    s32 row;
    s32 n;

    display = func_002CB3B8(mnuSceneResourceContext, 1);
    for (row = 0; row < 0xD; row++) {
        func_0024EDC0(arg0, arg1, arg2, arg3, row + 0x1F, 0x20, scaleX, scaleY, arg6);
    }
    for (row = 0; row < 0x11; row++) {
        cell = grid->cells + row * grid->width;
        n = 0xE;
        do {
            if (cell->value != 0) {
                func_00259890(arg0, arg1, arg2, arg3, selection, grid, scaleX, scaleY, cell, arg6);
            }
            cell++;
            n--;
        } while (n >= 0);
    }
    n = (display->flags & 1) != 0 ? 0x1E : 0x1A;
    for (row = 0; row <= n; row++) {
        func_0024EDC0(arg0, arg1, arg2, arg3, row, 0x20, scaleX, scaleY, arg6);
    }
    for (row = 0; row < 0x11; row++) {
        cell = grid->cells + row * grid->width;
        n = 0xE;
        do {
            if (cell->value != 0) {
                func_00259B40(arg0, arg1, arg2, arg3, selection, grid, scaleX, scaleY, cell, arg6);
            }
            cell++;
            n--;
        } while (n >= 0);
    }
}

void func_00257BD8(MantraPulseDisplayWork *work, MnuProfileProgress *selection) {
    mnuAdvanceMantraPulseGridEntries(selection, work->grid);
    mnuAdvanceWrappingFrame(&work->frame);
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
