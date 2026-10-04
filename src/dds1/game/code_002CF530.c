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

typedef struct SdfHandlerNode {
    struct SdfHandlerNode *next;
    s32 kind;
    s32 channel;
    s32 handlerId;
} SdfHandlerNode;

extern SdfHandlerNode *D_003BD2CC;

extern s32 func_0030B500(s32);
extern s32 RemoveIntcHandler(s32, s32);
extern s32 RemoveDmacHandler(s32, s32);
extern s32 RemoveSbusIntcHandler(s32);

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

void func_002CF7B8(SdfHandlerNode *target) {
    SdfHandlerNode **link = &D_003BD2CC;
    SdfHandlerNode *node = *link;

    while (node != NULL) {
        if (node == target) {
            *link = node->next;
            switch (node->kind) {
            case 0:
                func_0030B500(node->channel);
                RemoveIntcHandler(node->channel, node->handlerId);
                break;
            case 1:
                RemoveDmacHandler(node->channel, node->handlerId);
                break;
            case 2:
                RemoveSbusIntcHandler(node->channel);
                break;
            }
            sdfFreeMemoryFromEitherHeap(target);
            return;
        }
        /* Non-head handles are followed through their own link. */
        link = &target->next;
        node = *link;
    }
}

void sdfDrainPendingHandlers(void) {
    SdfHandlerNode *current;
    while ((current = D_003BD2CC) != 0) {
        func_002CF7B8(current);
    }
}

INCLUDE_SDATA(const s32, "game/code_002CF530", D_003BD2CC);
