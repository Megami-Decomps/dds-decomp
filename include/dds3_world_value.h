#ifndef DDS3_WORLD_VALUE_H
#define DDS3_WORLD_VALUE_H

#include "common.h"

struct dds3Basic_t;

/* Store the node's string lookup name in its shared world-value slot. */
void dds3SetUnitObjectId(struct dds3Basic_t *node, const char *objectName);

#endif /* DDS3_WORLD_VALUE_H */
