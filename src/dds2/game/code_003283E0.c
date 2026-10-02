#include "common.h"


extern void *sdfAllocateBlockBySizeThreshold(s32);

extern u32 D_004389BC;

extern void (*sdfTickCallback)(void);

extern void *sdfAllocSizeClassBlock();
extern void *sdfAllocGeneralBlock(void);
extern void *sdfResourceRetainAddress(void *);

void *sdfAllocateBlockBySizeThreshold(s32 size) {
    if (size >= 0x401) {
        return sdfResourceRetainAddress(sdfAllocGeneralBlock());
    }
    return sdfAllocSizeClassBlock(size);
}

extern s32 sdfChipIsInRange(void *);
extern void sdfReleaseChipBlock(void *);
extern void sdfReleaseCurrentResourceHandle(void *);
extern void sdfQueuePendingChipValue(void *);
extern void *sdfFindGeneralBlockByAddress(void *);
extern void sdfQueueNonzeroResourceId(void *);

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

INCLUDE_ASM(const s32, "game/code_003283E0", sdfAddHandler);

INCLUDE_ASM(const s32, "game/code_003283E0", func_00328668);

void sdfDrainPendingHandlers(void) {
    u32 current;
    while ((current = D_004389BC) != 0) {
        func_00328668(current);
    }
}
