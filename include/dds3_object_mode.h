#ifndef DDS3_OBJECT_MODE_H
#define DDS3_OBJECT_MODE_H

#include "common.h"

struct dds3Basic_t;

/* Set a resolved object's mode and default weight: weight is 1 for modes
 * 0, 4, and 5, and 0 for modes 1, 2, 3, and 6. Other modes leave both alone. */
void dds3ObjectBaseMatterAnimeSpeed(struct dds3Basic_t *object, u32 requestedMode);

#endif /* DDS3_OBJECT_MODE_H */
