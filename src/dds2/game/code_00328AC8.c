#include "sdk_ps2.h"
#include "common.h"
#include "sdf_handler_node.h"
#include "sdf_chip.h"
#include "ee_mmi.h"
#include "sdf.h"
#include "sdf_chip_page_link_view.h"
#include "sdf_semaphore_api.h"
#include "sdf_thread.h"
#include "sdf_thread_sleep.h"

#define SDF_CURRENT_THREAD_SELECTOR 0xffffffffffffffff
#define SDF_THREAD_COMPLETION_EVENT 2
#define SDF_WAKE_WORKER_STACK_BYTES 0x800
#define SDF_WAKE_WORKER_PRIORITY 0x3E
#define SDF_SLEEP_THREAD_PRIORITY 0x7C

s32 sdfVblankFlag __attribute__((section(".sdata"), aligned(8))) = 0;

extern void sdfDeleteTrackedThread(SdfThreadNode *node);

/* The negative lookup selector selects the calling thread's tracked entry. */
void sdfUnlinkAndDeleteCurrentThread(void) {
    SdfThreadNode *threadNode;

    threadNode = sdfFindThreadNode(SDF_CURRENT_THREAD_SELECTOR);
    sdfDeleteTrackedThread(threadNode);
}

extern s32 sdfVblankSema;
extern u8 D_00438B70;

/* Wake the worker only for its registered event; the byte counter never underflows. */
s32 VblankIntcHandler(s32 eventId) {
    if (eventId == SDF_THREAD_COMPLETION_EVENT) {
        iWakeupThread(sdfVblankSema);
        if (D_00438B70 != 0) {
            D_00438B70--;
        }
    }
    return 0;
}

extern SdfHandlerNode *sdfAddHandler(SdfHandlerKind, s32, SdfHandlerCallback, s32, s32);

/* Each wake-up broadcasts to every tracked thread under the list semaphore. */
void VblankThreadProc(void) {
    SdfThreadNode *threadEntry;

    sdfAddHandler(SDF_HANDLER_KIND_INTERRUPT, SDF_THREAD_COMPLETION_EVENT, VblankIntcHandler, -1, 0);
    EnableIntc(SDF_THREAD_COMPLETION_EVENT);
    for (;;) {
        SleepThread();
        sdfVblankFlag++;
        WaitSema(sdfThreadSema);
        for (threadEntry = sdfVblankThreadId; threadEntry != NULL; threadEntry = threadEntry->next) {
            WakeupThread(threadEntry->threadId);
        }
        SignalSema(sdfThreadSema);
    }
}

extern void InitThread(void);
extern s32 CreateThreadSimple(void (*entryAddress)(), void *workspace, s32 stackBytes, s32 priority);
extern char D_0045E8A0[];

/* Initialize the tracked-thread list and its semaphore before starting the wake worker. */
s32 sdfInitThreadSystem(void) {
    s32 threadId;

    InitThread();
    sdfVblankFlag = 0;
    sdfVblankThreadId = NULL;
    sdfThreadSema = sdfCreateSema(1, 1, 0);
    threadId = CreateThreadSimple(VblankThreadProc, D_0045E8A0, SDF_WAKE_WORKER_STACK_BYTES, SDF_WAKE_WORKER_PRIORITY);
    sdfVblankSema = threadId;
    return _StartThread(threadId, 0);
}

extern s32 sdfSleepThreadId;

/* Record the calling thread, set its sleep priority, then return the SDK sleep result. */
s32 sdfSetMainThreadPriority(void) {
    s32 threadId = GetThreadId();

    sdfSleepThreadId = threadId;
    ChangeThreadPriority(threadId, SDF_SLEEP_THREAD_PRIORITY);
    return SleepThread();
}

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
    s32 sizeClassIndex = 0;
    sdfChipCB_tag *sizeClass;
    SdfChipCell *result;
    s32 interruptsDisabled;

    if (size > 16) {
        EE_MMI_PLZCW(sizeClassIndex, size - 1);
        sizeClassIndex = 27 - (sizeClassIndex & 0xFF);
    }
    sizeClass = &sdfChipCB.classes[sizeClassIndex];
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
