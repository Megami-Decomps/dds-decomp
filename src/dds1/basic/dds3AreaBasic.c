#include "common.h"
#include "dds3obj.h"
#include "dds3_area_callback.h"


/* Exchange the area's slot, retaining the native selector pair (0, 5). */
s32 dds3ExchangeAreaSlot(void *areaObject) {
    dds3ExchangeSlot(areaObject, 0, 5);
    return 1;
}

/* A missing callback returns one; otherwise propagate its return unchanged. */
s32 dds3InvokeAreaCallback(Dds3AreaObjectView *areaObject) {
    s32 (*callback)(void);

    callback = areaObject->callbackRecord->callback;
    if (callback == NULL) {
        return 1;
    }
    return callback();
}
