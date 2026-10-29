#include "common.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "sdf.h"
#include "dds3_model_range.h"


/* The kind-11 destroy hook queues its optional resource allocation before
 * freeing its data block. dds3DeleteBasic frees the object after this returns. */
void dds3ModelBasicFreeModel(dds3Basic_t *object) {
    const Dds3ModelRangeDataView *rangeData;
    const void *ownedResourceData;

    rangeData = object->workdata;
    ownedResourceData = rangeData->ownedResourceData;
    if (ownedResourceData != NULL) {
        sdfMemFreeDelayedDirect(sdfMemGetHandleByPtr(ownedResourceData));
    }
    sdfChipFreeDirect((void *)rangeData);
}
