#ifndef DDS3_WORLD_VALUE_H
#define DDS3_WORLD_VALUE_H

#include "common.h"

struct dds3Basic_t;

/* Store objectName directly as the node's lookup key; no copy is made.
 * A NULL node is ignored and a NULL name clears the key. Keep non-NULL names
 * alive while the node can be searched. */
void dds3SetUnitObjectId(struct dds3Basic_t *node, const char *objectName);

#endif /* DDS3_WORLD_VALUE_H */
