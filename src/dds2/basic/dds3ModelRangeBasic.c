#include "common.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "sdf.h"
#include "dds3_model_range.h"


/* Queue the backing allocation, if present, then free the range-data block.
 * A non-NULL resourceData must match a used general-heap block.
 * The owning object's rangeData member is not cleared by this release. */
void dds3ModelBasicFreeModel(Dds3ModelRangeObjectView *modelRangeObject) {
    Dds3ModelRangeDataView *rangeData;
    const void *resourceData;
    sdfMemHandle_t *allocation;

    rangeData = modelRangeObject->rangeData;
    resourceData = rangeData->resourceData;
    if (resourceData != NULL) {
        allocation = sdfMemGetHandleByPtr(resourceData);
        sdfMemFreeDelayedDirect(allocation);
    }
    sdfChipFreeDirect(rangeData);
}
