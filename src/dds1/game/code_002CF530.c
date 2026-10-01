#include "common.h"

extern void func_002D00B8(void *);
extern void *func_002D0A80(void *);
extern void sdfQueueNonzeroResourceId(void *);

extern s32 sdfChipIsInRange(void *);
extern void sdfReleaseChipBlock(void *);
extern void sdfReleaseCurrentResourceHandle(void *);

extern void (*D_003BD2D4)(void);

extern void *sdfResourceRetainAddress(void *);

extern void *func_002D03F8(void);

extern void *func_002CFEB8();

extern u32 D_003BD2CC;

void *sdfAllocateBlockBySizeThreshold(s32 size) {
    if (size >= 0x401) {
        return sdfResourceRetainAddress(func_002D03F8());
    }
    return func_002CFEB8(size);
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
            func_002D00B8(data);
            return;
        }
        sdfQueueNonzeroResourceId(func_002D0A80(data));
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


void func_002CF670(const char *format, ...) {
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

