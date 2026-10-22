#include "common.h"
#include "sdf_chip.h"
#include "eff_event.h"


void dds3FreePoseSet(EffWorldNode *object);
void dds3ObjectBaseFreeControlWorkData(ObjBase *resource);
u8 dds3CheckPoseSetFlag(ObjectTransform *node, u32 flags);

void dds3OffPoseSetFlag(ObjectTransform *node, u32 flags);

void dds3SetRadiusPoseSet(ObjectTransform *node);

/* Release the event-data handle and its allocation after the inner object. */
void EventObjectBasicDestructor(EffWorldNode *eventObject) {
    DdsSlotResourceBlock *eventData;

    dds3FreePoseSet(eventObject);
    eventData = eventObject->data;
    dds3ObjectBaseFreeControlWorkData(eventData->resourceState);
    sdfChipFreeDirect(eventData);
}

/* Snapshot the node's vectors once its pending flag is observed. */
s32 EventObjectBasicMainProcuder(EffWorldNode *eventObject) {
    ObjectTransform *eventNode;

    eventNode = eventObject->inner;
    if (dds3CheckPoseSetFlag(eventNode, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) == 1) {
        dds3OffPoseSetFlag(eventNode, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING);
        dds3SetRadiusPoseSet(eventNode);
    }
    return 1;
}
