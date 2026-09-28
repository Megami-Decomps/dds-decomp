#include "common.h"

typedef struct {
    u32 handle;
} EventData;

typedef struct {
    u8 pad[0x18];
    EventData *unk18;
    void *unk1C;
} EventObj;

s32 func_0010F878(void *arg, s32 arg1);

void func_0010F860(void *arg, s32 arg1);

void effObjInnerVecBackup(void *arg);

void func_001163B0(EventObj *event) {
    EventData *data;

    func_0010F810();
    data = event->unk18;
    func_00111A68(data->handle);
    func_00328E48(data);
}

s32 func_001163F0(EventObj *arg) {
    void *data;

    data = arg->unk1C;
    if (func_0010F878(data, 1) == 1) {
        func_0010F860(data, 1);
        effObjInnerVecBackup(data);
    }
    return 1;
}
