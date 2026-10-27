#include "common.h"
#include "dds3obj.h"
#include "dds3_object_control_slot.h"
#include "dds3_area_callback.h"


/* Exchange the area's slot, retaining the native selector pair (0, 5). */
s32 CB_ReleaseObjectBasic(dds3Basic_t *areaObject) {
    dds3ObjectBaseSetControlBasicType(areaObject, 0, DDS3_OBJECT_HANDLER_SLOT);
    return 1;
}

/* A missing callback returns one; otherwise propagate its return unchanged. */
s32 CB_BasicDrawProcuder(dds3Basic_t *areaObject) {
    Dds3AreaObjectView *area = (Dds3AreaObjectView *)areaObject;
    s32 (*callback)(Dds3AreaObjectView *);

    callback = area->callbackRecord->callback;
    if (callback == NULL) {
        return 1;
    }
    return callback(area);
}
