#include "common.h"

#include "fpu.h"

extern s8 D_0037F510[];

extern s32 sdfAllocGeneralBlock(s32);

extern void *sdfMemoryGetBlockAddress(u32);

extern void func_00313BA8(s32, s32);

extern s32 sdfReleaseResourceAllocation(u32);

extern void sdfReleaseChipBlock();

extern void sdfClearTaskList();

extern void sdfDestroyCallbackWork();

extern u64 func_0019F460(s32, s32, u64, u64, u64, u64);

extern s32 kwlnTaskGetTaskByName(u32);


/* Task item descriptor (0x14): key plus optional handlers, defaults filled in by func_00312A48. */
typedef struct SdfTaskItemDesc {
    s32 key;                         /* 0x00 */
    s32 (*init)(void);               /* 0x04 */
    void (*destroy)(s32, s32);       /* 0x08 */
    s32 (*update)(s32, s32);         /* 0x0C */
    void (*callback)(s32, s32);      /* 0x10 */
} SdfTaskItemDesc;

extern void *func_00312A48(SdfTaskItemDesc *);



extern s32 kwlnTaskDestroyWithHierarchyByName(char *, s32);

extern void sdfGridReleaseAllCells();

extern f32 func_003532B8(f32);

extern f32 sdfSinPoly(f32);

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

extern void sdfQuatMultiply(f32 *, f32 *, f32 *);

extern f32 fldNormalizedVectorDot(f32 *, f32 *);

extern void func_00313A58(u8 *);

typedef struct SdfListNode {
    u32 index;                 /* 0x00 */
    s32 key;                   /* 0x04 */
    struct SdfListNode *next;  /* 0x08 */
    struct SdfListNode *prev;  /* 0x0C */
    void *value;               /* 0x10 */
} SdfListNode;

typedef struct SdfList {
    u32 allocation;            /* 0x00 */
    u32 count;                 /* 0x04 */
    SdfListNode *head;         /* 0x08 */
    SdfListNode *tail;         /* 0x0C */
    u32 userData;              /* 0x10 */
    void (*onRemove)();        /* 0x14: ordinal/payload hook, installed through the word-address API */
    void (*onDestroy)(s32, s32); /* 0x18: two-word callback ABI */
} SdfList;                     /* 0x1C, allocated by sdfCreateTaskHeader */

typedef struct TaskWork {
    u32 allocation;
    char *primaryTaskName;
    char *secondaryTaskName;
    SdfList *list;
    SdfListNode *currentNode; /* Next node to visit; reset to the list head at pass end. */
} TaskWork;

/* Low flag bits: 0 update, 1 callback, 2 initialize once, 15 pending removal.
 * The high word selects active, suspended, or pending-activation dispatch modes. */
typedef struct SdfTaskEntry {
    u32 flags;                   /* 0x00 */
    s32 key;                     /* 0x04 */
    s32 (*init)(void);           /* 0x08 */
    void (*destroy)(s32, s32);   /* 0x0C */
    s32 (*update)(s32, s32);     /* 0x10 */
    void (*callback)(s32, s32);  /* 0x14 */
    s32 initResult;              /* 0x18 */
} SdfTaskEntry;


extern f32 sdfQuatDot(f32 *, f32 *);

extern f32 func_00353140(f32);

typedef struct SdfGridCell {
    u32 index;
    u32 value;
} SdfGridCell;

typedef struct SdfGrid {
    u32 allocation;        /* 0x00 */
    SdfGridCell *cells;    /* 0x04 */
    SdfGridCell *cursor;   /* 0x08 */
    SdfGridCell *viewportOrigin; /* 0x0C */
    u32 cellCount;         /* 0x10 */
    u32 width;             /* 0x14 */
    void (*drawCell)(s32, s32, s32, struct SdfGrid *, SdfGridCell *, s32); /* 0x18 */
    void (*releaseCell)(u32, u32); /* 0x1C */
    void (*onDestroy)(s32, u32); /* 0x20 */
    u16 cellWidth;         /* 0x24 */
    u16 cellHeight;        /* 0x26 */
    u16 visibleColumns;    /* 0x28 */
    u16 visibleRows;       /* 0x2A */
    u16 columnMargin;      /* 0x2C */
    u16 rowMargin;         /* 0x2E */
    u32 userData;          /* 0x30 */
} SdfGrid;

extern void sdfConvertQuaternionRotationMatrix(f32 *, f32 *);
extern void sdfTransformDirectionByMatrix(f32 *, f32 *);
extern void func_0030F8D0(f32 *);
extern void fldNormalizedVectorCross(f32 *, f32 *, f32 *);
extern void sdfVec3ScaleInPlace(f32, f32 *);

extern void func_0019D550(u64, s32, s32);
extern void frFontSetChildColors(u64, u64);
extern u64 func_0019CE78(u64, u64, s32, u64, u64);
extern void frFontSetContextPair(u64, s32, s32);
extern void frFontStoreShiftedContextValue(u64, u64);
extern void frFontSetChainFlag(u64, u8);
extern void frFontSetFlagAndMeasureGlyphs(u64, s32);

extern u64 func_0019F798(s32, s32, u64, u64, u64, u64);
extern u64 func_0019F5E8(s32, s32, u64, u64, u64, u64);
extern u64 itfDrawBankTextWithLayoutFlags(s32, s32, u64, u64, u64, u64);

extern void *sdfAllocSizeClassBlock(s32);
extern void *memset(void *, s32, u32);

extern void func_00313BA8(s32, s32);

extern s32 func_00313BA0(void);
extern s32 func_00313BB0(s32, s32);

extern s32 sdfAllocGeneralBlock(s32);
extern u32 strlen(const char *);
extern s32 func_0035C860(char *buffer, const char *fmt, ...);
extern char D_004388D8[];
extern char D_004388E0[];
extern void sdfCallbackWorkOnRemove();

extern s32 sdfTaskWorkRunAllEntries(void);
extern s32 sdfTaskWorkRunAll(void);
extern void kwlnTaskCreate();
extern TaskWork *sdfCreateNamedTaskWork();

extern void sdfReleaseCurrentTaskOwnedResources(void);
extern s32 sdfTaskWorkRunAllEntries(void);
extern s32 sdfTaskWorkRunAll(void);
extern void sdfReleaseCurrentTaskOwnedResources(void);
extern void kwlnTaskCreate();
extern TaskWork *sdfCreateNamedTaskWork();

extern void func_00312E20(void);

void sdfSetTaskItemMode(void *list, s32 key, u32 mode) {
    u32 *item = sdfFindTaskItemValueByKey(list, key);
    if (item == NULL) {
        return;
    }
    switch (mode) {
    case 3:
        *item = (*(u16 *)item & ~1) | 0x10002;
        break;
    case 4:
        *item = (*(u16 *)item & ~2) | 0x10001;
        break;
    case 2:
        *item = *(u16 *)item | 0x20000;
        break;
    case 1:
        *item = *(u16 *)item | 0x100000;
        break;
    case 0:
        *item = *(u16 *)item | 0x10003;
        break;
    }
}

TaskWork *sdfCreateNamedTaskWork(char *name, s32 destroyCallback, u32 userData) {
    s32 allocation = sdfAllocGeneralBlock(0x14);
    TaskWork *work = sdfMemoryGetBlockAddress(allocation);

    memset(work, 0, 0x14);
    work->allocation = allocation;
    work->list = sdfCreateTaskHeader(userData);
    sdfSetTaskDestroyCallback((s32)work->list, destroyCallback);
    sdfSetTaskSecondaryCallback((s32)work->list, (s32)sdfCallbackWorkOnRemove);
    work->primaryTaskName = sdfAllocSizeClassBlock(strlen(name));
    work->secondaryTaskName = sdfAllocSizeClassBlock(strlen(name) + 5);
    func_0035C860(work->primaryTaskName, D_004388D8, name);
    func_0035C860(work->secondaryTaskName, D_004388E0, name);
    return work;
}

s64 sdfDestroyTaskResourceWork(TaskWork *work) {
    if (work != NULL) {
        sdfDestroyTaskWork(work->list);
        sdfReleaseChipBlock(work->primaryTaskName);
        sdfReleaseChipBlock(work->secondaryTaskName);
        return sdfReleaseResourceAllocation(work->allocation);
    }
}

void *func_00312A48(SdfTaskItemDesc *item) {
    SdfTaskEntry *work = sdfAllocSizeClassBlock(0x1C);

    memset(work, 0, 0x1C);
    work->flags = 0x100007;
    work->key = item->key;
    if (item->init == NULL) {
        work->init = func_00313BA0;
    } else {
        work->init = item->init;
    }
    if (item->destroy == NULL) {
        work->destroy = func_00313BA8;
    } else {
        work->destroy = item->destroy;
    }
    if (item->update == NULL) {
        work->update = func_00313BB0;
    } else {
        work->update = item->update;
    }
    if (item->callback == NULL) {
        work->callback = (void (*)(s32, s32))func_00313BB0;
    } else {
        work->callback = item->callback;
    }
    return work;
}

/* Pass the entry key and saved init result to its destructor, then release it. */
void sdfDestroyCallbackWork(SdfTaskEntry *entry) {
    if (entry != NULL) {
        void (*destroy)(s32, s32) = entry->destroy;
        destroy(entry->key, entry->initResult);
        sdfReleaseChipBlock(entry);
    }
}

void sdfCallbackWorkOnRemove(u32 unused, SdfTaskEntry *entry) {
    sdfDestroyCallbackWork(entry);
}

extern void *kwlnTaskGetUserValue(void);

/* Visit one entry: initialize, remove if pending, otherwise update.
 * An update result of -1 queues removal for its next visit. Returns 0 at pass end. */
s32 sdfTaskWorkStepEntry(TaskWork *work) {
    SdfListNode *node = work->currentNode;
    SdfTaskEntry *entry;
    u32 flags;

    if (node == NULL) {
        work->currentNode = work->list->head;
        return 0;
    }
    entry = (SdfTaskEntry *)node->value;
    flags = entry->flags;
    work->currentNode = node->next;
    switch (flags & 0xFFFF0000) {
    case 0x10000:
        if (flags & 4) {
            entry->initResult = entry->init();
            flags = entry->flags &= ~4;
        }
        if (flags & 0x8000) {
            sdfRemoveTaskItem(work, entry->key);
            return 1;
        }
        if (flags & 1) {
            if (entry->update(entry->key, entry->initResult) == -1) {
                entry->flags |= 0x8000;
            }
        }
        break;
    case 0x20000:
        break;
    case 0x100000:
        /* DDS2 leaves pending-mode activation to the callback pass. */
        break;
    }
    return 1;
}

/* Run one entry's callback phase; return 0 after resetting the cursor at pass end. */
s32 sdfTaskWorkStep(TaskWork *work) {
    SdfListNode *node = work->currentNode;
    SdfTaskEntry *entry;
    u32 flags;

    if (node == NULL) {
        work->currentNode = work->list->head;
        return 0;
    }
    entry = (SdfTaskEntry *)node->value;
    flags = entry->flags;
    work->currentNode = node->next;
    switch (flags & 0xFFFF0000) {
    case 0x10000:
        if (flags & 2) {
            entry->callback(entry->key, entry->initResult);
        }
        break;
    case 0x20000:
        break;
    case 0x100000:
        /* Activate the pending mode here; this visit does not invoke the callback. */
        entry->flags = (flags & 0xFFEFFFFF) | 0x10000;
        break;
    }
    return 1;
}

s32 sdfTaskWorkRunAllEntries(void) {
    TaskWork *work = kwlnTaskGetUserValue();

    if (work->currentNode == NULL) {
        return -1;
    }
    while (sdfTaskWorkStepEntry(work) == 1) {
    }
    return 0;
}

s32 sdfTaskWorkRunAll(void) {
    TaskWork *work = kwlnTaskGetUserValue();

    if (work->currentNode == NULL) {
        return -1;
    }
    while (sdfTaskWorkStep(work) == 1) {
    }
    return 0;
}

void sdfReleaseCurrentTaskOwnedResources(void) {
    sdfDestroyTaskResourceWork(kwlnTaskGetUserValue());
}

void func_00312E20(void) {
}

INCLUDE_ASM(const s32, "game/code_00312850", func_00312E28);

/* Set horizontal/vertical cursor margins without changing the viewport itself. */
void sdfSetShortPairValues(SdfGrid *grid, s32 columnMargin, s32 rowMargin) {
    grid->columnMargin = columnMargin;
    grid->rowMargin = rowMargin;
}

/* Release cells and invoke onDestroy(0, userData); a nonnull grid returns the allocation-release result. */
s64 sdfDestroyGridWork(SdfGrid *owner) {
    if (owner != NULL) {
        sdfGridReleaseAllCells();
        owner->onDestroy(0, owner->userData);
        return sdfReleaseResourceAllocation(owner->allocation);
    }
}

void func_00312FB0(void) {
    sdfGridReleaseAllCells();
}

SdfGridCell *sdfGridGetCell(SdfGrid *grid, s32 column, s32 row) {
    u32 cellIndex;

    cellIndex = row * grid->width + column;
    if (cellIndex >= grid->cellCount) {
        return NULL;
    }
    return &grid->cells[cellIndex];
}

/* Return the cursor's zero-based column and row; the grid width must be nonzero. */
void sdfGridGetCursorCoordinates(void *grid, u32 *column, u32 *row) {
    u32 *cursorCell = *(u32 **)((s32)grid + 8);
    *column = *cursorCell % *(u32 *)((s32)grid + 0x14);
    *row = *cursorCell / *(u32 *)((s32)grid + 0x14);
}

u32 sdfGridGetCellValue(SdfGrid *grid, s32 column, s32 row) {
    u32 cellIndex;

    cellIndex = row * grid->width + column;
    if (cellIndex >= grid->cellCount) {
        return 0;
    }
    return grid->cells[cellIndex].value;
}

void sdfGridSetCellValue(s32 grid, s32 column, s32 row, u32 value) {
    u32 cellIndex;

    cellIndex = row * *(s32 *)(grid + 0x14) + column;
    if (cellIndex < *(u32 *)(grid + 0x10)) {
        *(u32 *)(cellIndex * 8 + *(s32 *)(grid + 4) + 4) = value;
    }
}

SdfGridCell *sdfGridCursorUp(SdfGrid *grid) {
    SdfGridCell *cell = grid->cursor;
    u32 width = grid->width;
    u32 index = cell->index;

    cell -= width;
    if (index < width) {
        return NULL;
    }
    grid->cursor = cell;
    func_00313A58((u8 *)grid);
    return cell;
}

SdfGridCell *sdfGridCursorDown(SdfGrid *grid) {
    SdfGridCell *cell = grid->cursor;
    u32 width = grid->width;

    if (cell->index >= grid->cellCount - width) {
        return NULL;
    }
    cell += width;
    grid->cursor = cell;
    func_00313A58((u8 *)grid);
    return cell;
}

SdfGridCell *sdfGridCursorLeft(SdfGrid *grid) {
    SdfGridCell *cell = grid->cursor;
    u32 width = grid->width;
    u32 index = cell->index;

    cell -= 1;
    if (index % width == 0) {
        return NULL;
    }
    grid->cursor = cell;
    func_00313A58((u8 *)grid);
    return cell;
}

u8 *sdfGridCursorRight(u8 *grid) {
    u8 *cell = *(u8 **)(grid + 8);
    u32 width = *(u32 *)(grid + 0x14);
    u32 index = *(u32 *)cell;
    cell += 8;
    if (index % width == width - 1) {
        return NULL;
    }
    *(u8 **)(grid + 8) = cell;
    func_00313A58(grid);
    return cell;
}

SdfGridCell *sdfGridSetCursorCell(SdfGrid *grid, u32 column, u32 row) {
    SdfGridCell *result = NULL;

    if (column >= grid->width) {
        return result;
    }
    if (row >= grid->cellCount / grid->width) {
        return result;
    }
    grid->cursor = sdfGridGetCell(grid, column, row);
    func_00313A58((u8 *)grid);
    return grid->cursor;
}

INCLUDE_ASM(const s32, "game/code_00312850", func_00313280);

INCLUDE_ASM(const s32, "game/code_00312850", func_003133C8);

INCLUDE_ASM(const s32, "game/code_00312850", func_00313538);

INCLUDE_ASM(const s32, "game/code_00312850", func_003136A0);

SdfGridCell *sdfGridSelectFilledCell(SdfGrid *grid, u32 column, u32 row) {
    SdfGridCell *result = NULL;
    SdfGridCell *cell;

    if (column >= grid->width) {
        return result;
    }
    if (row >= grid->cellCount / grid->width) {
        return result;
    }
    cell = sdfGridGetCell(grid, column, row);
    if (cell->value == 0) {
        return NULL;
    }
    grid->cursor = cell;
    func_00313A58((u8 *)grid);
    return cell;
}

/* Draw the viewport's rectangular cell range relative to the supplied origin. */
void sdfGridDrawVisibleCells(s32 originX, s32 originY, s32 layer, SdfGrid *grid, s32 drawContext) {
    SdfGridCell *cell = grid->viewportOrigin;
    s32 firstRow = cell->index / grid->width;
    s32 firstColumn = cell->index % grid->width;
    s32 endRow = firstRow + grid->visibleRows;
    s32 row;
    s32 column;

    for (row = firstRow; row < endRow; row++) {
        cell = grid->cells + row * grid->width + firstColumn;
        for (column = firstColumn; column < firstColumn + grid->visibleColumns; column++) {
            grid->drawCell(originX + (column - firstColumn) * grid->cellWidth,
                           originY + (row - firstRow) * grid->cellHeight,
                           layer, grid, cell, drawContext);
            cell++;
        }
    }
}

/* Restore row-major cell indices and release each nonzero cell value. */
void sdfGridReleaseAllCells(SdfGrid *grid) {
    u32 cellIndex = 0;
    SdfGridCell *firstCell = grid->cells;
    SdfGridCell *cell;

    if (grid->cellCount != 0) {
        cell = firstCell;
        do {
            u32 cellValue = cell->value;
            cell->index = cellIndex;
            if (cellValue != 0) {
                grid->releaseCell(cellIndex, cellValue);
                cell->value = 0;
            }
            cellIndex++;
            cell++;
        } while (cellIndex < grid->cellCount);
    }
}

INCLUDE_ASM(const s32, "game/code_00312850", func_00313A58);

float sdfMultiplyAddFloat(float addend, float multiplicand, float multiplier) {
    return addend + multiplicand * multiplier;
}

s32 func_00313BA0(void) {
    return 0;
}

void func_00313BA8(s32 key, s32 initResult) {
}

s32 func_00313BB0(s32 key, s32 initResult) {
    return 0;
}
