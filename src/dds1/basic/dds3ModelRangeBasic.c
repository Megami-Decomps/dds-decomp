#include "common.h"

typedef struct {
    u8 pad[4];
    s32 unk4;
} ModelRangeData;

typedef struct {
    u8 pad[0x18];
    ModelRangeData *unk18;
} ModelRangeObj;

s32 func_002D0A80(s32 arg);
void sdfQueueNonzeroResourceId(s32 arg);
void sdfReleaseChipBlock(void *arg);

void dds3ReleaseModelRangeData(ModelRangeObj *object) {
    ModelRangeData *data;
    s32 resource;

    data = object->unk18;
    resource = data->unk4;
    if (resource != 0) {
        sdfQueueNonzeroResourceId(func_002D0A80(resource));
    }
    sdfReleaseChipBlock(data);
}
