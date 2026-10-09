#ifndef SDF_THREAD_H
#define SDF_THREAD_H

#include "common.h"

struct SdfThreadNode;

/* The tracked list stores the actual node returned by a thread-ID lookup. */
struct SdfThreadNode *sdfFindThreadNode(s32 threadId);

/* Register a thread node, then start its entry function on the supplied stack. */
void sdfStartTrackedThread(struct SdfThreadNode *node, void (*entry)(void),
                           void *stack, s64 stackSize, s32 priority, s32 arg);

#endif /* SDF_THREAD_H */
