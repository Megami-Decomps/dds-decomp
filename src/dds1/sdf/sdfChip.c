#include "sdk_ps2.h"
#include "common.h"
#include "sdf_chip.h"
#include "ee_mmi.h"
#include "sdf.h"
#include "sdf_chip_page_link_view.h"
#include "sdf_semaphore_api.h"
#include "sdf_thread.h"
#include "sdf_thread_sleep.h"
#include "sdf_handler_node.h"
#include "sdf_chip_range_api.h"
#include "sdf_heap_api.h"
#include "sdf_pending.h"
#include "sdf_memory_api.h"
#include "sdf_general_heap_api.h"

/* Promote the next list node to current; an empty list clears current. */
void sdfChipAllocNextPage(sdfChipCB_tag *sizeClass) {
    SdfChipPage *nextPage;

    nextPage = sizeClass->availablePages;
    if (nextPage != NULL) {
        sizeClass->availablePages = SDF_CHIP_PAGE_LINK_VIEW(nextPage)->nextInSizeClassAvailableList;
    }
    sizeClass->currentPage = nextPage;
}

/* Pop a recycled cell, count it live, and advance when its page becomes full. */
SdfChipCell *sdfChipAllocGetFree(sdfChipCB_tag *sizeClass, SdfChipPage *page) {
    SdfChipCell *freeCell = page->freeCells;
    page->usedCells++;
    page->freeCells = freeCell->nextFree;
    if (page->usedCells == sizeClass->cellCount) {
        sdfChipAllocNextPage(sizeClass);
    }
    return freeCell;
}

SdfChipCell *sdfChipAllocPageGrow(sdfChipCB_tag *sizeClass, SdfChipPage *page) {
    s16 cellCount = sizeClass->cellCount;
    s32 usedCells = (u16)page->usedCells + 1;
    s32 remaining = page->bumpCellsRemaining;
    SdfChipCell *cell;

    page->usedCells = usedCells;
    cell = (SdfChipCell *)(page->base + (cellCount - remaining) * sizeClass->cellSize);
    page->bumpCellsRemaining = remaining - 1;
    if (page->bumpCellsRemaining == 0) {
        page->allocate = sdfChipAllocGetFree;
        if ((s16)usedCells == cellCount) {
            sdfChipAllocNextPage(sizeClass);
        }
    }
    return cell;
}

SdfChipCell *sdfChipAllocNewPage(sdfChipCB_tag *sizeClass) {
    SdfChipPage *page = sdfChipPageFreeLink;

    sdfChipPageFreeLink = SDF_CHIP_PAGE_LINK_VIEW(page)->nextInFreePagePool;
    page->usedCells = 0;
    page->allocate = sdfChipAllocPageGrow;
    sizeClass->currentPage = page;
    page->sizeClass = sizeClass;
    page->bumpCellsRemaining = sizeClass->cellCount;
    return sdfChipAllocPageGrow(sizeClass, page);
}

/* Allocate a block of `size` bytes from the size class that covers it (classes are powers of two from 16 bytes up); each class hands out from its own slot list. */
void *sdfChipAllocDirect(s32 size) {
    s32 index = 0;
    sdfChipCB_tag *sizeClass;
    SdfChipCell *result;
    s32 interruptsDisabled;

    if (size > 16) {
        EE_MMI_PLZCW(index, size - 1);
        index = 27 - (index & 0xFF);
    }
    sizeClass = &sdfChipCB.classes[index];
    interruptsDisabled = DIntr();
    if (sizeClass->currentPage == NULL) {
        result = sdfChipAllocNewPage(sizeClass);
    } else {
        result = sizeClass->currentPage->allocate(sizeClass, sizeClass->currentPage);
    }
    if (interruptsDisabled != 0) {
        EIntr();
    }
    return result;
}

SdfChipPage *sdfChipPageInfo __attribute__((section(".sbss"), aligned(8)));

SdfChipPage *sdfChipPageFreeLink __attribute__((section(".sbss")));

u8 *sdfChipPageTop __attribute__((section(".sbss")));

u8 *sdfChipPageEnd __attribute__((section(".sbss")));

s32 sdfChipPageCount[2] __attribute__((section(".sbss")));

SdfPendingRequest dds3CleanupList __attribute__((section(".sbss"), aligned(8)));

SdfChipClassTable sdfChipCB __attribute__((section(".bss"), aligned(8)));

void sdfChipAllocNextPage(sdfChipCB_tag *sizeClass);

void *sdfChipAllocZeroDirect(s32 size) {
    return sdfZeroMemQuad(sdfChipAllocDirect(size), (size + 15) >> 4);
}

void sdfChipFreeDirect(void *memory) {
    SdfChipPage *page;
    sdfChipCB_tag *sizeClass;
    SdfChipCell *cell = memory;
    SdfChipPage **link;
    s32 interrupts;
    s16 count;
    s32 heapByteOffset;

    if (memory == NULL) {
        return;
    }
    heapByteOffset = (s32)memory - (s32)sdfChipPageTop;
    if (heapByteOffset < 0) {
        heapByteOffset += 0xFFF;
    }
    page = (SdfChipPage *)((u8 *)sdfChipPageInfo + (heapByteOffset >> 12) * sizeof(SdfChipPage));
    sizeClass = page->sizeClass;
    interrupts = DIntr();
    count = page->usedCells;
    page->usedCells = count - 1;
    if (page->usedCells == 0) {
        link = &sizeClass->availablePages;
        if (sizeClass->currentPage == page) {
            sdfChipAllocNextPage(sizeClass);
        } else {
            while (*link != page) {
                link = &SDF_CHIP_PAGE_LINK_VIEW(*link)->nextInSizeClassAvailableList;
            }
            *link = SDF_CHIP_PAGE_LINK_VIEW(page)->nextInSizeClassAvailableList;
        }
        page->sizeClass = NULL;
        SDF_CHIP_PAGE_LINK_VIEW(page)->nextInFreePagePool = sdfChipPageFreeLink;
        sdfChipPageFreeLink = page;
    } else {
        if (count == sizeClass->cellCount) {
            SDF_CHIP_PAGE_LINK_VIEW(page)->nextInSizeClassAvailableList = sizeClass->availablePages;
            sizeClass->availablePages = page;
        }
        cell->nextFree = page->freeCells;
        page->freeCells = cell;
    }
    if (interrupts != 0) {
        EIntr();
    }
}

void sdfChipFreeDelayedDirect(void *memory) {
    sdfDelayedResrcEntry(&dds3CleanupList, memory);
}

SdfChipAddressRangeResult sdfChipCheck(const void *address) {
    /* Keep the original signed EE address comparison after accepting a pointer. */
    s32 addressWord = (s32)address;
    SdfChipAddressRangeResult inRange;

    inRange = SDF_CHIP_ADDRESS_OUTSIDE_RANGE;
    if (addressWord >= (s32)sdfChipPageTop) {
        inRange = addressWord < (s32)sdfChipPageEnd;
    }
    return inRange;
}

#define SDF_CHIP_BLOCK_SHIFT 12

#define SDF_CHIP_BLOCK_BYTES (1 << SDF_CHIP_BLOCK_SHIFT)

extern void *malloc(u32 size);

/* Allocate the chip heap and page records, initialize free pages and size classes. */
void sdfChipInit(u32 heapSize) {
    u32 recordBytes;
    s32 alignment;
    s32 remaining;
    void *allocation;
    SdfChipPage *record;
    SdfChipPage *next;
    u8 *recordEnd;
    u8 *heapStart;
    u8 *page;
    s32 pageCount;
    sdfChipCB_tag *sizeClass;
    s32 unitSize;
    s32 cellCount;

    allocation = malloc(heapSize);
    sdfChipPageInfo = allocation;
    recordBytes = (heapSize / 0x1018U) * sizeof(SdfChipPage);
    recordEnd = (u8 *)allocation + recordBytes;
    record = allocation;
    sdfChipPageFreeLink = record;
    alignment = -(s32)recordEnd & 0x3F;
    remaining = (heapSize - recordBytes) - alignment;
    heapStart = recordEnd + alignment;
    sdfChipPageTop = heapStart;
    pageCount = remaining / SDF_CHIP_BLOCK_BYTES;
    sdfChipPageCount[0] = pageCount;
    sdfChipPageEnd = heapStart + pageCount * SDF_CHIP_BLOCK_BYTES;
    page = heapStart;
    do {
        next = record + 1;
        pageCount--;
        record->base = page;
        page += SDF_CHIP_BLOCK_BYTES;
        record->sizeClass = NULL;
        SDF_CHIP_PAGE_LINK_VIEW(record)->nextInFreePagePool = next;
        record = next;
    } while (pageCount != 0);
    SDF_CHIP_PAGE_LINK_VIEW(&next[-1])->nextInFreePagePool = NULL;

    sizeClass = sdfChipCB.classes;
    unitSize = 0x10;
    cellCount = 0x100;
    pageCount = SDF_CHIP_CLASS_COUNT;
    do {
        pageCount--;
        sizeClass->cellSize = unitSize;
        sizeClass->cellCount = cellCount;
        cellCount >>= 1;
        sizeClass->currentPage = NULL;
        unitSize <<= 1;
        sizeClass->availablePages = NULL;
        sizeClass++;
    } while (pageCount != 0);
    sdfDelayedResrcInitCB(&dds3CleanupList, sdfChipFreeDirect);
}

/* Fill `stats` with the chip heap's block totals and per-size-class usage. */
void sdfChipGetInfo(SdfChipHeapStats *stats) {
    SdfChipPage *block;
    s32 blocksRemaining;
    s32 classIndex;
    s32 partialBlocks;
    s32 emptyBlocks;
    s32 freeBytes;
    s32 freeCells;

    blocksRemaining = sdfChipPageCount[0];
    stats->blockCount = blocksRemaining;
    stats->totalBytes = blocksRemaining << SDF_CHIP_BLOCK_SHIFT;
    for (classIndex = 0; classIndex != SDF_CHIP_CLASS_COUNT; classIndex++) {
        stats->usedCells[classIndex] = 0;
    }
    block = sdfChipPageInfo;
    emptyBlocks = 0;
    partialBlocks = 0;
    freeBytes = 0;
    /* This post-tested walk assumes a nonzero heap block count. */
    do {
        if (block->sizeClass == NULL) {
            emptyBlocks++;
            freeBytes += SDF_CHIP_BLOCK_BYTES;
        } else {
            freeCells = block->sizeClass->cellCount - block->usedCells;
            if (freeCells != 0) {
                partialBlocks++;
                freeBytes += freeCells * block->sizeClass->cellSize;
            }
            stats->usedCells[block->sizeClass - sdfChipCB.classes] += block->usedCells;
        }
        block++;
    } while (--blocksRemaining != 0);
    stats->freeBytes = freeBytes;
    stats->emptyBlockCount = emptyBlocks;
    stats->partialBlockCount = partialBlocks;
}
