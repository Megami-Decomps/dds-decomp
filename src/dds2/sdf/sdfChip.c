#include "sdk_ps2.h"
#include "common.h"
#include "sdf_chip.h"
#include "sdf_chip_range_api.h"
#include "sdf_resource.h"
#include "sdf.h"
#include "sdf_chip_page_link_view.h"
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
    void *allocation;

    allocation = sdfChipAllocDirect(size);
    return sdfZeroMemQuad(allocation, (size + 0xf) >> 4);
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

SdfChipAddressRangeResult sdfChipIsInRange(const void *address) {
    /* Keep the original signed EE address comparison after accepting a pointer. */
    s32 addressWord = (s32)address;
    SdfChipAddressRangeResult withinRange;

    withinRange = SDF_CHIP_ADDRESS_OUTSIDE_RANGE;
    if (addressWord >= (s32)sdfChipPageTop) {
        withinRange = addressWord < (s32)sdfChipPageEnd;
    }
    return withinRange;
}
