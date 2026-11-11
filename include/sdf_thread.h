#ifndef SDF_THREAD_H
#define SDF_THREAD_H

#include "libsdf.h"

/* The tracked list stores the actual node returned by a thread-ID lookup. */
sdfThCB_t *sdfFindThreadNode(s32 threadId);

#endif
