#ifndef SDF_THREAD_WORKSPACE_API_H
#define SDF_THREAD_WORKSPACE_API_H

#include "common.h"

/* Allocate a workspace sized by stackBytes, pass the thread entry to
 * CreateThreadSimple, and return that function's result unchanged. */
s32 sdfCreateThreadWithStack(void (*entryAddress)(), s32 stackBytes, s32 priority);

#endif /* SDF_THREAD_WORKSPACE_API_H */
