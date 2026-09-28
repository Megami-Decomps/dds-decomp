#include "common.h"

typedef struct {
    u32 handle;
} EventData;

typedef struct {
    u8 pad[0x18];
    EventData *unk18;
    void *unk1C;
} EventObj;

s32 effObjTestNodeFlags(void *arg, s32 arg1);

void effObjClearNodeFlags(void *arg, s32 arg1);

void effObjInnerVecBackup(void *arg);

void dds3ReleaseEventData(EventObj *event) {
    EventData *data;

    effObjFreeInner();
    data = event->unk18;
    func_00111A68(data->handle);
    func_00328E48(data);
}

s32 func_001163F0(EventObj *arg) {
    void *data;

    data = arg->unk1C;
    if (effObjTestNodeFlags(data, 1) == 1) {
        effObjClearNodeFlags(data, 1);
        effObjInnerVecBackup(data);
    }
    return 1;
}
