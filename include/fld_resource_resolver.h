#ifndef FLD_RESOURCE_RESOLVER_H
#define FLD_RESOURCE_RESOLVER_H

#include "common.h"

/* Resolves a named world object or lazily creates/caches a dummy matter object. */
void *fldResolveWorldObjectByResourceId(u32 resourceId);

#endif
