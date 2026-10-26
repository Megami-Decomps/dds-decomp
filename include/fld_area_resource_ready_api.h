#ifndef FLD_AREA_RESOURCE_READY_API_H
#define FLD_AREA_RESOURCE_READY_API_H

#include "common.h"

typedef u8 FldAreaResourceReadyState;
enum {
    FLD_AREA_RESOURCE_NOT_READY = 0,
    FLD_AREA_RESOURCE_READY = 1
};

/* READY when the active area request completes or the shared ready flag is
 * set. */
FldAreaResourceReadyState fldIsAreaResourceReady(void);

#endif
