#include "common.h"
#include "dds3_object_flags.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "eff_light.h"
#include "eff_event.h"




extern void dds3SetRadiusPoseSet(dds3PoseSet_t *node);

/* Create an inner-vector object and snapshot its vector state after initialization. */
dds3Basic_t *dds3AddEventObjectBasic(s32 initialValue, u128 *position, u128 *rotation) {
    dds3Basic_t *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_RESOURCE_OWNER);

    obj->key = initialValue;
    dds3SetPosPoseSet(obj, position);
    dds3SetRotPoseSet(obj, rotation);
    dds3SetRadiusPoseSet(obj->inner);
    return obj;
}

dds3ObjectBaseControlWorkData_t *dds3EventObjectBasicGetControlWorkData(const dds3Basic_t *object) {
    const DdsSlotResourceBlock *block = (const DdsSlotResourceBlock *)object->data;
    return block->resourceState;
}



/* Allocate the kind-9 light payload and its auxiliary buffer. */
u32 lightconstructor(dds3Basic_t *object) {
    EffLightData *obj;

    dds3InitPoseSet(object);
    obj = (EffLightData *)sdfChipAllocDirect(sizeof(EffLightData));
    object->data = obj;
    obj->resourceState = dds3CreateSlotResourceState(object);
    dds3ObjectBaseOnFlag(object, 0x62);
    obj->flags = 0;
    obj->sourceVectors.named.color[0] = 0.7f;
    obj->sourceVectors.named.color[1] = 0.7f;
    obj->sourceVectors.named.color[2] = 0.7f;
    obj->sourceVectors.named.direction[1] = 1.0f;
    obj->sourceVectors.named.direction[2] = 0.5f;
    obj->ambientColor[0] = 0.2f;
    obj->ambientColor[1] = 0.2f;
    obj->ambientColor[2] = 0.2f;
    obj->ambientColor[3] = 1.0f;
    obj->parameter50 = 100.0f;
    obj->parameter54 = 800.0f;
    obj->parameter58 = 7.0f;
    obj->sourceVectors.named.color[3] = 0.0f;
    obj->sourceVectors.named.direction[0] = 0.0f;
    obj->sourceVectors.named.direction[3] = 0.0f;
    obj->word5C = 0;
    obj->buffer = sdfChipAllocDirect(0xE0);
    return 1;
}
