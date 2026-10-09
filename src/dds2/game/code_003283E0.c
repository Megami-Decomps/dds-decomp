#include "common.h"
#include "sdf_resource.h"
#include "sdf_chip.h"


extern void *sdfAllocateBlockBySizeThreshold(s32);

typedef struct SdfHandlerNode {
    struct SdfHandlerNode *next;
    s32 kind;
    s32 channel;
    s32 handlerId;
} SdfHandlerNode;

extern SdfHandlerNode *D_004389BC;

extern s32 func_00366780(s32);
extern s32 RemoveIntcHandler(s32, s32);
extern s32 RemoveDmacHandler(s32, s32);
extern s32 RemoveSbusIntcHandler(s32);

extern void (*sdfTickCallback)(void);


void *sdfAllocateBlockBySizeThreshold(s32 size) {
    if (size >= 0x401) {
        return (void *)sdfResourceRetainAddress(sdfAllocGeneralBlock(size));
    }
    return sdfAllocSizeClassBlock(size);
}

extern void sdfReleaseChipBlock(void *);
extern void sdfReleaseCurrentResourceHandle(void *);

void sdfFreeMemoryFromEitherHeap(void *data) {
    if (data != NULL) {
        if (sdfChipIsInRange((s32)data)) {
            sdfReleaseChipBlock(data);
            return;
        }
        sdfReleaseCurrentResourceHandle(data);
    }
}

void sdfReleaseChipOrRetainedResource(void *data) {
    if (data != NULL) {
        if (sdfChipIsInRange((s32)data)) {
            sdfQueuePendingChipRelease(data);
            return;
        }
        sdfQueueGeneralAllocationRelease(sdfFindGeneralBlockByAddress(data));
    }
}

void sdfFreeMemorySlotFromEitherHeap(void **slot) {
    void *data = *slot;
    if (data != NULL) {
        *slot = NULL;
        if (sdfChipIsInRange((s32)data)) {
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

void func_00328668(SdfHandlerNode *target) {
    SdfHandlerNode **link = &D_004389BC;
    SdfHandlerNode *node = *link;

    while (node != NULL) {
        if (node == target) {
            *link = node->next;
            switch (node->kind) {
            case 0:
                func_00366780(node->channel);
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
    while ((current = D_004389BC) != 0) {
        func_00328668(current);
    }
}
