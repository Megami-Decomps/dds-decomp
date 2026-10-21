#include "sdk_ps2.h"
#include "common.h"
#include "sdf_resource.h"
#include "sdf_chip.h"
#include "sdf_handler_node.h"

extern SdfHandlerNode *sdfHandlerNodeListHead;

extern s32 func_00366780(s32);

extern void (*sdfTickCallback)(void);


void *sdfAllocateBlockBySizeThreshold(s32 size) {
    if (size >= 0x401) {
        return (void *)sdfMemLock(sdfMemAllocDirect(size));
    }
    return sdfChipAllocDirect(size);
}

extern void sdfMemFreeHandleDirect(void *);

void sdfFreeMemoryFromEitherHeap(void *allocation) {
    if (allocation != NULL) {
        if (sdfChipIsInRange((s32)allocation)) {
            sdfChipFreeDirect(allocation);
            return;
        }
        sdfMemFreeHandleDirect(allocation);
    }
}

void sdfReleaseChipOrRetainedResource(void *allocation) {
    if (allocation != NULL) {
        if (sdfChipIsInRange((s32)allocation)) {
            sdfQueuePendingChipRelease(allocation);
            return;
        }
        sdfMemFreeDelayedDirect(sdfFindGeneralBlockByAddress(allocation));
    }
}

void sdfFreeMemorySlotFromEitherHeap(void **slot) {
    void *allocation = *slot;
    if (allocation != NULL) {
        *slot = NULL;
        if (sdfChipIsInRange((s32)allocation)) {
            sdfChipFreeDirect(allocation);
            return;
        }
        sdfMemFreeHandleDirect(allocation);
    }
}

void sdfPanicHaltPrintf(const char *format, ...) {
    for (;;) {
    }
}

extern void sdfPanicHaltPrintf(const char *format, ...) __attribute__((noreturn));
extern s32 func_00365990(s32, s32, s32, s32);
extern s32 func_003659C0(s32, s32, s32, s32);

SdfHandlerNode *sdfAddHandler(s32 kind, s32 channel, s32 handler, s32 arg, s32 next) {
    SdfHandlerNode *node;
    s32 id;

    switch (kind) {
    case 0:
        id = func_00365990(channel, handler, arg, next);
        break;
    case 1:
        id = func_003659C0(channel, handler, arg, next);
        break;
    case 2:
        id = AddSbusIntcHandler(channel, handler);
        break;
    default:
        sdfPanicHaltPrintf("sdfAddHandler : unknown type %d\n", kind);
        break;
    }
    node = sdfAllocateBlockBySizeThreshold(sizeof(*node));
    node->handlerKind = kind;
    node->channel = channel;
    node->handlerId = id;
    node->next = sdfHandlerNodeListHead;
    sdfHandlerNodeListHead = node;
    return node;
}

void sdfRemoveHandler(SdfHandlerNode *target) {
    SdfHandlerNode **link = &sdfHandlerNodeListHead;
    SdfHandlerNode *node = *link;

    while (node != NULL) {
        if (node == target) {
            *link = node->next;
            switch (node->handlerKind) {
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
    while ((current = sdfHandlerNodeListHead) != 0) {
        sdfRemoveHandler(current);
    }
}
