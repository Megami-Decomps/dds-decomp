#ifndef SDF_QUEUED_RESOURCE_API_H
#define SDF_QUEUED_RESOURCE_API_H

#include "common.h"

/* Queue a retained resource for deferred release; otherwise destroy it now. */
void sdfReleaseQueuedResource(void *resource, s32 retained);

#endif /* SDF_QUEUED_RESOURCE_API_H */
