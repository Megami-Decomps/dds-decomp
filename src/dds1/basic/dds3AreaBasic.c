#include "common.h"
#include "dds3obj.h"
#include "dds3_area_callback.h"


/* Exchange the area's slot, retaining the native selector pair (0, 5). */
s32 CB_ReleaseObjectBasic(void *areaObject) {
    dds3ObjectBaseSetControlBasicType(areaObject, 0, 5);
    return 1;
}

/* A missing callback returns one; otherwise propagate its return unchanged. */
s32 CB_BasicDrawProcuder(Dds3AreaObjectView *areaObject) {
    s32 (*callback)(Dds3AreaObjectView *);

    callback = areaObject->callbackRecord->callback;
    if (callback == NULL) {
        return 1;
    }
    return callback(areaObject);
}
