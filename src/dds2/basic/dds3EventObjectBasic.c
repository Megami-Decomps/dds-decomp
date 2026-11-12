#include "common.h"
#include "dds3_basic_tables.h"
#include "sdf_chip.h"
#include "eff_event.h"
#include "dds3_basic_lifecycle.h"
#include "dds3obj.h"
#include "dds3_object_flags.h"

/* Attach a 16-byte slot block before constructing and publishing its state handle.
 * Return the accepted result expected by dds3AllocBasic. */
Dds3BasicCreateResult EventObjectBasicConstructor(dds3Basic_t *object) {
    DdsSlotResourceBlock *resource;
    dds3ObjectBaseControlWorkData_t *handle;

    dds3InitPoseSet(object);
    resource = (DdsSlotResourceBlock *)sdfChipAllocDirect(0x10);
    object->workdata = resource;
    handle = dds3ObjectBaseInitControlWorkData(object);
    resource->baseControlWorkData = handle;
    return DDS3_BASIC_CREATE_ACCEPTED;
}

void dds3FreePoseSet(dds3Basic_t *object);
void dds3ObjectBaseFreeControlWorkData(dds3ObjectBaseControlWorkData_t *resource);
u8 dds3CheckPoseSetFlag(dds3PoseSet_t *node, u32 flags);

void dds3OffPoseSetFlag(dds3PoseSet_t *node, u32 flags);

void dds3SetRadiusPoseSet(dds3PoseSet_t *node);

/* Release the event-data handle and its allocation after the inner object. */
void EventObjectBasicDestructor(dds3Basic_t *eventObject) {
    DdsSlotResourceBlock *eventData;

    dds3FreePoseSet(eventObject);
    eventData = eventObject->workdata;
    dds3ObjectBaseFreeControlWorkData(eventData->baseControlWorkData);
    sdfChipFreeDirect(eventData);
}

/* Snapshot the node's vectors once its pending flag is observed. */
s32 EventObjectBasicMainProcuder(dds3Basic_t *eventObject) {
    dds3PoseSet_t *eventNode;

    eventNode = eventObject->poseset;
    if (dds3CheckPoseSetFlag(eventNode, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) == 1) {
        dds3OffPoseSetFlag(eventNode, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING);
        dds3SetRadiusPoseSet(eventNode);
    }
    return 1;
}

/* Create an inner-vector object and snapshot its vector state after initialization. */

dds3Basic_t *dds3AddEventObjectBasic(s32 initialValue, u128 *position, u128 *rotation) {
    dds3Basic_t *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_RESOURCE_OWNER);

    obj->key = initialValue;
    dds3SetPosPoseSet(obj, position);
    dds3SetRotPoseSet(obj, rotation);
    dds3SetRadiusPoseSet(obj->poseset);
    return obj;
}

dds3ObjectBaseControlWorkData_t *dds3EventObjectBasicGetControlWorkData(const dds3Basic_t *object) {
    const DdsSlotResourceBlock *block = (const DdsSlotResourceBlock *)object->workdata;
    return block->baseControlWorkData;
}

/* Allocate the kind-9 light payload and its auxiliary buffer. */

/* Complete lifecycle dispatch table for this basic kind. */
dds3BasicFunction_t EventObjectBasicTable = {
    EventObjectBasicConstructor,
    EventObjectBasicDestructor,
    EventObjectBasicMainProcuder,
    NULL,
    NULL,
    NULL,
};
