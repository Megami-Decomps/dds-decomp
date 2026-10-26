#ifndef SDF_QUEUED_RESOURCE_API_H
#define SDF_QUEUED_RESOURCE_API_H

#include "common.h"

struct DevRequest;

typedef s32 SdfMatListReleaseMode;
enum {
    SDF_MAT_LIST_RELEASE_CONTAINER_ONLY = 0,
    SDF_MAT_LIST_DEFER_FULL_RELEASE = 1
};

/* Container-only release leaves material entries allocated; deferred full
 * release queues the callback that frees both entries and the list. */
void sdfMatListFreeResrc(struct DevRequest *resource, SdfMatListReleaseMode releaseMode);

#endif /* SDF_QUEUED_RESOURCE_API_H */
