#include "common.h"

typedef struct {
    u8 pad[4];
    s32 unk4;
} ModelRangeData;

typedef struct {
    u8 pad[0x18];
    ModelRangeData *unk18;
} ModelRangeObj;

extern u64 func_00329930(s32);

void dds3ReleaseModelRangeData(ModelRangeObj *object) {
    ModelRangeData *data;
    s32 resource;
    u64 handle;

    data = object->unk18;
    resource = data->unk4;
    if (resource != 0) {
        handle = func_00329930(resource);
        func_003298C0(handle);
    }
    func_00328E48(data);
}
