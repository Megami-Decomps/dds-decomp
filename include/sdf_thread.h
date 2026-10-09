#ifndef SDF_THREAD_H
#define SDF_THREAD_H

#include "common.h"

/* Linked registry node stored in the title's tracked-thread table (0x8). */
typedef struct SdfThreadNode {
    struct SdfThreadNode *next; /* 0x00 */
    s32 threadId;               /* 0x04 */
} SdfThreadNode;

/* The tracked list stores the actual node returned by a thread-ID lookup. */
SdfThreadNode *sdfFindThreadNode(s32 threadId);

/* Register a thread node, then start its entry function on the supplied stack. */
void sdfStartTrackedThread(SdfThreadNode *node, void (*entry)(void),
                           void *stack, s64 stackSize, s32 priority, s32 arg);

#endif /* SDF_THREAD_H */
