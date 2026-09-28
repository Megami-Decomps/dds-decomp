#include "common.h"

void dds3ExchangeSlot(void *arg0, s32 arg1, s32 arg2);

typedef struct {
    u8 pad[0xC];
    s32 (*unkC)(void);
} AreaSub;

typedef struct {
    u8 pad[0x10];
    AreaSub *unk10;
} AreaObj;

s32 dds3ExchangeAreaSlot(void *arg) {
    dds3ExchangeSlot(arg, 0, 5);
    return 1;
}

s32 dds3InvokeAreaCallback(AreaObj *arg) {
    s32 (*func)(void);

    func = arg->unk10->unkC;
    if (func == NULL) {
        return 1;
    }
    return func();
}
