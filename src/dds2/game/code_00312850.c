#include "common.h"
#include "sdf_trig.h"
#include "sdf_quaternion.h"
#include "sdf_chip.h"
#include "fr_font_context.h"
#include "fr_font.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_resource.h"

#include "fpu.h"
#include "sdf.h"
#include "sdf_grid.h"
#include "sdf_grid_directions.h"
#include "sdf_task_work.h"

extern s8 D_0037F510[];













extern void sdfGridReleaseAllCells(SdfGrid *);

extern f32 acosf(f32);




extern f32 fldNormalizedVectorDot(f32 *, f32 *);

extern void sdfGridFollowCursorWithinMargins(SdfGrid *);


extern f32 sdfQuatDot(f32 *, f32 *);

extern f32 sinf(f32);

extern void sdfConvertQuaternionRotationMatrix(f32 *, f32 *);
#include "sdf_vector_transform.h"
extern f32 sdfVec3NormalizeInPlaceAndGetLength(f32 *vector);
extern void fldNormalizedVectorCross(f32 *, f32 *, f32 *);
extern void sdfVec3ScaleInPlace(f32, f32 *);

struct FrFontGlyph;

extern void frFontDrawGlyphChain(u64, s32, s32);

extern u64 itfCreateTightFont3TextGlyph(s32, s32, u64, u64, u64, u64);
extern u64 itfCreateFont2TextGlyph(s32, s32, u64, u64, u64, u64);
extern u64 itfDrawBankTextWithLayoutFlags(s32, s32, u64, u64, u64, u64);

extern void *memset(void *, s32, u32);



extern u32 strlen(const char *);
extern s32 sprintf(char *buffer, const char *fmt, ...);
extern char D_004388D8[];
extern char D_004388E0[];



extern void sdfNoOpGridDrawCallback(void);

void sdfSetTaskItemMode(TaskWork *work, s32 key, u32 mode) {
    SdfTaskEntry *item = sdfFindTaskItemValueByKey(work, key);
    if (item == NULL) {
        return;
    }
    switch (mode) {
    case SDF_TASK_ITEM_MODE_CALLBACK_ONLY:
        item->flags = (*(u16 *)&item->flags & ~SDF_TASK_ENTRY_UPDATE_ENABLED) |
                      SDF_TASK_ENTRY_ACTIVE | SDF_TASK_ENTRY_CALLBACK_ENABLED;
        break;
    case SDF_TASK_ITEM_MODE_UPDATE_ONLY:
        item->flags = (*(u16 *)&item->flags & ~SDF_TASK_ENTRY_CALLBACK_ENABLED) |
                      SDF_TASK_ENTRY_ACTIVE | SDF_TASK_ENTRY_UPDATE_ENABLED;
        break;
    case SDF_TASK_ITEM_MODE_SUSPENDED:
        item->flags = *(u16 *)&item->flags | SDF_TASK_ENTRY_SUSPENDED;
        break;
    case SDF_TASK_ITEM_MODE_PENDING_ACTIVATION:
        item->flags = *(u16 *)&item->flags | SDF_TASK_ENTRY_PENDING_ACTIVATION;
        break;
    case SDF_TASK_ITEM_MODE_ENABLE_UPDATE_AND_CALLBACK:
        item->flags = *(u16 *)&item->flags | SDF_TASK_ENTRY_ACTIVE |
                      SDF_TASK_ENTRY_UPDATE_ENABLED | SDF_TASK_ENTRY_CALLBACK_ENABLED;
        break;
    }
}

TaskWork *sdfCreateNamedTaskWork(char *name, SdfListCallback destroyCallback, void *userData) {
    SdfMemBlock *allocation = sdfAllocGeneralBlock(0x14);
    TaskWork *work = (TaskWork *)sdfMemoryGetBlockAddress(allocation);

    memset(work, 0, 0x14);
    work->allocation = allocation;
    work->list = sdfCreateTaskHeader(userData);
    sdfSetTaskDestroyCallback(work->list, destroyCallback);
    sdfSetTaskSecondaryCallback(work->list, sdfCallbackWorkOnRemove);
    work->primaryTaskName = sdfAllocSizeClassBlock(strlen(name));
    work->secondaryTaskName = sdfAllocSizeClassBlock(strlen(name) + 5);
    sprintf(work->primaryTaskName, D_004388D8, name);
    sprintf(work->secondaryTaskName, D_004388E0, name);
    return work;
}

void sdfDestroyTaskResourceWork(TaskWork *work) {
    if (work != NULL) {
        sdfDestroyTaskWork(work->list);
        sdfReleaseChipBlock(work->primaryTaskName);
        sdfReleaseChipBlock(work->secondaryTaskName);
        sdfReleaseResourceAllocation(work->allocation);
    }
}

SdfTaskEntry *sdfCreateTaskEntry(SdfTaskItemDesc *item) {
    SdfTaskEntry *work = sdfAllocSizeClassBlock(0x1C);

    memset(work, 0, 0x1C);
    work->flags = SDF_TASK_ENTRY_UPDATE_ENABLED | SDF_TASK_ENTRY_CALLBACK_ENABLED |
                  SDF_TASK_ENTRY_INITIALIZE_ONCE | SDF_TASK_ENTRY_PENDING_ACTIVATION;
    work->key = item->key;
    if (item->init == NULL) {
        work->init = sdfDefaultTaskEntryInit;
    } else {
        work->init = item->init;
    }
    if (item->destroy == NULL) {
        work->destroy = sdfNoOpKeyValueCallback;
    } else {
        work->destroy = item->destroy;
    }
    if (item->update == NULL) {
        work->update = sdfDefaultTaskEntryUpdate;
    } else {
        work->update = item->update;
    }
    if (item->callback == NULL) {
        work->callback = (void (*)(s32, s32))sdfDefaultTaskEntryUpdate;
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

void sdfCallbackWorkOnRemove(s32 key, SdfTaskEntry *entry) {
    sdfDestroyCallbackWork(entry);
}


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
    switch (flags & SDF_TASK_ENTRY_STATE_MASK) {
    case SDF_TASK_ENTRY_ACTIVE:
        if (flags & SDF_TASK_ENTRY_INITIALIZE_ONCE) {
            entry->initResult = entry->init();
            flags = entry->flags &= ~SDF_TASK_ENTRY_INITIALIZE_ONCE;
        }
        if (flags & SDF_TASK_ENTRY_REMOVE_PENDING) {
            sdfRemoveTaskItem(work, entry->key);
            return 1;
        }
        if (flags & SDF_TASK_ENTRY_UPDATE_ENABLED) {
            if (entry->update(entry->key, entry->initResult) == -1) {
                entry->flags |= SDF_TASK_ENTRY_REMOVE_PENDING;
            }
        }
        break;
    case SDF_TASK_ENTRY_SUSPENDED:
        break;
    case SDF_TASK_ENTRY_PENDING_ACTIVATION:
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
    switch (flags & SDF_TASK_ENTRY_STATE_MASK) {
    case SDF_TASK_ENTRY_ACTIVE:
        if (flags & SDF_TASK_ENTRY_CALLBACK_ENABLED) {
            entry->callback(entry->key, entry->initResult);
        }
        break;
    case SDF_TASK_ENTRY_SUSPENDED:
        break;
    case SDF_TASK_ENTRY_PENDING_ACTIVATION:
        /* Activate the pending mode here; this visit does not invoke the callback. */
        entry->flags = (flags & ~SDF_TASK_ENTRY_PENDING_ACTIVATION) | SDF_TASK_ENTRY_ACTIVE;
        break;
    }
    return 1;
}

s32 sdfTaskWorkRunAllEntries(KwlnTask *task) {
    TaskWork *work = (TaskWork *)kwlnTaskGetUserValue(task);

    if (work->currentNode == NULL) {
        return -1;
    }
    while (sdfTaskWorkStepEntry(work) == 1) {
    }
    return 0;
}

s32 sdfTaskWorkRunAll(KwlnTask *task) {
    TaskWork *work = (TaskWork *)kwlnTaskGetUserValue(task);

    if (work->currentNode == NULL) {
        return -1;
    }
    while (sdfTaskWorkStep(work) == 1) {
    }
    return 0;
}

void sdfReleaseCurrentTaskOwnedResources(KwlnTask *task) {
    sdfDestroyTaskResourceWork((TaskWork *)kwlnTaskGetUserValue(task));
}

/* Default empty grid draw callback; retain its retail zero-argument definition. */
void sdfNoOpGridDrawCallback(void) {
}

/* Allocate a grid header followed by its cells, install the no-op callbacks
 * (or the caller's destroy hook), and start with every cell released. */
SdfGrid *sdfGridCreate(s32 width, s32 height, u16 cellWidth, u16 cellHeight, u16 visibleColumns,
                       u16 visibleRows, u32 userData, void (*onDestroy)(s32, u32)) {
    u32 cellCount = width * height;
    s32 size = sizeof(SdfGrid) + cellCount * sizeof(SdfGridCell);
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(size);
    SdfGrid *grid = (SdfGrid *)sdfMemoryGetBlockAddress(allocation);

    memset(grid, 0, size);
    grid->allocation = allocation;
    grid->width = width;
    grid->cellCount = cellCount;
    grid->viewportOrigin = (SdfGridCell *)(grid + 1);
    grid->cursor = (SdfGridCell *)(grid + 1);
    grid->cells = (SdfGridCell *)(grid + 1);
    grid->releaseCell = (void *)sdfNoOpKeyValueCallback;
    grid->onDestroy = (void *)sdfNoOpKeyValueCallback;
    grid->drawCell = (void *)sdfNoOpGridDrawCallback;
    grid->userData = 0;
    grid->cellWidth = cellWidth;
    grid->cellHeight = cellHeight;
    grid->visibleColumns = visibleColumns;
    grid->visibleRows = visibleRows;
    grid->columnMargin = 0;
    grid->rowMargin = 0;
    if (userData != 0) {
        grid->userData = userData;
    }
    if (onDestroy != NULL) {
        grid->onDestroy = onDestroy;
    }
    sdfGridReleaseAllCells(grid);
    return grid;
}

/* Set horizontal/vertical cursor margins without changing the viewport itself. */
void sdfSetShortPairValues(SdfGrid *grid, s32 columnMargin, s32 rowMargin) {
    grid->columnMargin = columnMargin;
    grid->rowMargin = rowMargin;
}

/* Release cells, invoke onDestroy(0, userData), then release the grid header allocation. */
void sdfDestroyGridWork(SdfGrid *owner) {
    if (owner != NULL) {
        sdfGridReleaseAllCells(owner);
        owner->onDestroy(0, owner->userData);
        sdfReleaseResourceAllocation(owner->allocation);
    }
}

/* Release all cell values and restore indices, retaining the grid header. */
void sdfGridClear(SdfGrid *grid) {
    sdfGridReleaseAllCells(grid);
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
void sdfGridGetCursorCoordinates(SdfGrid *grid, u32 *column, u32 *row) {
    SdfGridCell *cell = grid->cursor;
    *column = cell->index % grid->width;
    *row = cell->index / grid->width;
}

u32 sdfGridGetCellValue(SdfGrid *grid, s32 column, s32 row) {
    u32 cellIndex;

    cellIndex = row * grid->width + column;
    if (cellIndex >= grid->cellCount) {
        return 0;
    }
    return grid->cells[cellIndex].value;
}

void sdfGridSetCellValue(SdfGrid *grid, s32 column, s32 row, u32 value) {
    u32 cellIndex;

    cellIndex = row * grid->width + column;
    if (cellIndex < grid->cellCount) {
        grid->cells[cellIndex].value = value;
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
    sdfGridFollowCursorWithinMargins(grid);
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
    sdfGridFollowCursorWithinMargins(grid);
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
    sdfGridFollowCursorWithinMargins(grid);
    return cell;
}

SdfGridCell *sdfGridCursorRight(SdfGrid *grid) {
    SdfGridCell *cell = grid->cursor;
    u32 width = grid->width;
    u32 index = cell->index;
    cell += 1;
    if (index % width == width - 1) {
        return NULL;
    }
    grid->cursor = cell;
    sdfGridFollowCursorWithinMargins(grid);
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
    sdfGridFollowCursorWithinMargins(grid);
    return grid->cursor;
}

extern const SdfGridDirections D_004388E8[];

/* Select an occupied cell above and follow its viewport margins, or return NULL. */
SdfGridCell *sdfGridCursorUpToFilledCell(SdfGrid *grid) {
    SdfGridDirections directions = D_004388E8[0];
    u32 cursorValue;
    s32 row;
    s32 column;
    s32 radius;
    s32 depth;
    s32 direction;
    s32 candidateColumn;

    cursorValue = (u32)grid->cursor;
    cursorValue = ((SdfGridCell *)cursorValue)->index;
    if (cursorValue < grid->width) {
        return NULL;
    }
    row = cursorValue / grid->width;
    column = cursorValue % grid->width;
    radius = 0;
    while (1) {
        radius++;
        if (radius >= 6) {
            return NULL;
        }
        if (row - 1 < 0) {
            continue;
        }
        for (depth = 1; row - depth >= 0; depth++) {
            for (direction = 0; direction < 3; direction++) {
                candidateColumn = column + directions.columns[direction] * radius;
                if ((u32)candidateColumn < grid->width && candidateColumn >= 0) {
                    cursorValue = (u32)sdfGridGetCell(grid, candidateColumn, row - depth);
                    if (((SdfGridCell *)cursorValue)->value != 0) {
                        grid->cursor = (SdfGridCell *)cursorValue;
                        sdfGridFollowCursorWithinMargins(grid);
                        return grid->cursor;
                    }
                }
            }
        }
    }
}

extern const SdfGridDirections D_004388F0[];

/* Select an occupied cell below and follow its viewport margins, or return NULL. */
SdfGridCell *sdfGridCursorDownToFilledCell(SdfGrid *grid) {
    SdfGridDirections directions = D_004388F0[0];
    u32 cursorValue;
    u32 count = grid->cellCount;
    u32 width = grid->width;
    u32 rows;
    u32 cursorRow;
    u32 cursorColumn;
    s32 radius;
    s32 offset;
    s32 direction;

    cursorValue = (u32)grid->cursor;
    cursorValue = ((SdfGridCell *)cursorValue)->index;
    if (cursorValue >= count - width) {
        return NULL;
    }
    rows = count / width;
    cursorRow = cursorValue / width;
    cursorColumn = cursorValue % width;
    radius = 0;
    for (;;) {
        u32 row;
        radius++;
        if (radius >= 6) {
            return NULL;
        }
        offset = 1;
        while (cursorRow + offset < rows) {
            row = cursorRow + offset;
            direction = 0;
            do {
                s32 column = directions.columns[direction] * radius + cursorColumn;
                direction++;
                if ((u32)column >= grid->width) {
                    continue;
                }
                if (column < 0) {
                    continue;
                }
                cursorValue = (u32)sdfGridGetCell(grid, column, row);
                if (((SdfGridCell *)cursorValue)->value != 0) {
                    grid->cursor = (SdfGridCell *)cursorValue;
                    sdfGridFollowCursorWithinMargins(grid);
                    return grid->cursor;
                }
            } while (direction < 3);
            offset++;
        }
    }
}

extern const SdfGridDirections D_004388F8[];

/* Select an occupied cell to the left and follow its viewport margins, or return NULL. */
SdfGridCell *sdfGridCursorLeftToFilledCell(SdfGrid *grid) {
    SdfGridDirections directions = D_004388F8[0];
    SdfGridCell *cell;
    u32 index;
    u32 column;
    u32 row;
    s32 radius;
    s32 distance;
    s32 direction;
    s32 candidateRow;

    cell = grid->cursor;
    index = cell->index;
    column = index % grid->width;
    if (column == 0) {
        return NULL;
    }
    row = index / grid->width;
    radius = 0;
    do {
        radius++;
        if (radius >= 6) {
            return NULL;
        }
        for (distance = 1; (s32)(column - distance) >= 0; distance++) {
            for (direction = 0; direction < 3; direction++) {
                candidateRow = row + directions.columns[direction] * radius;
                if ((u32)candidateRow < grid->cellCount / grid->width && candidateRow >= 0) {
                    cell = sdfGridGetCell(grid, column - distance, candidateRow);
                    if (cell->value != 0) {
                        grid->cursor = cell;
                        sdfGridFollowCursorWithinMargins(grid);
                        return grid->cursor;
                    }
                }
            }
        }
    } while (1);
}

extern const SdfGridDirections D_00438900[];

/* Select an occupied cell to the right and follow its viewport margins, or return NULL. */
SdfGridCell *sdfGridCursorRightToFilledCell(SdfGrid *grid) {
    SdfGridDirections directions = D_00438900[0];
    SdfGridCell *cell;
    u32 cursorValue;
    u32 cursorColumn;
    u32 cursorRow;
    s32 rowDistance;

    cursorValue = (u32)grid->cursor;
    cursorValue = ((SdfGridCell *)cursorValue)->index;
    cursorColumn = cursorValue % grid->width;
    if (cursorColumn == grid->width - 1) {
        return NULL;
    }
    cursorRow = cursorValue / grid->width;
    rowDistance = 0;
    for (;;) {
        s32 columnOffset;
        if (++rowDistance >= 6) {
            return NULL;
        }
        for (columnOffset = 1; cursorColumn + columnOffset < grid->width; columnOffset++) {
            s32 directionIndex;
            for (directionIndex = 0; directionIndex < 3; directionIndex++) {
                s32 row = (s32)cursorRow + directions.columns[directionIndex] * rowDistance;
                if ((u32)row < grid->cellCount / grid->width && row >= 0) {
                    cell = sdfGridGetCell(grid, cursorColumn + columnOffset, row);
                    if (cell->value != 0) {
                        grid->cursor = cell;
                        sdfGridFollowCursorWithinMargins(grid);
                        return grid->cursor;
                    }
                }
            }
        }
    }
}

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
    sdfGridFollowCursorWithinMargins(grid);
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

void sdfGridFollowCursorWithinMargins(SdfGrid *grid) {
    SdfGridCell *origin = grid->viewportOrigin;
    SdfGridCell *cursor = grid->cursor;
    u32 width = grid->width;
    s32 column = origin->index % width;
    s32 row = origin->index / width;
    s32 cursorColumn = cursor->index % width;
    s32 cursorRow = cursor->index / width;
    s32 nextRow = row;
    if (cursorColumn < column + grid->columnMargin) {
        column -= (column + grid->columnMargin) - cursorColumn;
        if (column < 0) {
            column = 0;
        }
    } else {
        if (cursorColumn >= (column + grid->visibleColumns) - grid->columnMargin) {
            column = column + cursorColumn + (grid->columnMargin - (column + grid->visibleColumns)) + 1;
            column = (u32)column > width - grid->visibleColumns ?
                width - grid->visibleColumns : column;
        }
    }
    if (cursorRow < row + grid->rowMargin) {
        nextRow = row - ((row + grid->rowMargin) - cursorRow);
        if (nextRow < 0) {
            nextRow = 0;
        }
    } else {
        if (cursorRow >= (row + grid->visibleRows) - grid->rowMargin) {
            nextRow = row + cursorRow + (grid->rowMargin - (row + grid->visibleRows)) + 1;
            if ((u32)nextRow >= grid->cellCount / width - grid->visibleRows) {
                nextRow = grid->cellCount / width - grid->visibleRows;
            }
        }
    }
    grid->viewportOrigin = &grid->cells[nextRow * width + column];
}


float sdfMultiplyAddFloat(float addend, float multiplicand, float multiplier) {
    return addend + multiplicand * multiplier;
}

s32 sdfDefaultTaskEntryInit(void) {
    return 0;
}

void sdfNoOpKeyValueCallback(s32 key, s32 initResult) {
}

s32 sdfDefaultTaskEntryUpdate(s32 key, s32 initResult) {
    return 0;
}
