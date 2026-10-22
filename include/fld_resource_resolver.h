#ifndef FLD_RESOURCE_RESOLVER_H
#define FLD_RESOURCE_RESOLVER_H

#include "common.h"

struct dds3Basic_t;

/* Read or update the cached world object associated with a resource record ID. */
struct dds3Basic_t *fldFindMatterIndex(s32 id);
void fldFindMatterIndexFrom(s32 id, struct dds3Basic_t *worldNode);

/* Resolves a named world object or lazily creates/caches a dummy matter object. */
struct dds3Basic_t *fldGetMatter_From_ModelRangeID(u32 resourceId);

/* Matches a resource name entry, then resolves its owner by the resource name. */
struct dds3Basic_t *fldGetModelRange_From_MatterName(const char *entryName);

#endif
