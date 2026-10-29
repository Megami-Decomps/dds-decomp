#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_object_control_slot.h"
#include "dds3_path.h"

extern void dds3MoverBasicFreePath(dds3Basic_t *slotObject);

void MoverBasicDestructor(dds3Basic_t *slotObject) {
    Dds3SlotResource *resource;

    resource = slotObject->workdata;
    dds3MoverBasicFreePath(slotObject);
    dds3ObjectBaseSetControlBasicType(resource->target, 0, DDS3_OBJECT_DATA_SLOT);
    sdfChipFreeDirect(resource);
}
