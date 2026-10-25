#ifndef FLD_EVENT_DISPLAY_RESOURCE_API_H
#define FLD_EVENT_DISPLAY_RESOURCE_API_H

#include "common.h"

struct FldFileResource;
struct dds3Basic_t;

/* Index returned for an event display resource stored in fldDisplayValues. */
typedef u32 FldEventDisplayResourceIndex;

FldEventDisplayResourceIndex fldEveHit_AddEveData(
    struct FldFileResource *resource, struct dds3Basic_t *object);

#endif /* FLD_EVENT_DISPLAY_RESOURCE_API_H */
