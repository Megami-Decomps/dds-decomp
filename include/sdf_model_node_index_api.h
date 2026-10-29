#ifndef SDF_MODEL_NODE_INDEX_API_H
#define SDF_MODEL_NODE_INDEX_API_H

#include "sdf_draw.h"

typedef s32 SdfModelNodeIndex;
#define SDF_MODEL_NODE_INDEX_NOT_FOUND (-1)

/* Named-ID lookups return a signed model node-list index or NOT_FOUND. */
SdfModelNodeIndex sdfModelGetNodeIndexByName(sdfModelRootNode_t *model, const char *name);

#endif /* SDF_MODEL_NODE_INDEX_API_H */
