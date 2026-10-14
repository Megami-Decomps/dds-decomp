#ifndef DDS3_WORLD_SELECTION_H
#define DDS3_WORLD_SELECTION_H

#include "dds3_world_primary.h"
#include "dds3_world_secondary.h"

/* Setting either selection does not transfer ownership. */
void dds3SetWorldObject(void *primaryObject);
void dds3SetWorldSecondaryObject(void *secondaryObject);

#endif
