#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_path.h"

extern void dds3ReleaseObjectResource(EffWorldNode *slotObject);

void dds3ReleaseWorldSlotResource(EffWorldNode *slotObject) {
    Dds3SlotResource *resource;

    resource = slotObject->data;
    dds3ReleaseObjectResource(slotObject);
    dds3ExchangeSlot(resource->target, 0, 1);
    sdfChipFreeDirect(resource);
}
