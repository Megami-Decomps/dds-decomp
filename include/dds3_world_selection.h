#ifndef DDS3_WORLD_SELECTION_H
#define DDS3_WORLD_SELECTION_H

#include "dds3_world_primary.h"
#include "dds3_world_secondary.h"

struct dds3Basic_t;

/* Setting either selection does not transfer ownership. */
void dds3SetActiveScene(struct dds3Basic_t *primaryObject);
void dds3SetControlScene(struct dds3Basic_t *secondaryObject);

#endif
