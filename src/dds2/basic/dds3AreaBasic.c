#include "common.h"
#include "dds3obj.h"
#include "dds3_object_control_slot.h"
#include "dds3_area_callback.h"

/* Exchange the area's slot, retaining the native selector pair (0, 5). */
Dds3BasicArrayVisitResult CB_ReleaseObjectBasic(dds3Basic_t *areaObject) {
    dds3ObjectBaseSetControlBasicType(areaObject, 0, DDS3_OBJECT_HANDLER_SLOT);
    return DDS3_BASIC_ARRAY_VISIT_CONTINUE;
}

/* Continue without an area callback; otherwise forward its visitor result. */
Dds3BasicArrayVisitResult CB_BasicDrawProcuder(dds3Basic_t *areaObject) {
    Dds3AreaObjectView *area = (Dds3AreaObjectView *)areaObject;
    Dds3BasicArrayVisitResult (*callback)(Dds3AreaObjectView *);

    callback = area->callbackRecord->callback;
    if (callback == NULL) {
        return DDS3_BASIC_ARRAY_VISIT_CONTINUE;
    }
    return callback(area);
}
