#ifndef SDF_REQUEST_H
#define SDF_REQUEST_H

#include "sdf_draw.h"

/* Operations on the complete buffered-request owner, distinct from draw packets. */
void sdfDestroyDevRequest(DevRequest *request);
void sdfDevResizeBufferedRequest(DevRequest *request, s32 count);
void sdfDevBufferedRequestGrow(DevRequest *request);

/* Bind resource and asset release queues to their matching destruction callbacks. */
void sdfRegisterResourceQueueCallbacks(void);
/* Initialize the buffered object-list release queue with its object destructor. */
void sdfInitializeObjectListRequest(void);

#endif
