#ifndef DDS3_OBJECT_RESOURCE_STATE_H
#define DDS3_OBJECT_RESOURCE_STATE_H

#include "common.h"

/* Width-preserving contract for the generic base's resourceState word:
 * model context (0), device motion (1), effect dependency (2), or released (3). */
typedef u32 Dds3ObjectResourceState;

/* Effect dependencies live in the effect object's payload, outside resourceHandle. */
#define DDS3_OBJECT_RESOURCE_EFFECT_DEPENDENCY ((Dds3ObjectResourceState)2)

#endif
