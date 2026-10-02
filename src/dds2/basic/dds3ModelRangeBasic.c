#include "common.h"

/* The backing resource is identified by its data address, not an allocation ID. */
typedef struct {
    u8 pad[4];
    s32 resourceAddress;
} ModelRangeData;

typedef struct {
    u8 pad[0x18];
    ModelRangeData *rangeData;
} ModelRangeObj;

extern u64 sdfFindGeneralBlockByAddress(s32 resourceAddress);

/* Queue the backing allocation, if present, then free the range-data block.
 * A nonzero resourceAddress must match a used general-heap block.
 * The owning object's rangeData member is not cleared by this release. */
void dds3ReleaseModelRangeData(ModelRangeObj *object) {
    ModelRangeData *rangeData;
    s32 resourceAddress;
    u64 allocationHandle;

    rangeData = object->rangeData;
    resourceAddress = rangeData->resourceAddress;
    if (resourceAddress != 0) {
        allocationHandle = sdfFindGeneralBlockByAddress(resourceAddress);
        sdfQueueNonzeroResourceId(allocationHandle);
    }
    sdfReleaseChipBlock(rangeData);
}
