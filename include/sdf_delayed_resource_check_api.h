#ifndef SDF_DELAYED_RESOURCE_CHECK_API_H
#define SDF_DELAYED_RESOURCE_CHECK_API_H

#include "common.h"

/* Returns 1 when delayed resource work is queued or pending, otherwise 0. */
typedef u8 SdfDelayedResourceCheckResult;

SdfDelayedResourceCheckResult sdfDelayedResrcCheck(void);

#endif /* SDF_DELAYED_RESOURCE_CHECK_API_H */
