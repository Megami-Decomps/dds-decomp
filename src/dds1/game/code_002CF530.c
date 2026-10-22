#include "sdk_ps2.h"
#include "common.h"
#include "sdf_resource.h"
#include "sdf_chip.h"
#include "sdf_handler_node.h"

extern void sdfMemFreePtrDirect(void *);

extern void (*sdfTickCallback)(void);




extern SdfHandlerNode *sdfDmacHandlerTable;

extern s32 DisableIntc(s32);

void *sdfAllocAuto(s32 size) {
    if (size >= 0x401) {
        return (void *)sdfMemLock(sdfMemAllocDirect(size));
    }
    return sdfChipAllocDirect(size);
}

void sdfFreeAuto(void *allocation) {
    if (allocation != NULL) {
        if (sdfChipCheck((s32)allocation)) {
            sdfChipFreeDirect(allocation);
            return;
        }
        sdfMemFreePtrDirect(allocation);
    }
}


void sdfFreeAutoDelayed(void *allocation) {
    if (allocation != NULL) {
        if (sdfChipCheck((s32)allocation)) {
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
        if (sdfChipCheck((s32)allocation)) {
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

SdfHandlerNode *sdfAddHandler(s32 kind, s32 channel, s32 handler, s32 arg, s32 next) {
    SdfHandlerNode *node;
    s32 id;

    switch (kind) {
    case 0:
        id = AddIntcHandler2(channel, handler, arg, next);
        break;
    case 1:
        id = AddDmacHandler2(channel, handler, arg, next);
        break;
    case 2:
        id = AddSbusIntcHandler(channel, handler);
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
            switch (node->handlerKind) {
            case 0:
                DisableIntc(node->channel);
                RemoveIntcHandler(node->channel, node->handlerId);
                break;
            case 1:
                RemoveDmacHandler(node->channel, node->handlerId);
                break;
            case 2:
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

SdfHandlerNode *sdfDmacHandlerTable __attribute__((section(".sdata"))) = NULL;

