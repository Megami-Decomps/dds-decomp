#include "common.h"

void dds3ExchangeSlot(void *arg0, s32 arg1, s32 arg2);

typedef struct {
    u8 pad[0xC];
    s32 (*callback)(void);
} AreaSub;

typedef struct {
    u8 pad[0x10];
    AreaSub *callbackRecord;
} AreaObj;

/* Exchange the area's slot, retaining the native selector pair (0, 5). */
s32 dds3ExchangeAreaSlot(void *areaObject) {
    dds3ExchangeSlot(areaObject, 0, 5);
    return 1;
}

/* A missing callback returns one; otherwise propagate its return unchanged. */
s32 dds3InvokeAreaCallback(AreaObj *areaObject) {
    s32 (*callback)(void);

    callback = areaObject->callbackRecord->callback;
    if (callback == NULL) {
        return 1;
    }
    return callback();
}
