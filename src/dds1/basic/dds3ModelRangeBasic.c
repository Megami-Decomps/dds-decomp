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
void func_002D0A10(s32 arg);
void func_002CFF98(void *arg);

void func_00116A50(ModelRangeObj *arg) {
    ModelRangeData *data;
    s32 tmp;

    data = arg->unk18;
    tmp = data->unk4;
    if (tmp != 0) {
        func_002D0A10(func_002D0A80(tmp));
    }
    func_002CFF98(data);
}
