#include "common.h"
#include "sdf_chip.h"
#include "eff_event.h"


void dds3FreePoseSet(dds3Basic_t *object);
void dds3ObjectBaseFreeControlWorkData(dds3ObjectBaseControlWorkData_t *resource);
u8 dds3CheckPoseSetFlag(dds3PoseSet_t *node, u32 flags);
void dds3OffPoseSetFlag(dds3PoseSet_t *node, u32 flags);
void dds3SetRadiusPoseSet(dds3PoseSet_t *node);

/* Release the event-data handle and its allocation after the inner object. */
void EventObjectBasicDestructor(dds3Basic_t *eventObject) {
    DdsSlotResourceBlock *eventData;

    dds3FreePoseSet(eventObject);
    eventData = eventObject->data;
    dds3ObjectBaseFreeControlWorkData(eventData->baseControlWorkData);
    sdfChipFreeDirect(eventData);
}

/* Snapshot the node's vectors once its pending flag is observed. */
s32 EventObjectBasicMainProcuder(dds3Basic_t *eventObject) {
    dds3PoseSet_t *eventNode;

    eventNode = eventObject->inner;
    if (dds3CheckPoseSetFlag(eventNode, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) == 1) {
        dds3OffPoseSetFlag(eventNode, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING);
        dds3SetRadiusPoseSet(eventNode);
    }
    return 1;
}
