#ifndef DDS3_WORLD_SELECTION_H
#define DDS3_WORLD_SELECTION_H

/* These slots borrow opaque object pointers; setting them does not transfer ownership. */
#include "dds3_world_secondary.h"
void dds3SetWorldObject(void *primaryObject);
void *dds3GetWorldObject(void);
void dds3SetWorldSecondaryObject(void *secondaryObject);

#endif
