#include "common.h"

typedef struct {
    s32 handle;
} EventData;

typedef struct {
    u8 pad[0x18];
    EventData *unk18;
    void *unk1C;
} EventObj;

void effObjFreeInner(void *arg);
void func_00111840(s32 arg);
void func_002CFF98(void *arg);
s32 effObjTestNodeFlags(void *arg, s32 arg1);
void effObjClearNodeFlags(void *arg, s32 arg1);
void effObjInnerVecBackup(void *arg);

void dds3ReleaseEventData(EventObj *event) {
    EventData *data;

    effObjFreeInner(event);
    data = event->unk18;
    func_00111840(data->handle);
    func_002CFF98(data);
}

s32 func_00116188(EventObj *event) {
    void *data;

    data = event->unk1C;
    if (effObjTestNodeFlags(data, 1) == 1) {
        effObjClearNodeFlags(data, 1);
        effObjInnerVecBackup(data);
    }
    return 1;
}
