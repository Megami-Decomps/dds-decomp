#include "common.h"
#include "sdf.h"

SdfChipPage *sdfChipPages __attribute__((section(".sbss"), aligned(8)));
SdfChipPage *sdfFreeChipPages __attribute__((section(".sbss")));
u8 *sdfChipHeapStart __attribute__((section(".sbss")));
u8 *sdfChipHeapEnd __attribute__((section(".sbss")));
s32 sdfChipPageCount[2] __attribute__((section(".sbss")));
SdfPendingRequest sdfChipReleaseRequest __attribute__((section(".sbss"), aligned(8)));

SdfChipClassTable sdfChipClassTable __attribute__((section(".bss"), aligned(8)));

void *sdfAllocSizeClassBlock(s32 arg0);
void *sdfClearQuadwords(void *arg0, s32 arg1);
void sdfPendingQueuePush(void *arg0, s32 arg1);
s32 func_00312C08(void);
s32 EIntr(void);
void sdfAdvanceNodeCursor(SdfChipClass *sizeClass);

void *sdfAllocAndClearQuadwords(s32 size) {
    return sdfClearQuadwords(sdfAllocSizeClassBlock(size), (size + 15) >> 4);
}

void sdfReleaseChipBlock(void *memory) {
    SdfChipPage *page;
    SdfChipClass *sizeClass;
    SdfChipCell *cell = memory;
    SdfChipPage **link;
    s32 interrupts;
    s16 count;
    s32 index;

    if (memory == NULL) {
        return;
    }
    index = (s32)memory - (s32)sdfChipHeapStart;
    if (index < 0) {
        index += 0xFFF;
    }
    page = (SdfChipPage *)((u8 *)sdfChipPages + (index >> 12) * sizeof(SdfChipPage));
    sizeClass = page->sizeClass;
    interrupts = func_00312C08();
    count = page->usedCells;
    page->usedCells = count - 1;
    if (page->usedCells == 0) {
        link = &sizeClass->availablePages;
        if (sizeClass->currentPage == page) {
            sdfAdvanceNodeCursor(sizeClass);
        } else {
            while (*link != page) {
                link = &(*link)->next;
            }
            *link = page->next;
        }
        page->sizeClass = NULL;
        page->next = sdfFreeChipPages;
        sdfFreeChipPages = page;
    } else {
        if (count == sizeClass->cellCount) {
            page->next = sizeClass->availablePages;
            sizeClass->availablePages = page;
        }
        cell->nextFree = page->freeCells;
        page->freeCells = cell;
    }
    if (interrupts != 0) {
        EIntr();
    }
}

void sdfQueuePendingChipValue(s32 value) {
    sdfPendingQueuePush(&sdfChipReleaseRequest, value);
}

s32 sdfChipIsInRange(s32 address) {
    s32 inRange;

    inRange = 0;
    if (address >= (s32)sdfChipHeapStart) {
        inRange = address < (s32)sdfChipHeapEnd;
    }
    return inRange;
}
