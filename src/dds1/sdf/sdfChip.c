#include "common.h"
#include "libsdf.h"

#include "ps2asm.h"
/* A free chip carries its free-list link at byte 8. */
typedef struct sdfChip_t
{
    u8 pad00[8];
    void *nextFree;
} sdfChip_t;

#define SDF_CHIP_CLASS_COUNT 7
#define SDF_CHIP_PAGE_SHIFT 12
#define SDF_CHIP_PAGE_BYTES (1 << SDF_CHIP_PAGE_SHIFT)


extern void *malloc(u32 size);
extern s32 DIntr(void);
extern void EIntr(void);
extern void *sdfZeroMemQuad(void *ptr, s32 quadCount);
extern void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, sdfDelayedResrcFreeFunc aFreeFunc);
extern void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem);

extern sdfChipCB_tag sdfChipCB[SDF_CHIP_CLASS_COUNT];
extern sdfChipPageInfo_tag *sdfChipPageInfo;
extern sdfChipPageInfo_tag *sdfChipPageFreeLink;
extern u8 *sdfChipPageTop;
extern u8 *sdfChipPageEnd;
extern s32 sdfChipPageCount;

/* Declared ahead of the cleanup list so the definitions below keep retail's
 * .sbss order. */
extern s32 sdfChipPageCountSpan[2] FAKEMATCH_ASM_NAME("sdfChipPageCount");
extern sdfDelayedResrcCB_t dds3CleanupList;

void sdfChipAllocNextPage(sdfChipCB_tag *aCB);
sdfChip_t *sdfChipAllocGetFree(sdfChipCB_tag *aCB, sdfChipPageInfo_tag *aPageInfo);
sdfChip_t *sdfChipAllocPageGrow(sdfChipCB_tag *aCB, sdfChipPageInfo_tag *aPageInfo);
void *sdfChipAllocDirect(s32 aSize);
void sdfChipFreeDirect(void *aPtr);

/* Promote the next page on the partial list to current; an empty list clears it. */
void sdfChipAllocNextPage(sdfChipCB_tag *aCB)
{
    sdfChipPageInfo_tag *nextPage;

    nextPage = aCB->freeLink;
    if (nextPage != NULL) {
        aCB->freeLink = nextPage->next;
    }
    aCB->currentPage = nextPage;
}

/* Pop a recycled chip and advance when its page becomes full. */
sdfChip_t *sdfChipAllocGetFree(sdfChipCB_tag *aCB, sdfChipPageInfo_tag *aPageInfo)
{
    sdfChip_t *chip = aPageInfo->freeLink;

    aPageInfo->allocCount++;
    aPageInfo->freeLink = chip->nextFree;
    if (aPageInfo->allocCount == aCB->chipsPerPage) {
        sdfChipAllocNextPage(aCB);
    }
    return chip;
}

/* Hand out the next never-used chip; the last one switches the page to the
 * free-list handler. */
sdfChip_t *sdfChipAllocPageGrow(sdfChipCB_tag *aCB, sdfChipPageInfo_tag *aPageInfo)
{
    s16 chipsPerPage = aCB->chipsPerPage;
    s32 allocCount = (u16)aPageInfo->allocCount + 1;
    s32 growLeft = aPageInfo->growLeft;
    sdfChip_t *chip;

    aPageInfo->allocCount = allocCount;
    chip = (sdfChip_t *)((u8 *)aPageInfo->page + (chipsPerPage - growLeft) * aCB->chipSize);
    aPageInfo->growLeft = growLeft - 1;
    if (aPageInfo->growLeft == 0) {
        aPageInfo->alloc = (void *)sdfChipAllocGetFree;
        if ((s16)allocCount == chipsPerPage) {
            sdfChipAllocNextPage(aCB);
        }
    }
    return chip;
}

sdfChip_t *sdfChipAllocNewPage(sdfChipCB_tag *aCB)
{
    sdfChipPageInfo_tag *page = sdfChipPageFreeLink;

    sdfChipPageFreeLink = page->next;
    page->allocCount = 0;
    page->alloc = (void *)sdfChipAllocPageGrow;
    aCB->currentPage = page;
    page->owner = aCB;
    page->growLeft = aCB->chipsPerPage;
    return sdfChipAllocPageGrow(aCB, page);
}

/* Allocate from the size class that covers the request; classes are powers of
 * two from 16 bytes up, and each page hands out from its own chip list. */
void *sdfChipAllocDirect(s32 aSize)
{
    s32 index = 0;
    sdfChipCB_tag *cb;
    sdfChip_t *chip;
    s32 intrEnabled;

    if (aSize > 16) {
        MMI_PLZCW(index, aSize - 1);
        index = 27 - (index & 0xFF);
    }
    cb = &sdfChipCB[index];
    intrEnabled = DIntr();
    if (cb->currentPage == NULL) {
        chip = sdfChipAllocNewPage(cb);
    } else {
        chip = ((sdfChip_t *(*)(sdfChipCB_tag *, sdfChipPageInfo_tag *))cb->currentPage->alloc)(
            cb, cb->currentPage);
    }
    if (intrEnabled != 0) {
        EIntr();
    }
    return chip;
}

void *sdfChipAllocZeroDirect(s32 aSize)
{
    return sdfZeroMemQuad(sdfChipAllocDirect(aSize), (aSize + 15) >> 4);
}

void sdfChipFreeDirect(void *aPtr)
{
    sdfChipPageInfo_tag *page;
    sdfChipCB_tag *cb;
    sdfChip_t *chip = aPtr;
    sdfChipPageInfo_tag **link;
    s32 intrEnabled;
    s16 allocCount;
    s32 heapByteOffset;

    if (aPtr == NULL) {
        return;
    }
    heapByteOffset = (s32)aPtr - (s32)sdfChipPageTop;
    if (heapByteOffset < 0) {
        heapByteOffset += SDF_CHIP_PAGE_BYTES - 1;
    }
    page = (sdfChipPageInfo_tag *)((u8 *)sdfChipPageInfo +
                                   (heapByteOffset >> SDF_CHIP_PAGE_SHIFT) *
                                       sizeof(sdfChipPageInfo_tag));
    cb = page->owner;
    intrEnabled = DIntr();
    allocCount = page->allocCount;
    page->allocCount = allocCount - 1;
    if (page->allocCount == 0) {
        link = &cb->freeLink;
        if (cb->currentPage == page) {
            sdfChipAllocNextPage(cb);
        } else {
            while (*link != page) {
                link = &(*link)->next;
            }
            *link = page->next;
        }
        page->owner = NULL;
        page->next = sdfChipPageFreeLink;
        sdfChipPageFreeLink = page;
    } else {
        if (allocCount == cb->chipsPerPage) {
            page->next = cb->freeLink;
            cb->freeLink = page;
        }
        chip->nextFree = page->freeLink;
        page->freeLink = chip;
    }
    if (intrEnabled != 0) {
        EIntr();
    }
}

void sdfChipFreeDelayedDirect(void *aPtr)
{
    sdfDelayedResrcEntry(&dds3CleanupList, aPtr);
}

s32 sdfChipCheck(s32 aPtr)
{
    s32 inRange;

    inRange = 0;
    if (aPtr >= (s32)sdfChipPageTop) {
        inRange = aPtr < (s32)sdfChipPageEnd;
    }
    return inRange;
}

/* Allocate the chip heap plus its page records, then build the free page list
 * and the size classes. */
void sdfChipInit(u32 aSize)
{
    u32 recordBytes;
    s32 align;
    s32 remaining;
    void *allocation;
    sdfChipPageInfo_tag *record;
    sdfChipPageInfo_tag *next;
    u8 *recordEnd;
    u8 *heapTop;
    u8 *page;
    s32 pageCount;
    sdfChipCB_tag *cb;
    s32 chipSize;
    s32 chipsPerPage;

    allocation = malloc(aSize);
    sdfChipPageInfo = allocation;
    recordBytes = (aSize / (SDF_CHIP_PAGE_BYTES + sizeof(sdfChipPageInfo_tag))) *
                  sizeof(sdfChipPageInfo_tag);
    recordEnd = (u8 *)allocation + recordBytes;
    record = allocation;
    sdfChipPageFreeLink = record;
    align = -(s32)recordEnd & 0x3F;
    remaining = (aSize - recordBytes) - align;
    heapTop = recordEnd + align;
    sdfChipPageTop = heapTop;
    pageCount = remaining / SDF_CHIP_PAGE_BYTES;
    sdfChipPageCount = pageCount;
    sdfChipPageEnd = heapTop + pageCount * SDF_CHIP_PAGE_BYTES;
    page = heapTop;
    do {
        next = record + 1;
        pageCount--;
        record->page = page;
        page += SDF_CHIP_PAGE_BYTES;
        record->owner = NULL;
        record->next = next;
        record = next;
    } while (pageCount != 0);
    next[-1].next = NULL;

    cb = sdfChipCB;
    chipSize = 0x10;
    chipsPerPage = 0x100;
    pageCount = SDF_CHIP_CLASS_COUNT;
    do {
        pageCount--;
        cb->chipSize = chipSize;
        cb->chipsPerPage = chipsPerPage;
        chipsPerPage >>= 1;
        cb->currentPage = NULL;
        chipSize <<= 1;
        cb->freeLink = NULL;
        cb++;
    } while (pageCount != 0);
    sdfDelayedResrcInitCB(&dds3CleanupList, (sdfDelayedResrcFreeFunc)sdfChipFreeDirect);
}

void sdfChipGetInfo(sdfChipInfo_t *aInfo)
{
    sdfChipPageInfo_tag *page;
    s32 pagesLeft;
    s32 index;
    s32 fragmentedPageCount;
    s32 freePageCount;
    s32 freeSize;
    s32 freeChips;

    pagesLeft = sdfChipPageCount;
    aInfo->pageCount = pagesLeft;
    aInfo->total = pagesLeft << SDF_CHIP_PAGE_SHIFT;
    for (index = 0; index != SDF_CHIP_CLASS_COUNT; index++) {
        aInfo->severalAllocCount[index] = 0;
    }
    page = sdfChipPageInfo;
    freePageCount = 0;
    fragmentedPageCount = 0;
    freeSize = 0;
    /* This post-tested walk assumes a nonzero page count. */
    do {
        if (page->owner == NULL) {
            freePageCount++;
            freeSize += SDF_CHIP_PAGE_BYTES;
        } else {
            freeChips = page->owner->chipsPerPage - page->allocCount;
            if (freeChips != 0) {
                fragmentedPageCount++;
                freeSize += freeChips * page->owner->chipSize;
            }
            aInfo->severalAllocCount[page->owner - sdfChipCB] += page->allocCount;
        }
        page++;
    } while (--pagesLeft != 0);
    aInfo->freeSize = freeSize;
    aInfo->freePageCount = freePageCount;
    aInfo->fragmentedPageCount = fragmentedPageCount;
}

/* DDS's build defines the allocator state in this unit, after the code so that
 * a -G0 build does not address it through $gp. The page count and the class
 * table each own a four-byte tail of their retail span, so they are defined
 * under other C names with the symbol's own. */
sdfChipPageInfo_tag *sdfChipPageInfo __attribute__((section(".sbss"), aligned(8)));
sdfChipPageInfo_tag *sdfChipPageFreeLink __attribute__((section(".sbss")));
u8 *sdfChipPageTop __attribute__((section(".sbss")));
u8 *sdfChipPageEnd __attribute__((section(".sbss")));
s32 sdfChipPageCountSpan[2] FAKEMATCH_ASM_NAME("sdfChipPageCount") __attribute__((section(".sbss")));
sdfDelayedResrcCB_t dds3CleanupList __attribute__((section(".sbss"), aligned(8)));
struct
{
    sdfChipCB_tag classes[SDF_CHIP_CLASS_COUNT];
    u8 tail[4];
} sdfChipCBSpan FAKEMATCH_ASM_NAME("sdfChipCB") __attribute__((section(".bss"), aligned(8)));
