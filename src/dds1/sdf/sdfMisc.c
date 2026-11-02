#include "sdk_ps2.h"
#include "sdf_request.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_trig.h"
#include "ee_mmi.h"
#include "sdf_chip.h"
#include "sdf_heap_api.h"
#include "sdf_draw.h"
#include "sdf_sif_command.h"
#include "sdf_dev_event.h"
#include "sdf_dev_protocol.h"
#include "sdf_dev_state.h"
#include "sdf_memory_api.h"

extern void sdfMemReallocRecord(struct sdfMemHandle_t *allocation, s32 size);

/* Allocate element storage with a fixed element stride and growth increment. */
DevRequest *sdfDevCreateBufferedRequest(s32 elementCount, s32 elementStride, s32 growthStep) {
    DevRequest *request = sdfChipAllocDirect(sizeof(*request));

    request->growStep = growthStep;
    request->usedCount = 0;
    request->capacity = elementCount;
    request->stride = elementStride;
    if (elementCount != 0) {
        request->backingAllocation = sdfMemAllocDirect(elementStride * elementCount);
        request->buffer = (void *)sdfMemLock(request->backingAllocation);
    } else {
        request->backingAllocation = 0;
        request->buffer = 0;
    }
    return request;
}

/* Release the backing allocation and the request object. */
void sdfItemArrayFree(DevRequest *request) {
    sdfMemFreeDirect(request->backingAllocation);
    sdfChipFreeDirect(request);
}

/* Grow capacity, preserving the SDK's signed 16-bit allocation-size arithmetic. */
void sdfItemArrayGrow(DevRequest *request) {
    if (request->backingAllocation == 0) {
        sdfDevResizeBufferedRequest(request, request->growStep);
        return;
    }
    sdfMemUnlock(request->backingAllocation);
    request->capacity = request->capacity + request->growStep;
    sdfMemReallocRecord(request->backingAllocation, (s16)request->capacity * request->stride);
    request->buffer = (void *)sdfMemLock(request->backingAllocation);
}

/* Resize storage and clamp the live entry count to the new capacity. */
void sdfDevResizeBufferedRequest(DevRequest *request, s32 elementCount) {
    if (request->backingAllocation == 0) {
        if (elementCount > 0) {
            request->capacity = elementCount;
            request->backingAllocation = sdfMemAllocDirect(request->stride * elementCount);
            request->buffer = (void *)sdfMemLock(request->backingAllocation);
        }
    } else if (elementCount <= 0) {
        sdfMemFreeDirect(request->backingAllocation);
        request->backingAllocation = 0;
        request->usedCount = 0;
        request->capacity = 0;
        request->buffer = 0;
    } else {
        sdfMemUnlock(request->backingAllocation);
        request->capacity = elementCount;
        sdfMemReallocRecord(request->backingAllocation, request->stride * elementCount);
        request->buffer = (void *)sdfMemLock(request->backingAllocation);
        if (elementCount < request->usedCount) {
            request->usedCount = elementCount;
        }
    }
}
