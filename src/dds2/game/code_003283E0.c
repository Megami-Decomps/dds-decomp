#include "sdk_ps2.h"
#include "common.h"
#include "sdf_resource.h"
#include "sdf_chip.h"
#include "sdf_handler_node.h"

extern SdfHandlerNode *sdfDmacHandlerTable;

extern s32 DisableIntc(s32);

extern void (*sdfTickCallback)(void);


void *sdfAllocAuto(s32 size) {
    if (size >= 0x401) {
        return (void *)sdfMemLock(sdfMemAllocDirect(size));
    }
    return sdfChipAllocDirect(size);
}

extern void sdfMemFreePtrDirect(const void *);

void sdfFreeAuto(void *allocation) {
    if (allocation != NULL) {
        if (sdfChipIsInRange((s32)allocation)) {
            sdfChipFreeDirect(allocation);
            return;
        }
        sdfMemFreePtrDirect(allocation);
    }
}

void sdfFreeAutoDelayed(void *allocation) {
    if (allocation != NULL) {
        if (sdfChipIsInRange((s32)allocation)) {
            sdfChipFreeDelayedDirect(allocation);
            return;
        }
        sdfMemFreeDelayedDirect(sdfMemGetHandleByPtr(allocation));
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
        sdfMemFreePtrDirect(allocation);
    }
}

void FatalError(const char *format, ...) {
    for (;;) {
    }
}

extern void FatalError(const char *format, ...) __attribute__((noreturn));
extern s32 AddIntcHandler2(s32, s32, s32, s32);
extern s32 AddDmacHandler2(s32, s32, s32, s32);

SdfHandlerNode *sdfAddHandler(SdfHandlerKind kind, s32 channel, SdfHandlerCallback handler, s32 arg, s32 next) {
    SdfHandlerNode *node;
    s32 id;

    switch ((s32)kind) {
    case SDF_HANDLER_KIND_INTERRUPT:
        id = AddIntcHandler2(channel, (s32)handler, arg, next);
        break;
    case SDF_HANDLER_KIND_DMA:
        id = AddDmacHandler2(channel, (s32)handler, arg, next);
        break;
    case SDF_HANDLER_KIND_SBUS:
        id = AddSbusIntcHandler(channel, (s32)handler);
        break;
    default:
        FatalError("sdfAddHandler : unknown type %d\n", kind);
        break;
    }
    node = sdfAllocAuto(sizeof(*node));
    node->handlerKind = kind;
    node->channel = channel;
    node->handlerId = id;
    node->next = sdfDmacHandlerTable;
    sdfDmacHandlerTable = node;
    return node;
}

void sdfIpuRemoveHandler(SdfHandlerNode *target) {
    SdfHandlerNode **link = &sdfDmacHandlerTable;
    SdfHandlerNode *node = *link;

    while (node != NULL) {
        if (node == target) {
            *link = node->next;
            switch ((s32)node->handlerKind) {
            case SDF_HANDLER_KIND_INTERRUPT:
                DisableIntc(node->channel);
                RemoveIntcHandler(node->channel, node->handlerId);
                break;
            case SDF_HANDLER_KIND_DMA:
                RemoveDmacHandler(node->channel, node->handlerId);
                break;
            case SDF_HANDLER_KIND_SBUS:
                RemoveSbusIntcHandler(node->channel);
                break;
            }
            sdfFreeAuto(target);
            return;
        }
        /* Non-head handles are followed through their own link. */
        link = &target->next;
        node = *link;
    }
}

void sdfDrainPendingHandlers(void) {
    SdfHandlerNode *current;
    while ((current = sdfDmacHandlerTable) != 0) {
        sdfIpuRemoveHandler(current);
    }
}
