#include "common.h"

extern void sdfQueuePendingChipValue(void *);
extern void *sdfFindGeneralBlockByAddress(void *);
extern void sdfQueueNonzeroResourceId(void *);

extern s32 sdfChipIsInRange(void *);
extern void sdfReleaseChipBlock(void *);
extern void sdfReleaseCurrentResourceHandle(void *);

extern void (*sdfTickCallback)(void);

extern void *sdfResourceRetainAddress(void *);

extern void *sdfAllocGeneralBlock(void);

extern void *sdfAllocSizeClassBlock();

extern u32 D_003BD2CC;

void *sdfAllocateBlockBySizeThreshold(s32 size) {
    if (size >= 0x401) {
        return sdfResourceRetainAddress(sdfAllocGeneralBlock());
    }
    return sdfAllocSizeClassBlock(size);
}

void sdfFreeMemoryFromEitherHeap(void *data) {
    if (data != NULL) {
        if (sdfChipIsInRange(data)) {
            sdfReleaseChipBlock(data);
            return;
        }
        sdfReleaseCurrentResourceHandle(data);
    }
}


void sdfReleaseChipOrRetainedResource(void *data) {
    if (data != NULL) {
        if (sdfChipIsInRange(data)) {
            sdfQueuePendingChipValue(data);
            return;
        }
        sdfQueueNonzeroResourceId(sdfFindGeneralBlockByAddress(data));
    }
}


void sdfFreeMemorySlotFromEitherHeap(void **slot) {
    void *data = *slot;
    if (data != NULL) {
        *slot = NULL;
        if (sdfChipIsInRange(data)) {
            sdfReleaseChipBlock(data);
            return;
        }
        sdfReleaseCurrentResourceHandle(data);
    }
}


void sdfPanicHaltPrintf(const char *format, ...) {
    for (;;) {
    }
}

INCLUDE_ASM(const s32, "game/code_002CF530", sdfAddHandler);

INCLUDE_ASM(const s32, "game/code_002CF530", func_002CF7B8);

void sdfDrainPendingHandlers(void) {
    u32 current;
    while ((current = D_003BD2CC) != 0) {
        func_002CF7B8(current);
    }
}

INCLUDE_SDATA(const s32, "game/code_002CF530", D_003BD2CC);

