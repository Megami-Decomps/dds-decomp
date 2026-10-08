#ifndef ITF_MEM_NODE_H
#define ITF_MEM_NODE_H

#include "common.h"

/* Free-list header stored immediately before each interface-pool payload. */
typedef struct MemNode {
    u32 slotIndex; /* Zero identifies the ring sentinel. */
    struct MemNode *next;
} MemNode;

typedef char MemNodeSizeCheck[(sizeof(MemNode) == 8) ? 1 : -1];
typedef char MemNodeNextOffsetCheck[((u32)&((MemNode *)0)->next == 4) ? 1 : -1];

MemNode *itfCreateMemNodeRing(s32 payloadBytes, s32 count);
void *itfDequeueMemNode(MemNode *pool);
s32 itfEnqueueMemNode(void *payload, MemNode *pool);
u32 itfReleaseMemNodeBuffer(MemNode *pool);

#endif
