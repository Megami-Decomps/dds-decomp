#ifndef SDF_HANDLER_NODE_H
#define SDF_HANDLER_NODE_H

#include "common.h"

/* Shared interrupt/DMA/SBUS handler-list node. The paired sdfAddHandler
 * providers allocate 0x10 bytes and write these four words at +0, +4, +8,
 * and +0xC; removal dispatch reads the three payload words. */
typedef struct SdfHandlerNode {
    struct SdfHandlerNode *next;
    s32 kind;
    s32 channel;
    s32 handlerId;
} SdfHandlerNode;

typedef char SdfHandlerNodeLayoutAssert[
    (sizeof(SdfHandlerNode) == 0x10 &&
     (u32)&((SdfHandlerNode *)0)->next == 0x00 &&
     (u32)&((SdfHandlerNode *)0)->kind == 0x04 &&
     (u32)&((SdfHandlerNode *)0)->channel == 0x08 &&
     (u32)&((SdfHandlerNode *)0)->handlerId == 0x0C) ? 1 : -1];

#endif
