#ifndef SDF_HANDLER_NODE_H
#define SDF_HANDLER_NODE_H

#include "common.h"

typedef enum SdfHandlerKind {
    SDF_HANDLER_KIND_INTERRUPT = 0,
    SDF_HANDLER_KIND_DMA = 1,
    SDF_HANDLER_KIND_SBUS = 2
} SdfHandlerKind;

/* Interrupt/DMA handler kinds supply different argument lists. */
typedef s32 (*SdfHandlerCallback)();

/* Shared interrupt/DMA/SBUS handler-list node. The paired sdfAddHandler
 * providers allocate 0x10 bytes and write these four words at +0, +4, +8,
 * and +0xC; removal dispatch reads the three payload words. */
typedef struct SdfHandlerNode {
    struct SdfHandlerNode *next;
    SdfHandlerKind handlerKind;
    s32 channel;
    s32 handlerId;
} SdfHandlerNode;

typedef char SdfHandlerNodeLayoutAssert[
    (sizeof(SdfHandlerNode) == 0x10 &&
     (u32)&((SdfHandlerNode *)0)->next == 0x00 &&
     (u32)&((SdfHandlerNode *)0)->handlerKind == 0x04 &&
     (u32)&((SdfHandlerNode *)0)->channel == 0x08 &&
     (u32)&((SdfHandlerNode *)0)->handlerId == 0x0C) ? 1 : -1];

#endif
