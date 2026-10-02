#include "common.h"

typedef struct {
    u8 pad[4];
    s32 resourceId;
} ModelRangeData;

typedef struct {
    u8 pad[0x18];
    ModelRangeData *rangeData;
} ModelRangeObj;

extern u64 func_00329930(s32);

void dds3ReleaseModelRangeData(ModelRangeObj *object) {
    ModelRangeData *data;
    s32 resource;
    u64 handle;

    data = object->rangeData;
    resource = data->resourceId;
    if (resource != 0) {
        handle = func_00329930(resource);
        sdfQueueNonzeroResourceId(handle);
    }
    sdfReleaseChipBlock(data);
}
