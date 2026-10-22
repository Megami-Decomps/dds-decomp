#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_path.h"

extern void dds3MoverBasicFreePath(EffWorldNode *slotObject);

void MoverBasicDestructor(EffWorldNode *slotObject) {
    Dds3SlotResource *resource;

    resource = slotObject->data;
    dds3MoverBasicFreePath(slotObject);
    dds3ObjectBaseSetControlBasicType(resource->target, 0, 1);
    sdfChipFreeDirect(resource);
}
