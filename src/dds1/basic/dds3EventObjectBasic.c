#include "common.h"

typedef struct {
    u8 pad[0x18];
    void *unk18;
    void *unk1C;
} EventObj;

void func_0010F5E8(void *arg);
void func_00111840(s32 arg);
void func_002CFF98(void *arg);
s32 func_0010F650(void *arg, s32 arg1);
void func_0010F638(void *arg, s32 arg1);
void effObjInnerVecBackup(void *arg);

void func_00116148(EventObj *arg) {
    void *data;

    func_0010F5E8(arg);
    data = arg->unk18;
    func_00111840(*(s32 *)data);
    func_002CFF98(data);
}

s32 func_00116188(EventObj *arg) {
    void *data;

    data = arg->unk1C;
    if (func_0010F650(data, 1) == 1) {
        func_0010F638(data, 1);
        effObjInnerVecBackup(data);
    }
    return 1;
}
