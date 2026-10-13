#ifndef DDS3_WORLD_VALUE_H
#define DDS3_WORLD_VALUE_H

#include "common.h"

struct EffWorldNode;

/* Store a 32-bit value on the world node when the node is present. */
void dds3SetWorldNodeValue(struct EffWorldNode *node, u32 value);

#endif /* DDS3_WORLD_VALUE_H */
