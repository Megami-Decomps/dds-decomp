#include "common.h"


extern s64 sdfAllocateBlockBySizeThreshold(s64);

extern u32 D_004389BC;

extern void (*D_004389C4)(void);

extern void *func_00328D68();
extern void *func_003292A8(void);
extern void sdfResourceRetainAddress(void *);

s64 sdfAllocateBlockBySizeThreshold(s64 size) {
    if (size >= 0x401) {
        sdfResourceRetainAddress(func_003292A8());
    } else {
        func_00328D68(size);
    }
}

extern s32 sdfChipIsInRange(void *);
extern void sdfReleaseChipBlock(void *);
extern void func_00329868(void *);
extern void func_00328F68(void *);
extern void *func_00329930(void *);
extern void sdfQueueNonzeroResourceId(void *);

void sdfFreeMemoryFromEitherHeap(void *data) {
    if (data != NULL) {
        if (sdfChipIsInRange(data)) {
            sdfReleaseChipBlock(data);
            return;
        }
        func_00329868(data);
    }
}

void sdfReleaseChipOrRetainedResource(void *data) {
    if (data != NULL) {
        if (sdfChipIsInRange(data)) {
            func_00328F68(data);
            return;
        }
        sdfQueueNonzeroResourceId(func_00329930(data));
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
        func_00329868(data);
    }
}

INCLUDE_ASM(const s32, "game/code_003283E0", func_00328520);

INCLUDE_ASM(const s32, "game/code_003283E0", sdfAddHandler);

INCLUDE_ASM(const s32, "game/code_003283E0", func_00328668);

void sdfDrainPendingHandlers(void) {
    u32 current;
    while ((current = D_004389BC) != 0) {
        func_00328668(current);
    }
}
