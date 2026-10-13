#include "common.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "sdf.h"
#include "dds3_model_range.h"


/* Queue the backing allocation, if present, then free the range-data block.
 * A nonzero resourceAddress must match a used general-heap block.
 * The owning object's rangeData member is not cleared by this release. */
void dds3ReleaseModelRangeData(Dds3ModelRangeObjectView *object) {
    Dds3ModelRangeDataView *rangeData;
    s32 resourceAddress;

    rangeData = object->rangeData;
    resourceAddress = rangeData->resourceAddress;
    if (resourceAddress != 0) {
        sdfQueueGeneralAllocationRelease(sdfFindGeneralBlockByAddress((void *)resourceAddress));
    }
    sdfReleaseChipBlock(rangeData);
}
