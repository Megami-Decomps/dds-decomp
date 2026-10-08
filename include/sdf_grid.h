#ifndef SDF_GRID_H
#define SDF_GRID_H

#include "common.h"

struct SdfMemBlock;

typedef struct SdfGridCell {
    u32 index;
    u32 value;
} SdfGridCell;

/* Grid header allocated immediately before its variable-size cell array. */
typedef struct SdfGrid {
    struct SdfMemBlock *allocation;
    SdfGridCell *cells;
    SdfGridCell *cursor;
    SdfGridCell *viewportOrigin;
    u32 cellCount;
    u32 width;
    void (*drawCell)(s32, s32, s32, struct SdfGrid *, SdfGridCell *, s32);
    void (*releaseCell)(u32, u32);
    void (*onDestroy)(s32, u32);
    u16 cellWidth;
    u16 cellHeight;
    u16 visibleColumns;
    u16 visibleRows;
    u16 columnMargin;
    u16 rowMargin;
    u32 userData;
} SdfGrid;

typedef char SdfGridLayoutAssert[
    (sizeof(SdfGridCell) == 8 && sizeof(SdfGrid) == 0x34 &&
     (u32)&((SdfGrid *)0)->allocation == 0 &&
     (u32)&((SdfGrid *)0)->cursor == 8 &&
     (u32)&((SdfGrid *)0)->drawCell == 0x18 &&
     (u32)&((SdfGrid *)0)->userData == 0x30)
        ? 1 : -1];

#endif
