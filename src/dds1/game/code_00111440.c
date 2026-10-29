#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_object_control_slot.h"
#include "dds3_path.h"

extern void dds3MoverBasicFreePath(dds3Basic_t *object);

void MoverBasicDestructor(dds3Basic_t *object) {
    Dds3SlotResource *resource;

    resource = object->workdata;
    dds3MoverBasicFreePath(object);
    dds3ObjectBaseSetControlBasicType(resource->target, 0, DDS3_OBJECT_DATA_SLOT);
    sdfChipFreeDirect(resource);
}
