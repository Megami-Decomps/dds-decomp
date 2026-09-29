#include "common.h"

extern void func_002D00B8(void *);
extern void *func_002D0A80(void *);
extern void func_002D0A10(void *);

extern s32 sdfChipIsInRange(void *);
extern void func_002CFF98(void *);
extern void func_002D09B8(void *);

extern void (*D_003BD2D4)(void);

extern u64 func_002CF530(u64);

extern u32 D_003BD2CC;

INCLUDE_ASM(const s32, "game/code_002CF530", func_002CF530);

void sdfFreeMemoryFromEitherHeap(void *data) {
    if (data != NULL) {
        if (sdfChipIsInRange(data)) {
            func_002CFF98(data);
            return;
        }
        func_002D09B8(data);
    }
}


void func_002CF5C0(void *data) {
    if (data != NULL) {
        if (sdfChipIsInRange(data)) {
            func_002D00B8(data);
            return;
        }
        func_002D0A10(func_002D0A80(data));
    }
}


void sdfFreeMemorySlotFromEitherHeap(void **slot) {
    void *data = *slot;
    if (data != NULL) {
        *slot = NULL;
        if (sdfChipIsInRange(data)) {
            func_002CFF98(data);
            return;
        }
        func_002D09B8(data);
    }
}


INCLUDE_ASM(const s32, "game/code_002CF530", func_002CF670);

INCLUDE_ASM(const s32, "game/code_002CF530", sdfAddHandler);

INCLUDE_ASM(const s32, "game/code_002CF530", func_002CF7B8);

void sdfDrainPendingHandlers(void) {
    u32 current;
    while ((current = D_003BD2CC) != 0) {
        func_002CF7B8(current);
    }
}

INCLUDE_SDATA(const s32, "game/code_002CF530", D_003BD2CC);

