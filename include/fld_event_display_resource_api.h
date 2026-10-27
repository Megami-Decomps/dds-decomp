#ifndef FLD_EVENT_DISPLAY_RESOURCE_API_H
#define FLD_EVENT_DISPLAY_RESOURCE_API_H

#include "common.h"

struct FldFileResource;
struct dds3Basic_t;

/* Index for a borrowed event display resource stored in fldDisplayValues.
 * Its owner keeps the record alive through room-event cleanup. */
typedef u32 FldEventDisplayResourceIndex;

FldEventDisplayResourceIndex fldEveHit_AddEveData(
    const struct FldFileResource *resource, struct dds3Basic_t *object);

#endif /* FLD_EVENT_DISPLAY_RESOURCE_API_H */
