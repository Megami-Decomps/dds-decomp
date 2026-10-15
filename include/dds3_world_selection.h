#ifndef DDS3_WORLD_SELECTION_H
#define DDS3_WORLD_SELECTION_H

#include "dds3_world_primary.h"
#include "dds3_world_secondary.h"

struct EffWorldNode;

/* Setting either selection does not transfer ownership. */
void dds3SetWorldObject(struct EffWorldNode *primaryObject);
void dds3SetWorldSecondaryObject(struct EffWorldNode *secondaryObject);

#endif
