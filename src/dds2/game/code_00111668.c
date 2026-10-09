#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_path.h"

extern void dds3ReleaseObjectResource(EffWorldNode *object);

void dds3ReleaseWorldSlotResource(EffWorldNode *object) {
    Dds3SlotResource *resource;

    resource = object->data;
    dds3ReleaseObjectResource(object);
    dds3ExchangeSlot(resource->target, 0, 1);
    sdfReleaseChipBlock(resource);
}
