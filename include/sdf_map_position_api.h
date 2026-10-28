#ifndef SDF_MAP_POSITION_API_H
#define SDF_MAP_POSITION_API_H

#include "common.h"

typedef s32 SdfMapPositionLookupResult;

enum SdfMapPositionLookupResultValue {
    SDF_MAP_POSITION_LOOKUP_NOT_FOUND = 0,
    SDF_MAP_POSITION_LOOKUP_FOUND = 1
};

struct sdfModelRootNode_t;

/* On success, write the requested MPOS record's position or matrix to VU state. */
SdfMapPositionLookupResult sdfModelGetMarkPos(
    struct sdfModelRootNode_t *model, s32 id);
SdfMapPositionLookupResult sdfModelGetMarkPosMatrix(
    struct sdfModelRootNode_t *model, s32 id);

#endif /* SDF_MAP_POSITION_API_H */
