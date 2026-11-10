#ifndef SDF_THREAD_H
#define SDF_THREAD_H

#include "common.h"

/* SDK thread registration node shared by graph and system management. */
typedef struct sdfThCB_t
{
    struct sdfThCB_t *next;
    s32 thId;
} sdfThCB_t;

/* The tracked list stores the actual node returned by a thread-ID lookup. */
sdfThCB_t *sdfFindThreadNode(s32 threadId);

void sdfStartThreadAndWait(sdfThCB_t *thread, void (*entry)(void), void *stack,
                           s64 stackSize, s32 priority, s32 argument);

#endif /* SDF_THREAD_H */
