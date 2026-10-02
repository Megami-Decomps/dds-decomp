#include "common.h"

typedef struct {
    u8 pad[4];
    s32 resourceId;
} ModelRangeData;

typedef struct {
    u8 pad[0x18];
    ModelRangeData *rangeData;
} ModelRangeObj;

s32 sdfFindGeneralBlockByAddress(s32 arg);
void sdfQueueNonzeroResourceId(s32 arg);
void sdfReleaseChipBlock(void *arg);

void dds3ReleaseModelRangeData(ModelRangeObj *object) {
    ModelRangeData *data;
    s32 resource;

    data = object->rangeData;
    resource = data->resourceId;
    if (resource != 0) {
        sdfQueueNonzeroResourceId(sdfFindGeneralBlockByAddress(resource));
    }
    sdfReleaseChipBlock(data);
}
