#ifndef FLD_AREA_RESOURCE_REQUEST_REUSE_API_H
#define FLD_AREA_RESOURCE_REQUEST_REUSE_API_H

#include "common.h"

typedef u8 FldAreaResourceRequestReuseResult;
enum {
    FLD_AREA_RESOURCE_REQUEST_NOT_REUSABLE = 0,
    FLD_AREA_RESOURCE_REQUEST_REUSABLE = 1
};

FldAreaResourceRequestReuseResult fldCanReuseAreaResourceRequest(s32 area, s32 room);

#endif /* FLD_AREA_RESOURCE_REQUEST_REUSE_API_H */
