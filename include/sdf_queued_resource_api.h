#ifndef SDF_QUEUED_RESOURCE_API_H
#define SDF_QUEUED_RESOURCE_API_H

#include "common.h"

struct DevRequest;

/* Queue a retained resource for deferred release; otherwise destroy it now. */
void sdfMatListFreeResrc(struct DevRequest *resource, s32 retained);

#endif /* SDF_QUEUED_RESOURCE_API_H */
