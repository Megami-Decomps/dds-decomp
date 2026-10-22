#include "sdk_ps2.h"
#include "common.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "sdf.h"
#include "sdf_pending.h"
#include "sdf_memory_api.h"

SdfChipPage *sdfChipPageInfo __attribute__((section(".sbss"), aligned(8)));
SdfChipPage *sdfChipPageFreeLink __attribute__((section(".sbss")));
u8 *sdfChipPageTop __attribute__((section(".sbss")));
u8 *sdfChipPageEnd __attribute__((section(".sbss")));
s32 sdfChipPageCount[2] __attribute__((section(".sbss")));
SdfPendingRequest dds3CleanupList __attribute__((section(".sbss"), aligned(8)));

SdfChipClassTable sdfChipCB __attribute__((section(".bss"), aligned(8)));

void sdfChipAllocNextPage(sdfChipCB_tag *sizeClass);

void *sdfAllocAndClearQuadwords(s32 size) {
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
                link = &(*link)->next;
            }
            *link = page->next;
        }
        page->sizeClass = NULL;
        page->next = sdfChipPageFreeLink;
        sdfChipPageFreeLink = page;
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

void sdfChipFreeDelayedDirect(void *memory) {
    sdfDelayedResrcEntry(&dds3CleanupList, memory);
}

s32 sdfChipCheck(s32 address) {
    s32 inRange;

    inRange = 0;
    if (address >= (s32)sdfChipPageTop) {
        inRange = address < (s32)sdfChipPageEnd;
    }
    return inRange;
}
