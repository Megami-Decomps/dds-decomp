#ifndef FLD_RESOURCE_RESOLVER_H
#define FLD_RESOURCE_RESOLVER_H

#include "common.h"

struct EffWorldNode;

/* Read or update the cached world object associated with a resource record ID. */
struct EffWorldNode *fldHitGetMatter(s32 id);
void fldHitLinkMatter(s32 id, struct EffWorldNode *worldNode);

/* Resolves a named world object or lazily creates/caches a dummy matter object. */
struct EffWorldNode *fldResolveWorldObjectByResourceId(u32 resourceId);

/* Matches a resource name entry, then resolves its owner by the resource name. */
struct EffWorldNode *fldResolveWorldObjectByResourceEntryName(const char *entryName);

#endif
