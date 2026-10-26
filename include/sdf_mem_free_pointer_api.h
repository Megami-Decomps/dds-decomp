#ifndef SDF_MEM_FREE_POINTER_API_H
#define SDF_MEM_FREE_POINTER_API_H

#include "common.h"

/* Resolve a payload address to its descriptor, then release that allocation. */
void sdfMemFreePtrDirect(const void *address);

#endif /* SDF_MEM_FREE_POINTER_API_H */
